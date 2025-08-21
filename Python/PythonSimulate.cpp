#include <GlobalBuildOptions.h>

#include "PythonSimulate.h"

#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QCheckBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QThread>

#ifdef _WIN32
#include <common01/SettingManager.h>
#else
#include <Common/SettingManager.h>
#endif
#include "CommandWindow/CommandWindow.h"
#include "singleton.h"
#include "UI/WorkingIndicator.h"



PythonSimulate::PythonSimulate(QWidget *parent)
    : QDialog{parent}
//      isDragging(false),
//      isDraggable(false)
{

    setMinimumWidth(600);
    setWindowTitle("AWorld Magic Simulation");

//    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
//        setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

//    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Tool);

//    setWindowFlags(Qt::Tool |Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);

    // Set the stylesheet to modify appearance
    this->setStyleSheet(
        "QDialog { background-color: white; }"
        "QPushButton { background-color: #E5E5E5; }"
        "QLineEdit, QTextEdit { background-color: white; border: 1px solid #ccc; }"
        "QLabel { background-color: transparent; }"
    );

    this->setAutoFillBackground(true);

    setprjPath_ = SettingManager::getDefaultProjectLocation();

    // 'PythonScript Run' 버튼 추가 및 연결된 함수 추가
    pythonScriptRunButton = new QPushButton("Run", this);
    connect(pythonScriptRunButton, &QPushButton::clicked, this, &PythonSimulate::OnPythonScriptRunClicked);  // 연결된 함수 필요

    // Connect the close button to the Destroy function
    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &PythonSimulate::Destroy);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃
    mainLayout->addSpacing(20);

    titlelbl_ = new QLabel("Simulation", this);
    Progresslbl = new QLabel("", this);
//    Progresslbl = new WorkingIndicator(this);

    QFont font1;
    font1.setPointSize(20);
    font1.setBold(true);

    titlelbl_->setFont(font1);
    titlelbl_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    titlelbl_->setContentsMargins(30, 0, 0, 0);

    mainLayout->addWidget(titlelbl_);

    Progresslbl->setFont(font1);
    Progresslbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    Progresslbl->setContentsMargins(30, 0, 0, 0);

    mainLayout->addWidget(Progresslbl);

    // 30px의 공백 추가
    mainLayout->addSpacing(20);

         // 기본 폰트 설정
    QFont baseFont;
    baseFont.setPointSize(10);
    baseFont.setBold(true);

    QString borderStyle = "border: 1px solid black;";

    // Vender
    QLabel *vendorLabel = new QLabel("Vendor", this);
    vendorLabel->setFont(baseFont);
    vendorLabel->setContentsMargins(0, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    vendorTxt = new QLineEdit(this);
    vendorTxt->setFixedSize(350, 20);
    vendorTxt->setAlignment(Qt::AlignLeft);
    vendorTxt->setStyleSheet(borderStyle);

    QHBoxLayout *vendorLayout = new QHBoxLayout();
    vendorLayout->addWidget(vendorLabel);
    vendorLayout->addWidget(vendorTxt);

    // Library
    QLabel *libLabel = new QLabel("Library", this);
    libLabel->setFont(baseFont);
    libLabel->setContentsMargins(0, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    libTxt = new QLineEdit(this);
    libTxt->setFixedSize(350, 20);
    libTxt->setAlignment(Qt::AlignLeft);
    libTxt->setStyleSheet(borderStyle);

    QHBoxLayout *libLayout = new QHBoxLayout();
    libLayout->addWidget(libLabel);
    libLayout->addWidget(libTxt);

    // Name
    QLabel *nameLabel = new QLabel("Name", this);
    nameLabel->setFont(baseFont);
    nameLabel->setContentsMargins(0, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    nameTxt = new QLineEdit(this);
    nameTxt->setFixedSize(350, 20);
    nameTxt->setAlignment(Qt::AlignLeft);
    nameTxt->setStyleSheet(borderStyle);

    QHBoxLayout *nameLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameTxt);

    // Version
    QLabel *versionLabel = new QLabel("Version", this);
    versionLabel->setFont(baseFont);
    versionLabel->setContentsMargins(0, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    versionTxt = new QLineEdit(this);
    versionTxt->setFixedSize(350, 20);
    versionTxt->setAlignment(Qt::AlignLeft);
    versionTxt->setStyleSheet(borderStyle);

    QHBoxLayout *versionLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    versionLayout->addWidget(versionLabel);
    versionLayout->addWidget(versionTxt);

    // Setup VLNV Layout
    QVBoxLayout *vlnvVLayout = new QVBoxLayout();
    vlnvVLayout->addLayout(vendorLayout);
    vlnvVLayout->addLayout(libLayout);
    vlnvVLayout->addLayout(nameLayout);
    vlnvVLayout->addLayout(versionLayout);

    QHBoxLayout *vlnvHLayout = new QHBoxLayout();
    vlnvHLayout->addStretch();
    vlnvHLayout->addLayout(vlnvVLayout);
    vlnvHLayout->addStretch();

    mainLayout->addLayout(vlnvHLayout);

    // Buttons
    pythonScriptRunButton->setFixedSize(150, 25);
    closeButton->setFixedSize(150, 25);

        // 버튼 배치 레이아웃 생성
    QHBoxLayout *btnsLayout = new QHBoxLayout();
    btnsLayout->addStretch(1);  // 여기서 1은 stretch factor로, 다른 위젯들보다 레이아웃 공간의 여분을 더 차지하게 됩니다. 이를 통해 버튼들이 우측으로 밀려나게 됩니다.
    btnsLayout->addWidget(pythonScriptRunButton);
    btnsLayout->addWidget(closeButton);

    mainLayout->addSpacing(30);  // VersionTxt와 버튼 사이에 30px의 공백 추가
    mainLayout->addLayout(btnsLayout);
}



//-----------------------------------------------------------------------------
// Function: pythonOutputReader : Read Python Output
//-----------------------------------------------------------------------------
void PythonSimulate::pythonOutputReader(QProcess *process)
{
    while (process->canReadLine())
    {
        QString output = process->readLine();

        QString outputTrimmed = output.replace("\n", "");

        if (outputTrimmed != "")
        {
            emit undefinedMessage(outputTrimmed);
        }
    }
}



//-----------------------------------------------------------------------------
// Function: checkPythonIsRunning : Check Python is running
//-----------------------------------------------------------------------------
void PythonSimulate::checkPythonIsRunning(QProcess::ExitStatus exitStatus)
{
    while (exitStatus == QProcess::NormalExit)
    {
        if (exitStatus == QProcess::NormalExit)
        {
            this->deleteLater();
            break;
        }
    }
}



void PythonSimulate::startProcess()
{
    emit inputMessage("runsim " + vendorTxt->text() + " " + libTxt->text() + " " + nameTxt->text() + " " + versionTxt->text());

    emit noticeMessage("[ SYSTEM ] | Simulation | Start Simulation Sequence.");

//    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);  // 항상 최상위에 유지 및 프레임 없음
//    setWindowFlags(windowFlags() | Qt::Window);
//    setWindowFlags(windowFlags() & ~Qt::Window);
//    clearWindowFlag(Qt::Window);

    titlelbl_->setText("Simulation");

    QStringList awshParam;
    awshParam.append("source " + setprjPath_ + "/setprj");

    QString vender = vendorTxt->text();
    QString lib = libTxt->text();
    QString name = nameTxt->text();
    QString ver = versionTxt->text();

    QStringList scriptParam;
    scriptParam.append(setprjPath_ + "/bin/awsh python -u " + setprjPath_ + "/script/merge_script/merge_script_6.py");
    scriptParam.append("-vendor");
    scriptParam.append(vender);
    scriptParam.append("-lib-name");
    scriptParam.append(lib);
    scriptParam.append("-module-name");
    scriptParam.append(name);
    scriptParam.append("-version");
    scriptParam.append(ver);

    QStringList moveRegressParam;
    moveRegressParam.append("cd " + setprjPath_ + "/env/simulation/regress/");

    QStringList regressParam;
    regressParam.append(setprjPath_ + "/bin/awsh make regress");

    QStringList mergeParam;
    mergeParam.append(awshParam.join(" "));
    mergeParam.append(scriptParam.join(" "));
    mergeParam.append(moveRegressParam);
    mergeParam.append(regressParam);

    QStringList runParam;
    runParam << "-c";
    runParam << mergeParam.join("; ");

    QCoreApplication::processEvents();
    setprjProcess.setWorkingDirectory(setprjPath_);
    setprjProcess.setProcessChannelMode(QProcess::MergedChannels);
    setprjProcess.start("csh", runParam);

    QObject::connect(&setprjProcess, &QProcess::readyReadStandardOutput, [=]() {
        pythonOutputReader(&setprjProcess);
    });

    QObject::connect(&setprjProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        checkPythonIsRunning(exitStatus);
    });
}



void PythonSimulate::mousePressEvent(QMouseEvent *event)
{
//    if (event->y() < titleBarHeight) {
//        // Ignore the event to prevent dragging
//        event->ignore();
//    } else {
//        // Process the event normally for other regions of the window
//        QDialog::mousePressEvent(event);
//    }

//    if (event->button() == Qt::LeftButton && isDraggable) {
//        // Record the initial position when the left mouse button is pressed
//        startPosition = event->globalPos();
//        isDragging = true;
//    }
}



void PythonSimulate::mouseMoveEvent(QMouseEvent *event)
{
    // Check if the mouse move event occurs in the title bar region
//           if (event->buttons() & Qt::LeftButton && event->y() < titleBarHeight) {
//               // Ignore the event to prevent dragging
//               event->ignore();
//           } else {
//               // Process the event normally for other regions of the window
//               QDialog::mouseMoveEvent(event);
//           }

//    if (isDragging) {
//          // Ignore mouse move events if dragging is in progress
//          int deltaX = event->globalX() - startPosition.x();
//          int deltaY = event->globalY() - startPosition.y();
//          move(x() + deltaX, y() + deltaY);
//          startPosition = event->globalPos();
//      }
}



void PythonSimulate::mouseReleaseEvent(QMouseEvent *event)
{
//    if (event->button() == Qt::LeftButton) {
//        // Stop dragging when the left mouse button is released
//        isDragging = false;
//    }
}



void PythonSimulate::OnPythonScriptRunClicked()
{
    pythonScriptRunButton->setEnabled(false);
    closeButton->setEnabled(false);
    vendorTxt->setEnabled(false);
    libTxt->setEnabled(false);
    nameTxt->setEnabled(false);
    versionTxt->setEnabled(false);

    startProcess();

    Progresslbl->setText("Running");
//    pythonScriptRunButton->setEnabled(true);
//    closeButton->setEnabled(true);

//    this->deleteLater();
}



void PythonSimulate::outputProcess(QProcess* process)
{
    //---------- Output Log --------
    QByteArray output = process->readAllStandardOutput();
    // QByteArray를 QString로 변환
    QString outputStr = QString::fromUtf8(output);
        // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
    QStringList stringList = outputStr.split("\n", Qt::SkipEmptyParts);
    // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습.)
    QList<QString> outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
        emit undefinedMessage(line);
    }

    QByteArray errorOutput =  process->readAllStandardError();

    if (errorOutput.count()>0)
    {
        // QByteArray를 QString로 변환
        outputStr = QString::fromUtf8(errorOutput);
        // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
        stringList = outputStr.split("\n", Qt::SkipEmptyParts);
        // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습니다.)
        outputList = stringList;

    //        qDebug() << "------------  Error ------------\n";

        // 출력 결과 확인
        for (const QString &line : outputList) {
            emit errorMessage(line);
        }
    }
}



void PythonSimulate::Destroy()
{
    this->close();
    delete this;
}
