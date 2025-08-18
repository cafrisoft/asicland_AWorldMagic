#include <QApplication>
#include <QSettings>

#include <AWorldMagicAPI/ProjectLibraryHandler.h>
#include <AWorldMagicAPI/FPGAHandler.h>

#include <KactusAPI/KactusAPI.h>

#include <KactusAPI/ProjectHandler.h>
#include <KactusAPI/LibraryHandler.h>
#include <KactusAPI/PluginManager.h>
#include <KactusAPI/PluginUtilityAdapter.h>
#include <KactusAPI/VersionHelper.h>

#include <UI/GraphicalMessageMediator/GraphicalMessageMediator.h>

#include <UI/Settings/SettingsUpdater.h>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // Set the identification tags for the application.
//    QCoreApplication::setOrganizationDomain(QStringLiteral("tut.fi"));
//    QCoreApplication::setOrganizationName(QStringLiteral("TUT"));
//    QCoreApplication::setApplicationName(QStringLiteral("Kactus2"));
//    QCoreApplication::setApplicationVersion(VersionHelper::createVersionString());

    QScopedPointer<MessageMediator> mediator(new GraphicalMessageMediator());

    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    //    SettingsUpdater su;
    //    su.runUpgrade(settings, mediator.data());
    SettingsUpdater::runUpgrade(settings, mediator.data());

    QStringList pluginsPath = settings.value(QStringLiteral("Platform/PluginsPath"),
        QStringList(QStringLiteral("Plugins"))).toStringList();
    PluginManager::getInstance().setPluginPaths(pluginsPath);

    ProjectHandler::initialize(mediator.data());
    QScopedPointer<ProjectHandler> project(ProjectHandler::getInstance());

    ProjectLibraryHandler::initialize(mediator.data());
    QScopedPointer<ProjectLibraryHandler> projectLibrary(ProjectLibraryHandler::getInstance());

    FPGAHandler::initialize(mediator.data());
    QScopedPointer<FPGAHandler> fpga(FPGAHandler::getInstance());

    LibraryHandler::initialize(mediator.data());
    QScopedPointer<LibraryHandler> library(LibraryHandler::getInstance());

    QScopedPointer<KactusAPI> coreAPI(new KactusAPI(project.data(), projectLibrary.data(), library.data(), mediator.data()));

    //    MainWindow w;
    MainWindow w(project.data(), projectLibrary.data(), fpga.data(), library.data(), mediator.data());
    //    w.show();
    w.showMaximized();


    a.processEvents();
    w.onLibrarySearch();

    return a.exec();
}
