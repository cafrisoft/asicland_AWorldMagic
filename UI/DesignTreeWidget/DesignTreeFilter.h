//-----------------------------------------------------------------------------
// File: ProjectTreeExFilter.h
//-----------------------------------------------------------------------------
// Project: AMW
// Author: P.HG
// Date: 04.06.2024
//
// Description:
// The class that does the filtering and sorting of items for the library tree view.
//-----------------------------------------------------------------------------

#ifndef PROJECTTREEEXFILTER_H
#define PROJECTTREEEXFILTER_H

#include "../LibraryFilter.h"

class LibraryInterface;

//-----------------------------------------------------------------------------
//! The class that does the filtering and sorting of items for the library tree view.
//-----------------------------------------------------------------------------
class ProjectTreeExFilter : public LibraryFilter
{
	Q_OBJECT

public:

    /*! The constructor
     *
     *      @param [in]  handler    The instance that manages the library.
     *      @param [in]  parent     The owner of this widget.
     *
    */
    ProjectTreeExFilter(LibraryInterface* handler, QObject *parent = 0);

	//! The destructor
    virtual ~ProjectTreeExFilter() = default;

    //! No copying
    ProjectTreeExFilter(const ProjectTreeExFilter& other) = delete;

    //! No assignment
    ProjectTreeExFilter& operator=(const ProjectTreeExFilter& other) = delete;

protected:

	/*! Checks if the row should be shown in the view or not.
	 *
	 *      @param [in]  sourceRow      Specifies the row number of the item
	 *      @param [in]  sourceParent   Specifies the parent of the item.
	 *
	 *      @return True if item is shown, otherwise false.
	*/
	virtual bool filterAcceptsRow(int sourceRow, QModelIndex const& sourceParent) const override final;

};

#endif // PROJECTTREEEXFILTER_H
