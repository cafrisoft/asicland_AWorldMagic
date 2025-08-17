/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSettingsDialog.cpp
 *      Project: AWM
 *      Description :
 *          Dialog for configuring project locations.
 */


#include "ProjectSettingsDialog.h"
#include "ProjectSettingsDelegate.h"

#include <UI/Dialogs/ProjectSettingsDialog/ProjectSettingsColumns.h>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QStringList>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::ProjectSettingsDialog()
//-----------------------------------------------------------------------------
ProjectSettingsDialog::ProjectSettingsDialog(QSettings& settings, QWidget* parent):
QDialog(parent),
settings_(settings),
iconProvider_(),
prjLocationsTable_(0),
addLocationButton_(new QPushButton(QIcon(":/resources/res/add.png"), QString(), this)),
removeLocationButton_(new QPushButton(QIcon(":/resources/res/remove.png"), QString(), this)),
changed_(false),
checkMarkIcon_(":/resources/res/checkMark.png")
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Configure Project"));

    // Create the project location group box.
    QGroupBox* locationGroup = new QGroupBox(tr("project locations"), this);

    prjLocationsTable_ = new QTableWidget(0, 3, locationGroup);

    // the headers for the table
    QStringList headers;
    headers.append(tr("Default"));
    headers.append(tr("Active"));
    headers.append(tr("Project path"));
    prjLocationsTable_->setHorizontalHeaderLabels(headers);

    // Cells are resized to match contents.
    prjLocationsTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    prjLocationsTable_->horizontalHeader()->setStretchLastSection(true);
    prjLocationsTable_->setAlternatingRowColors(true);
    prjLocationsTable_->verticalHeader()->hide();

    // Set the height of a row to be smaller than default.
    prjLocationsTable_->verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 8);

    prjLocationsTable_->setMinimumWidth(500);
    prjLocationsTable_->setColumnWidth(ProjectSettingsColumns::DEFAULT, 50);
    prjLocationsTable_->setColumnWidth(ProjectSettingsColumns::ACTIVE, 50);

    prjLocationsTable_->setItemDelegate(new ProjectSettingsDelegate(this));

    removeLocationButton_->setEnabled(false);

    QWidget* introWidget = new QWidget(this);
    QHBoxLayout* introLayout = new QHBoxLayout(introWidget);
    introLayout->addWidget(new QLabel(tr("<b>Set project locations on disk</b><br>"
        "Active directories and their sub-directories will be searched for IP-XACT files and "
        "read into the project."), this));

    QPalette introPalette = introWidget->palette();
    introPalette.setColor(QPalette::Window, Qt::white);
    introWidget->setPalette(introPalette);
    introWidget->setAutoFillBackground(true);

    QDialogButtonBox* listButtonBox = new QDialogButtonBox(Qt::Vertical);
    listButtonBox->addButton(addLocationButton_, QDialogButtonBox::ActionRole);
    listButtonBox->addButton(removeLocationButton_, QDialogButtonBox::ActionRole);

    QHBoxLayout* locationLayout = new QHBoxLayout(locationGroup);
    locationLayout->addWidget(prjLocationsTable_);
    locationLayout->addWidget(listButtonBox);

    QGroupBox* separator = new QGroupBox(this);
    separator->setFlat(true);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                       Qt::Horizontal, this);
    // Setup the layout.
    QVBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->addWidget(introWidget);
    topLayout->addWidget(locationGroup, 1);
    topLayout->addWidget(separator);
    topLayout->addWidget(buttonBox);

    resize(700, sizeHint().height() + 50);

    connect(prjLocationsTable_, SIGNAL(itemClicked(QTableWidgetItem*)),
        this, SLOT(onItemClicked(QTableWidgetItem*)), Qt::UniqueConnection);
    connect(prjLocationsTable_, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)),
        this, SLOT(onSelectLocation(QTableWidgetItem*, QTableWidgetItem*)), Qt::UniqueConnection);
    connect(addLocationButton_, SIGNAL(clicked()), this, SLOT(addLocation()));
    connect(removeLocationButton_, SIGNAL(clicked()), this, SLOT(removeLocation()));
    connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), 
        this, SLOT(accept()), Qt::UniqueConnection);
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), 
        this, SLOT(reject()), Qt::UniqueConnection);

    loadSettings();
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::~ProjectSettingsDialog()
//-----------------------------------------------------------------------------
ProjectSettingsDialog::~ProjectSettingsDialog()
{
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::validate()
//-----------------------------------------------------------------------------
bool ProjectSettingsDialog::validate()
{
    return true;
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::apply()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::apply()
{
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::addLocation()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::addLocation()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select project location:"));

    if (!directory.isEmpty())
    {
        bool isDefault = prjLocationsTable_->rowCount() == 0;
        createRowForDirectory(directory, true, isDefault);
        changed_ = true;
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::removeLocation()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::removeLocation()
{
    int row = prjLocationsTable_->currentRow();

    if (row >= 0)
    {
        QTableWidgetItem* defItem = prjLocationsTable_->takeItem(row, ProjectSettingsColumns::DEFAULT);
        delete defItem;

        QTableWidgetItem* activeItem = prjLocationsTable_->takeItem(row, ProjectSettingsColumns::ACTIVE);
        delete activeItem;

        QTableWidgetItem* pathItem = prjLocationsTable_->takeItem(row, ProjectSettingsColumns::PATH);
        delete pathItem;

        prjLocationsTable_->removeRow(row);
    }

    changed_ = true;
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::onSelectLocation()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::onSelectLocation(QTableWidgetItem* cur, QTableWidgetItem*)
{
    removeLocationButton_->setEnabled(cur != 0);
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::onItemClicked()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::onItemClicked(QTableWidgetItem* item)
{
    if (item->column() == ProjectSettingsColumns::DEFAULT || item->column() == ProjectSettingsColumns::ACTIVE)
    {
        changed_ = true;
        if (item->column() == ProjectSettingsColumns::DEFAULT)
        {
            changeDefaultPath(item);
        }
        else if (item->column() == ProjectSettingsColumns::ACTIVE)
        {
            changeActivePathStatus(item);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::changeDefaultPath()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::changeDefaultPath(QTableWidgetItem* item)
{
    if (!item->icon().isNull())
    {
        return;
    }

    item->setIcon(checkMarkIcon_);
    for (int index = 0; index < prjLocationsTable_->rowCount(); ++index)
    {
        QTableWidgetItem* tableItem = prjLocationsTable_->item(index, ProjectSettingsColumns::DEFAULT);
        if (tableItem != item)
        {
            if (!tableItem->icon().isNull())
            {
                tableItem->setIcon(QIcon());
            }
        }
        else
        {
            QTableWidgetItem* activeTableItem = prjLocationsTable_->item(index, ProjectSettingsColumns::ACTIVE);
            if (activeTableItem && activeTableItem->icon().isNull())
            {
                activeTableItem->setIcon(checkMarkIcon_);
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::changeActivePathStatus()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::changeActivePathStatus(QTableWidgetItem* item)
{
    if (!item->icon().isNull())
    {
        QTableWidgetItem* defaultItem = prjLocationsTable_->item(item->row(), ProjectSettingsColumns::DEFAULT);
        if (defaultItem && defaultItem->icon().isNull())
        {
            item->setIcon(QIcon());
        }
    }
    else
    {
        item->setIcon(checkMarkIcon_);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::accept()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::accept()
{
    // Create a string list containing all the locations and save it to the settings.
    QStringList locations;

    // the active locations used to search for IP-XACT objects
    QStringList activeLocations;

    // the checked item in the list is the default location
    QString defaultLocation;

    for (int i = 0; i < prjLocationsTable_->rowCount(); ++i)
    {
        QTableWidgetItem* defItem = prjLocationsTable_->item(i, ProjectSettingsColumns::DEFAULT);
        QTableWidgetItem* activeItem = prjLocationsTable_->item(i, ProjectSettingsColumns::ACTIVE);
        QTableWidgetItem* pathItem = prjLocationsTable_->item(i, ProjectSettingsColumns::PATH);
        
        // if the row contains the default project
        if (!defItem->icon().isNull())
        {
            defaultLocation = pathItem->text();
        }
        if (!activeItem->icon().isNull())
        {
            activeLocations.append(pathItem->text());
        }

        // add the project path to the known project locations
        locations.append(pathItem->text());
    }

    settings_.setValue("Project/Locations", locations);
    settings_.setValue("Project/DefaultLocation", defaultLocation);
    settings_.setValue("Project/ActiveLocations", activeLocations);

    if (changed_)
    {
        emit scanProject();
    }

    QDialog::accept();
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::loadSettings()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::loadSettings()
{
    // Load the project locations.
    QString defaultLocation = settings_.value(QStringLiteral("Project/DefaultLocation")).toString();
    QStringList activeLocations = settings_.value(QStringLiteral("Project/ActiveLocations")).toStringList();
    QStringList locations = settings_.value(QStringLiteral("Project/Locations")).toStringList();

    foreach (QString const& location, locations)
    {
        QString fullLocation = location;

        if (QFileInfo(location).isRelative())
        {
            fullLocation = QFileInfo(location).absoluteFilePath();
        }

        bool isDefaultLocation = location.compare(defaultLocation) == 0;
        createRowForDirectory(fullLocation, activeLocations.contains(location), isDefaultLocation);
    }

    prjLocationsTable_->setCurrentIndex(QModelIndex());

    changed_ = false;
}

//-----------------------------------------------------------------------------
// Function: ProjectSettingsDialog::createRowForDirectory()
//-----------------------------------------------------------------------------
void ProjectSettingsDialog::createRowForDirectory(QString const& directory, bool isActive, bool isDefault)
{
    int rowNumber = prjLocationsTable_->rowCount();

    prjLocationsTable_->insertRow(rowNumber);

    // Create the item for default column.
    QTableWidgetItem* defaultItem = new QTableWidgetItem();
    defaultItem->setFlags(Qt::ItemIsEnabled);

    if (isDefault)
    {        
        defaultItem->setIcon(checkMarkIcon_);
    }

    prjLocationsTable_->setItem(rowNumber, ProjectSettingsColumns::DEFAULT, defaultItem);

    // create the item for the active column
    QTableWidgetItem* activeItem = new QTableWidgetItem();
    activeItem->setFlags(Qt::ItemIsEnabled);

    if (isActive)
    {
        activeItem->setIcon(checkMarkIcon_);
    }

    prjLocationsTable_->setItem(rowNumber, ProjectSettingsColumns::ACTIVE, activeItem);

    QTableWidgetItem* pathItem = new QTableWidgetItem(directory);
    pathItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QFileInfo directoryInfo(directory);
    if (directoryInfo.exists())
    {
         pathItem->setIcon(iconProvider_.icon(directoryInfo));
    }
    else
    {
        pathItem->setIcon(QIcon(QStringLiteral(":/resources/Res/exclamation--frame.png")));
    }

    prjLocationsTable_->setItem(rowNumber, ProjectSettingsColumns::PATH, pathItem);
}
