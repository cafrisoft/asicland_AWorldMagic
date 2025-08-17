//-----------------------------------------------------------------------------
// File: DockWidgetHandler.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 10.10.2017
//
// Description:
// Manages the dockable widgets for the main window.
//-----------------------------------------------------------------------------
#ifndef USE_AWMWIDGET
#define USE_AWMWIDGET 1
#endif // USE_AWMWIDGET

#ifndef USE_CONFIGURATIONWINDOW
#define USE_CONFIGURATIONWINDOW 0
#endif // USE_CONFIGURATIONWINDOW

#ifndef USE_INTERFACEWINDOW
#define USE_INTERFACEWINDOW 0
#endif // USE_INTERFACEWINDOW

#ifndef USE_CONNECTIONWINDOW
#define USE_CONNECTIONWINDOW 0
#endif // USE_CONNECTIONWINDOW

#ifndef USE_ADHOC_WINDOW
#define USE_ADHOC_WINDOW 0
#endif // USE_ADHOC_WINDOW

#ifndef USE_DESIGNPARAMETERSWINDOW
#define USE_DESIGNPARAMETERSWINDOW 0
#endif // USE_DESIGNPARAMETERSWINDOW

#include <QList>
#include <QString>
//#include <QHelpEngine>
#include <QApplication>
#include <QSizePolicy>

#include <AWorldMagicAPI/ProjectLibraryHandler.h>
#include <AWorldMagicAPI/FPGAHandler.h>

#include <CommandWindow/LogOut.h>
#include <CommandWindow/ConsolLogTab.h>
#include <CommandWindow/MessageConsole.h>
#include <CommandWindow/ConsoleManager.h>

//#include <Help/HelpSystem/HelpWindow.h>
//#include <Help/HelpSystem/ContextHelpBrowser.h>

#include <KactusAPI/ProjectHandler.h>
#include <KactusAPI/LibraryHandler.h>
#include <KactusAPI/ListParameterFinder.h>
#include <KactusAPI/ExpressionFormatter.h>
#include <KactusAPI/MultipleParameterFinder.h>
#include <KactusAPI/ParametersInterface.h>
#include <KactusAPI/ComponentParameterFinder.h>

#include <Common/ui/GraphicalMessageMediator.h>

//#include <UI/Widgets/ParameterGroupBox/parametergroupbox.h>

#include <UI/Common/graphicsItems/ComponentItem.h>
#include <UI/Common/graphicsItems/ConnectionEndpoint.h>
#if USE_AWMWIDGET == 0
#include <UI/LibraryWidget/LibraryWidget.h>
#else
#include <UI/AWMWidget/AWMWidget.h>
#endif
#include <UI/ComponentEditor/componenteditor.h>
#include <UI/ComponentEditor/parameterReferenceTree/ParameterReferenceTreeWindow.h>
//#include <editors/common/DesignParameterReferenceTree/DesignParameterReferenceCounter.h>
//#include <editors/common/DesignParameterReferenceTree/DesignParameterReferenceTree.h>
#include <UI/Common/ComponentInstanceEditor/ComponentInstanceEditor.h>

#if USE_CONFIGURATIONWINDOW == 1
#include <editors/common/ConfigurationEditor/configurationeditor.h>
#endif // #if USE_CONFIGURATIONWINDOW == 1

#if USE_INTERFACEWINDOW == 1
#include <editors/common/InterfaceEditor/interfaceeditor.h>

#include <UI/I.h>
#endif // #if USE_INTERFACEWINDOW == 1

#if USE_CONNECTIONWINDOW == 1
#include <editors/common/ConnectionEditor/connectioneditor.h>
#endif // #if USE_CONNECTIONWINDOW == 1

//#include <editors/common/VendorExtensionEditor/VendorExtensionsEditor.h>

#include <UI/HWDesign/HWDesignWidget.h>
#include <UI/HWDesign/HWDesignDiagram.h>

#if USE_ADHOC_WINDOW == 1
#include <UI/HWDesign/AdHocItem.h>
#include <UI/HWDesign/AdhocEditor/AdhocEditor.h>
#endif // #if USE_ADHOC_WINDOW == 1

//#include <editors/SystemDesign/SystemDesignWidget.h>
//#include <editors/SystemDesign/SystemDetailsEditor/SystemDetailsEditor.h>

#include "DockWidgetHandler.h"

namespace 
{
    const QMap<TabDocument::SupportedWindows, QString> DOCK_SETTINGS =
    {
#if USE_CONNECTIONWINDOW == 1
        { TabDocument::CONFIGURATIONWINDOW, "ConfigurationVisibility" },
#endif // #if USE_CONNECTIONWINDOW == 1
//        { TabDocument::SYSTEM_DETAILS_WINDOW, "SystemDetailsVisibility" },
#if USE_CONNECTIONWINDOW == 1
        { TabDocument::CONNECTIONWINDOW, "ConnectionVisibility" },
#endif // #if USE_CONNECTIONWINDOW == 1
        { TabDocument::INSTANCEWINDOW, "InstanceVisibility" },
#if USE_ADHOC_WINDOW == 1
        { TabDocument::ADHOC_WINDOW, "AdHocEditorVisibility" },
#endif // #if USE_ADHOC_WINDOW == 1
#if USE_INTERFACEWINDOW == 1
        { TabDocument::INTERFACEWINDOW, "InterfaceVisibility" },
#endif // #if USE_INTERFACEWINDOW == 1
//        { TabDocument::LIBRARYWINDOW, "LibraryVisibility" },
        { TabDocument::CONSOLEWINDOW, "ConsoleVisibility" },
        { TabDocument::OUTPUTWINDOW, "OutputVisibility" },
        { TabDocument::REPORTWINDOW, "ReportVisibility" },
//        { TabDocument::CONTEXT_HELP_WINDOW, "ContextHelpVisibility" },
#if USE_DESIGNPARAMETERSWINDOW == 1
        { TabDocument::DESIGNPARAMETERSWINDOW, "DesignParameterVisibility" },
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1
//        { TabDocument::VENDOREXTENSIONWINDOW, "VendorExtensionVisibility" }
    };
};

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::DockWidgetHandler()
//-----------------------------------------------------------------------------
DockWidgetHandler::DockWidgetHandler(ProjectHandler* project,
                                     ProjectLibraryHandler* projectLibrary,
                                     FPGAHandler* fpga,
                                     LibraryHandler* library,
                                     MessageMediator* messageChannel,
                                     //    QToolBar* leftToolbar,
                                     QToolBar* rightToolbar,
                                     QMainWindow* parent) :
    projectHandler_(project),
    projectLibraryHandler_(projectLibrary),
    fpgaHandler_(fpga),
    libraryHandler_(library),
    messageChannel_(messageChannel),
  #if LIBRARYDOCK_DISABLE_20231229 == 0
    libraryDock_(0),
  #endif
    libraryWidget_(0),
    consoleWindow_(0),
    consoleWindowDock_(0),
    logWindow_(0),
    logWindowDock_(0),
    ReportWindow_(0),
    ReportWindowDock_(0),
//    contextHelpBrowser_(0),
//    contextHelpDock_(0),
#if USE_DESIGNPARAMETERSWINDOW == 1
    designParameterReferenceCounter_(0),
    designParametersEditor_(0),
    designParameterDock_(0),
    designParameterTree_(0),
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1
    designParameterFinder_(new ListParameterFinder()),
    designAndInstancesParameterFinder_(new MultipleParameterFinder()),
    instanceEditor_(0),
    instanceDock_(0),
#if USE_ADHOC_WINDOW == 1
    adhocEditor_(0),
    adhocDock_(0),
#endif // #if USE_ADHOC_WINDOW == 1
#if USE_CONFIGURATIONWINDOW == 1
    configurationEditor_(0),
    configurationDock_(0),
#endif // #if USE_CONFIGURATIONWINDOW == 1
//    systemDetailsEditor_(0),
//    systemDetailsDock_(0),
#if USE_INTERFACEWINDOW == 1
    interfaceEditor_(0),
    interfaceDock_(0),
#endif // #if USE_INTERFACEWINDOW == 1
#if USE_CONNECTIONWINDOW == 1
    connectionEditor_(0),
    connectionDock_(0),
#endif // #if USE_CONNECTIONWINDOW == 1
//    extensionDock_(0),
//    extensionEditor_(0),
//    helpWnd_(0),
    docks_(),
    visibilityControls_(),
    visibilities_(),
//    leftToolbar_(leftToolbar),
    rightToolbar_(rightToolbar),
//    leftActions_(new QActionGroup(this)),
    rightActions_(new QActionGroup(this)),
    mainWindow_(parent)
{
//    leftActions_->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
//    rightActions_->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

//    leftActions_->setExclusive(true);
    rightActions_->setExclusive(true);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupDockWidgets()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupDockWidgets()
{
    setupConsoleWindow();
    setupLogWindow();
    setupReportWindow();

////    setupContextHelp();
///
#if DOCKWIDGETHANDLER_20231228 == 1
    setupLibraryDock();
#endif

    setupDesignParametersEditor();
    setupInstanceEditor();
#if USE_ADHOC_WINDOW == 1
    setupAdHocEditor();
#endif // #if USE_ADHOC_WINDOW == 1
#if USE_CONNECTIONWINDOW == 1
    setupConfigurationEditor();
#endif // #if USE_CONNECTIONWINDOW == 1
//    setupSystemDetailsEditor();
//    setupInterfaceEditor();
#if USE_CONNECTIONWINDOW == 1
    setupConnectionEditor();
#endif // #if USE_CONNECTIONWINDOW == 1
//    setupVendorExtensionEditor();

    docks_ =
    {
#if USE_CONNECTIONWINDOW == 1
        { TabDocument::CONFIGURATIONWINDOW, configurationDock_ },
#endif // #if USE_CONNECTIONWINDOW == 1
//        { TabDocument::SYSTEM_DETAILS_WINDOW, systemDetailsDock_ },
#if USE_CONNECTIONWINDOW == 1
        { TabDocument::CONNECTIONWINDOW, connectionDock_ },
#endif // #if USE_CONNECTIONWINDOW == 1
        { TabDocument::INSTANCEWINDOW, instanceDock_ },
#if USE_ADHOC_WINDOW == 1
        { TabDocument::ADHOC_WINDOW, adhocDock_ },
#endif // #if USE_ADHOC_WINDOW == 1
#if USE_INTERFACEWINDOW == 1
        { TabDocument::INTERFACEWINDOW, interfaceDock_ },
#endif // #if USE_INTERFACEWINDOW == 1
//        { TabDocument::LIBRARYWINDOW, libraryDock_ },
        { TabDocument::CONSOLEWINDOW, consoleWindowDock_ },
        { TabDocument::OUTPUTWINDOW, logWindowDock_ },
        { TabDocument::REPORTWINDOW, ReportWindowDock_ }
//        { TabDocument::CONTEXT_HELP_WINDOW, contextHelpDock_ },
    #if USE_DESIGNPARAMETERSWINDOW == 1
            { TabDocument::DESIGNPARAMETERSWINDOW, designParameterDock_ },
    #endif // #if USE_DESIGNPARAMETERSWINDOW == 1
//        { TabDocument::VENDOREXTENSIONWINDOW, extensionDock_ }
    };

    // Action 설정 및 레이아웃 상 보여주는 컨트롤 설정
    for (auto const& dock : docks_)
    {
        auto viewAction = dock->toggleViewAction();
        viewAction->setIcon(dock->windowIcon());
        viewAction->setCheckable(true);

        connect(viewAction, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)), Qt::UniqueConnection);
        connect(dock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)), Qt::UniqueConnection);

        auto visibilityAction = new QAction(this);
        visibilityAction->setText(dock->windowTitle());
        visibilityAction->setCheckable(true);

        visibilityControls_.insert(docks_.key(dock), visibilityAction);
    }

    QAction* action = docks_[TabDocument::INSTANCEWINDOW]->toggleViewAction();

    rightActions_->addAction(action);
    rightToolbar_->addAction(action);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::addVisibilityActions()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupVisibilityActionMenu(QMenu& visibilityMenu)
{
    for (auto const& control : visibilityControls_)
    {
        visibilityMenu.addAction(control);
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::getProjectHandler()
//-----------------------------------------------------------------------------
ProjectHandler* DockWidgetHandler::getProjectHandler() const
{
    return projectHandler_;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::getProjectLibraryHandler()
//-----------------------------------------------------------------------------
ProjectLibraryHandler* DockWidgetHandler::getProjectLibraryHandler() const
{
    return projectLibraryHandler_;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::getFPGAHandler()
//-----------------------------------------------------------------------------
FPGAHandler* DockWidgetHandler::getFPGAHandler() const
{
    return fpgaHandler_;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::getLibraryHandler()
//-----------------------------------------------------------------------------
LibraryHandler* DockWidgetHandler::getLibraryHandler() const
{
    return libraryHandler_;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::loadVisiblities()
//-----------------------------------------------------------------------------
void DockWidgetHandler::loadVisiblities(QSettings const& settings)
{
    for (auto i = DOCK_SETTINGS.cbegin(); i != DOCK_SETTINGS.cend(); ++i)
    {
        const bool VISIBLE = settings.value(i.value(), true).toBool();
        const auto TYPE = i.key();

        visibilities_.insert(TYPE, VISIBLE);
        visibilityControls_[TYPE]->setChecked(VISIBLE);
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::saveVisibilitySettings()
//-----------------------------------------------------------------------------
void DockWidgetHandler::saveVisibilitySettings(QSettings& settings) const
{
    for (auto i = DOCK_SETTINGS.cbegin(); i != DOCK_SETTINGS.cend(); ++i)
    {
        settings.setValue(i.value(), visibilities_.value(i.key()));
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::loadFilterSettings()
//-----------------------------------------------------------------------------
void DockWidgetHandler::loadFilterSettings(QSettings& settings)
{
//    libraryWidget_->loadFilterSettings(settings);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::saveFilterSettings()
//-----------------------------------------------------------------------------
void DockWidgetHandler::saveFilterSettings(QSettings& settings) const
{
//    libraryWidget_->saveFilterSettings(settings);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::applySettings()
//-----------------------------------------------------------------------------
void DockWidgetHandler::applySettings()
{
    // Nothing to do.
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupConsoleWindow()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupConsoleWindow()
{
    consoleWindowDock_ = new QDockWidget(tr("Console"), mainWindow_);
    consoleWindowDock_->setVisible(true);
    consoleWindowDock_->setMinimumSize(200, 100);
    consoleWindowDock_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    consoleWindowDock_->setObjectName(tr("Console"));
//    consoleWindowDock_->setWindowIcon(QIcon(":resources/Res/log.png"));
//    consoleWindowDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    consoleWindowDock_->setAllowedAreas(Qt::BottomDockWidgetArea);
    consoleWindowDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    consoleWindow_ = new ConsolLogTab(consoleWindowDock_);
    consoleWindowDock_->setWidget(consoleWindow_);
    mainWindow_->addDockWidget(Qt::BottomDockWidgetArea, consoleWindowDock_);

//    GraphicalMessageMediator* guiChannel = dynamic_cast<GraphicalMessageMediator*>(messageChannel_);

    // Connect Signal from Console
    connect(consoleWindow_, SIGNAL(reloadLibrary()), this, SIGNAL(reloadLibrary()));
    connect(consoleWindow_, SIGNAL(reloadProject()), this, SIGNAL(reloadProject()));
    connect(consoleWindow_, SIGNAL(reloadProjectLibrary()), this, SIGNAL(reloadProjectLibrary()));
    connect(consoleWindow_, SIGNAL(openSchematic(VLNV const&)), this, SIGNAL(openSchematic(VLNV const&)));
    connect(consoleWindow_, SIGNAL(refreshSchematic(VLNV const&)), this, SIGNAL(refreshSchematic(VLNV const&)));
    connect(consoleWindow_, SIGNAL(closeSchematic(VLNV const&)), this, SIGNAL(closeSchematic(VLNV const&)));
    connect(consoleWindow_, SIGNAL(closeAllSchematic()), this, SIGNAL(closeAllSchematic()));
    connect(consoleWindow_, SIGNAL(openFile(QString)), this, SIGNAL(openFile(QString)));
    connect(consoleWindow_, SIGNAL(requestSave()), this, SIGNAL(requestSave()));
    connect(consoleWindow_, SIGNAL(requestSaveAll()), this, SIGNAL(requestSaveAll()));
    connect(consoleWindow_, SIGNAL(requestImportProject(QString const&, QString const&, QString const&)), this, SLOT(onImportProject_Console(QString const&, QString const&, QString const&)));
    connect(consoleWindow_, SIGNAL(requestExportProject(QString const&, QString const&, QString const&)), this, SLOT(onExportProject_Console(QString const&, QString const&, QString const&)));

    connect(consoleWindow_, SIGNAL(inputMessage(QString const&)), this, SLOT(onLogInputMessage(QString const&)));
    connect(consoleWindow_, SIGNAL(noticeMessage(QString const&)), this, SLOT(onLogNoticeMessage(QString const&)));
    connect(consoleWindow_, SIGNAL(errorMessage(QString const&)), this, SLOT(onLogErrorMessage(QString const&)));
    connect(consoleWindow_, SIGNAL(undefinedMessage(QString const&)), this, SLOT(onLogUndefinedMessage(QString const&)));

    connect(consoleWindow_, SIGNAL(noticeMessage(QString const&)), this, SLOT(onReportNoticeMessage(QString const&)));
    connect(consoleWindow_, SIGNAL(errorMessage(QString const&)), this, SLOT(onReportErrorMessage(QString const&)));
    connect(consoleWindow_, SIGNAL(undefinedMessage(QString const&)), this, SLOT(onReportUndefinedMessage(QString const&)));
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupLogWindow()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupLogWindow()
{
    logWindowDock_ = new QDockWidget(tr("Log"), mainWindow_);
    logWindowDock_->setVisible(true);
    logWindowDock_->setMinimumSize(200, 100);
    logWindowDock_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    logWindowDock_->setObjectName(tr("Log"));
//    logWindowDock_->setWindowIcon(QIcon(":resources/Res/log.png"));
//    logWindowDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    logWindowDock_->setAllowedAreas(Qt::BottomDockWidgetArea);
    logWindowDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    logWindow_ = new LogOut(logWindowDock_);
    logWindowDock_->setWidget(logWindow_);
    mainWindow_->addDockWidget(Qt::BottomDockWidgetArea, logWindowDock_);

    mainWindow_->tabifyDockWidget(consoleWindowDock_, logWindowDock_);

//    connect(this, SIGNAL(inputMessage_LogOut(const QString&)),
//        logWindow_, SLOT(onInputMessage(const QString&)), Qt::UniqueConnection);
//    connect(this, SIGNAL(noticeMessage_LogOut(const QString&)),
//        logWindow_, SLOT(onNoticeMessage(const QString&)), Qt::UniqueConnection);
//    connect(this, SIGNAL(errorMessage_LogOut(const QString&)),
//        logWindow_, SLOT(onErrorMessage(const QString&)), Qt::UniqueConnection);

    connect(this, SIGNAL(inputMessage(QString const&)), this, SLOT(onLogInputMessage(QString const&)));
    connect(this, SIGNAL(noticeMessage(QString const&)), this, SLOT(onLogNoticeMessage(QString const&)));
    connect(this, SIGNAL(errorMessage(QString const&)), this, SLOT(onLogErrorMessage(QString const&)));
    connect(this, SIGNAL(undefinedMessage(QString const&)), this, SLOT(onLogUndefinedMessage(QString const&)));

    auto guiChannel = dynamic_cast<GraphicalMessageMediator*>(messageChannel_);

    if (guiChannel)
    {
        connect(guiChannel, SIGNAL(inputMessage(QString const&)), this, SLOT(onLogInputMessage(QString const&)));
        connect(guiChannel, SIGNAL(noticeMessage(QString const&)), this, SLOT(onLogNoticeMessage(QString const&)));
        connect(guiChannel, SIGNAL(errorMessage(QString const&)), this, SLOT(onLogErrorMessage(QString const&)));
        connect(guiChannel, SIGNAL(undefinedMessage(QString const&)), this, SLOT(onLogUndefinedMessage(QString const&)));
    }
    // Force console visible on new messages.
//    connect(this, SIGNAL(errorMessage(const QString&)), logWindowDock_, SLOT(show()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupReportWindow()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupReportWindow()
{
    ReportWindowDock_ = new QDockWidget(tr("Report"), mainWindow_);
    ReportWindowDock_->setVisible(true);
    ReportWindowDock_->setMinimumSize(200, 100);
    ReportWindowDock_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ReportWindowDock_->setObjectName(tr("Report"));
//    ReportWindowDock_->setWindowIcon(QIcon(":resources/Res/log.png"));
//    ReportWindowDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    ReportWindowDock_->setAllowedAreas(Qt::BottomDockWidgetArea);
    ReportWindowDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    ReportWindow_ = new MessageConsole(ReportWindowDock_);
    ReportWindowDock_->setWidget(ReportWindow_);
    mainWindow_->addDockWidget(Qt::BottomDockWidgetArea, ReportWindowDock_);

    mainWindow_->tabifyDockWidget(logWindowDock_, ReportWindowDock_);

//    connect(this, SIGNAL(noticeMessage_LogOut(const QString&)), ReportWindow_, SLOT(onNoticeMessage(const QString&)));
//    connect(this, SIGNAL(errorMessage_LogOut(const QString&)), ReportWindow_, SLOT(onErrorMessage(const QString&)));

    connect(this, SIGNAL(noticeMessage(QString const&)), this, SLOT(onReportNoticeMessage(QString const&)));
    connect(this, SIGNAL(errorMessage(QString const&)), this, SLOT(onReportErrorMessage(QString const&)));
    connect(this, SIGNAL(undefinedMessage(QString const&)), this, SLOT(onReportUndefinedMessage(QString const&)));

//    connect(this, SIGNAL(errorMessage_LogOut(const QString&)),
//        ReportWindow_, SLOT(onErrorMessage(const QString&)), Qt::UniqueConnection);
//    connect(this, SIGNAL(noticeMessage_LogOut(const QString&)),
//        ReportWindow_, SLOT(onNoticeMessage(const QString&)), Qt::UniqueConnection);

    // Force console visible on new messages.
//    connect(this, SIGNAL(errorMessage(const QString&)), ReportWindowDock_, SLOT(show()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupContextHelp()
//-----------------------------------------------------------------------------
//void DockWidgetHandler::setupContextHelp()
//{
//    // Create the dock widget for the context help.
//    contextHelpDock_ = new QDockWidget(tr("Context Help"), mainWindow_);
//    contextHelpDock_->setObjectName(tr("Context Help"));
//    contextHelpDock_->setWindowIcon(QIcon(":resources/Res/system-help.png"));
//    contextHelpDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//    contextHelpDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
//        QDockWidget::DockWidgetFloatable);

//    // Initialize the help engine.
//    QSettings settings;
//    QString helpPath = settings.value("Platform/HelpPath", "Help").toString();

//    QDir helpDirectory(helpPath);
//    if (helpDirectory.isRelative())
//    {
//        helpPath.prepend(QLatin1Char('/'));
//        helpPath.prepend(QCoreApplication::applicationDirPath());
//    }

//    QHelpEngine* helpEngine = new QHelpEngine(helpPath + "/Kactus2Help.qhc", mainWindow_);
//    helpEngine->setupData();

//    // Create the help window.
//    helpWnd_ = new HelpWindow(helpEngine, mainWindow_);

//    // Create the context help browser.
//    contextHelpBrowser_ = new ContextHelpBrowser(helpEngine, "qthelp://com.tut.kactus2.2.0/doc",
//        contextHelpDock_);

//    QColor bgColor = QApplication::palette().color(QPalette::Window);
//    QString style = QString("QTextBrowser { border: 5px solid transparent; "
//        "background-color: #%1}").arg(bgColor.rgb() & 0x00FFFFFF, 0, 16);
//    contextHelpBrowser_->setStyleSheet(style);

//    contextHelpDock_->setWidget(contextHelpBrowser_);
//    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, contextHelpDock_);

//    connect(this, SIGNAL(helpUrlRequested(QString const&)),
//        contextHelpBrowser_, SLOT(onHelpRequested(QString const&)), Qt::UniqueConnection);
//    contextHelpBrowser_->onHelpRequested("welcome.html");
//}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupLibraryDock()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupLibraryDock()
{
#if DOCKWIDGETHANDLER_20231228 == 1

#if LIBRARYDOCK_DISABLE_20231229 == 0
    // create a container widget for dialer and library display
    // set up the dock widget for the library
    libraryDock_ = new QDockWidget(tr("IP-XACT Library"), mainWindow_);
    libraryDock_->setObjectName(tr("Library"));
    libraryDock_->setWindowIcon(QIcon(":resources/Res/library.png"));
    libraryDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    libraryDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

//    libraryWidget_ = new LibraryWidget(libraryHandler_, messageChannel_, mainWindow_);
    libraryWidget_ = new LibraryWidget(libraryHandler_, mainWindow_);

    libraryDock_->setWidget(libraryWidget_);

    mainWindow_->addDockWidget(Qt::LeftDockWidgetArea, libraryDock_);
#else

#if USE_AWMWIDGET == 0
    libraryWidget_ = new LibraryWidget(projectHandler_, libraryHandler_, mainWindow_);
#else
    libraryWidget_ = new AWMWidget(projectHandler_, projectLibraryHandler_, fpgaHandler_, libraryHandler_, mainWindow_);
#endif

    dummy_libraryWidget_ =  libraryWidget_ ;
    dummy_libraryWidget_->setVisible(true);
    dummy_libraryWidget_->setMinimumSize(200, 100);
    dummy_libraryWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
#endif

//    connect(this, SIGNAL(exportProject()), libraryWidget_, SLOT(onExportProject()), Qt::UniqueConnection);
//    connect(this, SIGNAL(importProject()), libraryWidget_, SLOT(onImportProject()), Qt::UniqueConnection);

    connect(this, SIGNAL(generateIntegrityReport()), libraryWidget_, SLOT(onGenerateIntegrityReport()), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createNewBus()), mainWindow_, SLOT(OnCreateNewBus()), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createNewHWComponent()), mainWindow_, SLOT(onCreateNewHWComponent()), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createBus(const VLNV&, const QString&)), mainWindow_, SLOT(createBus(const VLNV&, const QString&)), Qt::UniqueConnection);
    connect(libraryWidget_, SIGNAL(createComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness, QVector<TagData>, const VLNV&, const QString&)), mainWindow_, SLOT(createComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness, QVector<TagData>, const VLNV&, const QString&)), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createAbsDef(const VLNV&, const QString&)), mainWindow_, SLOT(createAbsDef(const VLNV&, const QString&)), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createDesignForExistingComponent(const VLNV&)), mainWindow_, SLOT(createDesignForExistingComponent(const VLNV&)), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createComDef(const VLNV&, const QString&)), mainWindow_, SLOT(createComDefinition(const VLNV&, const QString&)), Qt::UniqueConnection);
    connect(libraryWidget_, SIGNAL(createApiDef(const VLNV&, const QString&)), mainWindow_, SLOT(createApiDefinition(const VLNV&, const QString&)), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(createSWDesign(const VLNV&)), mainWindow_, SLOT(createSWDesign(const VLNV&)), Qt::UniqueConnection);
    connect(libraryWidget_, SIGNAL(createSystemDesign(const VLNV&)), mainWindow_, SLOT(createSystemDesign(const VLNV&)), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(projectSearch()), mainWindow_, SLOT(onProjectSearch()), Qt::UniqueConnection);

    connect(libraryWidget_, SIGNAL(inputMessage(QString const&)), this, SLOT(onLogInputMessage(QString const&)));
    connect(libraryWidget_, SIGNAL(noticeMessage(QString const&)), this, SLOT(onLogNoticeMessage(QString const&)));
    connect(libraryWidget_, SIGNAL(errorMessage(QString const&)), this, SLOT(onLogErrorMessage(QString const&)));
    connect(libraryWidget_, SIGNAL(undefinedMessage(QString const&)), this, SLOT(onLogUndefinedMessage(QString const&)));

    connect(libraryWidget_, SIGNAL(statusMessage(QString const&)), this, SIGNAL(statusMessage(QString const&)));
#endif


}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupDesignParametersEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupDesignParametersEditor()
{
#if USE_DESIGNPARAMETERSWINDOW == 1
    designParameterDock_ = new QDockWidget(tr("Design Parameters"), mainWindow_);
    designParameterDock_->setObjectName(tr("Design Parameters dock"));
    designParameterDock_->setAllowedAreas(Qt::BottomDockWidgetArea);
    designParameterDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1


    QSharedPointer<ExpressionFormatter> formatter(new ExpressionFormatter(designParameterFinder_));
    QSharedPointer<ExpressionFormatter> referenceTreeFormatter(
        new ExpressionFormatter(designAndInstancesParameterFinder_));
    designAndInstancesParameterFinder_->addFinder(designParameterFinder_);

#if USE_DESIGNPARAMETERSWINDOW == 1
    designParameterReferenceCounter_ = QSharedPointer<DesignParameterReferenceCounter>(
        new DesignParameterReferenceCounter(designParameterFinder_));

    designParameterTree_ =
        new DesignParameterReferenceTree(referenceTreeFormatter, designParameterReferenceCounter_, mainWindow_);

    designParametersEditor_ = new ParameterGroupBox(QSharedPointer<QList<QSharedPointer<Parameter> > >(),
        QSharedPointer<QList<QSharedPointer<Choice> > >(), designParameterFinder_, formatter, mainWindow_);

    ParameterReferenceTreeWindow* designParameterReferenceWindow =
        new ParameterReferenceTreeWindow(designParameterTree_, designParametersEditor_);

    designParameterDock_->setWidget(designParametersEditor_);
    mainWindow_->addDockWidget(Qt::BottomDockWidgetArea, designParameterDock_);

    connect(designParametersEditor_, SIGNAL(contentChanged()),
        this, SIGNAL(designChanged()), Qt::UniqueConnection);

    connect(designParametersEditor_, SIGNAL(increaseReferences(QString)),
        designParameterReferenceCounter_.data(), SLOT(increaseReferenceCount(QString)), Qt::UniqueConnection);
    connect(designParametersEditor_, SIGNAL(decreaseReferences(QString)),
        designParameterReferenceCounter_.data(), SLOT(decreaseReferenceCount(QString)), Qt::UniqueConnection);
    connect(designParametersEditor_, SIGNAL(openReferenceTree(QString const&, QString const&)),
        designParameterReferenceWindow, SLOT(openReferenceTree(QString const&, QString const&)),
        Qt::UniqueConnection);

    connect(designParametersEditor_,
        SIGNAL(recalculateReferencesToParameters(QVector<QString> const&, AbstractParameterInterface*)),
        designParameterReferenceCounter_.data(),
        SLOT(recalculateReferencesToParameters(QVector<QString> const&, AbstractParameterInterface*)),
        Qt::UniqueConnection);
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1

}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupInstanceEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupInstanceEditor()
{
    instanceDock_ = new QDockWidget(tr("Component Instance Details"), mainWindow_);
    instanceDock_->setVisible(true);
    instanceDock_->setMinimumSize(200, 100);
    instanceDock_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    instanceDock_->setObjectName(tr("Instance Editor"));
    instanceDock_->setWindowIcon(QIcon(":/resources/Res/hw-component-edit.png"));
    instanceDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    instanceDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    instanceEditor_ = new ComponentInstanceEditor(instanceDock_);
    instanceDock_->setWidget(instanceEditor_);
    instanceDock_->resize(400, 150);

    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, instanceDock_);

    instanceDock_->hide();

    connect(instanceEditor_, SIGNAL(contentChanged()), this, SIGNAL(designChanged()), Qt::UniqueConnection);
//    connect(instanceEditor_, SIGNAL(increaseReferences(QString)),
//        designParameterReferenceCounter_.data(), SLOT(increaseReferenceCount(QString)), Qt::UniqueConnection);
//    connect(instanceEditor_, SIGNAL(decreaseReferences(QString)),
//        designParameterReferenceCounter_.data(), SLOT(decreaseReferenceCount(QString)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupAdHocEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupAdHocEditor()
{
#if USE_ADHOC_WINDOW == 1
    adhocDock_ = new QDockWidget(tr("Ad-hoc Port Editor"), mainWindow_);
    adhocDock_->setObjectName(tr("Ad-hoc Port Editor"));
    adhocDock_->setWindowIcon(QIcon(":resources/Res/adhoc.png"));
    adhocDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    adhocDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    adhocEditor_ = new AdhocEditor(designParameterFinder_, adhocDock_);
    adhocDock_->setWidget(adhocEditor_);
    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, adhocDock_);

    connect(adhocEditor_, SIGNAL(increaseReferences(QString const&)),
        designParameterReferenceCounter_.data(), SLOT(increaseReferenceCount(QString const&)),
        Qt::UniqueConnection);
    connect(adhocEditor_, SIGNAL(decreaseReferences(QString const&)),
        designParameterReferenceCounter_.data(), SLOT(decreaseReferenceCount(QString const&)),
        Qt::UniqueConnection);

    connect(adhocEditor_, SIGNAL(contentChanged()), this, SIGNAL(designChanged()), Qt::UniqueConnection);
#endif // #if USE_ADHOC_WINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupConfigurationEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupConfigurationEditor()
{
#if USE_CONFIGURATIONWINDOW == 1
    configurationDock_ = new QDockWidget(tr("Design Configuration Details"), mainWindow_);
    configurationDock_->setObjectName(tr("Configuration editor"));
    configurationDock_->setWindowIcon(QIcon(":resources/Res/hierarchy-blue.png"));
    configurationDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    configurationDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    configurationEditor_ = new ConfigurationEditor(libraryHandler_, configurationDock_);
    configurationDock_->setWidget(configurationEditor_);
    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, configurationDock_);

    connect(configurationEditor_, SIGNAL(contentChanged()), this, SIGNAL(designChanged()), Qt::UniqueConnection);
#endif // #if USE_CONFIGURATIONWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupSystemDetailsEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupSystemDetailsEditor()
{
//    systemDetailsDock_ = new QDockWidget(tr("HW Mapping Details"), mainWindow_);
//    systemDetailsDock_->setObjectName(tr("HW Mapping Details Editor"));
//    systemDetailsDock_->setWindowIcon(QIcon(":resources/Res/mapping.png"));
//    systemDetailsDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    systemDetailsDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
//        QDockWidget::DockWidgetFloatable);

//    systemDetailsEditor_ = new SystemDetailsEditor(libraryHandler_, systemDetailsDock_);
//    systemDetailsDock_->setWidget(systemDetailsEditor_);
//    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, systemDetailsDock_);

//    connect(systemDetailsEditor_, SIGNAL(contentChanged()), this, SIGNAL(designChanged()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupInterfaceEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupInterfaceEditor()
{
#if USE_INTERFACEWINDOW == 1
    interfaceDock_ = new QDockWidget(tr("Interface Editor"), mainWindow_);
    interfaceDock_->setObjectName(tr("Interface Editor"));
    interfaceDock_->setWindowIcon(QIcon(":resources/Res/tool-interface.png"));
    interfaceDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    interfaceDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    interfaceEditor_ = new InterfaceEditor(interfaceDock_, libraryHandler_);
    interfaceDock_->setWidget(interfaceEditor_);
    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, interfaceDock_);

    connect(interfaceEditor_, SIGNAL(contentChanged()), this, SIGNAL(designChanged()), Qt::UniqueConnection);
#endif // #if USE_INTERFACEWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupConnectionEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupConnectionEditor()
{
#if USE_CONNECTIONWINDOW == 1
    connectionDock_ = new QDockWidget(tr("Connection Editor"), mainWindow_);
    connectionDock_->setObjectName(tr("Connection Editor"));
    connectionDock_->setWindowIcon(QIcon(":resources/Res/tool-interconnection.png"));
    connectionDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    connectionDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);

    connectionEditor_ = new ConnectionEditor(libraryHandler_, connectionDock_);
    connectionDock_->setWidget(connectionEditor_);
    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, connecComponentItemtionDock_);
#endif // #if USE_CONNECTIONWINDOW == 1

}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupVendorExtensionEditor()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupVendorExtensionEditor()
{
//    extensionDock_ = new QDockWidget(tr("Vendor Extensions (experimental)"), mainWindow_);
//    extensionDock_->setObjectName(tr("Vendor Extension Editor"));
//    extensionDock_->setWindowIcon(QIcon(":resources/Res/puzzle.png"));
//    extensionDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
//    extensionDock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
//        QDockWidget::DockWidgetFloatable);

//    extensionEditor_ = new VendorExtensionsEditor(connectionDock_);
//    extensionDock_->setWidget(extensionEditor_);
//    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, extensionDock_);
}


//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::clearItemSelection()
//-----------------------------------------------------------------------------
void DockWidgetHandler::clearItemSelection(HWDesignWidget* designWidget)
{
#if USE_ADHOC_WINDOW == 1
    if (designWidget != 0)
    {
        QSharedPointer<Design> containedDesign = designWidget->getDiagram()->getDesign();

        adhocEditor_->setVisibilityData(designWidget->getDiagram(), containedDesign,
            designWidget->getEditProvider(), designWidget->isProtected());
    }
    else
    {
        adhocEditor_->clearVisibilityData();
    }
#endif // #if USE_ADHOC_WINDOW == 1

    instanceEditor_->clear();

#if USE_INTERFACEWINDOW == 1
    interfaceEditor_->clear();
#endif // #if USE_INTERFACEWINDOW == 1

#if USE_CONNECTIONWINDOW == 1
    connectionEditor_->clear();
#endif // #if USE_CONNECTIONWINDOW == 1

#if USE_ADHOC_WINDOW == 1
    adhocEditor_->clearPortData();
#endif // #if USE_ADHOC_WINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::selectComponent()
//-----------------------------------------------------------------------------
void DockWidgetHandler::selectComponent(QWidget* currentTabWidget, ComponentItem* component)
{
#if USE_CONNECTIONWINDOW == 1
    connectionEditor_->clear();a();
#endif // #if USE_CONNECTIONWINDOW == 1

#if USE_INTERFACEWINDOW == 1
    interfaceEditor_->clear();
#endif // #if USE_INTERFACEWINDOW == 1

#if USE_ADHOC_WINDOW == 1
    adhocEditor_->clearPortData();
#endif // #if USE_ADHOC_WINDOW == 1

    DesignWidget* designWidget(0);

    HWDesignWidget* hwDesignWidget = dynamic_cast<HWDesignWidget*>(currentTabWidget);
    if (hwDesignWidget)
    {
        designWidget = hwDesignWidget;
    }
    else
    {
//        SystemDesignWidget* systemDesignWidget = dynamic_cast<SystemDesignWidget*>(currentTabWidget);
//        designWidget = systemDesignWidget;
    }

    QSharedPointer<Design> containingDesign = designWidget->getDiagram()->getDesign();

    // Update the instance and ad-hoc visibility editors.
    instanceEditor_->setComponentInstance(component, designWidget->getEditProvider(), containingDesign,
        designWidget->getDiagram()->getDesignConfiguration(), designWidget->getOpenViewName());

#if USE_ADHOC_WINDOW == 1
    HWComponentItem* hwComponent = dynamic_cast<HWComponentItem*>(component);
    if (hwComponent != 0)
    {
        adhocEditor_->setVisibilityData(hwComponent, containingDesign, designWidget->getEditProvider(),
            designWidget->isProtected());
    }
    else
    {
        adhocEditor_->clearVisibilityData();
    }
#endif // #if USE_ADHOC_WINDOW == 1

    VLNV componentVLNV = component->componentModel()->getVlnv();
    libraryWidget_->selectComponent(componentVLNV);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::selectConnectionInterface()
//-----------------------------------------------------------------------------
void DockWidgetHandler::selectConnectionInterface(QWidget* currentTabWidget, ConnectionEndpoint* interface)
{
#if USE_ADHOC_WINDOW == 1
    adhocEditor_->clearVisibilityData();
#endif // #if USE_ADHOC_WINDOW == 1

#if USE_CONNECTIONWINDOW == 1
    connectionEditor_->clear();
#endif // #if USE_CONNECTIONWINDOW == 1

    instanceEditor_->clear();

#if USE_INTERFACEWINDOW == 1
    if (!interface->isAdHoc())
    {
        DesignWidget* designWidget = dynamic_cast<DesignWidget*>(currentTabWidget);
        if (designWidget)
        {
            interfaceEditor_->setInterface(interface, designWidget->getDiagram()->getDesign(),
                designWidget->getDiagram()->getDesignConfiguration(), designWidget->getOpenViewName(),
                designWidget->getEditProvider(), designWidget->isProtected());
        }

#if USE_ADHOC_WINDOW == 1
        adhocEditor_->clearPortData();
#endif // #if USE_ADHOC_WINDOW == 1

    }
    else
    {
#if USE_ADHOC_WINDOW == 1

        AdHocItem* adhocEndPoint = dynamic_cast<AdHocItem*>(interface);
        if (adhocEndPoint)
        {
            HWDesignWidget* hwDesignWidget = dynamic_cast<HWDesignWidget*>(currentTabWidget);
            if (hwDesignWidget)
            {
                HWDesignDiagram* hwDiagram = dynamic_cast<HWDesignDiagram*>(hwDesignWidget->getDiagram());
                if (hwDiagram)
                {
                    adhocEditor_->setPortData(adhocEndPoint, hwDiagram, hwDesignWidget->getEditProvider());

                    auto dataSource = dynamic_cast<AdHocEnabled*>(adhocEndPoint->encompassingComp());
                    if (dataSource == nullptr)
                    {
                        dataSource = hwDiagram;
                    }

                    adhocEditor_->setVisibilityData(dataSource, hwDiagram->getDesign(), hwDesignWidget->getEditProvider(),
                        hwDesignWidget->isProtected());
                }
            }
        }
        else
        {
            adhocEditor_->clearPortData();
        }
#endif // #if USE_ADHOC_WINDOW == 1

        interfaceEditor_->clear();
    }
#endif // #if USE_INTERFACEWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::selectGraphicsConnection()
//-----------------------------------------------------------------------------
void DockWidgetHandler::selectGraphicsConnection(QWidget* currentTabWidget, GraphicsConnection* connection)
{
//    DesignWidget* designWidget = dynamic_cast<DesignWidget*>(currentTabWidget);

//    Q_ASSERT(connection);
#if USE_ADHOC_WINDOW == 1
    adhocEditor_->clearVisibilityData();
    adhocEditor_->clearPortData();
#endif // #if USE_ADHOC_WINDOW == 1

    instanceEditor_->clear();

#if USE_INTERFACEWINDOW == 1
    interfaceEditor_->clear();
#endif // #if USE_INTERFACEWINDOW == 1

#if USE_CONNECTIONWINDOW == 1
    connectionEditor_->setConnection(connection, designWidget->getDiagram());
#endif // #if USE_CONNECTIONWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::updateWindows()
//-----------------------------------------------------------------------------
void DockWidgetHandler::updateWindows(QWidget* currentTabWidget)
{
    for (auto i = docks_.cbegin(); i != docks_.cend(); ++i)
    {
        updateWindowAndControlVisibility(currentTabWidget, i.key(), i.value());
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::updateWindowAndControlVisibility()
//-----------------------------------------------------------------------------
void DockWidgetHandler::updateWindowAndControlVisibility(QWidget* currentTabWidget,
    TabDocument::SupportedWindows windowType, QDockWidget* dock)
{
    QAction* showAction = dock->toggleViewAction();

    bool allowedToShow = isSupportedWindowType(currentTabWidget, windowType);
    bool wantedToShow = visibilities_.value(windowType);
    bool selected = showAction->isChecked();

    visibilityControls_[windowType]->setVisible(allowedToShow);
    showAction->setVisible(allowedToShow && wantedToShow);
    showAction->setChecked(selected && allowedToShow && wantedToShow);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::isSupportedWindowType()
//-----------------------------------------------------------------------------
bool DockWidgetHandler::isSupportedWindowType(QWidget* currentTabWidget,
    TabDocument::SupportedWindows windowType) const
{
    return currentlySupportedWindows(currentTabWidget) & windowType;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::currentlySupportedWindows()
//-----------------------------------------------------------------------------
unsigned int DockWidgetHandler::currentlySupportedWindows(QWidget* currentTabWidget) const
{
    unsigned int windows = defaultWindows();

    TabDocument* doc = dynamic_cast<TabDocument*>(currentTabWidget);
    if (doc != nullptr)
    {
        windows = doc->getSupportedWindows();
    }

    return windows;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::defaultWindows()
//-----------------------------------------------------------------------------
unsigned int DockWidgetHandler::defaultWindows() const
{
//    return TabDocument::OUTPUTWINDOW | TabDocument::LIBRARYWINDOW | TabDocument::CONTEXT_HELP_WINDOW;
    return TabDocument::OUTPUTWINDOW | TabDocument::CONSOLEWINDOW | TabDocument::REPORTWINDOW;
//    return TabDocument::INSTANCEWINDOW;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::documentChanged()
//-----------------------------------------------------------------------------
void DockWidgetHandler::documentChanged(TabDocument* doc)
{    
    ComponentEditor* componentEditor = dynamic_cast<ComponentEditor*>(doc);
    DesignWidget* designwidget = dynamic_cast<DesignWidget*>(doc);

//    disconnect(extensionEditor_, SLOT(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)));

    if (doc)
    {
//        connect(doc, SIGNAL(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)),
//            extensionEditor_, SLOT(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)));
    }

    if (componentEditor)
    {
//        extensionEditor_->setContext(doc);

//        QSharedPointer<Component> editedComponent = componentEditor->getComponent();
//        QString extensionsID = QLatin1String("Component: ") + editedComponent->getVlnv().toString();

//        extensionEditor_->changeVendorExtensions(extensionsID, editedComponent);
    }
    else if(designwidget)
    {
//        extensionEditor_->setContext(doc);

//        QSharedPointer<Design> editedDesign = designwidget->getDiagram()->getDesign();
//        QString extensionsID = QLatin1String("Design: ") + editedDesign->getVlnv().toString();

//        extensionEditor_->changeVendorExtensions(extensionsID, editedDesign);
    }
    else
    {
//        extensionEditor_->clear();
    }

    // set the configuration manager to edit the active design
    if (designwidget)
    {
#if USE_CONFIGURATIONWINDOW == 1
        configurationEditor_->setConfiguration(designwidget);
#endif // #if USE_CONFIGURATIONWINDOW == 1

        QSharedPointer<Document> topItem = projectHandler_->getModel(doc->getDocumentVLNV());
        QSharedPointer<Component> topComponent = topItem.dynamicCast<Component>();
        if (topComponent)
        {
            instanceEditor_->setContext(topComponent, designwidget->getDiagram()->getEditProvider());
            instanceEditor_->setTopComponentActiveView(
                topComponent->getModel()->findView(designwidget->getOpenViewName()));
            instanceEditor_->setProtection(designwidget->isProtected());

            QSharedPointer<Design> design = designwidget->getDiagram()->getDesign();

#if USE_DESIGNPARAMETERSWINDOW == 1
            designParametersEditor_->setDisabled(designwidget->isProtected());
            designParametersEditor_->setNewParameters(design->getParameters());
            designParameterTree_->setDesign(design);
            designParameterReferenceCounter_->setDesign(design);
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1

            setupDesignParameterFinder(design);
        }

        if (doc->getSupportedWindows() & TabDocument::SYSTEM_DETAILS_WINDOW)
        {
//            systemDetailsEditor_->setSystem(designwidget);
        }
        else
        {
//            systemDetailsEditor_->clear();
        }
    }
    // active tab is not design widget so clear the editors associated with design widget
    else
    {
#if USE_CONFIGURATIONWINDOW == 1
        configurationEditor_->clear();
#endif // #if USE_CONFIGURATIONWINDOW == 1

//        systemDetailsEditor_->clear();
        instanceEditor_->clear();

#if USE_ADHOC_WINDOW == 1
        adhocEditor_->clearVisibilityData();
        adhocEditor_->clearPortData();
#endif // #if USE_ADHOC_WINDOW == 1

#if USE_INTERFACEWINDOW == 1
        interfaceEditor_->clear();
#endif // #if USE_INTERFACEWINDOW == 1

#if USE_CONNECTIONWINDOW == 1
        connectionEditor_->clear();
#endif // #if USE_CONNECTIONWINDOW == 1
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setupDesignParameterFinder()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setupDesignParameterFinder(QSharedPointer<Design> newDesign)
{
    designParameterFinder_->setParameterList(newDesign->getParameters());

    designAndInstancesParameterFinder_->removeAllFinders();
    designAndInstancesParameterFinder_->addFinder(designParameterFinder_);

    if (!newDesign->getComponentInstances()->isEmpty())
    {
        QList<QSharedPointer<const Component> > componentsWithFinders;

        for (QSharedPointer<ComponentInstance> instance : *newDesign->getComponentInstances())
        {
            QSharedPointer<const Document> referencedDocument;

            if (projectHandler_->contains(*instance->getComponentRef().data()))
            {
                referencedDocument = projectHandler_->getModelReadOnly(*instance->getComponentRef().data());
            }
            else {
                referencedDocument = projectLibraryHandler_->getModelReadOnly(*instance->getComponentRef().data());
            }

            if (referencedDocument)
            {
                QSharedPointer<const Component> instancedComponent =
                    referencedDocument.dynamicCast<const Component>();
                if (instancedComponent && !componentsWithFinders.contains(instancedComponent))
                {
                    QSharedPointer<ComponentParameterFinder> instanceFinder(
                        new ComponentParameterFinder(instancedComponent));
                    designAndInstancesParameterFinder_->addFinder(instanceFinder);

                    componentsWithFinders.append(instancedComponent);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::connectVisibilityControls()
//-----------------------------------------------------------------------------
void DockWidgetHandler::connectVisibilityControls()
{
    bool vis_Console = QSettings().value("Visible/Console").toBool();
    bool vis_Log = QSettings().value("Visible/Log").toBool();
    bool vis_Report = QSettings().value("Visible/Report").toBool();

    emit adjustVisibilityInWindow(TabDocument::CONSOLEWINDOW, vis_Console);
    emit adjustVisibilityInWindow(TabDocument::OUTPUTWINDOW, vis_Log);
    emit adjustVisibilityInWindow(TabDocument::REPORTWINDOW, vis_Report);

    connect(visibilityControls_[TabDocument::CONSOLEWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onConsoleAction(bool)), Qt::UniqueConnection);
    connect(visibilityControls_[TabDocument::OUTPUTWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onOutputAction(bool)), Qt::UniqueConnection);
    connect(visibilityControls_[TabDocument::REPORTWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onReportAction(bool)), Qt::UniqueConnection);
////    connect(visibilityControls_[TabDocument::CONTEXT_HELP_WINDOW], SIGNAL(toggled(bool)),
////        this, SLOT(onContextHelpAction(bool)), Qt::UniqueConnection);
//    connect(visibilityControls_[TabDocument::LIBRARYWINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onLibraryAction(bool)), Qt::UniqueConnection);
#if USE_CONFIGURATIONWINDOW == 1
    connect(visibilityControls_[TabDocument::CONFIGURATIONWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onConfigurationAction(bool)), Qt::UniqueConnection);
#endif // #if USE_CONFIGURATIONWINDOW == 1
//    connect(visibilityControls_[TabDocument::SYSTEM_DETAILS_WINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onSystemDetailsAction(bool)), Qt::UniqueConnection);
#if USE_CONNECTIONWINDOW == 1
    connect(visibilityControls_[TabDocument::CONNECTIONWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onConnectionAction(bool)), Qt::UniqueConnection);
#endif // #if USE_CONNECTIONWINDOW == 1
#if USE_INTERFACEWINDOW == 1
    connect(visibilityControls_[TabDocument::INTERFACEWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onInterfaceAction(bool)), Qt::UniqueConnection);
#endif // #if USE_INTERFACEWINDOW == 1
    connect(visibilityControls_[TabDocument::INSTANCEWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onInstanceAction(bool)), Qt::UniqueConnection);
#if USE_DESIGNPARAMETERSWINDOW == 1
    connect(visibilityControls_[TabDocument::DESIGNPARAMETERSWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onDesignParametersAction(bool)), Qt::UniqueConnection);
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1
#if USE_ADHOC_WINDOW == 1
    connect(visibilityControls_[TabDocument::ADHOC_WINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onAdHocEditorAction(bool)), Qt::UniqueConnection);
#endif // #if USE_ADHOC_WINDOW == 1

//    connect(visibilityControls_[TabDocument::VENDOREXTENSIONWINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onVendorExtensionVisibilityAction(bool)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::disconnectVisibilityControls()
//-----------------------------------------------------------------------------
void DockWidgetHandler::disconnectVisibilityControls()
{
    bool vis_Console = consoleWindow_->isVisible();
    bool vis_Log = logWindow_->isVisible();
    bool vis_Report = ReportWindow_->isVisible();

    QSettings().setValue("Visible/Console", vis_Console);
    QSettings().setValue("Visible/Log", vis_Log);
    QSettings().setValue("Visible/Report", vis_Report);

    emit adjustVisibilityInWindow(TabDocument::CONSOLEWINDOW, vis_Console);
    emit adjustVisibilityInWindow(TabDocument::OUTPUTWINDOW, vis_Log);
    emit adjustVisibilityInWindow(TabDocument::REPORTWINDOW, vis_Report);

    disconnect(visibilityControls_[TabDocument::CONSOLEWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onConsoleAction(bool)));
    disconnect(visibilityControls_[TabDocument::OUTPUTWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onOutputAction(bool)));
    disconnect(visibilityControls_[TabDocument::REPORTWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onReportAction(bool)));
//    disconnect(visibilityControls_[TabDocument::CONTEXT_HELP_WINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onContextHelpAction(bool)));
//    disconnect(visibilityControls_[TabDocument::LIBRARYWINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onLibraryAction(bool)));
#if USE_CONFIGURATIONWINDOW == 1
    disconnect(visibilityControls_[TabDocument::CONFIGURATIONWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onConfigurationAction(bool)));
#endif // #if USE_CONFIGURATIONWINDOW == 1
//    disconnect(visibilityControls_[TabDocument::SYSTEM_DETAILS_WINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onSystemDetailsAction(bool)));
#if USE_CONNECTIONWINDOW == 1
    disconnect(visibilityControls_[TabDocument::CONNECTIONWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onConnectionAction(bool)));
#endif // #if USE_CONNECTIONWINDOW == 1
#if USE_INTERFACEWINDOW == 1
    disconnect(visibilityControls_[TabDocument::INTERFACEWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onInterfaceAction(bool)));
#endif // #if USE_INTERFACEWINDOW == 1
    disconnect(visibilityControls_[TabDocument::INSTANCEWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onInstanceAction(bool)));
#if USE_DESIGNPARAMETERSWINDOW == 1
    disconnect(visibilityControls_[TabDocument::DESIGNPARAMETERSWINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onDesignParametersAction(bool)));
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1
#if USE_ADHOC_WINDOW == 1
    disconnect(visibilityControls_[TabDocument::ADHOC_WINDOW], SIGNAL(toggled(bool)),
        this, SLOT(onAdHocEditorAction(bool)));
#endif // #if USE_ADHOC_WINDOW == 1
//    disconnect(visibilityControls_[TabDocument::VENDOREXTENSIONWINDOW], SIGNAL(toggled(bool)),
//        this, SLOT(onVendorExtensionVisibilityAction(bool)));
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onConsoleAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onConsoleAction( bool show )
{
    emit adjustVisibilityInWindow(TabDocument::CONSOLEWINDOW, show);

    bool vis_Console = show;

    QSettings().setValue("Visible/Console", vis_Console);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onOutputAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onOutputAction( bool show )
{
    emit adjustVisibilityInWindow(TabDocument::OUTPUTWINDOW, show);

    bool vis_Log = show;

    QSettings().setValue("Visible/Log", vis_Log);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onReportAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onReportAction( bool show )
{
    emit adjustVisibilityInWindow(TabDocument::REPORTWINDOW, show);

    bool vis_Report = show;

    QSettings().setValue("Visible/Report", vis_Report);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onContextHelpAction()
//-----------------------------------------------------------------------------
//void DockWidgetHandler::onContextHelpAction( bool show )
//{
//    emit adjustVisibilityInWindow(TabDocument::CONTEXT_HELP_WINDOW, show);
//}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onLibraryAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onLibraryAction( bool show )
{
//    emit adjustVisibilityInWindow(TabDocument::LIBRARYWINDOW, show);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onConfigurationAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onConfigurationAction( bool show )
{
#if USE_CONFIGURATIONWINDOW == 1
    emit adjustVisibilityInWindow(TabDocument::CONFIGURATIONWINDOW, show);
#endif // #if USE_CONFIGURATIONWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onSystemDetailsAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onSystemDetailsAction( bool show )
{
//    emit adjustVisibilityInWindow(TabDocument::SYSTEM_DETAILS_WINDOW, show);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onConnectionAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onConnectionAction( bool show )
{
#if USE_CONNECTIONWINDOW == 1
    emit adjustVisibilityInWindow(TabDocument::CONNECTIONWINDOW, show);
#endif // #if USE_CONNECTIONWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onVendorExtensionVisibilityAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onVendorExtensionVisibilityAction(bool show)
{
//    emit adjustVisibilityInWindow(TabDocument::VENDOREXTENSIONWINDOW, show);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onInterfaceAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onInterfaceAction( bool show )
{
#if USE_INTERFACEWINDOW == 1
    emit adjustVisibilityInWindow(TabDocument::INTERFACEWINDOW, show);
#endif // #if USE_INTERFACEWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onAdHocEditorAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onAdHocEditorAction(bool show)
{
#if USE_ADHOC_WINDOW == 1
    emit adjustVisibilityInWindow(TabDocument::ADHOC_WINDOW, show);
#endif // #if USE_ADHOC_WINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onInstanceAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onInstanceAction( bool show )
{
    emit adjustVisibilityInWindow(TabDocument::INSTANCEWINDOW, show);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onDesignParametersAction()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onDesignParametersAction(bool show)
{
#if USE_DESIGNPARAMETERSWINDOW == 1
    emit adjustVisibilityInWindow(TabDocument::DESIGNPARAMETERSWINDOW, show);
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::onDockLocationChanged()
//-----------------------------------------------------------------------------
void DockWidgetHandler::onDockLocationChanged(Qt::DockWidgetArea area)
{
    auto const& dock = qobject_cast<QDockWidget*>(sender());
    auto action = dock->toggleViewAction();

    placeActionInToolbar(action, area);
}

void DockWidgetHandler::onImportProject()
{
    libraryWidget_->onImportProject();
//    emit importProject();
}

void DockWidgetHandler::onExportProject()
{
    libraryWidget_->onExportProject();
//    emit exportProject();
}



void DockWidgetHandler::onImportProject_Console(QString const &fileDest, QString const &filePath, QString const &fileName)
{
    libraryWidget_->onImportProject_Console(fileDest, filePath, fileName);
}

void DockWidgetHandler::onExportProject_Console(QString const &fileDest, QString const &filePath, QString const &fileName)
{
    libraryWidget_->onExportProject_Console(fileDest, filePath, fileName);
}




void DockWidgetHandler::onLogInputMessage(QString const& message)
{
    logWindow_->onInputMessage(message);
}



void DockWidgetHandler::onLogNoticeMessage(QString const& message)
{
    logWindow_->onNoticeMessage(message);
}



void DockWidgetHandler::onLogErrorMessage(QString const& message)
{
    logWindow_->onErrorMessage(message);
}



void DockWidgetHandler::onLogUndefinedMessage(QString const& message)
{
    logWindow_->onUndefinedMessage(message);
}



void DockWidgetHandler::onReportNoticeMessage(QString const& message)
{
    ReportWindow_->onNoticeMessage(message);
}



void DockWidgetHandler::onReportErrorMessage(QString const& message)
{
    ReportWindow_->onErrorMessage(message);
}



void DockWidgetHandler::onReportUndefinedMessage(QString const& message)
{
    ReportWindow_->onUndefinedMessage(message);
}



//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setWindowVisibilityForSupportedWindow()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setWindowVisibilityForSupportedWindow(QWidget* currentTabWidget,
                                                              TabDocument::SupportedWindows type, bool show)
{
    if (isSupportedWindowType(currentTabWidget, type))
    {
        setWindowVisibility(type, show);
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::setWindowVisibility()
//-----------------------------------------------------------------------------
void DockWidgetHandler::setWindowVisibility(TabDocument::SupportedWindows windowType, bool show)
{
    visibilities_.insert(windowType, show);

    visibilityControls_[windowType]->setChecked(show);
    docks_[windowType]->toggleViewAction()->setVisible(true);
    docks_[windowType]->setVisible(show);
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::placeActionInToolbar()
//-----------------------------------------------------------------------------
void DockWidgetHandler::placeActionInToolbar(QAction* action, Qt::DockWidgetArea area)
{
//    if ((area == Qt::LeftDockWidgetArea && leftActions_->actions().contains(action)) ||
//        (area == Qt::RightDockWidgetArea && rightActions_->actions().contains(action)))
    if ( area == Qt::RightDockWidgetArea && rightActions_->actions().contains(action) )
    {
        return;
    }

//    leftActions_->removeAction(action);
//    leftToolbar_->removeAction(action);

    rightActions_->removeAction(action);
    rightToolbar_->removeAction(action);

    if (area == Qt::LeftDockWidgetArea)
    {
//        leftActions_->addAction(action);
//        leftToolbar_->addAction(action);
    }
    else if (area == Qt::RightDockWidgetArea)
    {
        rightActions_->addAction(action);
        rightToolbar_->addAction(action);
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::changeProtection()
//-----------------------------------------------------------------------------
void DockWidgetHandler::changeProtection(TabDocument* doc, bool locked)
{
//    extensionEditor_->setLocked(locked);

    DesignWidget* designwidget = dynamic_cast<DesignWidget*>(doc);
    if (designwidget)
    {
        // update the editors to match the locked state
#if USE_CONFIGURATIONWINDOW == 1
        configurationEditor_->setLocked(locked);
#endif // #if USE_CONFIGURATIONWINDOW == 1
        instanceEditor_->setProtection(locked);
//        designParametersEditor_->setDisabled(locked);

        if (designwidget->getSupportedWindows() & TabDocument::SYSTEM_DETAILS_WINDOW)
        {
//            systemDetailsEditor_->setLocked(locked);
        }
    }
    else
    {

#if USE_CONFIGURATIONWINDOW == 1
        configurationEditor_->setLocked(true);
#endif // #if USE_CONFIGURATIONWINDOW == 1
//        systemDetailsEditor_->setLocked(true);
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::getDesignAndInstanceParameterFinder()
//-----------------------------------------------------------------------------
QSharedPointer<MultipleParameterFinder> DockWidgetHandler::getDesignAndInstanceParameterFinder() const
{
    return designAndInstancesParameterFinder_;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::getDesignParameterFinder()
//-----------------------------------------------------------------------------
QSharedPointer<ListParameterFinder> DockWidgetHandler::getDesignParameterFinder() const
{
    return designParameterFinder_;
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::refreshDesignDocument()
//-----------------------------------------------------------------------------
void DockWidgetHandler::refreshDesignDocument(QWidget* currentTabWidget)
{
    TabDocument* doc = static_cast<TabDocument*>(currentTabWidget);
   
    DesignWidget* designWidget = dynamic_cast<DesignWidget*>(doc);
    if (designWidget)
    {
        QSharedPointer<Document> topItem = projectHandler_->getModel(doc->getDocumentVLNV());
        QSharedPointer<Component> topComponent = topItem.dynamicCast<Component>();

        instanceEditor_->setContext(topComponent, designWidget->getDiagram()->getEditProvider());

        QSharedPointer<Design> design = designWidget->getDiagram()->getDesign();

#if USE_DESIGNPARAMETERSWINDOW == 1
        designParametersEditor_->setNewParameters(design->getParameters());
        designParameterTree_->setDesign(design);
        designParameterReferenceCounter_->setDesign(design);
#endif // #if USE_DESIGNPARAMETERSWINDOW == 1

        setupDesignParameterFinder(design);
    }
}

//-----------------------------------------------------------------------------
// Function: DockWidgetHandler::showHelp()
//-----------------------------------------------------------------------------
void DockWidgetHandler::showHelp()
{
//    helpWnd_->show();
}
