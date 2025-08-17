//-----------------------------------------------------------------------------
// File: AbstractionPortsViewEx.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// The view that displays the contents of the BusPortsModel.
//-----------------------------------------------------------------------------

#ifndef ABSTRACTIONPORTSVIEWEX_H
#define ABSTRACTIONPORTSVIEWEX_H

#include <QAction>
#include <QContextMenuEvent>
#include <QModelIndexList>
#include <QTableView>

#include <UI/Views/EditableTableView/editabletableview.h>


//-----------------------------------------------------------------------------
//! The view that displays the contents of the BusPortsModel.
//-----------------------------------------------------------------------------
class AbstractionPortsViewEx : public EditableTableView
{
	Q_OBJECT

public:

	/*!
     *  The constructor.
	 *
	 *      @param [in] parent  The owner of this widget.
     */
    AbstractionPortsViewEx(QWidget* parent);
	
	/*!
     *  The destructor.
     */
    virtual ~AbstractionPortsViewEx();

	/*!
     *  Get list of currently selected indexes.
	 *
	 *      @return The model indexes of the selected items.
     */
	QModelIndexList selected() const;

signals:

    /*!
     *  Emitted when a master mode should be added to the selected signal.
     */
    void addMaster();

    /*!
     *  Emitted when a slave mode should be added to the selected signal.
     */
    void addSlave();

    /*!
     *  Emitted when a system mode should be added to the selected signal.
     */
    void addSystem();

    /*!
     *  Emitted when the missing system group modes should be added to the selected signal.
     */
    void addAllSystems();

    /*!
     *  Emitted when the extend ports need to be reloaded from the extend abstraction definition.
     */
    void resetExtendPorts();

protected:

	//! Handler for context menu events
	virtual void contextMenuEvent(QContextMenuEvent* e);

private:
	//! No copying
    AbstractionPortsViewEx(const AbstractionPortsViewEx& other);

	//! No assignment
    AbstractionPortsViewEx& operator=(const AbstractionPortsViewEx& other);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Action for adding a master mode for a signal.
    QAction addMasterAction_;

    //! Action for adding a slave mode for a signal.
    QAction addSlaveAction_;

    //! Action for adding a system mode for a signal.
    QAction addSystemAction_;

    //! Action for adding the missing system modes for a signal.
    QAction addAllSystemsAction_;

    //! Action for reloading the extended abstraction definition ports.
    QAction resetExtendPortsAction_;
};

#endif // ABSTRACTIONPORTSVIEWEX_H
