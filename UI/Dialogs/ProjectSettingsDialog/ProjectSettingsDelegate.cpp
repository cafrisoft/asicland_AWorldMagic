/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSettingsDelegate.cpp
 *      Project: AWM
 *      Description :
 *          This delegate is used to draw the check marks in the middle of default and active project columns.
 */

#include "ProjectSettingsDelegate.h"

#include <UI/Dialogs/ProjectSettingsDialog/ProjectSettingsColumns.h>

#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QEvent>

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDelegate::ProjectSettingsDelegate()
//-----------------------------------------------------------------------------
ProjectSettingsDelegate::ProjectSettingsDelegate(QObject *parent): QStyledItemDelegate(parent)
{

}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDelegate::~ProjectSettingsDelegate()
//-----------------------------------------------------------------------------
ProjectSettingsDelegate::~ProjectSettingsDelegate()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDelegate::paint()
//-----------------------------------------------------------------------------
void ProjectSettingsDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option,
    QModelIndex const& index) const
{
	QStyleOptionViewItem viewItemOption(option);

    if (index.column() == ProjectSettingsColumns::DEFAULT ||index.column() == ProjectSettingsColumns::ACTIVE)
	{
        viewItemOption.decorationAlignment = Qt::AlignHCenter | Qt::AlignCenter;
        viewItemOption.decorationPosition = QStyleOptionViewItem::Top;
	}

	QStyledItemDelegate::paint(painter, viewItemOption, index);
}
