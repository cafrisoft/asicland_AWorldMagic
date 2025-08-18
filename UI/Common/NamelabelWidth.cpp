//-----------------------------------------------------------------------------
// File: NamelabelWidth.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 5.11.2014
//
// Description:
// Label width calculator for ConnectionEndPoints derived classes.
//-----------------------------------------------------------------------------

#include "NamelabelWidth.h"

//-----------------------------------------------------------------------------
// Function: NamelabelWidth::NamelabelWidth
//-----------------------------------------------------------------------------
NamelabelWidth::NamelabelWidth()
{
}

//-----------------------------------------------------------------------------
// Function: NamelabelWidth::~NamelabelWidth
//-----------------------------------------------------------------------------
NamelabelWidth::~NamelabelWidth()
{
}

//-----------------------------------------------------------------------------
// Function: NamelabelWidth::getTextLength
//-----------------------------------------------------------------------------
qreal NamelabelWidth::getTextLength( QString text, QFont font )
{
	QFontMetrics fontMetrics(font);

	return fontMetrics.horizontalAdvance( text );
    //return fontMetrics.width( text );     //. not surported Qt5.11. 부터 지원
}

//-----------------------------------------------------------------------------
// Function: NamelabelWidth::setNameLabel
//-----------------------------------------------------------------------------
QString NamelabelWidth::setNameLabel( QString labelName, QFont font, qreal width )
{
	QFontMetrics fontMetrics(font);
	qreal shortWidth = fontMetrics.horizontalAdvance(labelName);
//    qreal shortWidth = fontMetrics.width(labelName);     //. not surported Qt5.11. 부터 지원
	QString shortLabel = labelName;

	if ( shortWidth + MIDDLE + PADDING > width )
	{
	    for (int i = 1; i < labelName.size(); ++i)
    	{
	    	shortLabel = labelName.left(labelName.size() - i);
		    shortWidth = fontMetrics.horizontalAdvance(shortLabel);
//            shortWidth = fontMetrics.width(shortLabel);     //. not surported Qt5.11. 부터 지원


    		if ( shortWidth + MIDDLE + PADDING < width  )
	    	{
		    	// Replace the last letters with ...
			    return shortLabel.left(shortLabel.size() - 2) + "...";
    		}
	    }
	}

	return shortLabel;
}
