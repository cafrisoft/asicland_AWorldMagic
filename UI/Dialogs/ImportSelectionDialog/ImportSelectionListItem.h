/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ImportSelectionListItem.h
 *      Project: AWM
 *      Description :
 *          List item for VLNVs and files.
 */

#ifndef IMPORTSELECTIONLISTITEM_H
#define IMPORTSELECTIONLISTITEM_H

#include <IPXACTmodels/common/VLNV.h>

#include <QIcon>
#include <QListWidgetItem>

//-----------------------------------------------------------------------------
//! List item for VLNVs and files.
//-----------------------------------------------------------------------------
class ImportSelectionListItem : public QListWidgetItem
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
    ImportSelectionListItem(QIcon itemIcon, QString const& itemPath, bool isLocked, VLNV vlnv = VLNV());

	/*!
     *  The destructor.
     */
    virtual ~ImportSelectionListItem();

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
    ImportSelectionListItem::ItemType getType() const;

    /*!
     *  Check if the item is locked.
     *
     *      @return True, if the item is locked, false otherwise.
     */
    bool itemIsLocked() const;

private:

    //! No copying. No assignment.
    ImportSelectionListItem(const ImportSelectionListItem& other);
    ImportSelectionListItem& operator=(const ImportSelectionListItem& other);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! VLNV associated with the item.
    VLNV itemVlnv_;

    //! File path of the item.
    QString itemPath_;

    //! Type of the item.
    ImportSelectionListItem::ItemType type_;

    //! Lock status of the item.
    bool locked_;
};

#endif // IMPORTSELECTIONLISTITEM_H
