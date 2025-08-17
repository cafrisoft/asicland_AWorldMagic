#ifndef PYTHONFPGA_H
#define PYTHONFPGA_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QTextEdit>
#include <QDialog>

class PythonFPGA : public QDialog
{
    Q_OBJECT
public:
    explicit PythonFPGA(QWidget *parent = nullptr);
    void Destroy();

private:
    QLineEdit *nameTxt ;
    QLineEdit *versionTxt ;
    QLineEdit *venderTxt ;
    QLineEdit *libTxt ;
    QLabel *Progresslbl ;

    QPushButton *closeButton;
    QPushButton *pythonScriptRunButton;

      QProcess setprjProcess;

protected:
    QString setprjPath;
    QString genScriptPath;

private:


protected:
    void OnPythonScriptRunClicked();

signals:
    void inputMessage(QString const& message);
    void noticeMessage(QString const& message);
    void errorMessage(QString const& message);
    void undefinedMessage(QString const& message);

public slots:
    void startProcess();
};

#endif // PYTHONFPGA_H
