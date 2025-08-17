//-----------------------------------------------------------------------------
// File: BusInterfaceWizardGeneralOptionsPage.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 25.11.2013
//
// Description:
// Bus interface wizard page for bus interface general options.
//-----------------------------------------------------------------------------

#include "BusInterfaceWizardGeneralOptionsPage.h"
#include "BusInterfaceWizard.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/validators/BusInterfaceValidator.h>
#include <IPXACTmodels/Component/validators/AbstractionTypeValidator.h>

#include <KactusAPI/BusInterfaceInterface.h>
#include <KactusAPI/AbstractionTypeInterface.h>

#include <KactusAPI/LibraryInterface.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardVLNVSelectionPage::BusInterfaceWizardVLNVSelectionPage()
//-----------------------------------------------------------------------------
BusInterfaceWizardGeneralOptionsPage::BusInterfaceWizardGeneralOptionsPage(QSharedPointer<Component> component,
    QSharedPointer<BusInterface> busIf, LibraryInterface* lh, bool absDefEditable,
    QSharedPointer<ParameterFinder> parameterFinder, QSharedPointer<ExpressionFormatter> expressionFormatter,
    QSharedPointer<ExpressionParser> expressionParser, BusInterfaceInterface* busInterface,
    BusInterfaceWizard* parent):
QWizardPage(parent),
component_(component),
busIf_(busIf),
handler_(lh),
generalTab_(new BusIfGeneralTab(lh, busIf, component, parameterFinder, expressionFormatter, expressionParser,
    busInterface, busIf->name().toStdString(), this, parent)),
busInterface_(busInterface)
{
    setTitle(tr("Bus interface general options"));
    setSubTitle(tr("Setup the general options for the bus interface."));
    setFinalPage(false);

    generalTab_->setBusTypesLock(!absDefEditable);

    connect(generalTab_, SIGNAL(contentChanged()), this, SIGNAL(completeChanged()), Qt::UniqueConnection);
    connect(generalTab_, SIGNAL(increaseReferences(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(generalTab_, SIGNAL(decreaseReferences(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);

    setupLayout();
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardVLNVSelectionPage::nextId()
//-----------------------------------------------------------------------------
int BusInterfaceWizardGeneralOptionsPage::nextId() const
{
    return BusInterfaceWizard::PAGE_BUSDEFINITION;
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardVLNVSelectionPage::initializePage()
//-----------------------------------------------------------------------------
void BusInterfaceWizardGeneralOptionsPage::initializePage()
{
    generalTab_->refresh();
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardVLNVSelectionPage::isComplete()
//-----------------------------------------------------------------------------
bool BusInterfaceWizardGeneralOptionsPage::isComplete() const
{
    if (component_->hasInterface(busIf_->name()) && component_->getBusInterface(busIf_->name()) != busIf_)
    {
        return false;
    }        

    return mandatoryFieldsAreFilledIn();
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardGeneralOptionsPage::mandatoryFieldsAreFilledIn()
//-----------------------------------------------------------------------------
bool BusInterfaceWizardGeneralOptionsPage::mandatoryFieldsAreFilledIn() const
{
    return !busIf_->name().isEmpty() &&
        busIf_->getInterfaceMode() != General::INTERFACE_MODE_COUNT &&
        handler_->contains(busIf_->getBusType()) && 
        abstractionReferenceIsFound();
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardGeneralOptionsPage::abstractionReferenceIsFound()
//-----------------------------------------------------------------------------
bool BusInterfaceWizardGeneralOptionsPage::abstractionReferenceIsFound() const
{
    AbstractionTypeInterface* abstractionInterface = busInterface_->getAbstractionTypeInterface();

    General::InterfaceMode busMode = busInterface_->getActiveMode(busIf_->name().toStdString());
    QString systemGroup = busInterface_->getActiveSystemGroup(busIf_->name().toStdString());

    abstractionInterface->setAbstractionTypes(busIf_->getAbstractionTypes(), busMode, systemGroup);

    if (abstractionInterface->itemCount() == 0)
    {
        return false;
    }

    for (int i = 0; i < abstractionInterface->itemCount(); ++i)
    {
        bool abstractionHasReference = abstractionInterface->hasAbstractionReference(i);

        if (!abstractionHasReference)
        {
            return false;
        }

        bool referenceExists = handler_->contains(*abstractionInterface->getAbstractionReference(i));
        bool viewReferencesAreValid = abstractionInterface->hasValidViewReferences(i);

        if (!referenceExists || !viewReferencesAreValid)
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardVLNVSelectionPage::setupLayout()
//-----------------------------------------------------------------------------
void BusInterfaceWizardGeneralOptionsPage::setupLayout()
{
    QVBoxLayout* topLayout = new QVBoxLayout(this);    
    topLayout->addWidget(generalTab_);
}
