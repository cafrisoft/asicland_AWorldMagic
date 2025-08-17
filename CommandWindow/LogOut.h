/*
 *      Created on: 18.1.2024
 *      Author: P.H.G
 *      Filename: LogOut.h
 *      Project: AWM
 *      Description :
 *          This widget is used to print event information to the user.
 */

#ifndef LOGOUT_H
#define LOGOUT_H

#include <QWidget>
#include <QTextEdit>
#include <QAction>
#include <QContextMenuEvent>
#include <QFontDatabase>
#include <QPushButton>
#include <QDateTime>

//-----------------------------------------------------------------------------
//! This widget is used to print event information to the user.
//-----------------------------------------------------------------------------
class LogOut : public QWidget
{
	Q_OBJECT

public:

	/*! The constructor
	 *
	 *      @param [in] parent     The owner of this widget
	 *
	*/
    explicit LogOut(QWidget *parent);

	//! The destructor
    virtual ~LogOut() = default;

    //! No copying
    LogOut(const LogOut& other) = delete;

    //! No assignment
    LogOut& operator=(const LogOut& other) = delete;

private:
    QTextEdit *textEdit;
    QStringList workHistory;
    QPushButton *btnClear;
    QPushButton *btnTcl;
    QDateTime dateTimeNow;

    //! Copy the selected text to the clip board.
    QAction copyAction_;

    //! Select all text in the text edit.
    QAction selectAllAction_;

    //! Clear all text from the text edit.
    QAction clearAction_;

    QStringList syntaxSystem = {
        "SYSTEM:",
        "SYSTEM :",
        "SYSTEM|",
        "SYSTEM |",
        "SYSTEM",
        "[SYSTEM]:",
        "[SYSTEM] :",
        "[SYSTEM]|",
        "[SYSTEM] |",
        "[SYSTEM]",
        "<SYSTEM>:",
        "<SYSTEM> :",
        "<SYSTEM>|",
        "<SYSTEM> |",
        "<SYSTEM>",
        "[ SYSTEM ]:",
        "[ SYSTEM ] :",
        "[ SYSTEM ]|",
        "[ SYSTEM ] |",
        "[ SYSTEM ]",
        "< SYSTEM >:",
        "< SYSTEM > :",
        "< SYSTEM >|",
        "< SYSTEM > |",
        "< SYSTEM >",
        "System:",
        "System :",
        "System|",
        "System |",
        "System",
        "[System]:",
        "[System] :",
        "[System]|",
        "[System] |",
        "[System]",
        "<System>:",
        "<System> :",
        "<System>|",
        "<System> |",
        "<System>",
        "[ System ]:",
        "[ System ] :",
        "[ System ]|",
        "[ System ] |",
        "[ System ]",
        "< System >:",
        "< System > :",
        "< System >|",
        "< System > |",
        "< System >",
        "system:",
        "system :",
        "system|",
        "system |",
        "system",
        "[system]:",
        "[system] :",
        "[system]|",
        "[system] |",
        "[system]",
        "<system>:",
        "<system> :",
        "<system>|",
        "<system> |",
        "<system>",
        "[ system ]:",
        "[ system ] :",
        "[ system ]|",
        "[ system ] |",
        "[ system ]",
        "< system >:",
        "< system > :",
        "< system >|",
        "< system > |",
        "< system >"
    };

    QStringList syntaxInfo = {
        "INFO:",
        "INFO :",
        "INFO|",
        "INFO |",
        "INFO",
        "[INFO]:",
        "[INFO] :",
        "[INFO]|",
        "[INFO] |",
        "[INFO]",
        "<INFO>:",
        "<INFO> :",
        "<INFO>|",
        "<INFO> |",
        "<INFO>",
        "[ INFO ]:",
        "[ INFO ] :",
        "[ INFO ]|",
        "[ INFO ] |",
        "[ INFO ]",
        "< INFO >:",
        "< INFO > :",
        "< INFO >|",
        "< INFO > |",
        "< INFO >",
        "Info:",
        "Info :",
        "Info|",
        "Info |",
        "Info",
        "[Info]:",
        "[Info] :",
        "[Info]|",
        "[Info] |",
        "[Info]",
        "<Info>:",
        "<Info> :",
        "<Info>|",
        "<Info> |",
        "<Info>",
        "[ Info ]:",
        "[ Info ] :",
        "[ Info ]|",
        "[ Info ] |",
        "[ Info ]",
        "< Info >:",
        "< Info > :",
        "< Info >|",
        "< Info > |",
        "< Info >",
        "info:",
        "info :",
        "info|",
        "info |",
        "info",
        "[info]:",
        "[info] :",
        "[info]|",
        "[info] |",
        "[info]",
        "<info>:",
        "<info> :",
        "<info>|",
        "<info> |",
        "<info>",
        "[ info ]:",
        "[ info ] :",
        "[ info ]|",
        "[ info ] |",
        "[ info ]",
        "< info >:",
        "< info > :",
        "< info >|",
        "< info > |",
        "< info >"
    };

    QStringList syntaxWarning = {
        "WARNING:",
        "WARNING :",
        "WARNING|",
        "WARNING |",
        "WARNING",
        "[WARNING]:",
        "[WARNING] :",
        "[WARNING]|",
        "[WARNING] |",
        "[WARNING]",
        "<WARNING>:",
        "<WARNING> :",
        "<WARNING>|",
        "<WARNING> |",
        "<WARNING>",
        "[ WARNING ]:",
        "[ WARNING ] :",
        "[ WARNING ]|",
        "[ WARNING ] |",
        "[ WARNING ]",
        "< WARNING >:",
        "< WARNING > :",
        "< WARNING >|",
        "< WARNING > |",
        "< WARNING >",
        "Warning:",
        "Warning :",
        "Warning|",
        "Warning |",
        "Warning",
        "[Warning]:",
        "[Warning] :",
        "[Warning]|",
        "[Warning] |",
        "[Warning]",
        "<Warning>:",
        "<Warning> :",
        "<Warning>|",
        "<Warning> |",
        "<Warning>",
        "[ Warning ]:",
        "[ Warning ] :",
        "[ Warning ]|",
        "[ Warning ] |",
        "[ Warning ]",
        "< Warning >:",
        "< Warning > :",
        "< Warning >|",
        "< Warning > |",
        "< Warning >",
        "warning:",
        "warning :",
        "warning|",
        "warning |",
        "warning",
        "[warning]:",
        "[warning] :",
        "[warning]|",
        "[warning] |",
        "[warning]",
        "<warning>:",
        "<warning> :",
        "<warning>|",
        "<warning> |",
        "<warning>",
        "[ warning ]:",
        "[ warning ] :",
        "[ warning ]|",
        "[ warning ] |",
        "[ warning ]",
        "< warning >:",
        "< warning > :",
        "< warning >|",
        "< warning > |",
        "< warning >"
    };

    QStringList syntaxError = {
        "ERROR:",
        "ERROR :",
        "ERROR|",
        "ERROR |",
        "ERROR",
        "[ERROR]:",
        "[ERROR] :",
        "[ERROR]|",
        "[ERROR] |",
        "[ERROR]",
        "<ERROR>:",
        "<ERROR> :",
        "<ERROR>|",
        "<ERROR> |",
        "<ERROR>",
        "[ ERROR ]:",
        "[ ERROR ] :",
        "[ ERROR ]|",
        "[ ERROR ] |",
        "[ ERROR ]",
        "< ERROR >:",
        "< ERROR > :",
        "< ERROR >|",
        "< ERROR > |",
        "< ERROR >",
        "Error:",
        "Error :",
        "Error|",
        "Error |",
        "Error",
        "[Error]:",
        "[Error] :",
        "[Error]|",
        "[Error] |",
        "[Error]",
        "<Error>:",
        "<Error> :",
        "<Error>|",
        "<Error> |",
        "<Error>",
        "[ Error ]:",
        "[ Error ] :",
        "[ Error ]|",
        "[ Error ] |",
        "[ Error ]",
        "< Error >:",
        "< Error > :",
        "< Error >|",
        "< Error > |",
        "< Error >",
        "error:",
        "error :",
        "error|",
        "error |",
        "error",
        "[error]:",
        "[error] :",
        "[error]|",
        "[error] |",
        "[error]",
        "<error>:",
        "<error> :",
        "<error>|",
        "<error> |",
        "<error>",
        "[ error ]:",
        "[ error ] :",
        "[ error ]|",
        "[ error ] |",
        "[ error ]",
        "< error >:",
        "< error > :",
        "< error >|",
        "< error > |",
        "< error >"
    };

    /*! Set the color for text to be printed.
     *
     *      @param [in] color  The color to apply to text.
    */
    void setTextColor(QColor const& color);

    void checkUndefinedMessage(QString const& message);

    bool checkSyntax(const QString &inputText, const QStringList &syntaxList);

protected:
    //! Event handler for context menu requests.
    virtual void contextMenuEvent(QContextMenuEvent* event);

    void appendInput(QString inputText);
    void appendLog(QString inputText);
    void appendInfoLog(QString inputText);
    void appendWarningLog(QString inputText);
    void appendErrorLog(QString inputText);

signals:

public slots:
    void onInputMessage(QString const& message);
    void onNoticeMessage(QString const& message);
    void onErrorMessage(QString const& message);
    void onUndefinedMessage(QString const& message);
    void onClearLog();
    void onCreateTcl();
};

#endif // LOGOUT_H
