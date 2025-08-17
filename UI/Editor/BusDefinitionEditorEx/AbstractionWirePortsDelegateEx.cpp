//-----------------------------------------------------------------------------
// File: AbstractionWirePortsDelegateEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// The delegate that provides editors for logical port properties in Abstraction Definition.
//-----------------------------------------------------------------------------

#include <QComboBox>
#include <QStringList>
#include <QLineEdit>

#include "LogicalPortColumns.h"

#include "AbstractionWirePortsDelegateEx.h"


//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsDelegateEx::AbstractionWirePortsDelegateEx()
//-----------------------------------------------------------------------------
AbstractionWirePortsDelegateEx::AbstractionWirePortsDelegateEx(LibraryInterface* libraryAccess, QObject *parent):
    AbstractionPortsDelegateEx(libraryAccess, parent)
{

}

//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsDelegateEx::createEditor()
//-----------------------------------------------------------------------------
QWidget* AbstractionWirePortsDelegateEx::createEditor(QWidget* parent, QStyleOptionViewItem const& option,
    const QModelIndex& index ) const
{
    if (index.column() == LogicalPortColumns::DIRECTION)
    {
        QComboBox* box = new QComboBox(parent);

        QStringList list;
        list.append("in");
        list.append("out");
        list.append("inout");
        box->addItems(list);

        connect(box, SIGNAL(destroyed()), this, SLOT(commitAndCloseEditor()), Qt::UniqueConnection);
        return box;
    }
    else if (index.column() == LogicalPortColumns::DRIVER)
    {
        QComboBox* box = new QComboBox(parent);

        QStringList list;
        list.append(QString("none"));
        list.append(QString("any"));
        list.append(QString("clock"));
        list.append(QString("singleShot"));
        box->addItems(list);

        connect(box, SIGNAL(destroyed()), this, SLOT(commitAndCloseEditor()), Qt::UniqueConnection);
        return box;
    }
    else
    {
        return AbstractionPortsDelegateEx::createEditor(parent, option, index);
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsDelegateEx::getQualifierList()
//-----------------------------------------------------------------------------
QStringList AbstractionWirePortsDelegateEx::getQualifierList() const
{
    QStringList qualifierList = AbstractionPortsDelegateEx::getQualifierList();
    qualifierList.append(getExclusiveItems());

    return qualifierList;
}

//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsDelegateEx::editorIsLineEditor()
//-----------------------------------------------------------------------------
bool AbstractionWirePortsDelegateEx::editorIsLineEditor(int indexColumn) const
{
    return AbstractionPortsDelegateEx::editorIsLineEditor(indexColumn) ||
        indexColumn == LogicalPortColumns::DEFAULT_VALUE;
}

//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsDelegateEx::editorIsComboBox()
//-----------------------------------------------------------------------------
bool AbstractionWirePortsDelegateEx::editorIsComboBox(int indexColumn) const
{
    return AbstractionPortsDelegateEx::editorIsComboBox(indexColumn) ||
        indexColumn == LogicalPortColumns::DIRECTION || indexColumn == LogicalPortColumns::DRIVER;
}

//-----------------------------------------------------------------------------
// Function: AbstractionWirePortsDelegateEx::getExclusiveItems()
//-----------------------------------------------------------------------------
QStringList AbstractionWirePortsDelegateEx::getExclusiveItems() const
{
    QStringList list = { "clock", "reset", };
    return list;
}
