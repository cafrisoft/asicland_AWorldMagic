//-----------------------------------------------------------------------------
// File: SystemComponentDeleteCommand.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 09.02.2016
//
// Description:
// Delete command for component instances in system designs.
//-----------------------------------------------------------------------------

#include "SystemComponentDeleteCommand.h"

#include <IPXACTmodels/Design/Design.h>
#include <IPXACTmodels/Design/ComponentInstance.h>

#include <UI/Common/graphicsItems/ComponentItem.h>
#include <UI/Common/graphicsItems/GraphicsColumnLayout.h>
#include <UI/Common/graphicsItems/GraphicsConnection.h>

#include <UI/SystemDesign/SWPortItem.h>
#include <UI/SystemDesign/ApiGraphicsConnection.h>
#include <UI/SystemDesign/ComGraphicsConnection.h>

#include <UI/SystemDesign/UndoCommands/SystemDeleteCommands.h>
#include <UI/SystemDesign/UndoCommands/ApiConnectionDeleteCommand.h>
#include <UI/SystemDesign/UndoCommands/ComConnectionDeleteCommand.h>


//-----------------------------------------------------------------------------
// Function: SystemComponentDeleteCommand::SystemComponentDeleteCommand()
//-----------------------------------------------------------------------------
SystemComponentDeleteCommand::SystemComponentDeleteCommand(ComponentItem* item,
    QSharedPointer<Design> containingDesign, QUndoCommand* parent /* = 0 */):
QObject(),
QUndoCommand(parent),
item_(item),
stack_(dynamic_cast<IGraphicsItemStack*>(item->parentItem())),
scene_(item->scene()),
del_(true),
firstRun_(true),
containingDesign_(containingDesign)
{

}

//-----------------------------------------------------------------------------
// Function: SystemComponentDeleteCommand::~SystemComponentDeleteCommand()
//-----------------------------------------------------------------------------
SystemComponentDeleteCommand::~SystemComponentDeleteCommand()
{
    if (del_)
    {
        delete item_;
    }
}

//-----------------------------------------------------------------------------
// Function: SystemComponentDeleteCommand::undo()
//-----------------------------------------------------------------------------
void SystemComponentDeleteCommand::undo()
{
    // Add the item back to the scene.
    stack_->addItem(item_);
    del_ = false;

    containingDesign_->getComponentInstances()->append(item_->getComponentInstance());

    emit componentInstantiated(item_);

    // Execute child commands.
    QUndoCommand::undo();
}

//-----------------------------------------------------------------------------
// Function: SystemComponentDeleteCommand::redo()
//-----------------------------------------------------------------------------
void SystemComponentDeleteCommand::redo()
{
    if (firstRun_)
    {
        // Create child commands for removing connections.
        QList<GraphicsConnection*> connections;

        foreach (QGraphicsItem* childItem, item_->childItems())
        {
            if (childItem->type() != SWPortItem::Type)
            {
                continue;
            }

            SWPortItem* endpoint = static_cast<SWPortItem*>(childItem);

            foreach (GraphicsConnection* conn, endpoint->getConnections())
            {
                if (!connections.contains(conn))
                {
                    addConnectionDeleteCommand(conn);
                    connections.append(conn);
                }
            }

            if (endpoint->getOffPageConnector() != 0)
            {
                foreach (GraphicsConnection* conn, endpoint->getOffPageConnector()->getConnections())
                {
                    if (!connections.contains(conn))
                    {
                        addConnectionDeleteCommand(conn);
                        connections.append(conn);
                    }
                }
            }
        }

        firstRun_ = false;
    }

    // Execute child commands.
    QUndoCommand::redo();

    // Remove the item from the scene.
    stack_->removeItem(item_);
    scene_->removeItem(item_);
    del_ = true;

    containingDesign_->getComponentInstances()->removeAll(item_->getComponentInstance());


    emit componentInstanceRemoved(item_);
}

//-----------------------------------------------------------------------------
// Function: SystemComponentDeleteCommand::addConnectionDeleteCommand()
//-----------------------------------------------------------------------------
void SystemComponentDeleteCommand::addConnectionDeleteCommand(GraphicsConnection* connection)
{
    ApiGraphicsConnection* apiConnection = dynamic_cast<ApiGraphicsConnection*>(connection);
    if (apiConnection)
    {
        new ApiConnectionDeleteCommand(apiConnection, containingDesign_, this);
    }
    else
    {
        ComGraphicsConnection* comConnection = dynamic_cast<ComGraphicsConnection*>(connection);
        if (comConnection)
        {
            new ComConnectionDeleteCommand(comConnection, containingDesign_, this);
        }
    }
}
