/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectImportDialog.cpp
 *      Project: AWM
 *      Description :
 *          Dialog for selecting the imported items.
 */

#include "ProjectImportDialog.h"

#include <QLabel>
#include <QFileDialog>
#include <QSettings>

//-----------------------------------------------------------------------------
// Function: ProjectImportDialog::ProjectImportDialog()
//-----------------------------------------------------------------------------
ProjectImportDialog::ProjectImportDialog(QWidget* parent, Qt::WindowFlags f):
ImportSelectionDialog(tr("Import"), QStringLiteral(":/resources/res/import.png"),
    QStringLiteral(":/resources/res/arrowRight.png"), tr("Select items to be imported"), parent, f),
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
// Function: ProjectImportDialog::~ProjectImportDialog()
//-----------------------------------------------------------------------------
ProjectImportDialog::~ProjectImportDialog()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectImportDialog::connectSignals()
//-----------------------------------------------------------------------------
void ProjectImportDialog::connectSignals()
{
    ImportSelectionDialog::connectSignals();

    connect(&folderBrowseButton_, SIGNAL(clicked()), this, SLOT(onBrowseTarget()), Qt::UniqueConnection);
    connect(&folderPathEditor_, SIGNAL(textChanged(const QString&)),
        this, SLOT(onDestinationFolderChanged(const QString&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectImportDialog::onBrowseTarget()
//-----------------------------------------------------------------------------
void ProjectImportDialog::onBrowseTarget()
{
    QFileDialog browseDialog(this, tr("Select destination folder for import"), currentPath_);
    browseDialog.setNameFilter("apr (*.apr)");
//    browseDialog.setFileMode(QFileDialog::Directory);
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
// Function: ProjectImportDialog::onDestinationFolderChanged()
//-----------------------------------------------------------------------------
void ProjectImportDialog::onDestinationFolderChanged(const QString& newDestination)
{
    folderPathEditor_.setText(newDestination);
    currentPath_ = newDestination;

    QPalette pathPalette = folderPathEditor_.palette();

    if (!QFileInfo(newDestination).isDir())
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
// Function: ProjectImportDialog::getSourceFile()
//-----------------------------------------------------------------------------
QString ProjectImportDialog::getSourceFile() const
{
    return currentPath_;
}

//-----------------------------------------------------------------------------
// Function: ProjectImportDialog::setupLayout()
//-----------------------------------------------------------------------------
void ProjectImportDialog::setupLayout()
{
    QString introLabel = tr("Import");
    QString introText = tr("Import the selected items to the destination folder. ");
    QWidget* introWidget = setupIntroWidget(introLabel, introText);

    QString folderToolTip(tr("The destination folder for the Import."));
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
