//-----------------------------------------------------------------------------
// File: librarytreewidget.h
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.HG
// Date: 09.01.2024
//
// Description:
// ProjectTreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include "ProjectTreeView.h"

#include <IPXACTmodels/common/VLNV.h>

#include <QWidget>

class LibraryInterface;
class ProjectTreeModel;
class ProjectTreeFilter;

//-----------------------------------------------------------------------------
//! ProjectTreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------
class ProjectTreeWidget : public QWidget
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
    ProjectTreeWidget(LibraryInterface* handler, ProjectTreeModel* dataModel, QWidget* parent);

    //! The destructor
    virtual ~ProjectTreeWidget();

	/*! Select the library item that is identified by the vlnv.
	 *
     *      @param [in] vlnv The vlnv that identifies the LibraryItem to select.
	*/
	void selectItem(VLNV const& vlnv);

    /*!
     *  Gets the filter for the tree model.
     *
     *      @return The filter in use.
     */
    LibraryFilter* getFilter() const;

signals:

	//! Print a notification to the user
	void noticeMessage(const QString& msg);

	//! Print an error message to the user
	void errorMessage(const QString& msg);

	//! Emitted when an item identifying unique vlnv is selected.
	void itemSelected(const VLNV& vlnv);

    //! Import the selected item and it's sub-items.
    void importItem();

    void exportItem();

private:

    //! No copying.
    ProjectTreeWidget(const ProjectTreeWidget& other);

    //! No assignment.
    ProjectTreeWidget& operator=(const ProjectTreeWidget& other);

    //! Set up the connections between child widgets.
    void setupConnections(ProjectTreeModel* dataModel);
   
    //! The filter between the view and data model.
    ProjectTreeFilter* filter_;

    //! The table view to display the search results.
    ProjectTreeView view_;

    //! The model for the filter.
    ProjectTreeModel* dataModel_;

};

#endif // PROJECTTREEWIDGET_H
