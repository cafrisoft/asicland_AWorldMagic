/* 
 *  	Created on: 31.5.2012
 *      Author: Antti Kamppi
 * 		filename: summarylabel.cpp
 *		Project: Kactus 2
 */

#include "summarylabel.h"

#include <QFont>


 //-----------------------------------------------------------------------------
 // Function: SummaryLabel::SummaryLabel()
 //-----------------------------------------------------------------------------
SummaryLabel::SummaryLabel( const QString& text, QWidget *parent, bool showExtensionIcon):
QLabel(text, parent) {

	QFont usedFont = font();
	usedFont.setWeight(QFont::Bold);
	usedFont.setPointSize(usedFont.pointSize() + SummaryLabel::POINTSIZE_INCREMENT);
	setFont(usedFont);

    if (showExtensionIcon)
    {
        auto labelText = text;
        auto size = fontMetrics().height();

        setText("<html>" + labelText + "<img  src=':resources/Res/appicon.png'"
                                       "width='" + QString::number(size) + "' height='" + QString::number(size) + "' ></html>");

        setToolTip(tr("This is Kactus2 extension"));
    }
}

SummaryLabel::SummaryLabel( const QString& subtitle, const QString& content, QWidget *parent, bool showExtensionIcon):
QLabel(subtitle, parent) {

//    QFont usedFont = font();
//    usedFont.setWeight(QFont::Bold);
//    usedFont.setPointSize(usedFont.pointSize() + SummaryLabel::POINTSIZE_INCREMENT);
//    setFont(usedFont);

    if (showExtensionIcon)
    {
        setText("<html>\
            <a style='font-size: 24px; font-weight: bold; '>&nbsp;&nbsp;&nbsp;&nbsp;" + subtitle + "</a><br>\
            <a style='font-size: 16px;'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + content + "</a><br>\
        </html>");

//        setText("<html>\
//                <style>\
//                .subtitle {\
//                    font-weight: bold;\
//                    font-size: 24px;\
//                    padding-left: 20px;\
//                }\
//                .content {\
//                    font-size: 17px;\
//                    padding-left: 40px;\
//                }\
//                </style>\
//                <body>\
//                <a class='subtitle'>&nbsp;&nbsp;" + subtitle + "</a><br>\
//                <a class='content'>&nbsp;&nbsp;&nbsp;&nbsp;" + content + "</a>\
//                </body>\
//                </html>");
    }
}
