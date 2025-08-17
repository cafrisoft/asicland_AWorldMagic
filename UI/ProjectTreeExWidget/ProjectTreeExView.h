//-----------------------------------------------------------------------------
// File: ProjectTreeExView.h
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.HG
// Date: 04.06.2024
//
// Description:
// The widget to display the library in a tree.
//-----------------------------------------------------------------------------

#ifndef PROJECTTREEEXVIEW_H
#define PROJECTTREEEXVIEW_H

#ifndef NEW_DESIGN_MENU_SUPPORT_20240618
#define NEW_DESIGN_MENU_SUPPORT_20240618  1
#endif // NEW_DESIGN_MENU_SUPPORT_20240618

#ifndef PROJECT_EXPORT_MENU_SUPPORT_20240618
#define PROJECT_EXPORT_MENU_SUPPORT_20240618  1
#endif // PROJECT_EXPORT_MENU_SUPPORT_20240618

#ifndef PROJECT_IMPORT_MENU_SUPPORT_20240618
#define PROJECT_IMPORT_MENU_SUPPORT_20240618  1
#endif // PROJECT_IMPORT_MENU_SUPPORT_20240618

#include "ProjectTreeExFilter.h"

#include <UI/Views/ExpandingTreeView/ExpandingTreeView.h>

#include <QTreeView>
#include <QContextMenuEvent>
#include <QAction>
#include <QPoint>
#include <QModelIndex>
#include <QMouseEvent>

class LibraryInterface;

//-----------------------------------------------------------------------------
//! The widget to display the library in a tree.
//-----------------------------------------------------------------------------
class ProjectTreeExView : public ExpandingTreeView
{
	Q_OBJECT

public:

	/*! The constructor
	* 
	*      @param [in] handler  The instance that manages the library.
	*      @param [in] filter   The class that does the filtering for the view.
	*      @param [in] parent   The owner of this widget.
	*
	*/
        ProjectTreeExView(LibraryInterface* handler, ProjectTreeExFilter* filter, QWidget* parent = 0);

	//! The destructor.
        virtual ~ProjectTreeExView() = default;

signals:
        //! Open the given object in the component editor
        void openComponent(QModelIndex const& index);

        //! Open the selected hierarchical design
        void openDesign(QModelIndex const& index, QString const& viewName);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the SW design of a component
    void openSWDesign(QModelIndex const& index);
#endif // #if SWDESIGNMENU_SUPPORT_20231229 == 1

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the Memory design of a HW design.
    void openMemoryDesign(QModelIndex const& index, QString const& viewName);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SYSTEMMENU_SUPPORT_20231229 == 1
    //! Open the system design of a component
    void openSystemDesign(QModelIndex const& index);
#endif // SYSTEMMENU_SUPPORT_20231229

	//! Create new component
	void createNewComponent(QModelIndex const& index);

#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
    //! Create new design
    void createNewDesign();
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new SW design
    void createNewSWDesign(QModelIndex const& index);
#endif // #if SWDESIGNMENU_SUPPORT_20231229 == 1

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new system design
    void createNewSystemDesign(QModelIndex const& index);
#endif // #if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1

#if DELETEMENU_SUPPORT_20231229 == 1
    //! Delete the selected item
    void deleteItem(QModelIndex const& index);
#endif // DELETEMENU_SUPPORT_20231229

#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
    //! Export the selected item and it's sub-items.
//	void exportItem(QModelIndex const& index);
    void exportItem();
#endif // #if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
    //! Import the selected item and it's sub-items.
    void importItem();
#endif // #if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1

	//! Shows errors about the selected item.
	void showErrors(QModelIndex const& index);

	//! A drag operation has been initiated.
	void dragInitiated(QModelIndex const& index);

	//! Emitted when a library item identifying single vlnv is selected.
	void itemSelected(const VLNV& vlnv);

	//! Send error message to user.
	void errorMessage(QString const& message);

	//! Send notification to user.
	void noticeMessage(QString const& message);

public slots:

    //! Open component editor
    void onOpenComponent();

    //! Open a hierarchical component
    void onOpenDesign(QAction* viewAction);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the SW design of a component
    void onOpenSWDesign();
#endif // SWDESIGNMENU_SUPPORT_20231229

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the Memory design of a HW design.
    void onOpenMemoryDesign(QAction* viewAction);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the system design of a component
    void onOpenSystemDesign();
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

    //! Create new component
//    void onCreateComponent();

#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
    //! Create new design
    void onCreateDesign();
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new SW design
    void onCreateSWDesign();
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new SW design
    void onCreateSystemDesign();
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if DELETEMENU_SUPPORT_20231229 == 1
    //! Called when user selects "Delete item" in contextMenu
    void onDeleteAction();
#endif // DELETEMENU_SUPPORT_20231229


#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
    //! Called when user selects "Export item and sub-items" in contextMenu
    void onExportAction();
#endif // #if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
    //! Called when user selects "Import item and sub-items" in contextMenu
    void onImportAction();
#endif // #if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1

#if XMLMENU_SUPPORT_20231229 == 1
    //! Open the xml of the selected IP-Xact element.
    void onOpenXml();
#endif

#if SHOWERRORSMENU_SUPPORT_20231229 == 1
    //! Called when the user chooses Show Errors from the context menu.
    void onShowErrors();
#endif

    /*! Set the index to be the current index and select and expand it.
    *
    *      @param [in] index Specified the index to be set as the current index.
    */
    virtual void setCurrentIndex(QModelIndex const& index);

protected:

    //! The event handler for mouse double clicks
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

    //! The event handler for mousePress events
    virtual void mousePressEvent(QMouseEvent *event);

    //! The event handler for mouseMove events
    virtual void mouseMoveEvent(QMouseEvent *event);

    //! The event handler for mouse release events
    virtual void mouseReleaseEvent(QMouseEvent* event);

    /*! Handler for the context menu events.
    *
    *      @param [in] event The contextMenuEvent to be handled.
    */
    virtual void contextMenuEvent(QContextMenuEvent* event);

private slots:

private:

    //! No assignment
        ProjectTreeExView(const ProjectTreeExView& other);

	//! No copying
        ProjectTreeExView& operator=(const ProjectTreeExView& other);

    //! Expands/collapses all child indexes recursively.
    void setChildrenExpandStates(QModelIndex index, bool expanded);

    //! Set up the actions for the context menu
    void setupActions();

	//! The instance that gives the xml objects.
	LibraryInterface* handler_;

    //! The filter that is used to transform model indexes
    ProjectTreeExFilter* filter_;

	//! The starting position for drag
	QPoint startPos_;

	//! The index that is selected to be dragged
	QModelIndex dragIndex_;

	/**********************************************************************/
	// The actions for the contextMenu

    //! Open the component editor
    QAction* openComponentAction_;

	//! Open the hierarchical design of a component. Single action if only one view available, otherwise a menu.
    QAction* openHWDesignAction_;
	QMenu* openHWDesignMenu_;

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the SW design of a component
    QAction* openSWDesignAction_;
#endif

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    //! Action for opening a memory design. Single action if only one view is available, otherwise a menu.
    QAction* openMemoryDesignAction_;
    QMenu* openMemoryDesignMenu_;
#endif

    //! Create a new empty object, only copy the vlnv structure.
//	QAction* createNewComponentAction_;

#if NEW_DESIGN_MENU_SUPPORT_20240618 == 1
    //! Create new design for a component
    QAction* createNewDesignAction_;
#endif // #if NEW_DESIGN_MENU_SUPPORT_20240618 == 1

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new SW design for a component.
    QAction* createNewSWDesignAction_;
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new system design for a component.
    QAction* createNewSystemDesignAction_;
#endif

#if DELETEMENU_SUPPORT_20231229 == 1
    //! Delete the item from the library
    QAction* deleteAction_;
#endif // DELETEMENU_SUPPORT_20231229

#if PROJECT_EXPORT_MENU_SUPPORT_20240618 == 1
    //! Export the item(s) to a new location in file system
    QAction* exportAction_;
#endif // PROJECT_EXPORT_MENU_SUPPORT_20240618

#if PROJECT_IMPORT_MENU_SUPPORT_20240618 == 1
    //! Export the item(s) to a new location in file system
    QAction* importAction_;
#endif // PROJECT_IMPORT_MENU_SUPPORT_20240618

#if SHOWERRORSMENU_SUPPORT_20231229 == 1
    //! Show errors about the item.
    QAction* showErrorsAction_;
#endif

#if SYSTEMMENU_SUPPORT_20231229 == 1
    //! Open an existing system for editing.
    QAction* openSystemAction_;
#endif

#if XMLMENU_SUPPORT_20231229 == 1
    //! Open the xml file to be viewed by the user.
    QAction* openXmlAction_;
#endif

};

#endif // PROJECTTREEEXVIEW_H
