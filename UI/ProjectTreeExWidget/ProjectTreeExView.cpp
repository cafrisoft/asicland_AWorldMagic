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

#include <KactusAPI/LibraryItem.h>
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
    openComponentAction_(new QAction(tr("Open Component"), this)),
    openHWDesignAction_(new QAction(tr("Open HW Design"), this)),
    openHWDesignMenu_(new QMenu(tr("Open HW Design"), this))
#if SWDESIGNMENU_SUPPORT_20231229 == 1
    , openSWDesignAction_(new QAction(tr("Open SW Design"), this))
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    , openMemoryDesignAction_(new QAction(tr("Open Memory Design"), this))
    , openMemoryDesignMenu_(new QMenu(tr("Open Memory Design"), this))
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

//  , createNewComponentAction_(new QAction(tr("New HW Component"), this))

#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
  , createNewDesignAction_(new QAction(tr("New Design..."), this))
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    , createNewSWDesignAction_(new QAction(tr("New SW Design..."), this))
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    , createNewSystemDesignAction_(new QAction(tr("New System Design..."), this))
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if DELETEMENU_SUPPORT_20231229 == 1
    , deleteAction_(new QAction(tr("Delete Item..."), this))
#endif // DELETEMENU_SUPPORT_20231229

#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
    , exportAction_(new QAction(tr("Export..."), this))
#endif // PROJECT_EXPORT_MENU_SUPPORT_20240618

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
      , importAction_(new QAction(tr("Import..."), this))
#endif // PROJECT_IMPORT_MENU_SUPPORT_20240618

#if SHOWERRORSMENU_SUPPORT_20231229 == 1
    , showErrorsAction_(new QAction(tr("Show Errors..."), this))
#endif

#if SYSTEMMENU_SUPPORT_20231229 == 1
    , openSystemAction_(new QAction(tr("Open System Design"), this))
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if XMLMENU_SUPPORT_20231229 == 1
    , openXmlAction_(new QAction(tr("Open XML File"), this))
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
#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
        menu.addAction(createNewDesignAction_);
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
        menu.addAction(importAction_);
#endif // #if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1

#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
        menu.addAction(exportAction_);
#endif // PROJECT_EXPORT_MENU_SUPPORT_20240618

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1 || PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1 || NEW_DESIGN_MENU_SUPPORT_20240618 == 1
        menu.exec(event->globalPos());
#endif // #if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1 || PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1 || NEW_DESIGN_MENU_SUPPORT_20240618 == 1

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
#endif // MEMORYDESIGNMENU_SUPPORT_20231229
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
#endif // MEMORYDESIGNMENU_SUPPORT_20231229
                        }

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
#endif // SWDESIGNMENU_SUPPORT_20231229
                            break;
                        }
                    }

                    menu.addSeparator();

#if SWDESIGNMENU_SUPPORT_20231229 == 1
                    menuNew->addAction(createNewSWDesignAction_);
                    menu.addMenu(menuNew);
#endif // SWDESIGNMENU_SUPPORT_20231229

                }
            }
        }

        menu.addSeparator();

        if (!handler_->isValid(vlnv))
        {
#if SHOWERRORSMENU_SUPPORT_20231229 == 1
            menu.addAction(showErrorsAction_);
#endif
        }

#if XMLMENU_SUPPORT_20231229 == 1
        menu.addAction(openXmlAction_);
#endif

    }
    else {

    }

#if DELETEMENU_SUPPORT_20231229 == 1
    menu.addAction(deleteAction_);
#endif // DELETEMENU_SUPPORT_20231229

    menu.addSeparator();

    menu.exec(event->globalPos());
#endif

}

#if DELETEMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onDeleteAction()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onDeleteAction()
{
    emit deleteItem(filter_->mapToSource(currentIndex()));
}
#endif // DELETEMENU_SUPPORT_20231229

#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onExportAction()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onExportAction()
{
//	emit exportItem(filter_->mapToSource(currentIndex()));
    emit exportItem();
}
#endif // #if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onImportAction()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onImportAction()
{
    emit importItem();
}
#endif // #if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1

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
// Function: ProjectTreeExView::onOpenComponent()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenComponent()
{
    emit openComponent(filter_->mapToSource(currentIndex()));
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenDesign(QAction* viewAction)
{
	emit openDesign(filter_->mapToSource(currentIndex()), viewAction->text());
}

#if SWDESIGNMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenSWDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenSWDesign()
{
    emit openSWDesign(filter_->mapToSource(currentIndex()));
}
#endif // SWDESIGNMENU_SUPPORT_20231229

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenMemoryDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenMemoryDesign(QAction* viewAction)
{
    emit openMemoryDesign(filter_->mapToSource(currentIndex()), viewAction->text());
}
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onOpenSystemDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onOpenSystemDesign()
{
    emit openSystemDesign(filter_->mapToSource(currentIndex()));
}
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateComponent()
//-----------------------------------------------------------------------------
//void ProjectTreeExView::onCreateComponent()
//{
//	emit createNewComponent(filter_->mapToSource(currentIndex()));
//}

#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateDesign()
{
    emit createNewDesign();
}
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateSWDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateSWDesign()
{
    emit createNewSWDesign(filter_->mapToSource(currentIndex()));
}
#endif // SWDESIGNMENU_SUPPORT_20231229

//------
#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------
// Function: ProjectTreeExView::onCreateSystemDesign()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onCreateSystemDesign()
{
    emit createNewSystemDesign(filter_->mapToSource(currentIndex()));
}
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if XMLMENU_SUPPORT_20231229 == 1
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
#endif

#if SHOWERRORSMENU_SUPPORT_20231229 == 1
//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::onShowErrors()
//-----------------------------------------------------------------------------
void ProjectTreeExView::onShowErrors()
{
    emit showErrors(filter_->mapToSource(currentIndex()));
}
#endif


//-----------------------------------------------------------------------------
// Function: ProjectTreeExView::setupActions()
//-----------------------------------------------------------------------------
void ProjectTreeExView::setupActions()
{
    openComponentAction_->setStatusTip(tr("Open component"));
    openComponentAction_->setToolTip(tr("Open component"));
    connect(openComponentAction_, SIGNAL(triggered()), this, SLOT(onOpenComponent()), Qt::UniqueConnection);

#if LIBRARYTREE_CONTEXT_SUPPORT_20231229 == 1
    openHWDesignMenu_->setStatusTip(tr("Open a HW design"));
    openHWDesignMenu_->setToolTip(tr("Open a HW design"));
    connect(openHWDesignMenu_, SIGNAL(triggered(QAction*)),	this, SLOT(onOpenDesign(QAction*)), Qt::UniqueConnection);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    openSWDesignAction_->setStatusTip(tr("Open a SW design"));
    openSWDesignAction_->setToolTip(tr("Open a SW design"));
    connect(openSWDesignAction_, SIGNAL(triggered()), this, SLOT(onOpenSWDesign()), Qt::UniqueConnection);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    openMemoryDesignMenu_->setStatusTip(tr("Open a Memory design"));
    openMemoryDesignMenu_->setToolTip(tr("Open a Memory design"));
    connect(openMemoryDesignMenu_, SIGNAL(triggered(QAction*)),	this, SLOT(onOpenMemoryDesign(QAction*)),
        Qt::UniqueConnection);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SYSTEMMENU_SUPPORT_20231229 == 1
    openSystemAction_->setStatusTip(tr("Open system design for editing"));
    openSystemAction_->setToolTip(tr("Open system design for editing"));
    connect(openSystemAction_, SIGNAL(triggered()),	this, SLOT(onOpenSystemDesign()), Qt::UniqueConnection);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

//    createNewComponentAction_->setStatusTip(tr("Create new HW component"));
//	createNewComponentAction_->setToolTip(tr("Create new HW component"));
//	connect(createNewComponentAction_, SIGNAL(triggered()), this, SLOT(onCreateComponent()), Qt::UniqueConnection);

#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
    createNewDesignAction_->setStatusTip(tr("Create new design"));
    createNewDesignAction_->setToolTip(tr("Create new design"));
    connect(createNewDesignAction_, SIGNAL(triggered()), this, SLOT(onCreateDesign()), Qt::UniqueConnection);
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    createNewSWDesignAction_->setStatusTip(tr("Create new SW design"));
    createNewSWDesignAction_->setToolTip(tr("Create new SW design"));
    connect(createNewSWDesignAction_, SIGNAL(triggered()), this, SLOT(onCreateSWDesign()), Qt::UniqueConnection);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    createNewSystemDesignAction_->setStatusTip(tr("Create new system design"));
    createNewSystemDesignAction_->setToolTip(tr("Create new system design"));
    connect(createNewSystemDesignAction_, SIGNAL(triggered()),
        this, SLOT(onCreateSystemDesign()), Qt::UniqueConnection);
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if DELETEMENU_SUPPORT_20231229 == 1
    deleteAction_->setStatusTip(tr("Delete item from the library"));
    deleteAction_->setToolTip(tr("Delete the item from the library"));
    connect(deleteAction_, SIGNAL(triggered()),	this, SLOT(onDeleteAction()), Qt::UniqueConnection);
#endif // DELETEMENU_SUPPORT_20231229

#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
    exportAction_->setStatusTip(tr("Export item and it's sub-items to another location"));
    exportAction_->setToolTip(tr("Export item and it's sub-items to another location"));
    connect(exportAction_, SIGNAL(triggered()), this, SLOT(onExportAction()), Qt::UniqueConnection);
#endif // PROJECT_EXPORT_MENU_SUPPORT_20240618

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
    importAction_->setStatusTip(tr("Import item and it's sub-items to another location"));
    importAction_->setToolTip(tr("Import item and it's sub-items to another location"));
    connect(importAction_, SIGNAL(triggered()), this, SLOT(onImportAction()), Qt::UniqueConnection);
#endif // PROJECT_IMPORT_MENU_SUPPORT_20240618

#if SHOWERRORSMENU_SUPPORT_20231229 == 1
    showErrorsAction_->setStatusTip(tr("Show all errors of the item"));
    showErrorsAction_->setToolTip(tr("Show all errors of the item"));
    connect(showErrorsAction_, SIGNAL(triggered()), this, SLOT(onShowErrors()), Qt::UniqueConnection);
#endif

#if XMLMENU_SUPPORT_20231229 == 1
    connect(openXmlAction_, SIGNAL(triggered()), this, SLOT(onOpenXml()), Qt::UniqueConnection);
#endif


#endif  // LIBRARYTREE_CONTEXT_SUPPORT_20231229

}
