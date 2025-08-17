//-----------------------------------------------------------------------------
// File: AbstractionPortsEditorEx.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Editor for the ports of an abstraction definition.
//-----------------------------------------------------------------------------

#ifndef ABSTRACTIONPORTSEDITOREX_H
#define ABSTRACTIONPORTSEDITOREX_H

#include <QWidget>
#include <QObject>

#include <IPXACTmodels/generaldeclarations.h>

#include "AbstractionPortsViewEx.h"
#include "LogicalPortColumns.h"


class AbstractionPortsModelEx;
class AbstractionPortsDelegateEx;
class AbstractionDefinitionPortsSortFilterEx;
class AbstractionDefinition;
class BusDefinition;
class PortAbstractionInterface;
class LibraryInterface;

//-----------------------------------------------------------------------------
//! Editor for the ports of an abstraction definition.
//-----------------------------------------------------------------------------
class AbstractionPortsEditorEx : public QWidget
{
    Q_OBJECT

public:

    /*!
     *  The constructor.
     *     
     *      @param [in] libraryAccess   Interface to the library.
     *      @param [in] portInterface   Interface for accessing port abstractions.
     *      @param [in] portsModel      Model for this editor.
     *      @param [in] type            The type of ports the editor is targeted for: wire or transactional.
     *      @param [in] parent          The owner of the editor.
     */
    AbstractionPortsEditorEx(LibraryInterface* libraryAccess,
                             PortAbstractionInterface* portInterface,
                             AbstractionPortsModelEx* portModel,
                             LogicalPortColumns::AbstractionType type,
                             QWidget* parent);

    /*!
     *  The destructor.
     */
    virtual ~AbstractionPortsEditorEx() = default;

    /*!
     *  Reset the port abstraction model.
     */
    void resetPortModel();

    /*!
     *  Set the bus definition referenced by the abstraction definition.
     *
     *      @param [in] busDefinition   The referenced bus definition.
     */
    void setBusDef(QSharedPointer<BusDefinition> busDefinition);

signals:

    /*!
     *  Emitted when contents of this editor changes.
     */
    void contentChanged();

    /*!
     *  Emitted when an error should be printed to user.
     */
    void errorMessage(QString const& message);

    /*!
     *  Emitted when a notification should be printed to user.
     */
    void noticeMessage(QString const& message);

    /*!
     *  Inform that a port abstraction has been removed.
     *
     *      @param [in] portName    Name of the removed port abstraction.
     *      @param [in] mode        Mode of the removed port abstraction.
     */
    void portRemoved(QString const& portName, General::InterfaceMode const mode);

private slots:

    /*!
     *  Handles the adding of new master signals.
     */
    void onAddMaster();

    /*!
     *  Handles the adding of new slave signals.
     */
    void onAddSlave();

    /*!
     *  Handles the adding of new system signals.
     */
    void onAddSystem();

    /*!
     *  Handles the adding of missing system group signals.
     */
    void onAddAllSystems();

private:
    //! No copying. No assignment.
    AbstractionPortsEditorEx(const AbstractionPortsEditorEx& other);
    AbstractionPortsEditorEx& operator=(const AbstractionPortsEditorEx& other);

    /*!
     *  Sets the editor layout.
     */
    void setupLayout();

   
    /*!
     *  Get a list of the selected indexes.
     *
     *      @return List of the selected indexes.
     */
    QModelIndexList getSelectedIndexes();

    //! Hide columns specific to transactional ports.
    void hideTransactionalColumns();

    //! Hide columns specific to wire ports.
    void hideWireColumns();

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The table view to display the logical signals
    AbstractionPortsViewEx portView_;

    //! Proxy model for sorting abstract ports.
    AbstractionDefinitionPortsSortFilterEx* portProxy_;

    //! The model that contains the logical signals of Abstraction Definition.
    AbstractionPortsModelEx* portModel_;

    //! The item delegate for portView_.
    AbstractionPortsDelegateEx* portDelegate_;

};

#endif // ABSTRACTIONPORTSEDITOREX_H
