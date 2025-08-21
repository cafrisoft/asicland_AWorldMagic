/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectTreeView.cpp
 *      Project: AWM
 *      Description :
 *          Exports the selected library items.
 */

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

#include <UI/Dialogs/ProjectExportDialog/ProjectExportDialog.h>

#include "ProjectItemSelectionFactory.h"

//#include "MessageMediator.h"

#include "ProjectExporter.h"


//-----------------------------------------------------------------------------
// Function: ProjectExporter::ProjectExporter()
//-----------------------------------------------------------------------------
ProjectExporter::ProjectExporter(LibraryInterface* libraryAccess,
    QWidget* dialogParentWidget, QObject* parent):
QObject(parent),
libraryAccess_(libraryAccess),
dialogParentWidget_(dialogParentWidget)
{

}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::onExportItem()
//-----------------------------------------------------------------------------
void ProjectExporter::onExportItem(VLNV const& vlnv)
{
    onExportItems("", QList<VLNV>() << vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::onExportItems()
//-----------------------------------------------------------------------------
void ProjectExporter::onExportItems(const QString& name, const QList<VLNV> vlnvs)
{
    if (vlnvs.isEmpty())
    {
        return;
    }

    // get the current working directory and save it to be restored in the end of the function
    QDir savedWorkingDirectory = QDir::current();

    ProjectExportDialog* exportDialog = new ProjectExportDialog(dialogParentWidget_);
//    ProjectItemSelectionFactory::constructItemsForSelectionDialog(libraryAccess_, exportDialog, vlnvs);

    if (exportDialog->exec() == QDialog::Rejected)
    {
        return;
    }

    QString destinationPath = exportDialog->getTargetDirectory();
    QString sourcePath = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString() + "/" + name;

//    DocumentStatistics exportStatistics = exportSelectedObjects(exportDialog->getSelectedItems(), destinationPath, sourcePath);
    DocumentStatistics exportStatistics = exportSelectedObjects(destinationPath, sourcePath, name);

    if (exportStatistics.documentCount > 0) {
        QMessageBox::information(nullptr, "information", "Project export has been completed..");
    }

    QDir::setCurrent(savedWorkingDirectory.absolutePath());
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::onExportItem()
//-----------------------------------------------------------------------------
void ProjectExporter::onExportItem()
{
    QString dir = QFileDialog::getExistingDirectory(nullptr, "Select a directory for export",
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty()) {
        return;
    }

    QString destinationPath = dir;

    QString sourcePath = SettingManager::getDefaultProjectLocation();

    QDir companyDirectory(sourcePath);
    QString companyName = companyDirectory.dirName(); // 디렉토리명

    DocumentStatistics exportStatistics = exportSelectedObjects(destinationPath, sourcePath, companyName);

    if (exportStatistics.documentCount > 0) {
        QMessageBox::information(nullptr, "information", "Project export has been completed..");

        QString inputText = "export " + destinationPath;
        emit inputMessage(inputText);

        QString resultText = "[ INFO ] | Exported | Project exported.";
        emit noticeMessage(resultText);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::onExportItem()
//-----------------------------------------------------------------------------
//void ProjectExporter::onExportItem()
//{
//    // get the current working directory and save it to be restored in the end of the function
//    QDir savedWorkingDirectory = QDir::current();

//    ProjectExportDialog* exportDialog = new ProjectExportDialog(dialogParentWidget_);
////    ProjectItemSelectionFactory::constructItemsForSelectionDialog(libraryAccess_, exportDialog, vlnvs);

//    if (exportDialog->exec() == QDialog::Rejected)
//    {
//        return;
//    }

//    QString destinationPath = exportDialog->getTargetDirectory();
//    QString sourcePath = SettingManager::getDefaultProjectLocation();

//    QDir companyDirectory(sourcePath);
//    QString companyName = companyDirectory.dirName(); // 디렉토리명

//    DocumentStatistics exportStatistics = exportSelectedObjects(destinationPath, sourcePath, companyName);

//    if (exportStatistics.documentCount > 0) {
//        QMessageBox::information(nullptr, "information", "Project export has been completed..");
//    }

//    emit noticeMessage(createExportMessage(exportStatistics, destinationPath));

//    QDir::setCurrent(savedWorkingDirectory.absolutePath());
//}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::onExportProject()
//-----------------------------------------------------------------------------
void ProjectExporter::onExportProject()
{
    onExportItem();
//    QMessageBox::information(nullptr, "information", "Project export test..");
}

void ProjectExporter::compressFiles(const QStringList &inputFileNames, const QString &outputFileName)
{
//    QFile outputFile(outputFileName);

//    if (outputFile.open(QIODevice::WriteOnly))
//    {
//        QCompressor compressor(QCompressor::Gzip);

//        // Combine data from multiple files into a single QByteArray
//        QByteArray combinedData;
//        foreach (const QString &inputFileName, inputFileNames)
//        {
//            QFile inputFile(inputFileName);
//            if (inputFile.open(QIODevice::ReadOnly))
//            {
//                combinedData.append(inputFile.readAll());
//                inputFile.close();
//            }
//            else
//            {
//                // Handle file open error
//                qWarning() << "Failed to open file:" << inputFileName;
//            }
//        }

//        // Set the combined uncompressed data
//        compressor.addData(combinedData);

//        // Get the compressed data
//        QByteArray compressedData = compressor.result();

//        // Write the compressed data to the output file
//        QDataStream out(&outputFile);
//        out << compressedData;

//        outputFile.close();
//    }
//    else
//    {
//        // Handle output file open error
//        qWarning() << "Failed to open output file:" << outputFileName;
//    }
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::exportSelectedObjects()
//-----------------------------------------------------------------------------
//ProjectExporter::DocumentStatistics ProjectExporter::exportSelectedObjects(
//    QVector<ProjectSelectionListItem*> const& exportedItems, QString const& destinationPath, const QString& sourcePath)
//{
////    FileList handledFiles;
//    QDir destinationFolder(destinationPath);

////    InputSelection userSelections;
//    DocumentStatistics exportStatistics;

//    bool exportSuccess = false;

//    for (int i = 0; i < exportedItems.count(); i++)
//    {
//        ProjectSelectionListItem const* exportedItem = exportedItems.at(i);
//        exportSuccess = false;

//        directoryPath = libraryAccess_->getDirectoryPath(exportedItem->getVLNV());
//        QString absolutePath = libraryAccess_->getPath(exportedItem->getVLNV());

//        if (exportedItem->getType() == ProjectSelectionListItem::VLNVOJBECT)
//        {
//            QSharedPointer<Document> vlnvDocument = libraryAccess_->getModel(exportedItem->getVLNV());
//            QSharedPointer<Component> component = vlnvDocument.dynamicCast<Component>();

//            if (component)
//            {
//                QVector<ProjectSelectionListItem const*> exportedComponentFiles =
//                    getExportedFilesForComponent(i, exportedItems);

//                exportSuccess = exportComponent(component, exportedComponentFiles, destinationFolder, handledFiles,
//                    userSelections, exportStatistics);
//            }
//            else
//            {
//                exportSuccess =
//                    exportObject(destinationFolder, exportedItem->getVLNV(), handledFiles, userSelections);
//            }

//            if (exportSuccess)
//            {
//                exportStatistics.documentCount++;
//            }
//        }
//    }

//    return exportStatistics;
//}



//-----------------------------------------------------------------------------
// Function: ProjectImporter::requestExportProject()
//-----------------------------------------------------------------------------
void ProjectExporter::requestExportProject(QString const& fileDest, QString const& filePath, QString const& filename)
{
    exportSelectedObjects(fileDest, filePath, filename);
}


//-----------------------------------------------------------------------------
// Function: ProjectExporter::exportSelectedObjects()
//-----------------------------------------------------------------------------
ProjectExporter::DocumentStatistics ProjectExporter::exportSelectedObjects(QString const& destinationPath, const QString& sourcePath, const QString& filename)
{
    DocumentStatistics exportStatistics;

    QDir dir(sourcePath);
    dir.cdUp();

    QString parentPath = dir.absolutePath();

    QStringList tarParam;
    tarParam.append("tar -cvf");
    tarParam.append(destinationPath + "/" + filename + ".apr");
    tarParam.append("-C");
    tarParam.append(parentPath);
    tarParam.append(filename);

    QStringList exportParam;
    exportParam.append(tarParam.join(" "));

    QStringList runParam;
    runParam << "-c";
    runParam << exportParam.join("; ");

    QProcess process;

    QCoreApplication::processEvents();
    process.start("csh", runParam);

    if (process.waitForFinished(-1))
    {
        QByteArray result = process.readAllStandardOutput();
        qDebug() << "Command output:" << result;
    }

    else
    {
        QByteArray error = process.readAllStandardError();
        qDebug() << "Error output:" << error;
    }

    exportStatistics.documentCount++;

    return exportStatistics;
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::getExportedFilesForComponent()
//-----------------------------------------------------------------------------
QVector<ProjectSelectionListItem const*> ProjectExporter::getExportedFilesForComponent(int const& componentIndex,
    QVector<ProjectSelectionListItem *> const& exportedItems) const
{
    QVector<ProjectSelectionListItem const*> exportedFiles;

    for (int i = componentIndex + 1; i < exportedItems.count(); ++i)
    {
        ProjectSelectionListItem const* exportedItem = exportedItems.at(i);
        if (exportedItem->getType() == ProjectSelectionListItem::FILE)
        {
            exportedFiles.append(exportedItem);
        }
        else if (exportedItem->getType() == ProjectSelectionListItem::VLNVOJBECT)
        {
            QSharedPointer<const Document> vlnvDocument =
                libraryAccess_->getModelReadOnly(exportedItem->getVLNV());
            if (!vlnvDocument.dynamicCast<const Component>().isNull())
            {
                break;
            }
        }
    }

    return exportedFiles;
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::exportComponent()
//-----------------------------------------------------------------------------
bool ProjectExporter::exportComponent(QSharedPointer<Component> component,
    QVector<ProjectSelectionListItem const *> exportedComponentFiles, QDir const& destinationFolder,
    FileList& handledFiles, InputSelection & selections, DocumentStatistics& exportStatistics)
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
    for (auto file : exportedComponentFiles)
    {
        if (copyFile(ProjectSelectionListItem::FILE, QFileInfo(file->getPath()), component->getVlnv(),
            vlnvTargetDirectory, handledFiles, selections))
        {
            exportStatistics.fileCount++;
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
        // Bypass library since exported items are not stored in library.

        if (DocumentFileAccess::writeDocument(cloneComponent, cloneFileName) == false)
        {
//            emit errorMessage(QObject::tr("Could not open file %1 for writing.").arg(cloneFileName));
        }
    }

    QDir::setCurrent(savedWorkingDir.absolutePath());
    return createCloneComponent;
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::changeFilePath()
//-----------------------------------------------------------------------------
void ProjectExporter::changeFilePath(QSharedPointer<File> file, QSharedPointer<FileSet> containingFileSet,
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
// Function: ProjectExporter::exportObject()
//-----------------------------------------------------------------------------
bool ProjectExporter::exportObject(QDir const& destinationFolder, VLNV const& vlnv, FileList& handledFiles,
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

    bool fileWasExported = copyFile(ProjectSelectionListItem::VLNVOJBECT, documentFileInfo, vlnv,
        vlnvTargetDirectory, handledFiles, selections);

    QDir::setCurrent(savedWorkingDir.absolutePath());
    return fileWasExported;
}

//-----------------------------------------------------------------------------
// Function: ProjectExporter::copyFile()
//-----------------------------------------------------------------------------
bool ProjectExporter::copyFile(ProjectSelectionListItem::ItemType itemType, QFileInfo const& source,
    VLNV const& containingVLNV, QDir& targetDirectory, FileList& handledFiles, InputSelection& selections)
{
    if (handledFiles.contains(source))
    {
        return false;
    }

    if (source.exists() == false)
    {
        QString message(tr("Could not find file: %1").arg(source.fileName()));
        if (itemType == ProjectSelectionListItem::FILE)
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
// Function: ProjectExporter::setActionForExistingFile()
//-----------------------------------------------------------------------------
bool ProjectExporter::setActionForExistingFile(QString const& targetFileName, InputSelection& selections) const
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
// Function: ProjectExporter::createExportMessage()
//-----------------------------------------------------------------------------
QString ProjectExporter::createExportMessage(DocumentStatistics const& exportStatistics,
    QString const& destinationPath) const
{
    QString exportMessage = "Exported ";

    if (exportStatistics.documentCount > 0)
    {
        exportMessage += QString("%1 VLNV item(s)").arg(QString::number(exportStatistics.documentCount));

        if (exportStatistics.fileCount > 0)
        {
            exportMessage += QString(" and %1 file(s)").arg(QString::number(exportStatistics.fileCount));
        }
    }
    else if (exportStatistics.fileCount > 0)
    {
        exportMessage += QString("%1 file(s)").arg(QString::number(exportStatistics.fileCount));
    }
    else
    {
        exportMessage += "0 items";
    }

    exportMessage += QString(" to %1.").arg(destinationPath);

    return exportMessage;
}
