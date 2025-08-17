/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: NewProjectPage.cpp
 *      Project: AWM
 *      Description :
 *          New project page.
 */

#include "NewProjectPage.h"

#include <KactusAPI/Project.h>
#include <KactusAPI/LibraryInterface.h>

#include <UI/Widgets/ProjectSelectorWidget/ProjectSelectorWidget.h>

#include <UI/Widgets/ProjectEditor/ProjectEditor.h>

#include <QHBoxLayout>

//-----------------------------------------------------------------------------
// Function: NewProjectPage()
//-----------------------------------------------------------------------------
NewProjectPage::NewProjectPage(LibraryInterface* libHandler, QWidget *parentDlg):
    PropertyPageView(),
    projectEditor_(new ProjectEditor(libHandler, parentDlg, this)),
    libInterface_(libHandler),
    projectSelector_(new ProjectSelectorWidget(this)),
    titleLabel_(new QLabel( tr("New Project"), this)),
    descLabel_(new QLabel(tr("Creates a new project"), this))
//NewPage(libHandler, VLNV::INVALID, tr("New Project"), tr("Creates a new project"), parentDlg)
{
    Q_ASSERT_X(libHandler, "NewProjectPage constructor", "Null LibraryInterface pointer given as parameter");

    QFont font = titleLabel_->font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel_->setFont(font);

    connect(projectEditor_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()));
    connect(projectEditor_, SIGNAL(contentChanged()), this, SLOT(updateDirectory()));

    connect(projectSelector_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()));

    setupLayout();
    
    QVBoxLayout* topLayout = dynamic_cast<QVBoxLayout*>(layout());
    topLayout->addStretch(1);
}

//-----------------------------------------------------------------------------
// Function: ~NewProjectPage()
//-----------------------------------------------------------------------------
NewProjectPage::~NewProjectPage()
{

}

//-----------------------------------------------------------------------------
// Function: prevalidate()
//-----------------------------------------------------------------------------
bool NewProjectPage::prevalidate() const
{
    return (projectEditor_->isValid() && projectSelector_->isValid());
}

//-----------------------------------------------------------------------------
// Function: validate()
//-----------------------------------------------------------------------------
bool NewProjectPage::validate()
{
    Q_ASSERT(prevalidate());

    // Check if any of the projects already exists.
    bool validProject = isUnusedProject(projectEditor_->getProject());;

    if (!validProject)
    {
        showErrorForReservedProject(projectEditor_->getProject());
    }

    return validProject;
}

//-----------------------------------------------------------------------------
// Function: onPageChange()
//-----------------------------------------------------------------------------
bool NewProjectPage::onPageChange()
{
    projectSelector_->reset();

    projectEditor_->setProject(Project());
    return true;
}

//-----------------------------------------------------------------------------
// Function: apply()
//-----------------------------------------------------------------------------
void NewProjectPage::apply()
{
     emit createProject(projectEditor_->getProject(), selectedPath());
}

//-----------------------------------------------------------------------------
// Function: updateDirectory()
//-----------------------------------------------------------------------------
void NewProjectPage::updateDirectory()
{
    QString projectDir;

    Project project = projectEditor_->getProject();

    if (!project.getName().isEmpty())
    {
        projectDir += "/" + project.getName();
    }

//    if (!project.getVendor().isEmpty())
//    {
//        projectDir += "/" + project.getVendor();

//        if (!project.getLibrary().isEmpty())
//        {
//            projectDir += "/" + project.getLibrary();

//            if (!project.getName().isEmpty())
//            {
//                projectDir += "/" + project.getName();

//                if (!project.getVersion().isEmpty())
//                {
//                    projectDir += "/" + project.getVersion();
//                }
//            }
//        }
//    }

    projectSelector_->updatePath(projectDir);
}

//-----------------------------------------------------------------------------
// Function: NewProjectPage::isUnusedProject()
//-----------------------------------------------------------------------------
bool NewProjectPage::isUnusedProject(Project const& project) const
{
//    return !libInterface_->contains(project);
    return true;
}

//-----------------------------------------------------------------------------
// Function: NewProjectPage::selectedPath()
//-----------------------------------------------------------------------------
QString NewProjectPage::selectedPath() const
{
    return projectSelector_->getPath();
}

//-----------------------------------------------------------------------------
// Function: NewPage::showErrorForReservedProject()
//-----------------------------------------------------------------------------
void NewProjectPage::showErrorForReservedProject(Project const& project)
{
//    QString type = type2Show(project.getType());
//    QMessageBox msgBox(QMessageBox::Critical, QCoreApplication::applicationName(),
//        tr("The %1 cannot be created because the project %2"
//        " already exists in the library.").arg(type, project.toString()), QMessageBox::Ok, this);
//    msgBox.exec();
}

//-----------------------------------------------------------------------------
// Function: setupLayout()
//-----------------------------------------------------------------------------
void NewProjectPage::setupLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel_);
    layout->addWidget(descLabel_);
    layout->addSpacing(12);
    layout->addWidget(projectEditor_);
    layout->addWidget(projectSelector_);
}
