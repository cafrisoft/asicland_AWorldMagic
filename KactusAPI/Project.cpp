/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: Project.h
 *      Project: AWM
 *      Description :
 *          .
 */

#include "Project.h"

#include <QString>
#include <QStringBuilder>
#include <QStringList>
#include <QObject>
#include <QMap>


//-----------------------------------------------------------------------------
// Function: Project::Project()
//-----------------------------------------------------------------------------
Project::Project(): name_(), vendor_(), version_()
{
}

//-----------------------------------------------------------------------------
// Function: Project::Project()
//-----------------------------------------------------------------------------
Project::Project(QString const& name, QString const& vendor, QString const& version) :
    name_(name),
    vendor_(vendor),
    version_(version)
{
}

//-----------------------------------------------------------------------------
// Function: Project::getName()
//-----------------------------------------------------------------------------
QString Project::getName() const
{
    return name_;
}

//-----------------------------------------------------------------------------
// Function: Project::setName()
//-----------------------------------------------------------------------------
void Project::setName(QString const& name)
{
    name_ = name.simplified();
}

//-----------------------------------------------------------------------------
// Function: Project::getVendor()
//-----------------------------------------------------------------------------
QString Project::getVendor() const
{
    return vendor_;
}

//-----------------------------------------------------------------------------
// Function: Project::setVendor()
//-----------------------------------------------------------------------------
void Project::setVendor(QString const& vendor)
{
    vendor_ = vendor.simplified();
}

//-----------------------------------------------------------------------------
// Function: Project::getVersion()
//-----------------------------------------------------------------------------
QString Project::getVersion() const
{
    return version_;
}

//-----------------------------------------------------------------------------
// Function: Project::setVersion()
//-----------------------------------------------------------------------------
void Project::setVersion(QString const& version)
{
    version_ = version.simplified();
}

//-----------------------------------------------------------------------------
// Function: Project::isEmpty()
//-----------------------------------------------------------------------------
bool Project::isEmpty() const
{
    return name_.isEmpty() && vendor_.isEmpty() && version_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: Project::clear()
//-----------------------------------------------------------------------------
void Project::clear()
{
    name_.clear();
    vendor_.clear();
    version_.clear();
}

//-----------------------------------------------------------------------------
// Function: Project::isValid()
//-----------------------------------------------------------------------------
bool Project::isValid() const
{
    // If one of the identification fields is empty then this is invalid.
    return !name_.isEmpty() && !vendor_.isEmpty() && !version_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: Project::isValid()
//-----------------------------------------------------------------------------
bool Project::isValid(QVector<QString>& errors, QString const& parentIdentifier) const
{
    bool valid = true;

    if (name_.isEmpty())
    {
        errors.append(QObject::tr("No name specified for project within %1").arg(parentIdentifier));
        valid = false;
    }    

    if (vendor_.isEmpty())
    {
        errors.append(QObject::tr("No vendor specified for project within %1").arg(parentIdentifier));
        valid = false;
    }

    if (version_.isEmpty())
    {
        errors.append(QObject::tr("No version specified for project within %1").arg(parentIdentifier));
        valid = false;
    }

    return valid;
}
