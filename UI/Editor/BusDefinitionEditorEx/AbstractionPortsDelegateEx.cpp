//-----------------------------------------------------------------------------
// File: AbstractionPortsDelegateEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Master delegate for abstraction definition wire and transactional ports.
//-----------------------------------------------------------------------------

#include <QComboBox>
#include <QStringList>
#include <QLineEdit>

#include <IPXACTmodels/BusDefinition/BusDefinition.h>

#include <IPXACTmodels/utilities/BusDefinitionUtils.h>

#include <KactusAPI/LibraryInterface.h>

#include "LogicalPortColumns.h"

#include "AbstractionPortsDelegateEx.h"


//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::AbstractionPortsDelegateEx()
//-----------------------------------------------------------------------------
AbstractionPortsDelegateEx::AbstractionPortsDelegateEx(LibraryInterface* libraryAcces, QObject *parent):
EnumerationEditorConstructorDelegate(parent),
libraryAccess_(libraryAcces),
busDefinition_(0)
{
    setHideCheckAll(true);
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::createEditor()
//-----------------------------------------------------------------------------
QWidget* AbstractionPortsDelegateEx::createEditor(QWidget* parent, QStyleOptionViewItem const& option,
    const QModelIndex& index ) const
{
    if (index.column() == LogicalPortColumns::BUSWIDTH)
    {
        QLineEdit* line = new QLineEdit(parent);
        connect(line, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()), Qt::UniqueConnection);
        return line;
    }
    else if (index.column() == LogicalPortColumns::MODE)
    {
        QComboBox* box = new QComboBox(parent);

        QStringList list = { "master", "slave", "system" };
        box->addItems(list);

        connect(box, SIGNAL(destroyed()), this, SLOT(commitAndCloseEditor()), Qt::UniqueConnection);
        return box;
    }
    else if (index.column() == LogicalPortColumns::PRESENCE)
    {
        QComboBox* box = new QComboBox(parent);

        QStringList list = { "required", "optional", "illegal" };
        box->addItems(list);

        connect(box, SIGNAL(destroyed()), this, SLOT(commitAndCloseEditor()), Qt::UniqueConnection);
        return box;
    }
    else if (index.column() == LogicalPortColumns::SYSTEM_GROUP)
    {
        if (busDefinition_)
        {
            QComboBox* box = new QComboBox(parent);
            box->addItem("");
            box->addItems(BusDefinitionUtils::getSystemGroups(busDefinition_, libraryAccess_));

            connect(box, SIGNAL(destroyed()), this, SLOT(commitAndCloseEditor()), Qt::UniqueConnection);
            return box;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return EnumerationEditorConstructorDelegate::createEditor(parent, option, index);
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::setEditorData()
//-----------------------------------------------------------------------------
void AbstractionPortsDelegateEx::setEditorData(QWidget* editor, QModelIndex const& index ) const
{
    if (editorIsLineEditor(index.column()))
    {
        QLineEdit* line = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT_X(line, "BusPortsDelegate::setEditorData", "Type conversion failed for QLineEdit");

        QString text = index.data(Qt::DisplayRole).toString();
        line->setText(text);

        if (index.column() == LogicalPortColumns::NAME && text == QLatin1String("unnamed"))
        {
            line->clear();
        }
    }
    else if (editorIsComboBox(index.column()))
    {
        QComboBox* box = qobject_cast<QComboBox*>(editor);
        Q_ASSERT_X(box, "BusPortsDelegate::setEditorData", "Type conversion failed for combo box");

        QString text = index.data(Qt::DisplayRole).toString();
        int textIndex = box->findText(text);
        if (textIndex >= 0)
        {
            box->setCurrentIndex(textIndex);
        }
        else
        {
            box->setCurrentText(text);
        }
    }

    else
    {
        EnumerationEditorConstructorDelegate::setEditorData(editor, index);
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::editorIsLineEditor()
//-----------------------------------------------------------------------------
bool AbstractionPortsDelegateEx::editorIsLineEditor(int indexColumn) const
{
    return indexColumn == LogicalPortColumns::NAME || indexColumn == LogicalPortColumns::WIDTH ||
        indexColumn == LogicalPortColumns::BUSWIDTH || indexColumn == LogicalPortColumns::DESCRIPTION ||
        indexColumn == LogicalPortColumns::PAYLOADNAME || indexColumn == LogicalPortColumns::PAYLOADEXTENSION;
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::editorIsComboBox()
//-----------------------------------------------------------------------------
bool AbstractionPortsDelegateEx::editorIsComboBox(int indexColumn) const
{
    return indexColumn == LogicalPortColumns::MODE || indexColumn == LogicalPortColumns::PRESENCE ||
        indexColumn == LogicalPortColumns::SYSTEM_GROUP || indexColumn == LogicalPortColumns::PROTOCOLTYPE ||
        indexColumn == LogicalPortColumns::PAYLOADTYPE;
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::setModelData()
//-----------------------------------------------------------------------------
void AbstractionPortsDelegateEx::setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index)
    const
{
    if (editorIsLineEditor(index.column()))
    {
        QLineEdit* lineEditor = qobject_cast<QLineEdit*>(editor);
        Q_ASSERT_X(lineEditor, "BusPortsDelegate::setModelData", "Type conversion failed for normal QLineEdit");

        model->setData(index, lineEditor->text(), Qt::EditRole);
    }
    else if (editorIsComboBox(index.column()))
    {
        QComboBox* selector = qobject_cast<QComboBox*>(editor);
        Q_ASSERT_X(selector, "BusPortsDelegate::setModelData", "Type conversion failed for combo box");

        model->setData(index, selector->currentText(), Qt::EditRole);
    }

    else
    {
        EnumerationEditorConstructorDelegate::setModelData(editor, model, index);
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::commitAndCloseEditor()
//-----------------------------------------------------------------------------
void AbstractionPortsDelegateEx::commitAndCloseEditor()
{
	QWidget* editor = qobject_cast<QWidget*>(sender());

	if (editor)
    {
		emit commitData(editor);
		emit closeEditor(editor);
	}
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::setBusDef()
//-----------------------------------------------------------------------------
void AbstractionPortsDelegateEx::setBusDef(QSharedPointer<const BusDefinition> busDefinition)
{
    busDefinition_ = busDefinition;
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::getQualifierList()
//-----------------------------------------------------------------------------
QStringList AbstractionPortsDelegateEx::getQualifierList() const
{
    QStringList list = { "address", "data", };
    return list;
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::isEnumerationEditorColumn()
//-----------------------------------------------------------------------------
bool AbstractionPortsDelegateEx::isEnumerationEditorColumn(QModelIndex const& index) const
{
    if (index.column() == LogicalPortColumns::QUALIFIER)
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::getCurrentSelection()
//-----------------------------------------------------------------------------
QStringList AbstractionPortsDelegateEx::getCurrentSelection(QModelIndex const& index) const
{
    return index.data().toString().split(" ");
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::getAvailableItems()
//-----------------------------------------------------------------------------
QStringList AbstractionPortsDelegateEx::getAvailableItems() const
{
    return getQualifierList();
}

//-----------------------------------------------------------------------------
// Function: AbstractionPortsDelegateEx::setEnumerationDataToModel()
//-----------------------------------------------------------------------------
void AbstractionPortsDelegateEx::setEnumerationDataToModel(QModelIndex const& index, QAbstractItemModel* model, QStringList const& selectedItems) const
{
    model->setData(index, selectedItems);
}
