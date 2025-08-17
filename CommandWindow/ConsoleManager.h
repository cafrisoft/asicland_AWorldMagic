#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

#include <QObject>

#include <IPXACTmodels/common/VLNV.h>



class ConsoleManager : public QObject
{
    Q_OBJECT

public:
    static ConsoleManager& getInstance()
    {
        static ConsoleManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    //! The destructor.
    ~ConsoleManager() = default;

    enum OutputLevel {
        INPUT,
        SYSTEM,
        INFO,
        WARNING,
        ERROR
    };

private:
    ConsoleManager();

signals:
    void reloadLibrary();
    void reloadProject();
    void reloadProjectLibrary();
    void openSchematic(VLNV const&);
    void refreshSchematic(VLNV const&);
    void closeSchematic(VLNV const&);
    void closeAllSchematic();
    void openFile(QString);
    void requestSave();
    void requestSaveAll();
    void requestImportProject(QString const&, QString const&, QString const&);
    void requestExportProject(QString const&, QString const&, QString const&);

    void inputMessage(QString const&);
    void noticeMessage(QString const&);
    void errorMessage(QString const&);
    void undefinedMessage(QString const&);
};

#endif // CONSOLEMANAGER_H
