//-----------------------------------------------------------------------------
// File: AbstractionPortsViewEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// The view that displays the contents of the BusPortsModel.
//-----------------------------------------------------------------------------

#include <QMenu>

#include "AbstractionPortsModelEx.h"

#include "AbstractionPortsViewEx.h"


//-----------------------------------------------------------------------------
// Function: AbstractionPortsViewEx::AbstractionPortsViewEx()
//-----------------------------------------------------------------------------
AbstractionPortsViewEx::AbstractionPortsViewEx(QWidget *parent):
    EditableTableView(parent),
    addMasterAction_(tr("Add master signal"), this),
    addSlaveAction_(tr("Add slave signal"), this),
    addSystemAction_(tr("Add system signal"), this),
    addAllSystemsAction_(tr("Add all unconnected system signals"), this),
    resetExtendPortsAction_(tr("Reset extend ports"), this)
{
    connect(&addMasterAction_, SIGNAL(triggered()), this, SIGNAL(addMaster()), Qt::UniqueConnection);
    connect(&addSlaveAction_, SIGNAL(triggered()), this, SIGNAL(addSlave()), Qt::UniqueConnection);
    connect(&addSystemAction_, SIGNAL(triggered()), this, SIGNAL(addSystem()), Qt::UniqueConnection);
    connect(&addAllSystemsAction_, SIGNAL(triggered()), this, SIGNAL(addAllSystems()), Qt::UniqueConnection);
    connect(&resetExtendPortsAction_, SIGNAL(triggered()), this, SIGNAL(resetExtendPorts()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsViewEx::~AbstractionPortsViewEx()
//-----------------------------------------------------------------------------
AbstractionPortsViewEx::~AbstractionPortsViewEx()
{

}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsViewEx::selected()
//-----------------------------------------------------------------------------
QModelIndexList AbstractionPortsViewEx::selected() const
{
    return selectedIndexes();
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsViewEx::contextMenuEvent()
//-----------------------------------------------------------------------------
void AbstractionPortsViewEx::contextMenuEvent(QContextMenuEvent* event)
{
    pressedPoint_ = event->pos();

    QModelIndex index = indexAt(pressedPoint_);

    if (index.data(AbstractionPortsModelEx::isExtendLockedRole).toBool())
    {
        removeAction_.setDisabled(true);
    }
    else
    {
        removeAction_.setEnabled(true);
    }

    if (!(index.flags() & Qt::ItemIsEditable))
    {
        clearAction_.setDisabled(true);
    }
    else
    {
        clearAction_.setEnabled(true);
    }

    QMenu menu(this);
    if (index.isValid())
    {
        menu.addAction(&addMasterAction_);
        menu.addAction(&addSlaveAction_);
        menu.addAction(&addSystemAction_);
        menu.addAction(&addAllSystemsAction_);
        menu.addSeparator();
    }

    menu.addAction(&resetExtendPortsAction_);
    menu.addSeparator();

    menu.addAction(&addAction_);
    
    // if at least one valid item is selected
    if (index.isValid())
    {
        menu.addAction(&removeAction_);
        menu.addAction(&clearAction_);
        menu.addAction(&copyAction_);    
    }
    menu.addAction(&pasteAction_);

    if (importExportAllowed())
    {
        menu.addSeparator();
        menu.addAction(&importAction_);
        menu.addAction(&exportAction_);
    }

    menu.exec(event->globalPos());

    event->accept();
}

