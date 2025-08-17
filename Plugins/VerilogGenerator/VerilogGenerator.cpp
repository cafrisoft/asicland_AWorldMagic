//-----------------------------------------------------------------------------
// File: VerilogGenerator.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Janne Virtanen
// Date: 26.01.2017
//
// Description:
// Plugin for Verilog generation.
//-----------------------------------------------------------------------------

#include <QDir>

#include <IPXACTmodels/Design/Design.h>
#include <IPXACTmodels/designConfiguration/DesignConfiguration.h>

#include <KactusAPI/LibraryInterface.h>
//#include <KactusAPI/IPluginUtility.h>
#include <KactusAPI/MessageMediator.h>

#include <Plugins/common/LanguageHighlighter.h>
#include <Plugins/common/HDLParser/HDLCommandLineParser.h>
#include <Plugins/PluginSystem/GeneratorPlugin/MessagePasser.h>
#include <Plugins/PluginSystem/GeneratorPlugin/HDLGenerationDialog.h>

#include "VerilogSourceHighlight.h"

#include "VerilogWriterFactory/VerilogWriterFactory.h"

#include "VerilogGenerator.h"


//-----------------------------------------------------------------------------
// Function: VerilogGenerator::checkGeneratorSupport()
//-----------------------------------------------------------------------------
VerilogGenerator::VerilogGenerator(LibraryInterface* designInterface,
                                   LibraryInterface* libraryInterface,
                                   MessageMediator* messageChannel,
                                   QString const& versionString,
                                   QWidget *parent) : QObject(parent),
    designInterface_(designInterface),
    libraryInterface_(libraryInterface),
    messageChannel_(messageChannel),
    parentWidget_(parent),
    versionString_(versionString)
{
    settings_.generateInterfaces_ = false;
    settings_.lastFileSetName_ = "";
    settings_.lastViewName_ = "";
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getName()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getName() const
{
    return tr("Verilog Generator");
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getVersion()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getVersion() const
{
    return "2.4";
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getDescription()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getDescription() const
{
    return tr("Generates a Verilog module for a HW component or a hierarchy of modules for a HW design.");
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getVendor()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getVendor() const
{
    return tr("Tampere University (tuni.fi)");
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getLicense()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getLicense() const
{
    return tr("GPL2");
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getLicenseHolder()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getLicenseHolder() const
{
    return tr("Public");
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::checkGeneratorSupport()
//-----------------------------------------------------------------------------
bool VerilogGenerator::checkGeneratorSupport(QSharedPointer<Component const> component,
    QSharedPointer<Design const> design,
    QSharedPointer<DesignConfiguration const> designConfiguration) const
{
    // If design or design configuration exists, their implementation overrides the top component.
    if (design)
    {
        return design->getImplementation() == KactusAttribute::HW;
    }

    if (designConfiguration)
    {
        return designConfiguration->getImplementation() == KactusAttribute::HW;
    }
    
    // Else the availability is determined based on the top component.
    return component && component->getImplementation() == KactusAttribute::HW;    
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::runGenerator()
//-----------------------------------------------------------------------------
void VerilogGenerator::runGenerator(/*IPluginUtility* utility,*/
    QSharedPointer<Component> component,
    QSharedPointer<Design> design,
    QSharedPointer<DesignConfiguration> designConfiguration)
{
    // First state we are running. Tell the version.
    messageChannel_->showMessage(tr("Running %1 %2.").arg(getName(), getVersion()));

    // Must have a component under any condition.
    if (component.isNull())
    {
        messageChannel_->showError(tr("Null component given as a parameter."));
        return;
    }

    MessagePasser messages;

    GenerationTuple input;
    input.component = component;
    input.design = design;
    input.designConfiguration = designConfiguration;
    input.messages = &messages;

    VerilogWriterFactory factory(designInterface_, libraryInterface_, &messages, &settings_, versionString_, getVersion());

    // Create model for the configuration widget.
    QSharedPointer<GenerationControl> configuration(new GenerationControl
        (designInterface_, libraryInterface_, &factory, input, &settings_));

    // Create the dialog and execute: The user will accept/decline the configuration.
    HDLGenerationDialog dialog(configuration, QStringLiteral("Verilog"), parentWidget_);
    
    LanguageHighlighter highlighter(0);
    VerilogSourceHighlight style;
    style.apply(&highlighter);

    dialog.setPreviewHighlighter(&highlighter);

    connect(&messages, SIGNAL(errorMessage(const QString&)),
        &dialog, SLOT(onErrorMessage(const QString&)), Qt::UniqueConnection);
    connect(&messages, SIGNAL(noticeMessage(const QString&)),
       & dialog, SLOT(onNoticeMessage(const QString&)), Qt::UniqueConnection);

    dialog.onViewChanged();

    if (dialog.exec() == QDialog::Accepted)
    {
        messageChannel_->showMessage(tr("Generation complete."));
    }
    else
    {
        messageChannel_->showMessage(tr("Generation aborted."));
    }
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::runGenerator()
//-----------------------------------------------------------------------------
void VerilogGenerator::runGenerator(VLNV const& componentVLNV, QString const& viewName)
{
    QSharedPointer<Component> component = designInterface_->getModel<Component>(componentVLNV);
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
        designConfiguration = designInterface_->getModel<DesignConfiguration>(configVLNV);
        designVLNV = designConfiguration->getDesignRef();
    }
    else if (configVLNV.getType() == VLNV::DESIGN)
    {
        designVLNV = configVLNV;
        configVLNV.clear();
    }

    if (designVLNV.isEmpty() == false)
    {
        design = designInterface_->getModel<Design>(designVLNV);
    }

    // First state we are running. Tell the version.
    messageChannel_->showMessage(tr("Running %1 %2.").arg(getName(), getVersion()));

    // Must have a component under any condition.
    if (component.isNull())
    {
        messageChannel_->showError(tr("Null component given as a parameter."));
        return;
    }

    MessagePasser messages;

    GenerationTuple input;
    input.component = component;
    input.design = design;
    input.designConfiguration = designConfiguration;
    input.messages = &messages;

    VerilogWriterFactory factory(designInterface_, libraryInterface_, &messages, &settings_, versionString_, getVersion());

    // Create model for the configuration widget.
    QSharedPointer<GenerationControl> configuration(new GenerationControl
        (designInterface_, libraryInterface_, &factory, input, &settings_));

    // Create the dialog and execute: The user will accept/decline the configuration.
    HDLGenerationDialog dialog(configuration, QStringLiteral("Verilog"), parentWidget_);

    LanguageHighlighter highlighter(0);
    VerilogSourceHighlight style;
    style.apply(&highlighter);

    dialog.setPreviewHighlighter(&highlighter);

    connect(&messages, SIGNAL(errorMessage(const QString&)),
        &dialog, SLOT(onErrorMessage(const QString&)), Qt::UniqueConnection);
    connect(&messages, SIGNAL(noticeMessage(const QString&)),
       & dialog, SLOT(onNoticeMessage(const QString&)), Qt::UniqueConnection);

    dialog.onViewChanged();

    if (dialog.exec() == QDialog::Accepted)
    {
        messageChannel_->showMessage(tr("Generation complete."));
    }
    else
    {
        messageChannel_->showMessage(tr("Generation aborted."));
    }
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::getOutputFormat()
//-----------------------------------------------------------------------------
QString VerilogGenerator::getOutputFormat() const
{
    return QStringLiteral("Verilog");
}

//-----------------------------------------------------------------------------
// Function: VerilogGenerator::runGenerator()
//-----------------------------------------------------------------------------
void VerilogGenerator::runGenerator(/*IPluginUtility* utility,*/
    QSharedPointer<Component> component,
    QSharedPointer<Design> design,
    QSharedPointer<DesignConfiguration> designConfiguration,
    QString const& viewName,
    QString const& outputDirectory)
{
    messageChannel_->showMessage(tr("Running %1 %2.").arg(getName(), getVersion()));

    messageChannel_->showMessage(tr("Running generation for %1 and view '%2'.").arg(component->getVlnv().toString(),
        viewName));

    QDir targetDirectory;
    if (!targetDirectory.mkpath(outputDirectory))
    {
        messageChannel_->showError(tr("Could not create target directory: %1").arg(outputDirectory));
        return;
    }

    messageChannel_->showMessage(tr("Target directory: %1").arg(outputDirectory));

    settings_.lastViewName_ = viewName;    

    MessagePasser messages;

    GenerationTuple input;
    input.component = component;
    input.design = design;
    input.designConfiguration = designConfiguration;
    input.messages = &messages;

    VerilogWriterFactory factory(designInterface_, libraryInterface_, &messages, &settings_, versionString_, getVersion());

    // Use configuration to drive the generation.
    GenerationControl configuration(designInterface_, libraryInterface_, &factory, input, &settings_);
    configuration.getOutputControl()->setOutputPath(outputDirectory);
    configuration.getViewSelection()->setSaveToFileset(false);

    configuration.parseDocuments();
    if (configuration.writeDocuments())
    {
        messageChannel_->showMessage(tr("Generation complete."));
    }
    else
    {
        messageChannel_->showMessage(tr("Generation failed."));
    }
}
