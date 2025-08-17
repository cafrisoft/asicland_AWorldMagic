//-----------------------------------------------------------------------------
// File: absdefgroup.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 15.8.2011
//
// Description:
// Editor for the logical ports of an abstraction definition.
//-----------------------------------------------------------------------------

#include "absdefgroup.h"

#include <IPXACTmodels/BusDefinition/BusDefinition.h>
#include <IPXACTmodels/AbstractionDefinition/PortAbstraction.h>

#include <IPXACTmodels/utilities/BusDefinitionUtils.h>
#include <KactusAPI/PortAbstractionInterface.h>

#include <UI/Widgets/vlnvDisplayer/vlnvdisplayer.h>
#include <UI/Widgets/vlnvEditor/vlnveditor.h>

#include "AbstractionDefinitionPortsSortFilter.h"
#include "AbstractionPortsModel.h"
#include "LogicalPortColumns.h"

#include <KactusAPI/LibraryInterface.h>

#include <QSortFilterProxyModel>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: AbsDefGroup::AbsDefGroup()
//-----------------------------------------------------------------------------
AbsDefGroup::AbsDefGroup(LibraryInterface* libraryHandler, PortAbstractionInterface* portInterface,
    PortAbstractionInterface* extendInterface,
    QWidget *parent):
QWidget(parent),
vlnvDisplay_(new VLNVDisplayer(this, VLNV())),
extendEditor_(new VLNVEditor(VLNV::ABSTRACTIONDEFINITION, libraryHandler, this, this)),
busDisplay_(new VLNVDisplayer(this, VLNV())),
descriptionEditor_(new QPlainTextEdit(this)),
portTabs_(this),
portInterface_(portInterface),
extendInterface_(extendInterface),
portModel_(new AbstractionPortsModel(libraryHandler, portInterface, extendInterface, this)),
wirePortsEditor_(new AbstractionPortsEditor(libraryHandler, portInterface, portModel_, LogicalPortColumns::AbstractionType::WIRE, &portTabs_)),
transactionalPortsEditor_(new AbstractionPortsEditor(libraryHandler, portInterface, portModel_, LogicalPortColumns::AbstractionType::TRANSACTIONAL, &portTabs_)),
libraryHandler_(libraryHandler)
{
    vlnvDisplay_->setTitle(QStringLiteral("Abstraction definition"));
    extendEditor_->setTitle(tr("Extended abstraction definition"));
    extendEditor_->setMandatory(false);

    busDisplay_->setTitle(QStringLiteral("Bus Type"));

    portTabs_.addTab(wirePortsEditor_, QStringLiteral("Wire ports"));
    portTabs_.addTab(transactionalPortsEditor_, QStringLiteral("Transactional ports"));

    connect(wirePortsEditor_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(wirePortsEditor_, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(wirePortsEditor_, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(wirePortsEditor_, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)),
        this, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)), Qt::UniqueConnection);

    connect(transactionalPortsEditor_, SIGNAL(contentChanged()),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(transactionalPortsEditor_, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(transactionalPortsEditor_, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(transactionalPortsEditor_, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)),
        this, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)), Qt::UniqueConnection);

    connect(descriptionEditor_, SIGNAL(textChanged()), this, SLOT(onDescriptionChanged()), Qt::UniqueConnection);

    connect(extendEditor_, SIGNAL(vlnvEdited()), this, SLOT(onExtendChanged()), Qt::UniqueConnection);

	setupLayout();
}

//-----------------------------------------------------------------------------
// Function: AbsDefGroup::save()
//-----------------------------------------------------------------------------
void AbsDefGroup::save()
{
    portInterface_->save();
}

//-----------------------------------------------------------------------------
// Function: AbsDefGroup::setAbsDef()
//-----------------------------------------------------------------------------
void AbsDefGroup::setAbsDef(QSharedPointer<AbstractionDefinition> absDef)
{
    abstraction_ = absDef;

    portInterface_->setAbsDef(abstraction_);

    wirePortsEditor_->resetPortModel();
    transactionalPortsEditor_->resetPortModel();

    auto busDefinition = libraryHandler_->getModel<BusDefinition>(absDef->getBusType());

    wirePortsEditor_->setBusDef(busDefinition);
    transactionalPortsEditor_->setBusDef(busDefinition);

    vlnvDisplay_->setVLNV(absDef->getVlnv());

    extendEditor_->setVLNV(absDef->getExtends());

    busDisplay_->setVLNV(absDef->getBusType());

    if (abstractionContainsTransactionalPorts())
    {
        portTabs_.setCurrentWidget(transactionalPortsEditor_);
    }

    if (absDef->getExtends().isValid())
    {
        setupExtendedAbstraction();
    }

    if (!absDef->getDescription().isEmpty())
    {
        descriptionEditor_->setPlainText(absDef->getDescription());
    }
}

//-----------------------------------------------------------------------------
// Function: absdefgroup::abstractionContainsTransactionalPorts()
//-----------------------------------------------------------------------------
bool AbsDefGroup::abstractionContainsTransactionalPorts() const
{
#if TO_DO_ERROR_20240104 == 0
    return std::any_of(abstraction_->getLogicalPorts()->cbegin(), abstraction_->getLogicalPorts()->cend(),
        [](auto const& logicalPort) {return logicalPort->hasTransactional();});
#else
    return true;     //. error
#endif

}

//-----------------------------------------------------------------------------
// Function: absdefgroup::onDescriptionChanged()
//-----------------------------------------------------------------------------
void AbsDefGroup::onDescriptionChanged()
{
    abstraction_->setDescription(descriptionEditor_->toPlainText());
    emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: absdefgroup::onExtendChanged()
//-----------------------------------------------------------------------------
void AbsDefGroup::onExtendChanged()
{
    removeSignalsFromExtendedDefinition();

    abstraction_->setExtends(extendEditor_->getVLNV());
    setupExtendedAbstraction();

    emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: absdefgroup::setupExtendedAbstraction()
//-----------------------------------------------------------------------------
void AbsDefGroup::setupExtendedAbstraction()
{
    QSharedPointer<const AbstractionDefinition> extendedAbstraction = getExtendedAbstraction();
    extendInterface_->setAbsDef(extendedAbstraction);
    QString extendDescription = "";
    if (extendedAbstraction)
    {
        extendDescription = extendedAbstraction->getDescription();

        if (extendedAbstraction->getLogicalPorts() && !extendedAbstraction->getLogicalPorts()->isEmpty())
        {
            extendSignals(extendedAbstraction);
        }
    }

    descriptionEditor_->setPlaceholderText(extendDescription);
}

//-----------------------------------------------------------------------------
// Function: absdefgroup::getExtendedAbstraction()
//-----------------------------------------------------------------------------
QSharedPointer<const AbstractionDefinition> AbsDefGroup::getExtendedAbstraction() const
{
    VLNV extendedVLNV = abstraction_->getExtends();
    if (extendedVLNV.isValid() && extendedVLNV.getType() == VLNV::ABSTRACTIONDEFINITION)
    {
        QSharedPointer<const Document> extendedDocument = libraryHandler_->getModelReadOnly(extendedVLNV);
        if (extendedDocument)
        {
            QSharedPointer<const AbstractionDefinition> extendAbstraction =
                extendedDocument.dynamicCast<const AbstractionDefinition>();
            if (extendAbstraction)
            {
                return extendAbstraction;
            }
        }
    }

    return QSharedPointer<const AbstractionDefinition>();
}

//-----------------------------------------------------------------------------
// Function: absdefgroup::extendSignals()
//-----------------------------------------------------------------------------
void AbsDefGroup::extendSignals(QSharedPointer<const AbstractionDefinition> /*extendAbstraction*/)
{
    portModel_->setExtendedPorts();
}

//-----------------------------------------------------------------------------
// Function: absdefgroup::removeSignalsFromExtendedDefinition()
//-----------------------------------------------------------------------------
void AbsDefGroup::removeSignalsFromExtendedDefinition()
{
    if (abstraction_->getExtends() != extendEditor_->getVLNV() || extendEditor_->getVLNV().isEmpty())
    {
        QSharedPointer<const AbstractionDefinition> extendedAbstraction = getExtendedAbstraction();
        if (extendedAbstraction || extendEditor_->getVLNV().isEmpty())
        {
            portModel_->removeExtendedPorts();
        }
    }
}

//-----------------------------------------------------------------------------
// Function: AbsDefGroup::setupLayout()
//-----------------------------------------------------------------------------
void AbsDefGroup::setupLayout()
{
    QGroupBox* descriptionGroup = new QGroupBox(tr("Description"), this);

    QVBoxLayout* descriptionLayout = new QVBoxLayout(descriptionGroup);
    descriptionLayout->addWidget(descriptionEditor_);

    QGridLayout* topLayout = new QGridLayout(this);
    topLayout->addWidget(vlnvDisplay_, 0, 0, 1, 1);
    topLayout->addWidget(extendEditor_, 0, 1, 1, 1);
    topLayout->addWidget(descriptionGroup, 1, 0, 1, 1);
    topLayout->addWidget(busDisplay_, 1, 1, 1, 1);
    topLayout->addWidget(&portTabs_, 2, 0, 1, 2);

    topLayout->setColumnStretch(0, 1);
    topLayout->setColumnStretch(1, 1);

    topLayout->setRowStretch(0, 1);
    topLayout->setRowStretch(1, 1);
    topLayout->setRowStretch(2, 4);
}
