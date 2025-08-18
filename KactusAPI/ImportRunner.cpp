//-----------------------------------------------------------------------------
// File: ImportRunner.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 18.09.2014
//
// Description:
// Runs import plugins for a given file and component.
//-----------------------------------------------------------------------------

#include <QApplication>
#include <QRegularExpression>
#include <QScrollBar>
#include <QTextStream>
#include <QTextBlock>
#include <QFileInfo>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/FileSet.h>
#include <IPXACTmodels/Component/File.h>

#include <KactusAPI/LibraryHandler.h>
#include <KactusAPI/VersionHelper.h>

#include <KactusAPI/NullParser.h>

#include <KactusAPI/ExpressionSupport.h>
#include <KactusAPI/HighlightSource.h>
#include <KactusAPI/ImportHighlighter.h>

#include <Plugins/VerilogImport/VerilogImporter.h>
#include <Plugins/VerilogIncludeImport/VerilogIncludeImport.h>
#include <Plugins/VerilogSourceAnalyzer/VerilogSourceAnalyzer.h>
#include <Plugins/CppSourceAnalyzer/CppSourceAnalyzer.h>


//#include <IPlugin.h>
//#include <ISourceAnalyzerPlugin.h>
//#include <ImportPlugin.h>
//#include <IncludeImportPlugin.h>

//#include <FileHandler.h>

#include "IPlugin.h"
#include "ISourceAnalyzerPlugin.h"
#include "ImportPlugin.h"
#include "IncludeImportPlugin.h"

#include "FileHandler.h"

#include "ImportRunner.h"

//-----------------------------------------------------------------------------
// Function: ImportRunner::ImportRunner()
//-----------------------------------------------------------------------------
ImportRunner::ImportRunner(LibraryInterface* designInterface,
                           LibraryInterface* libraryInterface,
                           QSharedPointer<ComponentParameterFinder> parameterFinder,
                           QTabWidget* displayTabs,
                           QObject* parent):
    QObject(parent),
    designInterface_(designInterface),
    libraryInterface_(libraryInterface),
    expressionParser_(new NullParser()),
    ImportPlugins_(),
    analyzerPlugins_(),
    parameterFinder_(parameterFinder),
    displayTabs_(displayTabs),
    fileSuffixTable_(FileHandler::constructFileSuffixTable()),
    componentsInFile_()
{
    
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::gatherComponentsFromFile()
//-----------------------------------------------------------------------------
QStringList ImportRunner::constructComponentDataFromFile(QString const& filePath, QString const& componentXMLPath,
    QSharedPointer<const Component> targetComponent)
{
    componentsInFile_.clear();

    QStringList filetypes = filetypesOf(filePath, *targetComponent->getFileSets().data());
    QString const& fileContent = readInputFile(filePath, componentXMLPath);

    QStringList availableComponentNames;

    for (ImportPlugin const* parser : importPluginsForFileTypes(filetypes))
    {
        QStringList possibleComponents = parser->getFileComponents(fileContent);
        for (auto component : possibleComponents)
        {
            QString componentName = parser->getComponentName(component);
            availableComponentNames.append(componentName);

            ImportRunner::AvailableComponent newComponent{ componentName, component };
            componentsInFile_.append(newComponent);
        }
    }

    return availableComponentNames;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::run()
//-----------------------------------------------------------------------------
QSharedPointer<Component> ImportRunner::run(QString const& componentName, QString const& filePath,
    QString const& componentXmlPath, QSharedPointer<const Component> targetComponent)
{
    if (displayTabs_ != nullptr)
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        displayTabs_->clear();
    }

    QSharedPointer<Component> importComponent(new Component(*targetComponent.data()));
    parameterFinder_->setComponent(importComponent);

    importIncludes(filePath, componentXmlPath, importComponent);

    QStringList filetypes = filetypesOf(filePath, *importComponent->getFileSets().data());
   
    importFile(componentName, filePath, componentXmlPath, importPluginsForFileTypes(filetypes), importComponent);

    if (displayTabs_ != nullptr)
    {
        QApplication::restoreOverrideCursor();
    }

    return importComponent;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::importFileTypes()
//-----------------------------------------------------------------------------
QStringList ImportRunner::importFileTypes() const
{
    QStringList fileTypes;

    for (ImportPlugin const* parser : ImportPlugins_)
    {
        fileTypes.append(parser->getSupportedFileTypes());
    }
    fileTypes.removeDuplicates();

    return fileTypes;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::loadPlugins()
//-----------------------------------------------------------------------------
void ImportRunner::loadPlugins(PluginManager const& pluginManager)
{
#if PLUGIN_TOOLS_SUPPORT_20240905 == 1
    for (IPlugin* plugin : pluginManager.getActivePlugins())
    {
        ImportPlugin* importPlugin = dynamic_cast<ImportPlugin*>(plugin);
        ISourceAnalyzerPlugin* analyzerPlugin = dynamic_cast<ISourceAnalyzerPlugin*>(plugin);
        if (importPlugin)
        {
            ImportPlugins_.append(importPlugin);
            addExpressionParserIfPossible(importPlugin);
        }

        if (analyzerPlugin)
        {
            analyzerPlugins_.append(analyzerPlugin);
        }
    }
#else
//    VerilogImporter * verilogImporter = new VerilogImporter(designInterface_, libraryInterface_, messageChannel_, VersionHelper::createVersionString(), this);
    VerilogImporter * verilogImporter = new VerilogImporter();

    ImportPlugin* importPlugin = dynamic_cast<ImportPlugin*>(verilogImporter);

    if (importPlugin)
    {
        ImportPlugins_.append(importPlugin);
        addExpressionParserIfPossible(importPlugin);
    }

    VerilogIncludeImport * verilogIncludeImport = new VerilogIncludeImport();

    importPlugin = dynamic_cast<ImportPlugin*>(verilogIncludeImport);

    if (importPlugin)
    {
        ImportPlugins_.append(importPlugin);
        addExpressionParserIfPossible(importPlugin);
    }


    VerilogSourceAnalyzer * verilogSourceAnalyzer = new VerilogSourceAnalyzer();

    ISourceAnalyzerPlugin* analyzerPlugin = dynamic_cast<ISourceAnalyzerPlugin*>(verilogSourceAnalyzer);

    if (analyzerPlugin)
    {
        analyzerPlugins_.append(analyzerPlugin);
    }

    CppSourceAnalyzer * cppSourceAnalyzer = new CppSourceAnalyzer();

    analyzerPlugin = dynamic_cast<ISourceAnalyzerPlugin*>(cppSourceAnalyzer);

    if (analyzerPlugin)
    {
        analyzerPlugins_.append(analyzerPlugin);
    }

#endif // PLUGIN_TOOLS_SUPPORT_20240905

}

//-----------------------------------------------------------------------------
// Function: ImportRunner::setVerilogExpressionParser()
//-----------------------------------------------------------------------------
void ImportRunner::setExpressionParser(QSharedPointer<ExpressionParser> parser)
{
    expressionParser_ = parser;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::addExpressionParserIfPossible()
//-----------------------------------------------------------------------------
void ImportRunner::addExpressionParserIfPossible(ImportPlugin* importPlugin) const
{
    ExpressionSupport* pluginRequiringParser = dynamic_cast<ExpressionSupport*>(importPlugin);
    if (pluginRequiringParser)
    {
        pluginRequiringParser->setExpressionParser(expressionParser_);
    }
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::scrollToFirstHighlight()
//-----------------------------------------------------------------------------
void ImportRunner::scrollSourceDisplayToFirstHighlight(QPlainTextEdit* sourceDisplay) const
{
    QTextCursor cursor = sourceDisplay->textCursor();
    cursor.movePosition(QTextCursor::Start);
    QTextCharFormat initialFormat = cursor.charFormat();

    while (cursor.movePosition(QTextCursor::NextBlock) && cursor.charFormat() == initialFormat)
    {
        // Loop until first formatted block is found.
    }

    if (!cursor.atEnd())
    {
        int row = cursor.block().firstLineNumber();
        sourceDisplay->verticalScrollBar()->setValue(row);
    }
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::importIncludes()
//-----------------------------------------------------------------------------
void ImportRunner::importIncludes(QString const& filePath, QString const& componentXmlPath, 
    QSharedPointer<Component> importComponent)
{
    if (filePath.isEmpty())
    {
        return;
    }

    QList<FileDependencyDesc> dependencies;
    QString componentPath = QFileInfo(componentXmlPath).absolutePath() + QStringLiteral("/");
    QStringList filetypes = filetypesOf(filePath, *importComponent->getFileSets().data());

    for (ISourceAnalyzerPlugin* analyzer : analyzerPluginsForFileTypes(filetypes))
    {
        analyzer->beginAnalysis(importComponent.data(), componentPath);
        dependencies.append(analyzer->getFileDependencies(importComponent.data(), componentPath, 
            General::getAbsolutePath(componentXmlPath, filePath)));
        analyzer->endAnalysis(importComponent.data(), componentPath);
    }

    QList<ImportPlugin*> importPlugins = includeImportPluginsForFileTypes(filetypes);
    QString basePath = QFileInfo(General::getAbsolutePath(componentXmlPath, filePath)).absolutePath() + 
        QStringLiteral("/");

    for (FileDependencyDesc const& dependency : dependencies)
    {        
        importFile(QString(), dependency.filename, basePath, importPlugins, importComponent);
    }
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::filetypesOf()
//-----------------------------------------------------------------------------
QStringList ImportRunner::filetypesOf(QString const& fileName, QList<QSharedPointer<FileSet> > const& fileSets)
    const
{
    QStringList fileTypes;

    QRegularExpression filePattern(QStringLiteral("(?:^|[\\\\/])") + fileName + QStringLiteral("$"));

    for (auto const& fileSet : fileSets)
    {
        for (auto const& file : *fileSet->getFiles())
        {
            if (filePattern.match(file->name()).hasMatch())
            {
                fileTypes.append(*file->getFileTypes());
            }
        }
    }

    fileTypes.removeDuplicates();

    if (fileTypes.isEmpty() && !fileName.isEmpty())
    {
        QString fileSuffix = QFileInfo(fileName).suffix();
        fileTypes.append(FileHandler::getFileTypeForSuffix(fileSuffixTable_, fileSuffix));
    }

    return fileTypes;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::analyzerPluginsForFileTypes()
//-----------------------------------------------------------------------------
QList<ISourceAnalyzerPlugin*> ImportRunner::analyzerPluginsForFileTypes(QStringList filetypes) const
{
    QList<ISourceAnalyzerPlugin*> analysersForFiletype;
    for (ISourceAnalyzerPlugin* analyzer : analyzerPlugins_)
    {
        QStringList supportedTypes = analyzer->getSupportedFileTypes();
        for (QString const& filetype : filetypes)
        {
            if (supportedTypes.contains(filetype) && !analysersForFiletype.contains(analyzer))
            {
                analysersForFiletype.append(analyzer);
            }
        }
    }

    return analysersForFiletype;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::importFile()
//-----------------------------------------------------------------------------
void ImportRunner::importFile(QString const& componentName, QString const& filePath,
    QString const& absoluteBasePath, QList<ImportPlugin *> importPluginsForFile,
    QSharedPointer<Component> importComponent)
{

    QString componentDeclaration = getComponentFromFile(componentName);
    if (filePath.isEmpty() || (componentDeclaration.isEmpty() && !componentName.isEmpty()))
    {
        return;
    }

    QString const& fileContent = readInputFile(filePath, absoluteBasePath);

    QPlainTextEdit* sourceDisplayer = nullptr;
    Highlighter* highlighter = nullptr;

    if (displayTabs_ != nullptr)
    {
        sourceDisplayer = createSourceDisplayForFile(filePath);
        sourceDisplayer->setPlainText(fileContent);

        highlighter = new ImportHighlighter(sourceDisplayer, this);
        highlighter->applyFontColor(fileContent, Qt::gray);
    }

    QStringList compatibilityWarnings;
    for (ImportPlugin* parser : importPluginsForFile)
    {
        compatibilityWarnings.append(parser->getCompatibilityWarnings());
        
        HighlightSource* highlightSource = dynamic_cast<HighlightSource*>(parser);
        if (highlightSource)
        {
            highlightSource->setHighlighter(highlighter);
            parser->import(fileContent, componentDeclaration, importComponent);
            highlightSource->setHighlighter(nullptr);
        }
        else
        {
            parser->import(fileContent, componentDeclaration, importComponent);
        }
    }
    compatibilityWarnings.removeAll(QString());
    emit noticeMessage(compatibilityWarnings.join(QStringLiteral("\n")));


    if (displayTabs_ != nullptr)
    {
        displayTabs_->setCurrentIndex(displayTabs_->indexOf(sourceDisplayer));
        scrollSourceDisplayToFirstHighlight(sourceDisplayer);

        delete highlighter;
    }
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::getComponentFromFile()
//-----------------------------------------------------------------------------
QString ImportRunner::getComponentFromFile(QString const& componentName) const
{
    if (!componentName.isEmpty())
    {
        for (auto component : componentsInFile_)
        {
            if (component.componentName_.compare(componentName) == 0)
            {
                return component.componentFile_;
            }
        }
    }

    return QString();
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::createSourceDisplayForFile()
//-----------------------------------------------------------------------------
QPlainTextEdit* ImportRunner::createSourceDisplayForFile(QString const& filePath) const
{
    QPlainTextEdit* sourceDisplay = new QPlainTextEdit(displayTabs_);

    QFont font(QStringLiteral("Courier"));
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(9);

    sourceDisplay->setFont(font);
    sourceDisplay->setTabStopDistance(4 * sourceDisplay->fontMetrics().horizontalAdvance(QLatin1Char(' ')));
    //sourceDisplay->setTabStopWidth(4 * sourceDisplay->fontMetrics().width(QLatin1Char(' ')));     //. not surported Qt5.11. 부터 지원
    sourceDisplay->setReadOnly(true);
    sourceDisplay->setCursorWidth(0);

    displayTabs_->insertTab(0, sourceDisplay, QFileInfo(filePath).fileName());

    return sourceDisplay;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::readInputFile()
//-----------------------------------------------------------------------------
QString ImportRunner::readInputFile(QString const& relativePath, QString const& basePath) const
{
    QString fileContent = QString();

    QString absoluteFilePath = General::getAbsolutePath(basePath, relativePath);

//    QString absoluteFilePath;
//    QFileInfo relativeInfo(relativePath);
//    if (relativeInfo.isAbsolute()) {
//        absoluteFilePath =  relativePath;
//    }
//    else {
//        QDir baseDir = QFileInfo(basePath).dir();
//        absoluteFilePath = baseDir.filePath(relativePath);

////        absoluteFilePath = relativeInfo.absolutePath();
////        QDir relativeDir(relativeInfo.absolutePath());
////        absoluteFilePath =  relativeDir.absoluteFilePath(relativePath);
//    }

    QFile importedFile(absoluteFilePath);
    if (QFileInfo(absoluteFilePath).exists() && importedFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&importedFile);
        fileContent = stream.readAll();
        fileContent.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));

        importedFile.close();
    }
    else
    {
        fileContent = tr("<File %1 not found in the file sets.>").arg(relativePath);
    }

    return fileContent;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::importPluginsForFileTypes()
//-----------------------------------------------------------------------------
QList<ImportPlugin*> ImportRunner::importPluginsForFileTypes(QStringList const& filetypes) const
{
    QList<ImportPlugin*> compatiblePlugins;
    for (ImportPlugin* importer : ImportPlugins_)
    {
        if (dynamic_cast<IncludeImportPlugin*>(importer) == 0)
        {
            QStringList parserAcceptedFiletypes = importer->getSupportedFileTypes();
            for (QString const& filetype : filetypes)
            {
                if (parserAcceptedFiletypes.contains(filetype) && !compatiblePlugins.contains(importer))
                {
                    compatiblePlugins.prepend(importer);
                }
            }
        }
    }

    return compatiblePlugins;
}

//-----------------------------------------------------------------------------
// Function: ImportRunner::includeImportPluginsForFileTypes()
//-----------------------------------------------------------------------------
QList<ImportPlugin*> ImportRunner::includeImportPluginsForFileTypes(QStringList const& filetypes) const
{
    QList<ImportPlugin*> compatiblePlugins;
    for (ImportPlugin* importer : ImportPlugins_)
    {
        if (dynamic_cast<IncludeImportPlugin*>(importer) != 0)
        {
            QStringList parserAcceptedFiletypes = importer->getSupportedFileTypes();
            for (QString const& filetype : filetypes)
            {
                if (parserAcceptedFiletypes.contains(filetype) && !compatiblePlugins.contains(importer))
                {
                    compatiblePlugins.prepend(importer);
                }
            }
        }
    }

    return compatiblePlugins;
}
