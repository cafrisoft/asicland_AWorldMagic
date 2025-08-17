#ifndef PYTHONCOM_H
#define PYTHONCOM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QCloseEvent>

#include "sharedmemory.h"

class PythonCom : public QDialog
{
    Q_OBJECT
public:
    explicit PythonCom(QWidget *parent = nullptr);
    bool Create();
    void Destroy();

private:
    QLabel* DbgMessageLabel;

    QLineEdit *qtShareMemIDEdit;
    QLineEdit *qtMemContentsEdit;
    QLineEdit *pythonShareMemIDEdit;
    QLineEdit *pythonMemContentsEdit;
    QPushButton *closeButton;

    QLineEdit *pythonNameEdit;
    QLineEdit *pythonScriptPathEdit;

    QLineEdit *sendCmdEdit;
    QLineEdit *reciveCmdEdit;

    QProcess *process;

    SharedMemory* pQtMem;//(777, 120 + sizeof(sem_t));
    SharedMemory* pPyScriptMem;//(1234, 1024 + sizeof(sem_t),false);

    bool UseBinaryPy;

    // 2. 환경 변수를 읽음
    QProcessEnvironment _env;



protected:
    void OnPythonScriptRunClicked();
    void OnDataSendReceiveClicked();
    void OnReadShareMemClicked();
    void OnSendDatabyStdinClicked();
    void InitShareMem();

    // 오류가 발생했을 때 호출될 슬롯
    void OnProcessError(QProcess::ProcessError error);
    // 표준 오류 출력이 준비되었을 때 호출될 슬롯
    void OnProcessStdError();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // PYTHONCOM_H
