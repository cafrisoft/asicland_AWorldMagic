//-----------------------------------------------------------------------------
// File: AWMWidget.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 04.06.2024
//
// Description:
// Widget for Project & IP-XACT library widgets.
//-----------------------------------------------------------------------------

#ifndef AWMWIDGET_H
#define AWMWIDGET_H

#include <QWidget>

#include <KactusAPI/utils.h>

#include <IPXACTmodels/common/VLNV.h>
#include <IPXACTmodels/common/TagData.h>
#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>

#include <UI/Widgets/componentPreviewBox/ComponentPreviewBox.h>

#include <UI/Dialogs/TableViewDialog/TableViewDialog.h>

#include <UI/ProjectExporter.h>
#include <UI/ProjectImporter.h>
#include <UI/ItemExporter.h>

#include <QSettings>

class ProjectHandler;
class ProjectLibraryHandler;
class FPGAHandler;
class LibraryHandler;
class LibraryFilter;
class ProjectTreeExWidget;
class ProjectLibraryTreeWidget;
class FPGATreeWidget;
class LibraryTreeWidget;
class MessageMediator;

//-----------------------------------------------------------------------------
//! Widget for holding all IP-XACT library widgets.
//-----------------------------------------------------------------------------
class AWMWidget : public QWidget
{
    Q_OBJECT
public:

    /*! The constructor.
     *
     *      @param [in] library    The IP-XACT library interface.
     *      @param [in] parent     The parent widget.
    */
    explicit AWMWidget(ProjectHandler* project,
                       ProjectLibraryHandler* projectLibrary,
                       FPGAHandler* fpga,
                       LibraryHandler* library,
                       QWidget* parent = 0);

    //! The destructor.
    virtual ~AWMWidget() = default;

    /*! Gets the handle for library access.
     *
     *      @return     The library access handle.
    */
    LibraryHandler* getLibraryHandler() const;

    /*!
     *  Selects the given VLNV in all library views.
     *
     *      @param [in] componentVLNV   The VLVN to select.
     */
    void selectComponent(VLNV const& componentVLNV) const;

    /*!
     *  Selects the given VLNV in all library views.
     *
     *      @param [in] componentVLNV   The VLVN to select.
     */
    void selectComponent(LibraryInterface* lh, VLNV const& componentVLNV) const;

    void onImportProject_Console(QString const& fileDest, QString const& filePath, QString const& fileName);
    void onExportProject_Console(QString const& fileDest, QString const& filePath, QString const& fileName);

signals:
    void inputMessage(QString const&);
    void noticeMessage(QString const&);
    void errorMessage(QString const&);
    void undefinedMessage(QString const&);

    //! Signal that user wants to create a new abstraction definition for given bus definition.
    void createAbsDef(VLNV const& busDefVLNV, const QString& directory);

    //! Signal that user wants to create a new bus with given vlnv
    void createBus(VLNV const& vlnv, const QString& directory);
    
    //! Signal that user wants to create a new component with given vlnv
    void createComponent(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness, QVector<TagData> tags,
        VLNV const& vlnv, const QString& directory);

    //! Signal that user wants to create a new design for the given component.
    void createDesignForExistingComponent(VLNV const& vlnv);

    //! Signal that user wants to create a new API definition with given vlnv
    void createApiDef(VLNV const& vlnv, const QString& directory);

    //! Signal that user wants to create a new COM definition with given vlnv
    void createComDef(VLNV const& vlnv, const QString& directory);

    //! Signal that user wants to create a new SW design for the given component.
    void createSWDesign(VLNV const& vlnv);

    //! Signal that user wants to create a new system design for the given component.
    void createSystemDesign(VLNV const& vlnv);

    void statusMessage(QString const& message);

    void importItem();
    void exportItem();

    void importProject();
    void exportProject();

    //! Emitted when a library item identifying single vlnv is selected.
    void previewSelected(LibraryInterface* lh, const VLNV& vlnv);

    //! Create new I/O
    void createNewBus();

    //! Create new IP
    void createNewHWComponent();

    void projectSearch();

public slots:

    /*!
     *  Shows errors about the library item with the given VLNV.
     *
     *      @param [in] vlnv The VLNV of the library item.
     */
    void onShowErrors(VLNV const& vlnv);

    /*! Create a new item for given vlnv.
 *
 * The type of the vlnv defines the type of the object to create.
 * After creating the object an editor for it is opened.
 *
 *      @param [in] vlnv Identifies the object to create
 *
*/
    void onCreateNewItem(VLNV const& vlnv);

    /*! Create a new abstraction definition for given bus definition.
 *
 *      @param [in] busDefVLNV Identifies the bus definition to create the abs def for.
 *
*/
//    void onCreateAbsDef_Design(VLNV const& busDefVLNV);
    void onCreateAbsDef_ProjectLibrary(VLNV const& busDefVLNV);
    void onCreateAbsDef_Library(VLNV const& busDefVLNV);

    /*! Create new design with given vlnv.
     *
     *      @param [in] vlnv The vlnv that identifies
//    navigationTabs->addTab(treeWidget_, tr("VLNV Tree")); the design.
     *
    */
    void onCreateDesign(VLNV const& vlnv);

    //!  Shows a report of all errors within the library items.
    void onGenerateIntegrityReport();

    void onCloseIntegrityReport();

    void onRemoveVLNV(const QList<VLNV>);

    void onChangeProject();

    void onImportProject();
    void onExportProject();

    /*!
     * Sets the component that will be visualized.
     *
     *		@param [in] vlnv The vlnv of the component. If vlnv does not belong
     *		to component then the preview box is cleared.
     */
    void setComponent_Design(const VLNV& vlnv);

    /*!
     * Sets the component that will be visualized.
     *
     *		@param [in] vlnv The vlnv of the component. If vlnv does not belong
     *		to component then the preview box is cleared.
     */
    void setComponent_ProjectLibrary(const VLNV& vlnv);

    /*!
     * Sets the component that will be visualized.
     *
     *		@param [in] vlnv The vlnv of the component. If vlnv does not belong
     *		to component then the preview box is cleared.
     */
    void setComponent_Library(const VLNV& vlnv);

private:
    //! No copying
    AWMWidget(const AWMWidget& other);

    //! No assignment
    AWMWidget& operator=(const AWMWidget& other);

    //! Sets the widget layout.
    void setupLayout();

    //-----------------------------------------------------------------------------
    //! Data
    //-----------------------------------------------------------------------------
    //! The instance to manage desgin access.
    ProjectHandler* project_;

    //! The instance to manage project library access.
    ProjectLibraryHandler *projectLibraryHandler_;

    //! The instance to manage FPGA files access.
    FPGAHandler *fpgaHandler_;

    //! The instance to manage library access.
    LibraryHandler* libraryHandler_;

    ProjectExporter projectExporter_;

    ProjectImporter projectImporter_;

    ItemExporter itemExporter_;

    //! The widget containing the design items in a tree-like view.
    ProjectTreeExWidget* projectTreeWidget_;

    //! The widget containing the project library items in a tree-like view.
    ProjectLibraryTreeWidget* projectLibraryTreeWidget_;

    //! The widget containing the project library items in a tree-like view.
    FPGATreeWidget* fpgaTreeWidget_;

    //! The widget containing the library items in a tree-like view.
    LibraryTreeWidget* libraryTreeWidget_;

    //! The widget that contains the component preview.
    ComponentPreviewBox* previewWidget_;

    //! Widget for showing integrity report on-demand.
    TableViewDialog* integrityWidget_;

    bool hidePreview_;

};

#endif // AWMWIDGET_H
