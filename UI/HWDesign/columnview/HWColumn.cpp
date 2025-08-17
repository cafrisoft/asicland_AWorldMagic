//-----------------------------------------------------------------------------
// File: HWColumn.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Joni-Matti Maatta
// Date: 4.4.2011
//
// Description:
// Diagram column class.
//-----------------------------------------------------------------------------

#include "HWColumn.h"

#include <UI/HWDesign/HWMoveCommands.h>
#include <UI/HWDesign/HWComponentItem.h>

#include <IPXACTmodels/Component/Component.h>

#include <UI/Common/graphicsItems/GraphicsConnection.h>

#include <UI/Common/DesignDiagram.h>
#include <UI/Common/diagramgrid.h>
#include <UI/Common/DiagramUtil.h>


//-----------------------------------------------------------------------------
// Function: HWColumn()
//-----------------------------------------------------------------------------
HWColumn::HWColumn(QSharedPointer<ColumnDesc> desc, GraphicsColumnLayout* layout):
    GraphicsColumn(desc, layout)
{
}

//-----------------------------------------------------------------------------
// Function: ~HWColumn()
//-----------------------------------------------------------------------------
HWColumn::~HWColumn()
{
}

//-----------------------------------------------------------------------------
// Function: HWColumn::isItemAllowed()
//-----------------------------------------------------------------------------
bool HWColumn::isItemAllowed(QGraphicsItem* item, unsigned int allowedItems) const
{
    if (item->type() == GFX_TYPE_DIAGRAM_INTERFACE || item->type() == GFX_TYPE_DIAGRAM_ADHOC_INTERFACE)
    {
        return (allowedItems & ColumnTypes::INTERFACE);
    }
    else if (item->type() == GFX_TYPE_DIAGRAM_COMPONENT)
    {
        QSharedPointer<Component> comp = qgraphicsitem_cast<HWComponentItem*>(item)->componentModel();

        // Check if this is a packaged component (and has a strict type).
        if (comp->getVlnv().isValid())
        {
            return ((comp->isBridge() && (allowedItems & ColumnTypes::BRIDGE)) ||
                (comp->isChannel() && (allowedItems & ColumnTypes::CHANNEL)) ||
                (!comp->isBus() && (allowedItems & ColumnTypes::COMPONENT)));
        }
        else
        {
            // Otherwise this is an unpacked component and can be of any type.
            return (allowedItems & (ColumnTypes::BRIDGE | ColumnTypes::CHANNEL | ColumnTypes::COMPONENT));
        }
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
// Function: HWColumn::prepareColumnMove()
//-----------------------------------------------------------------------------
void HWColumn::prepareColumnMove()
{
    foreach (QGraphicsItem *item, scene()->items())
    {
        GraphicsConnection* conn = dynamic_cast<GraphicsConnection*>(item);
        if (conn != 0)
        {
            conn->beginUpdatePosition();
        }
    }
}

//-----------------------------------------------------------------------------
// Function: HWColumn::createMoveUndoCommand()
//-----------------------------------------------------------------------------
QSharedPointer<QUndoCommand> HWColumn::createMoveUndoCommand()
{
    QSharedPointer<QUndoCommand> cmd = GraphicsColumn::createMoveUndoCommand();

    foreach (QGraphicsItem *item, scene()->items())
    {
        GraphicsConnection* conn = dynamic_cast<GraphicsConnection*>(item);
        if (conn != 0)
        {
            conn->endUpdatePosition(cmd.data());
        }
    }

    return cmd;
}
