//-----------------------------------------------------------------------------
// File: ProjectLibraryTreeWidget.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 04.06.2024
//
// Description:
// ProjectLibraryTreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

#ifndef PROJECTLIBRARYTREEWIDGET_H
#define PROJECTLIBRARYTREEWIDGET_H

#include <QWidget>

#include <IPXACTmodels/common/VLNV.h>

#include "ProjectLibraryTreeView.h"

class LibraryInterface;
class ProjectLibraryTreeModel;
class ProjectLibraryTreeFilter;
class ProjectLibraryTreeView;

//-----------------------------------------------------------------------------
//! ProjectLibraryTreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------
class ProjectLibraryTreeWidget : public QWidget
{
	Q_OBJECT

public:

	/*! The constructor
	 *
	 *  Constructs the library tree view and the filter model for the tree view
	 *  and connects the view and filter model to the dataModel.
	 * 
	 *       @param [in] handler    The instance that manages the library.
	 *       @param [in] dataModel  The dataModel instance that holds the data to be displayed in the view.
	 *       @param [in] parent     The owner of this widget.
	 */
    ProjectLibraryTreeWidget(LibraryInterface* handler, ProjectLibraryTreeModel* dataModel, QWidget* parent);

    //! The destructor
    virtual ~ProjectLibraryTreeWidget();

	/*! Select the library item that is identified by the vlnv.
	 *
	 *      @param [in] vlnv The vlnv that identifies the libraryItem to select.
	*/
	void selectItem(VLNV const& vlnv);

    /*!
     *  Gets the filter for the tree model.
     *
     *      @return The filter in use.
     */
    ProjectLibraryTreeFilter* getFilter() const;

signals:

	//! Print a notification to the user
	void noticeMessage(const QString& msg);

	//! Print an error message to the user
	void errorMessage(const QString& msg);

	//! Emitted when an item identifying unique vlnv is selected.
	void itemSelected(const VLNV& vlnv);

    void copyVLNV(VLNV const& vlnv);

public slots:

    //! Open a hierarchical component
//    void onCopyVLNV(const VLNV& vlnv);

private:

	//! No copying.
    ProjectLibraryTreeWidget(const ProjectLibraryTreeWidget& other);

	//! No assignment.
    ProjectLibraryTreeWidget& operator=(const ProjectLibraryTreeWidget& other);

	//! Set up the connections between child widgets.
    void setupConnections(ProjectLibraryTreeModel* dataModel);
   
    //! The filter between the view and data model.
    ProjectLibraryTreeFilter* filter_;

	//! The table view to display the search results.
    ProjectLibraryTreeView view_;

	//! The model for the filter.
    ProjectLibraryTreeModel* dataModel_;

};

#endif // PROJECTLIBRARYTREEWIDGET_H
