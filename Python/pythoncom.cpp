#include "pythoncom.h"
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QCheckBox>
#include <QMessageBox>

PythonCom::PythonCom(QWidget *parent) : QDialog(parent)
{
    pQtMem = nullptr;
    pPyScriptMem = nullptr;
    UseBinaryPy = false;

    InitShareMem();
    setMinimumWidth(800);

    // 'Python Script Path' 라벨과 에디트 콘트롤 추가
    QLabel *pythonNameLbl = new QLabel("Python Excuter Name (ex. python2, python", this);
    pythonNameEdit = new QLineEdit(this);
    pythonNameEdit->setText("python3");

    QCheckBox *checkbox = new QCheckBox("Use Binary");

    // 'Python Script Path' 라벨과 에디트 콘트롤 추가
    QLabel *pythonScriptPathLabel = new QLabel("Python Script Path", this);
    pythonScriptPathEdit = new QLineEdit(this);
    pythonScriptPathEdit->setText("/home/dubi/QtWorks/BaseLayOut/script1.py");

    // 'PythonScript Run' 버튼 추가 및 연결된 함수 추가
    QPushButton *pythonScriptRunButton = new QPushButton("PythonScript Run", this);
    connect(pythonScriptRunButton, &QPushButton::clicked, this, &PythonCom::OnPythonScriptRunClicked);  // 연결된 함수 필요

    DbgMessageLabel = new QLabel("Debug Message", this);

    qtShareMemIDEdit = new QLineEdit(this);
    qtMemContentsEdit = new QLineEdit(this);
    pythonShareMemIDEdit = new QLineEdit(this);
    pythonMemContentsEdit = new QLineEdit(this);

    qtShareMemIDEdit->setText(pQtMem->keyToString().c_str());
    qtMemContentsEdit->setText(pQtMem->getData());
    pythonShareMemIDEdit->setText(pPyScriptMem->keyToString().c_str());
    pythonMemContentsEdit->setText(pPyScriptMem->getData());

    QLabel *Label1 = new QLabel("The message will be sent to the python script via stdin.", this);
    QLabel *Label2 = new QLabel("The message received from the python script via stdin.", this);
    sendCmdEdit = new QLineEdit(this);
    reciveCmdEdit= new QLineEdit(this);

    // 'Data SendRecive' 번튼 생성 및 연결함수 추가
    QPushButton *cmdtoPyButton = new QPushButton("Sent Message tp PyScrip by StdIn", this);
    connect(cmdtoPyButton, &QPushButton::clicked, this, &PythonCom::OnSendDatabyStdinClicked);  // 연결된 함수 필요

    QPushButton *dataSendButton = new QPushButton("Write QtShareMem", this);
    connect(dataSendButton, &QPushButton::clicked, this, &PythonCom::OnDataSendReceiveClicked);  // 연결된 함수 필요
    QPushButton *readShareMemButton = new QPushButton("Read Python Share Mem", this);
    connect(readShareMemButton, &QPushButton::clicked, this, &PythonCom::OnReadShareMemClicked);  // 연결된 함수 필요

    closeButton = new QPushButton("닫기", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(DbgMessageLabel);
    layout->addSpacing(30);

    layout->addWidget(pythonNameLbl);
    layout->addWidget(pythonNameEdit);
    layout->addWidget(pythonScriptPathLabel);
    layout->addWidget(checkbox);
    layout->addWidget(pythonScriptPathEdit);
    layout->addWidget(pythonScriptRunButton);

    layout->addSpacing(20);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
    layout->addWidget(Label1);
    layout->addWidget(sendCmdEdit);
    layout->addWidget(cmdtoPyButton);

    layout->addWidget(Label2);
    layout->addWidget(reciveCmdEdit);

    layout->addSpacing(20);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
    layout->addWidget(new QLabel("QT ShareMemID", this));
    layout->addWidget(qtShareMemIDEdit);
    layout->addWidget(new QLabel("QTMem Contents", this));
    layout->addWidget(qtMemContentsEdit);

    layout->addSpacing(20);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
    layout->addWidget(new QLabel("Python ShareMemID", this));
    layout->addWidget(pythonShareMemIDEdit);
    layout->addWidget(new QLabel("PythonMem Contents", this));
    layout->addWidget(pythonMemContentsEdit);

    layout->addSpacing(20);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
    layout->addWidget(dataSendButton);
    layout->addWidget(readShareMemButton);

    //closeButton 윗쪽에 한 라인의 공백삽입
    layout->addSpacing(20);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
    layout->addWidget(closeButton);

    // Connect the close button to the Destroy function
    connect(closeButton, &QPushButton::clicked, this, &PythonCom::Destroy);

    QObject::connect(checkbox, &QCheckBox::stateChanged, [=](int state) {
        if (state == Qt::Checked) {
            UseBinaryPy = true;
        } else {
            UseBinaryPy = false;
        }
    });

    qtMemContentsEdit->setText(pQtMem->getData());
    pythonShareMemIDEdit->setDisabled(true);
    pythonMemContentsEdit->setDisabled(true);

/*
    // 2. 환경 변수를 읽음
    _env = QProcessEnvironment::systemEnvironment();
    // 필요한 경우 환경 변수를 출력하거나 검사
    // qDebug() << env.toStringList();
*/
}

void PythonCom::InitShareMem()
{
    if(pQtMem)
        delete pQtMem;

    if(pPyScriptMem)
        delete pPyScriptMem;


    pQtMem = new SharedMemory("700", 4096); //??? + sizeof(sem_t)
    pQtMem->writeData("Hi I'am AWM");
    pPyScriptMem = new SharedMemory("701", 4096);//120 + sizeof(sem_t));
    pPyScriptMem->writeData("Hi I'am PySide");
}

bool PythonCom::Create()
{
    this->show();
    process = new QProcess(this);

    // 3. set Enviroment
    process->setProcessEnvironment(_env);


    // 오류가 발생했을 때 호출될 슬롯 연결
    QObject::connect(process, &QProcess::errorOccurred, this, &PythonCom::OnProcessError);

    // 표준 오류 출력이 준비되었을 때 호출될 슬롯 연결
    QObject::connect(process, &QProcess::readyReadStandardError, this, &PythonCom::OnProcessStdError);

    //..create  std_in, out pipe
    QObject::connect(process, &QProcess::readyReadStandardOutput, [&]() {
        if(!pQtMem)
            return;

        QByteArray data = process->readAllStandardOutput();
        QString strData = QString::fromUtf8(data);
        reciveCmdEdit->setText(strData);

        qDebug() << "From Python:" << data.trimmed();
    });

    return true;
}

// 오류가 발생했을 때 호출될 슬롯
void PythonCom::OnProcessError(QProcess::ProcessError error)
{
    if(pQtMem == nullptr)
        return;

    QString errorMsg;
    switch (error) {
    case QProcess::FailedToStart:
        errorMsg = "Failed to start the process!";
        break;
    case QProcess::Crashed:
        errorMsg = "The process crashed!";
        break;
    case QProcess::Timedout:
        errorMsg = "The process timed out!";
        break;
    case QProcess::ReadError:
        errorMsg = "A read error occurred!";
        break;
    case QProcess::WriteError:
        errorMsg = "A write error occurred!";
        break;
    case QProcess::UnknownError:
    default:
        errorMsg = "An unknown error occurred!";
        break;
    }
    qDebug() << errorMsg;
    DbgMessageLabel->setText(errorMsg);
}

// 표준 오류 출력이 준비되었을 때 호출될 슬롯
void PythonCom::OnProcessStdError()
{
    QByteArray errorData = process->readAllStandardError();
    QString errorStr = QString::fromUtf8(errorData);
    qDebug() << "Python Error:" << errorStr.trimmed();
    DbgMessageLabel->setText("Error from Python: " + errorStr);
}

//todo what call this... sometime called ... where from
void PythonCom::Destroy()
{
    if(pQtMem)
    {
        delete pQtMem;
        pQtMem = nullptr;
    }

    if(pPyScriptMem)
    {
        delete pPyScriptMem;
        pPyScriptMem = nullptr;
    }

    this->close();
    delete this;
}

void PythonCom::OnPythonScriptRunClicked()
{
    QStringList params;

    // Python 스크립트 실행 코드 작성
    //    process->start("python3", QStringList() << "-u" << "/home/dubi/QtWorks/QtCommuincation/script.py"); // '-u' option for direct show from python's response

    // 코드에서는 파이썬 실행 파일의 이름과 스크립트 경로를 직접 지정했습니다.
    // 여기서는 사용자가 입력한 값들을 사용합니다.

    if(UseBinaryPy) // use binary type python script
    {
        QMessageBox::information(this, "Is Check", "Use binary Python File");
        return;
    }
    else // use scripyt
    {
        params.append(pythonNameEdit->text());
        params.append("-u");
        params.append(pythonScriptPathEdit->text());
    }

    process->start(params.first(), params.mid(1));

    if (process->waitForStarted()) {
        qDebug() << "Process started successfully!";
        DbgMessageLabel->setText("Py script started successfully!");
    } else {
        qDebug() << "Failed to start the process!";
        DbgMessageLabel->setText("Py script starting Failed!");
    }
}

void PythonCom::OnDataSendReceiveClicked()
{
    std::string message2 = qtMemContentsEdit->text().toStdString();
    pQtMem->writeData(message2.c_str());
}

void PythonCom::OnReadShareMemClicked()
{
    //pPyScriptMem->InitRetry(); //if you have sharememory, get it
    pPyScriptMem->readData();
    pythonMemContentsEdit->setText(pPyScriptMem->getData());
    DbgMessageLabel->setText(pPyScriptMem->getData());
}

void PythonCom::OnSendDatabyStdinClicked()
{
    if(process->state() == QProcess::Running)
    {
        QString message = sendCmdEdit->text();
        message += '\n';
        process->write(message.toUtf8());
    }
    else
    {
        DbgMessageLabel->setText("process is stopped!");
    }
}

void PythonCom::closeEvent(QCloseEvent *event)
{
    // 이곳에서 window가 닫힐 때 수행할 동작을 구현합니다.

    // 그 외 필요한 정리 작업...
    if(pQtMem)
    {
        delete pQtMem;
        pQtMem = nullptr;
    }

    if(pPyScriptMem)
    {
        delete pPyScriptMem;
        pPyScriptMem = nullptr;
    }

    event->accept();  // 이벤트를 받아들입니다. 이 호출을 통해 창이 닫힙니다.
}
