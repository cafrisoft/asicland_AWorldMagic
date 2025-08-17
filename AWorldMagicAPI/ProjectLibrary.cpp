/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectLibrary.h
 *      Project: AWM
 *      Description :
 *          .
 */

#include <QString>
#include <QStringBuilder>
#include <QStringList>
#include <QObject>
#include <QMap>

#include "ProjectLibrary.h"


//-----------------------------------------------------------------------------
// Function: ProjectLibrary::ProjectLibrary()
//-----------------------------------------------------------------------------
ProjectLibrary::ProjectLibrary(): name_(), vendor_(), version_()
{
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::ProjectLibrary()
//-----------------------------------------------------------------------------
ProjectLibrary::ProjectLibrary(QString const& name, QString const& vendor, QString const& version) :
    name_(name),
    vendor_(vendor),
    version_(version)
{
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::getName()
//-----------------------------------------------------------------------------
QString ProjectLibrary::getName() const
{
    return name_;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::setName()
//-----------------------------------------------------------------------------
void ProjectLibrary::setName(QString const& name)
{
    name_ = name.simplified();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::getVendor()
//-----------------------------------------------------------------------------
QString ProjectLibrary::getVendor() const
{
    return vendor_;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::setVendor()
//-----------------------------------------------------------------------------
void ProjectLibrary::setVendor(QString const& vendor)
{
    vendor_ = vendor.simplified();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::getVersion()
//-----------------------------------------------------------------------------
QString ProjectLibrary::getVersion() const
{
    return version_;
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::setVersion()
//-----------------------------------------------------------------------------
void ProjectLibrary::setVersion(QString const& version)
{
    version_ = version.simplified();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::isEmpty()
//-----------------------------------------------------------------------------
bool ProjectLibrary::isEmpty() const
{
    return name_.isEmpty() && vendor_.isEmpty() && version_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::clear()
//-----------------------------------------------------------------------------
void ProjectLibrary::clear()
{
    name_.clear();
    vendor_.clear();
    version_.clear();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::isValid()
//-----------------------------------------------------------------------------
bool ProjectLibrary::isValid() const
{
    // If one of the identification fields is empty then this is invalid.
    return !name_.isEmpty() && !vendor_.isEmpty() && !version_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: ProjectLibrary::isValid()
//-----------------------------------------------------------------------------
bool ProjectLibrary::isValid(QVector<QString>& errors, QString const& parentIdentifier) const
{
    bool valid = true;

    if (name_.isEmpty())
    {
        errors.append(QObject::tr("No name specified for project library within %1").arg(parentIdentifier));
        valid = false;
    }    

    if (vendor_.isEmpty())
    {
        errors.append(QObject::tr("No vendor specified for project library within %1").arg(parentIdentifier));
        valid = false;
    }

    if (version_.isEmpty())
    {
        errors.append(QObject::tr("No version specified for project library within %1").arg(parentIdentifier));
        valid = false;
    }

    return valid;
}
