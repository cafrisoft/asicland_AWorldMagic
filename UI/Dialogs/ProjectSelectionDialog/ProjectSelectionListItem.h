/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSelectionListItem.h
 *      Project: AWM
 *      Description :
 *          List item for VLNVs and files.
 */

#ifndef PROJECTSELECTIONLISTITEM_H
#define PROJECTSELECTIONLISTITEM_H

#include <IPXACTmodels/common/VLNV.h>

#include <QIcon>
#include <QListWidgetItem>

//-----------------------------------------------------------------------------
//! List item for VLNVs and files.
//-----------------------------------------------------------------------------
class ProjectSelectionListItem : public QListWidgetItem
{

public:

    //! Type of the item.
    enum ItemType
    {
        VLNVOJBECT = 0,     // VLNV object item.
        FILE                // file item.
    };

	/*!
     *  The constructor.
	 *
     *      @param [in] itemIcon    Icon used by the item.
     *      @param [in] itemPath    File path of the item.
     *      @param [in] isLocked    The lock status of the item.
     *      @param [in] vlnv        VLNV associated with the item.
	 *
     */
    ProjectSelectionListItem(QIcon itemIcon, QString const& itemPath, bool isLocked, VLNV vlnv = VLNV());

	/*!
     *  The destructor.
     */
    virtual ~ProjectSelectionListItem();

    /*!
     *  Get the VLNV associated with the item.
     *
     *      @return The VLNV of the item.
     */
    VLNV getVLNV() const;

    /*!
     *  Get the file path of the item.
     *
     *      @return The file path of the item.
     */
    QString getPath() const;

    /*!
     *  Get the item type.
     *
     *      @return The type of the item.
     */
    ProjectSelectionListItem::ItemType getType() const;

    /*!
     *  Check if the item is locked.
     *
     *      @return True, if the item is locked, false otherwise.
     */
    bool itemIsLocked() const;

private:

    //! No copying. No assignment.
    ProjectSelectionListItem(const ProjectSelectionListItem& other);
    ProjectSelectionListItem& operator=(const ProjectSelectionListItem& other);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! VLNV associated with the item.
    VLNV itemVlnv_;

    //! File path of the item.
    QString itemPath_;

    //! Type of the item.
    ProjectSelectionListItem::ItemType type_;

    //! Lock status of the item.
    bool locked_;
};

#endif // PROJECTSELECTIONLISTITEM_H
