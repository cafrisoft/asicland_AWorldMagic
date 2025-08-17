//-----------------------------------------------------------------------------
// File: ComponentWizard.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti Maatta
// Date: 06.06.2013
//
// Description:
// Component wizard dialog.
//-----------------------------------------------------------------------------

#include "ComponentWizard.h"
#include "ComponentWizardPages.h"

#include "ComponentWizardIntroPage.h"
#include "ComponentWizardDependencyPage.h"
#include "ComponentWizardGeneralInfoPage.h"
#include "ComponentWizardImportPage.h"
#include "ComponentWizardViewsPage.h"
#include "ComponentWizardConclusionPage.h"

#include <KactusAPI/BusInterfaceInterface.h>
#include <KactusAPI/BusInterfaceInterfaceFactory.h>
#include <KactusAPI/IPXactSystemVerilogParser.h>
#include <KactusAPI/FileSetInterface.h>
#include <KactusAPI/FileInterface.h>
#include <KactusAPI/FileBuilderInterface.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/validators/FileValidator.h>
#include <IPXACTmodels/Component/validators/FileSetValidator.h>

#include <UI/ComponentEditor/referenceCounter/ParameterReferenceCounter.h>

#include <QLabel>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: ComponentWizard::ComponentWizard()
//-----------------------------------------------------------------------------
ComponentWizard::ComponentWizard(QSharedPointer<Component> component,
                                 QString const& basePath,
                                 LibraryInterface* designInterface,
                                 LibraryInterface* libraryInterface,
                                 QWidget* parent):
QWizard(parent),
originalComponent_(component),
workingComponent_(component),
parameterFinder_(new ComponentParameterFinder(workingComponent_)),
expressionFormatter_(new ExpressionFormatter(parameterFinder_)),
referenceCounter_(new ParameterReferenceCounter(parameterFinder_))
{
    setPixmap(QWizard::LogoPixmap, QPixmap(":resources/Res/hw-component.png"));
	setWindowTitle(tr("Component Wizard for %1").arg(component->getVlnv().toString()));
    setWizardStyle(ModernStyle);
    resize(800, 1000);

    setOption(NoBackButtonOnStartPage, true);
    setOption(NoCancelButton, true);
    setOption(NoDefaultButton, true);
    setOption(HaveFinishButtonOnEarlyPages, true);

    QSharedPointer<IPXactSystemVerilogParser> parser(new IPXactSystemVerilogParser(parameterFinder_));

    BusInterfaceInterface* busInterface = BusInterfaceInterfaceFactory::createBusInterface(
        parameterFinder_, expressionFormatter_, parser, component, libraryInterface);

    ComponentWizardImportPage* importPage = new ComponentWizardImportPage(originalComponent_,
                                                                          designInterface,
                                                                          libraryInterface,
                                                                          parameterFinder_,
                                                                          expressionFormatter_,
                                                                          busInterface, this);

    ComponentWizardConclusionPage* conclusionPage = new ComponentWizardConclusionPage(originalComponent_, libraryInterface,
        expressionFormatter_, this);

    FileSetInterface* fileSetInterface = createFileSetInterface(parser);

    setPage(ComponentWizardPages::INTRO, new ComponentWizardIntroPage(originalComponent_, this));
    setPage(ComponentWizardPages::GENERAL, new ComponentWizardGeneralInfoPage(originalComponent_, this));
    setPage(ComponentWizardPages::FILE_DEPENDENCY, new ComponentWizardDependencyPage(
        originalComponent_, parameterFinder_, basePath, fileSetInterface, this));
    setPage(ComponentWizardPages::IMPORT, importPage);
    setPage(ComponentWizardPages::VIEWS, new ComponentWizardViewsPage(
        libraryInterface, parameterFinder_, expressionFormatter_, this));
    setPage(ComponentWizardPages::CONCLUSION, conclusionPage);

    connect(importPage, SIGNAL(componentChanged(QSharedPointer<Component>)), 
        this, SLOT(onComponentChanged(QSharedPointer<Component>)), Qt::UniqueConnection);

    connect(importPage, SIGNAL(componentChanged(QSharedPointer<Component>)),
        conclusionPage, SLOT(onComponentChanged(QSharedPointer<Component>)), Qt::UniqueConnection);

    connect(importPage, SIGNAL(increaseReferences(QString)),
        referenceCounter_.data(), SLOT(increaseReferenceCount(QString)), Qt::UniqueConnection);
    connect(importPage, SIGNAL(decreaseReferences(QString)),
        referenceCounter_.data(), SLOT(decreaseReferenceCount(QString)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ComponentWizard::~ComponentWizard()
//-----------------------------------------------------------------------------
ComponentWizard::~ComponentWizard()
{

}

//-----------------------------------------------------------------------------
// Function: ComponentWizard::getComponent()
//-----------------------------------------------------------------------------
QSharedPointer<Component> ComponentWizard::getComponent()
{
    return workingComponent_;
}

//-----------------------------------------------------------------------------
// Function: ComponentWizard::cleanupPage()
//-----------------------------------------------------------------------------
void ComponentWizard::cleanupPage(int id)
{
    if (id == ComponentWizardPages::IMPORT)
    {
        workingComponent_ = originalComponent_;

        parameterFinder_->setComponent(workingComponent_);
    }

    QWizard::cleanupPage(id);
}

//-----------------------------------------------------------------------------
// Function: ComponentWizard::onComponentChanged()
//-----------------------------------------------------------------------------
void ComponentWizard::onComponentChanged(QSharedPointer<Component> component)
{
    workingComponent_ = component;

    parameterFinder_->setComponent(workingComponent_);
}

//-----------------------------------------------------------------------------
// Function: ComponentWizard::createFileSetInterface()
//-----------------------------------------------------------------------------
FileSetInterface* ComponentWizard::createFileSetInterface(QSharedPointer<ExpressionParser> parser)
{
    QSharedPointer<FileValidator> fileValidator(new FileValidator(parser));
    QSharedPointer<FileSetValidator> fileSetValidator(new FileSetValidator(fileValidator, parser));

    FileInterface* newFileInterface = new FileInterface(fileValidator, parser, expressionFormatter_);
    FileBuilderInterface* newBuilderInterface = new FileBuilderInterface(parser, expressionFormatter_);

    FileSetInterface* newFileSetInterface = new FileSetInterface(
        fileSetValidator, parser, expressionFormatter_, newFileInterface, newBuilderInterface);

    return newFileSetInterface;
}
