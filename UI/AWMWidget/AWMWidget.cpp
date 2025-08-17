//-----------------------------------------------------------------------------
// File: AWMWidget.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 04.06.2024
//
// Description:
// Widget for Project & IP-XACT library widgets.
//-----------------------------------------------------------------------------

#include <QTabWidget>
#include <QVBoxLayout>
#include <QSplitter>

#include <AWorldMagicAPI/ProjectLibraryHandler.h>
#include <AWorldMagicAPI/FPGAHandler.h>

#include <KactusAPI/ProjectHandler.h>
#include <KactusAPI/LibraryHandler.h>
#include <KactusAPI/hierarchymodel.h>

#include <UI/LibraryErrorModel.h>

#include <UI/LibraryItemSelectionFactory.h>

#include <UI/Dialogs/newObjectDialog/newobjectdialog.h>
#include <UI/Dialogs/ObjectSelectionDialog/ObjectSelectionListItem.h>
#include <UI/Dialogs/ObjectRemoveDialog/ObjectRemoveDialog.h>

#include <UI/ProjectTreeExWidget/ProjectTreeExWidget.h>
#include <UI/ProjectLibraryTreeWidget/ProjectLibraryTreeWidget.h>
#include <UI/FPGATreeWidget/FPGATreeWidget.h>
#include <UI/LibraryTreeWidget/LibraryTreeWidget.h>

#include <UI/GraphicalMessageMediator/GraphicalMessageMediator.h>

#include "AWMWidget.h"


//-----------------------------------------------------------------------------
// Function: AWMWidget::AWMWidget()
//-----------------------------------------------------------------------------
AWMWidget::AWMWidget(ProjectHandler* project,
                     ProjectLibraryHandler* projectLibrary,
                     FPGAHandler* fpga,
                     LibraryHandler* library,
                     QWidget *parent): QWidget(parent),
    project_(project),
    projectLibraryHandler_(projectLibrary),
    fpgaHandler_(fpga),
    libraryHandler_(library),
    projectExporter_(libraryHandler_, this, this),
    projectImporter_(libraryHandler_, this, this),
    itemExporter_(libraryHandler_, this, this),
    projectTreeWidget_(new ProjectTreeExWidget(project_, project_->getModel(), this)),
    projectLibraryTreeWidget_(new ProjectLibraryTreeWidget(projectLibraryHandler_, projectLibraryHandler_->getTreeModel(), this)),
    fpgaTreeWidget_(new FPGATreeWidget(fpgaHandler_, fpgaHandler_->getModel(), this)),
    libraryTreeWidget_(new LibraryTreeWidget(libraryHandler_, libraryHandler_->getTreeModel(), this)),
    previewWidget_(new ComponentPreviewBox(library, this)),
    integrityWidget_(nullptr),
    hidePreview_(false)
{
//    GraphicalMessageMediator* guiChannel = dynamic_cast<GraphicalMessageMediator*>(messageChannel);
//    if (guiChannel)
//    {
//        connect(guiChannel, SIGNAL(statusMessage(QString const&)),
//            this, SIGNAL(statusMessage(QString const&)));
//    }

//    connect(this, SIGNAL(exportProject()), &projectExporter_, SLOT(onExportProject()), Qt::UniqueConnection);
//    connect(this, SIGNAL(importProject()), &projectImporter_, SLOT(onImportProject()), Qt::UniqueConnection);

    connect(projectTreeWidget_, SIGNAL(itemSelected(const VLNV&)),
        project_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

//    connect(project_, SIGNAL(itemSelected(const VLNV&)),
//        previewWidget_, SLOT(setComponent(const VLNV&)), Qt::UniqueConnection);

    connect(project_, SIGNAL(itemSelected(const VLNV&)),
        this, SLOT(setComponent_Design(const VLNV&)), Qt::UniqueConnection);

    connect(this, SIGNAL(previewSelected(LibraryInterface* lh, const VLNV&)),
        previewWidget_, SLOT(setComponent(LibraryInterface* lh, const VLNV&)), Qt::UniqueConnection);

    connect(projectTreeWidget_, SIGNAL(createNewDesign()), project_, SIGNAL(createNewDesign()), Qt::UniqueConnection);

    connect(projectTreeWidget_, SIGNAL(exportItem()), &projectExporter_, SLOT(onExportItem()), Qt::UniqueConnection);
    connect(projectTreeWidget_, SIGNAL(importItem()), &projectImporter_, SLOT(onImportItem()), Qt::UniqueConnection);

    auto projectTreeModel = project_->getModel();

    connect(projectTreeModel, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(showErrors(const VLNV)),
        this, SLOT(onShowErrors(const VLNV)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(removeVLNV(const QList<VLNV>)),
        this, SLOT(onRemoveVLNV(const QList<VLNV>)), Qt::UniqueConnection);

    connect(projectTreeModel, SIGNAL(createBus(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createComDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createApiDef(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createComponent(const VLNV&)),
        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createDesign(const VLNV&)),
        this, SLOT(onCreateDesign(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createSWDesign(const VLNV&)),
        this, SIGNAL(createSWDesign(const VLNV&)), Qt::UniqueConnection);
    connect(projectTreeModel, SIGNAL(createSystemDesign(const VLNV&)),
        this, SIGNAL(createSystemDesign(const VLNV&)), Qt::UniqueConnection);

//    connect(projectLibraryTreeWidget_, SIGNAL(itemSelected(const VLNV&)),
//        projectLibraryHandler_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(projectLibraryTreeWidget_, SIGNAL(itemSelected(const VLNV&)),
        projectLibraryHandler_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(projectLibraryTreeWidget_, SIGNAL(copyVLNV(const VLNV&)),
        projectLibraryHandler_, SIGNAL(copyVLNV(const VLNV&)), Qt::UniqueConnection);

//    connect(projectLibraryHandler_, SIGNAL(itemSelected(const VLNV&)),
//        previewWidget_, SLOT(setComponent(const VLNV&)), Qt::UniqueConnection);

    connect(projectLibraryHandler_, SIGNAL(itemSelected(const VLNV&)),
        this, SLOT(setComponent_ProjectLibrary(const VLNV&)), Qt::UniqueConnection);

    connect(this, SIGNAL(previewSelected(LibraryInterface* lh, const VLNV&)),
        previewWidget_, SLOT(setComponent(LibraryInterface* lh, const VLNV&)), Qt::UniqueConnection);



    connect(libraryTreeWidget_, SIGNAL(createNewBus()), this, SIGNAL(createNewBus()), Qt::UniqueConnection);
    connect(libraryTreeWidget_, SIGNAL(createNewHWComponent()), this, SIGNAL(createNewHWComponent()), Qt::UniqueConnection);

    connect(libraryTreeWidget_, SIGNAL(itemSelected(const VLNV&)),
        libraryHandler_, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(libraryTreeWidget_, SIGNAL(changeDesignProtection(bool const&)),
        libraryHandler_, SIGNAL(changeDesignProtection(bool const&)), Qt::UniqueConnection);

//    connect(libraryHandler_, SIGNAL(itemSelected(const VLNV&)),
//        previewWidget_, SLOT(setComponent(const VLNV&)), Qt::UniqueConnection);

    connect(libraryHandler_, SIGNAL(itemSelected(const VLNV&)),
        this, SLOT(setComponent_Library(const VLNV&)), Qt::UniqueConnection);

    connect(this, SIGNAL(previewSelected(LibraryInterface* lh, const VLNV&)),
        previewWidget_, SLOT(setComponent(LibraryInterface* lh, const VLNV&)), Qt::UniqueConnection);


    auto projectLibraryTreeModel = projectLibraryHandler_->getTreeModel();

    connect(projectLibraryTreeModel, SIGNAL(errorMessage(const QString&)),
        this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(projectLibraryTreeModel, SIGNAL(noticeMessage(const QString&)),
        this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);

    connect(projectLibraryTreeModel, SIGNAL(showErrors(const VLNV)),
        this, SLOT(onShowErrors(const VLNV)), Qt::UniqueConnection);

    connect(projectLibraryTreeModel, SIGNAL(removeVLNV(const QList<VLNV>)),
        this, SLOT(onRemoveVLNV(const QList<VLNV>)), Qt::UniqueConnection);


    auto treeModel = libraryHandler_->getTreeModel();

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
        this, SLOT(onCreateAbsDef_Library(const VLNV&)), Qt::UniqueConnection);
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

//    auto hierarchyModel = libraryHandler_->getHierarchyModel();
//    connect(hierarchyModel, SIGNAL(errorMessage(const QString&)),
//        this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(removeVLNV(QList<VLNV>)),
//        this, SLOT(onRemoveVLNV(QList<VLNV>)), Qt::UniqueConnection);

//    connect(hierarchyModel, SIGNAL(createBusDef(const VLNV&)),
//        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createComponent(const VLNV&)),
//        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createBus(const VLNV&)),
//        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createComDef(const VLNV&)),
//        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createApiDef(const VLNV&)),
//        this, SLOT(onCreateNewItem(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createAbsDef(const VLNV&)),
//        this, SLOT(onCreateAbsDef(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createDesign(const VLNV&)),
//        this, SLOT(onCreateDesign(const VLNV&)), Qt::UniqueConnection);

//    connect(hierarchyModel, SIGNAL(createSWDesign(const VLNV&)),
//        this, SIGNAL(createSWDesign(const VLNV&)), Qt::UniqueConnection);
//    connect(hierarchyModel, SIGNAL(createSystemDesign(const VLNV&)),
//        this, SIGNAL(createSystemDesign(const VLNV&)), Qt::UniqueConnection);

//    connect(hierarchyModel, SIGNAL(exportItem(VLNV const&)),
//         &itemExporter_, SLOT(onExportItem(VLNV const&)), Qt::UniqueConnection);

    connect(&itemExporter_, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
    connect(&itemExporter_, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
    connect(&itemExporter_, SIGNAL(projectSearch()), this, SIGNAL(projectSearch()), Qt::UniqueConnection);

    connect(&projectImporter_, SIGNAL(inputMessage(QString const&)), this, SIGNAL(inputMessage(QString const&)));
    connect(&projectImporter_, SIGNAL(noticeMessage(QString const&)), this, SIGNAL(noticeMessage(QString const&)));
    connect(&projectImporter_, SIGNAL(errorMessage(QString const&)), this, SIGNAL(errorMessage(QString const&)));
    connect(&projectImporter_, SIGNAL(undefinedMessage(QString const&)), this, SIGNAL(undefinedMessage(QString const&)));

    connect(&projectImporter_, SIGNAL(projectSearch()), this, SIGNAL(projectSearch()), Qt::UniqueConnection);

    connect(&projectExporter_, SIGNAL(inputMessage(QString const&)), this, SIGNAL(inputMessage(QString const&)));
    connect(&projectExporter_, SIGNAL(noticeMessage(QString const&)), this, SIGNAL(noticeMessage(QString const&)));
    connect(&projectExporter_, SIGNAL(errorMessage(QString const&)), this, SIGNAL(errorMessage(QString const&)));
    connect(&projectExporter_, SIGNAL(undefinedMessage(QString const&)), this, SIGNAL(undefinedMessage(QString const&)));

    setupLayout();
} 

//-----------------------------------------------------------------------------
// Function: AWMWidget::getLibraryHandler()
//-----------------------------------------------------------------------------
LibraryHandler* AWMWidget::getLibraryHandler() const
{
    return libraryHandler_;
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::selectComponent()
//-----------------------------------------------------------------------------
void AWMWidget::selectComponent(VLNV const& componentVLNV) const
{
    projectTreeWidget_->selectItem(componentVLNV);

    projectLibraryTreeWidget_->selectItem(componentVLNV);

    previewWidget_->setComponent(componentVLNV);
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::selectComponent()
//-----------------------------------------------------------------------------
void AWMWidget::selectComponent(LibraryInterface* lh, VLNV const& componentVLNV) const
{
    projectTreeWidget_->selectItem(componentVLNV);

    projectLibraryTreeWidget_->selectItem(componentVLNV);

    previewWidget_->setComponent(lh, componentVLNV);
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::onShowErrors()
//-----------------------------------------------------------------------------
void AWMWidget::onShowErrors(VLNV const& vlnv)
{
    if (vlnv.isValid() == false || libraryHandler_->contains(vlnv) == false)
    {
        return;
    }

    QSharedPointer<Document> document = libraryHandler_->getModel(vlnv);

    // Show error list in a dialog.
    TableViewDialog* dialog = new TableViewDialog(this);
    dialog->setWindowTitle(tr("Errors in %1").arg(vlnv.toString()));
    dialog->setDescription(tr("<b>Integrity check</b><br>The following errors were found."));
    dialog->resize(700, 350);

    LibraryErrorModel* model = new LibraryErrorModel(dialog);
    model->addErrors(libraryHandler_->findErrorsInDocument(document, libraryHandler_->getPath(vlnv)), vlnv.toString());

    dialog->show();

    dialog->setModel(model);

    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::onCreateNewItem()
//-----------------------------------------------------------------------------
void AWMWidget::onCreateNewItem(VLNV const& vlnv)
{
    VLNV::IPXactType documentType = vlnv.getType();

    bool showAttributes = (documentType == VLNV::COMPONENT);
    NewObjectDialog newDesignDialog(libraryHandler_, vlnv.getType(), showAttributes, this);

    if (libraryHandler_->contains(vlnv) && libraryHandler_->getDocumentType(vlnv) == VLNV::COMPONENT)
    {
        QSharedPointer<const Document> document = libraryHandler_->getModelReadOnly(vlnv);
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
// Function: AWMWidget::onCreateAbsDef_ProjectLibrary()
//-----------------------------------------------------------------------------
void AWMWidget::onCreateAbsDef_ProjectLibrary(VLNV const& busDefVLNV)
{
    if (busDefVLNV.isValid() == false)
    {
        return;
    }

    Q_ASSERT(projectLibraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION);
    Q_ASSERT(projectLibraryHandler_->contains(busDefVLNV));

    QFileInfo busDefInfo(projectLibraryHandler_->getPath(busDefVLNV));
    QString directory = busDefInfo.absolutePath();

    emit createAbsDef(busDefVLNV, directory);
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::onCreateAbsDef_Library()
//-----------------------------------------------------------------------------
void AWMWidget::onCreateAbsDef_Library(VLNV const& busDefVLNV)
{
    if (busDefVLNV.isValid() == false)
    {
        return;
    }

    Q_ASSERT(libraryHandler_->getDocumentType(busDefVLNV) == VLNV::BUSDEFINITION);
    Q_ASSERT(libraryHandler_->contains(busDefVLNV));

    QFileInfo busDefInfo(libraryHandler_->getPath(busDefVLNV));
    QString directory = busDefInfo.absolutePath();

    emit createAbsDef(busDefVLNV, directory);
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::onCreateDesign()
//-----------------------------------------------------------------------------
void AWMWidget::onCreateDesign(VLNV const& vlnv)
{
    emit createDesignForExistingComponent(vlnv);
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::onGenerateIntegrityReport()
//-----------------------------------------------------------------------------
void AWMWidget::onGenerateIntegrityReport()
{
    if (!integrityWidget_)
     {
         integrityWidget_ = new TableViewDialog(this);
         integrityWidget_->setWindowTitle(tr("Library Integrity Report"));
         integrityWidget_->setDescription(tr("<b>Integrity check</b><br>The following errors were found."));
         integrityWidget_->resize(1000, 800);

         LibraryErrorModel* model = new LibraryErrorModel(integrityWidget_);

         for (auto const& vlnv : libraryHandler_->getAllVLNVs())
         {
             if (libraryHandler_->isValid(vlnv) == false)
             {
                 model->addErrors(libraryHandler_->findErrorsInDocument(libraryHandler_->getModel(vlnv), libraryHandler_->getPath(vlnv)), vlnv.toString());
             }
         }

         integrityWidget_->setModel(model);

         integrityWidget_->show();

         connect(integrityWidget_, SIGNAL(finished(int)), this, SLOT(onCloseIntegrityReport()));
     }

     integrityWidget_->raise();
}
//-----------------------------------------------------------------------------
// Function: AWMWidget::closeIntegrityReport()
//-----------------------------------------------------------------------------
void AWMWidget::onCloseIntegrityReport()
{
      integrityWidget_->deleteLater();
      integrityWidget_ = 0;
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::onRemoveVLNV()
//-----------------------------------------------------------------------------
void AWMWidget::onRemoveVLNV(const QList<VLNV> vlnvs)
{
    // create the dialog to select which items to remove
    ObjectRemoveDialog removeDialog(this);
    LibraryItemSelectionFactory::constructItemsForSelectionDialog(libraryHandler_, &removeDialog, vlnvs);

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

    libraryHandler_->removeObjects(removedVLNVs);

    for (auto const& path : removedFilePaths)
    {
        QFile file(path);
        if (file.remove() == false)
        {
//           emit errorMessage(tr("File %1 could not be removed from the file system.").arg(path));
        }
    }
}

void AWMWidget::onChangeProject()
{
    QString companyPath = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

    QDir companyDirectory(companyPath);
    QString companyName = companyDirectory.dirName(); // 디렉토리명
}

void AWMWidget:: onImportProject()
{
    projectImporter_.onImportProject();
//    emit importProject();
}

void AWMWidget::onExportProject()
{
    projectExporter_.onExportProject();
//    emit exportProject();
}

void AWMWidget::onImportProject_Console(QString const& fileDest, QString const& filePath, QString const& fileName)
{
    projectImporter_.requestImportProject(fileDest, filePath, fileName);
}

void AWMWidget::onExportProject_Console(QString const& fileDest, QString const& filePath, QString const& fileName)
{
    projectExporter_.requestExportProject(fileDest, filePath, fileName);
}

void AWMWidget::setComponent_Design(const VLNV& vlnv)
{
//    emit previewSelected(project_, vlnv);
    previewWidget_->setComponent(project_, vlnv);
}

void AWMWidget::setComponent_ProjectLibrary(const VLNV& vlnv)
{
//    emit previewSelected(projectLibraryHandler_, vlnv);
    previewWidget_->setComponent(projectLibraryHandler_, vlnv);
}

void AWMWidget::setComponent_Library(const VLNV& vlnv)
{
//    emit previewSelected(libraryHandler_, vlnv);
    previewWidget_->setComponent(libraryHandler_, vlnv);
}

//-----------------------------------------------------------------------------
// Function: AWMWidget::setupLayout()
//-----------------------------------------------------------------------------
void AWMWidget::setupLayout()
{
    QString companyPath = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

//    QWidget* titleGroup = new QWidget(this);
    QWidget* projectGroup = new QWidget(this);
    QWidget* libraryGroup = new QWidget(this);

//    QLabel *label = new QLabel("Hello World!", this);

//    auto *layout = new QVBoxLayout(titleGroup);
//    layout->addWidget(label);  // add label to the layout

//    layout->setContentsMargins(0, 0, 0, 0);

    // 현제 프로젝트명
    QFileInfo fileInfo(companyPath);
    QString projectName = fileInfo.baseName();

    QTabWidget* projectTabs = new QTabWidget(this);
    projectTabs->addTab(projectTreeWidget_, tr("Design"));
    projectTabs->addTab(projectLibraryTreeWidget_, tr("Library"));
//    projectTabs->addTab(fpgaTreeWidget_, tr("FPGA"));
    projectTabs->addTab(fpgaTreeWidget_, tr("Bin"));
//    projectTabs->addTab(projectTreeWidget_, tr("Project"));
//    projectTabs->addTab(projectTreeWidget_, projectName);

    auto projectLayout = new QVBoxLayout(projectGroup);

    projectLayout->addWidget(projectTabs, 1);

    projectLayout->setContentsMargins(0, 0, 0, 0);

    QTabWidget* libraryTabs = new QTabWidget(this);
    libraryTabs->addTab(libraryTreeWidget_, tr("Library"));

    auto libraryLayout = new QVBoxLayout(libraryGroup);

    libraryLayout->addWidget(libraryTabs, 1);

    libraryLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* previewGroup = new QWidget(this);

    auto previewLayout = new QGridLayout(previewGroup);

    QLabel* previewLabel = new QLabel(tr("Preview"), this);

    previewLayout->addWidget(previewLabel, 0, 0, 1, 1);
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
