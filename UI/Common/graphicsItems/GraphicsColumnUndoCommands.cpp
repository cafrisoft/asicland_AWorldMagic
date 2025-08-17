//-----------------------------------------------------------------------------
// File: GraphicsColumnUndoCommands.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti Maatta
// Date: 1.6.2012
//
// Description:
// Undo commands for graphics columns.
//-----------------------------------------------------------------------------

#include "GraphicsColumnUndoCommands.h"

#include "GraphicsColumn.h"
#include "GraphicsColumnLayout.h"

#include <UI/Common/DesignDiagram.h>

#include <IPXACTmodels/Design/Design.h>

//-----------------------------------------------------------------------------
// Function: GraphicsColumnMoveCommand::GraphicsColumnMoveCommand()
//-----------------------------------------------------------------------------
GraphicsColumnMoveCommand::GraphicsColumnMoveCommand(GraphicsColumnLayout* layout, GraphicsColumn* column,
    QPointF const& oldPos, QUndoCommand* parent):
QUndoCommand(parent),
    layout_(layout),
    column_(column),
    oldPos_(oldPos),
    newPos_(column->pos())
{
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnMoveCommand::~GraphicsColumnMoveCommand()
//-----------------------------------------------------------------------------
GraphicsColumnMoveCommand::~GraphicsColumnMoveCommand()
{
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnMoveCommand::undo()
//-----------------------------------------------------------------------------
void GraphicsColumnMoveCommand::undo()
{
    column_->setPos(oldPos_);
    layout_->onMoveColumn(column_);
    layout_->onReleaseColumn(column_);

    // Execute child commands.
    QUndoCommand::undo();
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnMoveCommand::redo()
//-----------------------------------------------------------------------------
void GraphicsColumnMoveCommand::redo()
{
    column_->setPos(newPos_);
    layout_->onMoveColumn(column_);
    layout_->onReleaseColumn(column_);

    // Execute child commands.
    QUndoCommand::redo();
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnChangeCommand()
//-----------------------------------------------------------------------------
GraphicsColumnChangeCommand::GraphicsColumnChangeCommand(GraphicsColumn* column,
    QSharedPointer<ColumnDesc> newDesc, DesignDiagram* diagram, QUndoCommand* parent):
QUndoCommand(parent),
columnItem_(column),
columnDesc_(column->getColumnDesc()),
oldDesc_(new ColumnDesc(*column->getColumnDesc().data())),
newDesc_(newDesc),
containingDesign_(diagram->getDesign()),
diagram_(diagram)
{
}

//-----------------------------------------------------------------------------
// Function: ~GraphicsColumnChangeCommand()
//-----------------------------------------------------------------------------
GraphicsColumnChangeCommand::~GraphicsColumnChangeCommand()
{
}

//-----------------------------------------------------------------------------
// Function: undo()
//-----------------------------------------------------------------------------
void GraphicsColumnChangeCommand::undo()
{
    changeModifiedColumnInDesign(oldDesc_);
}

//-----------------------------------------------------------------------------
// Function: redo()
//-----------------------------------------------------------------------------
void GraphicsColumnChangeCommand::redo()
{
    changeModifiedColumnInDesign(newDesc_);
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnUndoCommands::changeModifiedColumnInDesign()
//-----------------------------------------------------------------------------
void GraphicsColumnChangeCommand::changeModifiedColumnInDesign(QSharedPointer<ColumnDesc> newColumn)
{
    columnDesc_->setAllowedItems(newColumn->getAllowedItems());
    columnDesc_->setContentType(newColumn->getContentType());
    columnDesc_->setName(newColumn->name());
    //columnDesc_->setPosition(newColumn->getPosition());
    columnDesc_->setWidth(newColumn->getWidth());

    columnItem_->setColumnDesc(columnDesc_);

    if (newDesc_ && oldDesc_ && newDesc_->getWidth() != oldDesc_->getWidth())
    {
        diagram_->resetSceneRectangleForItems();
    }
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnResizeCommand::GraphicsColumnResizeCommand()
//-----------------------------------------------------------------------------
GraphicsColumnResizeCommand::GraphicsColumnResizeCommand(GraphicsColumn* column, unsigned int oldWidth,
    DesignDiagram* diagram, QUndoCommand* parent):
QUndoCommand(parent),
column_(column),
oldWidth_(oldWidth),
newWidth_(column_->getColumnDesc()->getWidth()),
diagram_(diagram)
{
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnResizeCommand::~GraphicsColumnResizeCommand()
//-----------------------------------------------------------------------------
GraphicsColumnResizeCommand::~GraphicsColumnResizeCommand()
{

}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnResizeCommand::undo()
//-----------------------------------------------------------------------------
void GraphicsColumnResizeCommand::undo()
{
    changeWidth(oldWidth_);
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnResizeCommand::redo()
//-----------------------------------------------------------------------------
void GraphicsColumnResizeCommand::redo()
{
    changeWidth(newWidth_);
}

//-----------------------------------------------------------------------------
// Function: GraphicsColumnUndoCommands::changeWidth()
//-----------------------------------------------------------------------------
void GraphicsColumnResizeCommand::changeWidth(unsigned int selectedWidth)
{
    column_->setWidth(selectedWidth);

    if (oldWidth_ != newWidth_)
    {
        diagram_->resetSceneRectangleForItems();
    }
}
