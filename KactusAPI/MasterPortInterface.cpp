//-----------------------------------------------------------------------------
// File: MasterPortInterface.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 14.12.2020
//
// Description:
// Master interface for editing component ports and port abstractions.
//-----------------------------------------------------------------------------

//#include <MasterPortInterface.h>
#include "MasterPortInterface.h"

#include <IPXACTmodels/common/TransactionalTypes.h>

//-----------------------------------------------------------------------------
// Function: MasterPortInterface::MasterPortInterface()
//-----------------------------------------------------------------------------
MasterPortInterface::MasterPortInterface():
NameGroupInterface()
{

}

//-----------------------------------------------------------------------------
// Function: MasterPortInterface::getIconPathForMissingPort()
//-----------------------------------------------------------------------------
std::string MasterPortInterface::getIconPathForMissingPort() const
{
    return (":resources/Res/cross.png");
}

//-----------------------------------------------------------------------------
// Function: MasterPortInterface::getIconPathForDirection()
//-----------------------------------------------------------------------------
std::string MasterPortInterface::getIconPathForDirection(DirectionTypes::Direction direction) const
{
    std::string directionPath(":resources/Res/cross.png");
    if (direction == DirectionTypes::IN)
    {
        directionPath = ":resources/Res/input.png";
    }
    else if (direction == DirectionTypes::OUT)
    {
        directionPath = ":resources/Res/output.png";
    }
    else if (direction == DirectionTypes::INOUT)
    {
        directionPath = ":resources/Res/inout.png";
    }
    else if (direction == DirectionTypes::DIRECTION_PHANTOM)
    {
        directionPath = ":resources/Res/phantom.png";
    }

    return directionPath;
}

//-----------------------------------------------------------------------------
// Function: MasterPortInterface::getIconPathForInitiative()
//-----------------------------------------------------------------------------
std::string MasterPortInterface::getIconPathForInitiative(QString const& initiative) const
{
    std::string iconPath(":resources/Res/cross.png");
    if (initiative.compare(TransactionalTypes::INITIATIVE_PROVIDES, Qt::CaseInsensitive) == 0)
    {
        iconPath = ":resources/Res/provides.png";
    }
    else if (initiative.compare(TransactionalTypes::INITIATIVE_REQUIRES, Qt::CaseInsensitive) == 0)
    {
        iconPath = ":resources/Res/requires.png";
    }
    else if (initiative.compare(TransactionalTypes::INITIATIVE_BOTH, Qt::CaseInsensitive) == 0)
    {
        iconPath = ":resources/Res/requires_provides.png";
    }
    else if (initiative.compare(TransactionalTypes::INITIATIVE_PHANTOM, Qt::CaseInsensitive) == 0)
    {
        iconPath = ":resources/Res/phantom.png";
    }

    return iconPath;
}
