//-----------------------------------------------------------------------------
// File: libraryitem.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Antti Kamppi
// Date: 24.06.2010
//
// Description:
// LibraryItem class is used to create the hierarchical structure of IP library.
//-----------------------------------------------------------------------------

#include "LibraryItem.h"

#include <IPXACTmodels/common/VLNV.h>

#include <QString>
#include <QVector>
#include <QObject>

//-----------------------------------------------------------------------------
// Function: LibraryItem::LibraryItem()
//-----------------------------------------------------------------------------
LibraryItem::LibraryItem(QObject* parent): QObject(parent),
    name_(tr("root")), 
    level_(Level::ROOT),
    childItems_(),
    parentItem_(0),
    vlnv_(),
    valid_(true)
{
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::LibraryItem()
//-----------------------------------------------------------------------------
LibraryItem::LibraryItem(VLNV const& vlnv, Level level, LibraryItem* parent): 
QObject(parent), 
    name_(), 
    level_(level),
    childItems_(), 
    parentItem_(parent),
    vlnv_(),
    valid_(true)
{
	// Choose name for the item in the tree.
	if (level == Level::VENDOR)
    {
		name_ = vlnv.getVendor();
    }
    else if (level == Level::LIBRARY)
    {
		name_ = vlnv.getLibrary();
    }
	else if (level == Level::NAME)
    {
		name_ = vlnv.getName();
    }
	else if (level == Level::VERSION)
    {
		name_ = vlnv.getVersion();
		vlnv_ = vlnv;
    }

	// If the vlnv_ is null then the item has child items.
	if (!vlnv_.isValid())
    {
		// a new item can't yet have children so no need to search for one with a same name
        childItems_.push_back(new LibraryItem(vlnv, static_cast<Level>(static_cast<int>(level) + 1), this));
	}
//    childItems_.push_back(new LibraryItem(vlnv, static_cast<Level>(static_cast<int>(level) + 1), this));
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::~LibraryItem()
//-----------------------------------------------------------------------------
LibraryItem::~LibraryItem()
{
	childItems_.clear();
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::createChild()
//-----------------------------------------------------------------------------
void LibraryItem::createChild(VLNV const& vlnv, Level level)
{
    LibraryItem* existingChild = findChildForLevel(level, vlnv);
    if (existingChild)
    {
        existingChild->createChild(vlnv, static_cast<Level>(static_cast<int>(level) + 1));
        return;
    }

	// no child with same name was found so create a new child
    LibraryItem* newItem = new LibraryItem(vlnv, static_cast<Level>(static_cast<int>(level) + 1), this);
    childItems_.push_back(newItem);
	
	return;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::name()
//-----------------------------------------------------------------------------
QString LibraryItem::name() const
{
	return name_;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::parent()
//-----------------------------------------------------------------------------
LibraryItem *LibraryItem::parent() const
{
	return parentItem_;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getLevel()
//-----------------------------------------------------------------------------
LibraryItem::Level LibraryItem::getLevel() const
{
	return level_;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::child()
//-----------------------------------------------------------------------------
LibraryItem *LibraryItem::child(int index) const
{
	return childItems_[index];
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::row()
//-----------------------------------------------------------------------------
int LibraryItem::row()
{
	if (parentItem_)
    {
		return parentItem_->getIndexOf(this);
	}

	// If parent does not exist then invalid index is returned.
	return -1;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getNumberOfChildren()
//-----------------------------------------------------------------------------
int LibraryItem::getNumberOfChildren() const
{
	return childItems_.size();
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getVLNV()
//-----------------------------------------------------------------------------
VLNV LibraryItem::getVLNV() const
{
	return vlnv_;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getIndexOf()
//-----------------------------------------------------------------------------
int LibraryItem::getIndexOf(LibraryItem* item)
{
	return childItems_.indexOf(item);
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::hasChildren()
//-----------------------------------------------------------------------------
bool LibraryItem::hasChildren() const
{
	return !childItems_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getVLNVs()
//-----------------------------------------------------------------------------
QVector<VLNV> LibraryItem::getVLNVs() const
{	
    QVector<VLNV> vlnvList;

	// if this is a leaf-object
	if (vlnv_.isValid())
    {
		vlnvList.append(vlnv_);
	}
	else 
    {
		for (LibraryItem const* child : childItems_)
        {
			vlnvList += child->getVLNVs();
		}
	}

    return vlnvList;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::removeChild()
//-----------------------------------------------------------------------------
void LibraryItem::removeChild(LibraryItem* childItem )
{
	Q_ASSERT_X(childItem, "LibraryItem::removeChild()",	"Null LibraryItem-pointer");

    int index = childItems_.indexOf(childItem);

	if (index != -1)
    {
		delete childItems_.value(index);
		childItems_.removeAt(index);
	}
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::findHighestUnique()
//-----------------------------------------------------------------------------
LibraryItem* LibraryItem::findHighestUnique(LibraryItem* childItem)
{
	// if this is the root item
	if (!parentItem_)
    {
		return childItem;
    }
	else if (!childItem)
    {
		return parentItem_->findHighestUnique(this);
    }
	else if (childItems_.contains(childItem) && childItems_.size() == 1)
    {
		// this is the highest to remove unless parent item is also removed.
		return parentItem_->findHighestUnique(this);
	}

	return childItem;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::findHighestUnique()
//-----------------------------------------------------------------------------
LibraryItem* LibraryItem::findHighestUnique(VLNV const& vlnv)
{
    if (level_ == Level::VERSION)
    {
        return parentItem_->findHighestUnique(this);
    }

    LibraryItem* existingChild = findChildForLevel(level_, vlnv);
    if (existingChild)
    {
        return existingChild->findHighestUnique(vlnv);
    }

	// if there was no child that matched the vlnv then this must be the highest unique that can be used.
	return this;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::findItem()
//-----------------------------------------------------------------------------
LibraryItem* LibraryItem::findItem(VLNV const& vlnv)
{
    if (level_ == Level::VERSION)
    {
        if (vlnv_ == vlnv)
        {
            return this;
        }
        else
        {
            return 0;
        }
    }

    LibraryItem* existingChild = findChildForLevel(level_, vlnv);
    if (existingChild)
    {
        return existingChild->findItem(vlnv);
    }

	// If no match was found.
	return 0;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::setVlnv()
//-----------------------------------------------------------------------------
void LibraryItem::setVlnv(VLNV& vlnv)
{
    if (level_ == Level::ROOT)
    {
        return;
    }
    
    
    if (level_ == Level::VENDOR)
    {
        vlnv.setVendor(name_);        
    }
    else if (level_ == Level::LIBRARY)
    {
        vlnv.setLibrary(name_);        
    }
    else if (level_ == Level::NAME)
    {
        vlnv.setName(name_);        
    }
    else if (level_ == Level::VERSION) 
    {
        vlnv.setVersion(name_);
        vlnv.setType(vlnv_.getType());    
    }

    parentItem_->setVlnv(vlnv);
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::clear()
//-----------------------------------------------------------------------------
void LibraryItem::clear()
{
	qDeleteAll(childItems_);
	childItems_.clear();
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getVendors()
//-----------------------------------------------------------------------------
QVector<LibraryItem*> LibraryItem::getVendors() const
{
    if (level_ == Level::ROOT)
    {
        return childItems_;
    }
    else
    {
        Q_ASSERT_X(false, "LibraryItem::getVendors", "If came to code that should never be executed");
        return QVector<LibraryItem*>();
    }
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getLibraries()
//-----------------------------------------------------------------------------
QVector<LibraryItem*> LibraryItem::getLibraries() const
{
    if (level_ == Level::VENDOR)
    {
        return childItems_;
    }

    QVector<LibraryItem*> list;
    if (level_ == Level::ROOT)
    {
        for (LibraryItem const* item : childItems_)
        {
            list += item->getLibraries();
        }
    }
    else
    {
        Q_ASSERT_X(false, "LibraryItem::getVendors", "Switch came to code that should never be executed");
    }

    return list;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getNames()
//-----------------------------------------------------------------------------
QVector<LibraryItem*> LibraryItem::getNames() const
{
    if (level_ == Level::LIBRARY)
    {
        return childItems_;
    }

    QVector<LibraryItem*> list;

    if (level_ == Level::ROOT || level_ == Level::VENDOR)
    {
        for (LibraryItem const* item : childItems_)
        {
            list += item->getNames();
        }
    }
    else
    {
        Q_ASSERT_X(false, "LibraryItem::getVendors", "Switch came to code that should never be executed");	
    }

    return list;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::getVersions()
//-----------------------------------------------------------------------------
QVector<LibraryItem*> LibraryItem::getVersions() const
{
    if (level_ == Level::NAME)
    {
        return childItems_;
    }

	QVector<LibraryItem*> list;
    for (LibraryItem const* item : childItems_)
    {
        list += item->getNames();
    }
    return list;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::isValid()
//-----------------------------------------------------------------------------
bool LibraryItem::isValid() const
{
    return valid_;         
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::setValid()
//-----------------------------------------------------------------------------
void LibraryItem::setValid(bool valid)
{
    valid_ = valid;
}

//-----------------------------------------------------------------------------
// Function: LibraryItem::findChildForLevel()
//-----------------------------------------------------------------------------
LibraryItem* LibraryItem::findChildForLevel(Level level, VLNV const& vlnv)
{
    QString childNameToMatch;

    if (level == Level::ROOT)
    {
        childNameToMatch = vlnv.getVendor();
    }
    else if (level == Level::VENDOR)
    {
        childNameToMatch = vlnv.getLibrary();
    }
    else if (level == Level::LIBRARY)
    {
        childNameToMatch = vlnv.getName();
    }
    else if (level == Level::NAME)
    {
        childNameToMatch = vlnv.getVersion();
    }	

    for (LibraryItem* child : childItems_)
    {
        if (child->name() == childNameToMatch)
        {
            return child;
        }
    }

    return 0;
}
