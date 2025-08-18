//-----------------------------------------------------------------------------
// File: addressblockeditor.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 24.08.2012
//
// Description:
// Editor for editing the details of registers in an address block.
//-----------------------------------------------------------------------------

#include "addressblockeditor.h"

#include "addressblockdelegate.h"
#include "addressblockmodel.h"
#include "ExpressionProxyModel.h"
#include "AddressBlockColumns.h"

#include <KactusAPI/IPXactSystemVerilogParser.h>
#include <KactusAPI/RegisterInterface.h>

#include <KactusAPI/LibraryInterface.h>

#include <UI/ComponentEditor/common/ParameterCompleter.h>
#include <UI/ComponentEditor/parameters/ComponentParameterModel.h>

#include <UI/Views/EditableTableView/editabletableview.h>

#include <QVBoxLayout>
#include <QHeaderView>

//-----------------------------------------------------------------------------
// Function: AddressBlockEditor::AddressBlockEditor()
//-----------------------------------------------------------------------------
AddressBlockEditor::AddressBlockEditor(QSharedPointer<QList<QSharedPointer<RegisterBase>>> registers,
    RegisterInterface* registerInterface, QSharedPointer<Component> component, LibraryInterface* handler,
    QSharedPointer<ParameterFinder> parameterFinder, QWidget* parent):
QGroupBox(tr("Registers summary"), parent),
view_(new EditableTableView(this)),
model_(0),
interface_(registerInterface),
registers_(registers)
{
    interface_->setRegisters(registers_);

    view_->verticalHeader()->show();
    view_->verticalHeader()->setMaximumWidth(300);
    view_->verticalHeader()->setMinimumWidth(view_->horizontalHeader()->fontMetrics().horizontalAdvance(tr("Name"))*2);
//    view_->verticalHeader()->setMinimumWidth(view_->horizontalHeader()->fontMetrics().width(tr("Name"))*2);     //. not surported Qt5.11. 부터 지원
    view_->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QSharedPointer<IPXactSystemVerilogParser> expressionParser(new IPXactSystemVerilogParser(parameterFinder));

    model_ = new AddressBlockModel(registerInterface, expressionParser, parameterFinder, this);

    ComponentParameterModel* componentParametersModel = new ComponentParameterModel(parameterFinder, this);
    componentParametersModel->setExpressionParser(expressionParser);

    ParameterCompleter* parameterCompleter = new ParameterCompleter(this);
    parameterCompleter->setModel(componentParametersModel);

    ExpressionProxyModel* proxy = new ExpressionProxyModel(expressionParser, this);

    proxy->setColumnToAcceptExpressions(AddressBlockColumns::REGISTER_OFFSET);
    proxy->setColumnToAcceptExpressions(AddressBlockColumns::REGISTER_SIZE);
    proxy->setColumnToAcceptExpressions(AddressBlockColumns::REGISTER_DIMENSION);
    proxy->setColumnToAcceptExpressions(AddressBlockColumns::IS_PRESENT);

    proxy->setSourceModel(model_);
	view_->setModel(proxy);

	//! Enable import/export csv file
	const QString compPath = handler->getDirectoryPath(component->getVlnv());
	QString defPath = QString("%1/registerList.csv").arg(compPath);
	view_->setDefaultImportExportPath(defPath);
	view_->setAllowImportExport(true);
    view_->setAllowElementCopying(true);
	view_->setItemsDraggable(false);
	view_->setSortingEnabled(true);

    view_->setItemDelegate(new AddressBlockDelegate(parameterCompleter, parameterFinder, this));

    connect(view_->itemDelegate(), SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(view_->itemDelegate(), SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);

	view_->sortByColumn(AddressBlockColumns::REGISTER_OFFSET, Qt::AscendingOrder);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(view_);

    connect(this, SIGNAL(addressUnitBitsChanged(int)),
        model_, SLOT(addressUnitBitsChanged(int)), Qt::UniqueConnection);

	connect(model_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(model_, SIGNAL(graphicsChanged(int)), this, SIGNAL(graphicsChanged(int)), Qt::UniqueConnection);
    connect(model_, SIGNAL(childAddressingChanged(int)), 
        this, SIGNAL(childAddressingChanged(int)), Qt::UniqueConnection);
    connect(model_, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
	connect(model_, SIGNAL(itemAdded(int)), this, SIGNAL(childAdded(int)), Qt::UniqueConnection);
	connect(model_, SIGNAL(itemRemoved(int)), this, SIGNAL(childRemoved(int)), Qt::UniqueConnection);

    connect(model_, SIGNAL(registerNameChanged(QString const&, QString const&)),
        this, SIGNAL(registerNameChanged(QString const&, QString const&)), Qt::UniqueConnection);

    connect(view_, SIGNAL(addItem(const QModelIndex&)),
        model_, SLOT(onAddItem(const QModelIndex&)), Qt::UniqueConnection);
   
	connect(view_, SIGNAL(removeItem(const QModelIndex&)),
        model_, SLOT(onRemoveItem(const QModelIndex&)), Qt::UniqueConnection);

    connect(view_->itemDelegate(), SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(view_->itemDelegate(), SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);

    connect(model_, SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);

    connect(model_, SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);

    connect(view_, SIGNAL(copyRows(QModelIndexList)),
        model_, SLOT(onCopyRows(QModelIndexList)), Qt::UniqueConnection);
    connect(view_, SIGNAL(pasteRows()), model_, SLOT(onPasteRows()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: AddressBlockEditor::refresh()
//-----------------------------------------------------------------------------
void AddressBlockEditor::refresh()
{
	view_->update();

    interface_->setRegisters(registers_);
}
