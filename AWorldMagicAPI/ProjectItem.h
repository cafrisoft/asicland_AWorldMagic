//-----------------------------------------------------------------------------
// File: ProjectItem.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 04.06.2024
//
// Description:
// ProjectItem class is used to create the hierarchical structure of IP library.
//-----------------------------------------------------------------------------

#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <QVector>
#include <QString>
#include <QObject>

#include <IPXACTmodels/common/VLNV.h>

#include <KactusAPI/KactusAPIGlobal.h>

//-----------------------------------------------------------------------------
//! ProjectItem class is used to create the hierarchical structure of IP library.
//-----------------------------------------------------------------------------
//class KACTUS2_API ProjectItem : public QObject
class ProjectItem : public QObject
{
	Q_OBJECT

public:

	/*! enum Level is used in constructing new items in library.
	 *
	 * Level is used to tell what information to use from VLNV tag in creating new component in hierarchy.
	 */
	enum class Level
    {
        ROOT = 0, 
		VENDOR, 
		LIBRARY,
		NAME,
		VERSION
    };

	/*! The default constructor.
	*
	*       @remark Only used when creating a root item for a tree structure.
	*/
        ProjectItem(QObject* parent);

	/*! The constructor
	*
	*      @param [in] vlnv     A VLNV tag that is to be added to database.
	*      @param [in] level    The level of the item in the hierarchy.
	*      @param [in] parent   The parent of this item.
	*/
        ProjectItem(VLNV const& vlnv, Level level, ProjectItem *parent);

        //! The destructor of the class ProjectItem.
        virtual ~ProjectItem();

	/*! Create a new child for an existing item.
	*
	*      @param [in] vlnv A VLNV tag that is to be added to database.
	*      @param [in] level The level of the item in the hierarchy.
	*/
	void createChild(VLNV const& vlnv, Level level);

	/*! Get the parent of the current node.
	 *
	 *      @return The parent of the item.
	 */
        ProjectItem* parent() const;

	/*! Get the name of the item.
	 *
	 *      @return The name of the item.
	 */
	QString name() const;

	/*! Get the level of the item in the hierarchy.
	 *
	 *      @return The level the item is in.
	 */
	Level getLevel() const;

	/*! Get the child item.
	 *
	 *      @param [in] index   index of the child item.
     *
	 *      @return The child.
	 */
        ProjectItem* child(int index) const;

	/*! Get the row number of the child item.
	 *
	 *      @return The row number where the item is located in its parents list.
	 */
	int row();

	/*! Get the number of child items the item has.
	 *
	 *      @return How many children item owns.
	 */
	int getNumberOfChildren() const;

	/*! Get the VLNV tag of this item.
	 *
     *      @return A VLNV instance that is represented by this item.
     *
	 *      @remark Valid VLNV pointer can only be returned if this item is level VERSION.
	 */
	VLNV getVLNV() const;

	/*! Get the index of the specified item.
	 *
	 *      @param [in] item    The library item that's row number is searched.
	 *
	 *      @return The row number of the specified item.
	 */
        int getIndexOf(ProjectItem* item);


	/*! Check if this item has children or not.
	 *
	 *      @return True if item has child-items, otherwise false.
	 */
	bool hasChildren() const;

	/*! Get list of all VLNV tags that exist under this item.
	 *
	 *      @return List of all VLNVs.
	 */
	QVector<VLNV> getVLNVs() const;

	/*! Remove a child of this item.
	 *
	 *      @param [in] childItem The child item that should be removed.
	 */
        void removeChild(ProjectItem* childItem);

        /*! Find the highest level ProjectItem that still identifies a single item.
	 * 
	 * This function checks if there are other children. If no other children
	 * are found then this function calls parent item to check if it can be
	 * used to identify a single item.
	 * 
	 *      @param [in] childItem   The item that is currently the highest possible item.
	 *
         *      @return ProjectItem* The item that is to be removed.
	 */
        ProjectItem* findHighestUnique(ProjectItem* childItem = 0);

        /*! Find the highest level ProjectItem that still identifies a single item.
	 * 
	 * This function finds the highest level possible to identify a single item.
	 *
	 *      @param [in] vlnv The vlnv element that must be matched.
	 *
	 *      @return The item that is the highest level item.
	 */
        ProjectItem* findHighestUnique(VLNV const& vlnv);

	/*! Find the library item that uniquely represents the specified vlnv.
	 *
	 *      @param [in] vlnv The vlnv that identifies the item to find.
	 *
         *      @return The ProjectItem that represents the vlnv in the tree.
	 */
        ProjectItem* findItem(VLNV const& vlnv);

	/*! Set the vlnv to match the tree structure of this item
	 *
	 *      @param [out] vlnv The vlnv instance to set.
	 */
	void setVlnv(VLNV& vlnv);

	//! Remove the children of this item.
	void clear();

	/*! Get all vendor items.
	 *
	 *      @return All vendor items under this item.
	*/
        QVector<ProjectItem*> getVendors() const;

	/*! Get all library items.
	 *
	 *      @return All library items under this item.
	*/
        QVector<ProjectItem*> getLibraries() const;

	/*! Get all name items.
	 *
	 *      @return All name items under this item.
	*/
        QVector<ProjectItem*> getNames() const;

	/*! Get all version items.
	 *
	 *      @return All version items under this item.
	*/
        QVector<ProjectItem*> getVersions() const;

    /*!
     *  Checks if the item is valid.
     *
     *      @return True, if the item is valid, otherwise false.
     */
    bool isValid() const;

    /*!
     *  Sets the validity of the item.
     *
     *      @param [in] valid   The validity to set.
     */
    void setValid(bool valid);

private:

	//! No copying
        ProjectItem(const ProjectItem &other);

	//! No assignment
        ProjectItem &operator=(const ProjectItem &other);


    /*!
     *  Finds the child item on the given level from the given VLNV.
     *
     *      @param [in] level   The level to get the item for.
     *      @param [in] vlnv    The VLNV to get the item from.
     *
     *      @return The child item on the given level.
     */
    ProjectItem* findChildForLevel(Level level, VLNV const& vlnv);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The name of the item on the level (i.e. TUT in vendor-level).
	QString name_;

	//! The level of the item.
	Level level_;

	//! The child items of current item in the hierarchy.
        QVector<ProjectItem*> childItems_;

	//! The parent of the item.
        ProjectItem *parentItem_;

	//! A VLNV tag that matches the version.
	VLNV vlnv_;

    bool valid_;
};

#endif // PROJECTITEM_H
