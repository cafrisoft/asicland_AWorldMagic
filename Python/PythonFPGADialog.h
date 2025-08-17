#ifndef PYTHONFPGADIALOG_H
#define PYTHONFPGADIALOG_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QTextEdit>
#include <QDialog>
#include <QEvent>
#include <QThread>
#include <QMovie>
#include <QVBoxLayout>



class PythonFPGADialog : public QDialog
{
    Q_OBJECT
public:
    explicit PythonFPGADialog(QWidget *parent = nullptr);
    ~PythonFPGADialog();
    void Destroy();
    void pythonOutputReader(QProcess *process);
    void checkPythonIsRunning(QProcess::ExitStatus exitStatus);

private:
    QLabel *titlelbl_;

    QLineEdit *vendorTxt ;
    QLineEdit *libTxt ;
    QLineEdit *nameTxt ;
    QLineEdit *versionTxt ;
    QLabel *Progresslbl ;

    QPushButton *closeButton;
    QPushButton *pythonScriptRunButton;

    QProcess setprjProcess;

protected:
    QString setprjPath_;

signals:
    void inputMessage(const QString& msg);
    void noticeMessage(const QString& msg);
    void errorMessage(const QString& msg);
    void undefinedMessage(const QString& msg);

public slots:
    void startProcess();

protected:
    void OnPythonScriptRunClicked();

    void outputProcess(QProcess* process);

};

#endif // PYTHONFPGADIALOG_H
