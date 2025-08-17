//-----------------------------------------------------------------------------
// File: vieweditor.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 14.4.2011
//
// Description:
// Editor to edit a view within a component.
//-----------------------------------------------------------------------------

#include "vieweditor.h"

#include "envidentifiereditor.h"

#include <KactusAPI/LibraryInterface.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/ComponentInstantiation.h>
#include <IPXACTmodels/Component/View.h>
#include <IPXACTmodels/common/FileBuilder.h>

#include <UI/Common/NameGenerationPolicy.h>

#include <UI/Widgets/nameGroupEditor/namegroupeditor.h>
#include <UI/Widgets/vlnvDisplayer/vlnvdisplayer.h>

#include <UI/ComponentEditor/common/ReferenceSelector/ReferenceSelector.h>
#include <UI/ComponentEditor/instantiations/ComponentInstantiationDisplayer.h>
#include <UI/ComponentEditor/instantiations/ModuleParameterEditor.h>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QScrollArea>

//-----------------------------------------------------------------------------
// Function: ViewEditor::ViewEditor()
//-----------------------------------------------------------------------------
ViewEditor::ViewEditor(QSharedPointer<Component> component, QSharedPointer<View> view,
    LibraryInterface* libHandler, QSharedPointer<ParameterFinder> parameterFinder,
    QSharedPointer<ExpressionFormatter> expressionFormatter, ModuleParameterInterface* parameterInterface,
    QWidget *parent) :
ItemEditor(component, libHandler, parent),
view_(view),
nameEditor_(new NameGroupEditor(view, this, tr("View name and description"))),
envIdentifier_(new EnvIdentifierEditor(view, this)),
componentInstantiationSelector_(new ReferenceSelector(this)),
designConfigurationInstantiationSelector_(new ReferenceSelector(this)),
designInstantiationSelector_(new ReferenceSelector(this)),
componentInstantiationDisplay_(new ComponentInstantiationDisplayer(this)),
hierarchyGroup_(new QGroupBox(tr("Design and configuration"), this)),
designConfigurationDisplay_(new VLNVDisplayer(this, VLNV())),
designDisplay_(new VLNVDisplayer(this, VLNV())),
moduleParameterEditor_(
    new ModuleParameterEditor(QSharedPointer<ComponentInstantiation>(new ComponentInstantiation()),
        component->getChoices(), parameterFinder, expressionFormatter, parameterInterface, this))
{
    moduleParameterEditor_->disableEditing();

    designConfigurationDisplay_->setTitle(tr("Design configuration"));
    designDisplay_->setTitle(tr("Design"));

    setupLayout();

    refresh();

    connect(nameEditor_, SIGNAL(contentChanged()),	this, SIGNAL(contentChanged()), Qt::UniqueConnection);

    connect(envIdentifier_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);

    connect(componentInstantiationSelector_, SIGNAL(itemSelected(QString const&)),
        this, SLOT(onComponentInstanceChanged(QString const&)), Qt::UniqueConnection);
    connect(designConfigurationInstantiationSelector_, SIGNAL(itemSelected(QString const&)),
        this, SLOT(onDesignConfigurationInstanceChanged(QString const&)), Qt::UniqueConnection);
    connect(designInstantiationSelector_, SIGNAL(itemSelected(QString const&)),
        this, SLOT(onDesignInstanceChanged(QString const&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ViewEditor::refresh()
//-----------------------------------------------------------------------------
void ViewEditor::refresh()
{
	nameEditor_->refresh();
	envIdentifier_->refresh();

    QStringList componentInstantiationNames;
    foreach(QSharedPointer<ComponentInstantiation> componentInstantiation, 
        *component()->getComponentInstantiations())
    {
        componentInstantiationNames.append(componentInstantiation->name());
    }
    componentInstantiationSelector_->refresh(componentInstantiationNames);
    componentInstantiationSelector_->selectItem(view_->getComponentInstantiationRef());
    onComponentInstanceChanged(view_->getComponentInstantiationRef());

    QStringList designConfigurationInstantiationNames;
    foreach(QSharedPointer<DesignConfigurationInstantiation> designConfigurationInstantiation, 
        *component()->getDesignConfigurationInstantiations())
    {
        designConfigurationInstantiationNames.append(designConfigurationInstantiation->name());
        if (designConfigurationInstantiation->name() == view_->getDesignConfigurationInstantiationRef())
        {
            designConfigurationDisplay_->setVLNV(
                *designConfigurationInstantiation->getDesignConfigurationReference());
        }
    }
    designConfigurationInstantiationSelector_->refresh(designConfigurationInstantiationNames);
    designConfigurationInstantiationSelector_->selectItem(view_->getDesignConfigurationInstantiationRef());
    onDesignConfigurationInstanceChanged(view_->getDesignConfigurationInstantiationRef());

    QStringList designInstantiationNames;
    foreach(QSharedPointer<DesignInstantiation> designInstantiation, *component()->getDesignInstantiations())
    {
        designInstantiationNames.append(designInstantiation->name());
        if (designInstantiation->name() == view_->getDesignInstantiationRef())
        {
            designDisplay_->setVLNV(*designInstantiation->getDesignReference());
        }
    }
    designInstantiationSelector_->refresh(designInstantiationNames);
    designInstantiationSelector_->selectItem(view_->getDesignInstantiationRef());
    onDesignInstanceChanged(view_->getDesignInstantiationRef()); 
}

//-----------------------------------------------------------------------------
// Function: ViewEditor::showEvent()
//-----------------------------------------------------------------------------
void ViewEditor::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    emit helpUrlRequested("componenteditor/view.html");
    refresh();
}

//-----------------------------------------------------------------------------
// Function: ViewEditor::onComponentInstanceChanged()
//-----------------------------------------------------------------------------
void ViewEditor::onComponentInstanceChanged(QString const& instanceName)
{
    QString previousInstance = view_->getComponentInstantiationRef();

    view_->setComponentInstantiationRef(instanceName);

    QSharedPointer<ComponentInstantiation> instantiation = 
        component()->getModel()->findComponentInstantiation(instanceName);
    componentInstantiationDisplay_->setInstantiation(instantiation);

    if (instantiation)
    {
        moduleParameterEditor_->setModuleParameters(instantiation);
    }
    moduleParameterEditor_->setVisible(instantiation.isNull() == false);

    if (previousInstance != instanceName)
    {
        emit contentChanged();
    }
}

//-----------------------------------------------------------------------------
// Function: ViewEditor::onDesignConfigurationInstanceChanged()
//-----------------------------------------------------------------------------
void ViewEditor::onDesignConfigurationInstanceChanged(QString const& instanceName)
{    
    QString previousInstance = view_->getDesignConfigurationInstantiationRef();

    view_->setDesignConfigurationInstantiationRef(instanceName);

    QSharedPointer<DesignConfigurationInstantiation> selectedInstantiation;

    foreach(QSharedPointer<DesignConfigurationInstantiation> configurationInstantiation, 
        *component()->getDesignConfigurationInstantiations())
    {
        if (configurationInstantiation->name() == instanceName)
        {
            selectedInstantiation = configurationInstantiation;
        }
    }

    VLNV selectedVLNV;
    if (selectedInstantiation)
    {
        selectedVLNV = *selectedInstantiation->getDesignConfigurationReference();
    }

    hierarchyGroup_->setVisible(!instanceName.isEmpty() || !designInstantiationSelector_->currentText().isEmpty());
    designConfigurationDisplay_->setVisible(selectedInstantiation.isNull() == false);
    designConfigurationDisplay_->setVLNV(selectedVLNV);

    if (previousInstance != instanceName)
    {
        emit contentChanged();
    }    
}

//-----------------------------------------------------------------------------
// Function: ViewEditor::onDesignInstanceChanged()
//-----------------------------------------------------------------------------
void ViewEditor::onDesignInstanceChanged(QString const& instanceName)
{    
    QString previousInstance = view_->getDesignInstantiationRef();

    view_->setDesignInstantiationRef(instanceName);

    QSharedPointer<DesignInstantiation> selectedInstantiation;

    foreach(QSharedPointer<DesignInstantiation> designInstantiation, *component()->getDesignInstantiations())
    {
        if (designInstantiation->name() == instanceName)
        {
            selectedInstantiation = designInstantiation;
        }
    }

    VLNV selectedVLNV;
    if (selectedInstantiation)
    {
        selectedVLNV = *selectedInstantiation->getDesignReference();
    }

    hierarchyGroup_->setVisible(!instanceName.isEmpty() || 
        !designConfigurationInstantiationSelector_->currentText().isEmpty());
    designDisplay_->setVisible(selectedInstantiation.isNull() == false);
    designDisplay_->setVLNV(selectedVLNV);

    if (previousInstance != instanceName)
    {
        emit contentChanged();
    }    
}

//-----------------------------------------------------------------------------
// Function: ViewEditor::setupLayout()
//-----------------------------------------------------------------------------
void ViewEditor::setupLayout()
{
    // create the scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QHBoxLayout* scrollLayout = new QHBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    scrollLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox* instantiationsGroup = new QGroupBox(tr("Instantiations"), this);
    QFormLayout* instancesLayout = new QFormLayout(instantiationsGroup);
    instancesLayout->addRow(tr("Component instantiation:"), componentInstantiationSelector_);
    instancesLayout->addRow(tr("Design configuration instantiation:"), designConfigurationInstantiationSelector_);
    instancesLayout->addRow(tr("Design instantiation:"), designInstantiationSelector_);

    QVBoxLayout* editorsLayout = new QVBoxLayout(hierarchyGroup_);
    editorsLayout->addWidget(designConfigurationDisplay_, 0);
    editorsLayout->addWidget(designDisplay_, 0);
    editorsLayout->addStretch();

    // create the top widget and set it under the scroll area
    QWidget* topWidget = new QWidget(scrollArea);

    // create the layout for the top widget
    QGridLayout* topLayout = new QGridLayout(topWidget);
    topLayout->addWidget(nameEditor_, 0, 0, 1, 1);
    topLayout->addWidget(componentInstantiationDisplay_, 0, 1, 1, 1); 
    topLayout->addWidget(instantiationsGroup, 1, 0, 1, 1);
    topLayout->addWidget(hierarchyGroup_, 1, 1, 2, 1);
    
    topLayout->addWidget(envIdentifier_, 2, 0, 1, 1);
    topLayout->addWidget(moduleParameterEditor_, 3, 0, 1, 2);
    topLayout->setRowStretch(0, 1);
    topLayout->setRowStretch(1, 1);
    topLayout->setRowStretch(2, 1);
    topLayout->setRowStretch(3, 5);
    topLayout->setColumnStretch(0, 1);
    topLayout->setColumnStretch(1, 1);
    topLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea->setWidget(topWidget);
}
