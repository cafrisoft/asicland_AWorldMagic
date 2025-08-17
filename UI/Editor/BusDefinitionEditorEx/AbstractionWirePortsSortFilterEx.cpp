//-----------------------------------------------------------------------------
// File: AbstractionWirePortsSortFilterEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Sort filter proxy model for abstraction definition wire ports.
//-----------------------------------------------------------------------------

#include <KactusAPI/PortAbstractionInterface.h>

#include "AbstractionWirePortsSortFilterEx.h"
#include "AbstractionPortsModelEx.h"

#include "LogicalPortColumns.h"


//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsSortFilterEx::AbstractionWirePortsSortFilterEx()
//-----------------------------------------------------------------------------
AbstractionWirePortsSortFilterEx::AbstractionWirePortsSortFilterEx(PortAbstractionInterface* portInterface,
                                                                   QObject *parent):
    AbstractionDefinitionPortsSortFilterEx(portInterface, parent)
{

}

//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsSortFilterEx::filterAcceptsRow()
//-----------------------------------------------------------------------------
bool AbstractionWirePortsSortFilterEx::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (source_parent.isValid())
    {
        return false;
    }

    std::string portName = portInterface_->getIndexedItemName(source_row);
    return portInterface_->portIsWire(portName);
}
