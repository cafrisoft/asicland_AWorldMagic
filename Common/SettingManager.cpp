#include <QSettings>
#include <QDebug>

#include "SettingManager.h"


SettingManager::SettingManager()
{

}

const QString SettingManager::getRootLocation()
{
    QString location = QSettings().value(QStringLiteral("ROOT/Location")).toString();

    return location;
}

const QString SettingManager::getProjectLocation()
{
    QString location = QSettings().value(QStringLiteral("ROOT/Location")).toString();

    location = location + "/project";

    return location;
}

const QString SettingManager::getDefaultProjectLocation()
{
    QString location = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

    return location;
}

const QString SettingManager::getDefaultLibraryLocation()
{
    QString location = QSettings().value(QStringLiteral("Library/DefaultLocation")).toString();

    return location;
}

const QString SettingManager::getScriptLocation()
{
    QString location = QSettings().value(QStringLiteral("Script/Location")).toString();

    return location;
}
