//-----------------------------------------------------------------------------
// File: ProjectTreeExWidget.cpp
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.HG
// Date: 04.06.2024
//
// Description:
// ProjectTreeExWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

#include <QObject>
#include <QVBoxLayout>

#include <KactusAPI/ProjectTreeExModel.h>

#include "ProjectTreeExFilter.h"
#include "ProjectTreeExView.h"
#include "ProjectTreeExWidget.h"

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::ProjectTreeExWidget()
//-----------------------------------------------------------------------------
ProjectTreeExWidget::ProjectTreeExWidget(LibraryInterface* handler, ProjectTreeExModel* projectTreeExModel, QWidget* parent):
    QTabWidget(parent),
//QWidget(parent),
    projectTreeExFilter_(new ProjectTreeExFilter(handler, this)),
    projectTreeExView_(handler, projectTreeExFilter_, this),
    projectTreeExModel_(projectTreeExModel)
{
    QWidget *designTab = new QWidget;
    QWidget *libraryTab = new QWidget;

    projectTreeExFilter_->setSourceModel(projectTreeExModel);

    // set view to use ProjectTreeExFilter as source model
    projectTreeExView_.setModel(projectTreeExFilter_);

	// the layout to manage the visible items in the widget
    QVBoxLayout* designLayout = new QVBoxLayout(this);
    designLayout->addWidget(&projectTreeExView_);
    designLayout->setContentsMargins(0, 0, 0, 0);

    designTab->setLayout(designLayout);

    addTab(designTab, tr("Design"));
    addTab(libraryTab, tr("Library"));

    setupProjectTreeConnections(projectTreeExModel);

    projectTreeExView_.sortByColumn(0, Qt::AscendingOrder);
    projectTreeExView_.setHeaderHidden(true);


//    addTreeViewTab("Tab 1", model1);
//    addTreeViewTab("Tab 2", model2);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::~ProjectTreeExWidget()
//-----------------------------------------------------------------------------
ProjectTreeExWidget::~ProjectTreeExWidget()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::setupProjectTreeConnections()
//-----------------------------------------------------------------------------
void ProjectTreeExWidget::setupProjectTreeConnections(ProjectTreeExModel* dataModel)
{
    connect(&projectTreeExView_, SIGNAL(errorMessage(QString const&)),
        this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(noticeMessage(QString const&)),
		this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);

	// connect the view to the tree model
    connect(&projectTreeExView_, SIGNAL(openComponent(QModelIndex const&)),
		    dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openDesign(QModelIndex const&, QString const&)),
		    dataModel, SLOT(onOpenDesign(QModelIndex const&, QString const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openMemoryDesign(const QModelIndex&, QString const&)),
        dataModel, SLOT(onOpenMemoryDesign(const QModelIndex&, QString const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openSWDesign(QModelIndex const&)),
            dataModel, SLOT(onOpenSWDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openSystemDesign(QModelIndex const&)),
        dataModel, SLOT(onOpenSystemDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createNewComponent(QModelIndex const&)),
		    dataModel, SLOT(onCreateNewComponent(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createNewDesign(QModelIndex const&)),
		    dataModel, SLOT(onCreateNewDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createNewSWDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewSWDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createNewSystemDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewSystemDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openBus(QModelIndex const&)),
		    dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openCatalog(QModelIndex const&)),
        dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createBus(QModelIndex const&)),
		    dataModel, SLOT(onCreateBus(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createAbsDef(QModelIndex const&)),
		    dataModel, SLOT(onCreateAbsDef(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openComDef(QModelIndex const&)),
            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createComDef(QModelIndex const&)),
            dataModel, SLOT(onCreateComDef(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(openApiDef(QModelIndex const&)),
            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(createApiDef(QModelIndex const&)),
            dataModel, SLOT(onCreateApiDef(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(deleteItem(QModelIndex const&)),
		    dataModel, SLOT(onDeleteItem(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&projectTreeExView_, SIGNAL(exportItem(QModelIndex const&)),
//		dataModel, SLOT(onExportItem(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(exportItem()), this, SIGNAL(exportItem()), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(importItem()), this, SIGNAL(importItem()), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(showErrors(QModelIndex const&)),
        dataModel, SLOT(onShowErrors(QModelIndex const&)), Qt::UniqueConnection);

    connect(&projectTreeExView_, SIGNAL(itemSelected(const VLNV&)),
            this, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(dataModel, SIGNAL(invalidateFilter()), projectTreeExFilter_, SLOT(invalidate()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::selectItem()
//-----------------------------------------------------------------------------
void ProjectTreeExWidget::selectItem(VLNV const& vlnv)
{	
	// if vlnv is not valid
	if (!vlnv.isValid())
    {
        projectTreeExView_.clearSelection();
	}
	
	// find the item to be selected
    LibraryItem* item = projectTreeExModel_->getRoot()->findItem(vlnv);

	// If item is not found then print an error message telling user that library is corrupted.
	if (!item)
    {
		emit errorMessage(tr("Selected item was not found, library is corrupted."));
		return;
	}

	// create an index to the item
    QModelIndex filteredIndex = projectTreeExFilter_->mapFromSource(projectTreeExModel_->index(item));
	
	// tell view to select the item
    projectTreeExView_.setCurrentIndex(filteredIndex);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::getFilter()
//-----------------------------------------------------------------------------
LibraryFilter* ProjectTreeExWidget::getFilter() const
{
    return projectTreeExFilter_;
}
