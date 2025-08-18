//-----------------------------------------------------------------------------
// File: FileHandler.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 08.10.2019
//
// Description:
// Handles data regarding files.
//-----------------------------------------------------------------------------

#include "FileHandler.h"

#include <QSettings>

//-----------------------------------------------------------------------------
// Function: FileHandler::constructFileSuffixTable()
//-----------------------------------------------------------------------------
QMultiMap<QString, QString> FileHandler::constructFileSuffixTable()
{
    QMultiMap<QString, QString> fileSuffixTable;

    QSettings settings;
    settings.beginGroup(QStringLiteral("FileTypes"));
    foreach(QString const& fileType, settings.childGroups())
    {
        settings.beginGroup(fileType);

        QStringList fileTypeExtensions = settings.value(QStringLiteral("Extensions")).toString().split(QLatin1Char(';'));
        foreach(QString const& extension, fileTypeExtensions)
        {
            fileSuffixTable.insert(extension, fileType);
        }
        settings.endGroup();
    }

    settings.endGroup();

    return fileSuffixTable;
}

//-----------------------------------------------------------------------------
// Function: FileHandler::getFileTypeForSuffix()
//-----------------------------------------------------------------------------
QString FileHandler::getFileTypeForSuffix(QMultiMap<QString, QString> const& fileSuffixTable, QString const& suffix)
{
//    QMultiMapIterator<QString, QString> tableIterator(fileSuffixTable);
//    while (tableIterator.hasNext())
//    {
//        tableIterator.next();

//        if (tableIterator.key() == suffix)
//        {
//            return tableIterator.value();
//        }
//    }

//    return QString();
    for (auto it = fileSuffixTable.begin(); it != fileSuffixTable.end(); ++it)
    {
        if (it.key() == suffix)
        {
            return it.value();
        }
    }

    return QString();
}
