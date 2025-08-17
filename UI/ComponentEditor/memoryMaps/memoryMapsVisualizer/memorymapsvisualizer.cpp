//-----------------------------------------------------------------------------
// File: memorymapsvisualizer.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 17.04.2012
//
// Description:
// A widget to visualize a set of memory maps.
//-----------------------------------------------------------------------------

#include "memorymapsvisualizer.h"

#include <UI/ComponentEditor/memoryMaps/memoryMapsVisualizer/memorymapview.h>
#include <UI/ComponentEditor/memoryMaps/memoryMapsVisualizer/memorymapscene.h>
#include <UI/ComponentEditor/memoryMaps/memoryMapsVisualizer/memorymapgraphitem.h>

#include <UI/Common/graphicsItems/visualizeritem.h>

#include <UI/Widgets/summaryLabel/summarylabel.h>

#include <QVBoxLayout>
#include <QPointF>
#include <QApplication>
//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::MemoryMapsVisualizer()
//-----------------------------------------------------------------------------
MemoryMapsVisualizer::MemoryMapsVisualizer(QWidget *parent):
ItemVisualizer(parent),
view_(new MemoryMapView(this)),
scene_(new MemoryMapScene(this)),
memGraphItems_()
{
	// display a label on top the table
	SummaryLabel* visualizationLabel = new SummaryLabel(tr("Memory maps visualization"), this);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(visualizationLabel, 0, Qt::AlignCenter);
	layout->addWidget(view_);
	layout->setContentsMargins(0, 0, 0, 0);

	view_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	view_->setScene(scene_);
}

//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::addMemoryMapItem()
//-----------------------------------------------------------------------------
void MemoryMapsVisualizer::addMemoryMapItem( MemoryMapGraphItem* memMapItem ) 
{
    memGraphItems_.append(memMapItem);

	scene_->addMemGraphItem(memMapItem);

	QPointF position = memMapItem->pos();
	view_->centerOn(position.x() - MemoryVisualizationItem::CHILD_INDENTATION, position.y());
}

//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::removeMemoryMapItem()
//-----------------------------------------------------------------------------
void MemoryMapsVisualizer::removeMemoryMapItem( MemoryMapGraphItem* memMapItem )
{
    memGraphItems_.removeAll(memMapItem);

	scene_->removeMemGraphItem(memMapItem);
}

//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::minimumSizeHint()
//-----------------------------------------------------------------------------
QSize MemoryMapsVisualizer::minimumSizeHint() const
{
	return QSize(VisualizerItem::DEFAULT_WIDTH + 80, VisualizerItem::DEFAULT_HEIGHT * 2);
}

//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::sizeHint()
//-----------------------------------------------------------------------------
QSize MemoryMapsVisualizer::sizeHint() const
{
	return minimumSizeHint();
}

//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::redoLayout()
//-----------------------------------------------------------------------------
void MemoryMapsVisualizer::redoLayout()
{
    scene_->rePosition();
}

//-----------------------------------------------------------------------------
// Function: MemoryMapsVisualizer::showEvent()
//-----------------------------------------------------------------------------
void MemoryMapsVisualizer::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    
    QApplication::setOverrideCursor(Qt::WaitCursor);

    emit displayed();

    for (MemoryMapGraphItem* memoryMap : memGraphItems_)
    {
        if (memoryMap->isPresent())
        {
            memoryMap->updateDisplay();
        }
    }
    
    redoLayout();

    QApplication::restoreOverrideCursor();
}
