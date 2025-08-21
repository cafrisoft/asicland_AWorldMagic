#include <GlobalBuildOptions.h>

#include <QApplication>
#include <QAction>
#include <QKeySequence>
#include <QCursor>
#include <QToolBar>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <AWorldMagicAPI/ProjectLibraryHandler.h>
#include <AWorldMagicAPI/FPGAHandler.h>

#include <IPXACTmodels/designConfiguration/DesignConfiguration.h>

#include <KactusAPI/KactusAPI.h>

#include <KactusAPI/PluginUtilityAdapter.h>

#include <KactusAPI/ProjectHandler.h>
#include <KactusAPI/LibraryHandler.h>
#include <KactusAPI/VersionHelper.h>
#include <KactusAPI/IGeneratorPlugin.h>
#include <KactusAPI/Project.h>



#ifdef _WIN32
#include <common01/SettingManager.h>
#else
#include <Common/SettingManager.h>
#endif

#include <Plugins/VerilogGenerator/VerilogGenerator.h>

#include <UI/ToolBar/DesignRightToolBar.h>

#include <UI/Settings/SettingsDialog.h>
#include <UI/Settings/SettingsUpdater.h>

#include <UI/DockWidgetHandler.h>
#include <UI/Common/DesignWidgetFactoryImplementation.h>

#include <UI/AbstractionDefinitionEditor/AbstractionDefinitionEditor.h>

#include <UI/Editor/BusDefinitionEditorEx/BusDefinitionEditorEx.h>

#include <UI/Editor/ComponentEditorEx/ComponentEditorEx.h>
#include <UI/Editor/ComponentEditorEx/ComponentEditorByProject.h>
#include <UI/Editor/TextEditor/TextEditor.h>

#include <UI/Common/NameGenerationPolicy.h>

#include <UI/Dialogs/newObjectDialog/newobjectdialog.h>

#include <UI/Common/graphicsItems/ComponentItem.h>
#include <UI/Common/graphicsItems/GraphicsColumnConstants.h>

#include <UI/GraphicalMessageMediator/GraphicalMessageMediator.h>

#include <UI/DrawingBoard/DrawingBoard.h>
#include <UI/ComponentEditor/componenteditor.h>
#include <UI/Dialogs/ProjectSettingsDialog/ProjectSettingsDialog.h>
#include <UI/Dialogs/LibrarySettingsDialog/LibrarySettingsDialog.h>

#include <UI/Dialogs/propertyPageDialog/PropertyPageDialog.h>

#include <UI/Dialogs/NewDesignDialog/NewDesignDialog.h>

#include <UI/Dialogs/NewIODialog/newiodialog.h>
#include <UI/Dialogs/NewIPDialog/newipdialog.h>
#include <UI/Dialogs/NewDesignExDialog/newdesignexdialog.h>
#include <UI/Dialogs/NewProjectDialog/newprojectdialog.h>

#include <UI/Dialogs/CopyProgressDialog/copyprogressdialog.h>

#include <UI/Widgets/ProgressWidget/progresswidget.h>

#include <UI/NewPages/NewCatalogPage.h>
#include <UI/NewPages/NewComponentPage.h>
#include <UI/NewPages/NewDesignPage.h>
#include <UI/NewPages/NewSWComponentPage.h>
#include <UI/NewPages/NewSWDesignPage.h>
#include <UI/NewPages/NewSystemPage.h>
#include <UI/NewPages/NewComDefinitionPage.h>
#include <UI/NewPages/NewApiDefinitionPage.h>
#include <UI/NewPages/NewBusDefinitionPage.h>
#include <UI/NewPages/NewProjectPage.h>

#include <UI/wizards/ComponentWizard/ComponentWizard.h>

#include <UI/HWDesign/HWDesignWidget.h>

#include <UI/WelcomeScreen.h>

#ifdef _WIN32
#include "common01/utility.h"
#else
#include "Common/utility.h"
#endif

#include "./Python/pythoncom.h"
#include "./Python/pythonscriptrunner.h"

#include "singleton.h"

#include "awsidetoolbar.h"
//#include "awelementwindow.h"
//#include "customtitlebar.h"

#include "mymainwindow.h"

#include "SplashScreen.h"

//#include "VersionHelper.h"

#include "mainwindow.h"


MainWindow::MainWindow(ProjectHandler* project,
                       ProjectLibraryHandler* projectLibrary,
                       FPGAHandler* fpga,
                       LibraryHandler* library,
                       MessageMediator* messageChannel,
                       QWidget *parent)
    : QMainWindow(parent),
      projectHandler_(project),
      projectLibraryHandler_(projectLibrary),
      fpgaHandler_(fpga),
      libraryHandler_(library),
      messageChannel_(messageChannel),
      toolbar_(new QToolBar(this)),
      sideToolBar_(new AWSideToolBar()),
      rightToolbar_(new QToolBar(this)),
//      designRightToolBar_(new DesignRightToolBar()),
      dockHandler_ (new DockWidgetHandler(projectHandler_,
                                          projectLibraryHandler_,
                                          fpgaHandler_,
                                          libraryHandler_,
                                          messageChannel,
                                          rightToolbar_,
                                          this)),

  #if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
      actVerilogGeneratort_(0),
  #endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618
  #if DIAGRAM_TOOLS_SUPPORT_20240104 == 1
      actAddColumn_(0),
      actToolSelect_(0),
      actToolConnect_(0),
      actToolInterface_(0),
      actToolDraft_(0),
      actToolToggleOffPage_(0),
      actToolLabel_(0),
  #endif // DIAGRAM_TOOLS_SUPPORT_20240104
//      actPythonCommunication_(0),
//      actPythonGenScript_(0),
      actZoomIn_(0),
      actZoomOut_(0),
      actZoomOriginal_(0),
      actFitInView_(0),
      actManualReload_(0),
      actProjectConfigure_(0),
      actLibraryConfigure_(0),
      workspace_(this, dockHandler_)
{
    Singleton::getInstance().setMainWindow(this);

    setWindowTitle("AWorld Magic ™");


//    GraphicalMessageMediator* guiChannel = dynamic_cast<GraphicalMessageMediator*>(messageChannel_);

    //ui->setupUi(this);
//    setMinimumSize(800, 600);

//    setWindowTitle(QCoreApplication::applicationName());
//    setWindowIcon(QIcon(":resources/Res/appicon.png"));

    // By default, the window is 1024x768 and set to maximized state.
    resize(1024, 768);
    setWindowState(Qt::WindowMaximized);

//    QString defaultStyleSheet(
//        "QCheckBox::indicator:unchecked { image: url(:resources/Res/traffic-light_gray.png);}"
//        "QCheckBox::indicator:indeterminate { image: url(:resources/Res/traffic-light_green_gray.png);}"
//        "QCheckBox::indicator:checked { image: url(:resources/Res/traffic-light_green.png);}"
//        "QGroupBox::title { subcontrol-origin: margin; margin: 0 8px; }"
//        "QGroupBox::indicator:unchecked {image: url(:resources/Res/traffic-light_gray.png);}"
//        "QGroupBox::indicator:checked {image: url(:resources/Res/traffic-light_green.png);}"
//        "QTableView::indicator:checked {image: url(:resources/Res/checkMark.png);}"
//        "QTableView::indicator:unchecked {image: none;}"
//        "QDockWidget::title {background-color: #89B6E2; font-size: 18pt; padding-left: 2px; padding-top: 2px;}"
//        "*[mandatoryField=\"true\"] { background-color: LemonChiffon; }");
//    setStyleSheet(defaultStyleSheet);

    setupToolbars();

//    setContextMenuPolicy(Qt::NoContextMenu);

    setupDrawBoard();

    setupVisibilitySetting();

    dockHandler_->setupDockWidgets();

    connectDockHandler();

    setupAndConnectProjectHandler();
    setupAndConnectProjectLibraryHandler();
    setupAndConnectFPGAHandler();
    setupAndConnectLibraryHandler();

//    setStatusBar(statusBar_);

    // some actions need the editors so set them up before the actions
    setupActions();

    // Restore program settings.
    restoreSettings();

    // don't display empty editors
    updateWindows();

    QSplitter* mainSplit = new QSplitter(this);
    mainSplit->addWidget(sideToolBar_);

#if LIBRARYDOCK_DISABLE_20231229 == 0
    mainSplit->addWidget(elementWindow);
#else
    mainSplit->addWidget(dockHandler_->getLibraryWidget());
#endif // LIBRARYDOCK_DISABLE_20231229

    mainSplit->addWidget(designTabs_);
//    mainSplit->addWidget(scriptEditor_);
//    scriptEditor_->setVisible(false);

    mainSplit->setSizes(QList<int>() << 30 << 170 << 600);

    setCentralWidget(mainSplit);

    onOpenWelcomeWindow();
}

MainWindow::~MainWindow()
{
    //delete ui;
}


//-----------------------------------------------------------------------------
// Function: MainWindow::onButtonClicked()
//-----------------------------------------------------------------------------
void MainWindow::onButtonClicked() {

    // 파일 선택 다이얼로그를 표시하고 선택된 파일의 경로를 얻습니다.
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*.cpp)"));

    // 사용자가 파일을 선택하면 아래 코드를 실행합니다.
    if (!filePath.isEmpty()) {
        // Send event to MyMainWindow via the Singleton
        MyMainWindow* mainWindow = Singleton::getInstance().getMainWorkWind();
        if(mainWindow) {
            QMetaObject::invokeMethod(mainWindow, "displayFilePathSlot", Q_ARG(QString, filePath));
        } else {
            qDebug() << "MyMainWindow instance is nullptr";
        }
    } else {
        qDebug() << "No file was selected";
    }
}


//-----------------------------------------------------------------------------
// Function: mainwindow::onClearItemSelection()
//-----------------------------------------------------------------------------
void MainWindow::onClearItemSelection()
{
    HWDesignWidget* designWidget = dynamic_cast<HWDesignWidget*>(designTabs_->currentWidget());

    dockHandler_->clearItemSelection(designWidget);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onDesignChanged()
//-----------------------------------------------------------------------------
void MainWindow::onDesignChanged()
{
    // find the currently open editor
    QWidget* widget = designTabs_->currentWidget();

    // if editor was found
    if (widget)
    {
        DesignWidget* editor = dynamic_cast<DesignWidget*>(widget);

        // if editor is design widget then set it to be modified.
        if (editor)
        {
            editor->setModified(true);
            updateMenuStrip();
        }
    }
}

//-----------------------------------------------------------------------------
// Function: MainWindow::setProjectLocations()
//-----------------------------------------------------------------------------
void MainWindow::setProjectLocations()
{
    QSettings settings;
    ProjectSettingsDialog dialog(settings, this);
    connect(&dialog, SIGNAL(scanProject()), this, SLOT(onProjectSearch()), Qt::UniqueConnection);

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: MainWindow::setLibraryLocations()
//-----------------------------------------------------------------------------
void MainWindow::setLibraryLocations()
{
    QSettings settings;
    LibrarySettingsDialog dialog(settings, this);
    connect(&dialog, SIGNAL(scanLibrary()), this, SLOT(onLibrarySearch()), Qt::UniqueConnection);

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onDesignDocumentRefreshed()
//-----------------------------------------------------------------------------
void MainWindow::onDesignDocumentRefreshed()
{
    dockHandler_->refreshDesignDocument(designTabs_->currentWidget());
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onAdjustVisibilityInWindow()
//-----------------------------------------------------------------------------
void MainWindow::onAdjustVisibilityInWindow(TabDocument::SupportedWindows type, bool show)
{
    int tabCount = designTabs_->count();
    QWidget* currentWidget = nullptr;
    if (tabCount > 0)
    {
        currentWidget = designTabs_->currentWidget();
    }

    dockHandler_->setWindowVisibilityForSupportedWindow(currentWidget, type, show);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenWelcomeWindow()
//-----------------------------------------------------------------------------
void MainWindow::onOpenWelcomeWindow()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    WelcomeScreen* welcomeScreen = new WelcomeScreen(this);

    designTabs_->addAndOpenDocument(welcomeScreen);

    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: createNew()
//-----------------------------------------------------------------------------
void MainWindow::createNew()
{
    // Create a property page dialog to work as a "New" dialog.
    PropertyPageDialog dialog(QSize(48, 48), 1, PropertyPageDialog::VIEW_ICONS,
                              PropertyPageDialog::APPLY_CURRENT, this);
    dialog.setFixedWidth(620);
    dialog.setWindowTitle(tr("New"));

    NewBusDefinitionPage* busPage = new NewBusDefinitionPage(libraryHandler_, &dialog);
    connect(busPage, SIGNAL(createBus(VLNV const&, QString const&)),
        this, SLOT(createBus(VLNV const&, QString const&)), Qt::UniqueConnection);
    dialog.addPage(QIcon(":resources/Res/bus-def.png"), tr("Bus Definition"), busPage);

    NewCatalogPage* catalogPage = new NewCatalogPage(libraryHandler_, &dialog);
    connect(catalogPage, SIGNAL(createCatalog(VLNV const&, QString const&)),
        this, SLOT(createCatalog(VLNV const&, QString const&)), Qt::UniqueConnection);
    dialog.addPage(QIcon(":resources/Res/catalog.png"), tr("Catalog"), catalogPage);

    // Add pages to the dialog.
    NewComponentPage* compPage = new NewComponentPage(libraryHandler_, &dialog);
    connect(compPage, SIGNAL(createComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            QVector<TagData>, VLNV const&, QString const&)),
        this, SLOT(createComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            QVector<TagData>, VLNV const&, QString const&)));
    dialog.addPage(QIcon(":resources/Res/hw-component.png"), tr("HW Component"), compPage);

    NewDesignPage* designPage = new NewDesignPage(libraryHandler_, &dialog);
    connect(designPage, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            QVector<TagData>, VLNV const&, QString const&)),
        this, SLOT(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            QVector<TagData>, VLNV const&, QString const&)));
    dialog.addPage(QIcon(":resources/Res/hw-design.png"), tr("HW Design"), designPage);

    NewSWComponentPage* swCompPage = new NewSWComponentPage(libraryHandler_, &dialog);
    connect(swCompPage, SIGNAL(createSWComponent(VLNV const&, QString const&)),
        this, SLOT(createSWComponent(VLNV const&, QString const&)));
    dialog.addPage(QIcon(":resources/Res/sw-component48x48.png"), tr("SW Component"), swCompPage);

    NewSWDesignPage* swDesignPage = new NewSWDesignPage(libraryHandler_, &dialog);
    connect(swDesignPage, SIGNAL(createSWDesign(VLNV const&, QString const&)),
            this, SLOT(createSWDesign(VLNV const&, QString const&)), Qt::UniqueConnection);
    dialog.addPage(QIcon(":resources/Res/sw-design48x48.png"), tr("SW Design"), swDesignPage);

    NewSystemPage* sysPage = new NewSystemPage(libraryHandler_, &dialog);
    connect(sysPage, SIGNAL(createSystem(VLNV const&, QString const&, VLNV const&, QString const&)),
        this, SLOT(createSystem(VLNV const&, QString const&, VLNV const&, QString const&)));
    dialog.addPage(QIcon(":resources/Res/system-component.png"), tr("System"), sysPage);

    NewApiDefinitionPage* apiDefPage = new NewApiDefinitionPage(libraryHandler_, &dialog);
    connect(apiDefPage, SIGNAL(createApiDefinition(VLNV const&, QString const&)),
        this, SLOT(createApiDefinition(VLNV const&, QString const&)), Qt::UniqueConnection);
    dialog.addPage(QIcon(":resources/Res/new-api_definition.png"), tr("API Definition"), apiDefPage);

    NewComDefinitionPage* comDefPage = new NewComDefinitionPage(libraryHandler_, &dialog);
    connect(comDefPage, SIGNAL(createComDefinition(VLNV const&, QString const&)),
            this, SLOT(createComDefinition(VLNV const&, QString const&)), Qt::UniqueConnection);
    dialog.addPage(QIcon(":resources/Res/new-com_definition.png"), tr("COM Definition"), comDefPage);

    dialog.finalizePages();

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: onCreateNewProject()
//-----------------------------------------------------------------------------
void MainWindow::onCreateNewProject()
{
    NewProjectDialog dialog;

    connect(&dialog, SIGNAL(createProject(Project const&, QString const&)),
            this, SLOT(onCreateProject(Project const&, QString const&)), Qt::UniqueConnection);

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: OnCreateNewBus()
//-----------------------------------------------------------------------------
void MainWindow::OnCreateNewBus()
{
    NewIODialog dialog;

    connect(&dialog, SIGNAL(createBus(VLNV const&)),
            this, SLOT(onCreateBus(VLNV const&)), Qt::UniqueConnection);

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: onCreateNewHWComponent()
//-----------------------------------------------------------------------------
void MainWindow::onCreateNewHWComponent()
{
    NewIPDialog dialog;

    connect(&dialog, SIGNAL(createComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            QVector<TagData>, VLNV const&, QString const&)),
        this, SLOT(onCreateHWComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            QVector<TagData>, VLNV const&, QString const&)));

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: onCreateNewDesign()
//-----------------------------------------------------------------------------
void MainWindow::onCreateNewDesign()
{
    NewDesignExDialog dialog;

    connect(&dialog, SIGNAL(createDesign(VLNV const&)),
            this, SLOT(onCreateDesign(VLNV const&)), Qt::UniqueConnection);

    dialog.exec();
}

//-----------------------------------------------------------------------------
// Function: openCSource()
//-----------------------------------------------------------------------------
//void MainWindow::openCSource(ComponentItem* compItem)
//{
//    Q_ASSERT(compItem != 0);

//    QSharedPointer<FileSet> fileSet = compItem->componentModel()->getFileSet("cSources");

//    if (fileSet == 0 || fileSet->getFiles()->count() == 0)
//    {
//        return;
//    }

//    QString filename;

//    if (fileSet->getFiles()->count() > 1)
//    {
//        // Show a dialog for selecting what source file to open.
//        ListSelectDialog saveDialog(this);
//        saveDialog.setWindowTitle(tr("Open Source"));
//        saveDialog.setDescription(tr("Select C source file to open:"));

//        for (QSharedPointer<File> file : *fileSet->getFiles())
//        {
//            saveDialog.addItem(new QListWidgetItem(file->name()));
//        }

//        if (saveDialog.exec() == QDialog::Rejected)
//        {
//            return;
//        }

//        filename = saveDialog.getSelectedItem()->text();
//    }
//    else
//    {
//        // Otherwise there is only one possibility.
//        filename = fileSet->getFiles()->first()->name();
//    }

//    if (compItem->componentModel()->getVlnv().isValid())
//    {
//        filename = General::getAbsolutePath(libraryHandler_->getPath(compItem->componentModel()->getVlnv()),
//            filename);
//    }

//    openCSource(filename, compItem->componentModel());
//}

//-----------------------------------------------------------------------------
// Function: MainWindow::openCSource()
//-----------------------------------------------------------------------------
//void MainWindow::openCSource(QString const& filename, QSharedPointer<Component> component)
//{
//    // Check if the source is already open and activate it.
//    for (int i = 0; i < designTabs_->count(); i++)
//    {
//        CSourceWidget* sourceWidget = dynamic_cast<CSourceWidget*>(designTabs_->widget(i));

//        if (sourceWidget != 0 && sourceWidget->getSourceFile() == filename)
//        {
//            designTabs_->setCurrentIndex(i);
//            return;
//        }
//    }

//    // Otherwise make sure that the file exists.
//    if (!QFile::exists(filename))
//    {
//        QMessageBox msgBox(QMessageBox::Critical, QCoreApplication::applicationName(),
//            "The source file " + filename + " is not found!", QMessageBox::Ok, this);
//        msgBox.exec();
//        return;
//    }

//    // Open the source to a view.
//    CSourceWidget* sourceWidget = new CSourceWidget(filename, component, libraryHandler_, this, this);
//    designTabs_->addAndOpenDocument(sourceWidget);

//    dynamic_cast<TabDocument*>(designTabs_->currentWidget())->setProtection(false);
//}

//-----------------------------------------------------------------------------
// Function: openDesign()
//-----------------------------------------------------------------------------
void MainWindow::openDesign(VLNV const& vlnv, QString const& viewName)
{
//    // The vlnv must always be for a component.
//    Q_ASSERT(libraryHandler_->getDocumentType(vlnv) == VLNV::COMPONENT);

//    QSharedPointer<Component> comp = libraryHandler_->getModel(vlnv).staticCast<Component>();

//    // Check if the design is already open.
//    VLNV refVLNV = comp->getHierRef(viewName);
//    VLNV designVLNV = libraryHandler_->getDesignVLNV(refVLNV);
//    if (isOpen(designVLNV) || hasInvalidReferences(comp->getHierRefs(), vlnv, libraryHandler_))
//    {
//        return;
//    }

//    QSharedPointer<Design> newDesign = libraryHandler_->getDesign(designVLNV);
//    dockHandler_->setupDesignParameterFinder(newDesign);

//    DesignWidgetFactoryImplementation factory(libraryHandler_,
//                                              dockHandler_->getDesignAndInstanceParameterFinder(),
//                                              dockHandler_->getDesignParameterFinder());

//    DesignWidget* designWidget = factory.makeHWDesignWidget(this);

//    // open the design in the designWidget
//    designWidget->setDesign(vlnv, viewName);

//    // if the design could not be opened
//    if (designWidget->getOpenDocument().isEmpty())
//    {
//        delete designWidget;
//        return;
//    }

//    connect(designWidget, SIGNAL(zoomChanged()), this, SLOT(onUpdateZoomTools()), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(modeChanged(DrawMode)),
//        this, SLOT(onDrawModeChanged(DrawMode)), Qt::UniqueConnection);

//    connect(designWidget, SIGNAL(destroyed(QObject*)), this, SLOT(onClearItemSelection()), Qt::UniqueConnection);

//    connect(designWidget, SIGNAL(openDesign(const VLNV&, const QString&)),
//        this, SLOT(openDesign(const VLNV&, const QString&)));
//    connect(designWidget, SIGNAL(openComponent(const VLNV&)),
//        this, SLOT(openComponent(const VLNV&)), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(openBus(VLNV const&)),
//        this, SLOT(openBus(VLNV const&)), Qt::UniqueConnection);

//    connect(designWidget, SIGNAL(componentSelected(ComponentItem*)),
//        this, SLOT(onComponentSelected(ComponentItem*)), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(interfaceSelected(ConnectionEndpoint*)),
//        this, SLOT(onInterfaceSelected(ConnectionEndpoint*)), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(connectionSelected(GraphicsConnection*)),
//        this, SLOT(onConnectionSelected(GraphicsConnection*)), Qt::UniqueConnection);

//    connect(designWidget, SIGNAL(clearItemSelection()),
//        this, SLOT(onClearItemSelection()), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(refreshed()), this, SLOT(onDesignDocumentRefreshed()), Qt::UniqueConnection);

//    designTabs_->addAndOpenDocument(designWidget);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::openBus()
//-----------------------------------------------------------------------------
void MainWindow::openBus(const VLNV& busDefVLNV)
{
    if (isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor && editor->getDocumentVLNV() == busDefVLNV)
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;

    if (libraryHandler_->contains(busDefVLNV) &&
        libraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = libraryHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, libraryHandler_, busDef);

    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::openAbsDef()
//-----------------------------------------------------------------------------
void MainWindow::openAbsDef(const VLNV& absDefVLNV)
{
    if (isOpen(absDefVLNV) || !absDefVLNV.isValid())
    {
        return;
    }

    // Check if the abstraction editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor && editor->getDocumentVLNV() == absDefVLNV)
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    QSharedPointer<AbstractionDefinition> absDef;

    if (absDefVLNV.isValid())
    {
        if (libraryHandler_->contains(absDefVLNV) &&
            libraryHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = libraryHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
                absDefVLNV.toString()));
            return;
        }
    }


    auto editor = new AbstractionDefinitionEditor(this, libraryHandler_, absDef);

    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenBus()
//-----------------------------------------------------------------------------
void MainWindow::onOpenBus(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef)
{
    if (isOpen(absDefVLNV) || isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor &&
            ((absDefVLNV.isValid() && editor->getDocumentVLNV() == absDefVLNV) ||
            editor->getDocumentVLNV() == busDefVLNV))
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;
    QSharedPointer<AbstractionDefinition> absDef;

    if (libraryHandler_->contains(busDefVLNV) &&
        libraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = libraryHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    if (absDefVLNV.isValid())
    {
        if (libraryHandler_->contains(absDefVLNV) &&
            libraryHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = libraryHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
                absDefVLNV.toString()));
            return;
        }
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, libraryHandler_, busDef, absDef, absDef && disableBusDef);
//    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, libraryHandler_, busDef);

//    auto editor = new AbstractionDefinitionEditor(this, libraryHandler_, absDef);


    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenAbsDef()
//-----------------------------------------------------------------------------
void MainWindow::onOpenAbsDef(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef)
{
    if (isOpen(absDefVLNV) || isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor &&
            ((absDefVLNV.isValid() && editor->getDocumentVLNV() == absDefVLNV) ||
            editor->getDocumentVLNV() == busDefVLNV))
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;
    QSharedPointer<AbstractionDefinition> absDef;

    if (libraryHandler_->contains(busDefVLNV) &&
        libraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = libraryHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    if (absDefVLNV.isValid())
    {
        if (libraryHandler_->contains(absDefVLNV) &&
            libraryHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = libraryHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
                absDefVLNV.toString()));
            return;
        }
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, libraryHandler_, busDef, absDef, absDef && disableBusDef);
//    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, libraryHandler_, busDef);

//    auto editor = new AbstractionDefinitionEditor(this, libraryHandler_, absDef);


    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::openComponent()
//-----------------------------------------------------------------------------
void MainWindow::openComponent(VLNV const& vlnv)
{
    if (isOpen(vlnv))
    {
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QSharedPointer<Component> component;

    if (libraryHandler_->contains(vlnv))
    {
        component = libraryHandler_->getModel(vlnv).dynamicCast<Component>();
    }
    else
    {
//        emit errorMessage(tr("[ ERROR ] | Open | VLNV %1 was not found in the library").arg(vlnv.toString()));
        QApplication::restoreOverrideCursor();
        return;
    }

    if (!component)
    {
//        emit errorMessage(tr("[ ERROR ] | Open | Document could not be opened for Component"));
        QApplication::restoreOverrideCursor();
        return;
    }

    ComponentEditor* editor = new ComponentEditor(libraryHandler_, component, this);

//    connect(editor, SIGNAL(openCSource(QString const&, QSharedPointer<Component>)),
//            this , SLOT(openCSource(QString const&, QSharedPointer<Component>)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openDesign(const VLNV&, const QString&)),
        this, SLOT(openDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openBus(const VLNV&)),
        this, SLOT(openBus(const VLNV&)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openAbsDef(const VLNV&)),
        this, SLOT(openAbsDef(const VLNV&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openComDefinition(const VLNV&)),
//        this, SLOT(openComDefinition(const VLNV&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openSWDesign(const VLNV&, const QString&)),
//        this, SLOT(openSWDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openSystemDesign(const VLNV&, const QString&)),
//        this, SLOT(openSystemDesign(const VLNV&, const QString&)), Qt::UniqueConnection);

    designTabs_->addAndOpenDocument(editor);

    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: onOpenDesign_Project()
//-----------------------------------------------------------------------------
void MainWindow::onOpenDesign_Project(VLNV const& vlnv, QString const& viewName)
{
    // The vlnv must always be for a component.
    Q_ASSERT(projectHandler_->getDocumentType(vlnv) == VLNV::COMPONENT);

    QSharedPointer<Component> comp = projectHandler_->getModel(vlnv).staticCast<Component>();

    // Check if the design is already open.
    VLNV refVLNV = comp->getHierRef(viewName);
    VLNV designVLNV = projectHandler_->getDesignVLNV(refVLNV);
    if (isOpen(designVLNV) || hasInvalidReferences(comp->getHierRefs(), vlnv, projectHandler_))
    {
        return;
    }

    QSharedPointer<Design> newDesign = projectHandler_->getDesign(designVLNV);
    dockHandler_->setupDesignParameterFinder(newDesign);

    DesignWidgetFactoryImplementation factory(projectHandler_,
                                              projectLibraryHandler_,
                                              dockHandler_->getDesignAndInstanceParameterFinder(),
                                              dockHandler_->getDesignParameterFinder());

    DesignWidget* designWidget = factory.makeHWDesignWidget(this);

    // open the design in the designWidget
    designWidget->setDesign(vlnv, viewName);

    // if the design could not be opened
    if (designWidget->getOpenDocument().isEmpty())
    {
        delete designWidget;
        return;
    }

    connect(designWidget, SIGNAL(zoomChanged()), this, SLOT(onUpdateZoomTools()), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(modeChanged(DrawMode)),
        this, SLOT(onDrawModeChanged(DrawMode)), Qt::UniqueConnection);

    connect(designWidget, SIGNAL(destroyed(QObject*)), this, SLOT(onClearItemSelection()), Qt::UniqueConnection);

    connect(designWidget, SIGNAL(openDesign(const VLNV&, const QString&)),
        this, SLOT(openDesign(const VLNV&, const QString&)));
    connect(designWidget, SIGNAL(openComponent(const VLNV&)),
        this, SLOT(openComponent(const VLNV&)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(openBus(VLNV const&)),
        this, SLOT(openBus(VLNV const&)), Qt::UniqueConnection);

    connect(designWidget, SIGNAL(componentSelected(ComponentItem*)),
        this, SLOT(onComponentSelected(ComponentItem*)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(interfaceSelected(ConnectionEndpoint*)),
        this, SLOT(onInterfaceSelected(ConnectionEndpoint*)), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(connectionSelected(GraphicsConnection*)),
//        this, SLOT(onConnectionSelected(GraphicsConnection*)), Qt::UniqueConnection);

    connect(designWidget, SIGNAL(clearItemSelection()), this, SLOT(onClearItemSelection()), Qt::UniqueConnection);

    connect(designWidget, SIGNAL(refreshed()), this, SLOT(onDesignDocumentRefreshed()), Qt::UniqueConnection);

    designTabs_->addAndOpenDocument(designWidget);
}



//-----------------------------------------------------------------------------
// Function: onOpenDesign_Console()
//-----------------------------------------------------------------------------
void MainWindow::onOpenDesign_Console(VLNV const& vlnv, QString const& viewName)
{
    // The vlnv must always be for a component.
    Q_ASSERT(projectHandler_->getDocumentType(vlnv) == VLNV::COMPONENT);
    QSharedPointer<Component> comp = projectHandler_->getModel(vlnv).staticCast<Component>();

    // Check if the design is already open.
    VLNV refVLNV = comp->getHierRef(viewName);
    VLNV designVLNV = projectHandler_->getDesignVLNV(refVLNV);

    if (isOpen(designVLNV) || hasInvalidReferences(comp->getHierRefs(), vlnv, projectHandler_))
    {
        return;
    }

    QSharedPointer<Design> newDesign = projectHandler_->getDesign(designVLNV);
    dockHandler_->setupDesignParameterFinder(newDesign);

    DesignWidgetFactoryImplementation factory(projectHandler_,
                                              projectLibraryHandler_,
                                              dockHandler_->getDesignAndInstanceParameterFinder(),
                                              dockHandler_->getDesignParameterFinder());
    DesignWidget* designWidget = factory.makeHWDesignWidget(this);

    // open the design in the designWidget
    designWidget->setDesign(vlnv, viewName);

    // if the design could not be opened
    if (designWidget->getOpenDocument().isEmpty())
    {
        delete designWidget;
        return;
    }

    connect(designWidget, SIGNAL(zoomChanged()), this, SLOT(onUpdateZoomTools()), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(modeChanged(DrawMode)),
        this, SLOT(onDrawModeChanged(DrawMode)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(destroyed(QObject*)), this, SLOT(onClearItemSelection()), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(openDesign(const VLNV&, const QString&)),
        this, SLOT(openDesign(const VLNV&, const QString&)));
    connect(designWidget, SIGNAL(openComponent(const VLNV&)),
        this, SLOT(openComponent(const VLNV&)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(openBus(VLNV const&)),
        this, SLOT(openBus(VLNV const&)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(componentSelected(ComponentItem*)),
        this, SLOT(onComponentSelected(ComponentItem*)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(interfaceSelected(ConnectionEndpoint*)),
        this, SLOT(onInterfaceSelected(ConnectionEndpoint*)), Qt::UniqueConnection);
//    connect(designWidget, SIGNAL(connectionSelected(GraphicsConnection*)),
//        this, SLOT(onConnectionSelected(GraphicsConnection*)), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(clearItemSelection()), this, SLOT(onClearItemSelection()), Qt::UniqueConnection);
    connect(designWidget, SIGNAL(refreshed()), this, SLOT(onDesignDocumentRefreshed()), Qt::UniqueConnection);

    designTabs_->addAndOpenDocument_Console(designWidget);
}



//-----------------------------------------------------------------------------
// Function: onRefreshDesign_Console()
//-----------------------------------------------------------------------------
void MainWindow::onRefreshDesign_Console(VLNV const& vlnv, QString const& viewName)
{
    // The vlnv must always be for a component.
    Q_ASSERT(projectHandler_->getDocumentType(vlnv) == VLNV::COMPONENT);
    QSharedPointer<Component> comp = projectHandler_->getModel(vlnv).staticCast<Component>();

    // Check if the design is already open.
    VLNV refVLNV = comp->getHierRef(viewName);
    VLNV designVLNV = projectHandler_->getDesignVLNV(refVLNV);

    if (!isOpen(designVLNV))
    {
        return;
    }

    if (isOpen(designVLNV) || hasInvalidReferences(comp->getHierRefs(), vlnv, projectHandler_))
    {
        // close();
        for (int i = 0; i < designTabs_->count(); i++)
        {
            TabDocument* document = dynamic_cast<TabDocument*>(designTabs_->widget(i));

            // if the document is already open is some tab
            if (document && document->getIdentifyingVLNV() == designVLNV)
            {
                designTabs_->refreshCurrentDocument();
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: onCloseDesign_Console()
//-----------------------------------------------------------------------------
void MainWindow::onCloseDesign_Console(VLNV const& vlnv, QString const& viewName)
{
    // The vlnv must always be for a component.
    Q_ASSERT(projectHandler_->getDocumentType(vlnv) == VLNV::COMPONENT);
    QSharedPointer<Component> comp = projectHandler_->getModel(vlnv).staticCast<Component>();

    // Check if the design is already open.
    VLNV refVLNV = comp->getHierRef(viewName);
    VLNV designVLNV = projectHandler_->getDesignVLNV(refVLNV);

    if (isOpen(designVLNV) || hasInvalidReferences(comp->getHierRefs(), vlnv, projectHandler_))
    {
        // close();
        for (int i = 0; i < designTabs_->count(); i++)
        {
            TabDocument* document = dynamic_cast<TabDocument*>(designTabs_->widget(i));
            // if the document is already open is some tab
            if (document && document->getIdentifyingVLNV() == designVLNV)
            {
                designTabs_->closeAndRemoveDocument_Console(i);

                emit noticeMessage("[ SYSTEM ] | Close | Design closed.");
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Function: onCloseAllDesign_Console()
//-----------------------------------------------------------------------------
void MainWindow::onCloseAllDesign_Console()
{
    designTabs_->closeAll();
}



//-----------------------------------------------------------------------------
// Function: onOpenFile_Console()
//-----------------------------------------------------------------------------
void MainWindow::onOpenFile_Console(QString filePath)
{
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* document = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (document && document->getDocumentName() == filePath)
        {
            designTabs_->setCurrentIndex(i);

            return;
        }

        else
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

            TextEditor* textEditor = new TextEditor(this);
            textEditor->loadFile(filePath);

            designTabs_->addAndOpenDocument_Console(textEditor);

            QApplication::restoreOverrideCursor();

            return;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenBus_Project()
//-----------------------------------------------------------------------------
void MainWindow::onOpenBus_Project(const VLNV& busDefVLNV)
{
    if (isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor && editor->getDocumentVLNV() == busDefVLNV)
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;

    if (projectHandler_->contains(busDefVLNV) &&
        projectHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = projectHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, projectHandler_, busDef);

    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenAbsDef_Project()
//-----------------------------------------------------------------------------
void MainWindow::onOpenAbsDef_Project(const VLNV& absDefVLNV)
{
    if (isOpen(absDefVLNV) || !absDefVLNV.isValid())
    {
        return;
    }

    // Check if the abstraction editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor && editor->getDocumentVLNV() == absDefVLNV)
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    QSharedPointer<AbstractionDefinition> absDef;

    if (absDefVLNV.isValid())
    {
        if (projectHandler_->contains(absDefVLNV) &&
            projectHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = projectHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
                absDefVLNV.toString()));
            return;
        }
    }


    auto editor = new AbstractionDefinitionEditor(this, projectHandler_, absDef);

    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenComponent_Project()
//-----------------------------------------------------------------------------
void MainWindow::onOpenComponent_Project(VLNV const& vlnv)
{
    if (isOpen(vlnv))
    {
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QSharedPointer<Component> component;

    if (projectHandler_->contains(vlnv))
    {
        component = projectHandler_->getModel(vlnv).dynamicCast<Component>();
    }
    else
    {
//        emit errorMessage(tr("[ ERROR ] | Open | VLNV %1 was not found in the library").arg(vlnv.toString()));
        QApplication::restoreOverrideCursor();
        return;
    }

    if (!component)
    {
//        emit errorMessage(tr("[ ERROR ] | Open | Document could not be opened for Component"));
        QApplication::restoreOverrideCursor();
        return;
    }

    ComponentEditorByProject* editor = new ComponentEditorByProject(projectHandler_, projectLibraryHandler_, component, this);

//    connect(editor, SIGNAL(openCSource(QString const&, QSharedPointer<Component>)),
//            this , SLOT(openCSource(QString const&, QSharedPointer<Component>)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openDesign(const VLNV&, const QString&)),
            this, SLOT(onOpenDesign_Project(const VLNV&, const QString&)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openBus(const VLNV&)),
        this, SLOT(openBus(const VLNV&)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openAbsDef(const VLNV&)),
        this, SLOT(openAbsDef(const VLNV&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openComDefinition(const VLNV&)),
//        this, SLOT(openComDefinition(const VLNV&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openSWDesign(const VLNV&, const QString&)),
//        this, SLOT(openSWDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openSystemDesign(const VLNV&, const QString&)),
//        this, SLOT(openSystemDesign(const VLNV&, const QString&)), Qt::UniqueConnection);

    designTabs_->addAndOpenDocument(editor);

    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenBus_projectLibrary()
//-----------------------------------------------------------------------------
void MainWindow::onOpenBus_projectLibrary(const VLNV& busDefVLNV)
{
    if (isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor && editor->getDocumentVLNV() == busDefVLNV)
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;

    if (projectLibraryHandler_->contains(busDefVLNV) &&
        projectLibraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = projectLibraryHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
//        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, projectLibraryHandler_, busDef);

    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenAbsDef_projectLibrary()
//-----------------------------------------------------------------------------
void MainWindow::onOpenAbsDef_projectLibrary(const VLNV& absDefVLNV)
{
    if (isOpen(absDefVLNV) || !absDefVLNV.isValid())
    {
        return;
    }

    // Check if the abstraction editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor && editor->getDocumentVLNV() == absDefVLNV)
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    QSharedPointer<AbstractionDefinition> absDef;

    if (absDefVLNV.isValid())
    {
        if (projectLibraryHandler_->contains(absDefVLNV) &&
            projectLibraryHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = projectLibraryHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
//            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
//                absDefVLNV.toString()));
            return;
        }
    }

    auto editor = new AbstractionDefinitionEditor(this, projectLibraryHandler_, absDef);

    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenBus_projectLibrary()
//-----------------------------------------------------------------------------
void MainWindow::onOpenBus_projectLibrary(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef)
{
    if (isOpen(absDefVLNV) || isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor &&
            ((absDefVLNV.isValid() && editor->getDocumentVLNV() == absDefVLNV) ||
            editor->getDocumentVLNV() == busDefVLNV))
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;
    QSharedPointer<AbstractionDefinition> absDef;

    if (projectLibraryHandler_->contains(busDefVLNV) &&
        projectLibraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = projectLibraryHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
//        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    if (absDefVLNV.isValid())
    {
        if (projectLibraryHandler_->contains(absDefVLNV) &&
            projectLibraryHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = projectLibraryHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
//            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
//                absDefVLNV.toString()));
            return;
        }
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, projectLibraryHandler_, busDef, absDef, absDef && disableBusDef);
//    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, projectLibraryHandler_, busDef);

//    auto editor = new AbstractionDefinitionEditor(this, projectLibraryHandler_, absDef);


    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenAbsDef_projectLibrary()
//-----------------------------------------------------------------------------
void MainWindow::onOpenAbsDef_projectLibrary(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef)
{
    if (isOpen(absDefVLNV) || isOpen(busDefVLNV) || !busDefVLNV.isValid())
    {
        return;
    }

    // Check if the bus editor is already open and activate it.
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* editor = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (editor &&
            ((absDefVLNV.isValid() && editor->getDocumentVLNV() == absDefVLNV) ||
            editor->getDocumentVLNV() == busDefVLNV))
        {
            designTabs_->setCurrentIndex(i);
            return;
        }
    }

    // Editor for given vlnv was not yet open so create one for it
    QSharedPointer<BusDefinition> busDef;
    QSharedPointer<AbstractionDefinition> absDef;

    if (projectLibraryHandler_->contains(busDefVLNV) &&
        projectLibraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION)
    {
        busDef = projectLibraryHandler_->getModel(busDefVLNV).dynamicCast<BusDefinition>();
    }
    else
    {
        emit errorMessage(tr("[ ERROR ] | Open | Bus definition %1 was not found in the library").arg(busDefVLNV.toString()));
        return;
    }

    if (absDefVLNV.isValid())
    {
        if (projectLibraryHandler_->contains(absDefVLNV) &&
            projectLibraryHandler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
        {
            absDef = projectLibraryHandler_->getModel(absDefVLNV).staticCast<AbstractionDefinition>();
        }
        else
        {
            emit errorMessage(tr("[ ERROR ] | Open | Abstraction definition %1 was not found in the library").arg(
                absDefVLNV.toString()));
            return;
        }
    }

    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, projectLibraryHandler_, busDef, absDef, absDef && disableBusDef);
//    BusDefinitionEditorEx* editor = new BusDefinitionEditorEx(this, projectLibraryHandler_, busDef);

//    auto editor = new AbstractionDefinitionEditor(this, projectLibraryHandler_, absDef);


    designTabs_->addAndOpenDocument(editor);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onOpenComponent_ProjectLibrary()
//-----------------------------------------------------------------------------
void MainWindow::onOpenComponent_ProjectLibrary(VLNV const& vlnv)
{
    if (isOpen(vlnv))
    {
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QSharedPointer<Component> component;

    if (projectLibraryHandler_->contains(vlnv))
    {
        component = projectLibraryHandler_->getModel(vlnv).dynamicCast<Component>();
    }
    else
    {
//        emit errorMessage(tr("[ ERROR ] | Open | VLNV %1 was not found in the library").arg(vlnv.toString()));
        QApplication::restoreOverrideCursor();
        return;
    }

    if (!component)
    {
//        emit errorMessage("[ ERROR ] | Open | Document could not be opened for Component");
        QApplication::restoreOverrideCursor();
        return;
    }

    ComponentEditorEx* editor = new ComponentEditorEx(projectLibraryHandler_, component, this);

//    connect(editor, SIGNAL(openCSource(QString const&, QSharedPointer<Component>)),
//            this , SLOT(openCSource(QString const&, QSharedPointer<Component>)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openDesign(const VLNV&, const QString&)),
            this, SLOT(onOpenComponent_ProjectLibrary(const VLNV&, const QString&)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openBus(const VLNV&)),
        this, SLOT(openBus(const VLNV&)), Qt::UniqueConnection);
    connect(editor, SIGNAL(openAbsDef(const VLNV&)),
        this, SLOT(openAbsDef(const VLNV&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openComDefinition(const VLNV&)),
//        this, SLOT(openComDefinition(const VLNV&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openSWDesign(const VLNV&, const QString&)),
//        this, SLOT(openSWDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
//    connect(editor, SIGNAL(openSystemDesign(const VLNV&, const QString&)),
//        this, SLOT(openSystemDesign(const VLNV&, const QString&)), Qt::UniqueConnection);

    designTabs_->addAndOpenDocument(editor);

    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onCopyVLNV_projectLibrary()
//-----------------------------------------------------------------------------
void MainWindow::onCopyVLNV_projectLibrary(VLNV const& vlnv)
{
    QString location = QSettings().value(QStringLiteral("Library/DefaultLocation")).toString();

    QString sourcePath = QString("%1/%2/%3/%4/%5").arg( location )
            .arg( vlnv.getVendor() )
            .arg( vlnv.getLibrary() )
            .arg( vlnv.getName() )
            .arg( vlnv.getVersion() );

    location = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

    location = location + "/library";

    QString destinationPath = QString("%1/%2/%3/%4/%5").arg( location )
            .arg( vlnv.getVendor() )
            .arg( vlnv.getLibrary() )
            .arg( vlnv.getName() )
            .arg( vlnv.getVersion() );


    bool bRet = Utility::copyDirectoryContents(sourcePath, destinationPath);

    if (bRet) {
        onProjectLibrarySearch();

        emit inputMessage("copylib " + vlnv.getVendor() + " " + vlnv.getLibrary() + " " + vlnv.getName() + " " + vlnv.getVersion());

        emit noticeMessage("[ SYSTEM ] | Copy Library | Selected library has been copied.");
    }

    else {
        emit errorMessage("[ ERROR ] | Copy Library | Something is wrong.");
    }

//    QFileInfo fileInfo(sourcePath);

//    if (fileInfo.isDir()) {
//        copyDirectoryRecursively(sourcePath, destinationPath);
//    }
}

//-----------------------------------------------------------------------------
// Function: MainWindow::onOpenFPGA()
//-----------------------------------------------------------------------------
void MainWindow::onOpenFPGA(QString const& filePath)
{
    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* document = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        if (document && document->getDocumentName() == filePath)
        {
            designTabs_->setCurrentIndex(i);

            return;
        }

        else
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

            TextEditor* textEditor = new TextEditor(this);
            textEditor->loadFile(filePath);

            designTabs_->addAndOpenDocument(textEditor);

            QApplication::restoreOverrideCursor();

            return;
        }
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    TextEditor* textEditor = new TextEditor(this);
    textEditor->loadFile(filePath);

    designTabs_->addAndOpenDocument(textEditor);

    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: MainWindow::onLastDocumentClosed()
//-----------------------------------------------------------------------------
void MainWindow::onLastDocumentClosed()
{
    updateWindows();
}

//-----------------------------------------------------------------------------
// Function: MainWindow::onDocumentChanged()
//-----------------------------------------------------------------------------
void MainWindow::onDocumentChanged(int index)
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->widget(index));

    // update the menu
    if (doc)
    {
        updateWindows();

#if VISIBILITY_TOOLS_SUPPORT_20240104 == 1
        updateVisibilityControlMenu(doc);
#endif

    }

    dockHandler_->documentChanged(doc);

    // Reset the draw mode to select.
    if (doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT))
    {
        doc->setMode(MODE_SELECT);
    }
    else
    {
#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1
        onDrawModeChanged(MODE_SELECT);
#endif

    }

    // Update the menu strip.
    updateMenuStrip();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onProjectSearch()
//
//-----------------------------------------------------------------------------
void MainWindow::onProjectSearch()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    projectHandler_->searchForIPXactFiles();
    projectLibraryHandler_->searchForIPXactFiles();
    fpgaHandler_->searchForFPGAFiles();
    libraryHandler_->searchForIPXactFiles();
    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onProjectLibrarySearch()
//
//----------------------------------------------------------------------------
void MainWindow::onProjectLibrarySearch()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    projectLibraryHandler_->searchForIPXactFiles();
    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onLibrarySearch()
//
//-----------------------------------------------------------------------------
void MainWindow::onLibrarySearch()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    projectHandler_->searchForIPXactFiles();
    projectLibraryHandler_->searchForIPXactFiles();
    fpgaHandler_->searchForFPGAFiles();
    libraryHandler_->searchForIPXactFiles();
    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onComponentSelected()
//-----------------------------------------------------------------------------
void MainWindow::onComponentSelected(ComponentItem* component)
{
    Q_ASSERT(component);

    dockHandler_->selectComponent(designTabs_->currentWidget(), component);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onInterfaceSelected()
//-----------------------------------------------------------------------------
void MainWindow::onInterfaceSelected(ConnectionEndpoint* interface)
{
    Q_ASSERT(interface);

    dockHandler_->selectConnectionInterface(designTabs_->currentWidget(), interface);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::createComponent()
//-----------------------------------------------------------------------------
void MainWindow::createComponent(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
    QVector<TagData> tags, VLNV const& vlnv, QString const& directory)
{
    Q_ASSERT(vlnv.isValid());

    // Create a component.
    QSharedPointer<Component> component = QSharedPointer<Component>(new Component(vlnv));

    // Set Kactus attributes.
    component->setHierarchy(prodHier);
    component->setFirmness(firmness);
    component->setTags(tags);
    component->setImplementation(KactusAttribute::HW);

    component->setVersion(VersionHelper::versionFileStr());

    // Create the file.
    if (!libraryHandler_->writeModelToFile(directory, component))
    {
        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
        return;
    }

    runComponentWizard(component, directory);

    // Open the component editor.
    openComponent(vlnv);

    unlockNewlyCreatedDocument(vlnv);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::createDesign()
//-----------------------------------------------------------------------------
void MainWindow::createDesign(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
    QVector<TagData> tags, VLNV const& vlnv, QString const& directory)
{
    Q_ASSERT(vlnv.isValid());

    //. TODO 디자인 프로세스 실행 코드 구현 요망
#if DESIGN_SCRIPT_ENABLE_20240125 == 0

    VLNV designVLNV(VLNV::DESIGN, vlnv.getVendor(), vlnv.getLibrary(),
        vlnv.getName().remove(".comp") + ".design", vlnv.getVersion());
    VLNV desConfVLNV(VLNV::DESIGNCONFIGURATION, vlnv.getVendor(), vlnv.getLibrary(),
        vlnv.getName().remove(".comp") + ".designcfg", vlnv.getVersion());

    // Create a component and a hierarchical view .
    QSharedPointer<Component> component(new Component(vlnv));

    // Set Kactus2 attributes.
    component->setHierarchy(prodHier);
    component->setFirmness(firmness);
    component->setTags(tags);

    component->setImplementation(KactusAttribute::HW);
    component->setVersion(VersionHelper::versionFileStr());

    QSharedPointer<View> newHierarchicalView(new View());
    newHierarchicalView->setName(NameGenerationPolicy::hierarchicalViewName());

    QSharedPointer<ConfigurableVLNVReference> tempReference (new ConfigurableVLNVReference(desConfVLNV));

    QSharedPointer<DesignConfigurationInstantiation> hierarchicalInstantiation
        (new DesignConfigurationInstantiation(desConfVLNV.getName() + "_" + desConfVLNV.getVersion()));
    hierarchicalInstantiation->setDesignConfigurationReference(tempReference);

    newHierarchicalView->setDesignConfigurationInstantiationRef(hierarchicalInstantiation->name());

    component->getDesignConfigurationInstantiations()->append(hierarchicalInstantiation);
    component->getViews()->append(newHierarchicalView);

    QStringList viewNames = component->getHierViews();
    Q_ASSERT(!viewNames.isEmpty());

    // Create the design and design configuration.
    QSharedPointer<Design> design(new Design(designVLNV));
    design->setDesignImplementation(KactusAttribute::HW);
    design->setVersion(VersionHelper::versionFileStr());

    QSharedPointer<DesignConfiguration> designConf(new DesignConfiguration(desConfVLNV));
    designConf->setDesignRef(designVLNV);
    designConf->setDesignConfigImplementation(KactusAttribute::HW);
    designConf->setVersion(VersionHelper::versionFileStr());

    // Create the files.
    projectHandler_->beginSave();

    bool success = true;

    if (!projectHandler_->writeModelToFile(directory, designConf))
    {
        success = false;
    }

    if (!projectHandler_->writeModelToFile(directory, design))
    {
        success = false;
    }

    if (!projectHandler_->writeModelToFile(directory, component))
    {
        success = false;
    }

    projectHandler_->endSave();

    if (success)
    {
        // Open the design.
        onOpenDesign_Project(vlnv, viewNames.first());

        unlockNewlyCreatedDocument(vlnv);
    }
    else
    {
        emit errorMessage("Error saving files to disk.");
    }
#else

    VLNV designVLNV(VLNV::DESIGN, vlnv.getVendor(), vlnv.getLibrary(),
        vlnv.getName().remove(".comp") + ".design", vlnv.getVersion());
    VLNV desConfVLNV(VLNV::DESIGNCONFIGURATION, vlnv.getVendor(), vlnv.getLibrary(),
        vlnv.getName().remove(".comp") + ".designcfg", vlnv.getVersion());

    // Create a component and a hierarchical view .
    QSharedPointer<Component> component(new Component(vlnv));

    // Set Kactus2 attributes.
    component->setHierarchy(prodHier);
    component->setFirmness(firmness);
    component->setTags(tags);

    component->setImplementation(KactusAttribute::HW);
    component->setVersion(VersionHelper::versionFileStr());

    QSharedPointer<View> newHierarchicalView(new View());
    newHierarchicalView->setName(NameGenerationPolicy::hierarchicalViewName());

    QSharedPointer<ConfigurableVLNVReference> tempReference (new ConfigurableVLNVReference(desConfVLNV));

    QSharedPointer<DesignConfigurationInstantiation> hierarchicalInstantiation
        (new DesignConfigurationInstantiation(desConfVLNV.getName() + "_" + desConfVLNV.getVersion()));
    hierarchicalInstantiation->setDesignConfigurationReference(tempReference);

    newHierarchicalView->setDesignConfigurationInstantiationRef(hierarchicalInstantiation->name());

    component->getDesignConfigurationInstantiations()->append(hierarchicalInstantiation);
    component->getViews()->append(newHierarchicalView);

    QStringList viewNames = component->getHierViews();
    Q_ASSERT(!viewNames.isEmpty());

    // Create the files.
    QString currentPath = QDir::currentPath();

    QString workingDirectory = SettingManager::getDefaultProjectLocation();

    QString scriptDirectory = workingDirectory + "/Script";

    QDir::setCurrent(workingDirectory);

    QString test = QDir::currentPath();

    QStringList arguments;

    QProcess *process;

    process = new QProcess(this);

     process->start("/bin/csh", QStringList() << "-c" << "source setprj");

    if (process->waitForFinished(-1)) {
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;
    }
    else {
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error output:" << error;
    }

    bool bRet = false;
    QString program;
    program = workingDirectory + "/bin/awsh";


    arguments.append("python2");
    arguments.append(scriptDirectory + "/hw_design_gen_by_tony/script_gen.py");
    arguments.append("-vendor");
//    arguments.append(pri.getName());
    arguments.append(vlnv.getVendor());
    arguments.append("-lib-name");
    //    arguments.append("platform");
    arguments.append(vlnv.getLibrary());
    arguments.append("-module-name");
//    arguments.append(pri.getName());
    arguments.append(vlnv.getName());
    arguments.append("-version");
//    arguments.append(pri.getVersion());
    arguments.append(vlnv.getVersion());

    process->setWorkingDirectory(workingDirectory);

    process->start(program, arguments);

    if (process->waitForFinished(-1)) {
        bRet = true;
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;

        emit inputMessage("create ip " + vlnv.getVendor() + " " + vlnv.getLibrary() + " " + vlnv.getName() + " " + vlnv.getVersion());
    }
    else {
        bRet = false;
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error output:" << error;
    }

    delete process;

    QDir::setCurrent(currentPath);

    if (bRet)
    {
        onLibrarySearch();

        // Open the design.
        onOpenDesign_Project(vlnv, viewNames.first());

        unlockNewlyCreatedDocument(vlnv);

        emit noticeMessage("[ SYSTEM ] | Create | IP has been created.");
    }

    else
    {
        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }

#endif // #if DESIGN_SCRIPT_ENABLE_20240125 == 0
}

//-----------------------------------------------------------------------------
// Function: mainwindow::createBus()
//-----------------------------------------------------------------------------
void MainWindow::createBus(VLNV const& vlnv, QString const& directory)
{
    Q_ASSERT(vlnv.isValid());
    Q_ASSERT(!directory.isEmpty());

    // create the vlnvs that identify the bus definition and abstraction definition
    VLNV busVLNV = vlnv;
    busVLNV.setType(VLNV::BUSDEFINITION);

    Q_ASSERT(!libraryHandler_->contains(busVLNV));

    VLNV absVLNV = busVLNV;

    // remove the possible .busDef from the end of the name field
    QString absDefName = busVLNV.getName();
    absDefName = absDefName.remove(".busDef", Qt::CaseInsensitive);

    absVLNV.setName(absDefName + ".absDef");
    absVLNV.setType(VLNV::ABSTRACTIONDEFINITION);

    // by default the abs def and bus def are saved to same directory
    QString absDirectory = directory;

    if (libraryHandler_->contains(absVLNV))
    {
        VLNV newAbsDefVLNV;

        if (!NewObjectDialog::saveAsDialog(this, libraryHandler_, absVLNV, newAbsDefVLNV, absDirectory,
            "Set VLNV for abstraction definition"))
        {
            // if user canceled
            return;
        }
        // save the created abstraction definition vlnv
        absVLNV = newAbsDefVLNV;
    }

    // Create a bus definition.
    QSharedPointer<BusDefinition> busDef = QSharedPointer<BusDefinition>(new BusDefinition());
    busDef->setVlnv(busVLNV);
    busDef->setVersion(VersionHelper::versionFileStr());

    // Create the file for the bus definition.
    bool success = true;

    if (!libraryHandler_->writeModelToFile(directory, busDef))
    {
        success = false;
    }

    // create an abstraction definition
    QSharedPointer<AbstractionDefinition> absDef = QSharedPointer<AbstractionDefinition>(new AbstractionDefinition());
    absDef->setVlnv(absVLNV);
    absDef->setVersion(VersionHelper::versionFileStr());

    // set reference from abstraction definition to bus definition
    absDef->setBusType(busVLNV);

    // create the file for the abstraction definition
    if (!libraryHandler_->writeModelToFile(absDirectory, absDef))
    {
        success = false;
    }

    if (success)
    {
        emit inputMessage("create io " + vlnv.getVendor() + " " + vlnv.getLibrary() + " " + vlnv.getName() + " " + vlnv.getVersion());

        // Open the bus editor.
        openBus(busVLNV);

        unlockNewlyCreatedDocument(busVLNV);

        emit noticeMessage("[ SYSTEM ] | Create | IP has been created.");
    }
    else
    {
        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }
}


bool MainWindow::OutputLogNoticeMessage(QByteArray result)
{
    //---------- Output Log --------
    QString outputStr = QString::fromUtf8(result);
    QStringList stringList = outputStr.split("\n");
    QList<QString> outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
        emit noticeMessage(line);
    }

    return true;
}

bool MainWindow::OutputLogErrorMessage(QByteArray result)
{
    //---------- Output Log --------
    QString outputStr = QString::fromUtf8(result);
    QStringList stringList = outputStr.split("\n");
    QList<QString> outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
        emit noticeMessage(line);
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: mainwindow::runProjectScript()
//-----------------------------------------------------------------------------
bool MainWindow::runProjectScript(Project const& pri, QString const& library, QString const& projectPath)
{
    QDir projectDirectory(projectPath);
    QString projectName = projectDirectory.dirName(); // 디렉토리명

    QString currentPath = QDir::currentPath();

    QString workingDirectory = projectPath;
    QString scriptDirectory = projectPath + "/script";

    QDir::setCurrent(workingDirectory);

    QString test = QDir::currentPath();

    bool bRet = false;

    QStringList arguments;

    QProcess *process;

    process = new QProcess(this);

    process->start("/bin/csh", QStringList() << "-c" << "source setprj" << projectPath);

    if (process->waitForFinished(-1)) {
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;
    }

    else {
        QByteArray result = process->readAllStandardError();
        qDebug() << "Error output:" << result;
    }

    QString program;
    program = workingDirectory + "/bin/awsh";

    arguments.append("python2");
    arguments.append(scriptDirectory + "/hw_design_gen_by_tony/script_gen.py");
//    arguments.append("-project-name");
//    arguments.append(projectName);
    arguments.append("-vendor");
    arguments.append(pri.getVendor());
    arguments.append("-lib-name");
    arguments.append(library);
    arguments.append("-module-name");
    arguments.append(library + "_for_" + pri.getVendor());
    arguments.append("-version");
    arguments.append(pri.getVersion());

    process->setWorkingDirectory(workingDirectory);

    process->start(program, arguments);

    if (process->waitForFinished(-1)) {
        bRet = true;

        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;

        emit inputMessage("create design " + pri.getVendor() + " " + library + " " + pri.getName() + " " + pri.getVersion());

        emit noticeMessage("[ SYSTEM ] | Create | Design has been created.");
    }

    else {
        bRet = false;
        QByteArray result = process->readAllStandardError();
        qDebug() << "Error output:" << result;

        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }

    delete process;

    QDir::setCurrent(currentPath);

    return bRet;
}

//-----------------------------------------------------------------------------
// Function: mainwindow::runDesignScript()
//-----------------------------------------------------------------------------
bool MainWindow::runDesignScript(VLNV const& vlnv)
{
    // Create the files.
    QString currentPath = QDir::currentPath();

    QString workingDirectory = SettingManager::getDefaultProjectLocation();
    QString scriptDirectory = workingDirectory + "/script";

    QDir::setCurrent(workingDirectory);

    QStringList arguments;

    QProcess *process;

    process = new QProcess(this);

     process->start("/bin/csh", QStringList() << "-c" << "source setprj");

    if (process->waitForFinished(-1)) {
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;
    }

    else {
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error output:" << error;
    }

    bool bRet = false;
    QString program;
    program = workingDirectory + "/bin/awsh";

    arguments.append("python2");
    arguments.append(scriptDirectory + "/hw_design_gen_by_tony/script_gen.py");
    arguments.append("-vendor");
    arguments.append(vlnv.getVendor());
    arguments.append("-lib-name");
    arguments.append(vlnv.getLibrary());
    arguments.append("-module-name");
    arguments.append(vlnv.getName());
    arguments.append("-version");
    arguments.append(vlnv.getVersion());

    process->setWorkingDirectory(workingDirectory);

    process->start(program, arguments);

    if (process->waitForFinished(-1)) {
        bRet = true;
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;

        OutputLogNoticeMessage(result);

        emit inputMessage("create design " + vlnv.getVendor() + " " + vlnv.getLibrary() + " " + vlnv.getName() + " " + vlnv.getVersion());

        emit noticeMessage("[ SYSTEM ] | Create | Design has been created.");
    }

    else {
        bRet = false;
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error output:" << error;

        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }

    delete process;

    QDir::setCurrent(currentPath);

    return bRet;
}

//-----------------------------------------------------------------------------
// Function: mainwindow::runIOScript()
//-----------------------------------------------------------------------------
bool MainWindow::runIOScript(VLNV const& vlnv)
{
//     python2 generate_io_for_gui.py -vendor test_1 -lib-name system -module-name gotit -version 1.0

    // Create the files.
    QString currentPath = QDir::currentPath();

    // 현제 프로젝토 위치가 최상위 위치...
//    QString workingDirectory = SettingManager::getScriptLocation();
//    QString scriptDirectory = workingDirectory + "/io_generator_for_gui";
    QString workingDirectory = SettingManager::getRootLocation();

    QString scriptDirectory = workingDirectory + "/sources/script/io_generator_for_gui";

    if (!QFileInfo(workingDirectory).isAbsolute())
    {
        workingDirectory = QFileInfo(workingDirectory).absoluteFilePath();
    }

    QDir::setCurrent(workingDirectory);

    QStringList arguments;

    QProcess *process;

    process = new QProcess(this);

    bool bRet = false;
    QString program = "python2";

    arguments.append(scriptDirectory + "/generate_io_for_gui.py");
    arguments.append("-vendor");
    arguments.append(vlnv.getVendor());
    arguments.append("-lib-name");
    arguments.append(vlnv.getLibrary());
    arguments.append("-module-name");
    arguments.append(vlnv.getName());
    arguments.append("-version");
    arguments.append(vlnv.getVersion());

    process->setWorkingDirectory(workingDirectory);

    process->start(program, arguments);

    if (process->waitForFinished(-1)) {
        bRet = true;
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;

        emit inputMessage("create io " + vlnv.getVendor() + " " + vlnv.getLibrary() + " " + vlnv.getName() + " " + vlnv.getVersion());

        emit noticeMessage("[ SYSTEM ] | Create | I/O has been created.");
    }

    else {
        bRet = false;
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error output:" << error;

        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }

    delete process;

    QDir::setCurrent(currentPath);

    return bRet;
}

//-----------------------------------------------------------------------------
// Function: mainwindow::runIPScript()
//-----------------------------------------------------------------------------
bool MainWindow::runIPScript(VLNV const& vlnv)
{
//    python2 generate_ip_for_gui.py -vendor test_1 -lib-name system -module-name gotit -version 1.0

    // Create the files.
    QString currentPath = QDir::currentPath();

    // 현제 프로젝토 위치가 최상위 위치...
//    QString workingDirectory = SettingManager::getScriptLocation();

//    QString scriptDirectory = workingDirectory + "/ip_generator_for_gui";

    QString workingDirectory = SettingManager::getRootLocation();

    QString scriptDirectory = workingDirectory + "/sources/script/ip_generator_for_gui";

    if (!QFileInfo(workingDirectory).isAbsolute())
    {
        workingDirectory = QFileInfo(workingDirectory).absoluteFilePath();
    }

    QDir::setCurrent(workingDirectory);

    QStringList arguments;

    QProcess *process;

    process = new QProcess(this);

    bool bRet = false;
    QString program = "python2";

    arguments.append(scriptDirectory + "/generate_ip_for_gui.py");
    arguments.append("-vendor");
    arguments.append(vlnv.getVendor());
    arguments.append("-lib-name");
    arguments.append(vlnv.getLibrary());
    arguments.append("-module-name");
    arguments.append(vlnv.getName());
    arguments.append("-version");
    arguments.append(vlnv.getVersion());

    process->setWorkingDirectory(workingDirectory);

    process->start(program, arguments);

    if (process->waitForFinished(-1)) {
        bRet = true;
        QByteArray result = process->readAllStandardOutput();
        qDebug() << "Command output:" << result;

        emit inputMessage("create ip " + vlnv.getVendor() + " " + vlnv.getLibrary() + " " + vlnv.getName() + " " + vlnv.getVersion());

        emit noticeMessage("[ SYSTEM ] | Create | IP has been created.");
    }

    else {
        bRet = false;
        QByteArray error = process->readAllStandardError();
        qDebug() << "Error output:" << error;

        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }

    delete process;

    QDir::setCurrent(currentPath);

    return bRet;
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onCreateProject()
//-----------------------------------------------------------------------------
void MainWindow::onCreateProject(Project const& project, QString const& projectLocation)
{
    Q_ASSERT(project.isValid());

    bool bRet = false;

    Project pri = project;

//    QString projectLocation = SettingManager::getDefaultProjectLocation();

//    if (!QFileInfo(projectLocation).isAbsolute())
//    {
//        projectLocation = QFileInfo(projectLocation).absoluteFilePath();
//    }

    // 2. core 에서 프로젝트로 주요 파일 복사
    QString coreDirectory = QSettings().value(QStringLiteral("ROOT/Core")).toString();

    QString sourcePath = coreDirectory;
    QString destinationPath = projectLocation;

    bRet = Utility::copyFile(this, sourcePath, destinationPath);

    if (bRet) {
        // 프로젝트 경로 설정
        // DefaultLocation 에 해당 프로젝트를 변경 해준다.
        QSettings().setValue("Project/DefaultLocation", projectLocation);

        emit inputMessage("create project " + project.getName() + " " + project.getVendor() + " " + project.getVersion());

        emit noticeMessage("[ SYSTEM ] | Create | Project has been created.");
    }

    else {
        emit errorMessage("[ ERROR ] | Create | Something is wrong.");
    }

    if (bRet) {
        // 1. conf 파일 생성
        QDate currentDate = QDate::currentDate();

        QString iniFilePath = projectLocation + "/awm.conf";

        QSettings settings(iniFilePath, QSettings::IniFormat);

        settings.setValue("apr/valid", true);
        settings.setValue("Project/name", pri.getName());
        settings.setValue("version/release", pri.getVersion());
        settings.setValue("version/date", currentDate.toString("yyyyMMdd"));
    }

    if (bRet) {
        bRet = runProjectScript(project, "platform", projectLocation);
    }

    if (bRet) {
        bRet = runProjectScript(project, "chip", projectLocation);
    }

    if (bRet) {
        bRet = runProjectScript(project, "system", projectLocation);
    }

    if (bRet) {
        onLibrarySearch();
    }
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onCreateBus()
//-----------------------------------------------------------------------------
void MainWindow::onCreateBus(VLNV const& vlnv/*, QString const& directory*/)
{
    Q_ASSERT(vlnv.isValid());
//    Q_ASSERT(!directory.isEmpty());

    if (!runIOScript(vlnv)) {
        return;
    }

    else {
        onLibrarySearch();
    }
}

//-----------------------------------------------------------------------------
// Function: mainwindow::onCreateHWComponent()
//-----------------------------------------------------------------------------
void MainWindow::onCreateHWComponent(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
    QVector<TagData> tags, VLNV const& vlnv, QString const& directory)
{
    Q_ASSERT(vlnv.isValid());

    // Create a component.
    QSharedPointer<Component> component = QSharedPointer<Component>(new Component(vlnv));

    // Set Kactus attributes.
    component->setHierarchy(prodHier);
    component->setFirmness(firmness);
    component->setTags(tags);
    component->setImplementation(KactusAttribute::HW);

    component->setVersion(VersionHelper::versionFileStr());

    // Create the file.
//    if (!libraryHandler_->writeModelToFile(directory, component))
//    {
//        emit errorMessage("Error saving file to disk.");
//        return;
//    }

    if (!runIPScript(vlnv)) {
        return;
    }

    onLibrarySearch();

    runComponentWizard(component, directory);

    onLibrarySearch();

    // Open the component editor.
//    openComponent(vlnv);

//    unlockNewlyCreatedDocument(vlnv);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::createDesign()
//-----------------------------------------------------------------------------
void MainWindow::onCreateDesign(VLNV const& vlnv)
{
    Q_ASSERT(vlnv.isValid());

    if (runDesignScript(vlnv))
    {
        onLibrarySearch();
    }
}

//-----------------------------------------------------------------------------
// Function: MainWindow::createDesignForExistingComponent()
//-----------------------------------------------------------------------------
void MainWindow::createDesignForExistingComponent(VLNV const& vlnv)
{
    Q_ASSERT(projectHandler_->contains(vlnv));
    Q_ASSERT(projectHandler_->getDocumentType(vlnv) == VLNV::COMPONENT);

    // Retrieve the component to which the design will be created.
    QSharedPointer<Document> libComp = projectHandler_->getModel(vlnv);
    QSharedPointer<Component> component = libComp.staticCast<Component>();

    // Ask the user the VLNV, target path and view name.
    NewDesignDialog dialog(projectHandler_, component, KactusAttribute::HW, this);
    dialog.setVLNV(VLNV(VLNV::DESIGN, component->getVlnv().getVendor(), component->getVlnv().getLibrary(), "", ""));

    QSettings settings;
    QStringList suggestions = settings.value("Policies/HWViewNames").toStringList();
    dialog.setViewNameSuggestions(suggestions);
    component->setVersion(VersionHelper::versionFileStr());

    QString baseViewName = "";
    QString viewName = "";

    if (!suggestions.isEmpty())
    {
        baseViewName = suggestions.first();

        viewName = baseViewName;
        unsigned int runningNumber = 1;

        for (QSharedPointer<View> view : *component->getViews())
        {
            if (view->name() == viewName)
            {
                ++runningNumber;
                viewName = baseViewName + QString::number(runningNumber);
            }
        }
    }

    dialog.setViewName(viewName);

    if (dialog.exec() == QDialog::Rejected)
    {
        return;
    }

    // Create the view.
    QSharedPointer<View> view (new View(dialog.getViewName()));

    VLNV designConfigVLNV = dialog.getDesignConfVLNV();
    QSharedPointer<DesignConfigurationInstantiation> hierarchyInstantiation
        (new DesignConfigurationInstantiation(NameGenerationPolicy::designConfigurationInstantiationName(dialog.getQualifierName())));
    hierarchyInstantiation->setDesignConfigurationReference(
        QSharedPointer<ConfigurableVLNVReference>( new ConfigurableVLNVReference( designConfigVLNV ) ) );

    view->setDesignConfigurationInstantiationRef(hierarchyInstantiation->name());

    component->getViews()->append(view);
    component->getDesignConfigurationInstantiations()->append(hierarchyInstantiation);

    // Create the design and design configuration objects.
    QSharedPointer<DesignConfiguration> designConf(new DesignConfiguration(dialog.getDesignConfVLNV()));
    designConf->setDesignRef(dialog.getDesignVLNV());
    designConf->setDesignConfigImplementation(KactusAttribute::HW);
    designConf->setVersion(VersionHelper::versionFileStr());

    QSharedPointer<Design> newDesign = QSharedPointer<Design>(new Design(dialog.getDesignVLNV()));
    newDesign->setDesignImplementation(KactusAttribute::HW);
    newDesign->setVersion(VersionHelper::versionFileStr());

    newDesign->addColumn(QSharedPointer<ColumnDesc>(new ColumnDesc(
        "IO", ColumnTypes::IO, 0, GraphicsColumnConstants::IO_COLUMN_WIDTH)));
    newDesign->addColumn(QSharedPointer<ColumnDesc>(new ColumnDesc(
        "Components", ColumnTypes::COMPONENTS, 0, GraphicsColumnConstants::COMPONENT_COLUMN_WIDTH)));
    newDesign->addColumn(QSharedPointer<ColumnDesc>(new ColumnDesc(
        "Components", ColumnTypes::COMPONENTS, 0, GraphicsColumnConstants::COMPONENT_COLUMN_WIDTH)));
    newDesign->addColumn(QSharedPointer<ColumnDesc>(new ColumnDesc(
        "Components", ColumnTypes::COMPONENTS, 0, GraphicsColumnConstants::COMPONENT_COLUMN_WIDTH)));

    projectHandler_->beginSave();

    bool success = true;

    if (!projectHandler_->writeModelToFile(dialog.getPath(), newDesign))
    {
        success = false;
    }

    if (!projectHandler_->writeModelToFile(dialog.getPath(), designConf))
    {
        success = false;
    }

    if (!projectHandler_->writeModelToFile(component))
    {
        success = false;
    }

    projectHandler_->endSave();

    if (success)
    {
        // Open the design.
        onOpenDesign_Project(vlnv, view->name());

        unlockNewlyCreatedDocument(vlnv);

    }
    else
    {
        emit errorMessage("[ ERROR ] | Create | Error saving files to disk.");
    }
}

//-----------------------------------------------------------------------------
// Function: MainWindow::unlockNewlyCreatedDocument()
//-----------------------------------------------------------------------------
void MainWindow::unlockNewlyCreatedDocument(VLNV const& vlnv)
{
    TabDocument* document = dynamic_cast<TabDocument*>(designTabs_->currentWidget());
    if (document && document->getIdentifyingVLNV() == vlnv)
    {
        document->setProtection(false);
    }
}

void MainWindow::saveCurrentDocument()
{
    designTabs_->saveCurrentDocument();
}

void MainWindow::saveCurrentDocumentAs()
{
    designTabs_->saveCurrentDocumentAs();
}

void MainWindow::saveAll()
{
    designTabs_->saveAll();
}

void MainWindow::onImportProject()
{
    dockHandler_->onImportProject();
}

void MainWindow::onExportProject()
{
    dockHandler_->onExportProject();
}

#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1

#if PLUGIN_TOOLS_SUPPORT_20240905 == 1
//-----------------------------------------------------------------------------
// Function: MainWindow::runGeneratorPlugin()
//-----------------------------------------------------------------------------
void MainWindow::runGeneratorPlugin(QAction* action)
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

    // Inform user that unsaved changes must be saved before continuing.
    if (doc->isModified())
    {
        QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
            "The document " + doc->getDocumentName() + " has unsaved changes and needs to be "
            "saved before generators can be run. Save and continue?",
            QMessageBox::Yes | QMessageBox::No, this);

        if (msgBox.exec() == QMessageBox::No || !doc->save())
        {
            return;
        }
    }

    // Retrieve the library component.
    VLNV compVLNV = doc->getDocumentVLNV();
    VLNV desVLNV = doc->getIdentifyingVLNV();
    QString viewName;

    // if the design is supported by the document type
    DesignWidget* desWidget = qobject_cast<DesignWidget*>(doc);
    if (desWidget)
    {
        // the vlnvs must be for different objects
        Q_ASSERT(compVLNV != desVLNV);

        // find the design config is one exists
        viewName = desWidget->getOpenViewName();
    }

    // Retrieve the plugin pointer from the action.
    IGeneratorPlugin* plugin = reinterpret_cast<IGeneratorPlugin*>(action->data().value<void*>());
    Q_ASSERT(plugin != 0);

    KactusAPI::runGenerator(plugin, compVLNV, viewName, QString(), this);

    // Refresh the document.
    doc->refresh();
}
#else

//-----------------------------------------------------------------------------
// Function: MainWindow::runGenerator()
//-----------------------------------------------------------------------------
void MainWindow::runGenerator()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

    // Inform user that unsaved changes must be saved before continuing.
    if (doc->isModified())
    {
        QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
            "The document " + doc->getDocumentName() + " has unsaved changes and needs to be "
            "saved before generators can be run. Save and continue?",
            QMessageBox::Yes | QMessageBox::No, this);

        if (msgBox.exec() == QMessageBox::No || !doc->save())
        {
            return;
        }
    }

    // Retrieve the library component.
    VLNV compVLNV = doc->getDocumentVLNV();
    VLNV desVLNV = doc->getIdentifyingVLNV();
    QString viewName;

    // if the design is supported by the document type
    DesignWidget* desWidget = qobject_cast<DesignWidget*>(doc);
    if (desWidget)
    {
        // the vlnvs must be for different objects
        Q_ASSERT(compVLNV != desVLNV);

        // find the design config is one exists
        viewName = desWidget->getOpenViewName();
    }

    // Retrieve the plugin pointer from the action.

#if PLUGIN_TOOLS_SUPPORT_20240905 == 1
    IGeneratorPlugin* plugin = reinterpret_cast<IGeneratorPlugin*>(action->data().value<void*>());
    Q_ASSERT(plugin != 0);

    KactusAPI::runGenerator(plugin, compVLNV, viewName, QString(), this);
#else
    VerilogGenerator * verilogGenerator = new VerilogGenerator(projectHandler_, projectLibraryHandler_, messageChannel_, VersionHelper::createVersionString(), this);

    verilogGenerator->runGenerator(compVLNV, viewName);
#endif // PLUGIN_TOOLS_SUPPORT_20240905

    // Refresh the document.
    doc->refresh();
}
#endif // PLUGIN_TOOLS_SUPPORT_20240905

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

//-----------------------------------------------------------------------------
// Function: onOpenSettings()
//-----------------------------------------------------------------------------
void MainWindow::onOpenSettings()
{

    SettingsDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        designTabs_->applySettings();
//        updateGeneratorPluginActions();
//        dockHandler_->applySettings();
//        scriptEditor_->applySettings();
    }
}

//-----------------------------------------------------------------------------
// Function: onShowAbout()
//-----------------------------------------------------------------------------
void MainWindow::onShowAbout()
{
    SplashScreen* splash = new SplashScreen(VersionHelper::createVersionString());
    splash->setAttribute(Qt::WA_DeleteOnClose);
    splash->setWindowFlags(splash->windowFlags() & ~(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint));
    splash->move(this->mapToGlobal(this->rect().center() - splash->rect().center()));
    splash->setWindowModality(Qt::ApplicationModal);
    splash->show();
    splash->showMessage("");
}

#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1

//-----------------------------------------------------------------------------
// Function: onDrawModeChanged()
//-----------------------------------------------------------------------------
void MainWindow::onDrawModeChanged(DrawMode mode)
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

    if (doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT))
    {
        if (mode == MODE_SELECT)
        {
            doc->setCursor(Qt::ArrowCursor);
        }
        else
        {
            doc->setCursor(Qt::CrossCursor);
        }
    }

    actToolSelect_->setChecked(mode == MODE_SELECT);
    actToolConnect_->setChecked(mode == MODE_CONNECT);
    actToolInterface_->setChecked(mode == MODE_INTERFACE);
    actToolDraft_->setChecked(mode == MODE_DRAFT);
    actToolToggleOffPage_->setChecked(mode == MODE_TOGGLE_OFFPAGE);
    actToolLabel_->setChecked(mode == MODE_LABEL);
}



//-----------------------------------------------------------------------------
// Function: MainWindow::onOpenSchematic()
//-----------------------------------------------------------------------------
void MainWindow::onOpenSchematic(VLNV const& vlnv)
{
    onOpenDesign_Console(vlnv, "hierarchical");
}



//-----------------------------------------------------------------------------
// Function: MainWindow::onRefreshSchematic()
//-----------------------------------------------------------------------------
void MainWindow::onRefreshSchematic(VLNV const& vlnv)
{
    onRefreshDesign_Console(vlnv, "hierarchical");
}



//-----------------------------------------------------------------------------
// Function: MainWindow::onCloseSchematic()
//-----------------------------------------------------------------------------
void MainWindow::onCloseSchematic(VLNV const& vlnv)
{
    onCloseDesign_Console(vlnv, "hierarchical");
}



//-----------------------------------------------------------------------------
// Function: MainWindow::onCloseSchematic()
//-----------------------------------------------------------------------------
void MainWindow::onCloseAllSchematic()
{
    onCloseAllDesign_Console();
}



//-----------------------------------------------------------------------------
// Function: MainWindow::onOpenFile()
//-----------------------------------------------------------------------------
void MainWindow::onOpenFile(QString filePath)
{
    onOpenFile_Console(filePath);
}



//-----------------------------------------------------------------------------
// Function: drawModeChange()
//-----------------------------------------------------------------------------
void MainWindow::drawModeChange(QAction *action)
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

    if (action == actToolSelect_)
    {
        doc->setMode(MODE_SELECT);
    }
    else if (action == actToolConnect_)
    {
        doc->setMode(MODE_CONNECT);
    }
    else if (action == actToolInterface_)
    {
        doc->setMode(MODE_INTERFACE);
    }
    else if (action == actToolDraft_)
    {
        doc->setMode(MODE_DRAFT);
    }
    else if (action == actToolToggleOffPage_)
    {
        doc->setMode(MODE_TOGGLE_OFFPAGE);
    }
    else if (action == actToolLabel_)
    {
        doc->setMode(MODE_LABEL);
    }
}


#endif

#if PROTECTION_TOOLS_SUPPORT_20240104 == 1

//-----------------------------------------------------------------------------
// Function: onProtectionChanged()
//-----------------------------------------------------------------------------
void MainWindow::onProtectionChanged(bool locked)
{
    if (locked)
    {
        actProtect_->setIcon(QIcon(":resources/Res/protection-locked.png"));
        actProtect_->setText(tr("Locked (Ctrl+L)"));
    }
    else
    {
        actProtect_->setIcon(QIcon(":resources/Res/protection-unlocked.png"));
        actProtect_->setText(tr("Unlocked (Ctrl+L)"));
    }
}

//-----------------------------------------------------------------------------
// Function: changeProtection()
//-----------------------------------------------------------------------------
void MainWindow::changeProtection(bool locked)
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

    // if there is no currently selected tab
    if (!doc)
    {
        return;
    }

    VLNV docVLNV = doc->getDocumentVLNV();
    Q_ASSERT(docVLNV.isValid());

    // if user set the document to be locked and it has been modified
    if (locked)
    {
        if (doc->isModified())
        {
            // ask user if he wants to save the changes made to document
            QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
                tr("The document has been modified. The changes need to be saved before the "
                "document can be locked. Save changes and continue?"), QMessageBox::Yes | QMessageBox::No, this);

            // if user does not want to save or save can't be done
            if (msgBox.exec() == QMessageBox::No || !doc->save())
            {
                actProtect_->setChecked(false);
                return;
            }
        }
        // lock the document
        doc->setProtection(locked);
    }

    //If user wants to unlock the document.
    else
    {
        // The list containing tabs for the same document.
        QVector<TabDocument*> otherDocs;

        // If the same document has been edited in another editor.
        TabDocument* docToSave = 0;

        for (int i = 0; i < designTabs_->count(); i++)
        {
            TabDocument* otherDoc = static_cast<TabDocument*>(designTabs_->widget(i));
            Q_ASSERT(otherDoc);

            // if the other document is for the same component
            if (otherDoc != doc && otherDoc->getDocumentVLNV() == docVLNV)
            {
                otherDocs.append(otherDoc);

                // If there is a document that is not locked and has been modified.
                if (!otherDoc->isProtected() && otherDoc->isModified())
                {
                    // there should only be one doc at a time that can be modified
                    Q_ASSERT(!docToSave);

                    // this was the document that was modified
                    docToSave = otherDoc;
                }
            }
        }

        // if there was document that was unlocked and modified
        if (docToSave)
        {
            // Ask the user if he wants to save and switch locks.
            QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
                tr("The document is being edited in another tab and has unsaved changes. Changes need to be saved "
                   "before this tab can be unlocked. Save changes and switch locks?"),
                QMessageBox::Yes | QMessageBox::No, this);

            // Restore the lock if the user canceled.
            if (msgBox.exec() == QMessageBox::No)
            {
                actProtect_->setChecked(true);
                return;
            }

            // if the document could not be saved
            if (!docToSave->save())
            {
                return;
            }
        }

        bool previouslyUnlocked = doc->isPreviouslyUnlocked();

        // User wanted to unlock this doc so go through other documents to see if they were unlocked.
        for (TabDocument* otherDoc : otherDocs)
        {
            otherDoc->setProtection(true);

            if (otherDoc->isPreviouslyUnlocked())
            {
                previouslyUnlocked = true;
            }
        }

        if (!previouslyUnlocked)
        {
            QString message = tr("Are you sure you want to unlock the document?\n");
            QString detailMsg = "";

            // If edited document was component and has been instantiated in other components, print them.
            if (docVLNV.isValid() && libraryHandler_->getDocumentType(docVLNV) == VLNV::COMPONENT)
            {
                QList<VLNV> list;
                int refCount = libraryHandler_->getOwners(list, docVLNV);
                if (refCount > 0)
                {
                    message += tr("Changes to the document can affect %1 other documents.").arg(refCount);

                    detailMsg += tr("The component has been instantiated in the following %1 component(s):\n").arg(
                        refCount);
                    for (VLNV const& owner : list)
                    {
                        detailMsg += "* " + owner.toString() + "\n";
                    }
                }
                else
                {
                    message += tr("Changes to the document can affect other documents. ");
                }
            }
            else
            {
                message += tr("Changes to the document can affect other documents. ");
            }

            message += "If you choose yes, this will not be asked next time for this document.";

            // Ask verification from the user.
            QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(), message,
                QMessageBox::Yes | QMessageBox::No, this);

            msgBox.setDetailedText(detailMsg);

            // if user did not want to unlock the document
            if (msgBox.exec() == QMessageBox::No)
            {
                actProtect_->setChecked(true);
                return;
            }
        }

        // mark the other documents also unlocked to keep the unlock history in sync.
        for (TabDocument* otherDoc : otherDocs)
        {
            otherDoc->setPreviouslyUnlocked();
        }

        // Unlock the document.
        doc->setProtection(locked);
    }

    dockHandler_->changeProtection(doc, locked);

    // Clear the item selection since the current instance is no longer valid.
    onClearItemSelection();

    onProtectionChanged(locked);

    updateMenuStrip();
}

#endif
//-----------------------------------------------------------------------------
// Function: updateMenuStrip()
//-----------------------------------------------------------------------------
void MainWindow::updateMenuStrip()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());
//    bool unlocked = doc != 0 && (!(doc->getFlags() & TabDocument::DOC_PROTECTION_SUPPORT) || !doc->isProtected());

//    bool isHWComp = false;
//    ComponentEditor* componentEditor = dynamic_cast<ComponentEditor*>(doc);
//    if (componentEditor)
//    {
//        isHWComp = componentEditor->isHWImplementation();
//    }

//    bool isHWDesign = dynamic_cast<HWDesignWidget*>(doc) != 0;
//    bool isSystemDesign = dynamic_cast<SystemDesignWidget*>(doc) != 0;

//    MemoryDesignDocument* memoryDocument = dynamic_cast<MemoryDesignDocument*>(doc);
//    bool isMemoryDesign = memoryDocument != 0;

//    actSave_->setEnabled(doc != 0 && doc->isModified());
//    actSaveAs_->setEnabled(doc != 0);
//    actSaveHierarchy_->setEnabled(componentEditor || dynamic_cast<DesignWidget*>(doc));
//    actPrint_->setEnabled(doc != 0 && (doc->getFlags() & TabDocument::DOC_PRINT_SUPPORT));
//    actImage`_->setEnabled(doc != 0 && doc->getFlags() & TabDocument::DOC_PRINT_SUPPORT);

//    generationToolsAction_->setEnabled(unlocked);
//    generationToolsAction_->setVisible(doc != 0 && (componentEditor != 0 || isHWDesign || isSystemDesign));

//    actGenDocumentation_->setEnabled((isHWDesign|| isHWComp) && unlocked);
//    actGenDocumentation_->setVisible((isHWDesign|| isHWComp));

//    actRunImport_->setEnabled(isHWComp && unlocked);
//    actRunImport_->setVisible(isHWComp);

//    configurationToolsAction_->setEnabled(unlocked);
//    configurationToolsAction_->setVisible(doc != 0 && (isHWComp || isHWDesign));
//    actionConfigureViews_->setEnabled(unlocked);
//    actionConfigureViews_->setVisible(isHWDesign || isHWComp);

//    editAction_->setVisible(doc != 0);
//    editAction_->setEnabled(doc != 0 && unlocked);
//    actUndo_->setVisible(doc != 0 && doc->getEditProvider() != 0);
//    actRedo_->setVisible(doc != 0 && doc->getEditProvider() != 0);
//    actUndo_->setEnabled(doc != 0 && doc->getEditProvider() != 0 && doc->getEditProvider()->canUndo());
//    actRedo_->setEnabled(doc != 0 && doc->getEditProvider() != 0 && doc->getEditProvider()->canRedo());

#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1
    diagramToolsAction_->setVisible(doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT));
    diagramToolsAction_->setEnabled(doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT) &&
        !doc->isProtected());
    actToolSelect_->setEnabled(doc != 0 && (doc->getSupportedDrawModes() & MODE_SELECT));
    actToolConnect_->setEnabled(doc != 0 && (doc->getSupportedDrawModes() & MODE_CONNECT));
    actToolInterface_->setEnabled(doc != 0 && (doc->getSupportedDrawModes() & MODE_INTERFACE));
    actToolDraft_->setEnabled(doc != 0 && (doc->getSupportedDrawModes() & MODE_DRAFT));
    actToolToggleOffPage_->setEnabled(doc != 0 && (doc->getSupportedDrawModes() & MODE_TOGGLE_OFFPAGE));
    actToolLabel_->setEnabled(doc != 0 && (doc->getSupportedDrawModes() & MODE_LABEL));
#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

//    bool oldProtectionState = actProtect_->isChecked();

//    protectAction_->setVisible(doc != 0 && (doc->getFlags() & TabDocument::DOC_PROTECTION_SUPPORT));
//    actProtect_->setEnabled(doc != 0 && (doc->getFlags() & TabDocument::DOC_PROTECTION_SUPPORT));
//    actProtect_->setChecked(doc != 0 && (doc->getFlags() & TabDocument::DOC_PROTECTION_SUPPORT) &&
//        doc->isProtected());

//    if (oldProtectionState != actProtect_->isChecked())
//        onProtectionChanged(actProtect_->isChecked());

//    actVisibilityControl_->setEnabled(doc != 0 && (doc->getFlags() & TabDocument::DOC_VISIBILITY_CONTROL_SUPPORT));
//    actVisibilityControl_->setVisible(doc != 0 && (doc->getFlags() & TabDocument::DOC_VISIBILITY_CONTROL_SUPPORT));

#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
    generationToolsAction_->setVisible(doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT));
    generationToolsAction_->setEnabled(doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT) &&
        !doc->isProtected());

    // Remove all plugin actions.
    QList<QAction*> actions = pluginActionGroup_->actions();

    for (QAction* action : actions)
    {
        action->setEnabled(doc != 0 && (doc->getFlags() & TabDocument::DOC_DRAW_MODE_SUPPORT) &&
            !doc->isProtected());
    }


//    updateGeneratorPluginActions();
//    setPluginVisibilities();

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618


    onUpdateZoomTools();

//    if (isMemoryDesign)
//    {
//        generationAction_->setVisible(false);

//        actionFilterAddressSpaceChains_->setChecked(memoryDocument->addressSpaceChainsAreFiltered());
//        actionCondenseMemoryItems_->setChecked(memoryDocument->memoryItemsAreCondensed());
//        actionFilterSegments_->setChecked(memoryDocument->addressSpaceSegmentsAreFilterted());
//        actionFilterAddressBlocks_->setChecked(memoryDocument->addressBlocksAreFiltered());
//        actionFilterRegisters_->setChecked(memoryDocument->addressBlockRegistersAreFiltered());
//        actionFilterFields_->setChecked(memoryDocument->fieldsAreFiltered());
//        actionFilterUnconnectedMemoryItems_->setChecked(memoryDocument->unconnectedMemoryItemsAreFiltered());
//    }

//    filteringAction_->setVisible(isMemoryDesign);
}

//-----------------------------------------------------------------------------
// Function: onUpdateZoomTools()
//-----------------------------------------------------------------------------
void MainWindow::onUpdateZoomTools()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());
    bool canZoom = doc != 0 && (doc->getFlags() & TabDocument::DOC_ZOOM_SUPPORT);

    viewToolsAction_->setVisible(canZoom&& !doc->isProtected());
    viewToolsAction_->setEnabled(canZoom);

    actZoomIn_->setVisible(canZoom);
    actZoomOut_->setVisible(canZoom);
    actZoomOriginal_->setVisible(canZoom);
    actFitInView_->setVisible(canZoom);

    actZoomIn_->setEnabled(canZoom && doc->getZoomLevel() < doc->getMaxZoomLevel());
    actZoomOut_->setEnabled(canZoom && doc->getZoomLevel() > doc->getMinZoomLevel());
    actZoomOriginal_->setEnabled(canZoom);
    actFitInView_->setEnabled(canZoom);
}

//-----------------------------------------------------------------------------
// Function: onZoomIn()
//-----------------------------------------------------------------------------
void MainWindow::onZoomIn()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());
    Q_ASSERT(doc->getFlags() & TabDocument::DOC_ZOOM_SUPPORT);
    doc->setZoomLevel(doc->getZoomLevel() + 10);
    onUpdateZoomTools();
}

//-----------------------------------------------------------------------------
// Function: onZoomOut()
//-----------------------------------------------------------------------------
void MainWindow::onZoomOut()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());
    Q_ASSERT(doc->getFlags() & TabDocument::DOC_ZOOM_SUPPORT);
    doc->setZoomLevel(doc->getZoomLevel() - 10);
    onUpdateZoomTools();
}

//-----------------------------------------------------------------------------
// Function: onZoomOriginal()
//-----------------------------------------------------------------------------
void MainWindow::onZoomOriginal()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());
    Q_ASSERT(doc->getFlags() & TabDocument::DOC_ZOOM_SUPPORT);
    doc->setZoomLevel(100);
    onUpdateZoomTools();
}

//-----------------------------------------------------------------------------
// Function: onFitInView()
//-----------------------------------------------------------------------------
void MainWindow::onFitInView()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());
    Q_ASSERT(doc->getFlags() & TabDocument::DOC_ZOOM_SUPPORT);
    doc->fitInView();
    onUpdateZoomTools();
}

//-----------------------------------------------------------------------------
// Function: onChangeDesignProtection()
//-----------------------------------------------------------------------------
void MainWindow::onChangeDesignProtection(bool const &locked)
{
//    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

//    // if there is no currently selected tab
//    if (!doc)
//    {
//        return;
//    }

//    VLNV docVLNV = doc->getDocumentVLNV();
//    Q_ASSERT(docVLNV.isValid());

//    // if user set the document to be locked and it has been modified
//    if (locked)
//    {
//        if (doc->isModified())
//        {
//            // ask user if he wants to save the changes made to document
//            QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
//                tr("The document has been modified. The changes need to be saved before the "
//                "document can be locked. Save changes and continue?"), QMessageBox::Yes | QMessageBox::No, this);

//            // if user does not want to save or save can't be done
//            if (msgBox.exec() == QMessageBox::No || !doc->save())
//            {
//                return;
//            }
//        }
//        // lock the document
//        doc->setProtection(locked);
//    }

//    //If user wants to unlock the document.
//    else
//    {
//        // The list containing tabs for the same document.
//        QVector<TabDocument*> otherDocs;

//        // If the same document has been edited in another editor.
//        TabDocument* docToSave = 0;

//        for (int i = 0; i < designTabs_->count(); i++)
//        {
//            TabDocument* otherDoc = static_cast<TabDocument*>(designTabs_->widget(i));
//            Q_ASSERT(otherDoc);

//            // if the other document is for the same component
//            if (otherDoc != doc && otherDoc->getDocumentVLNV() == docVLNV)
//            {
//                otherDocs.append(otherDoc);

//                // If there is a document that is not locked and has been modified.
//                if (!otherDoc->isProtected() && otherDoc->isModified())
//                {
//                    // there should only be one doc at a time that can be modified
//                    Q_ASSERT(!docToSave);

//                    // this was the document that was modified
//                    docToSave = otherDoc;
//                }
//            }
//        }

//        // if there was document that was unlocked and modified
//        if (docToSave)
//        {
//            // Ask the user if he wants to save and switch locks.
//            QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
//                tr("The document is being edited in another tab and has unsaved changes. Changes need to be saved "
//                   "before this tab can be unlocked. Save changes and switch locks?"),
//                QMessageBox::Yes | QMessageBox::No, this);

//            // Restore the lock if the user canceled.
//            if (msgBox.exec() == QMessageBox::No)
//            {
//                return;
//            }

//            // if the document could not be saved
//            if (!docToSave->save())
//            {
//                return;
//            }
//        }

//        bool previouslyUnlocked = doc->isPreviouslyUnlocked();

//        // User wanted to unlock this doc so go through other documents to see if they were unlocked.
//        for (TabDocument* otherDoc : otherDocs)
//        {
//            otherDoc->setProtection(true);

//            if (otherDoc->isPreviouslyUnlocked())
//            {
//                previouslyUnlocked = true;
//            }
//        }

//        if (!previouslyUnlocked)
//        {
//            QString message = tr("Are you sure you want to unlock the document?\n");
//            QString detailMsg = "";

//            // If edited document was component and has been instantiated in other components, print them.
//            if (docVLNV.isValid() && libraryHandler_->getDocumentType(docVLNV) == VLNV::COMPONENT)
//            {
//                QList<VLNV> list;
//                int refCount = libraryHandler_->getOwners(list, docVLNV);
//                if (refCount > 0)
//                {
//                    message += tr("Changes to the document can affect %1 other documents.").arg(refCount);

//                    detailMsg += tr("The component has been instantiated in the following %1 component(s):\n").arg(
//                        refCount);
//                    for (VLNV const& owner : list)
//                    {
//                        detailMsg += "* " + owner.toString() + "\n";
//                    }
//                }
//                else
//                {
//                    message += tr("Changes to the document can affect other documents. ");
//                }
//            }
//            else
//            {
//                message += tr("Changes to the document can affect other documents. ");
//            }

//            message += "If you choose yes, this will not be asked next time for this document.";

//            // Ask verification from the user.
//            QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(), message,
//                QMessageBox::Yes | QMessageBox::No, this);

//            msgBox.setDetailedText(detailMsg);

//            // if user did not want to unlock the document
//            if (msgBox.exec() == QMessageBox::No)
//            {
//                return;
//            }
//        }

//        // mark the other documents also unlocked to keep the unlock history in sync.
//        for (TabDocument* otherDoc : otherDocs)
//        {
//            otherDoc->setPreviouslyUnlocked();
//        }

//        // Unlock the document.
//        doc->setProtection(locked);
//    }

//    dockHandler_->changeProtection(doc, locked);

////    onProtectionChanged(locked);

}

//-----------------------------------------------------------------------------
// Function: MainWindow::setupDrawBoard()
//-----------------------------------------------------------------------------
void MainWindow::setupDrawBoard()
{
    designTabs_ = new DrawingBoard();
    designTabs_->setMovable(true);
    designTabs_->setTabsClosable(true);

    connect(designTabs_, SIGNAL(lastDocumentClosed()), this, SLOT(onLastDocumentClosed()), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(currentChanged(int)), this, SLOT(onDocumentChanged(int)), Qt::UniqueConnection);

    connect(designTabs_, SIGNAL(documentContentChanged()), this, SLOT(updateMenuStrip()), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(documentEditStateChanged()), this, SLOT(updateMenuStrip()));

//    connect(designTabs_, SIGNAL(helpUrlRequested(QString const&)),
//        this, SIGNAL(helpUrlRequested(QString const&)), Qt::UniqueConnection);

    connect(designTabs_, SIGNAL(inputMessage(QString const&)), dockHandler_, SIGNAL(inputMessage(QString const&)), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(errorMessage(QString const&)), dockHandler_, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(noticeMessage(QString const&)), dockHandler_, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(undefinedMessage(QString const&)), dockHandler_, SIGNAL(undefinedMessage(QString const&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: MainWindow::setupVisibilitySetting()
//-----------------------------------------------------------------------------
void MainWindow::setupVisibilitySetting()
{
    if (QSettings().value("Visible/Console").isNull())
    {
        QSettings().setValue("Visible/Console", true);
    }

    if (QSettings().value("Visible/Log").isNull())
    {
        QSettings().setValue("Visible/Log", true);
    }

    if (QSettings().value("Visible/Report").isNull())
    {
        QSettings().setValue("Visible/Report", true);
    }
}

//-----------------------------------------------------------------------------
// Function: mainwindow::setupAndConnectProjectHandler()
//-----------------------------------------------------------------------------
void MainWindow::setupAndConnectProjectHandler()
{
    connect(projectHandler_, SIGNAL(openDesign(const VLNV&, const QString&)),
        this, SLOT(onOpenDesign_Project(const VLNV&, const QString&)));

    connect(projectHandler_, SIGNAL(openComponent(const VLNV&)),
        this, SLOT(onOpenComponent_Project(const VLNV&)), Qt::UniqueConnection);

    connect(projectHandler_, SIGNAL(openBus(const VLNV&)),
        this, SLOT(onOpenBus_Project(const VLNV&)), Qt::UniqueConnection);

    connect(projectHandler_, SIGNAL(openAbsDef(const VLNV&)),
        this, SLOT(onOpenAbsDef_Project(const VLNV&)), Qt::UniqueConnection);


    connect(projectHandler_, SIGNAL(createNewDesign()),
        this, SLOT(onCreateNewDesign()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::setupAndConnectProjectLibraryHandler()
//-----------------------------------------------------------------------------
void MainWindow::setupAndConnectProjectLibraryHandler()
{
    connect(projectLibraryHandler_, SIGNAL(openComponent(const VLNV&)),
        this, SLOT(onOpenComponent_ProjectLibrary(const VLNV&)), Qt::UniqueConnection);

    connect(projectLibraryHandler_, SIGNAL(openBus(const VLNV&, const VLNV&, bool)),
        this, SLOT(onOpenBus_projectLibrary(const VLNV&, const VLNV&, bool)), Qt::UniqueConnection);

    connect(projectLibraryHandler_, SIGNAL(openBus(const VLNV&)),
        this, SLOT(onOpenBus_projectLibrary(const VLNV&)), Qt::UniqueConnection);

    connect(projectLibraryHandler_, SIGNAL(openAbsDef(const VLNV&, const VLNV&, bool)),
        this, SLOT(onOpenAbsDef_projectLibrary(const VLNV&, const VLNV&, bool)), Qt::UniqueConnection);

    connect(projectLibraryHandler_, SIGNAL(openAbsDef(const VLNV&)),
        this, SLOT(onOpenAbsDef_projectLibrary(const VLNV&)), Qt::UniqueConnection);

    connect(projectLibraryHandler_, SIGNAL(copyVLNV(const VLNV&)),
        this, SLOT(onCopyVLNV_projectLibrary(const VLNV&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::setupAndConnectFPGAHandler()
//-----------------------------------------------------------------------------
void MainWindow::setupAndConnectFPGAHandler()
{
    connect(fpgaHandler_, SIGNAL(openFPGA(QString const&)),
        this, SLOT(onOpenFPGA(QString const&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::setupAndConnectLibraryHandler()
//-----------------------------------------------------------------------------
void MainWindow::setupAndConnectLibraryHandler()
{
    connect(libraryHandler_, SIGNAL(openDesign(const VLNV&, const QString&)),
        this, SLOT(openDesign(const VLNV&, const QString&)));

    connect(libraryHandler_, SIGNAL(openComponent(const VLNV&)),
        this, SLOT(openComponent(const VLNV&)), Qt::UniqueConnection);

//    connect(libraryHandler_, SIGNAL(openSWDesign(const VLNV&, QString const&)),
//        this, SLOT(openSWDesign(const VLNV&, QString const&)), Qt::UniqueConnection);
//    connect(libraryHandler_, SIGNAL(openSystemDesign(const VLNV&, QString const&)),
//        this, SLOT(openSystemDesign(const VLNV&, QString const&)), Qt::UniqueConnection);

    connect(libraryHandler_, SIGNAL(openBus(const VLNV&)),
        this, SLOT(openBus(const VLNV&)), Qt::UniqueConnection);

    connect(libraryHandler_, SIGNAL(openAbsDef(const VLNV&)),
        this, SLOT(openAbsDef(const VLNV&)), Qt::UniqueConnection);

    connect(libraryHandler_, SIGNAL(openBus(const VLNV&, const VLNV&, bool)),
        this, SLOT(onOpenBus(const VLNV&, const VLNV&, bool)), Qt::UniqueConnection);

    connect(libraryHandler_, SIGNAL(openAbsDef(const VLNV&, const VLNV&, bool)),
        this, SLOT(onOpenAbsDef(const VLNV&, const VLNV&, bool)), Qt::UniqueConnection);

//    connect(libraryHandler_, SIGNAL(openComDefinition(const VLNV&)),
//        this, SLOT(openComDefinition(const VLNV&)), Qt::UniqueConnection);
//    connect(libraryHandler_, SIGNAL(openApiDefinition(const VLNV&)),
//        this, SLOT(openApiDefinition(const VLNV&)), Qt::UniqueConnection);

//    connect(libraryHandler_, SIGNAL(openApiDefinition(const VLNV&)),
//        this, SLOT(openApiDefinition(const VLNV&)), Qt::UniqueConnection);

    connect(libraryHandler_, SIGNAL(changeDesignProtection(bool const&)),
        this, SLOT(onChangeDesignProtection(bool const&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::connectTopMainWorkWindow()
//-----------------------------------------------------------------------------
void MainWindow::connectTopMainWorkWindow()
{
    connect(this, SIGNAL(inputMessage(QString const&)), dockHandler_, SIGNAL(inputMessage(QString const&)));
    connect(this, SIGNAL(noticeMessage(QString const&)), dockHandler_, SIGNAL(noticeMessage(QString const&)));
    connect(this, SIGNAL(errorMessage(QString const&)), dockHandler_, SIGNAL(errorMessage(QString const&)));
    connect(this, SIGNAL(undefinedMessage(QString const&)), dockHandler_, SIGNAL(undefinedMessage(QString const&)));

    //    connect(this, SIGNAL(helpUrlRequested(QString const&)),
    //        dockHandler_, SIGNAL(helpUrlRequested(QString const&)), Qt::UniqueConnection);

    connect(dockHandler_, SIGNAL(designChanged()), this, SLOT(onDesignChanged()), Qt::UniqueConnection);

    //    connect(dockHandler_, SIGNAL(statusMessage(QString const&)),
    //        statusBar_, SLOT(showMessage(QString const&)));

}

//-----------------------------------------------------------------------------
// Function: mainwindow::connectDockHandler()
//-----------------------------------------------------------------------------
void MainWindow::connectDockHandler()
{
//#if LOG_OUT_SUPPORT_20240104 == 1
//    connect(this, SIGNAL(inputMessage_LogOut(QString const&)),
//        dockHandler_, SIGNAL(inputMessage_LogOut(QString const&)), Qt::UniqueConnection);

//    connect(this, SIGNAL(noticeMessage_LogOut(QString const&)),
//        dockHandler_, SIGNAL(noticeMessage_LogOut(QString const&)), Qt::UniqueConnection);

//    connect(this, SIGNAL(errorMessage_LogOut(QString const&)),
//        dockHandler_, SIGNAL(errorMessage_LogOut(QString const&)), Qt::UniqueConnection);
//#endif // LOG_OUT_SUPPORT_20240104

    connect(this, SIGNAL(inputMessage(QString const&)), dockHandler_, SIGNAL(inputMessage(QString const&)));
    connect(this, SIGNAL(noticeMessage(QString const&)), dockHandler_, SIGNAL(noticeMessage(QString const&)));
    connect(this, SIGNAL(errorMessage(QString const&)), dockHandler_, SIGNAL(errorMessage(QString const&)));
    connect(this, SIGNAL(undefinedMessage(QString const&)), dockHandler_, SIGNAL(undefinedMessage(QString const&)));

//    connect(this, SIGNAL(helpUrlRequested(QString const&)),
//        dockHandler_, SIGNAL(helpUrlRequested(QString const&)), Qt::UniqueConnection);

    connect(dockHandler_, SIGNAL(designChanged()), this, SLOT(onDesignChanged()), Qt::UniqueConnection);

////    connect(dockHandler_, SIGNAL(statusMessage(QString const&)),
////        statusBar_, SLOT(showMessage(QString const&)));
///

    // Connect Signal from Console
    connect(dockHandler_, SIGNAL(reloadLibrary()), this, SLOT(onLibrarySearch()));
    connect(dockHandler_, SIGNAL(reloadProject()), this, SLOT(onProjectSearch()));
    connect(dockHandler_, SIGNAL(reloadProjectLibrary()), this, SLOT(onProjectLibrarySearch()));
    connect(dockHandler_, SIGNAL(openSchematic(VLNV const&)), this, SLOT(onOpenSchematic(VLNV const&)));
    connect(dockHandler_, SIGNAL(refreshSchematic(VLNV const&)), this, SLOT(onRefreshSchematic(VLNV const&)));
    connect(dockHandler_, SIGNAL(closeSchematic(VLNV const&)), this, SLOT(onCloseSchematic(VLNV const&)));
    connect(dockHandler_, SIGNAL(closeAllSchematic()), this, SLOT(onCloseAllSchematic()));
    connect(dockHandler_, SIGNAL(openFile(QString)), this, SLOT(onOpenFile(QString)));
    connect(dockHandler_, SIGNAL(requestSave()), this, SLOT(saveCurrentDocument()));
    connect(dockHandler_, SIGNAL(requestSaveAll()), this, SLOT(saveAll()));
}

//-----------------------------------------------------------------------------
// Function: MainWindow::hasInvalidReferences()
//-----------------------------------------------------------------------------
bool MainWindow::hasInvalidReferences(QList<VLNV> hierRefs, VLNV const& referencingVlnv, LibraryInterface* lh)
{
    bool invalidReferences = false;
    for (VLNV const& ref : hierRefs)
    {
        // if the hierarchy referenced object is not found in library
        if (!lh->contains(ref))
        {
            emit errorMessage(tr("[ ERROR ] | Invalid References | Component %1 has hierarchical reference to object %2,"
                " which is not found in library. Component is not valid and can not "
                "be opened in design view. Edit component with component editor to "
                "remove invalid references.").arg(referencingVlnv.toString(":")).arg(ref.toString(":")));
            invalidReferences = true;
        }

        // if the reference is to a wrong object type
        else if (lh->getDocumentType(ref) != VLNV::DESIGN &&
            lh->getDocumentType(ref) != VLNV::DESIGNCONFIGURATION)
        {
            emit errorMessage(tr("[ ERROR ] | Invalid References | Component %1 has hierarchical reference to object %2,"
                " which is not design or design configuration. Component is not valid and"
                " can not be opened in design view. Edit component with component editor to"
                " remove invalid references.").arg(referencingVlnv.toString(":")).arg(ref.toString(":")));
            invalidReferences = true;
        }

        // if the reference is for a design configuration then check that also
        // the design is found
        else if (lh->getDocumentType(ref) == VLNV::DESIGNCONFIGURATION)
        {
            VLNV designVLNV = lh->getDesignVLNV(ref);

            QSharedPointer<Document> libComp2 = lh->getModel(ref);
            QSharedPointer<DesignConfiguration> desConf = libComp2.staticCast<DesignConfiguration>();
            VLNV refToDesign = desConf->getDesignRef();

            // if the referenced design was not found in the library
            if (!designVLNV.isValid())
            {
                emit errorMessage(tr("[ ERROR ] | Invalid References | Component %1 has hierarchical reference to object %2, "
                    "which is design configuration and references to design %3. This "
                    "design is not found in library so component can not be opened in "
                    "design view. Edit component with component editor to remove invalid references").arg(
                    referencingVlnv.toString(":"), ref.toString(":"), refToDesign.toString(":")));
                invalidReferences = true;
            }
        }
    }

    return invalidReferences;
}


//-----------------------------------------------------------------------------
// Function: MainWindow::isOpen()
//-----------------------------------------------------------------------------
bool MainWindow::isOpen(VLNV const& vlnv) const
{
    if (!vlnv.isValid())
    {
        return false;
    }

    for (int i = 0; i < designTabs_->count(); i++)
    {
        TabDocument* document = dynamic_cast<TabDocument*>(designTabs_->widget(i));

        // if the document is already open is some tab
        if (document && document->getIdentifyingVLNV() == vlnv)
        {
            designTabs_->setCurrentIndex(i);
            return true;
        }
    }
    // document was not open
    return false;

//    return false;
}

//-----------------------------------------------------------------------------
// Function: restoreSettings()
//-----------------------------------------------------------------------------
void MainWindow::restoreSettings()
{
    workspace_.restoreSettings();
//    updateWorkspaceMenu();
//    actWorkspaces_->setText(workspace_.getCurrentWorkspace());

    dockHandler_->applySettings();
}

//-----------------------------------------------------------------------------
// Function: saveSettings()
//-----------------------------------------------------------------------------
void MainWindow::saveSettings()
{
    workspace_.saveSettings();
}

//-----------------------------------------------------------------------------
// Function: loadWorkspace()
//-----------------------------------------------------------------------------
void MainWindow::loadWorkspace(QString const& workspaceName)
{
    workspace_.loadWorkspace(workspaceName);

    updateWindows();
}

//-----------------------------------------------------------------------------
// Function: updateWorkspaceMenu()
//-----------------------------------------------------------------------------
//void MainWindow::updateWorkspaceMenu()
//{
//    workspace_.updateWorkspaceMenu(&workspaceMenu_);
//}

//-----------------------------------------------------------------------------
// Function: mainwindow::setupActions()
//-----------------------------------------------------------------------------
void MainWindow::setupActions()
{
    // the action to create a new hierarchical component
    actNew_ = new QAction(QIcon(":/resources/Res/file-new.png"), tr("New"), this);
    actNew_->setShortcut(QKeySequence::New);
    connect(actNew_, SIGNAL(triggered()), this, SLOT(createNew()));

    actSave_ = new QAction(QIcon(":/resources/Res/file-save.png"), tr("Save"), this);
    actSave_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    actSave_->setEnabled(false);
    connect(actSave_, SIGNAL(triggered()), this, SLOT(saveCurrentDocument()));
    connect(designTabs_, SIGNAL(documentModifiedChanged(bool)), actSave_, SLOT(setEnabled(bool)), Qt::UniqueConnection);

//    actSaveAs_ = new QAction(QIcon(":/resources/Res/file-save-as.png"), tr("Save As"), this);
//    actSaveAs_->setShortcut(QKeySequence::SaveAs);
//    actSaveAs_->setEnabled(false);
//    connect(actSaveAs_, SIGNAL(triggered()), designTabs_, SLOT(saveCurrentDocumentAs()));

    actSaveAll_ = new QAction(QIcon(":/resources/Res/file-save_all.png"), tr("Save All"), this);
    actSaveAll_->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(actSaveAll_, SIGNAL(triggered()), this, SLOT(saveAll()));

//    actSaveHierarchy_ = new QAction(QIcon(":/resources/Res/file-save_all.png"),
//        tr("Save Hierarchy"), this);
//    actSaveHierarchy_->setEnabled(false);
//    connect(actSaveHierarchy_, SIGNAL(triggered()), this, SLOT(saveCurrentDocumentHierarchy()));

//    auto saveMenu = new QMenu(this);
//    saveMenu->addAction(actSaveAll_);
//    saveMenu->addAction(actSaveHierarchy_);
//    actSaveAs_->setMenu(saveMenu);

//    actPrint_ = new QAction(QIcon(":/resources/Res/file-print.png"), tr("Print"), this);
//    actPrint_->setShortcut(QKeySequence::Print);
//    actPrint_->setEnabled(false);
//    connect(actPrint_, SIGNAL(triggered()), designTabs_, SLOT(printCurrentDocument()));

//    actImageExport_ = new QAction(QIcon(":/resources/Res/export.png"), tr("Export image"), this);
//    actImageExport_->setEnabled(false);
//    connect(actImageExport_, SIGNAL(triggered()), designTabs_, SLOT(exportCurrentDocumentAsImage()));

//    actUndo_ = new QAction(QIcon(":/resources/Res/edit-undo.png"), tr("Undo"), this);
//    actUndo_->setShortcut(QKeySequence::Undo);
//    connect(actUndo_, SIGNAL(triggered()), this, SLOT(undo()));

//    actRedo_ = new QAction(QIcon(":/resources/Res/edit-redo.png"),
//        tr("Redo"), this);
//    actRedo_->setShortcut(QKeySequence::Redo);
//    connect(actRedo_, SIGNAL(triggered()), this, SLOT(redo()));

//    actLibraryLocations_ = new QAction(QIcon(":/resources/Res/library-config.png"),
//        tr("Configure Library"), this);
//    connect(actLibraryLocations_, SIGNAL(triggered()), this, SLOT(setLibraryLocations()), Qt::UniqueConnection);

//    // the action to search for IP-Xact documents in file system
//    actLibrarySearch_ = new QAction(QIcon(":/resources/Res/library-refresh.png"),
//        tr("Refresh Library"), this);
//    connect(actLibrarySearch_, SIGNAL(triggered()),	this, SLOT(onLibrarySearch()), Qt::UniqueConnection);

//    // Check the library integrity
//    actCheckIntegrity_ = new QAction(QIcon(":/resources/Res/checkIntegrity.png"),
//        tr("View Library Integrity Report"), this);
//    connect(actCheckIntegrity_, SIGNAL(triggered()),
//        dockHandler_, SIGNAL(generateIntegrityReport()), Qt::UniqueConnection);

//    // initialize the action to generate documentation for the component/design
//    actGenDocumentation_ = new QAction(QIcon(":resources/Res/documentation.png"),
//        tr("Documentation"), this);
//    connect(actGenDocumentation_, SIGNAL(triggered()), this, SLOT(generateDoc()), Qt::UniqueConnection);

//    // initialize the action to run import wizard for component.
//    actRunImport_ = new QAction(QIcon(":resources/Res/import.png"), tr("Import File to Component"), this);
//    connect(actRunImport_, SIGNAL(triggered()), this, SLOT(onRunImportWizard()), Qt::UniqueConnection);

#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
    actVerilogGeneratort_ = new QAction(QIcon(":/resources/Res/verilogGenerator.png"), tr("Verilog Generator"), this);
    actVerilogGeneratort_->setCheckable(true);
    actVerilogGeneratort_->setChecked(true);

    pluginActionGroup_ = new QActionGroup(this);

    pluginActionGroup_->setExclusive(true);

#if PLUGIN_TOOLS_SUPPORT_20240905 == 0
    pluginActionGroup_->addAction(actVerilogGeneratort_);

    connect(pluginActionGroup_, SIGNAL(triggered(QAction*)),
            this, SLOT(runGenerator()), Qt::UniqueConnection);
#endif // PLUGIN_TOOLS_SUPPORT_20240905

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1
    // Initialize the action to set draw mode to selection mode.

    actToolSelect_ = new QAction(QIcon(":/resources/Res/tool-select.png"), tr("Select Tool"), this);
    actToolSelect_->setCheckable(true);
    actToolSelect_->setChecked(true);

    // Initialize the action to set draw mode to connection mode.
    actToolConnect_ = new QAction(QIcon(":/resources/Res/tool-interconnection.png"),
        tr("Interconnection Tool"), this);
    actToolConnect_->setCheckable(true);

    // Initialize the action to set draw mode to interface mode.
    actToolInterface_ = new QAction(QIcon(":/resources/Res/tool-interface.png"), tr("Interface Tool"), this);
    actToolInterface_->setCheckable(true);

    actToolDraft_ = new QAction(QIcon(":/resources/Res/tool-drafting.png"), tr("Drafting Tool"), this);
    actToolDraft_->setCheckable(true);

    actToolToggleOffPage_ = new QAction(QIcon(":/resources/Res/tool-toggle_offpage.png"),
        tr("Toggle Off-Page Tool"), this);
    actToolToggleOffPage_->setCheckable(true);

    actToolLabel_ = new QAction(QIcon(":/resources/Res/balloon.png"), tr("Sticky Note Tool"), this);
    actToolLabel_->setCheckable(true);

    // Initialize the action to add a new column.
    actAddColumn_ = new QAction(QIcon(":/resources/Res/diagram-add-column.png"), tr("Add Column"), this);
    connect(actAddColumn_, SIGNAL(triggered()), this, SLOT(addColumn()), Qt::UniqueConnection);

    modeActionGroup_ = new QActionGroup(this);
    modeActionGroup_->setExclusive(true);
    modeActionGroup_->addAction(actToolSelect_);
    modeActionGroup_->addAction(actToolConnect_);
    modeActionGroup_->addAction(actToolInterface_);
    modeActionGroup_->addAction(actToolDraft_);
    modeActionGroup_->addAction(actToolToggleOffPage_);
    modeActionGroup_->addAction(actToolLabel_);
    connect(modeActionGroup_, SIGNAL(triggered(QAction *)), this, SLOT(drawModeChange(QAction *)));

#endif



//    //! Initialize the action to condense memory graphics items.
//    actionCondenseMemoryItems_ = new QAction(QIcon(":resources/Res/compressMemoryItems.png"),
//        tr("Compress Memory Items"), this);
//    actionCondenseMemoryItems_->setCheckable(true);
//    connect(actionCondenseMemoryItems_, SIGNAL(triggered(bool)),
//        this, SLOT(onCondenseMemoryItems(bool)), Qt::UniqueConnection);

//    //! Initialize the action to filter chained address space memory connections.
//    actionFilterAddressSpaceChains_ = new QAction(QIcon(":/resources/Res/addressSpaceFilter.png"),
//        tr("Address Space Filter"), this);
//    actionFilterAddressSpaceChains_->setCheckable(true);
//    connect(actionFilterAddressSpaceChains_, SIGNAL(triggered(bool)),
//        this, SLOT(onFilterAddressSpaceChains(bool)), Qt::UniqueConnection);

//    //! Initialize the action to condense field graphics items.
//    actionCondenseFieldItems_ = new QAction(QIcon(":resources/Res/compressFields.png"),
//        tr("Compress Field Items"), this);
//    connect(actionCondenseFieldItems_, SIGNAL(triggered()),
//        this, SLOT(onCondenseFieldItems()), Qt::UniqueConnection);

//    //! Initialize the action to extend field graphics items.
//    actionExtendFieldItems_ = new QAction(QIcon(":resources/Res/extendFields.png"),
//        tr("Extend Field Items"), this);
//    connect(actionExtendFieldItems_, SIGNAL(triggered()), this, SLOT(onExtendFieldItems()), Qt::UniqueConnection);

//    //! Initialize the action to filter address space segments.
//    actionFilterSegments_ = new QAction(QIcon(":resources/Res/filterSegments.png"),
//        tr("Filter Segments"), this);
//    actionFilterSegments_->setCheckable(true);
//    connect(actionFilterSegments_, SIGNAL(triggered(bool)),
//        this, SLOT(onFilterSegments(bool)), Qt::UniqueConnection);

//    //! Initialize the action to filter memory map address blocks.
//    actionFilterAddressBlocks_ = new QAction(QIcon(":resources/Res/filterAddressBlocks.png"),
//        tr("Filter Address Blocks"), this);
//    actionFilterAddressBlocks_->setCheckable(true);
//    connect(actionFilterAddressBlocks_, SIGNAL(triggered(bool)),
//        this, SLOT(onFilterAddressBlocks(bool)), Qt::UniqueConnection);

//    //! Initialize the action to filter address block registers.
//    actionFilterRegisters_ = new QAction(QIcon(":resources/Res/filterRegisters.png"),
//        tr("Filter Registers"), this);
//    actionFilterRegisters_->setCheckable(true);
//    connect(actionFilterRegisters_, SIGNAL(triggered(bool)),
//        this, SLOT(onFilterRegisters(bool)), Qt::UniqueConnection);

//    //! Initialize the action to filter register fields.
//    actionFilterFields_ = new QAction(QIcon(":resources/Res/filterFields.png"), tr("Filter Fields"), this);
//    actionFilterFields_->setCheckable(true);
//    connect(actionFilterFields_, SIGNAL(triggered(bool)), this, SLOT(onFilterFields(bool)), Qt::UniqueConnection);

//    //! Initialize the action to filter unconnected memory items.
//    actionFilterUnconnectedMemoryItems_ = new QAction(QIcon(
//        ":resources/Res/filterUnconnectedMemoryItems.png"), tr("Filter unconnected items"), this);
//    actionFilterUnconnectedMemoryItems_->setCheckable(true);
//    connect(actionFilterUnconnectedMemoryItems_, SIGNAL(triggered(bool)),
//        this, SLOT(onFilterUnconnectedMemoryItems(bool)), Qt::UniqueConnection);

    // Initialize the action to zoom in.
    actZoomIn_ = new QAction(QIcon(":/resources/Res/view-zoom_in.png"), tr("Zoom In"), this);
    actZoomIn_->setEnabled(false);
    actZoomIn_->setVisible(false);
    connect(actZoomIn_, SIGNAL(triggered()), this, SLOT(onZoomIn()));

    // Initialize the action to zoom out.
    actZoomOut_ = new QAction(QIcon(":/resources/Res/view-zoom_out.png"), tr("Zoom Out"), this);
    actZoomOut_->setEnabled(false);
    actZoomOut_->setVisible(false);
    connect(actZoomOut_, SIGNAL(triggered()), this, SLOT(onZoomOut()));

    // Initialize the action to reset the zoom to original 1:1 ratio.
    actZoomOriginal_ = new QAction(QIcon(":/resources/Res/view-zoom_original.png"),
        tr("Original 1:1 Zoom"), this);
    actZoomOriginal_->setEnabled(false);
    actZoomOriginal_->setVisible(false);
    connect(actZoomOriginal_, SIGNAL(triggered()), this, SLOT(onZoomOriginal()));

    // Initialize the action to fit the document into the view.
    actFitInView_ = new QAction(QIcon(":/resources/Res/view-fit_best.png"),
        tr("Fit Document to View"), this);
    actFitInView_->setEnabled(false);
    actFitInView_->setVisible(false);
    connect(actFitInView_, SIGNAL(triggered()), this, SLOT(onFitInView()));

//    // the action for user to select the visible docks
//    actVisibleDocks_ = new QAction(QIcon(":resources/Res/dockSelect.png"), tr("Visible Windows"), this);
//    actVisibleDocks_->setMenu(&windowsMenu_);
//    connect(actVisibleDocks_, SIGNAL(triggered()), this, SLOT(selectVisibleDocks()), Qt::UniqueConnection);

//    actVisibleScript_ = new QAction(tr("Script editor"), this);
//    actVisibleScript_->setCheckable(true);
//    actVisibleScript_->setChecked(false);
//    connect(actVisibleScript_, SIGNAL(toggled(bool)), scriptEditor_, SLOT(setVisible(bool)));


//    // Initialize the action to manage visibility control.
//    actVisibilityControl_ = new QAction(QIcon(":resources/Res/visibility.png"), tr("Visibility Control"), this);
//    actVisibilityControl_->setEnabled(false);
//    actVisibilityControl_->setVisible(false);
//    actVisibilityControl_->setMenu(&visibilityMenu_);
//    connect(actVisibilityControl_, SIGNAL(triggered()), this, SLOT(openVisibilityControlMenu()), Qt::UniqueConnection);
//    connect(&visibilityMenu_, SIGNAL(triggered(QAction*)), this, SLOT(onVisibilityControlToggled(QAction*)));

//    // Initialize the action to manage workspaces.
//    actWorkspaces_ = new QAction(QIcon(":resources/Res/workspace.png"),	tr("Workspaces"), this);
//    connect(actWorkspaces_, SIGNAL(triggered()), this, SLOT(openWorkspaceMenu()), Qt::UniqueConnection);
//    actWorkspaces_->setMenu(&workspaceMenu_);

//    actRefresh_ = new QAction(QIcon(":/resources/Res/refresh.png"), tr("Refresh"), this);
//    actRefresh_->setShortcut(QKeySequence("F5"));
//    connect(actRefresh_, SIGNAL(triggered(bool)), designTabs_, SLOT(refreshCurrentDocument()));

//    actProtect_ = new QAction(QIcon(":/resources/Res/protection-unlocked.png"), tr("Unlocked"), this);
//    actProtect_->setCheckable(true);
//    actProtect_->setEnabled(false);
//    actProtect_->setShortcut(QKeySequence("Ctrl+L"));
//    connect(actProtect_, SIGNAL(triggered(bool)), this, SLOT(changeProtection(bool)));

//    // Initialize the action to open Kactus2 settings.
//    actSettings_ = new QAction(QIcon(":/resources/Res/settings-general.png"), tr("Settings"), this);
//    connect(actSettings_, SIGNAL(triggered()), this, SLOT(onOpenSettings()));

//    // Initialize the action to open the about box.
//    actAbout_= new QAction(QIcon(":/resources/Res/system-about.png"), tr("About"), this);
//    connect(actAbout_, SIGNAL(triggered()), this, SLOT(showAbout()), Qt::UniqueConnection);

//    // Initialize the action to open the help window.
//    actHelp_= new QAction(QIcon(":/resources/Res/system-help.png"), tr("Help"), this);
//    actHelp_->setShortcut(QKeySequence::HelpContents);
//    connect(actHelp_, SIGNAL(triggered()), dockHandler_, SLOT(showHelp()), Qt::UniqueConnection);

//    // Initialize the action to exit the program.
//    actExit_ = new QAction(QIcon(":/resources/Res/system-exit.png"), tr("Exit"), this);
//    connect(actExit_, SIGNAL(triggered()), this, SLOT(close()), Qt::UniqueConnection);

//    actionConfigureViews_ = new QAction(QIcon(":/resources/Res/viewConfiguration.png"),
//        tr("View Configuration"), this);
//    connect(actionConfigureViews_, SIGNAL(triggered()), this, SLOT(onConfigureViews()), Qt::UniqueConnection);

    // QToolBar에 추가할 QAction 객체를 생성합니다.
//    actPythonCommunication_ = new QAction("Python Communication", this);
//    actPythonGenScript_ = new QAction("Python Gen Script", this);
    actManualReload_ = new QAction("Manual Reload", this);
    actProjectConfigure_ = new QAction("Project Configure", this);
    actLibraryConfigure_ = new QAction("Library Configure", this);

    // QAction 객체를 툴바에 추가합니다.
//    toolbar_->addAction(actPythonCommunication_);

//    toolbar_->addAction(actPythonGenScript_);

    toolbar_->addAction(actManualReload_);
    connect(actManualReload_, SIGNAL(triggered()),	this, SLOT(onLibrarySearch()), Qt::UniqueConnection);

    toolbar_->addAction(actProjectConfigure_);
    connect(actProjectConfigure_, SIGNAL(triggered()),	this, SLOT(setProjectLocations()), Qt::UniqueConnection);

    toolbar_->addAction(actLibraryConfigure_);
    connect(actLibraryConfigure_, SIGNAL(triggered()),	this, SLOT(setLibraryLocations()), Qt::UniqueConnection);
//    connect(actLibrarySearch_, SIGNAL(triggered()),	this, SLOT(onLibrarySearch()), Qt::UniqueConnection);

    // QPushButton 객체를 생성합니다.
    QPushButton *button = new QPushButton("Edit", this);
    QIcon buttonIcon(":/resources/Res/Play.png");
    button->setIconSize(QSize(24, 24));  // Set the size of the icon to 24x24 pixels
    button->setIcon(buttonIcon);
    toolbar_->addWidget(button);    // QPushButton 객체를 툴바에 추가합니다.

    // 버튼 클릭 시의 슬롯 연결
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    connectVisibilityControls();

    setupMenus();

}

//-----------------------------------------------------------------------------
// Function: setupMenus()
//-----------------------------------------------------------------------------
void MainWindow::setupMenus()
{
//    ribbon_ = new Ribbon(this);
//    addToolBar(Qt::TopToolBarArea, ribbon_);

//    // The "File" group.
//    RibbonGroup* fileGroup = new RibbonGroup(tr("File"), ribbon_);
//    fileGroup->addAction(actNew_);
//    fileGroup->addAction(actSave_);
//    fileGroup->addAction(actSaveAs_);
//    fileGroup->addAction(actPrint_);
//    fileGroup->addAction(actImageExport_);
//    fileGroup->addAction(actRunImport_);

//    ribbon_->addGroup(fileGroup);
//    actRunImport_->setVisible(false);
//    actRunImport_->setEnabled(false);

//    // The "Library" group.
//    RibbonGroup* libGroup = new RibbonGroup(tr("Library"), ribbon_);
//    libGroup->addAction(actLibraryLocations_);
//    libGroup->addAction(actLibrarySearch_);
//    libGroup->addAction(actCheckIntegrity_);
//    ribbon_->addGroup(libGroup);

//    // The "protection" group
//    protectGroup_ = new RibbonGroup(tr("Protection"), ribbon_);
//    protectGroup_->addAction(actProtect_);

//    protectAction_ = ribbon_->addGroup(protectGroup_);
//    protectAction_->setVisible(false);

//    // The "Edit" group.
//    editGroup_ = new RibbonGroup(tr("Edit"), ribbon_);
//    editGroup_->addAction(actRefresh_);
//    editGroup_->addAction(actUndo_);
//    editGroup_->addAction(actRedo_);

//    editAction_ = ribbon_->addGroup(editGroup_);
//    editAction_->setVisible(false);
//    editAction_->setEnabled(false);

#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
    // The "Generation" group.
    generationToolsGroup_ = new QToolBar(tr("Generation"), toolbar_);

#if PLUGIN_TOOLS_SUPPORT_20240905 == 1
    createGeneratorPluginActions();

    generationToolsGroup_->addActions(pluginActionGroup_->actions());
#else
    generationToolsGroup_->addAction(actVerilogGeneratort_);
#endif // PLUGIN_TOOLS_SUPPORT_20240905

    generationToolsAction_ = toolbar_->addWidget(generationToolsGroup_);
    generationToolsAction_->setVisible(false);

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1

    //! The "Diagram Tools" group.
    diagramToolsGroup_ = new QToolBar(tr("Diagram Tools"), toolbar_);
    diagramToolsGroup_->addAction(actToolSelect_);
    diagramToolsGroup_->addAction(actToolConnect_);
    diagramToolsGroup_->addAction(actToolInterface_);
    diagramToolsGroup_->addAction(actToolDraft_);
    diagramToolsGroup_->addAction(actToolToggleOffPage_);
    diagramToolsGroup_->addAction(actToolLabel_);
    diagramToolsGroup_->addAction(actAddColumn_);

    diagramToolsAction_ = toolbar_->addWidget(diagramToolsGroup_);
    diagramToolsAction_->setVisible(false);
//    diagramToolsAction_->setVisible(true);

#endif

//    //! The "Filtering tools" group.
//    filteringGroup_ = new RibbonGroup(tr("Filtering Tools"), ribbon_);
//    filteringGroup_->addAction(actionFilterSegments_);
//    filteringGroup_->addAction(actionFilterAddressBlocks_);
//    filteringGroup_->addAction(actionFilterRegisters_);
//    filteringGroup_->addAction(actionFilterFields_);
//    filteringGroup_->addAction(actionFilterUnconnectedMemoryItems_);
//    filteringGroup_->addAction(actionFilterAddressSpaceChains_);
//    filteringGroup_->addAction(actionCondenseMemoryItems_);
//    filteringGroup_->addAction(actionCondenseFieldItems_);
//    filteringGroup_->addAction(actionExtendFieldItems_);

//    filteringAction_ = ribbon_->addGroup(filteringGroup_);
//    filteringAction_->setVisible(false);
//    filteringAction_->setEnabled(true);

    //! The "View" group.
    viewToolsGroup_ = new QToolBar(tr("View"), toolbar_);
//    viewGroup->addAction(actVisibleDocks_);
    viewToolsGroup_->addAction(actZoomIn_);
    viewToolsGroup_->addAction(actZoomOut_);
    viewToolsGroup_->addAction(actZoomOriginal_);
    viewToolsGroup_->addAction(actFitInView_);
//    viewToolsGroup_->addAction(actVisibilityControl_);

    viewToolsAction_ = toolbar_->addWidget(viewToolsGroup_);
    viewToolsAction_->setVisible(false);

//    ribbon_->addGroup(viewGroup);

//    //! The "Configuration tools" group.
//    configurationToolsGroup_ = new RibbonGroup(tr("Configuration Tools"), ribbon_);
//    configurationToolsGroup_->addAction(actionConfigureViews_);

//    configurationToolsAction_ = ribbon_->addGroup(configurationToolsGroup_);
//    configurationToolsAction_->setVisible(false);
//    configurationToolsAction_->setEnabled(false);

//    //! The "Workspace" group.
//    RibbonGroup* workspacesGroup = new RibbonGroup(tr("Workspace"), ribbon_);
//    workspacesGroup->addAction(actWorkspaces_);
//    workspacesGroup->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    ribbon_->addGroup(workspacesGroup);

//    connect(&workspace_, SIGNAL(requestMenuUpdate()), this, SLOT(updateWorkspaceMenu()), Qt::UniqueConnection);
//    connect(&workspace_, SIGNAL(workspaceChanged(QString const&)),
//        this, SLOT(onWorkspaceChanged(QString const&)), Qt::UniqueConnection);

//    //! The "System" group.
//    RibbonGroup* sysGroup = new RibbonGroup(tr("System"), ribbon_);
//    sysGroup->addAction(actSettings_);
//    sysGroup->addAction(actHelp_);
//    sysGroup->addAction(actAbout_);
//    sysGroup->addAction(actExit_);

//    ribbon_->addGroup(sysGroup);


//    // the menu to display the dock widgets
//    windowsMenu_.addAction(actVisibleScript_);
//    windowsMenu_.addSeparator();
//    dockHandler_->setupVisibilityActionMenu(windowsMenu_);
}

//-----------------------------------------------------------------------------
// Function: MainWindow::setupToolbars()
//-----------------------------------------------------------------------------
void MainWindow::setupToolbars()
{
//    QString toolbarStyle = "background-color: #DAE1E9";
    QString toolbarStyle = "background-color: #2682ED";

//    leftToolbar_->setObjectName("leftToolBar");
//    leftToolbar_->setOrientation(Qt::Vertical);
//    leftToolbar_->setMovable(false);
//    leftToolbar_->setIconSize(QSize(32, 32));
//    leftToolbar_->setStyleSheet(toolbarStyle);

//    addToolBar(Qt::LeftToolBarArea, leftToolbar_);

    rightToolbar_->setObjectName("rightToolbar");
    rightToolbar_->setOrientation(Qt::Vertical);
    rightToolbar_->setMovable(false);
    rightToolbar_->setIconSize(QSize(32, 32));
    rightToolbar_->setStyleSheet(toolbarStyle);

    addToolBar(Qt::RightToolBarArea, rightToolbar_);

    // QToolBar 객체를 MainWindow에 추가합니다.
    addToolBar(toolbar_);

//    designRightToolBar_->setFixedWidth(30);
    sideToolBar_->setFixedWidth(30);

    connect(sideToolBar_, SIGNAL(inputMessage(QString const&)), dockHandler_, SIGNAL(inputMessage(QString const&)), Qt::UniqueConnection);
    connect(sideToolBar_, SIGNAL(noticeMessage(QString const&)), dockHandler_, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
    connect(sideToolBar_, SIGNAL(errorMessage(QString const&)), dockHandler_, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
    connect(sideToolBar_, SIGNAL(undefinedMessage(QString const&)), dockHandler_, SIGNAL(undefinedMessage(QString const&)), Qt::UniqueConnection);

//    connect(designRightToolBar_, SIGNAL(noticeMessage_LogOut(QString const&)),
//        this, SIGNAL(noticeMessage_LogOut(QString const&)), Qt::UniqueConnection);

//    connect(designRightToolBar_, SIGNAL(errorMessage_LogOut(QString const&)),
//        this, SIGNAL(errorMessage_LogOut(QString const&)), Qt::UniqueConnection);
}



#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
//-----------------------------------------------------------------------------
// Function: MainWindow::createGeneratorPluginActions()
//-----------------------------------------------------------------------------
void MainWindow::createGeneratorPluginActions()
{
    for (IPlugin* plugin : PluginManager::getInstance().getActivePlugins())
    {
        IGeneratorPlugin* genPlugin = dynamic_cast<IGeneratorPlugin*>(plugin);

        if (genPlugin != 0)
        {
            // Add a small overlay icon to the plugin icon to visualize that this is a plugin.
            QIcon pluginBaseIcon = genPlugin->getIcon();

            QPixmap icon(32, 32);
            icon.fill(Qt::transparent);

            QPainter painter(&icon);
            painter.drawPixmap(0, 0, pluginBaseIcon.pixmap(32, 32));

            QAction* action = new QAction(icon, genPlugin->getName(), this);
            action->setData(QVariant::fromValue((void*)genPlugin));

            pluginActionGroup_->addAction(action);
        }
    }

    connect(pluginActionGroup_, SIGNAL(triggered(QAction*)),
            this, SLOT(runGeneratorPlugin(QAction*)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: MainWindow::updateGeneratorPluginActions()
//-----------------------------------------------------------------------------
void MainWindow::updateGeneratorPluginActions()
{
    // Remove all plugin actions.
    QList<QAction*> actions = generationToolsGroup_->actions();

    for (QAction* action : actions)
    {
        // Check if the action contains the plugin pointer.
        if (action->data().value<void*>() != 0)
        {
            pluginActionGroup_->removeAction(action);
            delete action;
        }
    }

    // Recreate the plugin actions.
    createGeneratorPluginActions();

    setPluginVisibilities();
}

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

//-----------------------------------------------------------------------------
// Function: mainwindow::updateWindows()
//-----------------------------------------------------------------------------
void MainWindow::updateWindows()
{
    dockHandler_->updateWindows(designTabs_->currentWidget());
}

//-----------------------------------------------------------------------------
// Function: MainWindow::connectVisibilityControls()
//-----------------------------------------------------------------------------
void MainWindow::connectVisibilityControls()
{
    connect(dockHandler_, SIGNAL(adjustVisibilityInWindow(TabDocument::SupportedWindows, bool)),
        this, SLOT(onAdjustVisibilityInWindow(TabDocument::SupportedWindows, bool)));
    dockHandler_->connectVisibilityControls();
}

//-----------------------------------------------------------------------------
// Function: MainWindow::disconnectVisibilityControls()
//-----------------------------------------------------------------------------
void MainWindow::disconnectVisibilityControls()
{
    disconnect(dockHandler_, SIGNAL(adjustVisibilityInWindow(TabDocument::SupportedWindows, bool)),
        this, SLOT(onAdjustVisibilityInWindow(TabDocument::SupportedWindows, bool)));
    dockHandler_->disconnectVisibilityControls();
}

//-----------------------------------------------------------------------------
// Function: MainWindow::runComponentWizard()
//-----------------------------------------------------------------------------
void MainWindow::runComponentWizard(QSharedPointer<Component> component, QString const& directory)
{
    // Open the component wizard.
    ComponentWizard wizard(component, directory, projectHandler_, libraryHandler_, this);

    QString styleSheet("*[mandatoryField=\"true\"] { background-color: LemonChiffon; }");
    wizard.setStyleSheet(styleSheet);

    if (wizard.exec() == QDialog::Accepted)
    {
        // Save wizard changes.
        if (!libraryHandler_->writeModelToFile(wizard.getComponent()))
        {
            emit errorMessage("Error saving file to disk.");
        }
    }
}

//-----------------------------------------------------------------------------
// Function: mainwindow::setPluginVisibilities()
//-----------------------------------------------------------------------------
void MainWindow::setPluginVisibilities()
{
    TabDocument* doc = static_cast<TabDocument*>(designTabs_->currentWidget());

    // Enable/disable the plugin generator actions based on the component being edited in the document.
    VLNV compVLNV;
    VLNV desVLNV;
    QSharedPointer<Component const> component;
    QSharedPointer<Design const> design;
    QSharedPointer<DesignConfiguration const> designConfiguration;

    if (doc)
    {
        compVLNV = doc->getDocumentVLNV();
        desVLNV = doc->getIdentifyingVLNV();
        QSharedPointer<Document const> libComp = libraryHandler_->getModelReadOnly(compVLNV);
        component = libComp.dynamicCast<Component const>();

        if (component)
        {
            // if the design is supported by the document type
            DesignWidget* desWidget = qobject_cast<DesignWidget*>(doc);
            if (desWidget)
            {
                // design is the object that identifies the editor
                QSharedPointer<Document const> libDes = libraryHandler_->getModelReadOnly(desVLNV);
                design = libDes.dynamicCast<Design const>();

                // find the design config is one exists
                QString viewName = desWidget->getOpenViewName();

                // The name must exist!
                Q_ASSERT(!viewName.isEmpty());

                VLNV desConfVLNV;

                // the implementation defines where to search for the hierarchy ref
                switch (desWidget->getImplementation())
                {
                case KactusAttribute::HW:
                case KactusAttribute::SW:
                    {
                        desConfVLNV = component->getHierRef(viewName);
                        break;
                    }
                case KactusAttribute::SYSTEM:
                    {
                        desConfVLNV = component->getHierSystemRef(viewName);
                        break;
                    }
                default:
                    {
                        Q_ASSERT(false);
                        return;
                    }
                }

                // if the hierarchy ref is not directly to the design but design config is in between
                if (desConfVLNV.isValid() && desConfVLNV != desVLNV)
                {
                     QSharedPointer<Document const> libDesConf = libraryHandler_->getModelReadOnly(desConfVLNV);
                     designConfiguration = libDesConf.dynamicCast<DesignConfiguration const>();
                }
            }
        }
    }

    for (QAction* action : pluginActionGroup_->actions())
    {
        IGeneratorPlugin* plugin = reinterpret_cast<IGeneratorPlugin*>(action->data().value<void*>());
        Q_ASSERT(plugin != 0);

        action->setVisible(component != 0 && plugin->checkGeneratorSupport(component, design, designConfiguration));
    }


//    bool isGenerationGroupVisible = false;
//    for (QAction* action : generationGroup_->actions())
//    {
//        if (action->isVisible())
//        {
//            isGenerationGroupVisible = true;
//            break;
//        }
//    }

//    generationAction_->setVisible(isGenerationGroupVisible && doc);
}

//-----------------------------------------------------------------------------
// Function: closeEvent()
//-----------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent* event)
{
    // disconnect the signals that would otherwise change the window states to be saved.
    disconnectVisibilityControls();

    // Go through all tab documents and ask the user what to do if they are not saved.
    designTabs_->closeAll();

    if (designTabs_->count() != 0)
    {
        event->ignore();
        return;
    }
}

//-----------------------------------------------------------------------------
// Function: mainwindow::hideEvent()
//-----------------------------------------------------------------------------
void MainWindow::hideEvent( QHideEvent* event )
{
    disconnectVisibilityControls();
    QMainWindow::hideEvent(event);
}

//-----------------------------------------------------------------------------
// Function: mainwindow::showEvent()
//-----------------------------------------------------------------------------
void MainWindow::showEvent( QShowEvent* event )
{
    connectVisibilityControls();
    QMainWindow::showEvent(event);
}
