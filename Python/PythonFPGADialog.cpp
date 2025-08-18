#include "PythonFPGADialog.h"

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
#include <QProgressDialog>
#include <QMovie>

#include <QtConcurrent/QtConcurrent>
#include <Common/SettingManager.h>
#include "CommandWindow/CommandWindow.h"
#include "singleton.h"

#define FPGA_READOUTPUT_SLOT 0



PythonFPGADialog::PythonFPGADialog(QWidget *parent)
    : QDialog{parent}
//    ,workerThread_(this)
{
    setMinimumWidth(600);
    setWindowTitle("AWorld Magic FPGA");    

//    setWindowFlags(Qt::Tool |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
//    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Tool);

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
    connect(pythonScriptRunButton, &QPushButton::clicked, this, &PythonFPGADialog::OnPythonScriptRunClicked);

    // Connect the close button to the Destroy function
    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &PythonFPGADialog::Destroy);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃
    mainLayout->addSpacing(20);

    titlelbl_ = new QLabel("FPGA", this);
    Progresslbl = new QLabel("", this);

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

    // Vendor
    QLabel *vendorLabel = new QLabel("Vendor", this);
    vendorLabel->setFont(baseFont);
    vendorLabel->setContentsMargins(0, 0, 0, 0);

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
    libLabel->setContentsMargins(0, 0, 0, 0);

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
    nameLabel->setContentsMargins(0, 0, 0, 0);

    nameTxt = new QLineEdit(this);
    nameTxt->setFixedSize(350, 20);
    nameTxt->setAlignment(Qt::AlignLeft);
    nameTxt->setStyleSheet(borderStyle);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameTxt);

    // Version
    QLabel *versionLabel = new QLabel("Version", this);
    versionLabel->setFont(baseFont);
    versionLabel->setContentsMargins(0, 0, 0, 0);

    versionTxt = new QLineEdit(this);
    versionTxt->setFixedSize(350, 20);
    versionTxt->setAlignment(Qt::AlignLeft);
    versionTxt->setStyleSheet(borderStyle);

    QHBoxLayout *versionLayout = new QHBoxLayout();
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



PythonFPGADialog::~PythonFPGADialog()
{

}



//-----------------------------------------------------------------------------
// Function: pythonOutputReader : Read Python Output
//-----------------------------------------------------------------------------
void PythonFPGADialog::pythonOutputReader(QProcess *process)
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
void PythonFPGADialog::checkPythonIsRunning(QProcess::ExitStatus exitStatus)
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



void PythonFPGADialog::startProcess()
{
    emit inputMessage("runfpga " + vendorTxt->text() + " " + libTxt->text() + " " + nameTxt->text() + " " + versionTxt->text());

    emit noticeMessage("[ SYSTEM ] | FPGA | Start FPGA Bit Stream Gen. Sequence.");

    Progresslbl->setText("FPGA Bit Stream Gen.");

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
    moveRegressParam.append("cd " + setprjPath_ + "/env/fpga/");

    QStringList fpgaParam;
    fpgaParam.append(setprjPath_ + "/bin/awsh vivado -mode tcl -quiet -source ./fpga_bit_gen_" + nameTxt->text() + ".tcl");

    QStringList mergeParam;
    mergeParam.append(awshParam.join(" "));
    mergeParam.append(scriptParam.join(" "));
    mergeParam.append(moveRegressParam);
    mergeParam.append(fpgaParam);

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



void PythonFPGADialog::OnPythonScriptRunClicked()
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



void PythonFPGADialog::outputProcess(QProcess* process)
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



void PythonFPGADialog::Destroy()
{
    this->close();
    delete this;
}
