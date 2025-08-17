/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: NewProjectPage.h
 *      Project: AWM
 *      Description :
 *          Project editor group box.
 */

#ifndef PROJECTEDITOR_H
#define PROJECTEDITOR_H

//#include "VLNVDataTree.h"
//#include "VLNVContentMatcher.h"

#include <KactusAPI/Project.h>
#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>

#include <QGroupBox>
#include <QLineEdit>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QLabel>

class LibraryInterface;
class LibraryItem;
class AssistedLineEdit;

//-----------------------------------------------------------------------------
//! ProjectEditor class.
//-----------------------------------------------------------------------------
class ProjectEditor : public QGroupBox
{
    Q_OBJECT

public:

    /*!
     *  The constructor.
     *
     *      @param [in] libHandler  The library handler.
     *      @param [in] parentWnd   The parent window (having a title bar).
     *      @param [in] parent      The parent widget.
     */
    ProjectEditor(LibraryInterface* libHandler, QWidget* parentWnd = 0, QWidget* parent = 0);

    /*!
     *  The destructor.
     */
    virtual ~ProjectEditor();

    /*!
     *  Adds a new name extension that will be shown in the dialog.
     *
     *      @param [in] extension The extension to add.
     */
    void addNameExtension(QString const& extension);

    /*!
     *  Generates a project that matches the contents of the editor.
     */
    Project getProject() const;

    /*!
     *  Returns true if the contents of the editor are valid for a project.
     */
    bool isValid() const;

	/*!
	 * Checks if the editor contains text in any field.
	 */
	bool isEmpty() const;

signals:

    /*!
     *  Emitted when the contents of the editor have changed.
     */
    void contentChanged();

public slots:
	
    /*!
     *  Set the widget to be mandatory or not. The default setting is mandatory on.
	 *
	 *      @param [in] mandatory   If true then all 4 vlnv fields are displayed as mandatory fields.
	 */
	void setMandatory(bool mandatory);

	/*! 
     *  Set the contents of the editor to match the given project.
	 *
     *		@param [in] project The project.
	 */
    void setProject(Project const& project);

    /*!
     *  Updates the data tree.
     */
    void updateFiltering();

    /*!
     *  Updates all matcher items based on the contents of the VLNV editor fields.
     */
    void updateMatcherItems();

    /*!
     *  Called when the vendor field has changed.
     */
    void updateLibraryMatcherItem();

    /*!
     *  Called when the library field has changed.
     */
    void updateNameMatcherItem();

    /*!
     *  Called when the name field has changed.
     */
    void updateVersionMatcherItem();

    /*!
     *  Sets the name-field to contain the given string.
     */
    void setName(QString const& name);

    /*!
     *  Sets the version-field to contain the given string.
     */
    void setVersion(QString const& version);

	/*!
     *  Sets the author-field to contain the given string.
     */
    void setAuthor(QString const& author);

	/*!
     *  Sets the description-field to contain the given string.
     */
    void setDescription(QString const& description);

protected:
	
    /*!
     *  Handler for drop events on drag & drop
	 *
	 *      @param [in] event   Pointer to the drop event.
	 */
	virtual void dropEvent(QDropEvent* event);

	/*!
     *  Handler for drag enter events in drag & drop.
	 *
	 *      @param [in] event   Pointer to the event.
	 */
	virtual void dragEnterEvent(QDragEnterEvent* event);
    
    /*!
     *  Handler for the show event.
     *
     *      @param [in] event   Pointer to the show event.
     */
    virtual void showEvent(QShowEvent* event);

private slots:

    /*!
     *  Refreshes the filtering and matcher items.
     */
    void refresh();

private:
    // Disable copying.
    ProjectEditor(ProjectEditor const& rhs);
    ProjectEditor& operator=(ProjectEditor const& rhs);

    /*!
     *  Initializes the Qt connections between signals and slots.
     */
    void initConnections();

    /*!
     *  Initializes the widgets of the project editor.
     *
     *      @param [in] parentWnd   The parent window.
     */
    void initWidgets(QWidget* parentWnd);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The VLNV types which to show in content assist.
//    QList<VLNV::IPXactType> contentTypes_;

    //! True if the filtering is "dirty" and needs to be updated.
    bool dirty_;

    //! The VLNV data tree.
//    VLNVDataTree dataTree_;

    //! Line edit for the name element.
    AssistedLineEdit* nameEdit_;

    //! Name extension label.
    QLabel nameExtensionLabel_;

    //! Content matcher for the name field.
//    VLNVContentMatcher nameMatcher_;

    //! Line edit for the version element.
    AssistedLineEdit* versionEdit_;

    //! Content matcher for the version field.
//    VLNVContentMatcher versionMatcher_;

    //! Line edit for the author element.
    AssistedLineEdit* authorEdit_;

    //! Content matcher for the author field.
//    VLNVContentMatcher authorMatcher_;

    //! Line edit for the description element.
    AssistedLineEdit* descriptionEdit_;

    //! Content matcher for the description field.
//    VLNVContentMatcher descriptionMatcher_;

    //! Pointer to the instance that manages the library
    LibraryInterface* handler_;

    //! Implementation filter data.
    bool implementationFilterEnabled_;

    //! Implementation filter.
    KactusAttribute::Implementation implementationFilter_;
};

//-----------------------------------------------------------------------------

#endif // PROJECTEDITOR_H
