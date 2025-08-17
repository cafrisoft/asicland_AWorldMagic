//-----------------------------------------------------------------------------
// File: LibraryWidget.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 13.05.2017
//
// Description:
// Widget for holding all IP-XACT library widgets.
//-----------------------------------------------------------------------------


#include <UI/LibraryErrorModel.h>

#include <KactusAPI/ProjectHandler.h>
#include <KactusAPI/LibraryHandler.h>
#include <KactusAPI/hierarchymodel.h>

#include <UI/LibraryItemSelectionFactory.h>

#if DIALERWIDGET_SUPPORT_20231229 == 1
#include <UI/Widgets/VLNVDialer/vlnvdialer.h>
#endif

#include <UI/Dialogs/newObjectDialog/newobjectdialog.h>
#include <UI/Dialogs/ObjectSelectionDialog/ObjectSelectionListItem.h>
#include <UI/Dialogs/ObjectRemoveDialog/ObjectRemoveDialog.h>

#include <UI/ProjectTreeExWidget/ProjectTreeExWidget.h>
#include <UI/LibraryTreeWidget/LibraryTreeWidget.h>

#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
#include <UI/HierarchyWidget/hierarchywidget.h>
#endif

#include <UI/GraphicalMessageMediator/GraphicalMessageMediator.h>

#include "LibraryWidget.h"


#include <QTabWidget>
#include <QVBoxLayout>
#include <QSplitter>


//-----------------------------------------------------------------------------
// Function: LibraryWidget::LibraryWidget()
//-----------------------------------------------------------------------------
//LibraryWidget::LibraryWidget(LibraryHandler* library, MessageMediator* messageChannel, QWidget *parent): QWidget(parent),
LibraryWidget::LibraryWidget(ProjectHandler* project, LibraryHandler* library, QWidget *parent): QWidget(parent),
#if DIALERWIDGET_SUPPORT_20231229 == 1
    dialer_(new VLNVDialer(this)),
#endif
    project_(project),
    library_(library),
    projectExporter_(library_, this, this),
    projectImporter_(library_, this, this),
    itemExporter_(library_, this, this),
#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
    hierarchyWidget_(new HierarchyWidget(library_, library_->getHierarchyModel(), this)),
#endif
    projectWidget_(new ProjectTreeExWidget(project_, project_->getModel(), this)),
//    projectWidget_(new ProjectTreeWidget(library_, library_->getTreeModel(), this)),
    libraryTreeWidget_(new LibraryTreeWidget(library_, library_->getTreeModel(), this)),
    previewWidget_(new ComponentPreviewBox(library, this)),
#if PREVIEW_HIDE_SUPPORT_20231229 == 1
    previewHideButton_(new QPushButton(QString(), this)),
#endif
    integrityWidget_(nullptr),
    hidePreview_(false)
{
//    GraphicalMessageMediator* guiChannel = dynamic_cast<GraphicalMessageMediator*>(messageChannel);
//    if (guiChannel)
//    {
//        connect(guiChannel, SIGNAL(statusMessage(QString const&)),
//            this, SIGNAL(statusMessage(QString const&)));
//    }

#if DIALERWIDGET_SUPPORT_20231229 == 1
    dialer_->setRootItem(library_->getTreeRoot());
#endif

#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
    connectLibraryFilter(hierarchyWidget_->getFilter());
#endif

    connectLibraryFilter(libraryTreeWidget_->getFilter());

#if PREVIEW_HIDE_SUPPORT_20231229 == 1
    QSettings settings;
    hidePreview_ = !settings.value("PreviewWidget/Hidden", true).toBool();

    previewHideButton_->setFlat(true);
    previewHideButton_->setToolTip(tr("Show preview"));

    onPreviewShowHideClick();
#endif

#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
    connect(hierarchyWidget_, SIGNAL(componentSelected(const VLNV&)),
        library_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(library_->getHierarchyModel(), SIGNAL(showErrors(const VLNV)),
        this, SLOT(onShowErrors(const VLNV)), Qt::UniqueConnection);
#endif

    connect(projectWidget_, SIGNAL(itemSelected(const VLNV&)),
        library_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(libraryTreeWidget_, SIGNAL(itemSelected(const VLNV&)),
        library_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    auto projectTreeModel = project_->getModel();

    connect(projectTreeModel, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(showErrors(const VLNV)),
        this, SLOT(onShowErrors(const VLNV)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(createBus(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createComDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createApiDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createComponent(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createAbsDef(const VLNV&)),
        this, SLOT(onCreateAbsDef(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createDesign(const VLNV&)),
        this, SLOT(onCreateDesign(const VLNV&)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(createSWDesign(const VLNV&)),
        this, SIGNAL(createSWDesign(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createSystemDesign(const VLNV&)),
        this, SIGNAL(createSystemDesign(const VLNV&)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(removeVLNV(const QList<VLNV>)),
        this, SLOT(onRemoveVLNV(const QList<VLNV>)), Qt::UniqueConnection);

//    connect(projectTreeModel, SIGNAL(exportItems(const QString&, const QList<VLNV>)),
//       &projectExporter_, SLOT(onExportItems(const QString&, const QList<VLNV>)), Qt::UniqueConnection);

    connect(projectWidget_, SIGNAL(exportItem()), &projectExporter_, SLOT(onExportItem()), Qt::UniqueConnection);
    connect(projectWidget_, SIGNAL(importItem()), &projectImporter_, SLOT(onImportItem()), Qt::UniqueConnection);

    auto treeModel = library_->getTreeModel();

#if DIALERWIDGET_SUPPORT_20231229 == 1
    connect(treeModel, SIGNAL(refreshDialer()), dialer_, SLOT(refreshLibrary()), Qt::UniqueConnection);
#endif

    connect(treeModel, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);

    connect(treeModel, SIGNAL(showErrors(const VLNV)),
        this, SLOT(onShowErrors(const VLNV)), Qt::UniqueConnection);

    connect(treeModel, SIGNAL(createBus(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(createComDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(createApiDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(createComponent(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(createAbsDef(const VLNV&)),
        this, SLOT(onCreateAbsDef(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(createDesign(const VLNV&)),
        this, SLOT(onCreateDesign(const VLNV&)), Qt::UniqueConnection);

    connect(treeModel, SIGNAL(createSWDesign(const VLNV&)),
        this, SIGNAL(createSWDesign(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(createSystemDesign(const VLNV&)),
        this, SIGNAL(createSystemDesign(const VLNV&)), Qt::UniqueConnection);

    connect(treeModel, SIGNAL(removeVLNV(const QList<VLNV>)),
        this, SLOT(onRemoveVLNV(const QList<VLNV>)), Qt::UniqueConnection);
    connect(treeModel, SIGNAL(exportItems(const QList<VLNV>)),
       &itemExporter_, SLOT(onExportItems(const QList<VLNV>)), Qt::UniqueConnection);

    auto hierarchyModel = library_->getHierarchyModel();
    connect(hierarchyModel, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(removeVLNV(QList<VLNV>)),
        this, SLOT(onRemoveVLNV(QList<VLNV>)), Qt::UniqueConnection);

    connect(hierarchyModel, SIGNAL(createBusDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createComponent(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createBus(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createComDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createApiDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createAbsDef(const VLNV&)),
        this, SLOT(onCreateAbsDef(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createDesign(const VLNV&)),
        this, SLOT(onCreateDesign(const VLNV&)), Qt::UniqueConnection);

    connect(hierarchyModel, SIGNAL(createSWDesign(const VLNV&)),
        this, SIGNAL(createSWDesign(const VLNV&)), Qt::UniqueConnection);
    connect(hierarchyModel, SIGNAL(createSystemDesign(const VLNV&)),
        this, SIGNAL(createSystemDesign(const VLNV&)), Qt::UniqueConnection);

    connect(hierarchyModel, SIGNAL(exportItem(VLNV const&)),
         &itemExporter_, SLOT(onExportItem(VLNV const&)), Qt::UniqueConnection);

     connect(&itemExporter_, SIGNAL(noticeMessage(const QString&)),
          this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);

     connect(&itemExporter_, SIGNAL(errorMessage(const QString&)),
         this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

     connect(&projectImporter_, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
     connect(&projectImporter_, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

     connect(&projectExporter_, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
     connect(&projectExporter_, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

     connect(&itemExporter_, SIGNAL(projectSearch()), this, SIGNAL(projectSearch()), Qt::UniqueConnection);

     connect(&projectImporter_, SIGNAL(projectSearch()), this, SIGNAL(projectSearch()), Qt::UniqueConnection);

 //    connect(projectImporter_, SIGNAL(projectSearch()), &projectExporter_, SLOT(onExportItem()), Qt::UniqueC\onnection);



     connect(library_, SIGNAL(itemSelected(const VLNV&)),
         previewWidget_, SLOT(setComponent(const VLNV&)), Qt::UniqueConnection);

#if PREVIEW_HIDE_SUPPORT_20231229 == 1
     connect(previewHideButton_, SIGNAL(clicked(bool)),
         this, SLOT(onPreviewShowHideClick()), Qt::UniqueConnection);
#endif


    setupLayout();
} 

//-----------------------------------------------------------------------------
// Function: LibraryWidget::getLibraryHandler()
//-----------------------------------------------------------------------------
LibraryHandler* LibraryWidget::getLibraryHandler() const
{
    return library_;
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::loadFilterSettings()
//-----------------------------------------------------------------------------
void LibraryWidget::loadFilterSettings(QSettings& settings)
{
#if DIALERWIDGET_SUPPORT_20231229 == 1
    dialer_->loadFilterSettings(settings);
#endif
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::saveFilterSettings()
//-----------------------------------------------------------------------------
void LibraryWidget::saveFilterSettings(QSettings& settings) const
{
#if DIALERWIDGET_SUPPORT_20231229 == 1
    dialer_->saveFilterSettings(settings);
#endif
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::selectComponent()
//-----------------------------------------------------------------------------
void LibraryWidget::selectComponent(VLNV const& componentVLNV) const
{
    projectWidget_->selectItem(componentVLNV);

    libraryTreeWidget_->selectItem(componentVLNV);

#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
    hierarchyWidget_->selectItems(componentVLNV);
#endif

    previewWidget_->setComponent(componentVLNV);
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onShowErrors()
//-----------------------------------------------------------------------------
void LibraryWidget::onShowErrors(VLNV const& vlnv)
{
    if (vlnv.isValid() == false || library_->contains(vlnv) == false)
    {
        return;
    }

    QSharedPointer<Document> document = library_->getModel(vlnv);

    // Show error list in a dialog.
    TableViewDialog* dialog = new TableViewDialog(this);
    dialog->setWindowTitle(tr("Errors in %1").arg(vlnv.toString()));
    dialog->setDescription(tr("<b>Integrity check</b><br>The following errors were found."));
    dialog->resize(700, 350);

    LibraryErrorModel* model = new LibraryErrorModel(dialog);
    model->addErrors(library_->findErrorsInDocument(document, library_->getPath(vlnv)), vlnv.toString());

    dialog->show();

    dialog->setModel(model);

    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onCreateNewItem()
//-----------------------------------------------------------------------------
void LibraryWidget::onCreateNewItem(VLNV const& vlnv)
{
    VLNV::IPXactType documentType = vlnv.getType();

    bool showAttributes = (documentType == VLNV::COMPONENT);
    NewObjectDialog newDesignDialog(library_, vlnv.getType(), showAttributes, this);

    if (library_->contains(vlnv) && library_->getDocumentType(vlnv) == VLNV::COMPONENT)
    {
        QSharedPointer<const Document> document = library_->getModelReadOnly(vlnv);
        QSharedPointer<const Component> component = document.staticCast<const Component>();

        KactusAttribute::ProductHierarchy prodHier = component->getHierarchy();
        KactusAttribute::Firmness firmness = component->getFirmness();
        QVector<TagData> tags = component->getTags();

        newDesignDialog.setKactusAttributes(prodHier, firmness, tags);
    }

    newDesignDialog.setVLNV(vlnv);

    if (documentType == VLNV::ABSTRACTIONDEFINITION || documentType == VLNV::BUSDEFINITION)
    {
        newDesignDialog.setWindowTitle("New Bus");
    }
    else if (documentType == VLNV::COMPONENT)
    {
        newDesignDialog.setWindowTitle("New Component");
    }
    else if (documentType == VLNV::APIDEFINITION)
    {
        newDesignDialog.setWindowTitle(tr("New API Definition"));
    }
    else if (documentType == VLNV::COMDEFINITION)
    {
        newDesignDialog.setWindowTitle(tr("New COM Definition"));
    }

    newDesignDialog.exec();

    if (newDesignDialog.result() == QDialog::Rejected)
    {
        return;
    }

    VLNV newVlnv = newDesignDialog.getVLNV();
    QString directory = newDesignDialog.getPath();

    // create an object of correct type
    if (documentType == VLNV::ABSTRACTIONDEFINITION || documentType == VLNV::BUSDEFINITION)
    {
        emit createBus(newVlnv, directory);
    }
    else if (documentType == VLNV::COMPONENT)
    {
        emit createComponent(newDesignDialog.getProductHierarchy(), newDesignDialog.getFirmness(),
            newDesignDialog.getTags(), newVlnv, directory);
    }
    else if (documentType == VLNV::APIDEFINITION)
    {
        emit createApiDef(newVlnv, directory);
    }
    else if (documentType == VLNV::COMDEFINITION)
    {
        emit createComDef(newVlnv, directory);
    }
    else
    {
        emit noticeMessage(tr("The item type is not supported"));
    }
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onCreateAbsDef()
//-----------------------------------------------------------------------------
void LibraryWidget::onCreateAbsDef(VLNV const& busDefVLNV)
{
    if (busDefVLNV.isValid() == false)
    {
        return;
    }

    Q_ASSERT(library_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION);
    Q_ASSERT(library_->contains(busDefVLNV));

    QFileInfo busDefInfo(library_->getPath(busDefVLNV));
    QString directory = busDefInfo.absolutePath();

    emit createAbsDef(busDefVLNV, directory);
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onCreateDesign()
//-----------------------------------------------------------------------------
void LibraryWidget::onCreateDesign(VLNV const& vlnv)
{
    emit createDesignForExistingComponent(vlnv);
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onGenerateIntegrityReport()
//-----------------------------------------------------------------------------
void LibraryWidget::onGenerateIntegrityReport()
{
    if (!integrityWidget_)
     {
         integrityWidget_ = new TableViewDialog(this);
         integrityWidget_->setWindowTitle(tr("Library Integrity Report"));
         integrityWidget_->setDescription(tr("<b>Integrity check</b><br>The following errors were found."));
         integrityWidget_->resize(1000, 800);

         LibraryErrorModel* model = new LibraryErrorModel(integrityWidget_);

         for (auto const& vlnv : library_->getAllVLNVs())
         {
             if (library_->isValid(vlnv) == false)
             {
                 model->addErrors(library_->findErrorsInDocument(library_->getModel(vlnv), library_->getPath(vlnv)), vlnv.toString());
             }
         }

         integrityWidget_->setModel(model);

         integrityWidget_->show();

         connect(integrityWidget_, SIGNAL(finished(int)), this, SLOT(onCloseIntegrityReport()));
     }

     integrityWidget_->raise();
}
//-----------------------------------------------------------------------------
// Function: LibraryWidget::closeIntegrityReport()
//-----------------------------------------------------------------------------
void LibraryWidget::onCloseIntegrityReport()
{
      integrityWidget_->deleteLater();
      integrityWidget_ = 0;
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onRemoveVLNV()
//-----------------------------------------------------------------------------
void LibraryWidget::onRemoveVLNV(const QList<VLNV> vlnvs)
{
    // create the dialog to select which items to remove
    ObjectRemoveDialog removeDialog(this);
    LibraryItemSelectionFactory::constructItemsForSelectionDialog(library_, &removeDialog, vlnvs);

    if (removeDialog.exec() == QDialog::Rejected)
    {
        return;
    }

    QList<VLNV> removedVLNVs;
    QStringList removedFilePaths;
    for (ObjectSelectionListItem const* removedItem : removeDialog.getSelectedItems())
    {
        if (removedItem->getType() == ObjectSelectionListItem::VLNVOJBECT)
        {
            removedVLNVs.append(removedItem->getVLNV());
        }
        else if (removedItem->getType() == ObjectSelectionListItem::FILE)
        {
            QFileInfo fileInfo(removedItem->getPath());
            if (fileInfo.exists())
            {
                removedFilePaths.append(removedItem->getPath());
            }
        }
    }

    library_->removeObjects(removedVLNVs);

    for (auto const& path : removedFilePaths)
    {
        QFile file(path);
        if (file.remove() == false)
        {
//           emit errorMessage(tr("File %1 could not be removed from the file system.").arg(path));
        }
    }
}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::onPreviewHideShowClick()
//-----------------------------------------------------------------------------
void LibraryWidget::onPreviewShowHideClick()
{
#if PREVIEW_HIDE_SUPPORT_20231229 == 1
    hidePreview_ = !hidePreview_;
    previewWidget_->setHidden(hidePreview_);

    if (hidePreview_)
    {
        previewHideButton_->setToolTip(tr("Show preview"));
        previewHideButton_->setIcon(QIcon(":resources/Res/preview.png"));
    }
    else
    {
        previewHideButton_->setToolTip(tr("Hide preview"));
        previewHideButton_->setIcon(QIcon(":resources/Res/hide.png"));
    }

    QSettings settings;
    settings.setValue("PreviewWidget/Hidden", hidePreview_);
#endif

}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::connectVLNVFilter()
//-----------------------------------------------------------------------------
void LibraryWidget::connectLibraryFilter(LibraryFilter* filter) const
{
#if DIALERWIDGET_SUPPORT_20231229 == 1
    connect(dialer_, SIGNAL(vendorChanged(const QString&)),
        filter, SLOT(onVendorChanged(const QString&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(libraryChanged(const QString&)),
        filter, SLOT(onLibraryChanged(const QString&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(nameChanged(const QString&)),
        filter, SLOT(onNameChanged(const QString&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(versionChanged(const QString&)),
        filter, SLOT(onVersionChanged(const QString&)), Qt::UniqueConnection);

    connect(dialer_, SIGNAL(firmnessChanged(const Utils::FirmnessOptions&)),
        filter, SLOT(onFirmnessChanged(const Utils::FirmnessOptions&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(implementationChanged(const Utils::ImplementationOptions&)),
        filter, SLOT(onImplementationChanged(const Utils::ImplementationOptions&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(hierarchyChanged(const Utils::HierarchyOptions&)),
        filter, SLOT(onHierarchyChanged(const Utils::HierarchyOptions&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(typeChanged(const Utils::TypeOptions&)),
        filter, SLOT(onTypeChanged(const Utils::TypeOptions&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(filtersChanged(Utils::FilterOptions const&)),
        filter, SLOT(onFiltersChanged(Utils::FilterOptions const&)), Qt::UniqueConnection);
    connect(dialer_, SIGNAL(tagFiltersChanged(QVector<TagData>)),
        filter, SLOT(onTagFilterChanged(QVector<TagData>)), Qt::UniqueConnection);
#endif
}

void LibraryWidget::onChangeProject()
{

    QString companyPath = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

    QDir companyDirectory(companyPath);
    QString companyName = companyDirectory.dirName(); // 디렉토리명


}

//-----------------------------------------------------------------------------
// Function: LibraryWidget::setupLayout()
//-----------------------------------------------------------------------------
void LibraryWidget::setupLayout()
{
    QString companyPath = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

    QWidget* projectGroup = new QWidget(this);
    QWidget* libraryGroup = new QWidget(this);

    QTabWidget* projectTabs = new QTabWidget(this);
    projectTabs->addTab(projectWidget_, tr("Project"));

//    QDir companyDirectory(companyPath);
//    QString companyName = companyDirectory.dirName(); // 디렉토리명

//    projectTabs->addTab(projectWidget_, companyName);

    QTabWidget* navigationTabs = new QTabWidget(this);
//    navigationTabs->addTab(libraryTreeWidget_, tr("VLNV Tree"));
    navigationTabs->addTab(libraryTreeWidget_, tr("Library"));

#if HIERARCHYWIDGET_SUPPORT_20231229 == 1
    navigationTabs->addTab(hierarchyWidget_, tr("Hierarchy"));
#endif

    auto projectLayout = new QVBoxLayout(projectGroup);
    projectLayout->addWidget(projectTabs, 1);

    auto libraryLayout = new QVBoxLayout(libraryGroup);

    libraryLayout->addWidget(navigationTabs, 1);
//    libraryLayout->addWidget(navigationTabs);

#if DIALERWIDGET_SUPPORT_20231229 == 1
    libraryLayout->addWidget(dialer_);
#endif

    libraryLayout->setContentsMargins(0, 0, 0, 0);

//    dialer_->setVisible(true);

    QWidget* previewGroup = new QWidget(this);

    auto previewLayout = new QGridLayout(previewGroup);

//    QLabel* previewLabel = new QLabel(tr("Component Preview"), this);
    QLabel* previewLabel = new QLabel(tr("Preview"), this);

    previewLayout->addWidget(previewLabel, 0, 0, 1, 1);

#if PREVIEW_HIDE_SUPPORT_20231229 == 1
    previewLayout->addWidget(previewHideButton_, 0, 1, 1, 1, Qt::AlignRight);
#endif

    previewLayout->addWidget(previewWidget_, 1, 0, 1, 2);
    previewLayout->setContentsMargins(4, 0, 4, 0);

    QSplitter* viewSplit = new QSplitter(this);
    viewSplit->setOrientation(Qt::Vertical);
    viewSplit->addWidget(projectGroup);
    viewSplit->addWidget(libraryGroup);
    viewSplit->addWidget(previewGroup);
    viewSplit->setStretchFactor(0, 4);
    viewSplit->setContentsMargins(0, 0, 0, 0);

    QSplitterHandle* handle = viewSplit->handle(1);
    QVBoxLayout* handleLayout = new QVBoxLayout(handle);
    handleLayout->setSpacing(0);
    handleLayout->setContentsMargins(2, 0, 0, 0);

    QFrame* line = new QFrame(handle);
    line->setLineWidth(2);
    line->setMidLineWidth(2);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    handleLayout->addWidget(line);

    viewSplit->setHandleWidth(10);

    auto containerLayout = new QVBoxLayout(this);
    containerLayout->addWidget(viewSplit, 1);
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(0, 0, 0, 0);
}
