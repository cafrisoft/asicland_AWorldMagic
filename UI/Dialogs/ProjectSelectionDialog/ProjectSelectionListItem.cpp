/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSelectionListItem.cpp
 *      Project: AWM
 *      Description :
 *          List item for VLNVs and files.
 */

// #include "ObjectExportListItem.h"
#include "ProjectSelectionListItem.h"

//-----------------------------------------------------------------------------
// Function: ProjectSelectionListItem::ProjectSelectionListItem()
//-----------------------------------------------------------------------------
ProjectSelectionListItem::ProjectSelectionListItem(QIcon itemIcon, QString const& itemPath, bool isLocked,
    VLNV vlnv):
QListWidgetItem(itemIcon, itemPath),
itemVlnv_(vlnv),
itemPath_(itemPath),
type_(VLNVOJBECT),
locked_(isLocked)
{
    if (!vlnv.isValid())
    {
        type_ = ProjectSelectionListItem::FILE;
    }

    setCheckState(Qt::Checked);

    setFlags(Qt::NoItemFlags);
    if (!isLocked)
    {
        setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionListItem::~ProjectSelectionListItem()
//-----------------------------------------------------------------------------
ProjectSelectionListItem::~ProjectSelectionListItem()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionListItem::getVLNV()
//-----------------------------------------------------------------------------
VLNV ProjectSelectionListItem::getVLNV() const
{
    return itemVlnv_;
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionListItem::getPath()
//-----------------------------------------------------------------------------
QString ProjectSelectionListItem::getPath() const
{
    return itemPath_;
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionListItem::getType()
//-----------------------------------------------------------------------------
ProjectSelectionListItem::ItemType ProjectSelectionListItem::getType() const
{
    return type_;
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionListItem::itemIsLocked()
//-----------------------------------------------------------------------------
bool ProjectSelectionListItem::itemIsLocked() const
{
    return locked_;
}
