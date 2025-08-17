//-----------------------------------------------------------------------------
// File: DesignWidgetFactory.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 11.02.2015
//
// Description:
// Interface for design widget factory.
//-----------------------------------------------------------------------------

#ifndef DESIGNWIDGETFACTORY_H
#define DESIGNWIDGETFACTORY_H

#include <KactusAPI/LibraryInterface.h>

#include <UI/Common/DesignWidget.h>


//-----------------------------------------------------------------------------
//! Interface for constructing design widgets.
//-----------------------------------------------------------------------------
class DesignWidgetFactory
{
    //Q_OBJECT

public:

    /*!
     *  Destructor.
     */
    virtual ~DesignWidgetFactory() {};

    virtual DesignWidget* makeHWDesignWidget(QWidget* parent = 0) = 0;
};

#endif // DESIGNWIDGETFACTORY_H
