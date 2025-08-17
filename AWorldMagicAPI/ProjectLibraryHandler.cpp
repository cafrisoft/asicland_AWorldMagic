//-----------------------------------------------------------------------------
// File: ProjectLibraryHandler.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 04.06.2024
//
// Description:
// ProjectLibraryHandler is the class that implements the services to manage the IP-XACT library.
//-----------------------------------------------------------------------------

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

#include <IPXACTmodels/common/Document.h>

#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>

#include <IPXACTmodels/BusDefinition/BusDefinition.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/FileSet.h>

#include <IPXACTmodels/Design/Design.h>

#include <IPXACTmodels/designConfiguration/DesignConfiguration.h>

#include <IPXACTmodels/common/VLNV.h>

#include <KactusAPI/Project.h>

#include <KactusAPI/MessageMediator.h>

#include <KactusAPI/utils.h>

#include <KactusAPI/TagManager.h>

#include "ProjectLibraryHandler.h"


// Singleton instance.
ProjectLibraryHandler* ProjectLibraryHandler::ProjectLibraryHandler_instance_ = nullptr;

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getInstance()
//-----------------------------------------------------------------------------
ProjectLibraryHandler* ProjectLibraryHandler::getInstance()
{
    if (ProjectLibraryHandler_instance_ == nullptr)
    {
        initialize(nullptr, nullptr);
    }

    return ProjectLibraryHandler::ProjectLibraryHandler_instance_;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::initialize()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::initialize(MessageMediator* messageChannel, QObject* parent)
{
    if (ProjectLibraryHandler_instance_ == nullptr)
    {
        ProjectLibraryHandler::ProjectLibraryHandler_instance_ = new ProjectLibraryHandler(messageChannel, parent);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::ProjectLibraryHandler()
//-----------------------------------------------------------------------------
ProjectLibraryHandler::ProjectLibraryHandler(MessageMediator* messageChannel, QObject* parent):
QObject(parent),
    messageChannel_(messageChannel),
    loader_(messageChannel),
    documentCache_(),
    urlTester_(Utils::URL_VALIDITY_REG_EXP, this),
    validator_(this),
    treeModel_(new ProjectLibraryTreeModel(this, this)),
    hierarchyModel_(new HierarchyModel(this, this)),
    saveInProgress_(false),
    checkResults_()
{
    // create the connections between models and library handler
    syncronizeModels();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getModel()
//-----------------------------------------------------------------------------
QSharedPointer<Document> ProjectLibraryHandler::getModel(VLNV const& vlnv)
{
    auto info = documentCache_.find(vlnv);
    if (info == documentCache_.end())
    {
        showNotFoundError(vlnv);
        return QSharedPointer<Document>();
    }

    // If object has not already been parsed, read it from the disk.
    if (info->document.isNull())
    {
        info->document = DocumentFileAccess::readDocument(info->path);
    }

    QSharedPointer<Document> copy;
    if (info->document.isNull() == false)
    {
        copy = info->document->clone();
    }

    return copy;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getModelReadOnly()
//-----------------------------------------------------------------------------
QSharedPointer<Document const> ProjectLibraryHandler::getModelReadOnly(VLNV const& vlnv)
{
    auto info = documentCache_.find(vlnv);
    if (info == documentCache_.end())
    {
        return QSharedPointer<Document const>();
    }

    if (info->document.isNull())
    {
        info->document = DocumentFileAccess::readDocument(info->path);
    }

    return info->document;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getAllVLNVs()
//-----------------------------------------------------------------------------
QList<VLNV> ProjectLibraryHandler::getAllVLNVs() const
{
    return documentCache_.keys();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::contains()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::contains(const VLNV& vlnv) const
{
    return documentCache_.contains(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getPath()
//-----------------------------------------------------------------------------
const QString ProjectLibraryHandler::getPath(VLNV const& vlnv) const
{
    return documentCache_.value(vlnv).path;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getDirectoryPath()
//-----------------------------------------------------------------------------
QString ProjectLibraryHandler::getDirectoryPath(VLNV const& vlnv) const
{
    QFileInfo pathInfo(getPath(vlnv));
    return pathInfo.absolutePath();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::writeModelToFile()
//-----------------------------------------------------------------------------
#if LIBRARYHANDLER_WRITEMODELTOFILE_20231206 == 1
bool ProjectLibraryHandler::writeModelToFile(QString const& path, QSharedPointer<Document> model)
{
    VLNV vlnv = model->getVlnv();
    Q_ASSERT(contains(vlnv) == false);

    // Create the path if it does not exist.
    if (QDir(path).exists() == false && QDir().mkpath(path) == false)
    {
//        messageChannel_->showFailure(tr("Could not create directory \"%1\".").arg(path));
        return false;
    }

    QString filePath = path + "/" + vlnv.getName() + "." + vlnv.getVersion() + ".xml";
    if (addObject(model, filePath) == false)
    {
        return false;
    }

    // the hierarchy model must be re-built
    hierarchyModel_->onResetModel();
    treeModel_->onAddVLNV(vlnv);

    return true;
}
#else
bool ProjectLibraryHandler::writeModelToFile(QString const& path, QSharedPointer<Document> model)
{
    VLNV vlnv = model->getVlnv();
    Q_ASSERT(contains(vlnv) == false);

    // Create the path if it does not exist.
    if (QDir(path).exists() == false && QDir().mkpath(path) == false)
    {
        messageChannel_->showFailure(tr("Could not create directory \"%1\".").arg(path));
        return false;
    }

    QString filePath = path + "/" + vlnv.getName() + "." + vlnv.getVersion() + ".xml";
    if (addObject(model, filePath) == false)
    {
        return false;
    }

    treeModel_->onAddVLNV(vlnv);

    return true;
}
#endif

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::writeModelToFile()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::writeModelToFile(QSharedPointer<Document> model)
{
    return addObject(model, getPath(model->getVlnv()));
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::searchForIPXactFiles()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::searchForIPXactFiles()
{
    clearCache();

    loadAvailableVLNVs();
    loadAvailableVerilogs();

    onCheckLibraryIntegrity();
    
    resetModels();

}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getNeededVLNVs()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::getNeededVLNVs(VLNV const& vlnv, QList<VLNV>& list)
{
    if (contains(vlnv) == false)
    {
//        messageChannel_->showError(tr("No item with following info was found in library: \n"
//            "Vendor: %1\n"
//            "Library: %2\n"
//            "Name: %3\n"
//            "Version: %4").arg(vlnv.getVendor(), vlnv.getLibrary(), vlnv.getName(), vlnv.getVersion()));
        return;
    }

    // This document is searched so add it to the list.
    list.append(vlnv);

    for (VLNV const& dependentVLNV : getModelReadOnly(vlnv)->getDependentVLNVs())
    {
        if (list.contains(dependentVLNV) == false)
        {
            getNeededVLNVs(dependentVLNV, list);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getDependencyFiles()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::getDependencyFiles(VLNV const& vlnv, QStringList& list)
{
    QSharedPointer<const Document> document = getModelReadOnly(vlnv);

    // Get path to the IP-Xact document that is used as base for relative file paths.
    QFileInfo documentFile(getPath(vlnv));
    QDir documentDirectory(documentFile.absolutePath());

    // Convert all relative file path to an absolute to make the file accessible in the caller function.
    for (QString const& relativePath : document->getDependentFiles())
    {
        QFileInfo dependentFile(documentDirectory.absoluteFilePath(relativePath));
        
        QString path = dependentFile.canonicalFilePath();
        if (dependentFile.exists() && list.contains(path) == false)
        {
            list.append(path);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getTreeRoot()
//-----------------------------------------------------------------------------
LibraryItem const* ProjectLibraryHandler::getTreeRoot() const
{
    return treeModel_->getRoot();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getDocumentType()
//-----------------------------------------------------------------------------
VLNV::IPXactType ProjectLibraryHandler::getDocumentType(VLNV const& vlnv)
{
    auto it = documentCache_.constFind(vlnv);
    if (it == documentCache_.constEnd())
    {
        return VLNV::INVALID;
    }

    return it.key().getType();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::referenceCount()
//-----------------------------------------------------------------------------
int ProjectLibraryHandler::referenceCount(VLNV const& vlnv) const
{
    QList<VLNV> list;
    return hierarchyModel_->getOwners(list, vlnv);
//    return 0;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getOwners()
//-----------------------------------------------------------------------------
int ProjectLibraryHandler::getOwners(QList<VLNV>& list, VLNV const& vlnvToSearch) const
{
    return hierarchyModel_->getOwners(list, vlnvToSearch);
//    return 0;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getChildren()
//-----------------------------------------------------------------------------
int ProjectLibraryHandler::getChildren(QList<VLNV>& list, VLNV const& vlnvToSearch) const
{
    if (vlnvToSearch.isValid() == false)
    {
        return 0;
    }

    hierarchyModel_->getChildren(list, vlnvToSearch);
    return list.size();

//    return 0;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getDesignVLNV()
//-----------------------------------------------------------------------------
VLNV ProjectLibraryHandler::getDesignVLNV(VLNV const& hierarchyRef)
{
    if (getDocumentType(hierarchyRef) == VLNV::DESIGNCONFIGURATION)
    {
        QSharedPointer<const DesignConfiguration> desConf = 
            getModelReadOnly(hierarchyRef).staticCast<const DesignConfiguration>();

        VLNV designVLNV = desConf->getDesignRef();
        if (!contains(designVLNV) || getDocumentType(designVLNV) != VLNV::DESIGN)
        {
            showNotFoundError(designVLNV);
            return VLNV();
        }

        designVLNV.setType(VLNV::DESIGN);
        return designVLNV;
    }
    else if (getDocumentType(hierarchyRef) == VLNV::DESIGN)
    {
        return hierarchyRef;
    }
    else if (contains(hierarchyRef) == false)
    {
        showNotFoundError(hierarchyRef);
        return VLNV();
    }
    else
    {
//        messageChannel_->showError(tr("VLNV: %1 was not valid hierarchical reference.").arg(hierarchyRef.toString()));
        return VLNV();
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getDesign()
//-----------------------------------------------------------------------------
QSharedPointer<Design> ProjectLibraryHandler::getDesign(VLNV const& hierarchyRef)
{
    VLNV designVlnv = getDesignVLNV(hierarchyRef);
    if (designVlnv.isValid() == false)
    {
        showNotFoundError(hierarchyRef);
        return QSharedPointer<Design>();
    }   

    return getModel(designVlnv).staticCast<Design>();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::isValid()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::isValid(VLNV const& vlnv)
{    
    auto it = documentCache_.constFind(vlnv);
    if (it != documentCache_.cend())
    {
        return documentCache_.value(vlnv).isValid;
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getTreeModel()
//-----------------------------------------------------------------------------
ProjectLibraryTreeModel* ProjectLibraryHandler::getTreeModel()
{
    return treeModel_;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getHierarchyModel()
//-----------------------------------------------------------------------------
HierarchyModel* ProjectLibraryHandler::getHierarchyModel()
{
    return hierarchyModel_;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onCheckLibraryIntegrity()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onCheckLibraryIntegrity()
{
//    messageChannel_->showStatusMessage(tr("Validating items. Please wait..."));

    checkResults_.documentCount = 0;
    checkResults_.fileCount = 0;

    QVector<TagData> documentTags;
    for (auto it = documentCache_.begin(); it != documentCache_.end(); ++it)
    {
        // TODO: Add model to cache only, if it is already previously cached.
        // Current hierarchy model forces all models to be loaded, but this should be changed.
        QSharedPointer<Document> model = it->document;
        if (model.isNull())
        {
            model = DocumentFileAccess::readDocument(it->path);
            it->document = model;
        }           

        if (model.isNull() == false)
        {
            documentTags += (model->getTags());
        }

        it->isValid = validateDocument(model, it->path);
        if (it->isValid == false)
        {
            checkResults_.documentCount++;
        }
    }

    TagManager& manager = TagManager::getInstance();
    manager.setTags(documentTags);

    showIntegrityResults();

//    messageChannel_->showStatusMessage(tr("Ready."));
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onEditItem()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onEditItem(VLNV const& vlnv)
{
    if (vlnv.isValid() == false)
    {
        return;
    }

    VLNV::IPXactType documentType = getDocumentType(vlnv);
    if (documentType == VLNV::COMPONENT)
    {
        emit openComponent(vlnv);
    }

    else if (documentType == VLNV::CATALOG)
    {
        emit openCatalog(vlnv);
    }

    else if (documentType == VLNV::COMDEFINITION)
    {
        emit openComDefinition(vlnv);
    }

    else if (documentType == VLNV::APIDEFINITION)
    {
        emit openApiDefinition(vlnv);
    }

    else if (documentType == VLNV::BUSDEFINITION)
    {
        QList<VLNV> absDefVLNVs;
        hierarchyModel_->getChildren(absDefVLNVs, vlnv);

        if (absDefVLNVs.size() == 1)
        {
            emit openBus(vlnv, absDefVLNVs.first(), false);
        }

        else // the abstraction definition can not be identified by the bus definition.
        {
            emit openBus(vlnv, VLNV(), false);
        }

//        emit openBus(vlnv);
    }

    else if (documentType == VLNV::ABSTRACTIONDEFINITION)
    {
        QSharedPointer<const Document> document = getModelReadOnly(vlnv);
        QSharedPointer<const AbstractionDefinition> absDef = document.staticCast<const AbstractionDefinition>();

        VLNV busDefVLNV = absDef->getBusType();

        if (contains(busDefVLNV) == false)
        {
            QString message(
                tr("Library does not contain bus definition %1 required by abstraction definition %2").arg(
                busDefVLNV.toString(), vlnv.toString()));

//            messageChannel_->showError(message);
            return;
        }

        QList<VLNV> absDefVLNVs;
        hierarchyModel_->getChildren(absDefVLNVs, busDefVLNV);

        // If there is exactly one abstraction definition per bus definition, open it.
        if (absDefVLNVs.size() == 1 && absDefVLNVs.first() == vlnv)
        {
            emit openAbsDef(busDefVLNV, vlnv, false);
        }
        else  // If there are multiple abstraction definitions for that one bus def, disable bus definition.
        {
            emit openAbsDef(busDefVLNV, vlnv, true);
        }

//        emit openAbsDef(vlnv);
    }

//    else
//    {
//        messageChannel_->showMessage(tr("Item can not be edited."));
//    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onOpenDesign()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onOpenDesign(VLNV const& vlnv, QString const& viewName)
{
    if (contains(vlnv) == false)
    {
//        messageChannel_->showError(tr("Component was not found"));
        return;
    }

    if (getDocumentType(vlnv) == VLNV::COMPONENT)
    {
         emit openDesign(vlnv, viewName);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onOpenMemoryDesign()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onOpenMemoryDesign(VLNV const& vlnv, QString const& activeView)
{
    QSharedPointer<const Component> component = getModelReadOnly(vlnv).dynamicCast<const Component>();
    if (component.isNull())
    {
//        messageChannel_->showError(tr("Component was not found"));
        return;
    }
       
    if (component->getHierViews().contains(activeView))
    {
        emit openMemoryDesign(vlnv, activeView);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onOpenSWDesign()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onOpenSWDesign(VLNV const& vlnv)
{
    QSharedPointer<const Component> component = getModelReadOnly(vlnv).dynamicCast<const Component>();
    if (component.isNull())
    {
//        messageChannel_->showError(tr("Component was not found"));
        return;
    }

    for (QSharedPointer<View> view : *component->getViews())
    {
        VLNV reference = component->getModel()->getHierRef(view->name());

        QSharedPointer<const Document> document = getModelReadOnly(reference);

        if (document && document->getImplementation() == KactusAttribute::SW)
        {
            emit openSWDesign(vlnv, view->name());
            return;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onOpenSystemDesign()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onOpenSystemDesign(VLNV const& vlnv)
{
    QSharedPointer<const Component> component = getModelReadOnly(vlnv).dynamicCast<const Component>();
    if (component.isNull())
    {
//        messageChannel_->showError(tr("Component was not found"));
        return;
    }

    QStringList systemViews = component->getSystemViewNames();
    if (!systemViews.isEmpty())
    {
        emit openSystemDesign(vlnv, systemViews.first());
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::removeObject()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::removeObject(VLNV const& vlnv)
{
    if (vlnv.isValid() == false || contains(vlnv) == false)
    {
        return;
    }

    QString path = documentCache_.find(vlnv)->path;

    documentCache_.remove(vlnv);

    treeModel_->onRemoveVLNV(vlnv);
    hierarchyModel_->onRemoveVLNV(vlnv);

    removeFile(path);
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::removeObjects()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::removeObjects(QList<VLNV> const& vlnvList)
{
    QStringList changedDirectories;
    for (auto const& removedItem : vlnvList)
    {
        changedDirectories.append(QFileInfo(getPath(removedItem)).absolutePath());
        removeObject(removedItem);
    }

    changedDirectories.removeDuplicates();
    loader_.clean(changedDirectories);

    QString removeMessage = QString("Deleted %1 VLNV item(s).").arg(QString::number(vlnvList.count()));

//    messageChannel_->showMessage(removeMessage);
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::beginSave()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::beginSave()
{
    saveInProgress_ = true;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::endSave()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::endSave()
{
    saveInProgress_ = false;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::onItemSaved()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::onItemSaved(VLNV const& vlnv)
{
    if (vlnv.isValid() == false)
    {
        return;
    }

    VLNV savedItem = vlnv;
    savedItem.setType(getDocumentType(vlnv));

    // Replace overwritten item with new and check validity.
    QSharedPointer<Document> model = getModel(vlnv);
    documentCache_.insert(vlnv, DocumentInfo(getPath(vlnv), model, validateDocument(model, getPath(vlnv))));
    
    treeModel_->onDocumentUpdated(vlnv);
    hierarchyModel_->onDocumentUpdated(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::syncronizeModels()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::syncronizeModels()
{
    // signals from tree model to library handler
 
    connect(treeModel_, SIGNAL(openDesign(const VLNV&, QString const&)),
        this, SLOT(onOpenDesign(const VLNV&, QString const&)), Qt::UniqueConnection);
    connect(treeModel_, SIGNAL(openMemoryDesign(const VLNV&, QString const&)),
        this, SLOT(onOpenMemoryDesign(const VLNV&, QString const&)), Qt::UniqueConnection);
    connect(treeModel_, SIGNAL(openSWDesign(const VLNV&)),
        this, SLOT(onOpenSWDesign(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel_, SIGNAL(openSystemDesign(const VLNV&)),
        this, SLOT(onOpenSystemDesign(const VLNV&)), Qt::UniqueConnection);
    connect(treeModel_, SIGNAL(editItem(const VLNV&)),
        this, SLOT(onEditItem(const VLNV&)), Qt::UniqueConnection);

    connect(this, SIGNAL(resetModel()),
        treeModel_, SLOT(onResetModel()), Qt::UniqueConnection);

    connect(this, SIGNAL(resetModel()),
        hierarchyModel_, SLOT(onResetModel()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::showNotFoundError()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::showNotFoundError(VLNV const& vlnv) const
{
//    messageChannel_->showError(tr("VLNV: %1 was not found in library.").arg(vlnv.toString()));
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::addObject()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::addObject(QSharedPointer<Document> model, QString const& filePath)
{
    QString targetPath = filePath;

    QFileInfo pathInfo(filePath);
    if (pathInfo.isSymLink() && pathInfo.exists())
    {
        targetPath = pathInfo.symLinkTarget();
    }

    DocumentFileAccess::writeDocument(model, targetPath);

    TagManager::getInstance().addNewTags(model->getTags());

    documentCache_.insert(model->getVlnv(), DocumentInfo(targetPath, model->clone(), validateDocument(model, targetPath)));

    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::clearCache()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::clearCache()
{
    documentCache_.clear();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::loadAvailableVLNVs()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::loadAvailableVLNVs()
{
//    messageChannel_->showStatusMessage(tr("Scanning library. Please wait..."));

    // Read all items before validation.
    // Validation will check for VLNVs in the library, so they must be available before validation.    
    for (auto const& target: loader_.parseLibrary())
    {
        if (contains(target.vlnv))
        {
//            messageChannel_->showMessage(tr("VLNV %1 was already found in the library").arg(
//                target.vlnv.toString()));
        }
        else
        {
            documentCache_.insert(target.vlnv, DocumentInfo(target.path));
        }
    }
//    messageChannel_->showStatusMessage(tr("Ready."));
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::loadAvailableVerilogs()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::loadAvailableVerilogs()
{
//    messageChannel_->showStatusMessage(tr("Scanning library. Please wait..."));

//    // Read all items before validation.
//    // Validation will check for VLNVs in the library, so they must be available before validation.
//    for (auto const& target: loader_.parseLibrary())
//    {
//        if (contains(target.vlnv))
//        {
//            messageChannel_->showMessage(tr("VLNV %1 was already found in the library").arg(
//                target.vlnv.toString()));
//        }
//        else
//        {
//            documentCache_.insert(target.vlnv, DocumentInfo(target.path));
//        }
//    }
//    messageChannel_->showStatusMessage(tr("Ready."));
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::resetModels()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::resetModels()
{
//    messageChannel_->showStatusMessage(tr("Updating library view. Please wait..."));

    emit resetModel();

//    messageChannel_->showStatusMessage(tr("Ready."));
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::showIntegrityResults()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::showIntegrityResults() const
{
//    messageChannel_->showMessage(tr("========== Project Library integrity check complete =========="));
//    messageChannel_->showMessage(tr("Total library object count: %1").arg(documentCache_.size()));
//    messageChannel_->showMessage(tr("Total file count in the library: %1").arg(checkResults_.fileCount));

    // if errors were found then print the summary of error types
//    if (checkResults_.documentCount > 0)
//    {
//        messageChannel_->showError(tr("Total items containing errors: %1").arg(checkResults_.documentCount));
//    }
}

//-----------------------------------------------------------------------------
// Function: LibraryData::removeFile()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::removeFile(QString const& filePath) const
{
    QFile file(filePath);
    if (file.remove() == false)
    {
//        messageChannel_->showError(tr("File %1 could not be removed from the file system.").arg(filePath));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::validateDocument()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::validateDocument(QSharedPointer<Document> document, QString const& documentPath)
{
    if (document.isNull() || QFileInfo(documentPath).exists() == false)
    {
        return false;
    }

    bool documentValid = validator_.validate(document);

    return documentValid && validateDependentVLNVReferencences(document) &&
        validateDependentDirectories(document, documentPath) &&
        validateDependentFiles(document, documentPath);
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::findErrorsInDocument()
//-----------------------------------------------------------------------------
QVector<QString> ProjectLibraryHandler::findErrorsInDocument(QSharedPointer<Document> document, QString const& path)
{
    QVector<QString> errorList;
    if (document.isNull())
    {
        if (path.isEmpty() == false)
        {
            auto it = std::find_if(documentCache_.cbegin(), documentCache_.cend(),
                [path](DocumentInfo const& info) { return info.path == path;  });
            
            if (it != documentCache_.cend())
            {
                errorList.append(tr("File %1 could not be read.").arg(path));
            }
        }

        return errorList;
    }

    if (QFileInfo(path).exists() == false)
    {
        errorList.append(tr("File %1 for the document was not found.").arg(path));
    }

    validator_.findErrorsIn(document, errorList);

    findErrorsInDependentVLNVReferencences(document, errorList);

    findErrorsInDependentDirectories(document, path, errorList);

    findErrorsInDependentFiles(document, path, errorList);

    return errorList;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::getType()
//-----------------------------------------------------------------------------
const QString ProjectLibraryHandler::getType() const
{
    return QString("Project");
}

bool ProjectLibraryHandler::writeProjectToFile(Project const& project, VLNV const& vlnv, QString const& directoryl)
{
    Q_ASSERT(contains(vlnv) == false);

    // Create the path if it does not exist.
//    if (QDir(path).exists() == false && QDir().mkpath(path) == false)
//    {
//        messageChannel_->showFailure(tr("Could not create directory \"%1\".").arg(path));
//        return false;
//    }

//    QDate currentDate = QDate::currentDate();

//    QString iniFilePath = directory + "/awm.conf";

//    QSettings settings(iniFilePath, QSettings::IniFormat);

//    // 데이터 저장
//    settings.setValue("apr/valid", true);
//    settings.setValue("version/release", pri.getVersion());
//    settings.setValue("version/date", currentDate.toString("yyyyMMdd"));
//    settings.setValue("owner/name", pri.getAuthor());
//    settings.setValue("owner/description", pri.getDescription());


//    QString filePath = path + "/" + vlnv.getName() + "." + vlnv.getVersion() + ".xml";
//    if (addObject(model, filePath) == false)
//    {
//        return false;
//    }

    treeModel_->onAddVLNV(vlnv);

    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::validateDependentVLNVReferencences()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::validateDependentVLNVReferencences(QSharedPointer<Document> document)
{
    for (VLNV const& vlnv : document->getDependentVLNVs())
    {
        if (contains(vlnv) == false)
        {
           return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::findErrorsInDependentVLNVReferencences()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::findErrorsInDependentVLNVReferencences(QSharedPointer<const Document> document,
    QVector<QString>& errorList)
{
    for (VLNV const& vlnv : document->getDependentVLNVs())
    {
        if (contains(vlnv) == false)
        {
            errorList.append(tr("The referenced VLNV was not found in the library: %1").arg(vlnv.toString()));
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::validateDependentDirectories()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::validateDependentDirectories(QSharedPointer<Document> document, QString const& documentPath)
{
    for (QString const& directoryPath : document->getDependentDirs())
    {
        QFileInfo directory(General::getAbsolutePath(documentPath, directoryPath));
        if (directory.exists() == false)
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::findErrorsInDependentDirectories()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::findErrorsInDependentDirectories(QSharedPointer<const Document> document,
    QString const& documentPath, QVector<QString>& errorList)
{
    for (QString const& directoryPath : document->getDependentDirs())
    {
        QFileInfo directory(General::getAbsolutePath(documentPath, directoryPath));
        if (directory.exists() == false)
        {
            errorList.append(tr("\tDirectory %1 was not found in the file system.").arg(directoryPath));
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectLibraryHandler::validateDependentFiles()
//-----------------------------------------------------------------------------
bool ProjectLibraryHandler::validateDependentFiles(QSharedPointer<Document> document, QString const& documentPath)
{
    for (QString filePath : document->getDependentFiles())
    {
        int pos = 0;
        if (urlTester_.validate(filePath, pos) != QValidator::Acceptable)
        {
            if (QFileInfo(filePath).isRelative())
            {
                filePath = General::getAbsolutePath(documentPath, filePath);
            }

            if (QFileInfo(filePath).exists() == false)
            {
                return false;
            }
            else
            {
                checkResults_.fileCount++;
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: LibraryData::findErrorsInDependentFiles()
//-----------------------------------------------------------------------------
void ProjectLibraryHandler::findErrorsInDependentFiles(QSharedPointer<const Document> document,
    QString const& documentPath, QVector<QString>& errorList)
{
    for (QString filePath : document->getDependentFiles())
    {
        // Check if the path is actually a URL to (external) location.
        int pos = 0;
        if (urlTester_.validate(filePath, pos) == QValidator::Acceptable)
        {
            checkResults_.fileCount++;
        }

        // The path was not URL so it must be file reference on the disk.
        else
        {
            QString absolutePath = filePath;
            if (QFileInfo(absolutePath).isRelative())
            {
                absolutePath = General::getAbsolutePath(documentPath, filePath);
            }

            if (QFileInfo(absolutePath).exists() == false)
            {
                errorList.append(tr("File %1 was not found in the file system.").arg(filePath));
            }
        }
    }
}
