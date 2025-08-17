//-----------------------------------------------------------------------------
// File: ComConnectionDeleteCommand.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 16.02.2016
//
// Description:
// Undo delete commands for COM connections in system design.
//-----------------------------------------------------------------------------

#include "ComConnectionDeleteCommand.h"

#include <IPXACTmodels/Design/Design.h>

#include <UI/Common/Association/Association.h>
#include <UI/Common/Association/AssociationRemoveCommand.h>

#include <UI/SystemDesign/ComGraphicsConnection.h>


//-----------------------------------------------------------------------------
// Function: ComConnectionDeleteCommand::ComConnectionDeleteCommand()
//-----------------------------------------------------------------------------
ComConnectionDeleteCommand::ComConnectionDeleteCommand(ComGraphicsConnection* connection,
                                                       QSharedPointer<Design> containingDesign,
                                                       QUndoCommand* parent /* = 0 */):
QUndoCommand(parent),
connection_(connection),
scene_(connection_->scene()),
containingDesign_(containingDesign),
deleteConnection_(true)
{
    foreach(Association* association, connection_->getAssociations())
    {
        new AssociationRemoveCommand(association, scene_, this);
    }
}

//-----------------------------------------------------------------------------
// Function: ComConnectionDeleteCommand::~ComConnectionDeleteCommand()
//-----------------------------------------------------------------------------
ComConnectionDeleteCommand::~ComConnectionDeleteCommand()
{
    if (deleteConnection_)
    {
        delete connection_;
    }
}

//-----------------------------------------------------------------------------
// Function: ComConnectionDeleteCommand::undo()
//-----------------------------------------------------------------------------
void ComConnectionDeleteCommand::undo()
{
    QUndoCommand::undo();

    scene_->addItem(connection_);

    if (connection_->connectEnds())
    {
        QSharedPointer<ComInterconnection> comInterconnection = connection_->getComInterconnection();
        QList<QSharedPointer<ComInterconnection> > containedComs = containingDesign_->getComConnections();
        containedComs.append(comInterconnection);
        
        containingDesign_->setComConnections(containedComs);
        containingDesign_->addRoute(connection_->getRouteExtension());
    }

    scene_->clearSelection();
    deleteConnection_ = false;
}

//-----------------------------------------------------------------------------
// Function: ComConnectionDeleteCommand::redo()
//-----------------------------------------------------------------------------
void ComConnectionDeleteCommand::redo()
{
    connection_->setSelected(false);
    scene_->removeItem(connection_);

    connection_->disconnectEnds();

    QSharedPointer<ComInterconnection> comInterconnection = connection_->getComInterconnection();
    QList<QSharedPointer<ComInterconnection> > containedComs = containingDesign_->getComConnections();
    containedComs.removeAll(comInterconnection);
    containingDesign_->setComConnections(containedComs);

    containingDesign_->removeRoute(connection_->getRouteExtension());

    deleteConnection_ = true;

    QUndoCommand::redo();
}
