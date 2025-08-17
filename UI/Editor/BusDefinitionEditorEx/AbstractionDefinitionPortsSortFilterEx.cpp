//-----------------------------------------------------------------------------
// File: AbstractionDefinitionPortsSortFilterEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Sort filter proxy model for abstraction definition ports.
//-----------------------------------------------------------------------------

#include <KactusAPI/KactusColors.h>

#include "AbstractionPortsModelEx.h"
#include "LogicalPortColumns.h"

#include "AbstractionDefinitionPortsSortFilterEx.h"


//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionPortsSortFilterEx::AbstractionDefinitionPortsSortFilterEx()
//-----------------------------------------------------------------------------
AbstractionDefinitionPortsSortFilterEx::AbstractionDefinitionPortsSortFilterEx(
     PortAbstractionInterface* portInterface, QObject *parent) :
QSortFilterProxyModel(parent),
portInterface_(portInterface)
{

}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionPortsSortFilterEx::data()
//-----------------------------------------------------------------------------
QVariant AbstractionDefinitionPortsSortFilterEx::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole)
    {
        return getBackgroundColorForIndex(index);
    }
    else
    {
        return QSortFilterProxyModel::data(index, role);
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionPortsSortFilterEx::getBackgroundColorForIndex()
//-----------------------------------------------------------------------------
QColor AbstractionDefinitionPortsSortFilterEx::getBackgroundColorForIndex(QModelIndex const& index) const
{
    bool systemGroupIsMandatory = isSystemGroupMandatory(index);
    if (index.column() == LogicalPortColumns::NAME || systemGroupIsMandatory)
    {
        return KactusColors::MANDATORY_FIELD;
    }
    else if ((index.column() == LogicalPortColumns::SYSTEM_GROUP && !systemGroupIsMandatory) ||
        undefinedMode(index))
    {
        return KactusColors::DISABLED_FIELD;
    }

    int previousRow = index.row() - 1;
    QString previousName("");
    QColor previousColor = KactusColors::REGULAR_FIELD;
    if (previousRow < 0)
    {
        return previousColor;
    }
    else
    {
        QModelIndex previousNameIndex = index.sibling(previousRow, LogicalPortColumns::NAME);
        previousName = previousNameIndex.data(Qt::DisplayRole).toString();
    }

    QModelIndex nameIndex = index.sibling(index.row(), LogicalPortColumns::NAME);
    QString name = nameIndex.data(Qt::DisplayRole).toString();
    previousColor = getPreviousColor(previousRow, index);

    if (name.compare(previousName) == 0)
    {
        return previousColor;
    }
    else if (previousColor == KactusColors::REGULAR_FIELD)
    {
        return KactusColors::FIELD_COLOR;
    }
    
    return KactusColors::REGULAR_FIELD;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionPortsSortFilterEx::getPreviousColor()
//-----------------------------------------------------------------------------
QColor AbstractionDefinitionPortsSortFilterEx::getPreviousColor(int const& previousRow, QModelIndex const& index)
    const
{
    QModelIndex previousColorIndex = index.sibling(previousRow, LogicalPortColumns::PRESENCE);

    QColor previousColor = previousColorIndex.data(Qt::BackgroundRole).value<QColor>();
    if (previousColor == KactusColors::DISABLED_FIELD)
    {
        previousColorIndex = index.sibling(previousRow, LogicalPortColumns::DESCRIPTION);
        previousColor = previousColorIndex.data(Qt::BackgroundRole).value<QColor>();
    }

    return previousColor;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionPortsSortFilterEx::isSystemGroupMandatory()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionPortsSortFilterEx::isSystemGroupMandatory(QModelIndex const& index) const
{
    if (index.column() == LogicalPortColumns::SYSTEM_GROUP)
    {
        QModelIndex modeIndex = index.sibling(index.row(), LogicalPortColumns::MODE);
        QString portMode = modeIndex.data(Qt::DisplayRole).toString();
        if (portMode.compare(QStringLiteral("system"), Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionPortsSortFilterEx::undefinedMode()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionPortsSortFilterEx::undefinedMode(QModelIndex const& index) const
{
    if (index.column() == LogicalPortColumns::PRESENCE || index.column() == LogicalPortColumns::DIRECTION ||
        index.column() == LogicalPortColumns::WIDTH || index.column() == LogicalPortColumns::DEFAULT_VALUE ||
        index.column() == LogicalPortColumns::INITIATIVE || index.column() == LogicalPortColumns::KIND ||
        index.column() == LogicalPortColumns::BUSWIDTH || index.column() == LogicalPortColumns::PROTOCOLTYPE ||
        index.column() == LogicalPortColumns::PAYLOADNAME || index.column() == LogicalPortColumns::PAYLOADTYPE ||
        index.column() == LogicalPortColumns::PAYLOADEXTENSION)
    {
        QModelIndex modeIndex = index.sibling(index.row(), LogicalPortColumns::MODE);

        QVariant modeData = modeIndex.data(Qt::ForegroundRole);
        QColor modeColor = modeData.value<QColor>();
        if (modeColor == KactusColors::ERROR)
        {
            return true;
        }
    }

    return false;
}

