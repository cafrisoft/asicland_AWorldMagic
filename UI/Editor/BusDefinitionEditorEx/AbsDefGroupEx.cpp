//-----------------------------------------------------------------------------
// File: AbsDefGroupEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Editor for the logical ports of an abstraction definition.
//-----------------------------------------------------------------------------

#include <QSortFilterProxyModel>
#include <QVBoxLayout>

#include <IPXACTmodels/BusDefinition/BusDefinition.h>
#include <IPXACTmodels/AbstractionDefinition/PortAbstraction.h>

#include <IPXACTmodels/utilities/BusDefinitionUtils.h>

#include <KactusAPI/PortAbstractionInterface.h>

#include <KactusAPI/LibraryInterface.h>

#include "AbstractionDefinitionPortsSortFilterEx.h"
#include "AbstractionPortsModelEx.h"
#include "LogicalPortColumns.h"

#include "AbsDefGroupEx.h"


//-----------------------------------------------------------------------------
// Function: AbsDefGroupEx::AbsDefGroupEx()
//-----------------------------------------------------------------------------
AbsDefGroupEx::AbsDefGroupEx(LibraryInterface* libraryHandler,
                             PortAbstractionInterface* portInterface,
                             PortAbstractionInterface* extendInterface,
                             QWidget *parent):
    QWidget(parent),
    portTabs_(this),
    portInterface_(portInterface),
    extendInterface_(extendInterface),
    portModel_(new AbstractionPortsModelEx(libraryHandler, portInterface, extendInterface, this)),
    wirePortsEditor_(new AbstractionPortsEditorEx(libraryHandler, portInterface, portModel_, LogicalPortColumns::AbstractionType::WIRE, &portTabs_)),
    libraryHandler_(libraryHandler)
{
    portTabs_.addTab(wirePortsEditor_, QStringLiteral("Wire ports"));

    connect(wirePortsEditor_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(wirePortsEditor_, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(wirePortsEditor_, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(wirePortsEditor_, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)),
        this, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)), Qt::UniqueConnection);

	setupLayout();
}

//-----------------------------------------------------------------------------
// Function: AbsDefGroupEx::save()
//-----------------------------------------------------------------------------
void AbsDefGroupEx::save()
{
    portInterface_->save();
}

//-----------------------------------------------------------------------------
// Function: AbsDefGroupEx::setAbsDef()
//-----------------------------------------------------------------------------
void AbsDefGroupEx::setAbsDef(QSharedPointer<AbstractionDefinition> absDef)
{
    abstraction_ = absDef;

    portInterface_->setAbsDef(abstraction_);

    wirePortsEditor_->resetPortModel();

    auto busDefinition = libraryHandler_->getModel<BusDefinition>(absDef->getBusType());

    wirePortsEditor_->setBusDef(busDefinition);

    portTabs_.setCurrentWidget(wirePortsEditor_);

}

//-----------------------------------------------------------------------------
// Function: AbsDefGroupEx::setBusDef()
//-----------------------------------------------------------------------------
void AbsDefGroupEx::setBusDef(QSharedPointer<BusDefinition> busDefinition)
{
    wirePortsEditor_->setBusDef(busDefinition);
}

//-----------------------------------------------------------------------------
// Function: AbsDefGroupEx::setupLayout()
//-----------------------------------------------------------------------------
void AbsDefGroupEx::setupLayout()
{
    QGridLayout* topLayout = new QGridLayout(this);
    topLayout->addWidget(&portTabs_, 0, 0, 1, 1);

    topLayout->setColumnStretch(0, 1);

    topLayout->setRowStretch(0, 1);
}
