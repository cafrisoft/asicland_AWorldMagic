//-----------------------------------------------------------------------------
// File: ExpressionLineEditor.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 18.10.2022
//
// Description:
// Line editor for expressions with parameter name completion.
//-----------------------------------------------------------------------------

#ifndef EXPRESSIONLINEEDITOR_H
#define EXPRESSIONLINEEDITOR_H

#include <UI/ComponentEditor/common/MasterExpressionEditor.h>

#include <QCompleter>
#include <QLineEdit>

class ParameterFinder;

//-----------------------------------------------------------------------------
//! Line editor for expressions with parameter name completion.
//-----------------------------------------------------------------------------
class ExpressionLineEditor : public QLineEdit, public MasterExpressionEditor
{
    Q_OBJECT
public:

	/*!
     *  The constructor.
     * 
     *      @param [in] parameterFinder     The parameter finder.
     *      @param [in] parent              The parent widget.
     */
    ExpressionLineEditor(QSharedPointer<ParameterFinder> parameterFinder, QWidget* parent = 0);

	/*!
     *  The destructor.
     */
	virtual ~ExpressionLineEditor() = default;

    /*!
     *  Sets a completer whose selection will be appended to the text.
     *
     *      @param [in] completer   The completer to set.
     */
    virtual void setAppendingCompleter(QCompleter* completer) override final;
    
    /*!
     *  Sets the expression in the editor.
     *
     *      @param [in] expression   The expression to set.
     */
    virtual void setExpression(QString const& expression) override final;

protected:

    /*!
     *  Captures the context menu event and accepts it.
     *
     *      @param [in] menuEvent   The event for context menu.
     */
    virtual void contextMenuEvent(QContextMenuEvent* menuEvent);

    /*!
     *  Captures the mouse press event and finishes editing the current word.
     *
     *      @param [in] mouseEvent  The event representing user input.
     */
    virtual void mousePressEvent(QMouseEvent* mouseEvent);

    /*!
     *  Captures the user input and edits the underlying expression accordingly.
     *
     *      @param [in] keyEvent   The event representing the user input.
     */
    virtual void keyPressEvent(QKeyEvent* keyEvent);

    /*!
     *  Selects the text when the editor is opened.
     *
     *      @param [in] focusEvent   The event representing the focus change.
     */
    virtual void focusInEvent(QFocusEvent* focusEvent);

    /*!
     *  Captures the mouse move and sets/clears notSelectingText flag according to mouse press.
     *
     *      @param [in] mouseEvent   The mouse event representing the mouse movement and button press.
     */
    virtual void mouseMoveEvent(QMouseEvent* mouseEvent);

    /*!
     *  Clears notSelectingText flag when the mouse button is released.
     *
     *      @param [in] mouseEvent   The event representing the mouse button release.
     */
    virtual void mouseReleaseEvent(QMouseEvent* mouseEvent);

    /*!
     *  Emits a signal informing of the ending of editing the expression.
     *
     *      @param [in] event   The event representing the changing of focus.
     */
    virtual void focusOutEvent(QFocusEvent *event);

signals:

    /*!
     *  Increase the amount of references to a parameter corresponding to the id.
     *
     *      @param [in] parameterId     The id of the parameter being referenced.
     */
    void increaseReference(QString const& parameterId);

    /*!
     *  Decrease the amount of references to a parameter corresponding to the id.
     *
     *      @param [in] parameterId     The id of the parameter being referenced.
     */
    void decreaseReference(QString const& parameterId);

    /*!
     *  Informs that the editing of the expression has finished.
     */
   void editingFinished();

private slots:

    /*!
     *  Called when a word completion has been selected. Replaces the word under the cursor with the selection.
     *
     *      @param [in] word   The selected word.
     */
    void complete(QModelIndex const& index);

    /*!
     *  Called when the cursor position changes. If a different word is under the new position, it is suggested
     *  for new completion.
     *
     *      @param [in] oldPosition     Old position of the cursor.
     *      @param [in] newPosition     New postiion of the cursor.
     */
    void updateCompletions(int oldPosition, int newPosition);

private:

    // Disable copying.
    ExpressionLineEditor(ExpressionLineEditor const& rhs);
    ExpressionLineEditor& operator=(ExpressionLineEditor const& rhs);

    /*!
     *  Changes the color of the font for the current word to red.
     */
    virtual void colorCurrentWordRed() override final;

    /*!
     *  Changes the color of the font for the current word to black.
     */
    virtual void colorCurrentWordBlack() override final;

    /*!
     *  Check if a selection exists.
     *
     *      @return True, if a selection exists, false otherwise.
     */
    virtual bool hasSelection() override final;

    /*!
     *  Let the parent widget handle the key press event.
     *
     *      @param [in] keyEvent    The selected key press event.
     */
    virtual void handleParentKeyPressEvent(QKeyEvent* keyEvent) override final;

    /*!
     *  Decrease references made to the selected ID.
     *
     *      @param [in] referenceItem   ID of the selected item.
     */
    virtual void handleReferenceDecrease(QString const& referenceItem) override final;

    /*!
     *  Increase references made to the selected ID.
     *
     *      @param [in] referenceItem   ID of the selected item.
     */
    virtual void handleReferenceIncrease(QString const& referenceItem) override final;

    /*!
     *  Set the completed parameter name for the editor.
     *
     *      @param [in] parameterName   The completed parameter name.
     */
    virtual void setCompletedParameterName(QString const& parameterName) override final;

    /*!
     *  Get the current cursor position.
     *
     *      @return The position of the cursor.
     */
    virtual int getCursorPosition() const override final;

    /*!
     *  Get the current text of the editor.
     *
     *      @return The current text.
     */
    virtual QString getCurrentText() const override final;

    /*!
     *  Get the selected text.
     *
     *      @return The selected text.
     */
    virtual QString getSelectedText() const override final;

    /*!
     *  Count the length of the first word in the current selection.
     *
     *      @return Length of the first word in the selection.
     */
    virtual int getSelectionFirstWord() const override final;

    /*!
     *  Count the length of the last word in the current selection.
     *
     *      @return Length of the last word in the selection.
     */
    virtual int getSelectionLastWord() const override final;
};

#endif // EXPRESSIONLINEEDITOR_H
