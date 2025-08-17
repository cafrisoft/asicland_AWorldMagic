#include "copythread.h"

#include <QDir>
#include <QFileInfo>

CopyThread::CopyThread(const QString& source, const QString& destination)
       : m_source(source), m_destination(destination)
{

}

void CopyThread::copyRecursive(const QString& src, const QString& dst)
{
    QDir sourceDir(src);
    QDir destinationDir(dst);

    if (!destinationDir.exists()) {
        destinationDir.mkpath(".");
    }

    QFileInfoList files = sourceDir.entryInfoList(QDir::Files);
    int totalFiles = files.size();
    int copiedFiles = 0;

    foreach(const QFileInfo& fileInfo, files) {
        QString srcFilePath = fileInfo.absoluteFilePath();
        QString destFilePath = destinationDir.filePath(fileInfo.fileName());

        if (QFile::copy(srcFilePath, destFilePath)) {
            copiedFiles++;
            emit progressChanged((copiedFiles * 100) / totalFiles);
        }
    }

    QFileInfoList dirs = sourceDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(const QFileInfo& dirInfo, dirs) {
        QString srcDirPath = dirInfo.absoluteFilePath();
        QString destDirPath = destinationDir.filePath(dirInfo.fileName());

        copyRecursive(srcDirPath, destDirPath);
    }
}
