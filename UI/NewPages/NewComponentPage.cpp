//-----------------------------------------------------------------------------
// File: NewComponentPage.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti Maatta
// Date: 9.6.2011
//
// Description:
// New component property page.
//-----------------------------------------------------------------------------

#include "NewComponentPage.h"

#include <IPXACTmodels/common/VLNV.h>
#include <KactusAPI/LibraryInterface.h>

#include <UI/Widgets/vlnvEditor/vlnveditor.h>
#include <UI/Widgets/kactusAttributeEditor/KactusAttributeEditor.h>

#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: NewComponentPage()
//-----------------------------------------------------------------------------
NewComponentPage::NewComponentPage(LibraryInterface* libInterface, QWidget* parentDlg):
NewPage(libInterface, VLNV::COMPONENT, tr("New HW Component"),
    tr("Creates a flat (non-hierarchical) HW component"), parentDlg),
attributeEditor_(new KactusAttributeEditor(this))
{
    connect(attributeEditor_, SIGNAL(productHierarchyChanged()), this, SLOT(onProductHierarchyChanged()));

    vlnvEditor_->setImplementationFilter(true, KactusAttribute::HW);

    // Setup the layout.
    QVBoxLayout* widgetLayout = dynamic_cast<QVBoxLayout*>(layout());
    Q_ASSERT(widgetLayout);
    widgetLayout->addSpacing(12);
    widgetLayout->insertWidget(3, attributeEditor_);
    widgetLayout->addStretch(1);

    onProductHierarchyChanged();
}

//-----------------------------------------------------------------------------
// Function: ~NewComponentPage()
//-----------------------------------------------------------------------------
NewComponentPage::~NewComponentPage()
{

}

//-----------------------------------------------------------------------------
// Function: validate()
//-----------------------------------------------------------------------------
bool NewComponentPage::validate()
{
    bool valid = NewPage::validate();

    // Check if the VLNV already exists.
    if (!valid)
    {        
        showErrorForReservedVLVN(vlnvEditor_->getVLNV());     
    }

    return valid;
}

//-----------------------------------------------------------------------------
// Function: apply()
//-----------------------------------------------------------------------------
void NewComponentPage::apply()
{
    emit createComponent(attributeEditor_->getProductHierarchy(), attributeEditor_->getFirmness(),
        attributeEditor_->getTags(), vlnvEditor_->getVLNV(), selectedPath());
}

//-----------------------------------------------------------------------------
// Function: onPageChange()
//-----------------------------------------------------------------------------
bool NewComponentPage::onPageChange()
{
    // Discard the VLNV and reset the attributes.
#if PRODUCTHIERARCHY_IP_ONLY_20231229 == 0
    attributeEditor_->setAttributes(KactusAttribute::FLAT, KactusAttribute::TEMPLATE, QVector<TagData>());
#else
    attributeEditor_->setAttributes(KactusAttribute::IP, KactusAttribute::TEMPLATE, QVector<TagData>());
#endif
    onProductHierarchyChanged();
    return NewPage::onPageChange();
}

//-----------------------------------------------------------------------------
// Function: onProductHierarchyChanged()
//-----------------------------------------------------------------------------
void NewComponentPage::onProductHierarchyChanged()
{
    // Update the VLNV's library field if it is either empty or any of the predefined ones.
    VLNV vlnv = vlnvEditor_->getVLNV();
    
    if (vlnv.getLibrary().isEmpty())
    {
        vlnv.setLibrary(KactusAttribute::hierarchyToString(attributeEditor_->getProductHierarchy()).toLower());
    }
    else
    {
        QString previousLibrary = vlnv.getLibrary().toLower();
        for (unsigned int i = 0; i < KactusAttribute::KTS_PRODHIER_COUNT; ++i)
        {
            if (previousLibrary ==
                KactusAttribute::hierarchyToString(static_cast<KactusAttribute::ProductHierarchy>(i)).toLower())
            {
                vlnv.setLibrary(KactusAttribute::hierarchyToString(attributeEditor_->getProductHierarchy()).toLower());
                break;
            }
        }
    }

    vlnvEditor_->setVLNV(vlnv);
}
