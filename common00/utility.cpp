#include "utility.h"

#include <QDir>
#include <QProcess>
#include <QDebug>

Utility::Utility()
{

}
void Utility::createSingleDirectory(const QString &path)
{
    QDir dir;
    if (dir.mkdir(path)) {
        qDebug() << "Single directory created:" << path;
    } else {
        qWarning() << "Failed to create single directory:" << path;
    }
}

bool Utility::copyDirectoryContents(const QString &sourcePath, const QString &destinationPath)
{
    QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) {
        qWarning() << "Source directory does not exist:" << sourcePath;
        return false;
    }

    QDir destinationDir(destinationPath);
    if (!destinationDir.exists()) {
        if (!destinationDir.mkpath(destinationPath)) {
            qWarning() << "Failed to create destination directory:" << destinationPath;
            return false;
        }
    }

    foreach (QString fileName, sourceDir.entryList(QDir::Files)) {
        QString sourceFilePath = sourceDir.absoluteFilePath(fileName);
        QString destinationFilePath = destinationDir.absoluteFilePath(fileName);
        if (!QFile::copy(sourceFilePath, destinationFilePath)) {
            qWarning() << "Failed to copy file:" << sourceFilePath << "to" << destinationFilePath;
            return false;
        }
    }

    foreach (QString subDirName, sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString sourceSubDirPath = sourceDir.absoluteFilePath(subDirName);
        QString destinationSubDirPath = destinationDir.absoluteFilePath(subDirName);
        if (!copyDirectoryContents(sourceSubDirPath, destinationSubDirPath)) {
            return false;
        }
    }

    return true;
}


//-----------------------------------------------------------------------------
// Function: Utility::copyFile()
//-----------------------------------------------------------------------------
bool Utility::copyFile(QObject *parent, const QString & sourcePath, const QString &destinationPath)
{
    // cp -RP "source patth" "destination path" *
    bool bRet = false;

    QDir dir(destinationPath);

    if (!dir.exists()) {
//        if (dir.mkdir(destinationPath)) {

            QString program = "cp";
            QStringList arguments;

            arguments.append("-RP");
            arguments.append(sourcePath);
            arguments.append(destinationPath);

            QProcess *process;

            process = new QProcess(parent);

            process->setProgram(program);
            process->setArguments(arguments);
            process->start();

            if (process->waitForFinished(-1)) {
                bRet = true;

                QByteArray result = process->readAllStandardOutput();
                qDebug() << "Command output:" << result;
            }
            else {
                bRet = false;

                QByteArray error = process->readAllStandardError();
                qDebug() << "Error output:" << error;
            }

            delete process;
//        }
    }

    return bRet;
}
