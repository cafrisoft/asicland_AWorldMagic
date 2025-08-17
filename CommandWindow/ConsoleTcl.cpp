#include "ConsoleTcl.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QSettings>
#include <QProcess>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QByteArray>

#include <Common/SettingManager.h>
#include <CommandWindow/ConsoleManager.h>



Tcl_Interp* ConsoleTcl::inter_tcl = nullptr;

QStringList ConsoleTcl::outputText;

QProcess ConsoleTcl::scriptProcess;

QString ConsoleTcl::path_RootAMagic;
QString ConsoleTcl::path_RootScript;
QString ConsoleTcl::path_GeneralLibrary;
QString ConsoleTcl::path_DefaultProject;



ConsoleTcl::ConsoleTcl(QWidget *parent) : QObject(parent)
{
    // Initialize TCL Interpreter
    if (inter_tcl == nullptr)
    {
        inter_tcl = Tcl_CreateInterp();

        if (Tcl_Init(inter_tcl) == TCL_ERROR)
        {
            Tcl_DeleteInterp(inter_tcl);
            inter_tcl = nullptr;
        }
    }

    // Setup TCL Custom Command
    Tcl_CreateCommand(inter_tcl, "debugtcl", ccTCL_debugTcl, NULL, NULL);   //Custom Command Setup : Debug TCL
    Tcl_CreateCommand(inter_tcl, "reload", ccTCL_reload, NULL, NULL);       //Custom Command Setup : Reload Library and Project
    Tcl_CreateCommand(inter_tcl, "runtcl", ccTCL_runTcl, NULL, NULL);       //Custom Command Setup : Run TCL File
    Tcl_CreateCommand(inter_tcl, "create", ccTCL_create, NULL, NULL);       //Custom Command Setup : Create New File
    Tcl_CreateCommand(inter_tcl, "import", ccTCL_import, NULL, NULL);       //Custom Command Setup : Import Project
    Tcl_CreateCommand(inter_tcl, "export", ccTCL_export, NULL, NULL);       //Custom Command Setup : Export Project
    Tcl_CreateCommand(inter_tcl, "open", ccTCL_open, NULL, NULL);           //Custom Command Setup : Open Project, Design or file
    Tcl_CreateCommand(inter_tcl, "close", ccTCL_close, NULL, NULL);         //Custom Command Setup : Close Project or Design
    Tcl_CreateCommand(inter_tcl, "save", ccTCL_save, NULL, NULL);           //Custom Command Setup : Save
    Tcl_CreateCommand(inter_tcl, "saveall", ccTCL_saveAll, NULL, NULL);     //Custom Command Setup : Save All
    Tcl_CreateCommand(inter_tcl, "copylib", ccTCL_copyLib, NULL, NULL);     //Custom Command Setup : Copy Library to Project
    Tcl_CreateCommand(inter_tcl, "putlib", ccTCL_putLib, NULL, NULL);       //Custom Command Setup : Put Library on Design Schematic
    Tcl_CreateCommand(inter_tcl, "autocon", ccTCL_autoCon, NULL, NULL);     //Custom Command Setup : Auto Connector
    Tcl_CreateCommand(inter_tcl, "autotop", ccTCL_autoTop, NULL, NULL);     //Custom Command Setup : Auto TopI/O Generate
    Tcl_CreateCommand(inter_tcl, "addtop", ccTCL_addTop, NULL, NULL);       //Custom Command Setup : Add New TopI/O
    Tcl_CreateCommand(inter_tcl, "connect", ccTCL_connect, NULL, NULL);     //Custom Command Setup : Connect TopI/O to IP or IP to IP,
    Tcl_CreateCommand(inter_tcl, "runsim", ccTCL_runSim, NULL, NULL);       //Custom Command Setup : Run Simulation Sequence
    Tcl_CreateCommand(inter_tcl, "runsynth", ccTCL_runSynth, NULL, NULL);   //Custom Command Setup : Run Synthesis Sequence
    Tcl_CreateCommand(inter_tcl, "runfpga", ccTCL_runFPGA, NULL, NULL);     //Custom Command Setup : Run FPGA Bit Stream Generate Sequence

    // Initialize Result QStringList
    outputText.clear();

}



//-----------------------------------------------------------------------------
// Function: Delete TCL Interpreter when not use
//-----------------------------------------------------------------------------
ConsoleTcl::~ConsoleTcl() {
    if (inter_tcl != nullptr)
    {
        Tcl_DeleteInterp(inter_tcl);
        inter_tcl = nullptr;
    }
}



//-----------------------------------------------------------------------------
// Function: tclInterpreter() : Check Input Command is TCL or not
//-----------------------------------------------------------------------------
void ConsoleTcl::tclInterpreter(QString inputCommand)
{
    path_RootAMagic = QSettings().value(QStringLiteral("ROOT/Location")).toString();
    path_RootScript = QSettings().value(QStringLiteral("Script/Location")).toString();
    path_GeneralLibrary = QSettings().value(QStringLiteral("Library/DefaultLocation")).toString();
    path_DefaultProject = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

    outputText.clear();

    QStringList inputCommands = inputCommand.split(" ");
    QString whatCommand = inputCommands.at(0);

    Tcl_CmdInfo cmdInfo;

    ConsoleTcl *consoleTcl_;
    consoleTcl_->appendInputMessage(inputCommand);

    if (Tcl_GetCommandInfo(inter_tcl, whatCommand.toStdString().c_str(), &cmdInfo))
    {
        const char* inputValue = inputCommand.toStdString().c_str();

        Tcl_Eval(inter_tcl, inputValue);
    }

    else
    {
        QString resultText = "[ ERROR ] | Tcl Interpreter | Command Not Found : " + inputCommand;
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);
    }
}



//-----------------------------------------------------------------------------
// Function: tclResultSender : Return TCL Result
//-----------------------------------------------------------------------------
QStringList ConsoleTcl::tclResultSender()
{
    return outputText;
}



//-----------------------------------------------------------------------------
// Function: pythonOutputReader : Return Python Result
//-----------------------------------------------------------------------------
void ConsoleTcl::pythonOutputReader(QProcess *process)
{
    ConsoleTcl *consoleTcl_;

    while (process->canReadLine())
    {
        QString output = process->readLine();

        QString outputTrimmed = output.replace("\n", "");

        if (output != "")
        {
            consoleTcl_->appendUndefinedMessage(outputTrimmed);
            outputText.append(outputTrimmed);
        }
    }
}



//-----------------------------------------------------------------------------
// Function: checkPythonIsRunning : Check Python is running
//-----------------------------------------------------------------------------
void ConsoleTcl::checkPythonIsRunning(QProcess::ExitStatus exitStatus)
{
    while (exitStatus == QProcess::NormalExit)
    {
        if (exitStatus == QProcess::NormalExit)
        {
            break;
        }
    }
}



// ////////////////////////// //
//  REQUESTING SIGNALS BELOW  //
// ////////////////////////// //

//-----------------------------------------------------------------------------
// Function: requestReloadLibrary() : Request for emitting signal to reload Library.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestReloadLibrary()
{
    emit ConsoleManager::getInstance().reloadLibrary();
}



//-----------------------------------------------------------------------------
// Function: requestReloadProject() : Request for emitting signal to reload Project.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestReloadProject()
{
    emit ConsoleManager::getInstance().reloadProject();
}



//-----------------------------------------------------------------------------
// Function: requestReloadProjectLibrary() : Request for emitting signal to reload Project's Library.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestReloadProjectLibrary()
{
    emit ConsoleManager::getInstance().reloadProjectLibrary();
}



//-----------------------------------------------------------------------------
// Function: requestOpenSchematic() : Request for emitting signal to open/refresh Design's Schematic.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestOpenSchematic(VLNV const& vlnv)
{
    emit ConsoleManager::getInstance().openSchematic(vlnv);
}



//-----------------------------------------------------------------------------
// Function: requestRefreshSchematic() : Request for emitting signal to close Design's Schematic.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestRefreshSchematic(VLNV const& vlnv)
{
    emit ConsoleManager::getInstance().refreshSchematic(vlnv);
}



//-----------------------------------------------------------------------------
// Function: requestCloseSchematic() : Request for emitting signal to close Design's Schematic.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestCloseSchematic(VLNV const& vlnv)
{
    emit ConsoleManager::getInstance().closeSchematic(vlnv);
}



//-----------------------------------------------------------------------------
// Function: requestCloseAllSchematic() : Request for emitting signal to close All Design's Schematic.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestCloseAllSchematic()
{
    emit ConsoleManager::getInstance().closeAllSchematic();
}



//-----------------------------------------------------------------------------
// Function: openFile() : Request for emitting signal to open file.
//-----------------------------------------------------------------------------
void ConsoleTcl::openFile(QString inputText)
{
    emit ConsoleManager::getInstance().openFile(inputText);
}



//-----------------------------------------------------------------------------
// Function: requestSave() : Request save.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestSave()
{
    emit ConsoleManager::getInstance().requestSave();
}



//-----------------------------------------------------------------------------
// Function: requestSaveAll() : Request save all.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestSaveAll()
{
    emit ConsoleManager::getInstance().requestSaveAll();
}



//-----------------------------------------------------------------------------
// Function: requestImportProject() : Request import project.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestImportProject(QString const& fileDest, QString const& filePath, QString const& fileName)
{
    emit ConsoleManager::getInstance().requestImportProject(fileDest, filePath, fileName);
}



//-----------------------------------------------------------------------------
// Function: requestExportProject() : Request export project.
//-----------------------------------------------------------------------------
void ConsoleTcl::requestExportProject(QString const& fileDest, QString const& filePath, QString const& fileName)
{
    emit ConsoleManager::getInstance().requestExportProject(fileDest, filePath, fileName);
}



//-----------------------------------------------------------------------------
// Function: appendInputMessage() : Request for emitting signal to append Console Input on LogTab.
//-----------------------------------------------------------------------------
void ConsoleTcl::appendInputMessage(QString inputText)
{
    emit ConsoleManager::getInstance().inputMessage(inputText);
}



//-----------------------------------------------------------------------------
// Function: appendNoticeMessage() : Request for emitting signal to append Console Input on LogTab.
//-----------------------------------------------------------------------------
void ConsoleTcl::appendNoticeMessage(QString inputText)
{
    emit ConsoleManager::getInstance().noticeMessage(inputText);
}



//-----------------------------------------------------------------------------
// Function: appendErrorMessage() : Request for emitting signal to append Console Input on LogTab.
//-----------------------------------------------------------------------------
void ConsoleTcl::appendErrorMessage(QString inputText)
{
    emit ConsoleManager::getInstance().errorMessage(inputText);
}



//-----------------------------------------------------------------------------
// Function: appendUndefinedMessage() : Request for emitting signal to append Console Input on LogTab.
//-----------------------------------------------------------------------------
void ConsoleTcl::appendUndefinedMessage(QString inputText)
{
    emit ConsoleManager::getInstance().undefinedMessage(inputText);
}



// /////////////////////////// //
//  TCL CUSTOM COMMANDS BELOW  //
// /////////////////////////// //

//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_debugTcl()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_debugTcl(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    QString resultText = "[ SYSTEM ] | Debug TCL | TCL Interpreter is Avaliable.";
    outputText.append(resultText);

    consoleTcl_->appendNoticeMessage(resultText);

    return TCL_OK;
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_reload()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_reload(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    consoleTcl_->requestReloadProject();
    consoleTcl_->requestReloadProjectLibrary();

    QString resultText = "[ SYSTEM ] | Reload | System reloaded.";
    outputText.append(resultText);

    consoleTcl_->appendNoticeMessage(resultText);

    return TCL_OK;
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_runTcl()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_runTcl(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2 || argc > 2)
    {
        QString resultText = "[ INFO ] | Run Tcl | Usage : runtcl {Absolute Path}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else {
        QStringList inputText;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        const char *filePath = inputText.at(1).toStdString().c_str();

        if (Tcl_EvalFile(inter_tcl, filePath) != TCL_OK)
        {
            QString resultText = "[ ERROR ] | Run Tcl | Can't run the file.";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            return TCL_OK;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_create()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_create(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Create | Usage : create [[io, ip, design] {Vendor} {Library} {Module Name} {Version} / project {Project Name} {Design Name} {Version}]";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_GeneralLibrary_Exist(path_GeneralLibrary);
        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_RootAMagic_ = path_RootAMagic;
        QString path_GeneralLibrary_ = path_GeneralLibrary;
        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (QString(inputText[1]) == "io")
        {
            if (argc < 6 || argc > 6)
            {
                QString resultText = "[ INFO ] | Create | Usage : create [[io, ip, design] {Vendor} {Library} {Module Name} {Version} / project {Project Name} {Design Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                if (check_GeneralLibrary_Exist.exists())
                {
                    QDir check_ThisLibrary_Exist(path_GeneralLibrary_ + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4] + "/" + inputText[5]);

                    if (!check_ThisLibrary_Exist.exists())
                    {
                        QStringList scriptParam;
                        scriptParam.append("-vendor");
                        scriptParam.append(inputText[2]);
                        scriptParam.append("-lib-name");
                        scriptParam.append(inputText[3]);
                        scriptParam.append("-module-name");
                        scriptParam.append(inputText[4]);
                        scriptParam.append("-version");
                        scriptParam.append(inputText[5]);

                        QStringList mergeParam;
                        mergeParam.append("python -u " + path_RootAMagic_ + "/sources/script/io_generator_for_gui/generate_io_for_gui.py " + scriptParam.join(" "));

                        QStringList runParam;
                        runParam << "-c";
                        runParam << mergeParam.join("; ");

                        QCoreApplication::processEvents();
                        scriptProcess.setWorkingDirectory(path_RootAMagic_);
                        scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                        scriptProcess.start("csh", runParam);

                        QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                            consoleTcl_->pythonOutputReader(&scriptProcess);
                        });

                        consoleTcl_->requestReloadLibrary();

                        return TCL_OK;
                    }

                    else
                    {
                        QString resultText = "[ WARNING ] | Create | File or Folder is already exist.";
                        outputText.append(resultText);

                        consoleTcl_->appendErrorMessage(resultText);

                        return TCL_OK;
                    }
                }

                else
                {
                    QString resultText = "[ WARNING ] | Create | No Library is Avaliable";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }

        else if (QString(inputText[1]) == "ip")
        {
            if (argc < 6 || argc > 6)
            {
                QString resultText = "[ INFO ] | Create | Usage : create [[io, ip, design] {Vendor} {Library} {Module Name} {Version} / project {Project Name} {Design Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                if (check_GeneralLibrary_Exist.exists())
                {
                    QDir check_ThisLibrary_Exist(path_GeneralLibrary_ + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4] + "/" + inputText[5]);

                    if (!check_ThisLibrary_Exist.exists())
                    {
                        QStringList scriptParam;
                        scriptParam.append("-vendor");
                        scriptParam.append(inputText[2]);
                        scriptParam.append("-lib-name");
                        scriptParam.append(inputText[3]);
                        scriptParam.append("-module-name");
                        scriptParam.append(inputText[4]);
                        scriptParam.append("-version");
                        scriptParam.append(inputText[5]);

                        QStringList mergeParam;
                        mergeParam.append("python -u " + path_RootAMagic_ + "/sources/script/ip_generator_for_gui/generate_ip_for_gui.py " + scriptParam.join(" "));

                        QStringList runParam;
                        runParam << "-c";
                        runParam << mergeParam.join("; ");

                        QCoreApplication::processEvents();
                        scriptProcess.setWorkingDirectory(path_RootAMagic_);
                        scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                        scriptProcess.start("csh", runParam);

                        QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                            consoleTcl_->pythonOutputReader(&scriptProcess);
                        });

                        consoleTcl_->requestReloadLibrary();

                        return TCL_OK;
                    }

                    else
                    {
                        QString resultText = "[ WARNING ] | Create | File or Folder is already exist.";
                        outputText.append(resultText);

                        consoleTcl_->appendErrorMessage(resultText);

                        return TCL_OK;
                    }
                }

                else
                {
                    QString resultText = "[ WARNING ] | Create | No Library is Avaliable";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }

        else if (QString(inputText[1]) == "project")
        {
            if (argc < 5 || argc > 5)
            {
                QString resultText = "[ INFO ] | Create | Usage : create [[io, ip, design] {Vendor} {Library} {Module Name} {Version} / project {Project Name} {Design Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                QDir check_ThisProject_Exist(path_RootAMagic_ + "/project/" + inputText[2]);

                QString path_ThisProject_ = path_RootAMagic_ + "/project/" + inputText[2];

                if (!check_ThisProject_Exist.exists())
                {
                    QStringList projParam;
                    projParam.append(path_RootAMagic_ + "/core");
                    projParam.append(path_ThisProject_);
                    projParam.append("chmod 777 -R " + path_ThisProject_);

                    QStringList setprjParam;
                    setprjParam.append("source " + path_ThisProject_ + "/setprj");

                    QStringList moveParam;
                    moveParam.append("cd " + path_ThisProject_ + "/script/hw_design_gen_by_tony");

                    QStringList designPlatformParam;
                    designPlatformParam.append("-vendor");
                    designPlatformParam.append(inputText[3]);
                    designPlatformParam.append("-lib-name");
                    designPlatformParam.append("platform");
                    designPlatformParam.append("-module-name");
                    designPlatformParam.append("platform_for_" + inputText[3]);
                    designPlatformParam.append("-version");
                    designPlatformParam.append(inputText[4]);

                    QStringList designChipParam;
                    designChipParam.append("-vendor");
                    designChipParam.append(inputText[3]);
                    designChipParam.append("-lib-name");
                    designChipParam.append("chip");
                    designChipParam.append("-module-name");
                    designChipParam.append("chip_for_" + inputText[3]);
                    designChipParam.append("-version");
                    designChipParam.append(inputText[4]);

                    QStringList designSystemParam;
                    designSystemParam.append("-vendor");
                    designSystemParam.append(inputText[3]);
                    designSystemParam.append("-lib-name");
                    designSystemParam.append("system");
                    designSystemParam.append("-module-name");
                    designSystemParam.append("system_for_" + inputText[3]);
                    designSystemParam.append("-version");
                    designSystemParam.append(inputText[4]);

                    QStringList mergeParam;
                    mergeParam.append("cp -RP " + projParam.join(" "));
                    mergeParam.append(setprjParam);
                    mergeParam.append(moveParam);
                    mergeParam.append(path_ThisProject_ + "/bin/awsh python -u script_gen.py " + designPlatformParam.join(" "));
                    mergeParam.append(path_ThisProject_ + "/bin/awsh python -u script_gen.py " + designChipParam.join(" "));
                    mergeParam.append(path_ThisProject_ + "/bin/awsh python -u script_gen.py " + designSystemParam.join(" "));

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_RootAMagic_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    QSettings().setValue("Project/DefaultLocation", path_ThisProject_);

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();

                    return TCL_OK;
                }

                else
                {
                    QString resultText = "[ WARNING ] | Create | Project is already exist.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }

        else if (QString(inputText[1]) == "design")
        {
            if (argc < 6 || argc > 6)
            {
                QString resultText = "[ INFO ] | Create | Usage : create [[io, ip, design] {Vendor} {Library} {Module Name} {Version} / project {Project Name} {Design Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                if (check_DefaultProject_Exist.exists())
                {
                    QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4] + "/" + inputText[5]);

                    if (!check_ThisDesign_Exist.exists())
                    {
                        QStringList designParam;
                        designParam.append("-vendor");
                        designParam.append(inputText[2]);
                        designParam.append("-lib-name");
                        designParam.append(inputText[3]);
                        designParam.append("-module-name");
                        designParam.append(inputText[4]);
                        designParam.append("-version");
                        designParam.append(inputText[5]);

                        QStringList mergeParam;
                        mergeParam.append(path_DefaultProject_ + "/bin/awsh " + "python -u " + path_DefaultProject_ + "/script/hw_design_gen_by_tony/script_gen.py " + designParam.join(" "));

                        QStringList runParam;
                        runParam << "-c";
                        runParam << mergeParam.join("; ");

                        QCoreApplication::processEvents();
                        scriptProcess.setWorkingDirectory(path_DefaultProject_);
                        scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                        scriptProcess.start("csh", runParam);

                        QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                            consoleTcl_->pythonOutputReader(&scriptProcess);
                        });

                        VLNV vlnv;
                        vlnv.setType(VLNV::COMPONENT);
                        vlnv.setVendor(inputText[2]);
                        vlnv.setLibrary(inputText[3]);
                        vlnv.setName(inputText[4]);
                        vlnv.setVersion(inputText[5]);

                        consoleTcl_->requestReloadProject();
                        consoleTcl_->requestReloadProjectLibrary();

                        return TCL_OK;
                    }

                    else
                    {
                        QString resultText = "[ WARNING ] | Create | Design is already exist.";
                        outputText.append(resultText);

                        consoleTcl_->appendErrorMessage(resultText);

                        return TCL_OK;
                    }
                }
            }
        }

        else
        {
            QString resultText = "[ INFO ] | Create | Usage : create [[io, ip, design] {Vendor} {Library} {Module Name} {Version} / project {Project Name} {Design Name} {Version}]";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_import()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_import(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Import | Usage : import {Absolute Path}";
        outputText.append(resultText);

        consoleTcl_->appendNoticeMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QString path_RootProject_ = path_RootAMagic + "/project/";

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc > 2)
        {
            QString resultText = "[ INFO ] | Import | Usage : import {Absolute Path}";
            outputText.append(resultText);

            consoleTcl_->appendNoticeMessage(resultText);

            return TCL_OK;
        }

        else
        {
            QFile check_AbsolutePath_Exist(inputText[1]);

            QString check_AbsolutePath_ = inputText[1];

            QStringList importProject_Split = check_AbsolutePath_.split("/");
            int importProject_Split_Length = importProject_Split.length();
            QString importProject_Name = importProject_Split[importProject_Split_Length - 1];

            if (check_AbsolutePath_Exist.exists())
            {
                consoleTcl_->requestImportProject(path_RootProject_, check_AbsolutePath_, importProject_Name);

                QSettings().setValue("Project/DefaultLocation", path_RootProject_ + importProject_Name.replace(".apr", ""));

                consoleTcl_->requestReloadLibrary();
                consoleTcl_->requestReloadProject();
                consoleTcl_->requestReloadProjectLibrary();

                QString resultText = "[ INFO ] | Import | Project imported.";
                outputText.append(resultText);

                consoleTcl_->appendNoticeMessage(resultText);

                return TCL_OK;
            }

            else
            {
                QString resultText = "[ ERROR ] | Import | No file exist.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_export()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_export(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Export | Usage : export {Absolute Path}";
        outputText.append(resultText);

        consoleTcl_->appendNoticeMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc > 2)
        {
            QString resultText = "[ INFO ] | Export | Usage : export {Absolute Path}";
            outputText.append(resultText);

            consoleTcl_->appendNoticeMessage(resultText);

            return TCL_OK;
        }

        else
        {
            QDir check_AbsolutePath_Exist(inputText[1]);

            QString check_AbsolutePath_ = inputText[1];

            QDir check_DefaultProject_Exist(path_DefaultProject);

            QString path_DefaultProject_ = path_DefaultProject;

            QStringList exportProject_Split = path_DefaultProject_.split("/");
            int exportProject_Split_Length = exportProject_Split.length();
            QString exportProject_Name = exportProject_Split[exportProject_Split_Length - 1];

            if (check_DefaultProject_Exist.exists())
            {
                if (check_AbsolutePath_Exist.exists())
                {
                    consoleTcl_->requestExportProject(check_AbsolutePath_, path_DefaultProject_, exportProject_Name);

                    consoleTcl_->requestReloadLibrary();
                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();

                    QString resultText = "[ INFO ] | Export | Project exported.";
                    outputText.append(resultText);

                    consoleTcl_->appendNoticeMessage(resultText);

                    return TCL_OK;
                }

                else
                {
                    QString resultText = "[ ERROR ] | Export | Destination folder is not exist.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }

            else
            {
                QString resultText = "[ ERROR ] | Export | Opened project is not exist.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_open()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_open(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Open | Usage : open [project {Name} / design {Vendor} {Library} {Name} {Version}]";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_RootAMagic_ = path_RootAMagic;
        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (QString(inputText[1]) == "project")
        {
            if (argc < 3 || argc > 3)
            {
                QString resultText = "[ INFO ] | Open | Usage : open [project {Name} / design {Vendor} {Library} {Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                QDir check_ThisProject_Exist(path_RootAMagic_ + "/project/" + inputText[2]);

                QString path_ThisProject_ = path_RootAMagic_ + "/project/" + inputText[2];

                if (check_ThisProject_Exist.exists())
                {
                    QSettings().setValue("Project/DefaultLocation", path_ThisProject_);

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();

                    return TCL_OK;
                }

                else
                {
                    QString resultText = "[ WARNING ] | Open | Can't find the Project.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }

        else if (QString(inputText[1]) == "design")
        {
            if (argc < 6 || argc > 6)
            {
                QString resultText = "[ INFO ] | Open | Usage : open [project {Name} / design {Vendor} {Library} {Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                if (check_DefaultProject_Exist.exists())
                {
                    QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4] + "/" + inputText[5]);

                    if (check_ThisDesign_Exist.exists())
                    {
                        VLNV vlnv;
                        vlnv.setType(VLNV::COMPONENT);
                        vlnv.setVendor(inputText[2]);
                        vlnv.setLibrary(inputText[3]);
                        vlnv.setName(inputText[4]);
                        vlnv.setVersion(inputText[5]);

                        consoleTcl_->requestOpenSchematic(vlnv);

                        return TCL_OK;
                    }

                    else
                    {
                        QString resultText = "[ WARNING ] | Open | Can't find the Design.";
                        outputText.append(resultText);

                        consoleTcl_->appendErrorMessage(resultText);

                        return TCL_OK;
                    }
                }

                else
                {
                    QString resultText = "[ WARNING ] | Open | Can't find the Project.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }

        else if (QString(inputText[1]) == "file")
        {
            if (argc < 3 || argc > 3)
            {
                QString resultText = "[ INFO ] | Open | Usage : open [project {Name} / design {Vendor} {Library} {Name} {Version} / file {Absoulte Path}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                QFile check_ThisFile_Exist(inputText[2]);

                QString path_ThisFile_ = inputText[2];

                if (check_ThisFile_Exist.exists())
                {
                    consoleTcl_->openFile(path_ThisFile_);

                    return TCL_OK;
                }

                else
                {
                    QString resultText = "[ WARNING ] | Open | Can't find the file.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }

        else
        {
            QString resultText = "[ INFO ] | Open | Usage : open [project {Name} / design {Vendor} {Library} {Name} {Version}]";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_close()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_close(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Close | Usage : close [Project / Design {Vendor} {Library} {Name} {Version}]";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (QString(inputText[1]) == "project")
        {
            if (argc < 2 || argc > 2)
            {
                QString resultText = "[ INFO ] | Close | Usage : close [Project / Design {Vendor} {Library} {Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                QSettings().setValue("Project/DefaultLocation", "");

                consoleTcl_->requestCloseAllSchematic();
                consoleTcl_->requestReloadProject();
                consoleTcl_->requestReloadProjectLibrary();

                return TCL_OK;
            }
        }

        else if (QString(inputText[1]) == "design")
        {
            if (argc < 6 || argc > 6)
            {
                QString resultText = "[ INFO ] | Close | Usage : close [Project / Design {Vendor} {Library} {Name} {Version}]";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                VLNV vlnv;
                vlnv.setType(VLNV::COMPONENT);
                vlnv.setVendor(inputText[2]);
                vlnv.setLibrary(inputText[3]);
                vlnv.setName(inputText[4]);
                vlnv.setVersion(inputText[5]);

                consoleTcl_->requestCloseSchematic(vlnv);

                return TCL_OK;
            }
        }

        else
        {
            QString resultText = "[ INFO ] | Close | Usage : close [Project / Design {Vendor} {Library} {Name} {Version}]";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_save()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_save(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    consoleTcl_->requestSave();

    consoleTcl_->requestReloadProject();
    consoleTcl_->requestReloadProjectLibrary();

    return TCL_OK;
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_saveAll()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_saveAll(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    consoleTcl_->requestSaveAll();

    consoleTcl_->requestReloadProject();
    consoleTcl_->requestReloadProjectLibrary();

    return TCL_OK;
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_copyLib()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_copyLib(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Copy Library | Usage : copylib {Vendor} {Library} {Module Name} {Version}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_GeneralLibrary_Exist(path_GeneralLibrary);
        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_RootAMagic_ = path_RootAMagic;
        QString path_GeneralLibrary_ = path_GeneralLibrary;
        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 5 || argc > 5)
        {
            QString resultText = "[ INFO ] | Copy Library | Usage : copylib {Vendor} {Library} {Module Name} {Version}";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_GeneralLibrary_Exist.exists())
            {
                QString path_Target = path_GeneralLibrary_ + "/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4];
                QString path_Dest = path_DefaultProject_ + "/library/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3];

                QDir check_Dest_Exist(path_Dest);

                QStringList mergeParam;

                if (!check_Dest_Exist.exists()) {
                    mergeParam.append("mkdir -p " + path_Dest);
                }

                mergeParam.append("cp -r " + path_Target + " " + path_Dest);

                QStringList runParam;
                runParam << "-c";
                runParam << mergeParam.join("; ");

                QCoreApplication::processEvents();
                scriptProcess.setWorkingDirectory(path_RootAMagic_);
                scriptProcess.start("csh", runParam);
                scriptProcess.waitForFinished(-1);

                QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                    consoleTcl_->pythonOutputReader(&scriptProcess);
                });

                consoleTcl_->requestReloadLibrary();

                return TCL_OK;
            }

            else if (!check_GeneralLibrary_Exist.exists())
            {
                QString resultText = "[ WARNING ] | Copy Library | No Library is Available.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else if (!check_DefaultProject_Exist.exists())
            {
                QString resultText = "[ WARNING ] | Copy Library | No Project is Available.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }

            else
            {
                QString resultText = "[ ERROR ] | Copy Library | Something is wrong.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_putLib()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_putLib(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Put Library | Usage : putlib {Design Vendor} {Design Library} {Design Name} {Design Version} {Vendor} {Library} {Name} {Version}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 9 || argc > 9)
        {
            QString resultText = "[ INFO ] | Put Library | Usage : putlib {Design Vendor} {Design Library} {Design Name} {Design Version} {Vendor} {Library} {Name} {Version}";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_DefaultProject_Exist.exists())
            {
                QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4]);
                QDir check_ThisLibrary_Exist(path_DefaultProject_ + "/library/" + inputText[5] + "/" + inputText[6] + "/" + inputText[7] + "/" + inputText[8]);

                if (check_ThisDesign_Exist.exists() && check_ThisLibrary_Exist.exists())
                {
                    QStringList setprjParam;
                    setprjParam.append("source " + path_DefaultProject_ + "/setprj");

                    QStringList moveScript;
                    moveScript.append("cd " + path_DefaultProject_ + "/script/put_library");

                    QStringList scriptParam;
                    scriptParam.append("-dvendor");
                    scriptParam.append(inputText[1]);
                    scriptParam.append("-dlibrary");
                    scriptParam.append(inputText[2]);
                    scriptParam.append("-dname");
                    scriptParam.append(inputText[3]);
                    scriptParam.append("-dversion");
                    scriptParam.append(inputText[4]);
                    scriptParam.append("-vendor");
                    scriptParam.append(inputText[5]);
                    scriptParam.append("-library");
                    scriptParam.append(inputText[6]);
                    scriptParam.append("-name");
                    scriptParam.append(inputText[7]);
                    scriptParam.append("-version");
                    scriptParam.append(inputText[8]);

                    QStringList mergeParam;
                    mergeParam.append(setprjParam);
                    mergeParam.append(moveScript);
                    mergeParam.append(path_DefaultProject_ + "/bin/awsh python -u put_library.py " + scriptParam.join(" "));

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_DefaultProject_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    VLNV vlnv;
                    vlnv.setType(VLNV::COMPONENT);
                    vlnv.setVendor(inputText[1]);
                    vlnv.setLibrary(inputText[2]);
                    vlnv.setName(inputText[3]);
                    vlnv.setVersion(inputText[4]);

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();
                    consoleTcl_->requestRefreshSchematic(vlnv);

                    return TCL_OK;
                }

                else {
                    QString resultText = "[ INFO ] | Put Library | Usage : putlib {Design Vendor} {Design Library} {Design Name} {Design Version} {Vendor} {Library} {Name} {Version}";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_autoCon()
// NOTE : 자동 연결이 가능한가? 어디에 뭘 연결할지 어떻게 알고?
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_autoCon(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    QString resultText = ("[ SYSTEM ] | Auto Connector | Custom Command (Auto Connector) is Avaliable.");
    outputText.append(resultText);

    consoleTcl_->appendNoticeMessage(resultText);

    return TCL_OK;
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_autoTop()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_autoTop(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Auto Top | Usage : autotop {Vendor} {Library} {Name} {Version}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 5 || argc > 5)
        {
            QString resultText = "[ INFO ] | Auto Top | Usage : autotop {Vendor} {Library} {Name} {Version}";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_DefaultProject_Exist.exists())
            {
                QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4]);

                if (check_ThisDesign_Exist.exists())
                {
                    QStringList setprjParam;
                    setprjParam.append("source " + path_DefaultProject_ + "/setprj");

                    QStringList moveScript;
                    moveScript.append("cd " + path_DefaultProject_ + "/script/top_port_create_n_connect_gen_by_tony");

                    QStringList scriptParam;
                    scriptParam.append("-vendor");
                    scriptParam.append(inputText[1]);
                    scriptParam.append("-lib-name");
                    scriptParam.append(inputText[2]);
                    scriptParam.append("-module-name");
                    scriptParam.append(inputText[3]);
                    scriptParam.append("-version");
                    scriptParam.append(inputText[4]);

                    QStringList mergeParam;
                    mergeParam.append(setprjParam);
                    mergeParam.append(moveScript);
                    mergeParam.append(path_DefaultProject_ + "/bin/awsh python -u script_gen_bus_if_8.py " + scriptParam.join(" "));

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_DefaultProject_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    VLNV vlnv;
                    vlnv.setType(VLNV::COMPONENT);
                    vlnv.setVendor(inputText[1]);
                    vlnv.setLibrary(inputText[2]);
                    vlnv.setName(inputText[3]);
                    vlnv.setVersion(inputText[4]);

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();
                    consoleTcl_->requestRefreshSchematic(vlnv);

                    return TCL_OK;
                }
            }

            else {
                QString resultText = "[ WARNING ] | Auto Top | No Project is Avaliable.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_addTop()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_addTop(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Add Top | Usage : addtop {Design Vendor} {Design Library} {Design Name} {Design Version} {I/O Vendor} {I/O Library} {I/O Name} {I/O Version} {Mode} {Top Port Name}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 11 || argc > 11)
        {
            QString resultText = "[ INFO ] | Add Top | Usage : addtop {Design Vendor} {Design Library} {Design Name} {Design Version} {I/O Vendor} {I/O Library} {I/O Name} {I/O Version} {Mode} {Top Port Name}";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_DefaultProject_Exist.exists())
            {
                QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4]);

                if (check_ThisDesign_Exist.exists())
                {
                    QStringList setprjParam;
                    setprjParam.append("source " + path_DefaultProject_ + "/setprj");

                    QStringList moveScript;
                    moveScript.append("cd " + path_DefaultProject_ + "/script/add_top");

                    QStringList scriptParam;
                    scriptParam.append("-dvendor");
                    scriptParam.append(inputText[1]);
                    scriptParam.append("-dlibrary");
                    scriptParam.append(inputText[2]);
                    scriptParam.append("-dname");
                    scriptParam.append(inputText[3]);
                    scriptParam.append("-dversion");
                    scriptParam.append(inputText[4]);
                    scriptParam.append("-vendor");
                    scriptParam.append(inputText[5]);
                    scriptParam.append("-library");
                    scriptParam.append(inputText[6]);
                    scriptParam.append("-name");
                    scriptParam.append(inputText[7]);
                    scriptParam.append("-version");
                    scriptParam.append(inputText[8]);
                    scriptParam.append("-mode");
                    scriptParam.append(inputText[9]);
                    scriptParam.append("-display-name");
                    scriptParam.append(inputText[10]);

                    QStringList mergeParam;
                    mergeParam.append(setprjParam);
                    mergeParam.append(moveScript);
                    mergeParam.append(path_DefaultProject_ + "/bin/awsh python -u add_top.py " + scriptParam.join(" "));

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_DefaultProject_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    VLNV vlnv;
                    vlnv.setType(VLNV::COMPONENT);
                    vlnv.setVendor(inputText[1]);
                    vlnv.setLibrary(inputText[2]);
                    vlnv.setName(inputText[3]);
                    vlnv.setVersion(inputText[4]);

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();
                    consoleTcl_->requestCloseSchematic(vlnv);
                    consoleTcl_->requestOpenSchematic(vlnv);

                    return TCL_OK;
                }

                else {
                    QString resultText = "[ WARNING ] | Auto Top | No Design is Avaliable.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }

            else {
                QString resultText = "[ WARNING ] | Auto Top | No Project is Avaliable.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_connect()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_connect(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Connect | Usage : connect {Design Vendor} {Design Library} {Design Name} {Design Version} [ioip (ipio) / ipip] [{IP Name} / topio] [{IP Port Name} / {Top Port Name}] [{IP Name} / topio] [{IP Port Name} / {Top Port Name}]";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 10 || argc > 10)
        {
            QString resultText = "[ INFO ] | Connect | Usage : connect {Design Vendor} {Design Library} {Design Name} {Design Version} [ioip (ipio) / ipip] [{IP Name} / topio] [{IP Port Name} / {Top Port Name}] [{IP Name} / topio] [{IP Port Name} / {Top Port Name}]";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_DefaultProject_Exist.exists())
            {
                QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4]);

                if (check_ThisDesign_Exist.exists())
                {
                    QStringList setprjParam;
                    setprjParam.append("source " + path_DefaultProject_ + "/setprj");

                    QStringList moveScript;
                    moveScript.append("cd " + path_DefaultProject_ + "/script/connect_from_io_ip_to_ip_by_tony");

                    QStringList scriptParam;
                    scriptParam.append("-vendor");
                    scriptParam.append(inputText[1]);
                    scriptParam.append("-lib-name");
                    scriptParam.append(inputText[2]);
                    scriptParam.append("-module-name");
                    scriptParam.append(inputText[3]);
                    scriptParam.append("-version");
                    scriptParam.append(inputText[4]);
                    scriptParam.append("-type");
                    scriptParam.append(inputText[5]);
                    scriptParam.append("-fip");
                    scriptParam.append(inputText[6]);
                    scriptParam.append("-fio");
                    scriptParam.append(inputText[7]);
                    scriptParam.append("-tip");
                    scriptParam.append(inputText[8]);
                    scriptParam.append("-tio");
                    scriptParam.append(inputText[9]);

                    QStringList mergeParam;
                    mergeParam.append(setprjParam);
                    mergeParam.append(moveScript);
                    mergeParam.append(path_DefaultProject_ + "/bin/awsh python -u connect.py " + scriptParam.join(" "));

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_DefaultProject_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    VLNV vlnv;
                    vlnv.setType(VLNV::COMPONENT);
                    vlnv.setVendor(inputText[1]);
                    vlnv.setLibrary(inputText[2]);
                    vlnv.setName(inputText[3]);
                    vlnv.setVersion(inputText[4]);

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();
                    consoleTcl_->requestRefreshSchematic(vlnv);

                    return TCL_OK;
                }

                else {
                    QString resultText = "[ WARNING ] | Connect | No Design is Avaliable.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }

            else {
                QString resultText = "[ WARNING ] | Connect | No Project is Avaliable.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_runSim()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_runSim(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | Simulation | Usage : runsim {Vendor} {Library} {Module Name} {Version}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 5 || argc > 5)
        {
            QString resultText = "[ INFO ] | Simulation | Usage : runsim {Vendor} {Library} {Module Name} {Version}";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_DefaultProject_Exist.exists())
            {
                QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4]);

                if (check_ThisDesign_Exist.exists())
                {
                    QStringList setprjParam;
                    setprjParam.append("source " + path_DefaultProject_ + "/setprj");

                    QStringList moveParamScript;
                    moveParamScript.append("cd " + path_DefaultProject_ + "/script/merge_script");

                    QStringList designParam;
                    designParam.append("-vendor");
                    designParam.append(inputText[1]);
                    designParam.append("-lib-name");
                    designParam.append(inputText[2]);
                    designParam.append("-module-name");
                    designParam.append(inputText[3]);
                    designParam.append("-version");
                    designParam.append(inputText[4]);

                    QStringList moveParamRegress;
                    moveParamRegress.append("cd " + path_DefaultProject_ + "/env/simulation/regress");

                    QStringList simParam;
                    simParam.append(path_DefaultProject_ + "/bin/awsh make regress");

                    QStringList mergeParam;
                    mergeParam.append(setprjParam);
                    mergeParam.append(moveParamScript);
                    mergeParam.append(path_DefaultProject_ + "/bin/awsh python -u merge_script_6.py " + designParam.join(" "));
                    mergeParam.append(moveParamRegress);
                    mergeParam.append(simParam);

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_DefaultProject_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();

                    return TCL_OK;
                }

                else {
                    QString resultText = "[ WARNING ] | Simulation | No Design is Avaliable.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }

            else
            {
                QString resultText = "[ WARNING ] | Simulation | No Project is Avaliable.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_runSynth()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_runSynth(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    QString resultText = ("[ SYSTEM ] | Synthesis | Custom Command (Synthesis) is Avaliable.");
    outputText.append(resultText);

    consoleTcl_->appendNoticeMessage(resultText);

    return TCL_OK;
}



//-----------------------------------------------------------------------------
// Function: CustomCommand : ccTCL_runFPGA()
//-----------------------------------------------------------------------------
int ConsoleTcl::ccTCL_runFPGA(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
{
    ConsoleTcl *consoleTcl_;

    if (argc < 2)
    {
        QString resultText = "[ INFO ] | FPGA Bit Stream Gen. | Usage : runfpga {Vendor} {Library} {Module Name} {Version}";
        outputText.append(resultText);

        consoleTcl_->appendErrorMessage(resultText);

        return TCL_OK;
    }

    else
    {
        QStringList inputText;

        QDir check_DefaultProject_Exist(path_DefaultProject);

        QString path_DefaultProject_ = path_DefaultProject;

        for (int i = 0; i < argc; ++i)
        {
            inputText.append(argv[i]);
        }

        if (argc < 5 || argc > 5)
        {
            QString resultText = "[ INFO ] | FPGA Bit Stream Gen. | Usage : runfpga {Vendor} {Library} {Module Name} {Version}";
            outputText.append(resultText);

            consoleTcl_->appendErrorMessage(resultText);

            return TCL_OK;
        }

        else
        {
            if (check_DefaultProject_Exist.exists())
            {
                QDir check_ThisDesign_Exist(path_DefaultProject_ + "/designs/" + inputText[1] + "/" + inputText[2] + "/" + inputText[3] + "/" + inputText[4]);

                if (check_ThisDesign_Exist.exists())
                {
                    QStringList setprjParam;
                    setprjParam.append("source " + path_DefaultProject_ + "/setprj");

                    QStringList moveParamScript;
                    moveParamScript.append("cd " + path_DefaultProject_ + "/script/merge_script");

                    QStringList designParam;
                    designParam.append("-vendor");
                    designParam.append(inputText[1]);
                    designParam.append("-lib-name");
                    designParam.append(inputText[2]);
                    designParam.append("-module-name");
                    designParam.append(inputText[3]);
                    designParam.append("-version");
                    designParam.append(inputText[4]);

                    QStringList moveParam;
                    moveParam.append("cd " + path_DefaultProject_ + "/env/fpga");

                    QStringList fpgaParam;
                    fpgaParam.append(path_DefaultProject_ + "/bin/awsh vivado -source fpga_bit_gen_" + inputText[3] + ".tcl");

                    QStringList mergeParam;
                    mergeParam.append(path_DefaultProject_ + "/bin/awsh python -u merge_script_6.py " + designParam.join(" "));
                    mergeParam.append(moveParam);
                    mergeParam.append(fpgaParam);

                    QStringList runParam;
                    runParam << "-c";
                    runParam << mergeParam.join("; ");

                    QCoreApplication::processEvents();
                    scriptProcess.setWorkingDirectory(path_DefaultProject_);
                    scriptProcess.setProcessChannelMode(QProcess::MergedChannels);
                    scriptProcess.start("csh", runParam);

                    QObject::connect(&scriptProcess, &QProcess::readyReadStandardOutput, [=]() {
                        consoleTcl_->pythonOutputReader(&scriptProcess);
                    });

                    consoleTcl_->requestReloadProject();
                    consoleTcl_->requestReloadProjectLibrary();

                    return TCL_OK;
                }

                else {
                    QString resultText = "[ WARNING ] | FPGA Bit Stream Gen. | No Design is Avaliable.";
                    outputText.append(resultText);

                    consoleTcl_->appendErrorMessage(resultText);

                    return TCL_OK;
                }
            }

            else
            {
                QString resultText = "[ WARNING ] | FPGA Bit Stream Gen. | No Project is Avaliable.";
                outputText.append(resultText);

                consoleTcl_->appendErrorMessage(resultText);

                return TCL_OK;
            }
        }
    }
}
