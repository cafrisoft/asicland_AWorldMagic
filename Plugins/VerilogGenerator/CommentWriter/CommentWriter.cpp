//-----------------------------------------------------------------------------
// File: CommentWriter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 08.08.2014
//
// Description:
// Class for writing C/Verilog comments.
//-----------------------------------------------------------------------------

#include "CommentWriter.h"

#include <QStringList>

//-----------------------------------------------------------------------------
// Function: CommentWriter::CommentWriter()
//-----------------------------------------------------------------------------
CommentWriter::CommentWriter(QString const& comment): comment_(comment), 
    lineLength_(DEFAULT_LINE_LENGHT), 
    indentSize_(0)
{

}

//-----------------------------------------------------------------------------
// Function: CommentWriter::~CommentWriter()
//-----------------------------------------------------------------------------
CommentWriter::~CommentWriter()
{

}

//-----------------------------------------------------------------------------
// Function: CommentWriter::write()
//-----------------------------------------------------------------------------
void CommentWriter::write(QTextStream& output) const
{
    if (nothingToWrite())
    {
        return;
    }

    QString indent = QString(" ").repeated(indentSize_);

    for (QString line : comment_.split("\n", Qt::KeepEmptyParts))
//    for (QString line : comment_.split("\n", QString::SplitBehavior::KeepEmptyParts))
    {
        do
        {
            auto lineEnd = line.length();

            if (line.length() > lineLength_ && line.lastIndexOf(" ", lineLength_) != -1)
            {
                lineEnd = line.lastIndexOf(" ", lineLength_);
            }
            else if (line.length() > lineLength_ && line.indexOf(" ", lineLength_) != -1)
            {
                lineEnd = line.indexOf(" ", lineLength_);
            }
            
            output << indent << "// " << line.left(lineEnd) << Qt::endl;
            line = line.remove(0, lineEnd + 1);
        } while (line.length() > 0);
    }
}

//-----------------------------------------------------------------------------
// Function: CommentWriter::nothingToWrite()
//-----------------------------------------------------------------------------
bool CommentWriter::nothingToWrite() const
{
    return comment_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: CommentWriter::setLineLength()
//-----------------------------------------------------------------------------
void CommentWriter::setLineLength(int maxLength)
{
    lineLength_ = maxLength;        
}

//-----------------------------------------------------------------------------
// Function: CommentWriter::setIndent()
//-----------------------------------------------------------------------------
void CommentWriter::setIndent(int size)
{
    indentSize_ = size;
}
