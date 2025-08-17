//-----------------------------------------------------------------------------
// File: LibraryWidget.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 13.05.2017
//
// Description:
// Widget for holding all IP-XACT library widgets.
//-----------------------------------------------------------------------------

#ifndef LIBRARYWIDGET_H
#define LIBRARYWIDGET_H

#include <QWidget>
#include <QStatusBar>
#include <QProgressBar>

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
class LibraryHandler;
#if DIALERWIDGET_SUPPORT_20231229 == 1
class VLNVDialer;
#endif
#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
class HierarchyWidget;
#endif
class LibraryFilter;
class ProjectTreeExWidget;
class LibraryTreeWidget;
class MessageMediator;

//-----------------------------------------------------------------------------
//! Widget for holding all IP-XACT library widgets.
//-----------------------------------------------------------------------------
class LibraryWidget : public QWidget
{
    Q_OBJECT
public:

    /*! The constructor.
     *
     *      @param [in] library    The IP-XACT library interface.
     *      @param [in] parent     The parent widget.
    */
//    LibraryWidget(LibraryHandler* library, MessageMediator* messageChannel, QWidget* parent = 0);
    explicit LibraryWidget(ProjectHandler* project, LibraryHandler* library, QWidget* parent = 0);

    //! The destructor.
    virtual ~LibraryWidget() = default;

    /*! Gets the handle for library access.
     *
     *      @return     The library access handle.
    */
    LibraryHandler* getLibraryHandler() const;
    
    /*!
     *  Set filter settings for the library.
     *
     *      @param [in] settings    Settings containing the library filters.
     */
    void loadFilterSettings(QSettings& settings);

    /*!
     *  Save settings for dock widget filters.
     *
     *      @param [in] settings    The settings.
     */
    void saveFilterSettings(QSettings& settings) const;

    /*!
     *  Selects the given VLNV in all library views.
     *
     *      @param [in] componentVLNV   The VLVN to select.
     */
    void selectComponent(VLNV const& componentVLNV) const;

signals:

    //! Emit a notice message to be printed to user.
    void noticeMessage(const QString& message);

    //! Emit an error message to be printed to user.
    void errorMessage(const QString& message);

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

    //! Import the selected item and it's sub-items.
//    void importItem();

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
    void onCreateAbsDef(VLNV const& busDefVLNV);

    /*! Create new design with given vlnv.
     *
     *      @param [in] vlnv The vlnv that identifies the design.
     *
    */
    void onCreateDesign(VLNV const& vlnv);

    //!  Shows a report of all errors within the library items.
    void onGenerateIntegrityReport();

    void onCloseIntegrityReport();

    void onRemoveVLNV(const QList<VLNV>);

    //! Hides/shows the component preview.
    void onPreviewShowHideClick();

    void onChangeProject();

private:
    //! No copying
    LibraryWidget(const LibraryWidget& other);

    //! No assignment
    LibraryWidget& operator=(const LibraryWidget& other);

    /*! Connects the given filter to the filtering widget.
     *
     *      @param [in} filter     The filter to connect.
    */
    void connectLibraryFilter(LibraryFilter* filter) const;

    //! Sets the widget layout.
    void setupLayout();

    //-----------------------------------------------------------------------------
    //! Data
    //-----------------------------------------------------------------------------

#if DIALERWIDGET_SUPPORT_20231229 == 1
    //! Widget with filtering options for the library.
    VLNVDialer* dialer_ ;
#endif

    //! The instance to manage library access.
    ProjectHandler* project_;

    //! The instance to manage library access.
    LibraryHandler* library_;

    ProjectExporter projectExporter_;

    ProjectImporter projectImporter_;

    ItemExporter itemExporter_;

#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
    //! The widget that contains the GUI items for the hierarchy tab.
    HierarchyWidget* hierarchyWidget_;
#endif

    //! The widget containing the library items in a tree-like view.
    ProjectTreeExWidget* projectWidget_;

    //! The widget containing the library items in a tree-like view.
    LibraryTreeWidget* libraryTreeWidget_;

    //! The widget that contains the component preview.
    ComponentPreviewBox* previewWidget_;

#if PREVIEW_HIDE_SUPPORT_20231229 == 1
    //! Button for hiding/showing component preview.
    QPushButton* previewHideButton_;
#endif

    //! Widget for showing integrity report on-demand.
    TableViewDialog* integrityWidget_;

    bool hidePreview_;

};

#endif // LIBRARYWIDGET_H
