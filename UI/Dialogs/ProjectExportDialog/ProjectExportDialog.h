/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectExportDialog.h
 *      Project: AWM
 *      Description :
 *          Dialog for selecting the exported items.
 */

#ifndef PROJECTEXPORTDIALOG_H
#define PROJECTEXPORTDIALOG_H

#include <UI/Dialogs/ProjectSelectionDialog/ProjectSelectionDialog.h>

#include <QPushButton>
#include <QLineEdit>

//-----------------------------------------------------------------------------
//! Dialog for selecting the exported items.
//-----------------------------------------------------------------------------
class ProjectExportDialog : public ProjectSelectionDialog
{
	Q_OBJECT

public:

    /*!
	 *  The Constructor.
	 *
	 *      @param [in] parent          The parent widget.
	 *      @param [in] f               Widget flags for the dialog.
	 */
    ProjectExportDialog(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

    /*!
	 *  The destructor.
	 */
    virtual ~ProjectExportDialog();

    /*!
     *  Get the path to the destination directory.
     *
     *      @return The path to the destination directory.
     */
    QString getTargetDirectory() const;

private slots:

    /*!
     *  Open up a dialog for selecting the destination path for the export.
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
    ProjectExportDialog(const ProjectExportDialog& other);
    ProjectExportDialog& operator=(const ProjectExportDialog& other);

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

#endif // PROJECTEXPORTDIALOG_H
