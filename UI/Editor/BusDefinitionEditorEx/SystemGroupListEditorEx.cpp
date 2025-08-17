//-----------------------------------------------------------------------------
// File: SystemGroupListEditorEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// List editor for bus interface system groups.
//-----------------------------------------------------------------------------

#include <QContextMenuEvent>
#include <QMenu>

#include <IPXACTmodels/common/VLNV.h>
#include <IPXACTmodels/BusDefinition/BusDefinition.h>

#include <IPXACTmodels/utilities/BusDefinitionUtils.h>

#include <KactusAPI/LibraryInterface.h>

#include "SystemGroupListEditorEx.h"


//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::SystemGroupListEditorEx()
//-----------------------------------------------------------------------------
SystemGroupListEditorEx::SystemGroupListEditorEx(LibraryInterface* library, QWidget *parent):
QListWidget(parent),
addAction_(new QAction(tr("Add system group"), this)),
removeAction_(new QAction(tr("Remove system group"), this)),
library_(library)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    connect(addAction_, SIGNAL(triggered()), this, SLOT(onAddNewAction()), Qt::UniqueConnection);
    connect(removeAction_, SIGNAL(triggered()), this, SLOT(onRemoveAction()), Qt::UniqueConnection);

    connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::setItems()
//-----------------------------------------------------------------------------
void SystemGroupListEditorEx::setItems(QSharedPointer<BusDefinition> busDefinition)
{
    blockSignals(true);

    while (count() > 0)
    {
        takeItem(0);
    }

    QStringList extendedSystemNames = getExtendedSystemGroupNames(busDefinition);
    foreach(QString extendSystemGroup, extendedSystemNames)
    {
        QListWidgetItem* newExtendItem = new QListWidgetItem(extendSystemGroup, this);
        newExtendItem->setFlags(newExtendItem->flags() & ~Qt::ItemIsEnabled);
    }

    foreach(QString systemGroup, busDefinition->getSystemGroupNames())
    {
        if (!extendedSystemNames.contains(systemGroup))
        {
            QListWidgetItem* newItem = new QListWidgetItem(systemGroup, this);
            newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
        }
    }

    blockSignals(false);
}

//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::getExtendedSystemGroupNames()
//-----------------------------------------------------------------------------
QStringList SystemGroupListEditorEx::getExtendedSystemGroupNames(QSharedPointer<BusDefinition> busDefinition) const
{
    QStringList extendedList;

    VLNV extendVLNV = busDefinition->getExtends();
    if (extendVLNV.isValid() && extendVLNV.getType() == VLNV::BUSDEFINITION)
    {
        QSharedPointer<Document> extendDocument = library_->getModel(extendVLNV);
        if (extendDocument)
        {
            QSharedPointer<BusDefinition> extendBus = extendDocument.dynamicCast<BusDefinition>();
            if (extendBus)
            {
                extendedList.append(BusDefinitionUtils::getSystemGroups(extendBus, library_));
            }
        }
    }

    return extendedList;
}

//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::mouseDoubleClickEvent()
//-----------------------------------------------------------------------------
void SystemGroupListEditorEx::mouseDoubleClickEvent(QMouseEvent *event)
{
    QListWidgetItem* selectedItem = itemAt(event->pos());
    if (!selectedItem)
    {
        onAddNewAction();
    }
    
    QListWidget::mouseDoubleClickEvent(event);
}

//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::contextMenuEvent()
//-----------------------------------------------------------------------------
void SystemGroupListEditorEx::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction(addAction_);

    QListWidgetItem* item = itemAt(event->pos());
    if (item && item->flags() & Qt::ItemIsEnabled)
    {
        menu.addAction(removeAction_);
    }

    menu.exec(event->globalPos());
    event->accept();
}

//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::onAddNewAction()
//-----------------------------------------------------------------------------
void SystemGroupListEditorEx::onAddNewAction()
{
    QListWidgetItem* newItem = new QListWidgetItem("new item", this);
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);

    emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: SystemGroupListEditorEx::onRemoveAction()
//-----------------------------------------------------------------------------
void SystemGroupListEditorEx::onRemoveAction()
{
    QModelIndex removedIndex = currentIndex();
    if (removedIndex.isValid())
    {
        takeItem(removedIndex.row());
    }

    emit contentChanged();
}
