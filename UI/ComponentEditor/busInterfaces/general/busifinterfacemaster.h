//-----------------------------------------------------------------------------
// File: busifinterfacemaster.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 7.4.2011
//
// Description:
// Editor to edit master or mirrored master settings of a bus interface.
//-----------------------------------------------------------------------------

#ifndef BUSIFINTERFACEMASTER_H
#define BUSIFINTERFACEMASTER_H

#include "busifinterfacemodeeditor.h"

#include <IPXACTmodels/generaldeclarations.h>

#include <UI/ComponentEditor/common/ReferenceSelector/ReferenceSelector.h>

#include <QString>
#include <QSharedPointer>
#include <QLineEdit>

class BusInterface;
class Component;
class ExpressionEditor;
class ExpressionParser;
class MasterInterface;
class ParameterFinder;

//-----------------------------------------------------------------------------
//! Editor to edit master or mirrored master settings of a bus interface.
//-----------------------------------------------------------------------------
class BusIfInterfaceMaster : public BusIfInterfaceModeEditor
{
	Q_OBJECT

public:

    /*!
	 *  The constructor.
	 *
	 *      @param [in] mode                The mode of the bus interface (master / mirrored master).
	 *      @param [in] busInterface        Interface for accessing bud interfaces.
     *      @param [in] busName             Name of the edited bus interface.
	 *      @param [in] component           The component being edited.
	 *      @param [in] parameterFinder     The parameter finder.
	 *      @param [in] expressionParser    The expression parser.
	 *      @param [in] parent              The owner of this editor.
	 */
    BusIfInterfaceMaster(General::InterfaceMode mode,
        BusInterfaceInterface* busInterface,
        std::string const& busName,
        QSharedPointer<Component> component,
        QSharedPointer<ParameterFinder> parameterFinder,
        QSharedPointer<ExpressionParser> expressionParser,
        QWidget *parent);

	//! The destructor
	virtual ~BusIfInterfaceMaster();

	/*! Check for the validity of the edited item.
	*
	*       @return True if item is valid.
	*/
	virtual bool isValid() const;

	/*! Restore the changes made in the editor back to ones in the model.
	*
	*/
	virtual void refresh();

	/*! Get the interface mode of the editor
	 * 
	 *      @return General::InterfaceMode Specifies the interface mode.
	*/
	virtual General::InterfaceMode getInterfaceMode() const;

	//! Save the interface mode-specific details to the bus interface.
	virtual void saveModeSpecific();

    /*!
     *  Remove the references from the expressions.
     */
    void removeReferencesFromExpressions();

signals:

    /*!
     *  Increase the number of references to the given parameter.
     *
     *      @param [in] id  The id of the target parameter.
     */
    void increaseReferences(QString id);

    /*!
     *  Decrease the number of references to the given parameter.
     *
     *      @param [in] id  The id of the target parameter.
     */
    void decreaseReferences(QString id);

private slots:

	/*!
	 *  Handler for changes in address space reference.
	 *
	 *      @param [in] addrSpaceName   The name of the referenced address space.
	 */
	void onAddressSpaceChange(QString const& addrSpaceName);

    /*!
     *  Handler for changes in base address.
     */
    void onBaseAddressChange();

private:

    //! No copying. No assignment.
	BusIfInterfaceMaster(const BusIfInterfaceMaster& other);
	BusIfInterfaceMaster& operator=(const BusIfInterfaceMaster& other);

    //-----------------------------------------------------------------------------
    //! Data.
    //-----------------------------------------------------------------------------

	//! Combo box to select an address space within component
	ReferenceSelector addressSpaceReferenceSelector_;

    //! Editor for the base address of an address space.
    ExpressionEditor* baseAddressEditor_;

    //! The parameter finder.
    QSharedPointer<ParameterFinder> parameterFinder_;

    //! The containing component.
    QSharedPointer<Component> component_;
};

#endif // BUSIFINTERFACEMASTER_H
