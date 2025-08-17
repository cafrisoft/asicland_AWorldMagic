//-----------------------------------------------------------------------------
// File: AbstractionWirePortsSortFilterEx.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Sort filter proxy model for abstraction definition wire ports.
//-----------------------------------------------------------------------------

#ifndef ABSTRACTIONWIREPORTSSORTFILTEREX_H
#define ABSTRACTIONWIREPORTSSORTFILTEREX_H

#include "AbstractionDefinitionPortsSortFilterEx.h"

//-----------------------------------------------------------------------------
//! Sort filter proxy model for abstraction definition wire ports.
//-----------------------------------------------------------------------------
class AbstractionWirePortsSortFilterEx : public AbstractionDefinitionPortsSortFilterEx
{

public:

	/*!
	 *  The constructor.
	 *
     *      @param [in] parent      Pointer to the owner of this model.
	 */
    AbstractionWirePortsSortFilterEx(PortAbstractionInterface* portInterface, QObject *parent);

	/*!
	 *  The destructor.
	 */
    virtual ~AbstractionWirePortsSortFilterEx() = default;

    //! No copying. No assignment.
    AbstractionWirePortsSortFilterEx(const AbstractionWirePortsSortFilterEx& other) = delete;
    AbstractionWirePortsSortFilterEx& operator=(const AbstractionWirePortsSortFilterEx& other) = delete;

protected:

    /*!
     *  Check if the filter accepts the selected row.
     *
     *      @param [in] source_row      The selected row.
     *      @param [in] source_parent   Parent index of the selected row.
     *
     *      @return True, if the row is accepted, false otherwise.
     */
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override final;

};

#endif // ABSTRACTIONWIREPORTSSORTFILTEREX_H
