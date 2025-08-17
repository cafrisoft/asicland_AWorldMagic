/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSettingsDialog.h
 *      Project: AWM
 *      Description :
 *          Dialog for configuring project locations.
 */


#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>
#include <QFileIconProvider>
#include <QPushButton>
#include <QSettings>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QIcon>

//-----------------------------------------------------------------------------
//! The dialog to display project locations and select active and default project paths.
//-----------------------------------------------------------------------------
class ProjectSettingsDialog : public QDialog
{
    Q_OBJECT

public:

    /*!
     *  Constructor.
     *
     *      @param [in/out] settings  The settings store.
     *      @param [in]     parent    The parent widget.
     */
    ProjectSettingsDialog(QSettings& settings, QWidget* parent = 0);

    /*!
     *  Destructor.
     */
    ~ProjectSettingsDialog();

    /*!
     *  Validates the contents of the page thoroughly.
     *
     *      @return True, if the contents are valid. False, if they are invalid.
     */
    bool validate();

    /*!
     *  Applies the changes that were done in the page.
     */
    void apply();

public slots:

    /*!
     *  Adds a new directory to the project locations.
     */
    void addLocation();

    /*!ProjectSettingsDialog_H
     *  Removes the currently selected project location.
     */
    void removeLocation();

	/*!
     *  Called when a location is selected iProjectSettingsDialog_Hn the locations list.
	 *
	 *      @param [in] cur     Pointer to the selected item.
	 *      @param [in] prev    Pointer to the previously selected item.
     */
	void onSelectLocation(QTableWidgetItem* cur, QTableWidgetItem* prev);

signals:
	
    /*!
     *  Emitted when the project settings has changed and a scan should be performed.
     */
    void scanProject();

private slots:

	/*!
     *  Handler for item clicks on the lib locations list.
	 *
	 *      @param [in] item    Pointer to the clicked item.
     */
	void onItemClicked(QTableWidgetItem* item);

    /*!
     *  Called when the user presses OK.
     */
    virtual void accept();

private:

    // Disable copying.
    ProjectSettingsDialog(ProjectSettingsDialog const& rhs);
    ProjectSettingsDialog& operator=(ProjectSettingsDialog const& rhs);

    /*!
     *  Loads the settings from the settings store.
     */
    void loadSettings();

    /*!
     *  Creates a row in the project path table for the given directory.
     *
     *      @param [in] directory   The directory to add as project path.
     *      @param [in] isActive    Is the directory is set active or not.
     *      @param [in] isDefault   Is the directory is set default or not.
     */
    void createRowForDirectory(QString const& directory, bool isActive, bool isDefault);

    /*!
     *  Change the default path for the project.
     *
     *      @param [in] item    Table item for the new default path.
     */
    void changeDefaultPath(QTableWidgetItem* item);

    /*!
     *  Change the active status of the selected project path.
     *
     *      @param [in] item    The selected table item.
     */
    void changeActivePathStatus(QTableWidgetItem* item);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

     //! The settings store.
	 QSettings& settings_;

     //! Icon provider for directory folders.
     QFileIconProvider iconProvider_;

     //! Project locations table
     QTableWidget* prjLocationsTable_;

	 //! Add location button.
	 QPushButton* addLocationButton_;

	 //! Remove location button.
	 QPushButton* removeLocationButton_;

	 //! OK button.
	 QPushButton* okButton_;

     //! Holds the info on if the user has changed the project or not.
	 bool changed_;

     //! Check icon for the active and default project paths.
     QIcon checkMarkIcon_;
};

//-----------------------------------------------------------------------------

#endif // PROJECTSETTINGSDIALOG_H
