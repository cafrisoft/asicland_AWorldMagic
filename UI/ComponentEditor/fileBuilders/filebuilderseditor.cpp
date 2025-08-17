//-----------------------------------------------------------------------------
// File: filebuilderseditor.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 31.05.2012
//
// Description:
// Editor to edit file builders.
//-----------------------------------------------------------------------------

#include "filebuilderseditor.h"
#include "filebuildersdelegate.h"

#include <KactusAPI/IPXactSystemVerilogParser.h>
#include <KactusAPI/FileBuilderInterface.h>

#include <IPXACTmodels/common/FileBuilder.h>

#include <UI/ComponentEditor/common/ParameterCompleter.h>
#include <UI/ComponentEditor/parameters/ComponentParameterModel.h>

#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: FileBuildersEditor::FileBuildersEditor()
//-----------------------------------------------------------------------------
FileBuildersEditor::FileBuildersEditor(FileBuilderInterface* fileBuilderInterface,
    QSharedPointer<ParameterFinder> parameterFinder, QSharedPointer<ExpressionParser> expressionParser,
    QSharedPointer<ExpressionFormatter> expressionFormatter,
    QSharedPointer<QList<QSharedPointer<FileBuilder>>> availableFileBuilders, QWidget* parent):
QGroupBox(tr("Default file build commands"), parent),
view_(this), 
model_(0),
proxy_(this),
availableFileBuilders_(availableFileBuilders),
fileBuilderInterface_(fileBuilderInterface)
{
    fileBuilderInterface_->setFileBuilders(availableFileBuilders_);

    model_ =
        new FileBuildersModel(fileBuilderInterface, parameterFinder, expressionFormatter, expressionParser, this);

    ComponentParameterModel* componentParametersModel = new ComponentParameterModel(parameterFinder, this);
    componentParametersModel->setExpressionParser(expressionParser);

    ParameterCompleter* parameterCompleter = new ParameterCompleter(this);
    parameterCompleter->setModel(componentParametersModel);

	// set view to be sortable
	view_.setSortingEnabled(true);

	// items can not be dragged
	view_.setItemsDraggable(false);

	// set the delegate to provide editors
    view_.setItemDelegate(new FileBuildersDelegate(parameterCompleter, parameterFinder, this));

	// set source model for proxy
    proxy_.setSourceModel(model_);
	// set proxy to be the source for the view
	view_.setModel(&proxy_);

	// sort the view
	view_.sortByColumn(0, Qt::AscendingOrder);

	// create the layout, add widgets to it
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(&view_);

    connect(model_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(model_, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(model_, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(addItem(const QModelIndex&)),
        model_, SLOT(onAddItem(const QModelIndex&)), Qt::UniqueConnection);
    connect(&view_, SIGNAL(removeItem(const QModelIndex&)),
        model_, SLOT(onRemoveItem(const QModelIndex&)), Qt::UniqueConnection);

    connect(view_.itemDelegate(), SIGNAL(increaseReferences(QString)), 
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(view_.itemDelegate(), SIGNAL(decreaseReferences(QString)), 
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);

    connect(model_, SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: FileBuildersEditor::~FileBuildersEditor()
//-----------------------------------------------------------------------------
FileBuildersEditor::~FileBuildersEditor()
{

}

//-----------------------------------------------------------------------------
// Function: FileBuildersEditor::isValid()
//-----------------------------------------------------------------------------
bool FileBuildersEditor::isValid() const
{
    return model_->isValid();
}

//-----------------------------------------------------------------------------
// Function: FileBuildersEditor::refresh()
//-----------------------------------------------------------------------------
void FileBuildersEditor::refresh()
{
    fileBuilderInterface_->setFileBuilders(availableFileBuilders_);

	view_.update();
}
