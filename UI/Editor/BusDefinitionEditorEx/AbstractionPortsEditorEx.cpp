//-----------------------------------------------------------------------------
// File: AbstractionPortsEditorEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Editor for the ports of an abstraction definition.
//-----------------------------------------------------------------------------

#include <QHeaderView>
#include <QVBoxLayout>

#include <IPXACTmodels/BusDefinition/BusDefinition.h>
#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>

#include "AbstractionPortsModelEx.h"
#include "AbstractionWirePortsSortFilterEx.h"
#include "AbstractionWirePortsDelegateEx.h"
#include "LogicalPortColumns.h"

#include "AbstractionPortsEditorEx.h"


//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::AbstractionPortsEditorEx()
//-----------------------------------------------------------------------------
AbstractionPortsEditorEx::AbstractionPortsEditorEx(LibraryInterface* libraryAccess,
                                                   PortAbstractionInterface* portInterface,
                                                   AbstractionPortsModelEx* portModel,
                                                   LogicalPortColumns::AbstractionType type,
                                                   QWidget* parent):
    QWidget(parent),
    portView_(this),
    portProxy_(nullptr),
    portModel_(portModel),
    portDelegate_(nullptr)
{
    if (type == LogicalPortColumns::AbstractionType::WIRE)
    {
        portProxy_ = new AbstractionWirePortsSortFilterEx(portInterface, this);
        portDelegate_ = new AbstractionWirePortsDelegateEx(libraryAccess, this);
    }
    else
    {
//        portProxy_ = new AbstractionTransactionalPortsSortFilter(portInterface, this);
//        portDelegate_ = new AbstractionTransactionalPortsDelegate(libraryAccess, this);
    }

    portProxy_->setSourceModel(portModel_);
    
    portView_.setModel(portProxy_);
    portView_.setSortingEnabled(true);
    portView_.setItemDelegate(portDelegate_);
    portView_.setAllowImportExport(true);
    portView_.setItemsDraggable(false);
    portView_.setAlternatingRowColors(false);
    portView_.sortByColumn(0, Qt::AscendingOrder);

    if (type == LogicalPortColumns::AbstractionType::WIRE)
    {
        hideTransactionalColumns();
    }
    else
    {
        hideWireColumns();
    }


    connect(&portView_, SIGNAL(addMaster()), this, SLOT(onAddMaster()), Qt::UniqueConnection);
    connect(&portView_, SIGNAL(addSlave()), this, SLOT(onAddSlave()), Qt::UniqueConnection);
    connect(&portView_, SIGNAL(addSystem()), this, SLOT(onAddSystem()), Qt::UniqueConnection);
    connect(&portView_, SIGNAL(addAllSystems()), this, SLOT(onAddAllSystems()), Qt::UniqueConnection);
    connect(&portView_, SIGNAL(resetExtendPorts()), portModel_, SLOT(onResetExtendPorts()), Qt::UniqueConnection);

    connect(portModel_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(portModel_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(portModel_, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(portModel_, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(portModel_, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)),
        this, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)), Qt::UniqueConnection);

    if (type == LogicalPortColumns::AbstractionType::WIRE)
    {
        connect(&portView_, SIGNAL(addItem(const QModelIndex&)),
            portModel_, SLOT(addWireSignal()), Qt::UniqueConnection);
    }
    else
    {
        connect(&portView_, SIGNAL(addItem(const QModelIndex&)),
            portModel_, SLOT(addTransactionalSignal()), Qt::UniqueConnection);
    }

    connect(&portView_, SIGNAL(removeItem(const QModelIndex&)),
        portModel_, SLOT(onRemoveItem(const QModelIndex&)), Qt::UniqueConnection);

    setupLayout();
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::onAddMaster()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::onAddMaster()
{
    QModelIndexList selection = getSelectedIndexes();
    portModel_->addMaster(selection);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::onAddSlave()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::onAddSlave()
{
    QModelIndexList selection = getSelectedIndexes();
    portModel_->addSlave(selection);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::onAddSystem()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::onAddSystem()
{
    QModelIndexList selection = getSelectedIndexes();
    portModel_->addSystem(selection);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::onAddAllSystems()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::onAddAllSystems()
{
    QModelIndexList selection = getSelectedIndexes();
    portModel_->addAllSystems(selection);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::getSelectedIndexes()
//-----------------------------------------------------------------------------
QModelIndexList AbstractionPortsEditorEx::getSelectedIndexes()
{
    QModelIndexList selection;
    foreach(QModelIndex index, portView_.selected())
    {
        selection.append(portProxy_->mapToSource(index));
    }

    return selection;
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::resetPortModel()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::resetPortModel()
{
    portModel_->resetPortModel();
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::setBusDef()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::setBusDef(QSharedPointer<BusDefinition> busDefinition)
{
    portDelegate_->setBusDef(busDefinition);
    portModel_->setBusDef(busDefinition);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::setupLayout()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::setupLayout()
{
    QVBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->addWidget(&portView_);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::hideTransactionalColumns()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::hideTransactionalColumns()
{
    auto header = portView_.horizontalHeader();
  
    header->hideSection(LogicalPortColumns::INITIATIVE);
    header->hideSection(LogicalPortColumns::KIND);
    header->hideSection(LogicalPortColumns::BUSWIDTH);
    header->hideSection(LogicalPortColumns::PROTOCOLTYPE);
    header->hideSection(LogicalPortColumns::PAYLOADNAME);
    header->hideSection(LogicalPortColumns::PAYLOADTYPE);
    header->hideSection(LogicalPortColumns::PAYLOADEXTENSION);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsEditorEx::hideWireColumns()
//-----------------------------------------------------------------------------
void AbstractionPortsEditorEx::hideWireColumns()
{
    auto header = portView_.horizontalHeader();
    
    header->hideSection(LogicalPortColumns::DIRECTION);
    header->hideSection(LogicalPortColumns::WIDTH);
    header->hideSection(LogicalPortColumns::DEFAULT_VALUE);
    header->hideSection(LogicalPortColumns::DRIVER);
}
