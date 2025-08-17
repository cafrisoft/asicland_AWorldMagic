//-----------------------------------------------------------------------------
// File: SystemGroupListEditorEx.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// List editor for bus interface system groups.
//-----------------------------------------------------------------------------

#ifndef SYSTEMGROUPLISTEDITOREX_H
#define SYSTEMGROUPLISTEDITOREX_H

#include <QListWidget>
#include <QAction>

class BusDefinition;
class LibraryInterface;

//-----------------------------------------------------------------------------
//! List editor for bus interface system groups.
//-----------------------------------------------------------------------------
class SystemGroupListEditorEx : public QListWidget
{
	Q_OBJECT

public:

	/*!
     *  The constructor.
	 *
     *      @param [in] library     Interface to the library.
	 *      @param [in] parent      The owner of this widget.
	 */
    SystemGroupListEditorEx(LibraryInterface* library, QWidget *parent);

    /*!
     *  The destructor.
     */
    virtual ~SystemGroupListEditorEx() = default;

	/*!
     *  Set the bus definition that is being edited.
	 *
	 *      @param [in] busDefinition   The bus definition
	 */
    void setItems(QSharedPointer<BusDefinition> busDefinition);

signals:

	/*!
     *  Emitted when user changes the state of one of the elements.
     */
	void contentChanged();

protected:

    /*!
     *  Handles the context menu events.
     *
     *      @param [in] event   The current context menu event.
     */
    virtual void contextMenuEvent(QContextMenuEvent* event);

    /*!
     *  Handles the double click events.
     *
     *      @param [in] event   The current double click event.
     */
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private slots:

    /*!
     *  Handles the creation of new system group items.
     */
    void onAddNewAction();

    /*!
     *  Handles the removal of system group items.
     */
    void onRemoveAction();

private:
	//! No copying. No assignment.
    SystemGroupListEditorEx(const SystemGroupListEditorEx& other);
    SystemGroupListEditorEx& operator=(const SystemGroupListEditorEx& other);

    /*!
     *  Get the system group names from the extended bus definitions.
     *
     *      @param [in] busDefinition   The selected bus definition.
     *
     *      @return List of the extended bus definition system group names.
     */
    QStringList getExtendedSystemGroupNames(QSharedPointer<BusDefinition> busDefinition) const;

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Action for creating new system group names.
    QAction* addAction_;

    //! Action for removing system group names.
    QAction* removeAction_;

    //! The library interface.
    LibraryInterface* library_;
};

#endif // SYSTEMGROUPLISTEDITOREX_H
