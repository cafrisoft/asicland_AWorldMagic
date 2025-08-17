//-----------------------------------------------------------------------------
// File: AdHocConnectionAddCommand.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Esko Pekkarinen
// Date: 02.10.2015
//
// Description:
// <Short description of the class/file contents>
//-----------------------------------------------------------------------------

#include "AdHocConnectionAddCommand.h"

#include <UI/Common/DesignDiagram.h>

#include <UI/HWDesign/AdHocConnectionItem.h>

#include <IPXACTmodels/Design/Design.h>

//-----------------------------------------------------------------------------
// Function: AhHocConnectionAddCommand::AhHocConnectionAddCommand()
//-----------------------------------------------------------------------------
AdHocConnectionAddCommand::AdHocConnectionAddCommand(DesignDiagram* scene,
    AdHocConnectionItem* connection,
    QSharedPointer<Design> design,
    QUndoCommand* parent) : QUndoCommand(parent),
    connection_(connection),
    scene_(scene),
    design_(design),   
    del_(false)
{

}

//-----------------------------------------------------------------------------
// Function: AhHocConnectionAddCommand::~AhHocConnectionAddCommand()
//-----------------------------------------------------------------------------
AdHocConnectionAddCommand::~AdHocConnectionAddCommand()
{
    if (del_)
    {
        delete connection_;
    }
}

//-----------------------------------------------------------------------------
// Function: AhHocConnectionAddCommand::undo()
//-----------------------------------------------------------------------------
void AdHocConnectionAddCommand::undo()
{
    // Disconnect the ends.
    connection_->disconnectEnds();
    connection_->setSelected(false);

    // Remove the interconnection from the scene.
    scene_->removeItem(connection_);
    del_ = true;

    design_->getAdHocConnections()->removeAll(connection_->getAdHocConnection());
    design_->removeRoute(connection_->getRouteExtension());
    
    // Execute child commands.
    QUndoCommand::undo();
}

//-----------------------------------------------------------------------------
// Function: AhHocConnectionAddCommand::redo()
//-----------------------------------------------------------------------------
void AdHocConnectionAddCommand::redo()
{
    // Execute child commands.
    QUndoCommand::redo();

    if (!scene_->items().contains(connection_))
    {
        scene_->addItem(connection_);
    }

    design_->getAdHocConnections()->append(connection_->getAdHocConnection());
    design_->addRoute(connection_->getRouteExtension());

    connection_->connectEnds();

    scene_->clearSelection();
    connection_->setVisible(true);
    del_ = false;
}
