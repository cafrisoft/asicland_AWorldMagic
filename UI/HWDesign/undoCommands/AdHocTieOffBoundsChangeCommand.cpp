//-----------------------------------------------------------------------------
// File: AdHocTieOffBoundsChangeCommand.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 27.04.2017
//
// Description:
// Undo command for changing tie off left and right bounds in the ad hoc editor.
//-----------------------------------------------------------------------------

#include "AdHocTieOffBoundsChangeCommand.h"

#include <IPXACTmodels/common/PartSelect.h>
#include <IPXACTmodels/Design/AdHocConnection.h>
#include <IPXACTmodels/Design/Design.h>

#include <UI/HWDesign/AdHocItem.h>
#include <UI/HWDesign/HWComponentItem.h>
#include <UI/HWDesign/HWDesignDiagram.h>

//-----------------------------------------------------------------------------
// Function: AdHocTieOffBoundsChangeCommand::AdHocTieOffBoundsChangeCommand()
//-----------------------------------------------------------------------------
AdHocTieOffBoundsChangeCommand::AdHocTieOffBoundsChangeCommand(AdHocItem* portItem, QString const& newLeftValue,
    QString const& newRightValue, QSharedPointer<AdHocConnection> tieOffConnection, HWDesignDiagram* designDiagram,
    QUndoCommand* parent):
AdHocTiedValueCommand(portItem, tieOffConnection, designDiagram),
AdHocBoundsChangeCommand(AdHocTiedValueCommand::getReferencedPort(), newLeftValue, newRightValue, parent)
{

}

//-----------------------------------------------------------------------------
// Function: AdHocTieOffBoundsChangeCommand::~AdHocTieOffBoundsChangeCommand()
//-----------------------------------------------------------------------------
AdHocTieOffBoundsChangeCommand::~AdHocTieOffBoundsChangeCommand()
{

}

//-----------------------------------------------------------------------------
// Function: AdHocTieOffBoundsChangeCommand::undo()
//-----------------------------------------------------------------------------
void AdHocTieOffBoundsChangeCommand::undo()
{
    AdHocBoundsChangeCommand::undo();

    addOrRemoveTiedValueConnection();

    emit refreshEditors();
}

//-----------------------------------------------------------------------------
// Function: AdHocTieOffBoundsChangeCommand::redo()
//-----------------------------------------------------------------------------
void AdHocTieOffBoundsChangeCommand::redo()
{
    AdHocBoundsChangeCommand::redo();

    addOrRemoveTiedValueConnection();

    emit refreshEditors();
}
