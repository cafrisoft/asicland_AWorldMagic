/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectImporter.cpp
 *      Project: AWM
 *      Description :
 *          Imports the selected library items.
 */

#include <QDirIterator>
//#include <QByteArray>
//#include <QCompressor>
//#include <QDecompressor>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>

#include <KactusAPI/DocumentFileAccess.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/FileSet.h>
#include <IPXACTmodels/Component/File.h>

#ifdef _WIN32
#include <common01/SettingManager.h>
#else
#include <Common/SettingManager.h>
#endif

#include <UI/Dialogs/ProjectImportDialog/ProjectImportDialog.h>

#include "ProjectItemSelectionFactory.h"

//#include "MessageMediator.h"

#include "ProjectImporter.h"


//-----------------------------------------------------------------------------
// Function: ProjectImporter::ProjectImporter()
//-----------------------------------------------------------------------------
ProjectImporter::ProjectImporter(LibraryInterface* libraryAccess,
    QWidget* dialogParentWidget, QObject* parent):
QObject(parent),
libraryAccess_(libraryAccess),
dialogParentWidget_(dialogParentWidget)
{

}

QString ProjectImporter::containsFileWithExtension(const QString& directoryPath, const QString& targetExtension)
{
    bool ret = false;
    QString path = "";

    // QDir를 사용하여 디렉토리 내의 파일 확인
    QDir directory(directoryPath);

    // 디렉토리가 존재하는지 확인
    if (directory.exists()) {
        // 디렉토리 내의 파일 목록 가져오기
        QStringList files = directory.entryList(QStringList() << "*." + targetExtension, QDir::Files);

        // 특정 확장자를 가진 파일이 있는지 확인
        if (!files.isEmpty()) {
            path = directoryPath + "/" + files.first();
            ret = true;
        }
    }

//    return ret;
    return path;
}

bool ProjectImporter::containsFilesWithExtension(const QString& directoryPath, const QString& targetExtension)
{
    QDirIterator it(directoryPath, QStringList() << "*." + targetExtension, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    // 디렉토리 하부에서 특정 확장자를 가진 파일 찾기
    while (it.hasNext()) {
        it.next();
        return true; // 파일을 찾으면 바로 종료
    }

    return false; // 파일을 찾지 못함
}

bool ProjectImporter::removeRecursively(const QString& directoryPath)
{
    bool ret = false;

    // QDir를 사용하여 디렉토리 삭제
    QDir directory(directoryPath);

    if (directory.exists()) {
        // removeRecursively() 함수를 사용하여 디렉토리 삭제
        if (directory.removeRecursively()) {
            ret = true;
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::onImportItem()
//-----------------------------------------------------------------------------
void ProjectImporter::onImportItem()
{
//    QString currentPath = SettingManager::getDefaultProjectLocation();
    QString currentPath = QDir::homePath();

    QFileDialog browseDialog(nullptr, tr("Select a file for import"), currentPath);
    browseDialog.setNameFilter("apr (*.apr)");
    browseDialog.setFileMode(QFileDialog::ExistingFile);

    int exec = browseDialog.exec();
    if (exec== QDialog::Accepted)
    {
        QStringList selectedFiles = browseDialog.selectedFiles();

        if (!selectedFiles.isEmpty())   currentPath = selectedFiles.first();
        else    return;
    }
    else if (exec== QDialog::Rejected)
    {
        return;
    }

    QString sourcePath = currentPath;

    QFileInfo fileInfo(sourcePath);

    QString name = fileInfo.baseName();

    QString defaultLocation = SettingManager::getProjectLocation()+ "/" + name;

    QDir defaultLocationDir(defaultLocation);
    defaultLocationDir.cdUp();

    QString destinationPath = defaultLocationDir.absolutePath();
    QString validPath = defaultLocation;

    // 1. 디렉토리 확인 후 리네임
    QDir dir(validPath);

    if (dir.exists()) {
        QMessageBox msgBox;
        msgBox.setText("There is a project at that path. Are you sure you want to proceed with this?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        // 사용자의 선택에 따라 다른 동작 수행
        if (msgBox.exec() == QMessageBox::No) {
            return;
        }
    }

    DocumentStatistics importStatistics = importSelectedObjects(destinationPath, sourcePath, name);

//    if (importStatistics.documentCount > 0) {
//        //. 1 conf 파일 여부 확인, 3. conf valid 기 true 여부 확인
//        QString path = containsFileWithExtension(validPath, "conf");

//        if (path.isEmpty())
//        {
////        if ( !containsFileWithExtension(validPath, "conf") ) {
//            // delecte directory
//            removeRecursively(validPath);

//            QMessageBox::warning(nullptr, "Warning", "Project file is not verified.");

//            importStatistics.documentCount = 0;
//        }
//        else {
//            QSettings settings(path, QSettings::IniFormat);

//            bool valid = settings.value("apr/valid").toBool();

//            if (!valid) {
//                // delecte directory
//                removeRecursively(validPath);

//                QMessageBox::warning(nullptr, "Warning", "Not a valid project.");

//                importStatistics.documentCount = 0;
//            }
//        }
//    }

//    if (importStatistics.documentCount > 0) {
//        //. 2 xml  파일 여부 확인
//        if ( !containsFilesWithExtension(validPath, "xml") ) {
//            // delecte directory
//            removeRecursively(validPath);

//            QMessageBox::warning(nullptr, "Warning", "The default file is not resolved.");

//            importStatistics.documentCount = 0;
//        }

//        //. 3 conf valid 기 true 여부 확인

//    }


    if (importStatistics.documentCount > 0) {

        QSettings().setValue("Project/ActiveLocations", destinationPath + "/" + name);
        QSettings().setValue("Project/DefaultLocation", destinationPath + "/" + name);
        QSettings().setValue("Project/Locations", destinationPath + "/" + name);

        QMessageBox::information(nullptr, "information", "Project import is complete.");

        QString inputText = "import " + sourcePath;
        emit inputMessage(inputText);

        QString resultText = "[ INFO ] | Import | Project imported.";
        emit noticeMessage(resultText);

        emit projectSearch();
    }
}

////-----------------------------------------------------------------------------
//// Function: ProjectImporter::onImportItems()
////-----------------------------------------------------------------------------
//void ProjectImporter::onImportItems()
//{
//    // get the current working directory and save it to be restored in the end of the function
//    QDir savedWorkingDirectory = QDir::current();

//    ProjectImportDialog* importDialog = new ProjectImportDialog(dialogParentWidget_);
////    ProjectItemSelectionFactory::constructItemsForSelectionDialog(libraryAccess_, importDialog, vlnvs);

//    if (importDialog->exec() == QDialog::Rejected)
//    {
//        return;
//    }

//    QString sourcePath = importDialog->getSourceFile();

//    QFileInfo fileInfo(sourcePath);

//    QString name = fileInfo.baseName();


//    QString defaultLocation = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString() + "/";

//    QDir defaultLocationDir(defaultLocation);
//    defaultLocationDir.cdUp();

//    QString destinationPath = defaultLocationDir.absolutePath();
//    QString validPath = defaultLocation;
////    QString destinationPath = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString() + "/";
////    QString validPath =  QSettings().value(QStringLiteral("Project/DefaultLocation")).toString() + "/" + name;

//    // 1. 디렉토리 확인 후 리네임
//    QDir dir(validPath);

//    if (dir.exists()) {
//        QMessageBox msgBox;
//        msgBox.setText("There is a project at that path. Are you sure you want to proceed with this?");
//        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//        msgBox.setDefaultButton(QMessageBox::No);

//        // 사용자의 선택에 따라 다른 동작 수행
//        if (msgBox.exec() == QMessageBox::No) {
//            return;
//        }
//    }

//    DocumentStatistics importStatistics = importSelectedObjects(destinationPath, sourcePath, name);

////    if (importStatistics.documentCount > 0) {
////        //. 1 conf 파일 여부 확인, 3. conf valid 기 true 여부 확인
////        QString path = containsFileWithExtension(validPath, "conf");

////        if (path.isEmpty())
////        {
//////        if ( !containsFileWithExtension(validPath, "conf") ) {
////            // delecte directory
////            removeRecursively(validPath);

////            QMessageBox::warning(nullptr, "Warning", "Project file is not verified.");

////            importStatistics.documentCount = 0;
////        }
////        else {
////            QSettings settings(path, QSettings::IniFormat);

////            bool valid = settings.value("apr/valid").toBool();

////            if (!valid) {
////                // delecte directory
////                removeRecursively(validPath);

////                QMessageBox::warning(nullptr, "Warning", "Not a valid project.");

////                importStatistics.documentCount = 0;
////            }
////        }
////    }

////    if (importStatistics.documentCount > 0) {
////        //. 2 xml  파일 여부 확인
////        if ( !containsFilesWithExtension(validPath, "xml") ) {
////            // delecte directory
////            removeRecursively(validPath);

////            QMessageBox::warning(nullptr, "Warning", "The default file is not resolved.");

////            importStatistics.documentCount = 0;
////        }

////        //. 3 conf valid 기 true 여부 확인

////    }


//    if (importStatistics.documentCount > 0) {

//        QSettings().setValue("Project/ActiveLocations", destinationPath + "/" + name);
//        QSettings().setValue("Project/DefaultLocation", destinationPath + "/" + name);
//        QSettings().setValue("Project/Locations", destinationPath + "/" + name);

//        QMessageBox::information(nullptr, "information", "Project import is complete.");

//        emit projectSearch();
//    }

//    emit noticeMessage(createImportMessage(importStatistics, destinationPath));

//    QDir::setCurrent(savedWorkingDirectory.absolutePath());
//}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::onImportProject()
//-----------------------------------------------------------------------------
void ProjectImporter::onImportProject()
{
    onImportItem();
//    QMessageBox::information(nullptr, "information", "Project import test..");
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::importSelectedObjects()
//-----------------------------------------------------------------------------
//ProjectImporter::DocumentStatistics ProjectImporter::importSelectedObjects(
//    QVector<ImportSelectionListItem*> const& importedItems, QString const& destinationPath, const QString& sourcePath)
//{
//    //    FileList handledFiles;
//    QDir destinationFolder(destinationPath);

//    //    InputSelection userSelections;
//    DocumentStatistics importStatistics;

//    bool importSuccess = false;

//    for (int i = 0; i < importedItems.count(); i++)
//    {
//        ImportSelectionListItem const* importedItem = importedItems.at(i);
//        importSuccess = false;

//        directoryPath = libraryAccess_->getDirectoryPath(importedItem->getVLNV());
//        QString absolutePath = libraryAccess_->getPath(importedItem->getVLNV());

//        if (importedItem->getType() == ImportSelectionListItem::VLNVOJBECT)
//        {
//            QSharedPointer<Document> vlnvDocument = libraryAccess_->getModel(importedItem->getVLNV());
//            QSharedPointer<Component> component = vlnvDocument.dynamicCast<Component>();

//            if (component)
//            {
//                QVector<ImportSelectionListItem const*> importedComponentFiles =
//                    getImportedFilesForComponent(i, importedItems);

//                importSuccess = importComponent(component, importedComponentFiles, destinationFolder, handledFiles,
//                    userSelections, importStatistics);
//            }
//            else
//            {
//                importSuccess =
//                    importObject(destinationFolder, importedItem->getVLNV(), handledFiles, userSelections);
//            }

//            if (importSuccess)
//            {
//                importStatistics.documentCount++;
//            }
//        }
//    }

//    return importStatistics;
//}



//-----------------------------------------------------------------------------
// Function: ProjectImporter::onImportProject()
//-----------------------------------------------------------------------------
void ProjectImporter::requestImportProject(QString const& fileDest, QString const& filePath, QString const& filename)
{
    importSelectedObjects(fileDest, filePath, filename);
}



//-----------------------------------------------------------------------------
// Function: ProjectImporter::importSelectedObjects()
//-----------------------------------------------------------------------------
ProjectImporter::DocumentStatistics ProjectImporter::importSelectedObjects(QString const& destinationPath, const QString& sourceFile, const QString& filename)
{
    // 2. apr valid 확인


//    InputSelection userSelections;
    DocumentStatistics importStatistics;

    QStringList tarParam;
    tarParam.append("tar -xvf");
    tarParam.append(sourceFile);
    tarParam.append("-C");
    tarParam.append(destinationPath);

    QStringList chmodParam;
    chmodParam.append("chmod 777 -R " + destinationPath + "/" + filename);

    QStringList importParam;
    importParam.append(tarParam.join(" "));
    importParam.append(chmodParam.join(" "));

    QStringList runParam;
    runParam << "-c";
    runParam << importParam.join("; ");

    QProcess process;

    QCoreApplication::processEvents();
    process.start("csh", runParam);

    if (process.waitForFinished(-1))
    {
        QByteArray result = process.readAllStandardOutput();
        qDebug() << "Command output:" << result;

        importStatistics.documentCount++;
    }

    else
    {
        QByteArray error = process.readAllStandardError();
        qDebug() << "Error output:" << error;

        importStatistics.documentCount == 0;
    }

    return importStatistics;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::getImportedFilesForComponent()
//-----------------------------------------------------------------------------
QVector<ImportSelectionListItem const*> ProjectImporter::getImportedFilesForComponent(int const& componentIndex,
    QVector<ImportSelectionListItem *> const& importedItems) const
{
    QVector<ImportSelectionListItem const*> importedFiles;

    for (int i = componentIndex + 1; i < importedItems.count(); ++i)
    {
        ImportSelectionListItem const* importedItem = importedItems.at(i);
        if (importedItem->getType() == ImportSelectionListItem::FILE)
        {
            importedFiles.append(importedItem);
        }
        else if (importedItem->getType() == ImportSelectionListItem::VLNVOJBECT)
        {
            QSharedPointer<const Document> vlnvDocument =
                libraryAccess_->getModelReadOnly(importedItem->getVLNV());
            if (!vlnvDocument.dynamicCast<const Component>().isNull())
            {
                break;
            }
        }
    }

    return importedFiles;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::importComponent()
//-----------------------------------------------------------------------------
bool ProjectImporter::importComponent(QSharedPointer<Component> component,
    QVector<ImportSelectionListItem const *> importedComponentFiles, QDir const& destinationFolder,
    FileList& handledFiles, InputSelection & selections, DocumentStatistics& importStatistics)
{
    QString documentPath = libraryAccess_->getPath(component->getVlnv());
    QFileInfo documentFileInfo(documentPath);
    if (handledFiles.contains(documentFileInfo))
    {
        return false;
    }
    else
    {
        handledFiles.append(documentFileInfo);
    }

    QSharedPointer<Document> cloneDocument = component->clone();
    if (!cloneDocument)
    {
        return false;
    }

    QSharedPointer<Component> cloneComponent = cloneDocument.dynamicCast<Component>();
    if (!cloneComponent)
    {
        return false;
    }

    const QDir savedWorkingDir = QDir::current();

    QDir::setCurrent(destinationFolder.absolutePath());

    QDir vlnvTargetDirectory;
    QString directoryPath = component->getVlnv().toString("/");
    if (!vlnvTargetDirectory.mkpath(directoryPath))
    {
//        emit errorMessage(tr("Could not create directory structure, aborting."));
        QDir::setCurrent(savedWorkingDir.absolutePath());
        return false;
    }

    vlnvTargetDirectory.setPath(directoryPath);

    QString targetDirectoryPath = vlnvTargetDirectory.absolutePath();
    QDir::setCurrent(targetDirectoryPath);

    QString cloneFileName =
        cloneComponent->getVlnv().getName() + '.' + cloneComponent->getVlnv().getVersion() + ".xml";

    QFileInfo cloneComponentInfo(cloneFileName);
    bool createCloneComponent = true;
    if (QFile::exists(cloneFileName) && !setActionForExistingFile(cloneFileName, selections))
    {
        createCloneComponent = false;
    }

    //! Copy selected files.
    for (auto file : importedComponentFiles)
    {
        if (copyFile(ImportSelectionListItem::FILE, QFileInfo(file->getPath()), component->getVlnv(),
            vlnvTargetDirectory, handledFiles, selections))
        {
            importStatistics.fileCount++;
        }
    }

    //! Change the path of the contained, unselected files.
    QString originalDirectoryPath = libraryAccess_->getDirectoryPath(component->getVlnv());
    for (auto fileSet : *cloneComponent->getFileSets())
    {
        for (auto currentFile : *fileSet->getFiles())
        {
            if (!QFile::exists(currentFile->name()))
            {
                QFileInfo currentFileInfo(currentFile->name());
                if (QFile::exists(currentFileInfo.fileName()))
                {
                    currentFile->setName(currentFileInfo.fileName());
                }
                else
                {
                    changeFilePath(currentFile, fileSet, component, originalDirectoryPath, vlnvTargetDirectory);
                }
            }
        }
    }

    if (createCloneComponent)
    {
        // Bypass library since imported items are not stored in library.

        if (DocumentFileAccess::writeDocument(cloneComponent, cloneFileName) == false)
        {
//            emit errorMessage(QObject::tr("Could not open file %1 for writing.").arg(cloneFileName));
        }
    }

    QDir::setCurrent(savedWorkingDir.absolutePath());
    return createCloneComponent;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::changeFilePath()
//-----------------------------------------------------------------------------
void ProjectImporter::changeFilePath(QSharedPointer<File> file, QSharedPointer<FileSet> containingFileSet,
    QSharedPointer<Component> originalComponent, QString const& originalComponentPath,
    QDir const& destinationFolder)
{
    for (auto fileSet : *originalComponent->getFileSets())
    {
        if (fileSet->name() == containingFileSet->name())
        {
            for (auto originalFile : *fileSet->getFiles())
            {
                if (file->name() == originalFile->name())
                {
                    QString originalPath = originalComponentPath + '/' + originalFile->name();
                    QString directoryRelativePath = destinationFolder.relativeFilePath(originalPath);

                    file->setName(directoryRelativePath);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::importObject()
//-----------------------------------------------------------------------------
bool ProjectImporter::importObject(QDir const& destinationFolder, VLNV const& vlnv, FileList& handledFiles,
    InputSelection &selections)
{
    if (libraryAccess_->contains(vlnv) == false)
    {
        return false;
    }

    const QDir savedWorkingDir = QDir::current();

    QDir::setCurrent(destinationFolder.absolutePath());

    QDir vlnvTargetDirectory;
    QString directoryPath = vlnv.toString("/");
    if (!vlnvTargetDirectory.mkpath(directoryPath))
    {
//        emit errorMessage(tr("Could not create directory structure, aborting."));
        QDir::setCurrent(savedWorkingDir.absolutePath());
        return false;
    }

    vlnvTargetDirectory.setPath(directoryPath);

    QString documentPath = libraryAccess_->getPath(vlnv);
    QFileInfo documentFileInfo(documentPath);

    bool fileWasImported = copyFile(ImportSelectionListItem::VLNVOJBECT, documentFileInfo, vlnv,
        vlnvTargetDirectory, handledFiles, selections);

    QDir::setCurrent(savedWorkingDir.absolutePath());
    return fileWasImported;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::copyFile()
//-----------------------------------------------------------------------------
bool ProjectImporter::copyFile(ImportSelectionListItem::ItemType itemType, QFileInfo const& source,
    VLNV const& containingVLNV, QDir& targetDirectory, FileList& handledFiles, InputSelection& selections)
{
    if (handledFiles.contains(source))
    {
        return false;
    }

    if (source.exists() == false)
    {
        QString message(tr("Could not find file: %1").arg(source.fileName()));
        if (itemType == ImportSelectionListItem::FILE)
        {
            message += QStringLiteral(" in component ") + containingVLNV.toString();
        }

//        emit errorMessage(message);
        return false;
    }

    // Add file to handledFiles list to indicate that we don't need to check later in possible recursive calls.
    handledFiles.append(source);

    // save the current directory to be restored at the end of the function
    QDir savedCurrentDir = QDir::current();

    QString targetFileName = source.fileName();
    QDir::setCurrent(targetDirectory.absolutePath());
    if (QFile::exists(targetFileName))
    {
        if (!setActionForExistingFile(targetFileName, selections))
        {
            QDir::setCurrent(savedCurrentDir.absolutePath());
            return false;
        }
    }

    QFile sourceFile(source.filePath());

    if (!sourceFile.copy(targetFileName))
    {
//        emit errorMessage(tr("File %1 couldn't be copied").arg(source.fileName()));
    }

    // restore the current directory to the state it was before this function
    QDir::setCurrent(savedCurrentDir.absolutePath());
    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::setActionForExistingFile()
//-----------------------------------------------------------------------------
bool ProjectImporter::setActionForExistingFile(QString const& targetFileName, InputSelection& selections) const
{
    QMessageBox::StandardButton answer = QMessageBox::No;

    if (selections.noToAll)
    {
        answer = QMessageBox::No;
    }
    else if (selections.yesToAll)
    {
        answer = QMessageBox::Yes;
    }
    else
    {
        // If "yes to all" or "no to all" has not been clicked, ask user what to do.
        answer = QMessageBox::question(dialogParentWidget_, tr("Overwrite file?"),
            tr("The file %1 already exists, would you like to overwrite the file?").arg(targetFileName),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll,
            QMessageBox::No);

        if (answer == QMessageBox::YesToAll)
        {
            selections.yesToAll = true;
        }
        else if (answer == QMessageBox::NoToAll)
        {
            selections.noToAll = true;
        }
    }

    if (answer == QMessageBox::Yes || answer == QMessageBox::YesToAll)
    {
        QFile fileToOverwrite(targetFileName);
        fileToOverwrite.remove();
    }
    else if (answer == QMessageBox::No || answer == QMessageBox::NoToAll)
    {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectImporter::createImportMessage()
//-----------------------------------------------------------------------------
QString ProjectImporter::createImportMessage(DocumentStatistics const& importStatistics,
    QString const& destinationPath) const
{
    QString importMessage = "Imported ";

    if (importStatistics.documentCount > 0)
    {
        importMessage += QString("%1 VLNV item(s)").arg(QString::number(importStatistics.documentCount));

        if (importStatistics.fileCount > 0)
        {
            importMessage += QString(" and %1 file(s)").arg(QString::number(importStatistics.fileCount));
        }
    }
    else if (importStatistics.fileCount > 0)
    {
        importMessage += QString("%1 file(s)").arg(QString::number(importStatistics.fileCount));
    }
    else
    {
        importMessage += "0 items";
    }

    importMessage += QString(" to %1.").arg(destinationPath);

    return importMessage;
}
