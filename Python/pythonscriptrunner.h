#ifndef PYTHONSCRIPTRUNNER_H
#define PYTHONSCRIPTRUNNER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QTextEdit>
#include <QDialog>

class PythonScriptRunner : public QDialog
{
    Q_OBJECT
public:
    explicit PythonScriptRunner(QWidget *parent = nullptr);
    void Destroy();

signals:

private:
    QLabel* DbgMessageLabel;

    QLineEdit *qtShareMemIDEdit;
    QLineEdit *qtMemContentsEdit;
    QLineEdit *pythonShareMemIDEdit;
    QLineEdit *pythonMemContentsEdit;
    QPushButton *closeButton;

    QLineEdit *pythonNameEdit;

    QLineEdit *pythonScriptPathEdit;
    QLineEdit *genScriptWorkingPathEdit;

    QLineEdit *sendCmdEdit;

    QProcess setprjProcess;
    QLineEdit *genParameter;
    QTextEdit *resultTextBox;
    QTextEdit *errTextBox;

protected:
    QString setprjPath;
    QString genScriptPath;
    QString genScriptWorkingPath;

    void OnPythonScriptRunClicked();
//    void closeEvent(QCloseEvent *event) override;

};

#endif // PYTHONSCRIPTRUNNER_H
