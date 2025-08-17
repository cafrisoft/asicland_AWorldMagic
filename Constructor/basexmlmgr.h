#ifndef BASEXMLMGR_H
#define BASEXMLMGR_H

#include <QString>

class BaseXMLMgr
{
public:
    BaseXMLMgr();

    QString configPath;
    QString aMagicRoot; // /AMagic
    QString awshPath;
    QString scriptRoot; //스크립트의 위치
    QString actFunctionNm; //fuction name으로 io.xml 만들때는 ""으로  나온다.
    QString actScriptFile;
    QString libraryPath;

    void SetScriptRootPath(const QString &path);
    bool LoadScriptPath(QString funcScriptNm);

    bool XMLParsedTree();
    void saveStringToFile(const QString &content, const QString &filePath);
    QString readFile(const QString& filePath);
    bool CreateFolder(const QString &folderPath, const QString &fileToDelete);
};

#endif // BASEXMLMGR_H
