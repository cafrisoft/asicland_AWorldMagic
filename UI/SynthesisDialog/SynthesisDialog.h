#ifndef SYNTHESISDIALOG_H
#define SYNTHESISDIALOG_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QDialog>


class SynthesisDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SynthesisDialog(QWidget *parent = nullptr);
    ~SynthesisDialog();
    void Destroy();

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
#if LOG_OUT_SUPPORT_20240104 == 1
    /*!
     *  Transfers notice messages between the main window and the message log out console.
     *
     *      @param [in] message     The notice message.
     */
    void inputMessage_LogOut(QString const& message);

    /*!
     *  Transfers notice messages between the main window and the message log out console.
     *
     *      @param [in] message     The notice message.
     */
    void noticeMessage_LogOut(QString const& message);

    /*!
     *  Transfers error messages between the main window and the message log out console.
     *
     *      @param [in] message     The error message.
     */
    void errorMessage_LogOut(QString const& message);

#endif // LOG_OUT_SUPPORT_20240104

    //! Print a message to the user.
    void inputMessage(const QString& msg);
    void noticeMessage(const QString& msg);
    void errorMessage(const QString& msg);
    void undefinedMessage(const QString& msg);

public slots:
    void startProcess();

protected:
    void OnPythonScriptRunClicked();

    void outputLogWindow(const QString &msg, bool sucess = true);
    void outputProcess(QProcess* process);

};

#endif // SYNTHESISDIALOG_H
