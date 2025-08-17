//-----------------------------------------------------------------------------
// File: Component.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 22.08.2012
//
// Description:
// The editor to edit the address blocks of a single memory map.
//-----------------------------------------------------------------------------

#include "memorymapeditor.h"

#include "memorymapmodel.h"
#include "memorymapdelegate.h"
#include "MemoryMapColumns.h"

#include <KactusAPI/AddressBlockInterface.h>

#include <KactusAPI/LibraryInterface.h>

#include <IPXACTmodels/Component/Component.h>

#include <UI/Views/EditableTableView/editabletableview.h>
#include <UI/Widgets/summaryLabel/summarylabel.h>

#include <UI/ComponentEditor/common/ParameterCompleter.h>
#include <UI/ComponentEditor/parameters/ComponentParameterModel.h>
#include <UI/ComponentEditor/memoryMaps/MemoryBlockFilter.h>

#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: MemoryMapEditor::MemoryMapEditor()
//-----------------------------------------------------------------------------
MemoryMapEditor::MemoryMapEditor(QSharedPointer<Component> component, LibraryInterface* handler,
    QSharedPointer<ParameterFinder> parameterFinder, QSharedPointer<ExpressionParser> expressionParser,
    AddressBlockInterface* blockInterface, QSharedPointer<QList<QSharedPointer<MemoryBlockBase>>> blocks,
    QWidget* parent):
QGroupBox(tr("Address blocks summary"), parent),
view_(new EditableTableView(this)),
model_(new MemoryMapModel(blockInterface, expressionParser, parameterFinder, this)),
interface_(blockInterface),
blocks_(blocks),
component_(component),
proxy_()
{
    interface_->setMemoryBlocks(blocks_);
    interface_->setupSubInterfaces(component_);

    ComponentParameterModel* componentParameterModel = new ComponentParameterModel(parameterFinder, this);
    componentParameterModel->setExpressionParser(expressionParser);

    ParameterCompleter* parameterCompleter = new ParameterCompleter(this);
    parameterCompleter->setModel(componentParameterModel);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(view_);

    proxy_ = new MemoryBlockFilter(expressionParser, blockInterface, this);
    proxy_->setColumnToAcceptExpressions(MemoryMapColumns::BASE_COLUMN);
    proxy_->setColumnToAcceptExpressions(MemoryMapColumns::RANGE_COLUMN);
    proxy_->setColumnToAcceptExpressions(MemoryMapColumns::WIDTH_COLUMN);
    proxy_->setColumnToAcceptExpressions(MemoryMapColumns::IS_PRESENT);

	proxy_->setSourceModel(model_);
	view_->setModel(proxy_);

	//! Enable import/export csv file
    const QString compPath = handler->getDirectoryPath(component->getVlnv());
	QString defPath = QString("%1/addrBlockList.csv").arg(compPath);
	view_->setDefaultImportExportPath(defPath);
	view_->setAllowImportExport(true);
    view_->setAllowElementCopying(true);

	view_->setItemsDraggable(false);
	view_->setSortingEnabled(true);

    view_->setItemDelegate(new MemoryMapDelegate(parameterCompleter, parameterFinder, this));

	view_->sortByColumn(MemoryMapColumns::BASE_COLUMN, Qt::AscendingOrder);

	connect(model_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(model_, SIGNAL(graphicsChanged(int)), this, SIGNAL(graphicsChanged(int)), Qt::UniqueConnection);
    
    connect(model_, SIGNAL(childAddressingChanged(int)), 
        this, SIGNAL(childAddressingChanged(int)), Qt::UniqueConnection);
	connect(model_, SIGNAL(itemAdded(int)),	this, SIGNAL(childAdded(int)), Qt::UniqueConnection);
	connect(model_, SIGNAL(itemRemoved(int)), this, SIGNAL(childRemoved(int)), Qt::UniqueConnection);

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

    connect(this, SIGNAL(assignNewAddressUnitBits(QString const&)),
        model_, SLOT(addressUnitBitsUpdated(QString const&)), Qt::UniqueConnection);

    connect(model_, SIGNAL(blockNameChanged(QString const&, QString const&)),
        this, SIGNAL(addressBlockNameChanged(QString const&, QString const&)), Qt::UniqueConnection);

    connect(model_, SIGNAL(invalidateOtherFilter()), this, SIGNAL(invalidateOtherFilter()), Qt::UniqueConnection);

    connect(this, SIGNAL(invalidateThisFilter()), proxy_, SLOT(invalidate()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: MemoryMapEditor::~MemoryMapEditor()
//-----------------------------------------------------------------------------
MemoryMapEditor::~MemoryMapEditor()
{
}

//-----------------------------------------------------------------------------
// Function: MemoryMapEditor::refresh()
//-----------------------------------------------------------------------------
void MemoryMapEditor::refresh()
{
	view_->update();

    interface_->setMemoryBlocks(blocks_);
    interface_->setupSubInterfaces(component_);
}
