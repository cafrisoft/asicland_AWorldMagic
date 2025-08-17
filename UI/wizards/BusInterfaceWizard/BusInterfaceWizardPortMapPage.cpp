//-----------------------------------------------------------------------------
// File: BusInterfaceWizardPortMapPage.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 25.11.2013
//
// Description:
// Bus interface wizard page for defining port maps.
//-----------------------------------------------------------------------------

#include "BusInterfaceWizardPortMapPage.h"

#include "BusInterfaceWizard.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/FileSet.h>
#include <IPXACTmodels/Component/validators/BusInterfaceValidator.h>
#include <IPXACTmodels/Component/validators/AbstractionTypeValidator.h>

#include <KactusAPI/BusInterfaceInterface.h>
#include <KactusAPI/AbstractionTypeInterface.h>
#include <KactusAPI/BusInterfaceInterface.h>

#include <KactusAPI/LibraryInterface.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::BusInterfaceWizardPortMapPage()
//-----------------------------------------------------------------------------
BusInterfaceWizardPortMapPage::BusInterfaceWizardPortMapPage(QSharedPointer<Component> component,
    QSharedPointer<BusInterface> busIf, LibraryInterface* lh, QStringList physicalPorts,
    QSharedPointer<ExpressionParser> expressionParser, QSharedPointer<ParameterFinder> parameterFinder,
    BusInterfaceInterface* busInterface, PortMapInterface* portMapInterface, BusInterfaceWizard* parent):
QWizardPage(parent),
busIf_(busIf),
handler_(lh),
portMapTab_(handler_, component, busInterface, busIf_->name().toStdString(), expressionParser, parameterFinder,
    portMapInterface, this),
busInterface_(busInterface)
{
    setTitle(tr("Port Maps"));
    setSubTitle(tr("Create port maps for interface %1.").arg(busIf->name()));
    setFinalPage(false);
    
    portMapTab_.setPhysicalPorts(physicalPorts);

    connect(&portMapTab_, SIGNAL(errorMessage(QString const&)),
        this, SLOT(showErrorMessage(QString const&)), Qt::UniqueConnection);    
    connect(&portMapTab_, SIGNAL(contentChanged()), this, SIGNAL(completeChanged()), Qt::UniqueConnection);

    setupLayout();
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::~BusInterfaceWizardPortMapPage()
//-----------------------------------------------------------------------------
BusInterfaceWizardPortMapPage::~BusInterfaceWizardPortMapPage()
{

}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::nextId()
//-----------------------------------------------------------------------------
int BusInterfaceWizardPortMapPage::nextId() const
{
    return BusInterfaceWizard::PAGE_SUMMARY;
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::initializePage()
//-----------------------------------------------------------------------------
void BusInterfaceWizardPortMapPage::initializePage()
{
    portMapTab_.setAbstractionDefinitions();
    portMapTab_.refresh();    
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::isComplete()
//-----------------------------------------------------------------------------
bool BusInterfaceWizardPortMapPage::isComplete() const
{
    return busInterface_->validateBusInterface(busIf_->name().toStdString());
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::showErrorMessage()
//-----------------------------------------------------------------------------
void BusInterfaceWizardPortMapPage::showErrorMessage(QString const& msg)
{
    QMessageBox warningDialog(QMessageBox::Warning, tr("Warning"), msg, QMessageBox::Ok, this);
    warningDialog.exec();
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceWizardPortMapPage::setupLayout()
//-----------------------------------------------------------------------------
void BusInterfaceWizardPortMapPage::setupLayout()
{
    QVBoxLayout* topLayout = new QVBoxLayout(this);
    topLayout->addWidget(&portMapTab_);
}
