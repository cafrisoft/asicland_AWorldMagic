#include "pythonscriptrunner.h"
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QCheckBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include "singleton.h"
#include "CommandWindow/CommandWindow.h"

PythonScriptRunner::PythonScriptRunner(QWidget *parent)
    : QDialog{parent}
{
    setMinimumWidth(800);

    setWindowTitle("Simulate Runner");


    // Set the stylesheet to modify appearance
    this->setStyleSheet(
        "QDialog { background-color: white; }"
        "QPushButton { background-color: #E5E5E5; }"
        "QLineEdit, QTextEdit { background-color: white; border: 1px solid #ccc; }"
        "QLabel { background-color: transparent; }"
    );


    this->setAutoFillBackground(true);


    DbgMessageLabel = new QLabel("[  Generate Script Test Dialog  ]", this);

    QLabel *pythonNameLbl = new QLabel("setprj Path", this);
    pythonNameEdit = new QLineEdit(this);

    // Get the home directory of the current user
    QString homeDir = QDir::homePath();
    setprjPath = homeDir + "/AMagic";
    genScriptPath = homeDir + "/AMagic/bin/awsh";
    genScriptWorkingPath = homeDir + "/AMagic/script/hw_design_gen_by_tony";

    // Set the working directory to 'AMagic' folder inside the user's home directory
    pythonNameEdit->setText(setprjPath);
    pythonNameEdit->setDisabled(true);

    QLabel *pythonScriptPathLabel = new QLabel("Genarate Script Path", this);
    pythonScriptPathEdit = new QLineEdit(this);
    pythonScriptPathEdit->setText(genScriptPath);
    pythonScriptPathEdit->setDisabled(true);

    QLabel *getScriptWorkingDirLabel = new QLabel("Genarate Script Working Dir", this);
    genScriptWorkingPathEdit = new QLineEdit(this);
    genScriptWorkingPathEdit->setText(genScriptWorkingPath);

    QLabel *getScriptParm = new QLabel("Genarate Script Parameter", this);
    genParameter = new QLineEdit(this);

    genParameter->setText("awsh ./script_gen -vendor asicland -lib-name system -module-name test_system -version 230912.1");

    // 'PythonScript Run' 버튼 추가 및 연결된 함수 추가
    QPushButton *pythonScriptRunButton = new QPushButton("PythonScript Run", this);
    connect(pythonScriptRunButton, &QPushButton::clicked, this, &PythonScriptRunner::OnPythonScriptRunClicked);  // 연결된 함수 필요

    QLabel* resultLabel = new QLabel("Result", this);
    resultTextBox = new QTextEdit(this);
    QLabel* errLabel = new QLabel("Error", this);
    errTextBox = new QTextEdit(this);
    resultTextBox->setMinimumHeight(200);
    resultTextBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 윈도우의 폭과 높이를 모두 사용
    errTextBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 윈도우의 폭과 높이를 모두 사용


    closeButton = new QPushButton("닫기", this);

      QVBoxLayout *layout = new QVBoxLayout(this);
      layout->addWidget(DbgMessageLabel);
      layout->addSpacing(20);

      layout->addWidget(pythonNameLbl);
      layout->addWidget(pythonNameEdit);

      layout->addSpacing(10);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(pythonScriptPathLabel);
      layout->addWidget(pythonScriptPathEdit);

      layout->addSpacing(10);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(getScriptWorkingDirLabel);
      layout->addWidget(genScriptWorkingPathEdit);

      layout->addSpacing(10);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(getScriptParm);
      layout->addWidget(genParameter);

      layout->addSpacing(10);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(pythonScriptRunButton);
      layout->addSpacing(15);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(resultLabel);
      layout->addWidget(resultTextBox);
      layout->addSpacing(5);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(errLabel);
      layout->addWidget(errTextBox);

      layout->addSpacing(10);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(closeButton);

      //closeButton 윗쪽에 한 라인의 공백삽입
      layout->addSpacing(20);  // 20 픽셀의 공백을 추가. 필요에 따라 조절 가능
      layout->addWidget(closeButton);

      // Connect the close button to the Destroy function
      connect(closeButton, &QPushButton::clicked, this, &PythonScriptRunner::Destroy);
}

void PythonScriptRunner::OnPythonScriptRunClicked()
{
      resultTextBox->clear();
      errTextBox->clear();

      //---[1] setprj -----


      setprjProcess.setWorkingDirectory(setprjPath);
      setprjProcess.start("csh", QStringList() << "-c" << "source setprj");
      setprjProcess.waitForFinished(-1);

      //---------- Output Log --------
      QByteArray output = setprjProcess.readAllStandardOutput();
      // QByteArray를 QString로 변환
      QString outputStr = QString::fromUtf8(output);
      QStringList stringList = outputStr.split("\n", Qt::SkipEmptyParts);

      QList<QString> outputList = stringList;
      resultTextBox->append(" ==========  Output (source setprj) ========= \n");

            // 출력 결과 확인
      for (const QString &line : outputList) {
          //qDebug() << line;
          resultTextBox->append(line);
      }

      QByteArray errorOutput = setprjProcess.readAllStandardError();
      if(errorOutput.count()>0)
      {
          // QByteArray를 QString로 변환
          outputStr = QString::fromUtf8(errorOutput);
                     // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
          //....stringList = outputStr.split("\n", Qt::SkipEmptyParts);
          stringList = outputStr.split("\n", Qt::SkipEmptyParts);

          // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습니다.)
          outputList = stringList;
          qDebug() << "------------  Error ------------\n";
                       // 출력 결과 확인
          for (const QString &line : outputList) {
              errTextBox->append(line);
          }
      }

      QString param = genParameter->text();
      QStringList parmlist = param.split(' ');
      genScriptWorkingPath = genScriptWorkingPathEdit->text();

      //genScriptPath = homeDir + "/AMagic/bin/awsh";

      setprjProcess.setWorkingDirectory(genScriptWorkingPath);  //homeDir + "/AMagic/script/hw_design_gen_by_tony"
//      setprjProcess.start(genScriptPath, QStringList() << "./script_gen" << "-vendor" << "tony" << "-lib-name" << "ok" << "-module-name" << "gotit" << "-version" << "1.0");
      //genParameter->setText("./script_gen -vendor tony -lib-name ok -module-name gotit -version 1.0");
      setprjProcess.start("csh", parmlist);
      setprjProcess.waitForFinished(-1);


      //---------- Output Log --------
      output = setprjProcess.readAllStandardOutput();
      // QByteArray를 QString로 변환
      outputStr = QString::fromUtf8(output);
            // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
      //....stringList = outputStr.split("\n", Qt::SkipEmptyParts);
      stringList = outputStr.split("\n", Qt::SkipEmptyParts);
      // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습니다.)
      outputList = stringList;
      resultTextBox->append("========== Output (make xml) =========\n");
      Singleton::getInstance().getMyCommandWind()->LogOut("========== Output (make xml) =========\n");

            // 출력 결과 확인
      for (const QString &line : outputList) {
          //qDebug() << line;
          resultTextBox->append(line);
          Singleton::getInstance().getMyCommandWind()->LogOut(line);
      }

      errorOutput = setprjProcess.readAllStandardError();
      Singleton::getInstance().getMyCommandWind()->LogOut(QString("------------  Error(make xml) ------------\n"), true);
      if(errorOutput.count()>0)
      {
          errTextBox->append("------------  Error(make xml) ------------\n");

          // QByteArray를 QString로 변환
          outputStr = QString::fromUtf8(errorOutput);
                     // 줄바꿈 문자를 기준으로 문자열 분리하여 QStringList에 담기
          //....stringList = outputStr.split("\n", Qt::SkipEmptyParts);
          stringList = outputStr.split("\n", Qt::SkipEmptyParts);
          // QStringList를 QList<QString>로 변환 (사실상 QStringList 자체가 QList<QString>의 서브 클래스이므로 필요에 따라 이 변환을 생략할 수 있습니다.)
          outputList = stringList;

                      // 출력 결과 확인
          for (const QString &line : outputList) {
              //qDebug() << line;
              errTextBox->append(line);

              Singleton::getInstance().getMyCommandWind()->LogOut(line, true);
          }
      }

}

void PythonScriptRunner::Destroy()
{
      this->close();
      delete this;
}
