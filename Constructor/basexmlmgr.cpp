#include "basexmlmgr.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

BaseXMLMgr::BaseXMLMgr()
{
    configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    SetScriptRootPath("AMagic");
}


void BaseXMLMgr::SetScriptRootPath(const QString &rootPath)  {

    QSettings settings(configPath, QSettings::IniFormat);

    aMagicRoot = QDir::homePath() + "/" + rootPath;
    awshPath   = aMagicRoot + "/bin/awsh";
    scriptRoot = aMagicRoot + "/script";
    libraryPath = aMagicRoot + "/library";

    settings.setValue("amagic", aMagicRoot);
}

bool BaseXMLMgr::LoadScriptPath(QString funcScriptNm)
{
    QSettings settings(configPath, QSettings::IniFormat);

    aMagicRoot = settings.value("scriptroot", "").toString();
    if(aMagicRoot=="")
    {
        SetScriptRootPath("AMagic");
    }
    else
    {
        awshPath   = aMagicRoot + "/bin/awsh";
        scriptRoot = aMagicRoot + "/script";
        libraryPath = aMagicRoot + "/library";
    }
    actFunctionNm = funcScriptNm;
    actScriptFile = scriptRoot + "//"+ funcScriptNm;

    return true;
}

void BaseXMLMgr::saveStringToFile(const QString &content, const QString &filePath)
{
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << content;
        file.close();
        //QDebug() << "File saved successfully.";
    } else {
        //QDebug() << "Failed to save file.";
    }
}

QString BaseXMLMgr::readFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";  // 또는 오류 메시지 반환
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return content;
}

bool BaseXMLMgr::CreateFolder(const QString &folderPath, const QString &fileToDelete)
{
    // 폴더 생성
    QDir dir;
    if (!dir.mkpath(folderPath)) {
        return false;  // 폴더 생성에 실패한 경우 false 반환
    }

    // 지울 파일이 해당 폴더에 존재하는지 확인
    QString fullFilePath = /*folderPath + "/" +*/ fileToDelete;
    if (QFile::exists(fullFilePath)) {
        if (!QFile::remove(fullFilePath)) {
            return false;  // 파일 삭제에 실패한 경우 false 반환
        }
    }

    return true;  // 성공적으로 폴더를 생성하고 필요한 경우 파일을 삭제한 경우 true 반환
}
