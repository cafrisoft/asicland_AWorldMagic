//-----------------------------------------------------------------------------
// File: BusDefinitionEditorEx.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Bus defition editor is an editor for Bus- and Abstraction definitions.
//-----------------------------------------------------------------------------

#ifndef BUSDEFINITIONEDITOREX_H
#define BUSDEFINITIONEDITOREX_H

#include <QSharedPointer>
#include <QSplitter>
#include <QStackedWidget>

#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>
#include <IPXACTmodels/BusDefinition/BusDefinition.h>

#include <KactusAPI/LibraryInterface.h>

#include <UI/Widgets/TabDocument/TabDocument.h>

#include "BusDefGroupEx.h"
#include "AbsDefGroupEx.h"


class AbstractionDefinitionValidator;
class BusDefinitionValidator;
class ExpressionParser;
class PortAbstractionInterface;

//-----------------------------------------------------------------------------
//! Bus Editor is editor for Bus- and Abstraction definition.
//-----------------------------------------------------------------------------
class BusDefinitionEditorEx : public TabDocument
{
	Q_OBJECT

public:

	/*!
     *  The constructor.
	 *
	 *      @param [in] parent          The owner of this widget.
	 *      @param [in] libHandler      The instance that manages library.
	 *      @param [in] busDef          The bus definition to edit.
     */
    BusDefinitionEditorEx(QWidget *parent,
                          LibraryInterface* libHandler,
                          QSharedPointer<BusDefinition> busDef,
                          QSharedPointer<AbstractionDefinition> absDef = QSharedPointer<AbstractionDefinition>(),
                          bool disableBusDef = false);
//    QSharedPointer<BusDefinition> busDef):
	
	/*!
     *  The destructor.
     */
    ~BusDefinitionEditorEx() override = default;

    //! No copying. No assignment.
    BusDefinitionEditorEx(const BusDefinitionEditorEx& other) = delete;
    BusDefinitionEditorEx& operator=(const BusDefinitionEditorEx& other) = delete;

	/*!
     *  Get the VLNV that can be used to identify the Bus.
	 *
	 *      @return The VLNV that identifies the Bus.
     */
	VLNV getIdentifyingVLNV() const override;

    /*!
     *  Refreshes the editor.
     */
    void refresh() override;

    /*!
     *  Sets the protection state of the document.
     *
     *      @param [in] locked True for locked state; false for unlocked.
     */
    void setProtection(bool locked) override;

	/*!
     *  Get the vlnv of the current bus definition.
	 *
	 *      @return VLNV of the bus definition/abstraction definition being edited.
	 */
	VLNV getDocumentVLNV() const override;

    /*!
     *  Sets the edited bus definition.
     * 
     *      @param [in] busDef   The bus definition to set. Must not be NULL.
     */
    void setBusDef(QSharedPointer<BusDefinition> busDef);

    /*!
     *  Sets the edited abstraction definition.
     *
     *      @param [in] absDef   The bus definition to set. Must not be NULL.
     */
    void setAbsDef(QSharedPointer<AbstractionDefinition> absDef);



public slots:

    /*!
     *  Validates the document against the IP-XACT standard.
     *
     *      @param [out] errorList Error message list for reporting standard violations.
     *
     *      @return True if the document is valid. False if there were any violations.
     */
    virtual bool validate(QVector<QString>& errorList);

	/*!
     *  Saves the document and resets the modified state.
     */
	virtual bool save();

	/*!
     *  Saves the document as new object and resets modifies state
     */
	virtual bool saveAs();

    void showBusDefinitionEditor();
    void showAbstractionDefinitionEditor();

signals:

    /*!
     *  Inform that a port abstraction has been removed.
     *
     *      @param [in] portName    Name of the removed port abstraction.
     *      @param [in] mode        Mode of the removed port abstraction.
     */
    void portRemoved(QString const& portName, General::InterfaceMode const mode);

protected:
    
    /*!
     *  Called when the editor is shown.
     *
     *      @param [in] event   The show event.
     */
    virtual void showEvent(QShowEvent* event);

private:

	/*!
     *  Set the layout of the widget
     */
	void setupLayout();

    /*!
     *  Create interface for accessing port abstractions.
     *
     *      @return Interface for accessing port abstractions.
     */
    PortAbstractionInterface* createPortAbstractionInterface();

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

	//! The instance that handles the library
	LibraryInterface* libHandler_ = nullptr;

//    QStackedWidget *stackedWidget_;

    //! The splitter to contain the navigation tree.
//	QSplitter navigationSplitter_;

	//! The original bus definition to use when saving.
	QSharedPointer<BusDefinition> busDef_ = nullptr;

	//! Group containing elements to edit bus definition
    BusDefGroupEx busDefGroup_;

    //! The original abstraction definition to use when saving.
    QSharedPointer<AbstractionDefinition> absDef_;

    //! Group containing elements to edit abstraction definition.
    AbsDefGroupEx absDefGroup_;

    //! Parser for expressions in definitions.
    QSharedPointer<ExpressionParser> expressionParser_ = nullptr;

    //! Validator for bus definition.
    QSharedPointer<BusDefinitionValidator> busDefinitionValidator_ =nullptr;

    //! Validator for abstraction definition.
    QSharedPointer<AbstractionDefinitionValidator> absDefinitionValidator_;

    //! Flag for save status on bus definition.
    bool busDefinitionSaved_= false;

    QStackedWidget *stackedWidgets_;
};

#endif // BUSDEFINITIONEDITOREX_H
