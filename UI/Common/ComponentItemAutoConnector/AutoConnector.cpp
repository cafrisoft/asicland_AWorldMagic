//-----------------------------------------------------------------------------
// File: AutoConnector.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 12.02.2019
//
// Description:
// Automatically connects items of two component items.
//-----------------------------------------------------------------------------

#include "AutoConnector.h"

#include <UI/Common/graphicsItems/ComponentItem.h>

#include <UI/Common/ComponentItemAutoConnector/ListFiller.h>
#include <UI/Common/ComponentItemAutoConnector/TableAutoConnector.h>
#include <UI/Common/ComponentItemAutoConnector/AutoConnectorConnectionTable.h>
#include <UI/Common/ComponentItemAutoConnector/AutoConnectorConnectionDelegate.h>
#include <UI/Common/ComponentItemAutoConnector/AutoConnectorListFilter.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QStandardItemModel>

//-----------------------------------------------------------------------------
// Function: AutoConnector::AutoConnector()
//-----------------------------------------------------------------------------
AutoConnector::AutoConnector(QString const& firstComponentName, QString const& secondComponentName,
    QSharedPointer<Component> firstComponent, QSharedPointer<Component> secondComponent, ListFiller* listFiller,
    TableAutoConnector* tableInitializer, QString const& itemName, TableItemMatcher* itemMatcher, QWidget* parent):
QWidget(parent),
firstComponent_(firstComponent),
secondComponent_(secondComponent),
firstListFilter_(),
secondListFilter_(),
firstItemList_(),
secondItemList_(),
connectorTable_(),
tableInitializer_(tableInitializer)
{
    setupLayout(firstComponentName, secondComponentName, listFiller, itemName, itemMatcher);
}

//-----------------------------------------------------------------------------
// Function: AutoConnector::~AutoConnector()
//-----------------------------------------------------------------------------
AutoConnector::~AutoConnector()
{

}

//-----------------------------------------------------------------------------
// Function: AutoConnector::getConnectedItems()
//-----------------------------------------------------------------------------
QVector<QPair<QString, QString> > AutoConnector::getConnectedItems() const
{
    return connectorTable_->getConnectedItems();
}

//-----------------------------------------------------------------------------
// Function: AutoConnector::setupLayout()
//-----------------------------------------------------------------------------
void AutoConnector::setupLayout(QString const& firstComponentName, QString const& secondComponentName,
    ListFiller* listFiller, QString const& itemName, TableItemMatcher* itemMatcher)
{
    firstItemList_ = new QListView(this);
    secondItemList_ = new QListView(this);

    firstItemList_->setDragEnabled(true);
    firstItemList_->setDragDropMode(QAbstractItemView::DragOnly);
    secondItemList_->setDragEnabled(true);
    secondItemList_->setDragDropMode(QAbstractItemView::DragOnly);

    QCheckBox* firstHideBox(new QCheckBox(tr("Hide connected"), this));
    QCheckBox* secondHideBox(new QCheckBox(tr("Hide connected"), this));

    firstHideBox->setChecked(true);
    secondHideBox->setChecked(true);

    QVBoxLayout* firstComponentLayout(new QVBoxLayout());
    QVBoxLayout* secondComponentLayout(new QVBoxLayout());
    firstComponentLayout->addWidget(firstItemList_, 1);
    firstComponentLayout->addWidget(firstHideBox);
    secondComponentLayout->addWidget(secondItemList_, 1);
    secondComponentLayout->addWidget(secondHideBox);

    QGroupBox* firstComponentGroup(new QGroupBox(firstComponentName, this));
    QGroupBox* secondComponentGroup(new QGroupBox(secondComponentName, this));
    firstComponentGroup->setLayout(firstComponentLayout);
    secondComponentGroup->setLayout(secondComponentLayout);

    connectorTable_ = new AutoConnectorConnectionTable(firstComponent_, secondComponent_, firstItemList_,
        secondItemList_, firstComponentName, secondComponentName, itemMatcher, this);
    connectorTable_->setItemDelegate(new AutoConnectorConnectionDelegate(
        firstComponent_, secondComponent_, firstItemList_, secondItemList_, itemMatcher, this));

    connect(connectorTable_->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)),
        this, SLOT(invalidateListFilters()), Qt::UniqueConnection);
    connect(connectorTable_->model(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
        this, SLOT(invalidateListFilters()), Qt::UniqueConnection);

    QVBoxLayout* connectedItemsLayout(new QVBoxLayout());
    connectedItemsLayout->addWidget(connectorTable_);

    QGroupBox* connectedItemsGroup(new QGroupBox(tr("Connected ") + itemName));
    connectedItemsGroup->setAlignment(Qt::AlignHCenter);
    connectedItemsGroup->setLayout(connectedItemsLayout);

    QHBoxLayout* mainLayout(new QHBoxLayout(this));
    mainLayout->addWidget(firstComponentGroup);
    mainLayout->addWidget(connectedItemsGroup, 2);
    mainLayout->addWidget(secondComponentGroup);

    QStandardItemModel* firstListModel(new QStandardItemModel(this));
    QStandardItemModel* secondListModel(new QStandardItemModel(this));

    listFiller->initializeList(firstListModel, firstComponent_);
    listFiller->initializeList(secondListModel, secondComponent_);

    firstListFilter_ = new AutoConnectorListFilter(connectorTable_, 0, firstHideBox);
    firstListFilter_->setSourceModel(firstListModel);
    firstItemList_->setModel(firstListFilter_);
    secondListFilter_ = new AutoConnectorListFilter(connectorTable_, 1, secondHideBox);
    secondListFilter_->setSourceModel(secondListModel);
    secondItemList_->setModel(secondListFilter_);
}

//-----------------------------------------------------------------------------
// Function: AutoConnector::invalidateListFilters()
//-----------------------------------------------------------------------------
void AutoConnector::invalidateListFilters()
{
    firstListFilter_->invalidate();
    secondListFilter_->invalidate();
}

//-----------------------------------------------------------------------------
// Function: AutoConnector::connectSelectedItems()
//-----------------------------------------------------------------------------
void AutoConnector::connectSelectedItems()
{
    tableInitializer_->connectSelectedFromLists(firstItemList_, secondItemList_, connectorTable_);
}

//-----------------------------------------------------------------------------
// Function: AutoConnector::connectAutomatically()
//-----------------------------------------------------------------------------
void AutoConnector::connectAutomatically()
{
    tableInitializer_->initializeTable(connectorTable_, firstComponent_, secondComponent_);
}

//-----------------------------------------------------------------------------
// Function: AutoConnector::clearConnectedItems()
//-----------------------------------------------------------------------------
void AutoConnector::clearConnectedItems()
{
    tableInitializer_->clearTable(connectorTable_);
}
