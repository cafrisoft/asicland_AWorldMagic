#ifndef LOGOUTTAB_H
#define LOGOUTTAB_H

#include <QTextEdit>
#include <QWidget>

#include <QPlainTextEdit>
#include <QAction>
#include <QContextMenuEvent>

class LogOutTab : public QWidget
{
    Q_OBJECT
public:
    explicit LogOutTab(QWidget *parent = 0);

private:
    //! Copy the selected text to the clip board.
    QAction copyAction_;

    //! Select all text in the text edit.
    QAction selectAllAction_;

    //! Clear all text from the text edit.
    QAction clearAction_;

public:
    QTextEdit* textEdit;

signals:

public slots:

    /*! Prints an error message to the text edit.
     *
     *      @param [in] message     The message to print.
    */
    void onErrorMessage(QString const& message);

    /*! Prints a notice message to the text edit.
     *
     *      @param [in] message  The message to print.
    */
    void onNoticeMessage(QString const& message);

protected:

    //! Event handler for context menu requests.
    virtual void contextMenuEvent(QContextMenuEvent* event);

private:
    /*! Set the color for text to be printed.
     *
     *      @param [in] color  The color to apply to text.
    */
    void setTextColor(QColor const& color);


public :
    void LogPrint(const QString &log, bool isErr=false);
};

#endif // LOGOUTTAB_H
