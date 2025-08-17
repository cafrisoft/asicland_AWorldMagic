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
ProjectTreeExWidget::ProjectTreeExWidget(LibraryInterface* handler,
                                         ProjectTreeExModel* projectTreeExModel,
//                                         ProjectTreeExModel* projectTreeExModel,
                                         QWidget* parent):
//    QTabWidget(parent),
QWidget(parent),
    filter_(new ProjectTreeExFilter(handler, this)),
    view_(handler, filter_, this),
    model_(projectTreeExModel)
{
    filter_->setSourceModel(projectTreeExModel);

    // set view to use ProjectTreeExFilter as source model
    view_.setModel(filter_);

	// the layout to manage the visible items in the widget
    QVBoxLayout* designLayout = new QVBoxLayout(this);
    designLayout->addWidget(&view_);
    designLayout->setContentsMargins(0, 0, 0, 0);

    setupProjectTreeConnections(projectTreeExModel);

    view_.sortByColumn(0, Qt::AscendingOrder);
    view_.setHeaderHidden(true);
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
    connect(&view_, SIGNAL(errorMessage(QString const&)),
        this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(noticeMessage(QString const&)),
		this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(showErrors(QModelIndex const&)),
        dataModel, SLOT(onShowErrors(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(itemSelected(const VLNV&)),
            this, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(dataModel, SIGNAL(invalidateFilter()), filter_, SLOT(invalidate()), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openDesign(QModelIndex const&, QString const&)),
		    dataModel, SLOT(onOpenDesign(QModelIndex const&, QString const&)), Qt::UniqueConnection);

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    connect(&view_, SIGNAL(openMemoryDesign(const QModelIndex&, QString const&)),
        dataModel, SLOT(onOpenMemoryDesign(const QModelIndex&, QString const&)), Qt::UniqueConnection);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    connect(&view_, SIGNAL(openSWDesign(QModelIndex const&)),
            dataModel, SLOT(onOpenSWDesign(QModelIndex const&)), Qt::UniqueConnection);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMMENU_SUPPORT_20231229 == 1
    connect(&view_, SIGNAL(openSystemDesign(QModelIndex const&)),
        dataModel, SLOT(onOpenSystemDesign(QModelIndex const&)), Qt::UniqueConnection);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

    // connect the view to the tree model
    connect(&view_, SIGNAL(openComponent(QModelIndex const&)),
            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(createNewComponent(QModelIndex const&)),
//		    dataModel, SLOT(onCreateNewComponent(QModelIndex const&)), Qt::UniqueConnection);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    connect(&view_, SIGNAL(createNewSWDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewSWDesign(QModelIndex const&)), Qt::UniqueConnection);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    connect(&view_, SIGNAL(createNewSystemDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewSystemDesign(QModelIndex const&)), Qt::UniqueConnection);
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if DELETEMENU_SUPPORT_20231229 == 1
    connect(&view_, SIGNAL(deleteItem(QModelIndex const&)),
            dataModel, SLOT(onDeleteItem(QModelIndex const&)), Qt::UniqueConnection);
#endif // DELETEMENU_SUPPORT_20231229

    connect(&view_, SIGNAL(createNewDesign()), this, SIGNAL(createNewDesign()), Qt::UniqueConnection);

    connect(&view_, SIGNAL(exportItem()), this, SIGNAL(exportItem()), Qt::UniqueConnection);

    connect(&view_, SIGNAL(importItem()), this, SIGNAL(importItem()), Qt::UniqueConnection);

}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::selectItem()
//-----------------------------------------------------------------------------
void ProjectTreeExWidget::selectItem(VLNV const& vlnv)
{	
	// if vlnv is not valid
	if (!vlnv.isValid())
    {
        view_.clearSelection();
	}
	
	// find the item to be selected
    LibraryItem* item = model_->getRoot()->findItem(vlnv);

	// If item is not found then print an error message telling user that library is corrupted.
	if (!item)
    {
//		emit errorMessage(tr("Selected item was not found, library is corrupted."));
		return;
	}

	// create an index to the item
    QModelIndex filteredIndex = filter_->mapFromSource(model_->index(item));
	
	// tell view to select the item
    view_.setCurrentIndex(filteredIndex);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExWidget::getFilter()
//-----------------------------------------------------------------------------
LibraryFilter* ProjectTreeExWidget::getFilter() const
{
    return filter_;
}
