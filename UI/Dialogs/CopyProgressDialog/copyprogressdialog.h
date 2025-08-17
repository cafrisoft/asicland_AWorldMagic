#ifndef COPYPROGRESSDIALOG_H
#define COPYPROGRESSDIALOG_H

#include <QProgressDialog>

class CopyProgressDialog : public QProgressDialog
{
    Q_OBJECT

public:
    explicit CopyProgressDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    CopyProgressDialog(const QString &labelText, const QString &cancelButtonText,
                    int minimum, int maximum, QWidget *parent = Q_NULLPTR,
                    Qt::WindowFlags flags = Qt::WindowFlags());
    ~CopyProgressDialog();

    bool copyFileWithProgress(const QString &sourcePath, const QString &destinationPath);
    bool copyDirectoryWithProgress(const QString &sourcePath, const QString &destinationPath);
};

#endif // COPYPROGRESSDIALOG_H
