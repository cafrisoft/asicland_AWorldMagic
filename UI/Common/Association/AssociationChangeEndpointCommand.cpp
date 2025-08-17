//-----------------------------------------------------------------------------
// File: AssociationChangeEndpointCommand.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Esko Pekkarinen
// Date: 16.06.2014
//
// Description:
// Command for changing the ending item of an association item.
//-----------------------------------------------------------------------------

#include "AssociationChangeEndpointCommand.h"

#include <UI/Common/Association/Associable.h>
#include <UI/Common/Association/Association.h>

//-----------------------------------------------------------------------------
// Function: AssociationChangeEndpointCommand()
//-----------------------------------------------------------------------------
AssociationChangeEndpointCommand::AssociationChangeEndpointCommand(Association* association, Associable* oldEndpoint, 
    Associable* newEndpoint, QUndoCommand* parent) : 
    QUndoCommand(parent), 
    association_(association),
    oldEndpoint_(oldEndpoint),
    newEndpoint_(newEndpoint)
{

}

//-----------------------------------------------------------------------------
// Function: ~AssociationChangeEndpointCommand()
//-----------------------------------------------------------------------------
AssociationChangeEndpointCommand::~AssociationChangeEndpointCommand()
{

}

//-----------------------------------------------------------------------------
// Function: undo()
//-----------------------------------------------------------------------------
void AssociationChangeEndpointCommand::undo()
{
    association_->setEndItem(oldEndpoint_);

    // Execute child commands.
    QUndoCommand::undo();
}

//-----------------------------------------------------------------------------
// Function: redo()
//-----------------------------------------------------------------------------
void AssociationChangeEndpointCommand::redo()
{
    association_->setEndItem(newEndpoint_);

    // Execute child commands.
    QUndoCommand::redo();
}
