/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectImportDialog.h
 *      Project: AWM
 *      Description :
 *          Dialog for selecting the imported items.
 */

#ifndef PROJECTIMPORTDIALOG_H
#define PROJECTIMPORTDIALOG_H

#include <UI/Dialogs/ImportSelectionDialog/ImportSelectionDialog.h>

#include <QPushButton>
#include <QLineEdit>

//-----------------------------------------------------------------------------
//! Dialog for selecting the imported items.
//-----------------------------------------------------------------------------
class ProjectImportDialog : public ImportSelectionDialog
{
	Q_OBJECT

public:

    /*!
	 *  The Constructor.
	 *
	 *      @param [in] parent          The parent widget.
	 *      @param [in] f               Widget flags for the dialog.
	 */
    ProjectImportDialog(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

    /*!
	 *  The destructor.
	 */
    virtual ~ProjectImportDialog();

    /*!
     *  Get the path to the destination directory.
     *
     *      @return The path to the destination directory.
     */
    QString getSourceFile() const;

private slots:

    /*!
     *  Open up a dialog for selecting the destination path for the import.
     */
    void onBrowseTarget();

    /*!
     *  Handles the changes in the changing of the destination path.
     *
     *      @param [in] newDestination  The new destination path.
     */
    void onDestinationFolderChanged(const QString& newDestination);

protected:

    /*!
     *  Connect the signals for folder browsing.
     */
    virtual void connectSignals();

    /*!
     *  Setup the dialog layout.
     */
    virtual void setupLayout();

private:

	//! No copying. No assignment.
    ProjectImportDialog(const ProjectImportDialog& other);
    ProjectImportDialog& operator=(const ProjectImportDialog& other);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Editor for the destination folder.
    QLineEdit folderPathEditor_;

    //! Button for browsing the destination folder.
    QPushButton folderBrowseButton_;

    //! The current path.
    QString currentPath_;
};

#endif // PROJECTIMPORTDIALOG_H
