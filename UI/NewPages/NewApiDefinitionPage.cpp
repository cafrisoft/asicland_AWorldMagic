//-----------------------------------------------------------------------------
// File: NewApiDefinitionPage.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti Maatta
// Date: 17.4.2012
//
// Description:
// New API definition page.
//-----------------------------------------------------------------------------

#include "NewApiDefinitionPage.h"

#include <KactusAPI/LibraryInterface.h>

#include <UI/Widgets/vlnvEditor/vlnveditor.h>

#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: NewApiDefinitionPage()
//-----------------------------------------------------------------------------
NewApiDefinitionPage::NewApiDefinitionPage(LibraryInterface* libInterface, QWidget* parentDlg):
NewPage(libInterface, VLNV::APIDEFINITION, tr("New API definition"), tr("Creates an API definition"), parentDlg)
{
    QVBoxLayout* topLayout = dynamic_cast<QVBoxLayout*>(layout());
    topLayout->addStretch(1);
}

//-----------------------------------------------------------------------------
// Function: ~NewApiDefinitionPage()
//-----------------------------------------------------------------------------
NewApiDefinitionPage::~NewApiDefinitionPage()
{

}

//-----------------------------------------------------------------------------
// Function: validate()
//-----------------------------------------------------------------------------
bool NewApiDefinitionPage::validate()
{
    // Check if the VLNV already exists.    
    bool valid = NewPage::validate();    

    if (!valid)
    {
        showErrorForReservedVLVN(vlnvEditor_->getVLNV());
    }

    return valid;
}

//-----------------------------------------------------------------------------
// Function: apply()
//-----------------------------------------------------------------------------
void NewApiDefinitionPage::apply()
{
    emit createApiDefinition(vlnvEditor_->getVLNV(), selectedPath());
}
