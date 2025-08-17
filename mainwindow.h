#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef GENERATORPLUGIN_TOOLS_SUPPORT_20240618
#define GENERATORPLUGIN_TOOLS_SUPPORT_20240618  1
#endif

//#ifndef PLUGIN_TOOLS_SUPPORT_20240905
//#define PLUGIN_TOOLS_SUPPORT_20240905  0
//#endif

#include <QMainWindow>
#include <QActionGroup>
#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QDockWidget>
#include <QHideEvent>
#include <QProcess>

#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>
#include <IPXACTmodels/common/TagData.h>

#include <IPXACTmodels/common/VLNV.h>

#include <KactusAPI/PluginManager.h>

#include <UI/Widgets/TabDocument/TabDocument.h>

#include "WorkspaceSettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LibraryInterface;
class ProjectHandler;
class ProjectLibraryHandler;
class FPGAHandler;
class LibraryHandler;
class MessageMediator;
class Component;
class ComponentItem;
class Project;
class VLNV;
class DockWidgetHandler;
class DrawingBoard;
class AWSideToolBar;
class DesignRightToolBar;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ProjectHandler* project,
               ProjectLibraryHandler* projectLibrary,
               FPGAHandler* fpga,
               LibraryHandler* library,
               MessageMediator* messageChannel,
               QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //! The instance that manages the IP-Xact project
    ProjectHandler *projectHandler_;

    //! The instance that manages the project library
    ProjectLibraryHandler *projectLibraryHandler_;

    //! The instance that manages the FPGA files
    FPGAHandler *fpgaHandler_;

    //! The instance that manages the IP-Xact library
    LibraryHandler *libraryHandler_;

    //! Contains the open documents as each in it's own tab.
    DrawingBoard* designTabs_;

    QToolBar *toolbar_;

    AWSideToolBar* sideToolBar_;

    QToolBar* rightToolbar_;

//    DesignRightToolBar* designRightToolBar_;

    //! The dock widget handler.
    DockWidgetHandler* dockHandler_;

    //! Create a new document in the IP-Xact library
    QAction* actNew_;

    //! Action to save the current design.
    QAction* actSave_;

    //! Action to save the current design as new object
    QAction* actSaveAs_;

    //! Action to save all designs.
    QAction* actSaveAll_;

    //! The Diagram Tools group.
    QToolBar* viewToolsGroup_;

    //! Action for controlling diagram tools visibility.
    QAction* viewToolsAction_ = nullptr;

    //! Action to zoom in.
    QAction* actZoomIn_;

    //! Action to zoom out.
    QAction* actZoomOut_;

    //! Action to reset the zoom to original 1:1 mapping.
    QAction* actZoomOriginal_;

    //! Action to adjust zoom to fit the document fully in view.
    QAction* actFitInView_;

    //!
//    QAction* actPythonCommunication_;

    //!
//    QAction* actPythonGenScript_;

    //!
    QAction* actManualReload_;

    //!
    QAction* actProjectConfigure_;

    //!
    QAction* actLibraryConfigure_;

#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
    //! The Diagram Tools group.
    QToolBar* generationToolsGroup_;

    //! Action for controlling generation group visibility.
    QAction* generationToolsAction_ = nullptr;

    //! Action group for plugin generators.
    QActionGroup* pluginActionGroup_;


    //! Action to .
    QAction* actVerilogGeneratort_;


#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1
    //! The Diagram Tools group.
    QToolBar* diagramToolsGroup_;

    QActionGroup* modeActionGroup_;


    //! Action for controlling diagram tools visibility.
    QAction* diagramToolsAction_ = nullptr;

    //! Action to select objects in a diagram.
    QAction* actToolSelect_;

    //! Action to draw connections in a diagram.
    QAction* actToolConnect_;

    //! Action to add interfaces in a diagram.
    QAction* actToolInterface_;

    //! Action to drafting.
    QAction* actToolDraft_;

    //! Action to toggle off-page connection in a diagram.
    QAction* actToolToggleOffPage_;

    //! Action to create labels.
    QAction* actToolLabel_;

    //! Action to add a new column to the current diagram.
    QAction* actAddColumn_;

#endif // DIAGRAM_TOOLS_SUPPORT_20240104

    //! Settings for workspaces.
    WorkspaceSettings workspace_;

    MessageMediator* messageChannel_;

signals:
    void inputMessage(QString const&);
    void noticeMessage(QString const&);
    void errorMessage(QString const&);
    void undefinedMessage(QString const&);

private slots:
    void onButtonClicked();

    //! Handler for design widget's clearItemSelection signal.
    void onClearItemSelection();

    //! Handler for signals that inform that design has changed.
    void onDesignChanged();

    //! Opens a dialog for setting project locations.
    void setProjectLocations();

    //! Opens a dialog for setting library locations.
    void setLibraryLocations();

    /*!
     *  Update the instance editor, when the design document is refreshed.
     */
    void onDesignDocumentRefreshed();

    /*!
     *  Adjust the visibility of the selected dock widget.
     *
     *      @param [in] type    Type of the selected dock widget.
     *      @param [in] show    New value for the visibility of the dock widget.
     */
    void onAdjustVisibilityInWindow(TabDocument::SupportedWindows type, bool show);

public slots:
    bool OutputLogNoticeMessage(QByteArray result);
    bool OutputLogErrorMessage(QByteArray result);
    bool runProjectScript(Project const& pri, QString const& library, QString const& projectPath);
    bool runDesignScript(VLNV const& vlnv);
    bool runIOScript(VLNV const& vlnv);
    bool runIPScript(VLNV const& vlnv);

    void onOpenWelcomeWindow();
    /*!
     *  Opens the "New" dialog.
     */
    void createNew();

    /*!
     *  Opens the "New Project" dialog.
     */
    void onCreateNewProject();

    /*!
     *  Opens the "New IO" dialog.
     */
    void OnCreateNewBus();

    /*!
     *  Opens the "New IP" dialog.
     */
    void onCreateNewHWComponent();

    /*!
     *  Opens the "New Design" dialog.
     */
    void onCreateNewDesign();

    /*!
     *  Opens the source file of the given component.
     *
     *      @param [in] compItem The component item.
     */
//    void openCSource(ComponentItem* compItem);

    /*!
     *  Opens the specific C source file of the given component.
     *
     *      @param [in] filename   The name of the file to open.
     *      @param [in] component  The component containing the file.
     */
//    void openCSource(QString const& filename, QSharedPointer<Component> component);

    /*!
     *  Opens a HW design.
     *
     *      @param [in] vlnv           The VLNV of the top-level component.
     *      @param [in] viewName       The name of the view to open.
     */
    void openDesign(VLNV const& vlnv = VLNV(), QString const& viewName = QString());

    /*!
     *  Open component editor to edit a component details.
     *
     *      @param [in] vlnv           Identifies the component to edit.
     */
    void openComponent(const VLNV& vlnv);

    /*!
     *  Open bus to be edited.
     *
     *      @param [in] busDefVLNV      Identifies the bus definition.
     */
    void openBus(const VLNV& busDefVLNV);

    void openAbsDef(const VLNV& absDefVLNV);

    void onOpenBus(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef);
    void onOpenAbsDef(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef);

    /*!
     *  Opens a HW design.
     *
     *      @param [in] vlnv           The VLNV of the top-level component.
     *      @param [in] viewName       The name of the view to open.
     */
    void onOpenDesign_Project(VLNV const& vlnv = VLNV(), QString const& viewName = QString());
    void onOpenDesign_Console(VLNV const& vlnv = VLNV(), QString const& viewName = QString());
    void onRefreshDesign_Console(VLNV const& vlnv = VLNV(), QString const& viewName = QString());
    void onCloseDesign_Console(VLNV const& vlnv = VLNV(), QString const& viewName = QString());
    void onCloseAllDesign_Console();
    void onOpenFile_Console(QString filePath);

    /*!
     *  Open component editor to edit a component details.
     *
     *      @param [in] vlnv           Identifies the component to edit.
     */
    void onOpenComponent_Project(const VLNV& vlnv);

    /*!
     *  Open bus to be edited.
     *
     *      @param [in] busDefVLNV      Identifies the bus definition.
     */
    void onOpenBus_Project(const VLNV& busDefVLNV);

    void onOpenAbsDef_Project(const VLNV& absDefVLNV);

    /*!
     *  Open component editor to edit a component details.
     *
     *      @param [in] vlnv           Identifies the component to edit.
     */
    void onOpenComponent_ProjectLibrary(const VLNV& vlnv);

    /*!
     *  Open bus to be edited.
     *
     *      @param [in] busDefVLNV      Identifies the bus definition.
     */
    void onOpenBus_projectLibrary(const VLNV& busDefVLNV);

    void onOpenAbsDef_projectLibrary(const VLNV& absDefVLNV);

    void onOpenBus_projectLibrary(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef);
    void onOpenAbsDef_projectLibrary(const VLNV& busDefVLNV, const VLNV& absDefVLNV, bool disableBusDef);

    /*!
     *  Open component editor to edit a component details.
     *
     *      @param [in] vlnv           Identifies the component to edit.
     */
    void onCopyVLNV_projectLibrary(const VLNV& vlnv);


    /*!
     *  Open FPGA file.
     *
     *      @param [in] viewName       The file path of the FPGA to open.
     */
    void onOpenFPGA(QString const& filePath);

    //! Called when user closes the last design tab.
    void onLastDocumentClosed();

    //! Called when the user has selected another tab.
    void onDocumentChanged(int index);

    /*!
     *  Search the file system for new IP-Xact projects.
     *
     *  This slot is called when user clicks to search for IP-Xact files.6
     */
    void onProjectLibrarySearch();

    /*!
     *  Search the file system for new IP-Xact projects.
     *
     *  This slot is called when user clicks to search for IP-Xact files.6
     */
    void onProjectSearch();

    /*!
     *  Search the file system for new IP-Xact libraries.
     *
     *  This slot is called when user clicks to search for IP-Xact files.
     */
    void onLibrarySearch();

    /*!
     *  Called when user selects a component on the draw board.
     *
     *      @param [in] component   Pointer to the DiagramComponent instance that is selected.
     */
    void onComponentSelected(ComponentItem* component);

    /*!
     *  Called when user selects an interface on the draw board.
     *
     *      @param [in] interface   Pointer to the interface instance that is selected.
     */
    void onInterfaceSelected(ConnectionEndpoint* interface);

    /*!
     *  Creates a new component to the library.
     *
     *      @param [in] prodHier    The product hierarchy attribute.
     *      @param [in] firmness	The firmness attribute.
     *      @param [in] tags        Document tags.
     *      @param [in] vlnv        The component's VLNV.
     *      @param [in] directory   The directory where to save the component.
     *
     *      @remarks The component editor is opened automatically after successful creation.
     */

    void createComponent(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
        QVector<TagData> tags, VLNV const& vlnv, QString const& directory);

    /*!
     *  Creates a new design.
     *
     *      @param [in] prodHier    The product hierarchy attribute.
     *      @param [in] firmness	The firmness attribute.
     *      @param [in] tags        Document tags.
     *      @param [in] vlnv        The component's VLNV.
     *      @param [in] directory   The directory where to save the component.
     *
     *      @remarks The HW design editor is opened automatically after successful creation.
     */
    void createDesign(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
        QVector<TagData> tags, VLNV const& vlnv, QString const& directory);

    /*!
     *  Creates a new bus definition and abstraction definition
     *
     *      @param [in] vlnv        The vlnv for the bus definition
     *      @param [in] directory   The directory where to save the bus definition and abstraction definition.
     */
    void createBus(VLNV const& vlnv, QString const& directory);

    /*!
     *  Creates a new project
     *
     *      @param [in] Project      The project
     */
    void onCreateProject(Project const& project, QString const& projectLocation);

    /*!
     *  Creates a new IO
     *
     *      @param [in] vlnv        The component's VLNV.
     *      @param [in] directory   The directory where to save the project.
     */
//    void onCreateBus(VLNV const& vlnv, QString const& directory);
    void onCreateBus(VLNV const& vlnv);

    /*!
     *  Creates a new project
     *
     *      @param [in] prodHier    The product hierarchy attribute.
     *      @param [in] firmness	The firmness attribute.
     *      @param [in] tags        Document tags.
     *      @param [in] vlnv        The component's VLNV.
     *      @param [in] directory   The directory where to save the component.
     */
    void onCreateHWComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
                    QVector<TagData>, VLNV const&, QString const&);

    /*!
     *  Creates a new Design
     *
     *      @param [in] vlnv           The vlnv
     *      @param [in] directory   The directory where to save the project.
     */
    void onCreateDesign(VLNV const& vlnv);

    /*!
     *  Creates a new design for an existing component.
     *
     *      @param [in] vlnv        The component's VLNV.
     *
     *      @remarks The HW design editor is opened automatically after successful creation.
     */
    void createDesignForExistingComponent(VLNV const& vlnv);

    void unlockNewlyCreatedDocument(VLNV const& vlnv);

    void saveCurrentDocument();
    void saveCurrentDocumentAs();
    void saveAll();

    void onImportProject();
    void onExportProject();

    /*!
     *  Opens the settings dialog.
     */
    void onOpenSettings();

    /*!
     *  Shows the about box.
     */
    void onShowAbout();

#if DIAGRAM_TOOLS_SUPPORT_20240104 == 1

    //! Called when the draw mode has changed.
    void onDrawModeChanged(DrawMode mode);
    void drawModeChange(QAction *action);

#endif

#if PROTECTION_TOOLS_SUPPORT_20240104 == 1

    //! Called when the state of the protection button changes.
    void onProtectionChanged(bool locked);

    //! Changes the protection of the current document.
    void changeProtection(bool locked);
#endif

    //! Called when the menu strip needs to be updated.
    void updateMenuStrip();

    //! Called when the zoom has changed.
    void onUpdateZoomTools();

    //! Zooms in the current document.
    void onZoomIn();

    //! Zooms out the current document.
    void onZoomOut();

    //! Resets the zoom of the current document to the original 1:1 ratio.
    void onZoomOriginal();

    //! Fits the current document fully in view.
    void onFitInView();

    void onChangeDesignProtection(bool const &locked);


#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1

#if PLUGIN_TOOLS_SUPPORT_20240905 == 1
    /*!
     *  Runs the plugin generator specified by the given action.
     *
     *      @param [in] action The action corresponding to the plugin.
     */
    void runGeneratorPlugin(QAction* action);
#else
    void runGenerator();
#endif // PLUGIN_TOOLS_SUPPORT_20240905

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

    void onOpenSchematic(VLNV const& vlnv);
    void onRefreshSchematic(VLNV const& vlnv);
    void onCloseSchematic(VLNV const& vlnv);
    void onCloseAllSchematic();
    void onOpenFile(QString);


private:
    /*!
     *  Set up the drawing view where hierarchical components are edited.
     */
    void setupDrawBoard();

    void setupVisibilitySetting();
    /*!
     *  Get the project handler from the dock widget handler and connected the necessary signals.
     */
    void setupAndConnectProjectHandler();

    /*!
     *  Get the project handler from the dock widget handler and connected the necessary signals.
     */
    void setupAndConnectProjectLibraryHandler();

    /*!
     *  Get the FPGA handler from the dock widget handler and connected the necessary signals.
     */
    void setupAndConnectFPGAHandler();

    /*!
     *  Get the library handler from the dock widget handler and connected the necessary signals.
     */
    void setupAndConnectLibraryHandler();

    void connectTopMainWorkWindow();

    /*!
     *  Connect the necessary signals of the dock widget handler.
     */
    void connectDockHandler();

    /*!
     *  Checks if the given list of VLVNs contain invalid references.
     *
     *      @param [in] hierRefs            The list of VLNVs to check
     *      @param [in] referencingVlnv     The referencing VLNV.
     *
     *      @return True, if at least one invalid reference is found, otherwise false.
     */
    bool hasInvalidReferences(QList<VLNV> hierRefs, VLNV const& referencingVlnv, LibraryInterface* lh);

    /*!
     *  Check if a document with the given vlnv is already open.
     *
     *      @param [in] vlnv    The vlnv that identifies the document.
     *
     *      @remark If a document with given vlnv is open then the document is shown.
     *
     *      @return True if the document was already open.
     */
    bool isOpen(const VLNV& vlnv) const;

    /*!
     *  Restores the program's settings.
     */
    void restoreSettings();

    /*!
     *  Save the program's settings.
     */
    void saveSettings();

    /*!
     *  Restores the settings for the given workspace.
     *
     *      @param [in] workspaceName The name of the workspace.
     */
    void loadWorkspace(QString const& workspaceName);

    /*!
     *  Updates the workspace menu.
     */
//    void updateWorkspaceMenu();

    /*!
     *  Set up the actions in the tool bars
     */
    void setupActions();

    /*!
     *  Set up the menus for the main window.
     *
     *  This function is used by the setupActions() after it has created the actions for the menus and should not
     *  be called directly.
     */
    void setupMenus();

    void setupToolbars();

#if GENERATORPLUGIN_TOOLS_SUPPORT_20240618 == 1
    /*!
     *  Adds generator plugin actions to the generation group.
     */
    void createGeneratorPluginActions();

    /*!
     *  Updates the generation group.
     */
    void updateGeneratorPluginActions();

#endif // GENERATORPLUGIN_TOOLS_SUPPORT_20240618

    /*!
     *  Update the windows menu to contain the supported windows and visibility of the windows.
     */
    void updateWindows();

    /*!
     *  Connects all the visibility controls for windows.
     */
    void connectVisibilityControls();

    /*!
     *  Disconnects all the visibility controls for windows.
     */
    void disconnectVisibilityControls();

    /*!
     *  Runs the component editor for the given component.
     *
     *      @param [in] component   The component to run the wizard for.
     *      @param [in] directory   The directory containing the component xml file.
     */
    void runComponentWizard(QSharedPointer<Component> component, QString const& directory);

    /*!
     *  Set the visibilities for the plug ins.
     */
    void setPluginVisibilities();

protected:

    //! Called when the user requests to close the program.
    virtual void closeEvent(QCloseEvent* event);

    //! Called when user i.e minimizes the main window.
    virtual void hideEvent(QHideEvent* event);

    //! Called when user i.e maximizes the main window after it has been minimized.
    virtual void showEvent(QShowEvent* event);



};
#endif // MAINWINDOW_H
