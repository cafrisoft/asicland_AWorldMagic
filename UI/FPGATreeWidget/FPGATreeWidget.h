//-----------------------------------------------------------------------------
// File: FPGATreeWidget.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// FPGATreeWidget is the widget that displays the FPGA content in a tree.
//-----------------------------------------------------------------------------

#ifndef FPGATREEWIDGET_H
#define FPGATREEWIDGET_H

#include <QWidget>

#include "FPGATreeView.h"


class FPGAHandler;
class FPGATreeFilter;
class FPGATreeModel;
//class FPGATreeView;

//-----------------------------------------------------------------------------
//! FPGATreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------
class FPGATreeWidget : public QWidget
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
    FPGATreeWidget(FPGAHandler* handler, FPGATreeModel* dataModel, QWidget* parent);

    //! The destructor
    virtual ~FPGATreeWidget();

signals:

	//! Print a notification to the user
	void noticeMessage(const QString& msg);

	//! Print an error message to the user
    void errorMessage(const QString& msg);

    //! Open the selected FPGA
    void openFPGA(QModelIndex const& index);

public slots:
    //! Open the selected FPGA.
    void onOpenFPGA(QModelIndex const& index);

    //!
    void onRefresh();

private:
	//! Set up the connections between child widgets.
    void setupConnections(FPGATreeModel* dataModel);
   
    //! The filter between the view and data model.
    FPGATreeFilter* filter_;

	//! The table view to display the search results.
    FPGATreeView view_;

	//! The model for the filter.
    FPGATreeModel* dataModel_;

};

#endif // PROJECTLIBRARYTREEWIDGET_H
