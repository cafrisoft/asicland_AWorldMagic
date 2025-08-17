//-----------------------------------------------------------------------------
// File: LogicalPortColumns.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Common definitions for abstraction definition logical port table.
//-----------------------------------------------------------------------------

#ifndef LOGICAL_PORT_COLUMNS
#define LOGICAL_PORT_COLUMNS

namespace LogicalPortColumns
{
    //! Enumeration for columns in abstraction definition logical port table.
    enum Columns
    {
        NAME = 0,
        MODE,
        PRESENCE,
        DIRECTION,
        INITIATIVE,
        KIND,
        WIDTH,
        BUSWIDTH,
        DEFAULT_VALUE,
        DRIVER,
        QUALIFIER,
        SYSTEM_GROUP,
        PROTOCOLTYPE,
        PAYLOADNAME,
        PAYLOADTYPE,
        PAYLOADEXTENSION,
        DESCRIPTION,
        COLUMN_COUNT
    };

    enum class AbstractionType
    {
        WIRE,
        TRANSACTIONAL
    };
}

#endif // !LOGICAL_PORT_COLUMNS
