//-----------------------------------------------------------------------------
// File: ProjectLibraryTreeFilter.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 04.06.2024
//
// Description:
// The class that does the filtering and sorting of items for the library tree view.
//-----------------------------------------------------------------------------

#ifndef PROJECTLIBRARYTREEFILTER_H
#define PROJECTLIBRARYTREEFILTER_H

#include "../LibraryFilter.h"

class LibraryInterface;

//-----------------------------------------------------------------------------
//! The class that does the filtering and sorting of items for the library tree view.
//-----------------------------------------------------------------------------
class ProjectLibraryTreeFilter : public LibraryFilter
{
	Q_OBJECT

public:

    /*! The constructor
     *
     *      @param [in]  handler    The instance that manages the library.
     *      @param [in]  parent     The owner of this widget.
     *
    */
    ProjectLibraryTreeFilter(LibraryInterface* handler, QObject *parent = 0);

	//! The destructor
    virtual ~ProjectLibraryTreeFilter() = default;

    //! No copying
    ProjectLibraryTreeFilter(const ProjectLibraryTreeFilter& other) = delete;

    //! No assignment
    ProjectLibraryTreeFilter& operator=(const ProjectLibraryTreeFilter& other) = delete;

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

#endif // PROJECTLIBRARYTREEFILTER_H
