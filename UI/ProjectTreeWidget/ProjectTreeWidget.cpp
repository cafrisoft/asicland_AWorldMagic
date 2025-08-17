//-----------------------------------------------------------------------------
// File: ProjectTreeWidget.cpp
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.HG
// Date: 09.01.2024
//
// Description:
// ProjectTreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

#include "ProjectTreeWidget.h"
#include "ProjectTreeView.h"
#include <KactusAPI/ProjectTreeModel.h>

#include <QObject>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: ProjectTreeWidget::ProjectTreeWidget()
//-----------------------------------------------------------------------------
ProjectTreeWidget::ProjectTreeWidget(LibraryInterface* handler, ProjectTreeModel* dataModel, QWidget* parent):
QWidget(parent),
    filter_(new ProjectTreeFilter(handler, this)),
    view_(handler, filter_, this),
    dataModel_(dataModel)
{
    filter_->setSourceModel(dataModel);

        // set view to use ProjectTreeFilter as source model
	view_.setModel(filter_);

	// the layout to manage the visible items in the widget
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(&view_);
	layout->setContentsMargins(0, 0, 0, 0);

	setupConnections(dataModel);

	view_.sortByColumn(0, Qt::AscendingOrder);
    view_.setHeaderHidden(true);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeWidget::~ProjectTreeWidget()
//-----------------------------------------------------------------------------
ProjectTreeWidget::~ProjectTreeWidget()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectTreeWidget::setupConnections()
//-----------------------------------------------------------------------------
void ProjectTreeWidget::setupConnections(ProjectTreeModel* dataModel)
{
	connect(&view_, SIGNAL(errorMessage(QString const&)),
        this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(noticeMessage(QString const&)),
		this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);

	// connect the view to the tree model
	connect(&view_, SIGNAL(openComponent(QModelIndex const&)),
		    dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(openDesign(QModelIndex const&, QString const&)), 
		    dataModel, SLOT(onOpenDesign(QModelIndex const&, QString const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openMemoryDesign(const QModelIndex&, QString const&)), 
        dataModel, SLOT(onOpenMemoryDesign(const QModelIndex&, QString const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openSWDesign(QModelIndex const&)), 
            dataModel, SLOT(onOpenSWDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openSystemDesign(QModelIndex const&)), 
        dataModel, SLOT(onOpenSystemDesign(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(createNewComponent(QModelIndex const&)),
		    dataModel, SLOT(onCreateNewComponent(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(createNewDesign(QModelIndex const&)),
		    dataModel, SLOT(onCreateNewDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(createNewSWDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewSWDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(createNewSystemDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewSystemDesign(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(openBus(QModelIndex const&)),
		    dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openCatalog(QModelIndex const&)),
        dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(createBus(QModelIndex const&)),
		    dataModel, SLOT(onCreateBus(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(createAbsDef(QModelIndex const&)),
		    dataModel, SLOT(onCreateAbsDef(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openComDef(QModelIndex const&)),
            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(createComDef(QModelIndex const&)),
            dataModel, SLOT(onCreateComDef(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openApiDef(QModelIndex const&)),
            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(createApiDef(QModelIndex const&)),
            dataModel, SLOT(onCreateApiDef(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(deleteItem(QModelIndex const&)),
		    dataModel, SLOT(onDeleteItem(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(exportItem(QModelIndex const&)),
//		dataModel, SLOT(onExportItem(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(exportItem()), this, SIGNAL(exportItem()), Qt::UniqueConnection);

    connect(&view_, SIGNAL(importItem()), this, SIGNAL(importItem()), Qt::UniqueConnection);

    connect(&view_, SIGNAL(showErrors(QModelIndex const&)),
        dataModel, SLOT(onShowErrors(QModelIndex const&)), Qt::UniqueConnection);

	connect(&view_, SIGNAL(itemSelected(const VLNV&)),
		this, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

	connect(dataModel, SIGNAL(invalidateFilter()), filter_, SLOT(invalidate()), Qt::UniqueConnection);
}

//void ProjectTreeWidget::OnTest(QModelIndex const& i)
//{

//}

//-----------------------------------------------------------------------------
// Function: ProjectTreeWidget::selectItem()
//-----------------------------------------------------------------------------
void ProjectTreeWidget::selectItem(VLNV const& vlnv)
{	
	// if vlnv is not valid
	if (!vlnv.isValid())
    {
		view_.clearSelection();
	}
	
	// find the item to be selected
    LibraryItem* item = dataModel_->getRoot()->findItem(vlnv);

	// If item is not found then print an error message telling user that library is corrupted.
	if (!item)
    {
//		emit errorMessage(tr("Selected item was not found, library is corrupted."));
		return;
	}

	// create an index to the item
	QModelIndex filteredIndex = filter_->mapFromSource(dataModel_->index(item));
	
	// tell view to select the item
	view_.setCurrentIndex(filteredIndex);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeWidget::getFilter()
//-----------------------------------------------------------------------------
LibraryFilter* ProjectTreeWidget::getFilter() const
{
    return filter_;
}
