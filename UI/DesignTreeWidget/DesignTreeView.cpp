//-----------------------------------------------------------------------------
// File: ProjectTreeExView.cpp
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.HG
// Date: 04.06.2024
//
// Description:
// The widget to display the library in a tree.
//-----------------------------------------------------------------------------

#include "ProjectTreeExView.h"

#include <KactusAPI//LibraryItem.h>
#include <KactusAPI/LibraryInterface.h>

#include <IPXACTmodels/common/VLNV.h>
#include <IPXACTmodels/common/Document.h>

#include <IPXACTmodels/Component/Component.h>

#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>

#include <QApplication>
#include <QDesktopServices>
#include <QDrag>
#include <QFileInfo>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::ProjectTreeExView()
//-----------------------------------------------------------------------------
ProjectTreeExView::ProjectTreeExView(LibraryInterface* handler, ProjectTreeExFilter* filter, QWidget* parent):
    ExpandingTreeView(parent),
    handler_(handler),
    filter_(filter),
    startPos_(),
    dragIndex_(),
    openHWDesignAction_(new QAction(tr("Open HW Design"), this)),
    openHWDesignMenu_(new QMenu(tr("Open HW Design"), this))
#if SWDESIGNMENU_SUPPORT_20231229 == 1
    , openSWDesignAction_(new QAction(tr("Open SW Design"), this))
#endif

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    , openMemoryDesignAction_(new QAction(tr("Open Memory Design"), this))
    , openMemoryDesignMenu_(new QMenu(tr("Open Memory Design"), this))
#endif

    , openComponentAction_(new QAction(tr("Open Component"), this))

#if NEWDESIGNMENU_SUPPORT_20231229 == 1
  , createNewDesignAction_(new QAction(tr("New HW Design..."), this))
#endif // #if NEWDESIGNMENU_SUPPORT_20231229 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    , createNewSWDesignAction_(new QAction(tr("New SW Design..."), this))
#endif

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    , createNewSystemDesignAction_(new QAction(tr("New System Design..."), this))
#endif

#if DELETEMENU_SUPPORT_20231229 == 1
    , deleteAction_(new QAction(tr("Delete Item..."), this))
#endif

#if EXPORTMENU_PRJ_SUPPORT_20240111 == 1
    , exportAction_(new QAction(tr("Export..."), this))
#endif

  #if IMPORTMENU_PRJ_SUPPORT_20240111 == 1
      , importAction_(new QAction(tr("Import..."), this))
  #endif

#if SHOWERRORSMENU_SUPPORT_20231229 == 1
    , showErrorsAction_(new QAction(tr("Show Errors..."), this))
#endif

#if ADDSIGNALSMENU_SUPPORT_20231229 == 1
    , addSignalsAction_(new QAction(tr("New Abstraction Definition..."), this))
#endif

#if BUSMENU_SUPPORT_20231229 == 1
    , openBusAction_(new QAction(tr("Open"), this))
#endif

#if CATALOGMENU_SUPPORT_20231229 == 1
    , openCatalogAction_(new QAction(tr("Open Catalog"), this))
#endif

#if COMDEFMENU_SUPPORT_20231229 == 1
    , openComDefAction_(new QAction(tr("Open"), this))
#endif

#if APIDEFMENU_SUPPORT_20231229 == 1
    , openApiDefAction_(new QAction(tr("Open"), this))
#endif

#if SYSTEMMENU_SUPPORT_20231229 == 1
    , openSystemAction_(new QAction(tr("Open System Design"), this))
#endif

#if XMLMENU_SUPPORT_20231229 == 1
    , openXmlAction_(new QAction(tr("Open XML File"), this))
#endif

#if CONTAININGFOLDERMENU_SUPPORT_20231229 == 1
    , openContainingFolderAction_(new QAction(tr("Open Containing Folder"), this))
#endif

#if EXPANDMENU_SUPPORT_20231229 == 1
    , expandChilds_(new QAction(tr("Expand Branches"), this))
    , collapseChilds_(new QAction(tr("Collapse Branches"), this))
    , expandAllAction_(new QAction(tr("Expand All"), this))
    , collapseAllAction_(new QAction(tr("Collapse All"), this))
#endif
{
        Q_ASSERT_X(filter, "ProjectTreeExView constructor", "Null filter pointer given");

    setIconSize(QSize(20, 20));

	setSortingEnabled(true);

	setSelectionBehavior(QAbstractItemView::SelectItems);
	setSelectionMode(QAbstractItemView::SingleSelection);

	// Items can not be edited.
	setEditTriggers(QAbstractItemView::NoEditTriggers);

    setupActions();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::contextMenuEvent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::contextMenuEvent(QContextMenuEvent* event)
{
#if LIBRARYTREE_CONTEXT_SUPPORT_20231229 == 1

    event->accept();


    QMenu menu(this);

    QModelIndex current = currentIndex();
    if (!current.isValid())
    {
#if IMPORTMENU_PRJ_SUPPORT_20240111 == 1
        menu.addAction(importAction_);

#if EXPORTMENU_PRJ_SUPPORT_20240111 == 1
        menu.addAction(exportAction_);
#endif

        menu.exec(event->globalPos());
#endif // #if IMPORTMENU_PRJ_SUPPORT_20240111 == 1

        return;
    }

//    QMenu menu(this);

    // Get original model index so internalPointer can be used.
    QModelIndex sourceIndex = filter_->mapToSource(current);
    LibraryItem::Level  level = static_cast<LibraryItem*>(sourceIndex.internalPointer())->getLevel();
    VLNV vlnv = static_cast<LibraryItem*>(sourceIndex.internalPointer())->getVLNV();

    if (vlnv.isValid())
    {
        QSharedPointer<Document const> document = handler_->getModelReadOnly(vlnv);

        if (document)
        {
            VLNV::IPXactType documentType = handler_->getDocumentType(vlnv);
            if (documentType == VLNV::COMPONENT)
            {
                QSharedPointer<Component const> component = document.staticCast<Component const>();

                menu.addAction(openComponentAction_);

                // depending on the type of the component
                if (component->getImplementation() == KactusAttribute::SYSTEM)
                {
                    if (component->hasSystemViews())
                    {
#if SYSTEMMENU_SUPPORT_20231229 == 1
                        menu.addAction(openSystemAction_);
#endif
                    }
                }
                else
                {
//                    QMenu* menuNew = menu.addMenu(tr("Add"));

                    if (component->getImplementation() == KactusAttribute::HW)
                    {
                        QStringList hierarchicalViewNames = component->getHierViews();
                        if (!hierarchicalViewNames.isEmpty())
                        {
                            openHWDesignMenu_->clear();

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
                            openMemoryDesignMenu_->clear();
#endif

                            for (QString const& viewName : hierarchicalViewNames)
                            {
                                openHWDesignMenu_->addAction(new QAction(viewName, openHWDesignMenu_));

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
                                openMemoryDesignMenu_->addAction(new QAction(viewName, openMemoryDesignMenu_));
#endif
                            }

                            if (hierarchicalViewNames.count() == 1)
                            {
                                menu.addAction(openHWDesignAction_);
                                connect(openHWDesignAction_, SIGNAL(triggered()),
                                    openHWDesignMenu_->actions().first(), SLOT(trigger()));

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
                                menu.addAction(openMemoryDesignAction_);
                                connect(openMemoryDesignAction_, SIGNAL(triggered()),
                                    openMemoryDesignMenu_->actions().first(), SLOT(trigger()));
#endif
                            }
                            else
                            {
                                menu.addMenu(openHWDesignMenu_);
#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
                                menu.addMenu(openMemoryDesignMenu_);
#endif
                            }
                        }

                        if (component->hasSystemViews())
                        {
#if SYSTEMMENU_SUPPORT_20231229 == 1
                            menu.addAction(openSystemAction_);
#endif
                        }

#if NEWDESIGNMENU_SUPPORT_20231229 == 1
                        menuNew->addAction(createNewDesignAction_);
#endif // #if NEWDESIGNMENU_SUPPORT_20231229 == 1

                        // Add New System Design action only if the component contains hierarchical HW views.
                        if (!component->getHierViews().isEmpty())
                        {
#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
                            menuNew->addAction(createNewSystemDesignAction_);
#endif
                        }
                    }

                    for (QSharedPointer<View> view : *component->getViews())
                    {
                        VLNV reference = component->getModel()->getHierRef(view->name());

                        QSharedPointer<const Document> document = handler_->getModelReadOnly(reference);

                        if (document && document->getImplementation() == KactusAttribute::SW)
                        {
#if SWDESIGNMENU_SUPPORT_20231229 == 1
                            menu.addAction(openSWDesignAction_);
#endif
                            break;
                        }
                    }

                    menu.addSeparator();

#if SWDESIGNMENU_SUPPORT_20231229 == 1
                    menuNew->addAction(createNewSWDesignAction_);
                    menu.addMenu(menuNew);
#endif

                }
            }

            else if (documentType == VLNV::ABSTRACTIONDEFINITION)
            {
#if BUSMENU_SUPPORT_20231229 == 1
                menu.addAction(openBusAction_);
#endif
            }

            else if (documentType == VLNV::BUSDEFINITION)
            {
#if BUSMENU_SUPPORT_20231229 == 1
                menu.addAction(openBusAction_);
#endif
                menu.addSeparator();

#if ADDSIGNALSMENU_SUPPORT_20231229 == 1
                QMenu* menuNew = menu.addMenu(tr("Add"));
                menuNew->addAction(addSignalsAction_);
                menu.addMenu(menuNew);
#endif
            }

            else if (documentType == VLNV::CATALOG)
            {
#if CATALOGMENU_SUPPORT_20231229 == 1
                menu.addAction(openCatalogAction_);
#endif
            }

            else if (documentType == VLNV::COMDEFINITION)
            {
#if COMDEFMENU_SUPPORT_20231229 == 1
                menu.addAction(openComDefAction_);
#endif
            }

            else if (documentType == VLNV::APIDEFINITION)
            {
#if APIDEFMENU_SUPPORT_20231229 == 1
                menu.addAction(openApiDefAction_);
#endif
            }
        }

        menu.addSeparator();

        if (!handler_->isValid(vlnv))
        {
#if SHOWERRORSMENU_SUPPORT_20231229 == 1
            menu.addAction(showErrorsAction_);
#endif
        }

#if CONTAININGFOLDERMENU_SUPPORT_20231229 == 1
        menu.addAction(openContainingFolderAction_);
#endif

#if XMLMENU_SUPPORT_20231229 == 1
        menu.addAction(openXmlAction_);
#endif

    }
    else {

    }

#if EXPORTMENU_PRJ_SUPPORT_20240111 == 1

    //    if (level == LibraryItem::Level::VENDOR) {

    //        if (!vlnv.isValid()) {
    //            menu.addAction(exportAction_);
    //        }

    //    }

#endif

#if DELETEMENU_SUPPORT_20231229 == 1
    menu.addAction(deleteAction_);
#endif

    menu.addSeparator();

#if EXPANDMENU_SUPPORT_20231229 == 1
    if (model()->rowCount(current) > 0)
    {
        menu.addAction(expandChilds_);
        menu.addAction(collapseChilds_);
    }
    menu.addAction(expandAllAction_);
    menu.addAction(collapseAllAction_);
#endif

    menu.exec(event->globalPos());
#endif

}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onDeleteAction()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onDeleteAction()
{	
	emit deleteItem(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onExportAction()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onExportAction()
{
//	emit exportItem(filter_->mapToSource(currentIndex()));
    emit exportItem();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onImportAction()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onImportAction()
{
    emit importItem();
}


//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::mouseDoubleClickEvent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::mouseDoubleClickEvent(QMouseEvent* event)
{
	// accept the event so it is not passed forwards
	event->accept();

	QModelIndex current = currentIndex();
	if (!current.isValid())
    {
		return;
	}

	emit openComponent(filter_->mapToSource(current));

	// let the default handler process the event
	QTreeView::mouseDoubleClickEvent(event);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::mousePressEvent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::mousePressEvent(QMouseEvent* event)
{
	startPos_ = event->pos();
	QModelIndex index = indexAt(startPos_);

    if (event->button() == Qt::LeftButton)
    {
        dragIndex_ = filter_->mapToSource(index);
    }

	setCurrentIndex(index);

    bool previouslyExpanded = isExpanded(index);
    QTreeView::mousePressEvent(event);

    // Change expand state if the click did not change it otherwise.
    if (event->button() == Qt::LeftButton && index.isValid() && (isExpanded(index) == previouslyExpanded))
    {
        setExpanded(index, !previouslyExpanded);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::mouseReleaseEvent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::mouseReleaseEvent(QMouseEvent* event)
{
	// find the clicked index
	QModelIndex index = indexAt(event->pos());
	if (index.isValid())
    {
		QModelIndex sourceIndex = filter_->mapToSource(index);
        LibraryItem* item = static_cast<LibraryItem*>(sourceIndex.internalPointer());

		// if item contains a single vlnv
        VLNV vlnv = item->getVLNV();
		if (vlnv.isValid())
        {
			emit itemSelected(vlnv);
		}
	}

	QTreeView::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::mouseMoveEvent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::mouseMoveEvent(QMouseEvent* event)
{
	// if left mouse button is pressed down
	if (event->buttons() & Qt::LeftButton)
    {
		int dragDistance = (event->pos() - startPos_).manhattanLength();

		// make sure the drag distance is large enough to start the drag.
		if (dragDistance >= QApplication::startDragDistance())
        {
            LibraryItem* item = static_cast<LibraryItem*>(dragIndex_.internalPointer());

            if (item != 0)
            {
			    VLNV vlnv = item->getVLNV();
    			
			    // if vlnv is valid
			    if (vlnv.isValid())
                {
				    QMimeData* mimeData = new QMimeData;
				    mimeData->setImageData(QVariant::fromValue(vlnv));

                    QDrag* drag = new QDrag(this);
				    drag->setMimeData(mimeData);
				    drag->exec(Qt::MoveAction | Qt::CopyAction | Qt::LinkAction);
			    }
            }
		}
	}
	QTreeView::mouseMoveEvent(event);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::setCurrentIndex()
//-----------------------------------------------------------------------------
void ProjectTreeExView::setCurrentIndex(QModelIndex const& index)
{
	// Expand the tree to full length all the way from the root.
	QModelIndex temp = index;

	while (temp.parent().isValid())
    {
		temp = temp.parent();
		expand(temp);
	}

	QTreeView::setCurrentIndex(index);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenDesign(QAction* viewAction)
{
	emit openDesign(filter_->mapToSource(currentIndex()), viewAction->text());
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenSWDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenSWDesign()
{
    emit openSWDesign(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenMemoryDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenMemoryDesign(QAction* viewAction)
{
    emit openMemoryDesign(filter_->mapToSource(currentIndex()), viewAction->text());
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenSystemDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenSystemDesign()
{
    emit openSystemDesign(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenComponent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenComponent()
{
	emit openComponent(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateComponent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateComponent()
{
	emit createNewComponent(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateDesign()
{
	emit createNewDesign(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateSWDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateSWDesign()
{
    emit createNewSWDesign(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateSystemDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateSystemDesign()
{
    emit createNewSystemDesign(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenBus()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenBus()
{
	emit openBus(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenCatalog()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenCatalog()
{
    emit openCatalog(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateBus()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateBus()
{
	emit createBus(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onAddSignals()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onAddSignals()
{
	emit createAbsDef(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenComDef()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenComDef()
{
    emit openComDef(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateComDef()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateComDef()
{
    emit createComDef(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenApiDef()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenApiDef()
{
    emit openApiDef(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateApiDef()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateApiDef()
{
    emit createApiDef(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onShowErrors()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onShowErrors()
{
    emit showErrors(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenXml()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenXml()
{
	QModelIndex index = filter_->mapToSource(currentIndex());
    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
	
    VLNV vlnv = item->getVLNV();
	if (vlnv.isValid())
    {
		// open the file in operating system's default editor
		QDesktopServices::openUrl(QUrl::fromLocalFile(handler_->getPath(vlnv)));
	}
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenContainingFolder()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenContainingFolder()
{
    QModelIndex index = filter_->mapToSource(currentIndex());
    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    
    VLNV vlnv = item->getVLNV();
    if (vlnv.isValid())
    {
        QString path = QFileInfo(handler_->getPath(vlnv)).absolutePath();

        // Open the folder in the operating system's default file browser.
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }    
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onExpandBranches()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onExpandBranches()
{
    QModelIndex index = currentIndex();

    setExpanded(index, true);
    setChildrenExpandStates(index, true);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCollapseBranches()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCollapseBranches()
{
    QModelIndex index = currentIndex();

    setChildrenExpandStates(index, false);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::setChildrenExpandStates()
//-----------------------------------------------------------------------------
void ProjectTreeExView::setChildrenExpandStates(QModelIndex index, bool expanded)
{
    if (index.isValid())
    {
        int childCount = model()->rowCount(index);
        for (int i = 0; i < childCount; i++)
        {
            QModelIndex childIndex = model()->index(i, 0, index);
            setExpanded(childIndex, expanded);
            setChildrenExpandStates(childIndex, expanded);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::setupActions()
//-----------------------------------------------------------------------------
void ProjectTreeExView::setupActions()
{
#if LIBRARYTREE_CONTEXT_SUPPORT_20231229 == 1
    openHWDesignMenu_->setStatusTip(tr("Open a HW design"));
    openHWDesignMenu_->setToolTip(tr("Open a HW design"));
    connect(openHWDesignMenu_, SIGNAL(triggered(QAction*)),	this, SLOT(onOpenDesign(QAction*)), Qt::UniqueConnection);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    openSWDesignAction_->setStatusTip(tr("Open a SW design"));
    openSWDesignAction_->setToolTip(tr("Open a SW design"));
    connect(openSWDesignAction_, SIGNAL(triggered()), this, SLOT(onOpenSWDesign()), Qt::UniqueConnection);
#endif

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    openMemoryDesignMenu_->setStatusTip(tr("Open a Memory design"));
    openMemoryDesignMenu_->setToolTip(tr("Open a Memory design"));
    connect(openMemoryDesignMenu_, SIGNAL(triggered(QAction*)),	this, SLOT(onOpenMemoryDesign(QAction*)),
        Qt::UniqueConnection);
#endif

    openComponentAction_->setStatusTip(tr("Open component editor"));
    openComponentAction_->setToolTip(tr("Open component editor"));
    connect(openComponentAction_, SIGNAL(triggered()), this, SLOT(onOpenComponent()), Qt::UniqueConnection);

#if NEWDESIGNMENU_SUPPORT_20231229 == 1
    createNewDesignAction_->setStatusTip(tr("Create new HW design"));
    createNewDesignAction_->setToolTip(tr("Create new HW design"));
    connect(createNewDesignAction_, SIGNAL(triggered()), this, SLOT(onCreateDesign()), Qt::UniqueConnection);
#endif // #if NEWDESIGNMENU_SUPPORT_20231229 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    createNewSWDesignAction_->setStatusTip(tr("Create new SW design"));
    createNewSWDesignAction_->setToolTip(tr("Create new SW design"));
    connect(createNewSWDesignAction_, SIGNAL(triggered()), this, SLOT(onCreateSWDesign()), Qt::UniqueConnection);
#endif

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    createNewSystemDesignAction_->setStatusTip(tr("Create new system design"));
    createNewSystemDesignAction_->setToolTip(tr("Create new system design"));
    connect(createNewSystemDesignAction_, SIGNAL(triggered()),
        this, SLOT(onCreateSystemDesign()), Qt::UniqueConnection);
#endif

#if DELETEMENU_SUPPORT_20231229 == 1
    deleteAction_->setStatusTip(tr("Delete item from the library"));
    deleteAction_->setToolTip(tr("Delete the item from the library"));
    connect(deleteAction_, SIGNAL(triggered()),	this, SLOT(onDeleteAction()), Qt::UniqueConnection);
#endif

#if EXPORTMENU_PRJ_SUPPORT_20240111 == 1
    exportAction_->setStatusTip(tr("Export item and it's sub-items to another location"));
    exportAction_->setToolTip(tr("Export item and it's sub-items to another location"));
    connect(exportAction_, SIGNAL(triggered()), this, SLOT(onExportAction()), Qt::UniqueConnection);
#endif

#if IMPORTMENU_PRJ_SUPPORT_20240111 == 1
    importAction_->setStatusTip(tr("Import item and it's sub-items to another location"));
    importAction_->setToolTip(tr("Import item and it's sub-items to another location"));
    connect(importAction_, SIGNAL(triggered()), this, SLOT(onImportAction()), Qt::UniqueConnection);
#endif


#if SHOWERRORSMENU_SUPPORT_20231229 == 1
    showErrorsAction_->setStatusTip(tr("Show all errors of the item"));
    showErrorsAction_->setToolTip(tr("Show all errors of the item"));
    connect(showErrorsAction_, SIGNAL(triggered()), this, SLOT(onShowErrors()), Qt::UniqueConnection);
#endif

#if BUSMENU_SUPPORT_20231229 == 1
    openBusAction_->setStatusTip(tr("Open"));
    openBusAction_->setToolTip(tr("Open"));
    connect(openBusAction_, SIGNAL(triggered()), this, SLOT(onOpenBus()), Qt::UniqueConnection);
#endif

#if CATALOGMENU_SUPPORT_20231229 == 1
    openCatalogAction_->setStatusTip(tr("Open a catalog"));
    openCatalogAction_->setToolTip(tr("Open a catalog"));
    connect(openCatalogAction_, SIGNAL(triggered()), this, SLOT(onOpenCatalog()), Qt::UniqueConnection);
#endif

#if ADDSIGNALSMENU_SUPPORT_20231229 == 1
    addSignalsAction_->setStatusTip(tr("Create new abstraction definition for the bus"));
    addSignalsAction_->setToolTip(tr("Create new abstraction definition for the bus"));
    connect(addSignalsAction_, SIGNAL(triggered()),	this, SLOT(onAddSignals()), Qt::UniqueConnection);
#endif

#if COMDEFMENU_SUPPORT_20231229 == 1
    openComDefAction_->setStatusTip(tr("Open"));
    openComDefAction_->setToolTip(tr("Open"));
    connect(openComDefAction_, SIGNAL(triggered()), this, SLOT(onOpenComDef()), Qt::UniqueConnection);
#endif

#if APIDEFMENU_SUPPORT_20231229 == 1
    openApiDefAction_->setStatusTip(tr("Open"));
    openApiDefAction_->setToolTip(tr("Open"));
    connect(openApiDefAction_, SIGNAL(triggered()), this, SLOT(onOpenApiDef()), Qt::UniqueConnection);
#endif

#if SYSTEMMENU_SUPPORT_20231229 == 1
    openSystemAction_->setStatusTip(tr("Open system design for editing"));
    openSystemAction_->setToolTip(tr("Open system design for editing"));
    connect(openSystemAction_, SIGNAL(triggered()),	this, SLOT(onOpenSystemDesign()), Qt::UniqueConnection);
#endif

#if XMLMENU_SUPPORT_20231229 == 1
    connect(openXmlAction_, SIGNAL(triggered()), this, SLOT(onOpenXml()), Qt::UniqueConnection);
#endif

#if CONTAININGFOLDERMENU_SUPPORT_20231229 == 1
    connect(openContainingFolderAction_, SIGNAL(triggered()),
        this, SLOT(onOpenContainingFolder()), Qt::UniqueConnection);
#endif

#if EXPANDMENU_SUPPORT_20231229 == 1
    connect(expandChilds_, SIGNAL(triggered()), this, SLOT(onExpandBranches()), Qt::UniqueConnection);

    connect(collapseChilds_, SIGNAL(triggered()), this, SLOT(onCollapseBranches()), Qt::UniqueConnection);

    connect(expandAllAction_, SIGNAL(triggered()), this, SLOT(expandAll()), Qt::UniqueConnection);

    connect(collapseAllAction_, SIGNAL(triggered()), this, SLOT(collapseAll()), Qt::UniqueConnection);
#endif  // EXPANDMENU_SUPPORT_20231229

#endif  // LIBRARYTREE_CONTEXT_SUPPORT_20231229

}
