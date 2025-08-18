//-----------------------------------------------------------------------------
// File: VerilogTopDefaultWriter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Janne Virtanen
// Date: 24.01.2017
//
// Description:
// Class for writing default values for ports of the top component.
//-----------------------------------------------------------------------------

#include "VerilogTopDefaultWriter.h"

//-----------------------------------------------------------------------------
// Function: VerilogTopDefaultWriter::VerilogTopDefaultWriter()
//-----------------------------------------------------------------------------
VerilogTopDefaultWriter::VerilogTopDefaultWriter(QSharedPointer<MetaPort> mPort) : mPort_(mPort)
{
}

//-----------------------------------------------------------------------------
// Function: VerilogTopDefaultWriter::~VerilogTopDefaultWriter()
//-----------------------------------------------------------------------------
VerilogTopDefaultWriter::~VerilogTopDefaultWriter()
{
}

//-----------------------------------------------------------------------------
// Function: VerilogTopDefaultWriter::write()
//-----------------------------------------------------------------------------
void VerilogTopDefaultWriter::write(QTextStream& output) const
{
    if (mPort_->port_->getDirection() != DirectionTypes::OUT && mPort_->port_->getDirection() != DirectionTypes::INOUT)
    {
        return;
    }

    if (mPort_->defaultValue_.isEmpty())
    {
        return;
    }

    output << "    assign " << mPort_->port_->name() << " = " << mPort_->defaultValue_ << ";" << Qt::endl;
//    output << "    assign " << mPort_->port_->name() << " = " << mPort_->defaultValue_ << ";" << Qt::endl;    //. not surported Qt.5.14 부터 지원

}
