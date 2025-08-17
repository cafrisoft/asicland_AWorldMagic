/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectImporter.h
 *      Project: AWM
 *      Description :
 *          Imports the selected library items.
 */

#ifndef PROJECTIMPORTER_H
#define PROJECTIMPORTER_H

#include <IPXACTmodels/common/VLNV.h>

#include <UI/Dialogs/ImportSelectionDialog/ImportSelectionListItem.h>

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QString>
#include <QWidget>

using FileList = QVector<QFileInfo>;

class LibraryInterface;
class Component;
class File;
class FileSet;

//-----------------------------------------------------------------------------
//! Imports the selected library items.
//-----------------------------------------------------------------------------
class ProjectImporter : public QObject
{
    Q_OBJECT

public:

    /*!
     *  The constructor.
     *
     *      @param [in] libraryAccess       Access to the library.
     *      @param [in] dialogParentWidget  Parent widget for the import dialog.
     *      @param [in] parent              The parent object.
     */
    ProjectImporter(LibraryInterface* libraryAccess,
        QWidget* dialogParentWidget, QObject* parent = nullptr);

    /*!
     *  The destructor.
     */
    virtual ~ProjectImporter() = default;

    //! No copying. No assignment.
    ProjectImporter(const ProjectImporter &other) = delete;
    ProjectImporter &operator=(const ProjectImporter &other) = delete;

    void requestImportProject(QString const& fileDest, QString const& filePath, QString const& filename);

public slots:

    /*!
     *  Import a library item and it's subitems to a new location.
     *
     */
//    void onImportItems();

    /*!
     *  Import a group of items to a new location
     *
     */
    void onImportItem();

    void onImportProject();

signals:
    void inputMessage(QString const&);
    void noticeMessage(QString const&);
    void errorMessage(QString const&);
    void undefinedMessage(QString const&);

    void projectSearch();

private:

    //! Struct for collecting document statistics e.g. in import.
    struct DocumentStatistics
    {
        int fileCount = 0;
        int documentCount = 0;
    };

    struct InputSelection
    {
        bool yesToAll = false;
        bool noToAll = false;
    };

    /*!
     *  Import the selected objects.
     *
     *      @param [in] importedItems       The selected items to be imported.
     *      @param [in] destinationPath     The import destination path.
     *
     *      @return The count of the imported items in a pair of 1. VLNVs and 2. files.
     */
//    DocumentStatistics importSelectedObjects(QVector<ImportSelectionListItem*> const& importedItems,
//        QString const& destinationPath, const QString& sourcePath);
    DocumentStatistics importSelectedObjects(QString const& destinationPath, const QString& sourceFile, const QString& filename);

    /*!
     *  Get the selected imported files of the current component.
     *
     *      @param [in] componentIndex  Index of the selected component.
     *      @param [in] importedItems   List of all the selected import items.
     *
     *      @return List of the imported files of the current component.
     */
    QVector<ImportSelectionListItem const*> getImportedFilesForComponent(int const& componentIndex,
        QVector<ImportSelectionListItem*> const& importedItems) const;

    /*!
     *  Import the selected component document.
     *
     *      @param [in] component               The selected component.
     *      @param [in] importedComponentFiles  Selected files to be imported with the component.
     *      @param [in] destinationFolder       Destination folder for the import.
     *      @param [in] handledFiles            List of imported files.
     *      @param [in] selections              Selections for overwriting existing files.
     *      @param [in] importStatistics        Import statistics.
     *
     *      @return True, if the import was a success, false otherwise.
     */
    bool importComponent(QSharedPointer<Component> component,
        QVector<ImportSelectionListItem const*> importedComponentFiles, QDir const& destinationFolder,
        FileList& handledFiles, InputSelection & selections, DocumentStatistics& importStatistics);

    /*!
     *  Change the path of the selected file.
     *
     *      @param [in] file                    The selected file.
     *      @param [in] containingFileSet       File set containing the file.
     *      @param [in] originalComponent       The original component.
     *      @param [in] originalComponentPath   Path to the original component.
     *      @param [in] destinationFolder       Destination folder for the import.
     */
    void changeFilePath(QSharedPointer<File> file, QSharedPointer<FileSet> containingFileSet,
        QSharedPointer<Component> originalComponent, QString const& originalComponentPath,
        QDir const& destinationFolder);

    /*!
     *  Import the selected VLNV object.
     *
     *      @param [in] destinationFolder   Destination folder for the import.
     *      @param [in] vlnv                VLNV of the imported item.
     *      @param [in] handledFiles        List of the files that have been handled.
     *      @param [in] selections          Container for yes/no to all status.
     *
     *      @return True, if the selected VLNV object was imported, false otherwise.
     */
    bool importObject(QDir const& destinationFolder, VLNV const& vlnv, FileList& handledFiles,
        InputSelection &selections);
    
    /*!
     *  Copy a single file.
     *
     *      @param [in] itemType                Type of the copied file.
     *      @param [in] source                  The source file to be copied
     *      @param [in] containingVLNV          VLNV of the containing document.
     *      @param [in/out] targetDirectory     The directory where the file is copied to.
     *      @param [in/out] handledFiles        List of files that have been copied
     *      @param [in/out] yesToAll            Info if user has selected "yes to all" to overwrite
     *      @param [in/out] noToAll             Info is user has selected "No to all" to overwrite
     *
     *      @return True, if the file was copied, false otherwise.
     */
    bool copyFile(ImportSelectionListItem::ItemType itemType, QFileInfo const& source, VLNV const& containingVLNV,
        QDir& targetDirectory, FileList& handledFiles, InputSelection& selections);

    /*!
     *  Set the action for overwriting an existing file.
     *
     *      @param [in] targetFileName  Name of the selected file.
     *      @param [in] selections      Selections made for overwriting an existing file.
     *
     *      @return True, if the existing file is to be overwritten, false otherwise.
     */
    bool setActionForExistingFile(QString const& targetFileName, InputSelection& selections) const;

    /*!
     *  Create the import message.
     *
     *      @param [in] vlnvCount           The amount of imported VLNV items.
     *      @param [in] fileCount           The amount of imported files.
     *      @param [in] destinationPath     The import destination.
     *
     *      @return The constructed import message.
     */
    QString createImportMessage(DocumentStatistics const& importStatistics, QString const& destinationPath) const;

    QString containsFileWithExtension(const QString& directoryPath, const QString& targetExtension);
    bool containsFilesWithExtension(const QString& directoryPath, const QString& targetExtension);
    bool removeRecursively(const QString& directoryPath);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    
    //! Access to the library.
    LibraryInterface* libraryAccess_;

    //! Parent widget for the import dialog.
    QWidget* dialogParentWidget_;
};

#endif // PROJECTIMPORTER_H
