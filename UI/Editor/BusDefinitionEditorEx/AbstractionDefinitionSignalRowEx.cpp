//-----------------------------------------------------------------------------
// File: AbstractionDefinitionSignalRowEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Represents a row in the abstraction ports table by grouping the Port, Wire and Transactional elements.
//-----------------------------------------------------------------------------

#include <IPXACTmodels/AbstractionDefinition/PortAbstraction.h>
#include <IPXACTmodels/AbstractionDefinition/WirePort.h>
#include <IPXACTmodels/AbstractionDefinition/TransactionalPort.h>

#include "AbstractionDefinitionSignalRowEx.h"


//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::()
//-----------------------------------------------------------------------------
AbstractionDefinitionSignalRowEx::AbstractionDefinitionSignalRowEx():
    abstraction_(QSharedPointer<PortAbstraction>(new PortAbstraction())),
    mode_(General::INTERFACE_MODE_COUNT),
    wire_(),
    transactional_(),
    lockExtendData_(false),
    lockPortData_(false)
{

}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::operator==()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionSignalRowEx::operator==(AbstractionDefinitionSignalRowEx const& other) const
{
    return  abstraction_->getLogicalName() == other.abstraction_->getLogicalName() &&
        mode_ == other.mode_ && comparedSignalIsSameType(other) &&
        lockExtendData_ == other.lockExtendData_ && lockPortData_ == other.lockPortData_ &&
        (mode_ != General::SYSTEM || getSystemGroup() == other.getSystemGroup());
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::getSystemGroup()
//-----------------------------------------------------------------------------
QString AbstractionDefinitionSignalRowEx::getSystemGroup() const
{
    if (mode_ == General::SYSTEM)
    {
        if (wire_)
        {
            return wire_->getSystemGroup();
        }
        else if (transactional_)
        {
            return transactional_->getSystemGroup();
        }
    }

    return QString("");
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::comparedSignalIsSameType()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionSignalRowEx::comparedSignalIsSameType(
    AbstractionDefinitionSignalRowEx const& comparisonSignal) const
{
    if ((wire_ && comparisonSignal.wire_) || (transactional_ && comparisonSignal.transactional_))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::operator!=()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionSignalRowEx::operator!=(AbstractionDefinitionSignalRowEx const& other) const
{
    return  abstraction_->getLogicalName() != other.abstraction_->getLogicalName() ||
        mode_ != other.mode_ ||
        !comparedSignalIsSameType(other) ||
        getSystemGroup() != other.getSystemGroup() ||
        lockExtendData_ != other.lockExtendData_ || lockPortData_ != other.lockPortData_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::SignalRow::operator<()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionSignalRowEx::operator<( const AbstractionDefinitionSignalRowEx& other ) const
{
	// Order by name, mode and system group.
	if (abstraction_->getLogicalName() == other.abstraction_->getLogicalName()) 
    {
        if (mode_ == other.mode_)
        {
            return getSystemGroup() < other.getSystemGroup();
        }

		return mode_ < other.mode_;
	}
	else
    {
		return abstraction_->getLogicalName() < other.abstraction_->getLogicalName();
    }
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::getMode()
//-----------------------------------------------------------------------------
General::InterfaceMode AbstractionDefinitionSignalRowEx::getMode() const
{
    return mode_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::setMode()
//-----------------------------------------------------------------------------
void AbstractionDefinitionSignalRowEx::setMode(General::InterfaceMode const& newMode)
{
    mode_ = newMode;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::getPortAbstraction()
//-----------------------------------------------------------------------------
QSharedPointer<PortAbstraction> AbstractionDefinitionSignalRowEx::getPortAbstraction() const
{
    return abstraction_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::setPortAbstraction()
//-----------------------------------------------------------------------------
void AbstractionDefinitionSignalRowEx::setPortAbstraction(QSharedPointer<PortAbstraction> newAbstraction)
{
    abstraction_ = newAbstraction;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::getWire()
//-----------------------------------------------------------------------------
QSharedPointer<WirePort> AbstractionDefinitionSignalRowEx::getWire() const
{
    return wire_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::setWire()
//-----------------------------------------------------------------------------
void AbstractionDefinitionSignalRowEx::setWire(QSharedPointer<WirePort> newWire)
{
    wire_ = newWire;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::getTransactional()
//-----------------------------------------------------------------------------
QSharedPointer<TransactionalPort> AbstractionDefinitionSignalRowEx::getTransactional() const
{
    return transactional_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::setTransactional()
//-----------------------------------------------------------------------------
void AbstractionDefinitionSignalRowEx::setTransactional(QSharedPointer<TransactionalPort> newTransactional)
{
    transactional_ = newTransactional;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::isExtendDataLocked()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionSignalRowEx::isExtendDataLocked() const
{
    return lockExtendData_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::lockExtendData()
//-----------------------------------------------------------------------------
void AbstractionDefinitionSignalRowEx::lockExtendData(bool newLockStatus)
{
    lockExtendData_ = newLockStatus;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::isPortDataLocked()
//-----------------------------------------------------------------------------
bool AbstractionDefinitionSignalRowEx::isPortDataLocked() const
{
    return lockPortData_;
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionSignalRowEx::lockPortData()
//-----------------------------------------------------------------------------
void AbstractionDefinitionSignalRowEx::lockPortData(bool newLockStatus)
{
    lockPortData_ = newLockStatus;
}
