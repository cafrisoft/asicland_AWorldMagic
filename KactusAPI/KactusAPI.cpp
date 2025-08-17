//-----------------------------------------------------------------------------
// File: KactusAPI.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Esko Pekkarinen
// Date: 25.02.2021
//
// Description:
// Defines the operations available for accessing Kactus2 core.
//-----------------------------------------------------------------------------

#include "KactusAPI.h"

#include <KactusAPI/VersionHelper.h>
#include <KactusAPI/ConsoleMediator.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Design/Design.h>
#include <IPXACTmodels/designConfiguration/DesignConfiguration.h>

#include <KactusAPI/IPXactSystemVerilogParser.h>

#include <KactusAPI/ProjectHandler.h>
#include <KactusAPI/LibraryHandler.h>

//#include <IPlugin.h>
//#include <IGeneratorPlugin.h>
//#include <PluginUtilityAdapter.h>
//#include <CLIGenerator.h>

#include "IPlugin.h"
#include "IGeneratorPlugin.h"
#include "PluginUtilityAdapter.h"
#include "CLIGenerator.h"

#include <KactusAPI/ImportRunner.h>

#include <QFileInfo>

LibraryInterface* KactusAPI::proejct_ = nullptr;
LibraryInterface* KactusAPI::proejctLibrary_ = nullptr;
LibraryInterface* KactusAPI::library_ = nullptr;
MessageMediator* KactusAPI::messageChannel_ = new ConsoleMediator();

//-----------------------------------------------------------------------------
// Function: KactusAPI::KactusAPI()
//-----------------------------------------------------------------------------
KactusAPI::KactusAPI(LibraryInterface* project,
                     LibraryInterface* proejctLibrary,
                     LibraryInterface* library,
                     MessageMediator* messageChannel)
{
    if (proejct_ == nullptr)
    {
        proejct_ = project;
    }

    if (proejctLibrary_ == nullptr)
    {
        proejctLibrary_ = proejctLibrary;
    }

    if (library_ == nullptr)
    {
        library_ = library;
    }

    if (messageChannel != nullptr)
    {
        delete messageChannel_;
        messageChannel_ = messageChannel;
    }
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getLibrary()
//-----------------------------------------------------------------------------
LibraryInterface* KactusAPI::getProject()
{
    if (proejct_ == nullptr)
    {
        proejct_ = ProjectHandler::getInstance();
    }

    Q_ASSERT(proejct_ != nullptr);
    return proejct_;
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getProjectLibrary()
//-----------------------------------------------------------------------------
LibraryInterface* KactusAPI::getProjectLibrary()
{
    if (proejctLibrary_ == nullptr)
    {
        proejctLibrary_ = ProjectHandler::getInstance();
    }

    Q_ASSERT(proejctLibrary_ != nullptr);
    return proejctLibrary_;
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getLibrary()
//-----------------------------------------------------------------------------
LibraryInterface* KactusAPI::getLibrary()
{
    if (library_ == nullptr)
    {
        library_ = LibraryHandler::getInstance();
    }

    Q_ASSERT(library_ != nullptr);
    return library_;
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getMessageChannel()
//-----------------------------------------------------------------------------
MessageMediator* KactusAPI::getMessageChannel()
{
    return messageChannel_;
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getVersion()
//-----------------------------------------------------------------------------
QString KactusAPI::getVersion()
{
    return QStringLiteral("Kactus2 ") + VersionHelper::versionFull();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getVersionFileString()
//-----------------------------------------------------------------------------
QString KactusAPI::getVersionFileString()
{
    return VersionHelper::versionFileStr();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getProjectPaths()
//-----------------------------------------------------------------------------
QStringList KactusAPI::getProjectPaths()
{
    QSettings settings;

    return  settings.value(QStringLiteral("Project/ActiveLocations")).toStringList();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::setProjectPaths()
//-----------------------------------------------------------------------------
void KactusAPI::setProjectPaths(QStringList const& activeLocations, QStringList const& allLocations)
{
    QSettings settings;

    QStringList oldLocations = settings.value(QStringLiteral("Project/Locations")).toStringList();
    QStringList newLocations = allLocations;

    if (allLocations.empty())
    {
        newLocations = oldLocations;
    }

    QStringList oldActives = settings.value(QStringLiteral("Project/ActiveLocations")).toStringList();
    QStringList newActives = activeLocations;

    for (auto location : newActives)
    {
        if (newLocations.contains(location) == false)
        {
            newLocations.append(location);
        }
    }

    if (newLocations != oldLocations || newActives != oldActives)
    {
        settings.setValue(QStringLiteral("Project/Locations"), newLocations);
        settings.setValue(QStringLiteral("Project/ActiveLocations"), newActives);
        library_->searchForIPXactFiles();
    }
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getDefaultProjectPath()
//-----------------------------------------------------------------------------
QString KactusAPI::getDefaultProjectPath()
{
    QSettings settings;
    return settings.value(QStringLiteral("Project/DefaultLocation")).toString();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::setDefaultProjectPath()
//-----------------------------------------------------------------------------
void KactusAPI::setDefaultProjectPath(QString const& path)
{
    QSettings settings;

    QStringList locations = settings.value(QStringLiteral("Project/Locations")).toStringList();
    if (locations.contains(path))
    {
        settings.setValue(QStringLiteral("Project/DefaultLocation"), path);
    }
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getLibraryPaths()
//-----------------------------------------------------------------------------
QStringList KactusAPI::getLibraryPaths()
{
    QSettings settings;

    return  settings.value(QStringLiteral("Library/ActiveLocations")).toStringList();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::setLibraryPaths()
//-----------------------------------------------------------------------------
void KactusAPI::setLibraryPaths(QStringList const& activeLocations, QStringList const& allLocations)
{
    QSettings settings;

    QStringList oldLocations = settings.value(QStringLiteral("Library/Locations")).toStringList();
    QStringList newLocations = allLocations;

    if (allLocations.empty())
    {
        newLocations = oldLocations;
    }

    QStringList oldActives = settings.value(QStringLiteral("Library/ActiveLocations")).toStringList();
    QStringList newActives = activeLocations;

    for (auto location : newActives)
    {
        if (newLocations.contains(location) == false)
        {
            newLocations.append(location);
        }
    }

    if (newLocations != oldLocations || newActives != oldActives)
    {
        settings.setValue(QStringLiteral("Library/Locations"), newLocations);
        settings.setValue(QStringLiteral("Library/ActiveLocations"), newActives);
        library_->searchForIPXactFiles();
    }
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::setLibraryPaths()
//-----------------------------------------------------------------------------
QString KactusAPI::getDefaultLibraryPath()
{
    QSettings settings;
    return settings.value(QStringLiteral("Library/DefaultLocation")).toString();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::setLibraryPaths()
//-----------------------------------------------------------------------------
void KactusAPI::setDefaultLibraryPath(QString const& path)
{
    QSettings settings;

    QStringList locations = settings.value(QStringLiteral("Library/Locations")).toStringList();
    if (locations.contains(path))
    {
        settings.setValue(QStringLiteral("Library/DefaultLocation"), path);
    }
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::importFile()
//-----------------------------------------------------------------------------
int KactusAPI::importFile(LibraryInterface* designInterface,
                          LibraryInterface* libraryInterface,
                          QString const& filePath,
                          VLNV const& targetVLNV,
                          bool overwrite)
{
    bool existing = libraryInterface->contains(targetVLNV);

    if (existing && overwrite == false)
    {
        return 0;
    }

    QSharedPointer<Component const> existingComponent(nullptr);
    QString xmlPath = getDefaultLibraryPath() + QStringLiteral("/") + targetVLNV.toString(QStringLiteral("/")) +
        QStringLiteral("/") + targetVLNV.getName() + QStringLiteral(".xml");

    if (existing)
    {
        existingComponent = libraryInterface->getModel<Component>(targetVLNV);
        xmlPath = libraryInterface->getPath(targetVLNV);
    }
    else
    {
        existingComponent = QSharedPointer<Component const>(new Component(targetVLNV));
    }

    QSharedPointer<ComponentParameterFinder> parameterFinder(new ComponentParameterFinder(existingComponent));
    QSharedPointer<ExpressionParser> expressionParser(new IPXactSystemVerilogParser(parameterFinder));

    ImportRunner runner(designInterface, libraryInterface, parameterFinder, nullptr, nullptr);
    runner.setExpressionParser(expressionParser);
    runner.loadPlugins(PluginManager::getInstance());

    QStringList names = runner.constructComponentDataFromFile(filePath, xmlPath, existingComponent);

    if (names.isEmpty() == false)
    {
        QSharedPointer<Component> importedComponent = 
            runner.run(names.first(), filePath, xmlPath, existingComponent);
  
        if (existing)
        {
            libraryInterface->writeModelToFile(importedComponent);
        }
        else
        {
            libraryInterface->writeModelToFile(xmlPath, importedComponent);
        }

        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::runGenerator()
//-----------------------------------------------------------------------------
void KactusAPI::runGenerator(IGeneratorPlugin* plugin, VLNV const& componentVLNV, QString const& viewName,
    QString const& outputDirectory, QWidget* parentWidget)
{    
    QSharedPointer<Component> component = proejct_->getModel<Component>(componentVLNV);
    if (component == nullptr)
    {
        return;
    }

    VLNV designVLNV; 
    QSharedPointer<Design> design;
    
    VLNV configVLNV = component->getHierRef(viewName);
    QSharedPointer<DesignConfiguration> designConfiguration;    

    if (configVLNV.getType() == VLNV::DESIGNCONFIGURATION)
    {        
        designConfiguration = proejct_->getModel<DesignConfiguration>(configVLNV);
        designVLNV = designConfiguration->getDesignRef();
    }
    else if (configVLNV.getType() == VLNV::DESIGN)
    {
        designVLNV = configVLNV;
        configVLNV.clear();
    }

    if (designVLNV.isEmpty() == false)
    {
        design = proejct_->getModel<Design>(designVLNV);
    }

    PluginUtilityAdapter adapter(proejct_, messageChannel_, VersionHelper::createVersionString(), parentWidget);

    CLIGenerator* cliRunnable = dynamic_cast<CLIGenerator*>(plugin);
    if (parentWidget == nullptr && cliRunnable)
    {
        cliRunnable->runGenerator(&adapter, component, design, designConfiguration, viewName, outputDirectory);
    }
    else if (plugin != nullptr)
    {
        plugin->runGenerator(&adapter, component, design, designConfiguration);
    }    
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getPlugins()
//-----------------------------------------------------------------------------
QList<IPlugin*> KactusAPI::getPlugins()
{    
    return PluginManager::getInstance().getAllPlugins();
}

//-----------------------------------------------------------------------------
// Function: KactusAPI::getDocumentFilePath()
//-----------------------------------------------------------------------------
QString KactusAPI::getDocumentFilePath(VLNV const& vlnv)
{
    return QFileInfo(library_->getPath(vlnv)).absolutePath();
}
