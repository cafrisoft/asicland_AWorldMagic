//-----------------------------------------------------------------------------
// File: ImportColors.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 29.09.2014
//
// Description:
// Common highlight colors for imported elements.
//-----------------------------------------------------------------------------

#ifndef IMPORTCOLORS_H
#define IMPORTCOLORS_H

#include <KactusAPI/KactusColors.h>

#include <QColor>

namespace ImportColors
{
    QColor const PORT = KactusColors::SW_COMPONENT;

    QColor const MODELPARAMETER = KactusColors::HW_BUS_COMPONENT;

    QColor const PARAMETER = KactusColors::MEMORY_BLOCK;

    QColor const VIEWNAME = QColor("Moccasin");

    QColor const INSTANCECOLOR = QColor(201, 82, 251);
}

#endif // IMPORTCOLORS_H
