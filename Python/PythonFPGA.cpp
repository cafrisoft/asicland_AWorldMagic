#include "PythonFPGA.h"

#include "singleton.h"
#include "CommandWindow/CommandWindow.h"

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


PythonFPGA::PythonFPGA(QWidget *parent)
    : QDialog{parent}
{
    setMinimumWidth(600);
    setWindowTitle("AWorld Magic FPGA");

//    setWindowFlags(Qt::Tool |Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    // Set the stylesheet to modify appearance
    this->setStyleSheet(
        "QDialog { background-color: white; }"
        "QPushButton { background-color: #E5E5E5; }"
        "QLineEdit, QTextEdit { background-color: white; border: 1px solid #ccc; }"
        "QLabel { background-color: transparent; }"
    );

    this->setAutoFillBackground(true);

    // Get the home directory of the current user
    QString homeDir = QDir::homePath();

    setprjPath = QSettings().value(QStringLiteral("ROOT/Location")).toString();

    // 'PythonScript Run' 버튼 추가 및 연결된 함수 추가
    pythonScriptRunButton = new QPushButton("Run", this);
    connect(pythonScriptRunButton, &QPushButton::clicked, this, &PythonFPGA::OnPythonScriptRunClicked);  // 연결된 함수 필요
    closeButton = new QPushButton("Close", this);
    // Connect the close button to the Destroy function
    connect(closeButton, &QPushButton::clicked, this, &PythonFPGA::Destroy);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃
    mainLayout->addSpacing(20);

    QLabel *label1 = new QLabel("FPGA", this);
    Progresslbl = new QLabel("", this);
    QFont font1;
    font1.setPointSize(20);
    font1.setBold(true);
    label1->setFont(font1);
    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label1->setContentsMargins(60, 0, 0, 0);
    mainLayout->addWidget(label1);

    Progresslbl->setFont(font1);
    Progresslbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    Progresslbl->setContentsMargins(60, 0, 0, 0);
    mainLayout->addWidget(Progresslbl);

    // 30px의 공백 추가
    mainLayout->addSpacing(20);

         // 기본 폰트 설정
    QFont baseFont;
    baseFont.setPointSize(10);
    baseFont.setBold(true);

    QString borderStyle = "border: 1px solid black;";

    // Vender 라벨 및 TextBox
    QLabel *venderLabel = new QLabel("Vendor", this);
    venderLabel->setFont(baseFont);
    venderLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    venderTxt = new QLineEdit(this);
    venderTxt->setFixedSize(400, 20);
    venderTxt->setAlignment(Qt::AlignLeft);
    venderTxt->setStyleSheet(borderStyle);

    QHBoxLayout *venderLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    venderLayout->addWidget(venderLabel);
    venderLayout->addWidget(venderTxt);
    mainLayout->addLayout(venderLayout);


    QLabel *libLabel = new QLabel("library", this);
    libLabel->setFont(baseFont);
    libLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    libTxt = new QLineEdit(this);
    libTxt->setFixedSize(400, 20);
    libTxt->setAlignment(Qt::AlignLeft);
    libTxt->setStyleSheet(borderStyle);

    QHBoxLayout *libLayout = new QHBoxLayout();  
    libLayout->addWidget(libLabel);
    libLayout->addWidget(libTxt);
    mainLayout->addLayout(libLayout);

    // Name 라벨 및 TextBox
    QLabel *nameLabel = new QLabel("Name", this);
    nameLabel->setFont(baseFont);
    nameLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    nameTxt = new QLineEdit(this);
    nameTxt->setFixedSize(400, 20);
    nameTxt->setAlignment(Qt::AlignLeft);
    nameTxt->setStyleSheet(borderStyle);

    QHBoxLayout *nameLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameTxt);
    mainLayout->addLayout(nameLayout);

    // Version 라벨 및 TextBox
    QLabel *versionLabel = new QLabel("Version", this);
    versionLabel->setFont(baseFont);
    versionLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    versionTxt = new QLineEdit(this);
    versionTxt->setFixedSize(400, 20);
    versionTxt->setAlignment(Qt::AlignLeft);
    versionTxt->setStyleSheet(borderStyle);

    QHBoxLayout *versionLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    versionLayout->addWidget(versionLabel);
    versionLayout->addWidget(versionTxt);
    mainLayout->addLayout(versionLayout);


    //. 테스트 밸류
    venderTxt->setText("asicland");
    libTxt->setText("system");
    nameTxt->setText("example_system_for_zigbee");
    versionTxt->setText("1.0");


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

void PythonFPGA::startProcess()
{
    //--------------------------------------
    // (1) setprj 를 실행
    //--------------------------------------
    Progresslbl->setText("setprj");
    QCoreApplication::processEvents(); // UI 이벤트를 즉시 처리합니다.

    QStringList awshParmlist;
    awshParmlist.append("-c");
    awshParmlist.append("source setprj");

    setprjProcess.setWorkingDirectory(setprjPath);
    setprjProcess.start("csh", awshParmlist);

    //setprjProcess.start("bash", QStringList() << "-c" << "source setprj"); - csh로 대체
    setprjProcess.waitForFinished(-1);

#if LOG_OUT_SUPPORT_20240104 == 0
    Singleton::getInstance().getMyCommandWind()->LogOut(awshParmlist.join(" "));
#else
    emit noticeMessage(awshParmlist.join(" "));
#endif // LOG_OUT_SUPPORT_20240104

    //---------- Output Log --------
    QByteArray output = setprjProcess.readAllStandardOutput();
    // QByteArray를 QString로 변환
    QString outputStr = QString::fromUtf8(output);
        // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
    QStringList stringList = outputStr.split("\n", QString::SkipEmptyParts);
    // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습.)
    QList<QString> outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut(line, false);
#else
        emit noticeMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
    }

    QByteArray errorOutput = setprjProcess.readAllStandardError();
    if (errorOutput.count()>0)
    {
#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut(QString("------------  Error(setprj) ------------\n"), true);
#else
        emit errorMessage(QString("------------  Error(setprj) ------------\n"));
#endif // LOG_OUT_SUPPORT_20240104

        // QByteArray를 QString로 변환
        outputStr = QString::fromUtf8(errorOutput);
        // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
        stringList = outputStr.split("\n", QString::SkipEmptyParts);
        // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습니다.)
        outputList = stringList;

//        qDebug() << "------------  Error ------------\n";

        // 출력 결과 확인
        for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
            Singleton::getInstance().getMyCommandWind()->LogOut(line, true);
#else
            emit errorMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
        }
    }

    //----------------------------------------------------------------------------
    // 		(2) 실제 스크립트 실행  -- merge 스크립트
    //----------------------------------------------------------------------------

    Progresslbl->setText("merge_script");

    QString vender= venderTxt->text();
    QString lib   = libTxt->text();
    QString name  = nameTxt->text();
    QString ver   = versionTxt->text();

/*
      vender ="asicland";
      lib ="system";
      name ="example_system_for_zigbee";
      ver ="1.0";
*/

    QStringList parmlist;
    parmlist.append("-vendor");
    parmlist.append(vender);
    parmlist.append("-lib-name");
    parmlist.append(lib);
    parmlist.append("-module-name");
    parmlist.append(name);
    parmlist.append("-version");
    parmlist.append(ver);

    QStringList runParmlist;

    //runParmlist << "source setprj";  //23.11.03삭제


    runParmlist << setprjPath + "/bin/awsh " + "python " + setprjPath + "/script/merge_script/merge_script_6.py " +  parmlist.join(" ");
    //parmlist << "/home/dubi/AMagic/bFramelessWindowHintin/awsh python" + setprjPath + "/script/merge_script/merge_script_6.py -vendor asicland -lib-name system -module-name example_system_for_zigbee -version 1.0";
    //parmlist << "/home/dubi/AMagic/bin/awsh " + setprjPath + "/script/merge_script/merge_script_6 -vendor asicland -lib-name system -module-name example_system_for_zigbee -version 1.0";

    QStringList arguments;
    arguments << "-c";
    arguments << runParmlist.join("; ");
    // arguments << runParmlist.join("; ");

#if LOG_OUT_SUPPORT_20240104 == 0
    Singleton::getInstance().getMyCommandWind()->LogOut("========== Output (run mergescript) =========\n script >> ");
    Singleton::getInstance().getMyCommandWind()->LogOut("setprjPath : " + setprjPath + "\n");
    Singleton::getInstance().getMyCommandWind()->LogOut("arguments  : " + arguments.join(" ")+ "\n");
#else
    emit noticeMessage("========== Output (run mergescript) =========\n script >> ");
    emit noticeMessage("setprjPath : " + setprjPath + "\n");
    emit noticeMessage("arguments  : " + arguments.join(" ")+ "\n");
#endif // LOG_OUT_SUPPORT_20240104

    QCoreApplication::processEvents(); // UI 이벤트를 즉시 처리합니다.

    setprjProcess.setWorkingDirectory(setprjPath);
    setprjProcess.start("csh", arguments);
    setprjProcess.waitForFinished(-1);

    //---------- Output Log --------
    output = setprjProcess.readAllStandardOutput();
    // QByteArray를 QString로 변환
    outputStr = QString::fromUtf8(output);
    stringList = outputStr.split("\n", QString::SkipEmptyParts);
    outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut(line);

#else
        emit noticeMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
    }

    errorOutput = setprjProcess.readAllStandardError();

    if (errorOutput.count()>0)
    {
#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut(QString("------------  Error(run mergescript) ------------\n"), true);
#else
        emit errorMessage(QString("------------  Error(run mergescript) ------------\n"));
#endif // LOG_OUT_SUPPORT_20240104

        // QByteArray를 QString로 변환
        outputStr = QString::fromUtf8(errorOutput);
        stringList = outputStr.split("\n", QString::SkipEmptyParts);
        outputList = stringList;

        // 출력 결과 확인
        for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
            Singleton::getInstance().getMyCommandWind()->LogOut(line, true);
#else
            emit errorMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
        }
    }

    //////////////////////// dumy  /////////////////
    QThread::sleep(1);

    setprjProcess.start("csh", QStringList(""));
    setprjProcess.waitForFinished(-1);

    output = setprjProcess.readAllStandardOutput();
    // QByteArray를 QString로 변환
    outputStr = QString::fromUtf8(output);
    // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
    stringList = outputStr.split("\n", QString::SkipEmptyParts);
    outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut(line);
#else
        emit noticeMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
    }
    //////////////////////////////////////////////


    //. 예외 처리 항목
    //.TODO :  3. /AMagic/fpga/ 에 아래와 같은 파일이 있는지 확인 - fpga_bit_gen_{앞서 지정한 Design Name}.tcl : VLNV N 부분에 해당
    //.TODO :  4. /AMagic/fpga/magic_sources/constraints/ 에 아래와 같은 파일이 있는지 확인 - {앞서 지정한 Design Name}.xdc : VLNV N 부분에 해당


    //--------------------------------------
    //   (3) 최종 시뮬레이트 동작 (fpga)
    //--------------------------------------
    if(1) {
        Progresslbl->setText("fpga");

        runParmlist.clear();
        QString runpath = setprjPath + "/fpga/";

        //runParmlist << "source setprj";  <-- 23.11.03

        //runParmlist << setprjPath + "/bin/awsh vivado -source ./fpga_bit_gen_{앞서 지정한 Design Name}.tcl
        runParmlist << "cd " + runpath;
//        runParmlist << setprjPath + "/bin/awsh vivado -source ./fpga_bit_gen_" + nameTxt->text() + ".tcl";
        runParmlist << setprjPath + "/bin/awsh vivado -source " + runpath + "/fpga_bit_gen_" + nameTxt->text() + ".tcl";

        arguments << "-c" << runParmlist.join("; ");
        //arguments <<  runParmlist.join("; ");
        QString paramString1 = arguments.join(" ");

        setprjProcess.setWorkingDirectory(setprjPath); //<---

#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut("========== Output (run vivado) =========\n script >> ");
        Singleton::getInstance().getMyCommandWind()->LogOut(setprjPath + " => "+ paramString1+ "\n");
#else
        emit noticeMessage("========== Output (run vivado) =========\n script >> ");
        emit noticeMessage(setprjPath + " => "+ paramString1+ "\n");
#endif // LOG_OUT_SUPPORT_20240104

        QCoreApplication::processEvents(); // UI 이벤트를 즉시 처리합니다.

        setprjProcess.start("csh", arguments);
        setprjProcess.waitForFinished(-1);

        //---------- Output Log --------
        output = setprjProcess.readAllStandardOutput();
        // QByteArray를 QString로 변환
        outputStr = QString::fromUtf8(output);
        // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
        stringList = outputStr.split("\n", QString::SkipEmptyParts);
        outputList = stringList;

        // 출력 결과 확인
        for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
            Singleton::getInstance().getMyCommandWind()->LogOut(line);
#else
            emit noticeMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
        }

        errorOutput = setprjProcess.readAllStandardError();

        if (errorOutput.count()>0)
        {
#if LOG_OUT_SUPPORT_20240104 == 0
            Singleton::getInstance().getMyCommandWind()->LogOut(QString("------------  Error(run vivado) ------------\n"), true);
#else
            emit errorMessage(QString("------------  Error(run vivado) ------------\n"));
#endif // LOG_OUT_SUPPORT_20240104

            // QByteArray를 QString로 변환
            outputStr = QString::fromUtf8(errorOutput);
            stringList = outputStr.split("\n", QString::SkipEmptyParts);
            outputList = stringList;

            // 출력 결과 확인
            for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
                Singleton::getInstance().getMyCommandWind()->LogOut(line, true);
#else
                emit errorMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
            }
        }
    }

    //////////////////////// dumy  /////////////////
    QThread::sleep(1);

    setprjProcess.start("csh", QStringList(""));
    setprjProcess.waitForFinished(-1);

    output = setprjProcess.readAllStandardOutput();
    // QByteArray를 QString로 변환
    outputStr = QString::fromUtf8(output);
    // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
    stringList = outputStr.split("\n", QString::SkipEmptyParts);
    outputList = stringList;

    // 출력 결과 확인
    for (const QString &line : outputList) {
#if LOG_OUT_SUPPORT_20240104 == 0
        Singleton::getInstance().getMyCommandWind()->LogOut(line);
#else
        emit noticeMessage(line);
#endif // LOG_OUT_SUPPORT_20240104
    }
    //////////////////////////////////////////////


}

void PythonFPGA::OnPythonScriptRunClicked()
{
    pythonScriptRunButton->setEnabled(false);
    closeButton->setEnabled(false);

    startProcess();

    Progresslbl->setText("end");
    pythonScriptRunButton->setEnabled(true);
    closeButton->setEnabled(true);

}

void PythonFPGA::Destroy()
{
    this->close();
    delete this;
}
