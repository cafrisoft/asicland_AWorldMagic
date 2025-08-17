#ifndef CONSOLETCL_H
#define CONSOLETCL_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QList>
#include <QProcess>
#include <QObject>

#include <tcl.h>

#include <IPXACTmodels/common/VLNV.h>
#include <CommandWindow/ConsoleManager.h>



class ProjectHandler;
class ProjectLibraryHandler;
class FPGAHandler;
class LibraryHandler;

class ConsoleTcl : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleTcl(QWidget *parent = nullptr);
    ~ConsoleTcl();

    // TCL Command Functions
    void tclInterpreter(QString inputCommand);

    // TCL Result Sender
    QStringList tclResultSender();
    void pythonOutputReader(QProcess *process);
    void checkPythonIsRunning(QProcess::ExitStatus exitStatus);

    // TCL Custom Commands
    static int ccTCL_debugTcl(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_reload(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_runTcl(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_create(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_import(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_export(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_open(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_close(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_save(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_saveAll(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_copyLib(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_putLib(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_autoCon(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_autoTop(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_addTop(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_connect(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_runSim(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_runSynth(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);
    static int ccTCL_runFPGA(ClientData clientData, Tcl_Interp *inter_tcl, int argc, const char *argv[]);

    // Request for emitting signal
    void requestReloadLibrary();
    void requestReloadProject();
    void requestReloadProjectLibrary();
    void requestOpenSchematic(VLNV const& vlnv);
    void requestRefreshSchematic(VLNV const& vlnv);
    void requestCloseSchematic(VLNV const& vlnv);
    void requestCloseAllSchematic();
    void openFile(QString filePath);
    void requestSave();
    void requestSaveAll();
    void requestImportProject(QString const& fileDest, QString const& filePath, QString const& fileName);
    void requestExportProject(QString const& fileDest, QString const& filePath, QString const& fileName);

    void appendInputMessage(QString inputText);
    void appendNoticeMessage(QString inputText);
    void appendErrorMessage(QString inputText);
    void appendUndefinedMessage(QString inputText);

private:
    static Tcl_Interp* inter_tcl;
    static QStringList outputText;
    static QProcess scriptProcess;

protected:
    static QString path_RootAMagic;
    static QString path_RootScript;
    static QString path_GeneralLibrary;
    static QString path_DefaultProject;

signals:

public slots:

};

#endif // CONSOLETCL_H
