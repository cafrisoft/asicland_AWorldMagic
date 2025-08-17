/*
 *  	Created on: 11.1.2024
 *     Author: P.H.G
 * 	Filename: ProjectPathSelector.cpp
 *		Project: AWM
 */

#include "ProjectPathSelector.h"

#include <QStringList>
#include <QVariant>
#include <QFileInfo>

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::ProjectPathSelector()
//-----------------------------------------------------------------------------
ProjectPathSelector::ProjectPathSelector(QWidget *parent):
QComboBox(parent),
settings_(this),
validator_(new ProjectPathValidator(this)),
projectLocations_(),
pathEditor_(new ProjectPathEditor(this))
{
    refresh();

        QString defaultPath = settings_.value("Project/DefaultLocation", QString()).toString();
    if (!QFileInfo(defaultPath).isAbsolute())
    {
		defaultPath = QFileInfo(defaultPath).absoluteFilePath();
	}

	// if the default directory is found then select it
	int index = findText(defaultPath);
	if (index >= 0)
    {
		setCurrentIndex(index);
	}

	setEditable(true);

	setLineEdit(pathEditor_);
	pathEditor_->setUnmodifiablePath(defaultPath);

	setValidator(validator_);
	validator_->setUnmodifiablePath(defaultPath);

        setToolTip(tr("Select one of the active project locations you have listed in the settings."));

    connect(pathEditor_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()));

    connect(this, SIGNAL(currentIndexChanged(int)),	this, SLOT(onIndexChanged()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::~ProjectPathSelector()
//-----------------------------------------------------------------------------
ProjectPathSelector::~ProjectPathSelector()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::isValid()
//-----------------------------------------------------------------------------
bool ProjectPathSelector::isValid()
{
    Q_ASSERT(pathEditor_);
    return !currentText().isEmpty() && hasIndexFor(currentText());
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::currentLocation()
//-----------------------------------------------------------------------------
QString ProjectPathSelector::currentLocation() const
{
	int index = currentIndex();
        return projectLocations_.value(index, QString());
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::refresh()
//-----------------------------------------------------------------------------
void ProjectPathSelector::refresh()
{
    // Clear previous known project locations.
    projectLocations_.clear();
    clear();

    // the active project locations
    QStringList activeLocations = settings_.value("Project/ActiveLocations", QStringList()).toStringList();
    
    // make sure all locations are absolute paths
    foreach (QString location, activeLocations)
    {
        if (!QFileInfo(location).isAbsolute())
        {
            QString fullLocation = QFileInfo(location).absoluteFilePath();
            projectLocations_.append(fullLocation);
        }
        else
        {
            projectLocations_.append(location);
        }
    }

    addItems(projectLocations_);
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::getProjectLocations()
//-----------------------------------------------------------------------------
QStringList ProjectPathSelector::getProjectLocations()
{
    return projectLocations_;
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::hasIndexFor()
//-----------------------------------------------------------------------------
bool ProjectPathSelector::hasIndexFor(QString const& path)
{
    foreach(QString projectPath, projectLocations_)
    {
        if (path.startsWith(projectPath))
        {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::setIndexFor()
//-----------------------------------------------------------------------------
bool ProjectPathSelector::setIndexFor(QString const& path)
{
    foreach(QString projectPath, projectLocations_)
    {
        if (path.startsWith(projectPath))
        {
            setCurrentIndex(findText(projectPath));
            setCurrentText(path);
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: ProjectPathSelector::onIndexChanged()
//-----------------------------------------------------------------------------
void ProjectPathSelector::onIndexChanged()
{
	const QString path = currentLocation();

	pathEditor_->setUnmodifiablePath(path);
	validator_->setUnmodifiablePath(path);
}
