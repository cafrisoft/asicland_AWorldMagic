//-----------------------------------------------------------------------------
// File: registereditor.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 24.08.2012
//
// Description:
// Editor for editing the details of fields in a register.
//-----------------------------------------------------------------------------

#include "registereditor.h"

#include "ExpressionProxyModel.h"
#include "registertablemodel.h"
#include "registerdelegate.h"
#include "RegisterColumns.h"

#include <KactusAPI/LibraryInterface.h>

#include <KactusAPI/IPXactSystemVerilogParser.h>
#include <KactusAPI/FieldInterface.h>

#include <UI/Views/EditableTableView/editabletableview.h>

#include <UI/ComponentEditor/common/ParameterCompleter.h>
#include <UI/ComponentEditor/parameters/ComponentParameterModel.h>

#include <QVBoxLayout>
#include <QHeaderView>

//-----------------------------------------------------------------------------
// Function: registereditor::RegisterEditor()
//-----------------------------------------------------------------------------
RegisterEditor::RegisterEditor(QSharedPointer<QList<QSharedPointer<Field>>> fields,
    QSharedPointer<Component> component, LibraryInterface* handler,
    QSharedPointer<ParameterFinder> parameterFinder, FieldInterface* fieldInterface, QWidget* parent):
QGroupBox(tr("Fields summary"), parent),
view_(new EditableTableView(this)),
model_(0),
interface_(fieldInterface),
fields_(fields)
{
    interface_->setFields(fields_);

    QSharedPointer<IPXactSystemVerilogParser> expressionParser(new IPXactSystemVerilogParser(parameterFinder));
    view_->verticalHeader()->show();
    view_->verticalHeader()->setMaximumWidth(300);
//    view_->verticalHeader()->setMinimumWidth(view_->horizontalHeader()->fontMetrics().horizontalAdvance(tr("Name"))*2);
    view_->verticalHeader()->setMinimumWidth(view_->horizontalHeader()->fontMetrics().width(tr("Name"))*2);     //. not surported Qt5.11. 부터 지원
    view_->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    model_ = new RegisterTableModel(fieldInterface, expressionParser, parameterFinder, this);

    ComponentParameterModel* componentParametersModel = new ComponentParameterModel(parameterFinder, this);
    componentParametersModel->setExpressionParser(expressionParser);

    ParameterCompleter* parameterCompleter = new ParameterCompleter(this);
    parameterCompleter->setModel(componentParametersModel);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(view_);

    ExpressionProxyModel* proxy = new ExpressionProxyModel(expressionParser, this);
    proxy->setColumnToAcceptExpressions(RegisterColumns::OFFSET_COLUMN);
    proxy->setColumnToAcceptExpressions(RegisterColumns::WIDTH_COLUMN);
    proxy->setColumnToAcceptExpressions(RegisterColumns::IS_PRESENT_COLUMN);

	proxy->setSourceModel(model_);
	view_->setModel(proxy);

	//! \brief Enable import/export csv file
    const QString compPath = handler->getDirectoryPath(component->getVlnv());
	QString defPath = QString("%1/fieldListing.csv").arg(compPath);
    view_->setDefaultImportExportPath(defPath);
	
    view_->setAllowImportExport(true);
	view_->setItemsDraggable(false);
	view_->setSortingEnabled(true);
    view_->setAllowElementCopying(true);

    view_->setItemDelegate(new RegisterDelegate(parameterCompleter, parameterFinder, this));

    view_->sortByColumn(RegisterColumns::OFFSET_COLUMN, Qt::AscendingOrder);

    connect(model_, SIGNAL(graphicsChanged(int)), this, SIGNAL(graphicsChanged(int)), Qt::UniqueConnection);
    connect(model_, SIGNAL(addressingChanged(int)), this, SIGNAL(addressingChanged(int)), Qt::UniqueConnection);
	connect(model_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
	connect(model_, SIGNAL(fieldAdded(int)), this, SIGNAL(childAdded(int)), Qt::UniqueConnection);
	connect(model_, SIGNAL(fieldRemoved(int)), this, SIGNAL(childRemoved(int)), Qt::UniqueConnection);

    connect(model_, SIGNAL(fieldNameChanged(QString const&, QString const&)),
        this, SIGNAL(fieldNameChanged(QString const&, QString const&)), Qt::UniqueConnection);

    connect(model_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);

    connect(view_->itemDelegate(), SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(view_->itemDelegate(), SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);

	connect(view_, SIGNAL(addItem(const QModelIndex&)),
        model_, SLOT(onAddItem(const QModelIndex&)), Qt::UniqueConnection);
	connect(view_, SIGNAL(removeItem(const QModelIndex&)),
		model_, SLOT(onRemoveItem(const QModelIndex&)), Qt::UniqueConnection);

    connect(model_, SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);
    connect(model_, SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);

    connect(view_, SIGNAL(copyRows(QModelIndexList)),
        model_, SLOT(onCopyRows(QModelIndexList)), Qt::UniqueConnection);
    connect(view_, SIGNAL(pasteRows()), model_, SLOT(onPasteRows()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: registereditor::refresh()
//-----------------------------------------------------------------------------
void RegisterEditor::refresh()
{
	view_->update();

    interface_->setFields(fields_);
}
