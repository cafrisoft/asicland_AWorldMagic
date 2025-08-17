//-----------------------------------------------------------------------------
// File: messageconsole.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 22.02.2011
//
// Description:
// This widget is used to print event information to the user.
//-----------------------------------------------------------------------------

#ifndef MESSAGECONSOLE_H
#define MESSAGECONSOLE_H

#include <QWidget>
#include <QTextEdit>
#include <QAction>
#include <QContextMenuEvent>
#include <QFontDatabase>
#include <QPushButton>

#include <CommandWindow/ConsoleManager.h>

//-----------------------------------------------------------------------------
//! This widget is used to print event information to the user.
//-----------------------------------------------------------------------------
class MessageConsole : public QWidget
{
    Q_OBJECT

public:

	/*! The constructor
	 *
	 *      @param [in] parent     The owner of this widget
	 *
	*/
	explicit MessageConsole(QWidget *parent);

	//! The destructor
    virtual ~MessageConsole() = default;

    //! No copying
    MessageConsole(const MessageConsole& other) = delete;

    //! No assignment
    MessageConsole& operator=(const MessageConsole& other) = delete;

private:
    QTextEdit *textEdit;
    QPushButton *btnClear;
    QPushButton *btnInfo;
    QPushButton *btnWarning;
    QPushButton *btnError;
    QStringList reportConsole;

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

    void checkErrorMessage(QString const& message);
    void checkUndefinedMessage(QString const& message);

    bool checkSyntax(const QString &inputText, const QStringList &syntaxList);

protected:

    //! Event handler for context menu requests.
    virtual void contextMenuEvent(QContextMenuEvent* event);

signals:

public slots:
    void onReloadReport();
    void onClearReport();

    void appendReport(QString inputText, ConsoleManager::OutputLevel outputLevel);

    void onInputMessage(QString const& message);
    void onNoticeMessage(QString const& message);
    void onErrorMessage(QString const& message);
    void onUndefinedMessage(QString const& message);
};

#endif // MESSAGECONSOLE_H
