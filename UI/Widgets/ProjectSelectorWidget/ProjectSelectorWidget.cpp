/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSelectorWidget.h
 *      Project: AWM
 *      Description :
 *          Widget for selecting a path under project locations
 */

#include "ProjectSelectorWidget.h"

#include <UI/Widgets/ProjectSelectorWidget/ProjectPathSelector/ProjectPathSelector.h>

#include <UI/Dialogs/ProjectSettingsDialog/ProjectSettingsDialog.h>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>


//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::ProjectSelectorWidget()
//-----------------------------------------------------------------------------
ProjectSelectorWidget::ProjectSelectorWidget(QWidget* parent):
    QWidget(parent),
    projectSelector_(new ProjectPathSelector(this)),
    browseButton_(new QPushButton(tr("Browse..."),this)),
    directorySet_(false)
{
    connect(browseButton_, SIGNAL(clicked()),this, SLOT(onBrowse()), Qt::UniqueConnection);
    connect(projectSelector_, SIGNAL(contentChanged()),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(projectSelector_, SIGNAL(editTextChanged(QString const&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);

    setupLayout();
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::~ProjectSelectorWidget()
//-----------------------------------------------------------------------------
ProjectSelectorWidget::~ProjectSelectorWidget()
{
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::getPath()
//-----------------------------------------------------------------------------
QString ProjectSelectorWidget::getPath() const
{
    return projectSelector_->currentText();
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::reset()
//-----------------------------------------------------------------------------
void ProjectSelectorWidget::reset()
{
    directorySet_ = false;
    projectSelector_->refresh();
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::isValid()
//-----------------------------------------------------------------------------
bool ProjectSelectorWidget::isValid() const
{
    return projectSelector_->isValid();
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::onBrowse()
//-----------------------------------------------------------------------------
void ProjectSelectorWidget::onBrowse()
{
    QString baseDirectory = QFileInfo(projectSelector_->currentText()).filePath();
    if (baseDirectory.size() < 1)
    {
        baseDirectory = projectSelector_->currentLocation();
    }

    QString targetDirectory = QFileDialog::getExistingDirectory(this, tr("Choose target directory"),
        baseDirectory);

    if (targetDirectory.size() < 1)
    {
        return;
    }

    targetDirectory = QFileInfo(targetDirectory).filePath();

    if (targetDirectory.size() < 1)
    {
        targetDirectory = ".";
    }

    QSettings settings;
    while (!projectSelector_->hasIndexFor(targetDirectory))
    {
        QMessageBox warningDialog(QMessageBox::Warning, "Warning", 
            "Chosen path is not in any active project. Do you want to configure libraries?",
            QMessageBox::Yes | QMessageBox::Cancel, this);

        warningDialog.setDetailedText(targetDirectory + "\nis not in any of the active libraries:\n" + 
            projectSelector_->getProjectLocations().join("\n"));

        if (warningDialog.exec() == QMessageBox::Yes)
        {
            ProjectSettingsDialog dialog(settings, this);
            dialog.exec();
            projectSelector_->refresh();
        }
        else //if( msg == QMessageBox::Cancel )
        {
            return;
        }
    }
    
    projectSelector_->setIndexFor(targetDirectory);
    directorySet_ = true;
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::updateDirectory()
//-----------------------------------------------------------------------------
void ProjectSelectorWidget::updatePath(QString const& path)
{
    if (!directorySet_)
    {
        QString directory = projectSelector_->currentLocation();

        projectSelector_->setEditText(directory + path);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectorWidget::setupLayout()
//-----------------------------------------------------------------------------
void ProjectSelectorWidget::setupLayout()
{
    QHBoxLayout *widgetLayout = new QHBoxLayout(this);
    widgetLayout->addWidget(new QLabel(tr("Directory:"), this));
    widgetLayout->addWidget(projectSelector_, 1);
    widgetLayout->addWidget(browseButton_);

    widgetLayout->setContentsMargins(0, 0, 0, 0);
}
