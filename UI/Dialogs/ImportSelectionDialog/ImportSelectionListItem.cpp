/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ImportSelectionListItem.cpp
 *      Project: AWM
 *      Description :
 *          List item for VLNVs and files.
 */


#include "ImportSelectionListItem.h"

//-----------------------------------------------------------------------------
// Function: ImportSelectionListItem::ImportSelectionListItem()
//-----------------------------------------------------------------------------
ImportSelectionListItem::ImportSelectionListItem(QIcon itemIcon, QString const& itemPath, bool isLocked,
    VLNV vlnv):
QListWidgetItem(itemIcon, itemPath),
itemVlnv_(vlnv),
itemPath_(itemPath),
type_(VLNVOJBECT),
locked_(isLocked)
{
    if (!vlnv.isValid())
    {
        type_ = ImportSelectionListItem::FILE;
    }

    setCheckState(Qt::Checked);

    setFlags(Qt::NoItemFlags);
    if (!isLocked)
    {
        setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionListItem::~ImportSelectionListItem()
//-----------------------------------------------------------------------------
ImportSelectionListItem::~ImportSelectionListItem()
{

}

//-----------------------------------------------------------------------------
// Function: ImportSelectionListItem::getVLNV()
//-----------------------------------------------------------------------------
VLNV ImportSelectionListItem::getVLNV() const
{
    return itemVlnv_;
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionListItem::getPath()
//-----------------------------------------------------------------------------
QString ImportSelectionListItem::getPath() const
{
    return itemPath_;
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionListItem::getType()
//-----------------------------------------------------------------------------
ImportSelectionListItem::ItemType ImportSelectionListItem::getType() const
{
    return type_;
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionListItem::itemIsLocked()
//-----------------------------------------------------------------------------
bool ImportSelectionListItem::itemIsLocked() const
{
    return locked_;
}
