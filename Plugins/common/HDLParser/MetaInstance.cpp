//-----------------------------------------------------------------------------
// File: MetaInstance.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Janne Virtanen
// Date: 11.01.2017
//
// Description:
// An instance of a component instance within a hierarchy with its parameters and interfaces parsed.
//-----------------------------------------------------------------------------

#include "MetaInstance.h"

#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>
#include <IPXACTmodels/AbstractionDefinition/PortAbstraction.h>
#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/PortMap.h>

#include <KactusAPI/MessageMediator.h>

#include <KactusAPI/LibraryInterface.h>


//-----------------------------------------------------------------------------
// Function: MetaInstance::MetaInstance()
//-----------------------------------------------------------------------------
MetaInstance::MetaInstance(QSharedPointer<ComponentInstance> componentInstance,
                           LibraryInterface* designInterface,
                           LibraryInterface* libraryInterface,
                           MessageMediator* messages,
                           QSharedPointer<Component> component,
                           QSharedPointer<View> activeView) :
    MetaComponent(messages, component, activeView),
    componentInstance_(componentInstance),
    designInterface_(designInterface),
    libraryInterface_(libraryInterface),
    interfaces_(new QMap<QString,QSharedPointer<MetaInterface> >)
{

}

//-----------------------------------------------------------------------------
// Function:  MetaInstance::parseInstance()
//-----------------------------------------------------------------------------
void MetaInstance::parseInstance()
{
    // Initialize the parameter parsing: Find parameters from both the instance and the top component.
    QSharedPointer<QList<QSharedPointer<Parameter> > > ilist(getParameters());
    QSharedPointer<ListParameterFinder> instanceFinder(new ListParameterFinder);
    instanceFinder->setParameterList(ilist);

    // Create parser using the applicable finders.
    IPXactSystemVerilogParser instanceParser(instanceFinder);

    // Parse the interfaces and ports.
    parseInterfaces();
    parsePorts(instanceParser);
    parsePortAssignments(instanceParser);

    parseMetaParameters(); 
}

//-----------------------------------------------------------------------------
// Function: MetaInstance::parseExpression()
//-----------------------------------------------------------------------------
QString MetaInstance::parseExpression(IPXactSystemVerilogParser const& parser,  QString const& expression)
{
    QString value = parser.parseExpression(expression);

    if (value == QStringLiteral("x"))
    {
        return QStringLiteral("0");
    }

    return value;
}

//-----------------------------------------------------------------------------
// Function: MetaInstance::parseInterfaces()
//-----------------------------------------------------------------------------
void MetaInstance::parseInterfaces()
{
    foreach(QSharedPointer<BusInterface> busInterface, *getComponent()->getBusInterfaces())
    {
        // Find the correct abstraction type.        
        QString viewName = QString();        
        if (getActiveView())
        {
            viewName = getActiveView()->name();
        }

        QSharedPointer<AbstractionType> absType = busInterface->getAbstractionContainingView(viewName);
        if (!absType)
        {
            messages_->showError(
                QObject::tr("Component %1: Bus interface %2 does not have an abstraction type.")
                .arg(getComponent()->getVlnv().toString(), busInterface->name()));
            continue;
        }

        // An abstraction definition is needed. It comes through VLNV reference.
        QSharedPointer<ConfigurableVLNVReference> absRef = absType->getAbstractionRef();
        if (!absRef)
        {
            messages_->showError(
                QObject::tr("Component %1: Abstraction type in bus interface %2 does not have abstraction reference.")
                .arg(getComponent()->getVlnv().toString(), busInterface->name()));
            continue;
        }

        // Find the abstraction definition from the library.
//        QSharedPointer<AbstractionDefinition> absDef = libraryInterface_->getModel<AbstractionDefinition>(*absRef);
        QSharedPointer<AbstractionDefinition> absDef;
        absDef = designInterface_->getModel<AbstractionDefinition>(*absRef);

        if (!absDef)
        {
            absDef = libraryInterface_->getModel<AbstractionDefinition>(*absRef);

            if (!absDef)
            {
                messages_->showError(
                    QObject::tr("Component %1: Abstraction definition for bus interface %2 was not found: %3")
                    .arg(getComponent()->getVlnv().toString(), busInterface->name(), absRef->toString()));
                continue;
            }
        }

        // Create "our" interface for each IP-XACT interface. Take the relevant values.
        QSharedPointer<MetaInterface> mInterface(new MetaInterface);
        mInterface->interface_ = busInterface;
        mInterface->absType_ = absType;
        mInterface->absDef_ = absDef;

        // Insert to the interface to the list.
        interfaces_->insert(busInterface->name(), mInterface);
    }
}

//-----------------------------------------------------------------------------
// Function: MetaInstance::parsePorts()
//-----------------------------------------------------------------------------
void MetaInstance::parsePorts(IPXactSystemVerilogParser const& parser)
{
    foreach (QSharedPointer<Port> cport, *getComponent()->getPorts())
    {        
        QSharedPointer<MetaPort> mPort(new MetaPort);
        mPort->port_ = cport;
        mPort->arrayBounds_.first = "";
        mPort->arrayBounds_.second = "";
        mPort->width_ = "";
        mPort->vectorBounds_.first = parseExpression(parser, cport->getLeftBound());
        mPort->vectorBounds_.second = parseExpression(parser, cport->getRightBound());
        mPort->defaultValue_ = parseExpression(parser, cport->getDefaultValue());

        mPort->isWire_ = false;
        mPort->isTransactional_ = false;

        if (cport->getWire())
        {
            mPort->isWire_ = true;
            mPort->arrayBounds_.first = parseExpression(parser, cport->getArrayLeft());
            mPort->arrayBounds_.second = parseExpression(parser, cport->getArrayRight());
        }
        else if (cport->getTransactional())
        {
            mPort->isTransactional_ = true;
            mPort->width_ = parseExpression(parser, cport->getTransactional()->getBusWidth());
        }

        getPorts()->insert(cport->name(), mPort);
    }
}

//-----------------------------------------------------------------------------
// Function: MetaInstance::parsePortAssignments()
//-----------------------------------------------------------------------------
void MetaInstance::parsePortAssignments(IPXactSystemVerilogParser const& parser)
{
    for (QSharedPointer<MetaInterface> mInterface: *interfaces_)
    {
        for (QSharedPointer<PortMap> pMap: *mInterface->absType_->getPortMaps())
        {
            if (!pMap->getPhysicalPort() || !pMap->getLogicalPort() ||
                !getPorts()->contains(pMap->getPhysicalPort()->name_))
            {
                continue;
            }

            // The physical port must match an existing component port.
            QSharedPointer<MetaPort> mPort = getPorts()->value(pMap->getPhysicalPort()->name_);

            // The abstraction definition must have a port abstraction with the same name.
            QSharedPointer<PortAbstraction> portAbstraction = mInterface->absDef_->getPort(pMap->getLogicalPort()->name_);

            if (!portAbstraction)
            {
                messages_->showError(QObject::tr("Component %1, Bus interface %2: Port abstraction"
                    " was not found for logical port %3 at abstraction definition %4.")
                    .arg(getComponent()->getVlnv().toString(),
                    mInterface->interface_->name(),
                    pMap->getLogicalPort()->name_,
                    mInterface->absDef_->getVlnv().toString()));
                continue;
            }

            // Parse the port map bounds.
            QPair<QString, QString> logicalBounds = logicalPortBoundsInMapping(parser, pMap);
            QPair<QString, QString> physicalBounds = physicalPortBoundsInMapping(parser, pMap);

            // If physical bounds do not exist, they are the same as the port bounds.
            if (physicalBounds.first.isEmpty() || physicalBounds.second.isEmpty())
            {
                physicalBounds = mPort->vectorBounds_;
            }

            // If logical bounds do not exist, they are the same as the physical bounds.
            if (logicalBounds.first.isEmpty() || logicalBounds.second.isEmpty())
            {
                // Pick the total width of the physical bounds.
                int left = parseExpression(parser, physicalBounds.first).toInt();
                int right = parseExpression(parser, physicalBounds.second).toInt();

                // This is [abs(physical.left physical.right):0]
                logicalBounds.first = QString::number(abs(left - right));
                logicalBounds.second = QStringLiteral("0");
            }

            // Every mapping using the port creates a new assignment for the port.
            QSharedPointer<MetaPortAssignment> mUpPortAssignment(new MetaPortAssignment);
            mUpPortAssignment->mappedInterface_ = mInterface;
            mUpPortAssignment->logicalBounds_ = logicalBounds;
            mUpPortAssignment->physicalBounds_ = physicalBounds;
            mUpPortAssignment->invert_ = pMap->getInvert().toBool();

            // The default value comes from the port abstraction.
            mUpPortAssignment->defaultValue_ = portAbstraction->getDefaultValue();

            // Create a copy of the assignments.
            QSharedPointer<MetaPortAssignment> mDownPortAssignment(new MetaPortAssignment(*mUpPortAssignment));

            // There must be an assignment for both directions in hierarchy.
            mPort->upAssignments_.insert(pMap->getLogicalPort()->name_, mUpPortAssignment);
            mPort->downAssignments_.insert(pMap->getLogicalPort()->name_, mDownPortAssignment);

            // Associate the meta port with the interface.
            mInterface->ports_.insert(mPort->port_->name(), mPort);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: MetaInstance::logicalPortBoundsInMapping()
//-----------------------------------------------------------------------------
QPair<QString, QString> MetaInstance::logicalPortBoundsInMapping(IPXactSystemVerilogParser const& parser,
    QSharedPointer<PortMap> portMap) const
{
    QPair<QString, QString> bounds;

    QSharedPointer<PortMap::LogicalPort> logicalPort = portMap->getLogicalPort();

    if (logicalPort && logicalPort->range_)
    {
        // Pick the range expressions as the logical bounds.
        bounds.first = parseExpression(parser, logicalPort->range_->getLeft());
        bounds.second = parseExpression(parser, logicalPort->range_->getRight());
    }

    return bounds;
}

//-----------------------------------------------------------------------------
// Function: MetaInstance::physicalPortBoundsInMapping()
//-----------------------------------------------------------------------------
QPair<QString, QString> MetaInstance::physicalPortBoundsInMapping(IPXactSystemVerilogParser const& parser,
    QSharedPointer<PortMap> portMap) const
{
    QPair<QString, QString> bounds;

    QSharedPointer<PortMap::PhysicalPort> physPort = portMap->getPhysicalPort();

    if (physPort && physPort->partSelect_)
    {
        // Pick the part select expressions as the total width of the physical bounds.
        bounds.first = parseExpression(parser, physPort->partSelect_->getLeftRange());
        bounds.second = parseExpression(parser, physPort->partSelect_->getRightRange());
    }

    return bounds;
}
