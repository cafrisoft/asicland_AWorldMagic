//-----------------------------------------------------------------------------
// File: CppSourceAnalyzer.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Honkonen, Joni-Matti Maatta
// Date: 18.1.2013
//
// Description:
// C/C++ source analyzer plugin.
//-----------------------------------------------------------------------------

#include <QRegularExpression>
#include <QCryptographicHash>
#include <QTextStream>

#include "CppSourceAnalyzer.h"


//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::CppSourceAnalyzer()
//-----------------------------------------------------------------------------
CppSourceAnalyzer::CppSourceAnalyzer(): fileTypes_()
{
    fileTypes_.append("cSource");
    fileTypes_.append("cppSource");
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getName()
//----------------------------------------------------------------------------
QString CppSourceAnalyzer::getName() const
{
    return "C/C++ Source Analyzer";
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getVersion()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::getVersion() const
{
    return "1.1";
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getDescription()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::getDescription() const
{
    return "Analyzes file dependencies from C/C++ files.";
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getVendor()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::getVendor() const
{
    return tr("Tampere University (tuni.fi)");
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getLicense()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::getLicense() const
{
    return tr("GPL2");
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getLicenseHolder()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::getLicenseHolder() const
{
    return tr("Public");
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getSettingsWidget()
//-----------------------------------------------------------------------------
QWidget* CppSourceAnalyzer::getSettingsWidget()
{
    return nullptr;
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getSupportedFileTypes()
//-----------------------------------------------------------------------------
QStringList CppSourceAnalyzer::getSupportedFileTypes() const
{
    return fileTypes_;
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::calculateHash()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::calculateHash(QString const& filename)
{
    // Try to open the file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        // File could not be opened, show error
        return 0;
    }

    QString source = getSourceData(file);

    // Calculate the hash
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(source.toLatin1());

    QString result = hash.result().toHex();
    return result;
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getFileDependencies()
//-----------------------------------------------------------------------------
QList<FileDependencyDesc> CppSourceAnalyzer::getFileDependencies(Component const* /*component*/,
                                            QString const& /*componentPath*/,
                                            QString const& filename)
{
    QList<FileDependencyDesc> dependencies;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QString source = getSourceData(file);

        // Find #includes
        QTextStream sourceStream(&source);
        while(!sourceStream.atEnd())
        {
            const QString currentLine = sourceStream.readLine();
            int includePosition = currentLine.indexOf("#include");
            // include found (can they be somewhere else than start of line?)
            if (includePosition == 0)
            {
                FileDependencyDesc dependency;

                int includeStart = 0;
                int includeEnd = 0;
                // includes with < >
                if (currentLine.indexOf("<") != -1)
                {
                    includeStart = currentLine.indexOf("<") + 1;
                    includeEnd = currentLine.indexOf(">");
                }
                // includes with " "
                else
                {
                    includeStart = currentLine.indexOf("\"") + 1;
                    includeEnd = currentLine.indexOf("\"", includeStart+1);
                }
                // Include found, TODO: store it
                QString includeName = currentLine.mid(includeStart, includeEnd-includeStart);
                dependency.filename = includeName;
                dependencies.append(dependency);
            }
        }
    }

    return dependencies;
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::getSourceData()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::getSourceData(QFile& file)
{
    // Read the file data
    QString fileData;
    while (!file.atEnd())
    {
        QString line = file.readLine();
        line = line.simplified();

        if (line == "")
        {
            // Skip empty lines
            continue;
        }

        // check if the line ends in backslash (split line) 
        while (line.length() > 0 && line.at(line.length()-1) == '\\')
        {
            line = line.left(line.length()-1);
            QString nextLine = file.readLine();
            line.append(nextLine.simplified());
        }

        fileData.append(line.append("\n"));
    }
    
    // Remove comments from the source
    QString finalData = removeComments(fileData);

    return finalData;
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::removeComments()
//-----------------------------------------------------------------------------
QString CppSourceAnalyzer::removeComments(QString const& source)
{
    QString finalData = source;

    QRegularExpression tokenExp("(/\\*|//|\")");
    QRegularExpression stringEndExp("(\\\\\"|\"|\n)");
    
    int index = finalData.indexOf(tokenExp);
    auto tokenMatch = tokenExp.match(finalData);

    while (tokenMatch.hasMatch())
    {
        auto token = tokenMatch.captured(1);
        if (token == "//")
        {
            // Single-line comment. Strip to the end of the line.
            int endIndex = finalData.indexOf('\n', index + tokenMatch.capturedLength());

            if (endIndex == -1)
            {
                endIndex = finalData.length();
            }

            finalData.remove(index, endIndex - index);
            index = index + 1;
        }
        else if (token == "/*")
        {
            // Multi-line string begins. Strip to the end marker.
            int endIndex = finalData.indexOf("*/", index + tokenMatch.capturedLength());

            if (endIndex == -1)
            {
                endIndex = finalData.length();
            }
            else
            {
                endIndex += 2;
            }
            
            finalData.remove(index, endIndex - index);
            index = index + 1;
        }
        else if (token == "\"")
        {
            // String begins. Just skip the string to its end.
            int endIndex = finalData.indexOf(stringEndExp, index + 1);
            auto stringEndMatch = stringEndExp.match(finalData, index + 1);

            while (stringEndMatch.hasMatch() && stringEndMatch.captured(1) != "\n")
            {
                if (stringEndMatch.captured(1) == "\"")
                {
                    ++endIndex;
                    break;
                }

                endIndex = finalData.indexOf(stringEndExp, endIndex + stringEndMatch.capturedLength());
                stringEndMatch = stringEndExp.match(finalData, endIndex);
            }

            if (endIndex == -1)
            {
                endIndex = finalData.length();
            }

            index = endIndex;
        }

        index = finalData.indexOf(tokenExp, index);
    }

    return finalData;
}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::beginAnalysis()
//-----------------------------------------------------------------------------
void CppSourceAnalyzer::beginAnalysis(Component const* /*component*/, QString const& /*componentPath*/)
{

}

//-----------------------------------------------------------------------------
// Function: CppSourceAnalyzer::endAnalysis()
//-----------------------------------------------------------------------------
void CppSourceAnalyzer::endAnalysis(Component const* /*component*/, QString const& /*componentPath*/)
{

}

QList<IPlugin::ExternalProgramRequirement> CppSourceAnalyzer::getProgramRequirements()
{
	return QList<IPlugin::ExternalProgramRequirement>();
}
