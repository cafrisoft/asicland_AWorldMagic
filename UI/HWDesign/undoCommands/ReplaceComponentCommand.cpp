//-----------------------------------------------------------------------------
// File: ReplaceComponentCommand.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 04.04.2016
//
// Description:
// Undo command for replacing a HW component instance with another one.
//-----------------------------------------------------------------------------

#include "ReplaceComponentCommand.h"

#include <UI/Common/graphicsItems/ComponentItem.h>
#include <UI/Common/graphicsItems/ConnectionUndoCommands.h>
#include <UI/Common/graphicsItems/CommonGraphicsUndoCommands.h>
#include <UI/Common/graphicsItems/GraphicsColumnLayout.h>
#include <UI/Common/graphicsItems/GraphicsColumn.h>
#include <UI/Common/graphicsItems/GraphicsConnection.h>

#include <UI/Common/DesignDiagram.h>
#include <UI/Common/Association/AssociationChangeEndpointCommand.h>

#include <UI/HWDesign/HWMoveCommands.h>
#include <UI/HWDesign/AdHocConnectionItem.h>
#include <UI/HWDesign/HWConnection.h>
#include <UI/HWDesign/ActiveBusInterfaceItem.h>
#include <UI/HWDesign/ActivePortItem.h>
#include <UI/HWDesign/HierarchicalPortItem.h>
#include <UI/HWDesign/HWComponentItem.h>
#include <UI/HWDesign/HWChangeCommands.h>

#include <UI/HWDesign/undoCommands/HWComponentAddCommand.h>
#include <UI/HWDesign/undoCommands/ConnectionDeleteCommand.h>
#include <UI/HWDesign/undoCommands/ComponentDeleteCommand.h>
#include <UI/HWDesign/undoCommands/AdHocConnectionDeleteCommand.h>

//-----------------------------------------------------------------------------
// Function: ReplaceComponentCommand::ReplaceComponentCommand()
//-----------------------------------------------------------------------------
ReplaceComponentCommand::ReplaceComponentCommand(HWComponentItem* oldComp, HWComponentItem* newComp,
    DesignDiagram* diagram, QUndoCommand* parent):
QUndoCommand(parent),
position_(oldComp->scenePos()),
newComp_(newComp),
diagram_(diagram)
{
    new ItemMoveCommand(newComp_, newComp_->scenePos(), newComp_->getParentStack(), position_,
        oldComp->getParentStack(), diagram, this);

    new ItemMoveCommand(oldComp, position_, oldComp->getParentStack(), newComp_->scenePos(),
        newComp_->getParentStack(), diagram, this);

    QStringList connectionNames;
    changeConnections(oldComp, newComp_, connectionNames);
    changeConnections(newComp_, oldComp, connectionNames);

    foreach(Association* association, oldComp->getAssociations())
    {
        new AssociationChangeEndpointCommand(association, oldComp, newComp_, this);
    }

    QSharedPointer<Design> design = diagram->getDesign();
    new ComponentChangeNameCommand(oldComp, newComp_->name(), design, this);
    new ComponentChangeNameCommand(newComp_, oldComp->name(), design, this);
}

//-----------------------------------------------------------------------------
// Function: ReplaceComponentCommand::changeConnections()
//-----------------------------------------------------------------------------
void ReplaceComponentCommand::changeConnections(HWComponentItem* oldComponentItem, 
    HWComponentItem* newComponentItem, QStringList& connectionNames)
{
    foreach (ConnectionEndpoint* oldEndpoint, oldComponentItem->getEndpoints())
    {
        if (!oldEndpoint->getConnections().isEmpty() || 
            (oldEndpoint->getOffPageConnector() && !oldEndpoint->getOffPageConnector()->getConnections().isEmpty()))
        {
            HWConnectionEndpoint* newEndpoint = 0;

            if (oldEndpoint->type() == ActiveBusInterfaceItem::Type)
            {
                newEndpoint = newComponentItem->getBusPort(oldEndpoint->name());
            }
            else if (oldEndpoint->type() == ActivePortItem::Type)
            {
                newEndpoint = newComponentItem->getAdHocPort(oldEndpoint->name());
            }

            if (newEndpoint != 0)
            {
                createConnectionExchangeCommands(oldEndpoint, newEndpoint, connectionNames);
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ReplaceComponentCommand::createConnectionExchangeCommands()
//-----------------------------------------------------------------------------
void ReplaceComponentCommand::createConnectionExchangeCommands(ConnectionEndpoint* oldEndpoint, 
    HWConnectionEndpoint* newEndpoint, QStringList& connectionNames)
{
    // Create a move command to move the port to the same place where it is in the old component.
    new PortMoveCommand(newEndpoint, newEndpoint->pos(), oldEndpoint->pos(), diagram_, this);

    // Exchange connections between the endpoints.
    foreach (GraphicsConnection* connection, oldEndpoint->getConnections())
    {
        if (!connectionNames.contains(connection->name()))
        {
            new ConnectionExchangeCommand(connection, oldEndpoint, newEndpoint, this);
            connectionNames.append(connection->name());
        }
    }

    if (oldEndpoint->getOffPageConnector() &&
        !oldEndpoint->getOffPageConnector()->getConnections().isEmpty())
    {
        foreach (GraphicsConnection* connection, oldEndpoint->getOffPageConnector()->getConnections())
        {
            if (!connectionNames.contains(connection->name()))
            {
                new ConnectionExchangeCommand(connection, oldEndpoint->getOffPageConnector(),
                    newEndpoint->getOffPageConnector(), this);
                connectionNames.append(connection->name());
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ReplaceComponentCommand::~ReplaceComponentCommand()
//-----------------------------------------------------------------------------
ReplaceComponentCommand::~ReplaceComponentCommand()
{
}

//-----------------------------------------------------------------------------
// Function: ReplaceComponentCommand::undo()
//-----------------------------------------------------------------------------
void ReplaceComponentCommand::undo()
{
    // Execute child commands.
    QUndoCommand::undo();
}

//-----------------------------------------------------------------------------
// Function: ReplaceComponentCommand::redo()
//-----------------------------------------------------------------------------
void ReplaceComponentCommand::redo()
{
    // Place the new component to the exact same location as the old one.
    newComp_->setParentItem(0);
    newComp_->setPos(position_);

    // Execute child commands.
    QUndoCommand::redo();
}
