//-----------------------------------------------------------------------------
// File: DesignWidgetFactoryImplementation.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 11.02.2015
//
// Description:
// Class for design widget factory.
//-----------------------------------------------------------------------------

#include "DesignWidgetFactoryImplementation.h"

#include <UI/HWDesign/HWDesignWidget.h>

//-----------------------------------------------------------------------------
// Function: DesignWidgetFactoryImplementation::DesignWidgetFactoryImplementation()
//-----------------------------------------------------------------------------
DesignWidgetFactoryImplementation::DesignWidgetFactoryImplementation(LibraryInterface* designInterface,
                                                                     LibraryInterface* libraryInterface,
                                                                     QSharedPointer<MultipleParameterFinder> designAndInstanceParameterFinder,
                                                                     QSharedPointer<ListParameterFinder> designParameterFinder):
    designInterface_(designInterface),
    libraryInterface_(libraryInterface),
    designAndInstanceParameterFinder_(designAndInstanceParameterFinder),
    designParameterFinder_(designParameterFinder)
{

}

//-----------------------------------------------------------------------------
// Function: DesignWidgetFactoryImplementation::~DesignWidgetFactoryImplementation()
//-----------------------------------------------------------------------------
DesignWidgetFactoryImplementation::~DesignWidgetFactoryImplementation()
{

}

//-----------------------------------------------------------------------------
// Function: DesignWidgetFactoryImplementation::makeHWDesignWidget()
//-----------------------------------------------------------------------------
DesignWidget* DesignWidgetFactoryImplementation::makeHWDesignWidget(QWidget* parent /* = 0 */)
{
    DesignWidget* hwDesignWidget (new HWDesignWidget(designInterface_,
                                                     libraryInterface_,
                                                     designAndInstanceParameterFinder_,
                                                     designParameterFinder_,
                                                     parent));

    return hwDesignWidget;
}
