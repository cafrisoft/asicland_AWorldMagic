//-----------------------------------------------------------------------------
// File: FPGATreeFilter.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// The class that does the filtering and sorting of items for the FPGA tree view.
//-----------------------------------------------------------------------------

#ifndef FPGATREEFILTER_H
#define FPGATREEFILTER_H

#include <QSortFilterProxyModel>


//-----------------------------------------------------------------------------
//! The class that does the filtering and sorting of items for the library tree view.
//-----------------------------------------------------------------------------
class FPGATreeFilter : public QSortFilterProxyModel
{
	Q_OBJECT

public:

    /*! The constructor
     *
     *      @param [in]  handler    The instance that manages the library.
     *      @param [in]  parent     The owner of this widget.
     *
    */
    FPGATreeFilter(QObject *parent = 0);

	//! The destructor
    virtual ~FPGATreeFilter() = default;

private:
    QStringList m_filters;

protected:

	/*! Checks if the row should be shown in the view or not.
	 *
	 *      @param [in]  sourceRow      Specifies the row number of the item
	 *      @param [in]  sourceParent   Specifies the parent of the item.
	 *
	 *      @return True if item is shown, otherwise false.
    */
    virtual bool filterAcceptsRow(int sourceRow, QModelIndex const& sourceParent) const override;

    /*! Compares two items by value.
     *
     *      @param left [in]    Index identifying the left item.
     *      @param right [in]   Index identifying the right item.
     *
     *      @return True, if left < right, otherwise false.
    */
    virtual bool lessThan(QModelIndex const& left, QModelIndex const& right) const;

public:
    void setFilters(const QStringList &filters);

};

#endif // FPGATREEFILTER_H
