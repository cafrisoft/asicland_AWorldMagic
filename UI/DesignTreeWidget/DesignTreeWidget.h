//-----------------------------------------------------------------------------
// File: ProjectTreeExWidget.h
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.H.G
// Date: 04.06.2024
//
// Description:
// ProjectTreeExWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

#ifndef PROJECTTREEEXWIDGET_H
#define PROJECTTREEEXWIDGET_H

#include <QTabWidget>
//#include <QWidget>

#include <IPXACTmodels/common/VLNV.h>

#include "ProjectTreeExView.h"

class LibraryInterface;
class ProjectTreeExModel;
class ProjectTreeExView;

//-----------------------------------------------------------------------------
//! ProjectTreeExWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

class ProjectTreeExWidget : public QTabWidget
//class ProjectTreeExWidget : public QWidget
{
	Q_OBJECT

public:

	/*! The constructor
         *ProjectTreeExView_H
	 *  Constructs the library tree view and the filter model for the tree view
	 *  and connects the view and filter model to the dataModel.
	 * 
	 *       @param [in] handler    The instance that manages the library.
	 *       @param [in] dataModel  The dataModel instance that holds the data to be displayed in the view.
	 *       @param [in] parent     The owner of this widget.
	 */
    ProjectTreeExWidget(LibraryInterface* handler, ProjectTreeExModel* dataModel, QWidget* parent);

    //! The destructor
    virtual ~ProjectTreeExWidget();

	/*! Select the library item that is identified by the vlnv.
	 *
     *      @param [in] vlnv The vlnv that ideProjectTreeExView_Hntifies the LibraryItem to select.
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
    ProjectTreeExWidget(const ProjectTreeExWidget& other);

    //! No assignment.
    ProjectTreeExWidget& operator=(const ProjectTreeExWidget& other);

    //! Set up the connections between child widgets.
    void setupProjectTreeConnections(ProjectTreeExModel* dataModel);
   
    //! The filter between the view and data model.
    ProjectTreeExFilter* projectTreeExFilter_;

    //! The table view to display the search results.
    ProjectTreeExView projectTreeExView_;

    //! The model for the filter.
    ProjectTreeExModel* projectTreeExModel_;

};

#endif // PROJECTTREEEXWIDGET_H
