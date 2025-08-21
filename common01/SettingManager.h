#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QObject>


class SettingManager : public QObject
{
    Q_OBJECT

public:
    static SettingManager& getInstance()
    {
        static SettingManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    //! The destructor.
    ~SettingManager() = default;

private:
    SettingManager(SettingManager const&) = delete; // Prevent copy
    void operator=(SettingManager const&) = delete; // Prevent assignment

    SettingManager();

public:
    static const QString getRootLocation();
    static const QString getProjectLocation();
    static const QString getDefaultProjectLocation();
    static const QString getDefaultLibraryLocation();

    static const QString getScriptLocation();

signals:
    void reloadLibrary();
};

#endif // SETTINGMANAGER_H
