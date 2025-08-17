//-----------------------------------------------------------------------------
// File: SystemAddCommands.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Joni-Matti Maatta
// Date: 13.10.2011
//
// Description:
// Undo add commands for system designs.
//-----------------------------------------------------------------------------

#include "SystemAddCommands.h"

#include <IPXACTmodels/Design/ComponentInstance.h>

#include <UI/Common/DesignDiagram.h>
#include <UI/Common/graphicsItems/ComponentItem.h>

#include <UI/SystemDesign/HWMappingItem.h>
#include <UI/SystemDesign/SWComponentItem.h>
#include <UI/SystemDesign/SystemColumn.h>
#include <UI/SystemDesign/SystemComponentItem.h>
#include <UI/SystemDesign/SWInterfaceItem.h>


//-----------------------------------------------------------------------------
// Function: SWPortAddCommand()
//-----------------------------------------------------------------------------
SWPortAddCommand::SWPortAddCommand(SystemComponentItem* component, QPointF const& pos, DesignDiagram* diagram,
    QUndoCommand* parent):
QUndoCommand(parent),
component_(component),
pos_(pos),
port_(0),
scene_(component->scene()),
del_(false),
diagram_(diagram)
{
}

//-----------------------------------------------------------------------------
// Function: SWPortAddCommand()
//-----------------------------------------------------------------------------
SWPortAddCommand::SWPortAddCommand(SystemComponentItem* component, SWPortItem* port, DesignDiagram* diagram,
    QUndoCommand* parent):
QUndoCommand(parent),
component_(component),
pos_(),
port_(port),
scene_(component->scene()),
del_(false),
diagram_(diagram)
{
}

//-----------------------------------------------------------------------------
// Function: ~SWPortAddCommand()
//-----------------------------------------------------------------------------
SWPortAddCommand::~SWPortAddCommand()
{
    if (del_)
    {
        delete port_;
    }
}

//-----------------------------------------------------------------------------
// Function: undo()
//-----------------------------------------------------------------------------
void SWPortAddCommand::undo()
{
    Q_ASSERT(port_ != 0);

    // Remove the port from the component and from the scene
    component_->removePort(port_);
    scene_->removeItem(port_);
    del_ = true;

    // Execute child commands.
    QUndoCommand::undo();
}

//-----------------------------------------------------------------------------
// Function: redo()
//-----------------------------------------------------------------------------
void SWPortAddCommand::redo()
{
    // Add a port to the component.
    if (port_ == 0)
    {
        port_ = component_->addPort(pos_);
    }
    else
    {
        component_->addPort(port_);
    }

    del_ = false;

    // Child commands need not be executed because the other ports change their position
    // in a deterministic way.
    //QUndoCommand::redo();

    diagram_->resetSceneRectangleForItems();
}
