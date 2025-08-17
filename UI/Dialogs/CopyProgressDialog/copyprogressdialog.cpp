#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>

#include "copyprogressdialog.h"


CopyProgressDialog::CopyProgressDialog(QWidget *parent, Qt::WindowFlags flags) :
    QProgressDialog(parent, flags)
{

}

CopyProgressDialog::CopyProgressDialog(const QString &labelText,
                                       const QString &cancelButtonText,
                                       int minimum,
                                       int maximum,
                                       QWidget *parent,
                                       Qt::WindowFlags flags) :
    QProgressDialog(labelText, cancelButtonText, minimum, maximum, parent, flags)
{

}

CopyProgressDialog::~CopyProgressDialog()
{
}

bool CopyProgressDialog::copyFileWithProgress(const QString &sourcePath, const QString &destinationPath)
{
    QFile sourceFile(sourcePath);
    if (!sourceFile.exists()) {
        qWarning() << "Source file does not exist:" << sourcePath;
        return false;
    }

    if (!sourceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open source file for reading:" << sourcePath;
        return false;
    }

    QFile destinationFile(destinationPath);
    if (!destinationFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open destination file for writing:" << destinationPath;
        sourceFile.close();
        return false;
    }

    qint64 totalSize = sourceFile.size();
    qint64 copiedSize = 0;
    const qint64 bufferSize = 4096;
    char buffer[bufferSize];

    while (!sourceFile.atEnd() && !wasCanceled()) {
        qint64 bytesRead = sourceFile.read(buffer, bufferSize);
        if (bytesRead == -1) {
            qWarning() << "Failed to read from source file:" << sourcePath;
            sourceFile.close();
            destinationFile.close();
            return false;
        }

        qint64 bytesWritten = destinationFile.write(buffer, bytesRead);
        if (bytesWritten == -1) {
            qWarning() << "Failed to write to destination file:" << destinationPath;
            sourceFile.close();
            destinationFile.close();
            return false;
        }

        copiedSize += bytesWritten;
        setValue(static_cast<int>((copiedSize * 100) / totalSize));
        QApplication::processEvents();
    }

    sourceFile.close();
    destinationFile.close();

    if (wasCanceled()) {
        destinationFile.remove();
        return false;
    }

    return true;
}

bool CopyProgressDialog::copyDirectoryWithProgress(const QString &sourcePath, const QString &destinationPath)
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

    QStringList fileNames = sourceDir.entryList(QDir::Files);
    setMaximum(fileNames.size());

    for (int i = 0; i < fileNames.size(); ++i) {
        QString sourceFilePath = sourceDir.absoluteFilePath(fileNames.at(i));
        QString destinationFilePath = destinationDir.absoluteFilePath(fileNames.at(i));

        setLabelText(QString("Copying %1...").arg(fileNames.at(i)));
        if (!copyFileWithProgress(sourceFilePath, destinationFilePath)) {
            return false;
        }

        setValue(i + 1);
        if (wasCanceled()) {
            return false;
        }
    }

    QStringList dirNames = sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &dirName, dirNames) {
        QString sourceSubDirPath = sourceDir.absoluteFilePath(dirName);
        QString destinationSubDirPath = destinationDir.absoluteFilePath(dirName);
        if (!copyDirectoryWithProgress(sourceSubDirPath, destinationSubDirPath)) {
            return false;
        }
    }

    return true;
}
