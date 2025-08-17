//-----------------------------------------------------------------------------
// File: TopAdHocVisibilityChangeCommand.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 25.10.2017
//
// Description:
// Undo command for changing port adhoc visibility in top component of design diagram.
//-----------------------------------------------------------------------------

#include "TopAdHocVisibilityChangeCommand.h"

#include <UI/HWDesign/HWDesignDiagram.h>
#include <UI/HWDesign/columnview/HWColumn.h>
#include <UI/HWDesign/undoCommands/AdHocTieOffConnectionDeleteCommand.h>
#include <UI/HWDesign/undoCommands/HWColumnAddCommand.h>

#include <UI/Common/graphicsItems/GraphicsColumnLayout.h>
#include <UI/Common/graphicsItems/GraphicsColumnConstants.h>

#include <IPXACTmodels/Design/AdHocConnection.h>
#include <IPXACTmodels/Design/Design.h>

//-----------------------------------------------------------------------------
// Function: TopAdHocVisibilityChangeCommand::TopAdHocVisibilityChangeCommand()
//-----------------------------------------------------------------------------
TopAdHocVisibilityChangeCommand::TopAdHocVisibilityChangeCommand(HWDesignDiagram* diagram, QString const& portName,
    bool newVisibility, QUndoCommand* parent):
AdHocVisibilityChangeCommand(diagram, portName, newVisibility, parent)
{
    setupTieOffConnectionForDeletion(diagram, portName);

    if (newVisibility)
    {
        setupColumnAddition(diagram);
    }
}

//-----------------------------------------------------------------------------
// Function: TopAdHocVisibilityChangeCommand::setupTieOffConnectionForDeletion()
//-----------------------------------------------------------------------------
void TopAdHocVisibilityChangeCommand::setupTieOffConnectionForDeletion(HWDesignDiagram* diagram,
    QString const& portName)
{
    if (diagram && diagram->getDesign())
    {
        QSharedPointer<Design> containingDesign = diagram->getDesign();
        if (containingDesign)
        {
            foreach (QSharedPointer<AdHocConnection> connection, *containingDesign->getAdHocConnections())
            {
                if (!connection->getTiedValue().isEmpty())
                {
                    if ((connection->getInternalPortReferences()->size() == 1 &&
                        connection->getExternalPortReferences()->isEmpty() &&
                        connection->getInternalPortReferences()->first()->getPortRef() == portName) ||
                        (connection->getExternalPortReferences()->size() == 1 &&
                        connection->getInternalPortReferences()->isEmpty() &&
                        connection->getExternalPortReferences()->first()->getPortRef() == portName))
                    {
                        new AdHocTieOffConnectionDeleteCommand(containingDesign, connection, this);
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: TopAdHocVisibilityChangeCommand::setupColumnAddition()
//-----------------------------------------------------------------------------
void TopAdHocVisibilityChangeCommand::setupColumnAddition(HWDesignDiagram* diagram)
{
    GraphicsColumnLayout* diagramLayout = diagram->getLayout().data();
    foreach (GraphicsColumn* column, diagramLayout->getColumns())
    {
        if (column->getColumnDesc()->getAllowedItems() & ColumnTypes::INTERFACE)
        {
            return;
        }
    }

    QSharedPointer<ColumnDesc> newColumnDescription(
        new ColumnDesc(QString("IO"), ColumnTypes::IO, 0, GraphicsColumnConstants::IO_COLUMN_WIDTH));

    HWColumn* newColumn(new HWColumn(newColumnDescription, diagramLayout));

    new HWColumnAddCommand(diagramLayout, newColumn, diagram, this);
}

//-----------------------------------------------------------------------------
// Function: TopAdHocVisibilityChangeCommand::~TopAdHocVisibilityChangeCommand()
//-----------------------------------------------------------------------------
TopAdHocVisibilityChangeCommand::~TopAdHocVisibilityChangeCommand()
{

}
