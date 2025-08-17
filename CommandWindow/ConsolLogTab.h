#ifndef CONSOLLOGTAB_H
#define CONSOLLOGTAB_H

#include "ConsoleTcl.h"

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QFontDatabase>
#include <QDateTime>
#include <QStringList>
#include <QShortcut>

#include <tcl.h>

#include <IPXACTmodels/common/VLNV.h>
#include <CommandWindow/ConsoleManager.h>



class ConsolLogTab : public QWidget
{
    Q_OBJECT

public:
    explicit ConsolLogTab(QWidget *parent = nullptr);

private:
    ConsoleTcl *consoleTcl;

    QLineEdit *lineEdit;
    QTextEdit *textEdit;

    QDateTime dateTimeNow;

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

    QStringList commandHistory;
    int commandHistoryNow;

    bool checkSyntax(const QString &inputText, const QStringList &syntaxList);

protected :
    void commandHistoryLoadUp();
    void commandHistoryLoadDown();
    void inputCommand();
    void outputCommand(QString inputText, ConsoleManager::OutputLevel outputLevel);

signals:
    void reloadLibrary();
    void reloadProject();
    void reloadProjectLibrary();
    void openSchematic(VLNV const&);
    void refreshSchematic(VLNV const&);
    void closeSchematic(VLNV const&);
    void closeAllSchematic();
    void openFile(QString);
    void requestSave();
    void requestSaveAll();
    void requestImportProject(QString const&, QString const&, QString const&);
    void requestExportProject(QString const&, QString const&, QString const&);

    void inputMessage(QString const&);
    void noticeMessage(QString const&);
    void errorMessage(QString const&);
    void undefinedMessage(QString const&);

public slots:
};

#endif // CONSOLLOGTAB_H
