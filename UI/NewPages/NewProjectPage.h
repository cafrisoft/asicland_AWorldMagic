/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: NewProjectPage.h
 *      Project: AWM
 *      Description :
 *          New project page.
 */

#ifndef NEWPROJECTPAGE_H
#define NEWPROJECTPAGE_H

#include "NewPage.h"

class Project;
class LibraryInterface;
class ProjectEditor;
class ProjectSelectorWidget;

//-----------------------------------------------------------------------------
//! New project page.
//-----------------------------------------------------------------------------
class NewProjectPage : public PropertyPageView
{
	Q_OBJECT

public:

    NewProjectPage(LibraryInterface* libHandler, QWidget *parent);
	
	//! The destructor.
    virtual ~NewProjectPage();

    /*!
     *  Pre-validates the contents of the page. This is used for enabling/disabling the OK button.
     *
     *      @return True, if the contents are valid and OK button should be enabled.
     *              False, if the contents are invalid and OK button should be disabled.
     *
     *      @remarks Must not show any message boxes.
     */
    virtual bool prevalidate() const;

    /*!
     *  Validates the contents of the page thoroughly.
     *
     *      @return True, if the contents are valid. False, if they are invalid.
     *
     *      @remarks Showing message boxes for errors is allowed.
     */
    virtual bool validate();

    /*!
     *  Called when the page is to be changed and this page would be hidden.
     *
     *      @return False, if the page change should be rejected. Otherwise true.
     */
    virtual bool onPageChange();

    /*!
     *  Applies the changes that were done in the page.
     */
    virtual void apply();


signals:

    //! Signaled when new project should be created.
    void createProject(Project const& project, QString const& directory);

public slots:

    /*!
     *  Updates the directory based on the project.
     */
    void updateDirectory();

protected:

    /*!
     *  Checks if the given project is not found in the library.
     *
     *      @param [in] project   The project to check.
     *
     *      @return True, if the project is not already in the library, otherwise false.
     */
    bool isUnusedProject(Project const& project) const;

    /*!
     *  Gets the selected path within the library.
     *
     *      @return The selected path.
     */
    QString selectedPath() const;

    /*!
     *  Shows an error message for a project already found in the library.
     *
     *      @param [in] project    The reserved project.
     */
    void showErrorForReservedProject(Project const& project);

    //! project editor.
    ProjectEditor* projectEditor_;

    //! The library interface.
    LibraryInterface* libInterface_;

private:
	//! No copying.
    NewProjectPage(const NewProjectPage& other);

	//! No assignment.
    NewProjectPage& operator=(const NewProjectPage& other);

    /*!
     *  Sets the basic layout for the page.
     */
    void setupLayout();

    //! Project selector.
    ProjectSelectorWidget* projectSelector_;

    //! Page title.
    QLabel* titleLabel_;

    //! Page description.
    QLabel* descLabel_;
};

#endif // NEWPROJECTPAGE_H
