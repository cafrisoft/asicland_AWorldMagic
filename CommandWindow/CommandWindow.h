#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>

class ConsolLogTab;
class LogOutTab;
class CommandWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CommandWindow(QWidget *parent = nullptr);
    void LogOut(QString log, bool isErr=false);

protected:
    ConsolLogTab *consolLogTab_;
    LogOutTab *logOutTab_;

signals:
    void reloadLibrary();

public slots:
};

#endif // COMMANDWINDOW_H
