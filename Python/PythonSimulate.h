#ifndef PYTHONSIMULATE_H
#define PYTHONSIMULATE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QTextEdit>
#include <QDialog>
#include <QMouseEvent>



class PythonSimulate : public QDialog
{
    Q_OBJECT
public:
    explicit PythonSimulate(QWidget *parent = nullptr);
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

private:

signals:
#if LOG_OUT_SUPPORT_20240104 == 1
    /*!
     *  Transfers error messages between the main window and the message log out console.
     *
     *      @param [in] message     The error message.
     */
    void inputMessage_LogOut(QString const& message);

    /*!
     *  Transfers error messages between the main window and the message log out console.
     *
     *      @param [in] message     The error message.
     */
    void errorMessage_LogOut(QString const& message);

    /*!
     *  Transfers notice messages between the main window and the message log out console.
     *
     *      @param [in] message     The notice message.
     */
    void noticeMessage_LogOut(QString const& message);

#endif // LOG_OUT_SUPPORT_20240104

    //! Print a message to the user.
    void inputMessage(const QString& msg);
    void noticeMessage(const QString& msg);
    void errorMessage(const QString& msg);
    void undefinedMessage(const QString& msg);

public slots:
    void startProcess();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

//    virtual void mousePressEvent(QMouseEvent  *event);
//    virtual void mouseMoveEvent(QMouseEvent  *event);
//    virtual void mouseReleaseEvent(QMouseEvent  *event);

    void OnPythonScriptRunClicked();

    void outputProcess(QProcess* process);

};

#endif // PYTHONSIMULATE_H
