//-----------------------------------------------------------------------------
// File: ProjectItem.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 04.06.2024
//
// Description:
// ProjectItem class is used to create the hierarchical structure of IP library.
//-----------------------------------------------------------------------------

#include "ProjectItem.h"

#include <IPXACTmodels/common/VLNV.h>

#include <QString>
#include <QVector>
#include <QObject>

//-----------------------------------------------------------------------------
// Function: ProjectItem::ProjectItem()
//-----------------------------------------------------------------------------
ProjectItem::ProjectItem(QObject* parent): QObject(parent),
    name_(tr("root")), 
    level_(Level::ROOT),
    childItems_(),
    parentItem_(0),
    vlnv_(),
    valid_(true)
{
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::ProjectItem()
//-----------------------------------------------------------------------------
ProjectItem::ProjectItem(VLNV const& vlnv, Level level, ProjectItem* parent):
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
        childItems_.push_back(new ProjectItem(vlnv, static_cast<Level>(static_cast<int>(level) + 1), this));
	}
//    childItems_.push_back(new ProjectItem(vlnv, static_cast<Level>(static_cast<int>(level) + 1), this));
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::~ProjectItem()
//-----------------------------------------------------------------------------
ProjectItem::~ProjectItem()
{
	childItems_.clear();
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::createChild()
//-----------------------------------------------------------------------------
void ProjectItem::createChild(VLNV const& vlnv, Level level)
{
    ProjectItem* existingChild = findChildForLevel(level, vlnv);
    if (existingChild)
    {
        existingChild->createChild(vlnv, static_cast<Level>(static_cast<int>(level) + 1));
        return;
    }

	// no child with same name was found so create a new child
    ProjectItem* newItem = new ProjectItem(vlnv, static_cast<Level>(static_cast<int>(level) + 1), this);
    childItems_.push_back(newItem);
	
	return;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::name()
//-----------------------------------------------------------------------------
QString ProjectItem::name() const
{
	return name_;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::parent()
//-----------------------------------------------------------------------------
ProjectItem *ProjectItem::parent() const
{
	return parentItem_;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getLevel()
//-----------------------------------------------------------------------------
ProjectItem::Level ProjectItem::getLevel() const
{
	return level_;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::child()
//-----------------------------------------------------------------------------
ProjectItem *ProjectItem::child(int index) const
{
	return childItems_[index];
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::row()
//-----------------------------------------------------------------------------
int ProjectItem::row()
{
	if (parentItem_)
    {
		return parentItem_->getIndexOf(this);
	}

	// If parent does not exist then invalid index is returned.
	return -1;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getNumberOfChildren()
//-----------------------------------------------------------------------------
int ProjectItem::getNumberOfChildren() const
{
	return childItems_.size();
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getVLNV()
//-----------------------------------------------------------------------------
VLNV ProjectItem::getVLNV() const
{
	return vlnv_;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getIndexOf()
//-----------------------------------------------------------------------------
int ProjectItem::getIndexOf(ProjectItem* item)
{
	return childItems_.indexOf(item);
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::hasChildren()
//-----------------------------------------------------------------------------
bool ProjectItem::hasChildren() const
{
	return !childItems_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getVLNVs()
//-----------------------------------------------------------------------------
QVector<VLNV> ProjectItem::getVLNVs() const
{	
    QVector<VLNV> vlnvList;

	// if this is a leaf-object
	if (vlnv_.isValid())
    {
		vlnvList.append(vlnv_);
	}
	else 
    {
                for (ProjectItem const* child : childItems_)
        {
			vlnvList += child->getVLNVs();
		}
	}

    return vlnvList;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::removeChild()
//-----------------------------------------------------------------------------
void ProjectItem::removeChild(ProjectItem* childItem )
{
        Q_ASSERT_X(childItem, "ProjectItem::removeChild()",	"Null ProjectItem-pointer");

    int index = childItems_.indexOf(childItem);

	if (index != -1)
    {
		delete childItems_.value(index);
		childItems_.removeAt(index);
	}
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::findHighestUnique()
//-----------------------------------------------------------------------------
ProjectItem* ProjectItem::findHighestUnique(ProjectItem* childItem)
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
// Function: ProjectItem::findHighestUnique()
//-----------------------------------------------------------------------------
ProjectItem* ProjectItem::findHighestUnique(VLNV const& vlnv)
{
    if (level_ == Level::VERSION)
    {
        return parentItem_->findHighestUnique(this);
    }

    ProjectItem* existingChild = findChildForLevel(level_, vlnv);
    if (existingChild)
    {
        return existingChild->findHighestUnique(vlnv);
    }

	// if there was no child that matched the vlnv then this must be the highest unique that can be used.
	return this;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::findItem()
//-----------------------------------------------------------------------------
ProjectItem* ProjectItem::findItem(VLNV const& vlnv)
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

    ProjectItem* existingChild = findChildForLevel(level_, vlnv);
    if (existingChild)
    {
        return existingChild->findItem(vlnv);
    }

	// If no match was found.
	return 0;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::setVlnv()
//-----------------------------------------------------------------------------
void ProjectItem::setVlnv(VLNV& vlnv)
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
// Function: ProjectItem::clear()
//-----------------------------------------------------------------------------
void ProjectItem::clear()
{
	qDeleteAll(childItems_);
	childItems_.clear();
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getVendors()
//-----------------------------------------------------------------------------
QVector<ProjectItem*> ProjectItem::getVendors() const
{
    if (level_ == Level::ROOT)
    {
        return childItems_;
    }
    else
    {
        Q_ASSERT_X(false, "ProjectItem::getVendors", "If came to code that should never be executed");
        return QVector<ProjectItem*>();
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getLibraries()
//-----------------------------------------------------------------------------
QVector<ProjectItem*> ProjectItem::getLibraries() const
{
    if (level_ == Level::VENDOR)
    {
        return childItems_;
    }

    QVector<ProjectItem*> list;
    if (level_ == Level::ROOT)
    {
        for (ProjectItem const* item : childItems_)
        {
            list += item->getLibraries();
        }
    }
    else
    {
        Q_ASSERT_X(false, "ProjectItem::getVendors", "Switch came to code that should never be executed");
    }

    return list;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getNames()
//-----------------------------------------------------------------------------
QVector<ProjectItem*> ProjectItem::getNames() const
{
    if (level_ == Level::LIBRARY)
    {
        return childItems_;
    }

    QVector<ProjectItem*> list;

    if (level_ == Level::ROOT || level_ == Level::VENDOR)
    {
        for (ProjectItem const* item : childItems_)
        {
            list += item->getNames();
        }
    }
    else
    {
        Q_ASSERT_X(false, "ProjectItem::getVendors", "Switch came to code that should never be executed");
    }

    return list;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::getVersions()
//-----------------------------------------------------------------------------
QVector<ProjectItem*> ProjectItem::getVersions() const
{
    if (level_ == Level::NAME)
    {
        return childItems_;
    }

        QVector<ProjectItem*> list;
    for (ProjectItem const* item : childItems_)
    {
        list += item->getNames();
    }
    return list;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::isValid()
//-----------------------------------------------------------------------------
bool ProjectItem::isValid() const
{
    return valid_;         
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::setValid()
//-----------------------------------------------------------------------------
void ProjectItem::setValid(bool valid)
{
    valid_ = valid;
}

//-----------------------------------------------------------------------------
// Function: ProjectItem::findChildForLevel()
//-----------------------------------------------------------------------------
ProjectItem* ProjectItem::findChildForLevel(Level level, VLNV const& vlnv)
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

    for (ProjectItem* child : childItems_)
    {
        if (child->name() == childNameToMatch)
        {
            return child;
        }
    }

    return 0;
}
