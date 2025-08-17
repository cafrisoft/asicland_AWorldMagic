//-----------------------------------------------------------------------------
// File: AbsDefGroupEx.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Editor for the logical ports of an abstraction definition.
//-----------------------------------------------------------------------------

#ifndef ABSDEFGROUPEX_H
#define ABSDEFGROUPEX_H

#include <QWidget>
#include <QSharedPointer>
#include <QSortFilterProxyModel>
#include <QModelIndexList>
#include <QTabWidget>

#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>

#include "AbstractionPortsEditorEx.h"


class LibraryInterface;
class AbstractionDefinitionPortsSortFilterEx;
class AbstractionPortsModelEx;
class PortAbstractionInterface;

//-----------------------------------------------------------------------------
//! Editor for the logical ports of an abstraction definition.
//-----------------------------------------------------------------------------
class AbsDefGroupEx : public QWidget
{
	Q_OBJECT

public:

	/*!
     *  The constructor.
	 *
     *      @param [in] libraryHandler  Allows access to the library.
     *      @param [in] portInterface   Interface for accssing port abstractions.
	 *      @param [in] parent          The owner of the editor.
	 */
        AbsDefGroupEx(LibraryInterface* libraryHandler,
                      PortAbstractionInterface* portInterface,
                      PortAbstractionInterface* extendInterface,
                      QWidget *parent);
	
	/*!
     *  The destructor.
     */
        ~AbsDefGroupEx() override = default;

    //! No copying. No assignment.
    AbsDefGroupEx(const AbsDefGroupEx& other) = delete;
    AbsDefGroupEx& operator=(const AbsDefGroupEx& other) = delete;

	/*!
     *  Saves the changes made in the editor.
     */
	void save();

	/*!
     *  Set the abstraction definition for the editor.
	 *
	 *      @param [in] absDef  The Abstraction definition to edit.
     */
	void setAbsDef(QSharedPointer<AbstractionDefinition> absDef);

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

private:
    /*!
     *  Sets the editor layout.
     */
    void setupLayout();

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Contains tabs for wire and transactional ports.
    QTabWidget portTabs_;

    //! Interface for accessing port abstractions.
    PortAbstractionInterface* portInterface_;

    //! Interface for accessing extened port abstractions.
    PortAbstractionInterface* extendInterface_;

    //! The abstraction definition ports model.
    AbstractionPortsModelEx* portModel_;

    //! Editor for wire ports.
    AbstractionPortsEditorEx* wirePortsEditor_;

    //! The edited abstraction definition.
    QSharedPointer<AbstractionDefinition> abstraction_ = nullptr;

    //! The library interface.
    LibraryInterface* libraryHandler_;
 
};

#endif // ABSDEFGROUPEX_H
