/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectExportDialog.cpp
 *      Project: AWM
 *      Description :
 *          Dialog for selecting the exported items.
 */

#include "ProjectExportDialog.h"

#include <QLabel>
#include <QFileDialog>

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::ProjectExportDialog()
//-----------------------------------------------------------------------------
ProjectExportDialog::ProjectExportDialog(QWidget* parent, Qt::WindowFlags f):
ProjectSelectionDialog(tr("Export"), QStringLiteral(":/resources/res/export.png"),
    QStringLiteral(":/resources/res/arrowRight.png"), tr("Select items to be exported"), parent, f),
folderPathEditor_(QString(), this),
folderBrowseButton_(QIcon(":/resources/res/opened-folder.png"), QString(), this),
currentPath_(QString())
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString styleSheet("*[mandatoryField=\"true\"] { background-color: LemonChiffon; }");
    setStyleSheet(styleSheet);

    disableOkButton();
    setupLayout();
    connectSignals();
}

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::~ProjectExportDialog()
//-----------------------------------------------------------------------------
ProjectExportDialog::~ProjectExportDialog()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::connectSignals()
//-----------------------------------------------------------------------------
void ProjectExportDialog::connectSignals()
{
    ProjectSelectionDialog::connectSignals();

    connect(&folderBrowseButton_, SIGNAL(clicked()), this, SLOT(onBrowseTarget()), Qt::UniqueConnection);
    connect(&folderPathEditor_, SIGNAL(textChanged(const QString&)),
        this, SLOT(onDestinationFolderChanged(const QString&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::onBrowseTarget()
//-----------------------------------------------------------------------------
void ProjectExportDialog::onBrowseTarget()
{
    QFileDialog browseDialog(this, tr("Select destination folder for export"), currentPath_);
    browseDialog.setFileMode(QFileDialog::Directory);
    if (browseDialog.exec() == QDialog::Accepted)
    {
        QStringList selectedFiles = browseDialog.selectedFiles();

        if (!selectedFiles.isEmpty())
        {
            QString selectedFolder = selectedFiles.first();
            onDestinationFolderChanged(selectedFolder);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::onDestinationFolderChanged()
//-----------------------------------------------------------------------------
void ProjectExportDialog::onDestinationFolderChanged(const QString& newDestination)
{
    folderPathEditor_.setText(newDestination);
    currentPath_ = newDestination;

    QPalette pathPalette = folderPathEditor_.palette();
    
    if (QFileInfo(newDestination).isDir())
    {
        pathPalette.setColor(QPalette::Text, Qt::black);
        enableOkButton();
    }
    else
    {
        pathPalette.setColor(QPalette::Text, Qt::red);
        disableOkButton();
    }

    folderPathEditor_.setPalette(pathPalette);
}

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::getTargetDirectory()
//-----------------------------------------------------------------------------
QString ProjectExportDialog::getTargetDirectory() const
{
    return currentPath_;
}

//-----------------------------------------------------------------------------
// Function: ProjectExportDialog::setupLayout()
//-----------------------------------------------------------------------------
void ProjectExportDialog::setupLayout()
{
    QString introLabel = tr("Export");
    QString introText = tr("Export the selected items to the destination folder. ") +
        tr(" The original files will be retained in their containing folders.\n") +
        tr("Selected files are exported to the folder of their containing component.") +
        tr(" Unselected files are given paths to their original location.");
    QWidget* introWidget = setupIntroWidget(introLabel, introText);

    QString folderToolTip(tr("The destination folder for the export."));
    QLabel* folderLabel = new QLabel(tr("Destination folder:"), this);
    folderLabel->setToolTip(folderToolTip);
    folderPathEditor_.setToolTip(folderToolTip);
    folderPathEditor_.setProperty("mandatoryField", true);
    folderBrowseButton_.setToolTip("Browse.");

    QHBoxLayout* folderBrowseLayout = new QHBoxLayout();
    folderBrowseLayout->addWidget(folderLabel);
    folderBrowseLayout->addWidget(&folderPathEditor_);
    folderBrowseLayout->addWidget(&folderBrowseButton_);

    QHBoxLayout* buttonLayout = setupButtonLayout();

    QVBoxLayout* overallLayout = new QVBoxLayout(this);
    overallLayout->addWidget(introWidget);
    overallLayout->addLayout(folderBrowseLayout, 0);
//    overallLayout->addWidget(getItemList(), 1);
    overallLayout->addLayout(buttonLayout, 0);
}
