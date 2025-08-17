//-----------------------------------------------------------------------------
// File: FPGATreeView.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// The widget to display the FPGA in a tree.
//-----------------------------------------------------------------------------

#ifndef FPGATREEVIEW_H
#define FPGATREEVIEW_H

#include <QTreeView>


class FPGAHandler;
class FPGATreeFilter;

//-----------------------------------------------------------------------------
//! The widget to display the library in a tree.
//-----------------------------------------------------------------------------
class FPGATreeView : public QTreeView
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
    FPGATreeView(FPGAHandler* handler, FPGATreeFilter* filter, QWidget* parent = 0);

	//! The destructor.
    virtual ~FPGATreeView() = default;

private:
    //! The instance that gives the xml objects.
    FPGAHandler* handler_;

    //! The filter that is used to transform model indexes
    FPGATreeFilter* filter_;

protected:

signals:
    //! Open the selected hierarchical design
    void openFPGA(QModelIndex const& index);

public slots:

private:
    //! Set up the actions for the context menu
    void setupActions();

protected:
    //! The event handler for mouse double clicks
    virtual void mouseDoubleClickEvent(QMouseEvent * event);



};

#endif // FPGATREEVIEW_H
