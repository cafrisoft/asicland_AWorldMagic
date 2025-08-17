#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>


class Utility : public QObject
{
public:
    Utility();

    static void createSingleDirectory(const QString &path);
    static bool copyDirectoryContents(const QString &sourcePath, const QString &destinationPath);
    static bool copyFile(QObject *parent, const QString & sourcePath, const QString &destinationPath);
};

#endif // UTILITY_H
