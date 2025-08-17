//-----------------------------------------------------------------------------
// File: SystemViewConfigurationValidator.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 09.02.2016
//
// Description:
// Validator for the kactus2:SystemViewConfiguration.
//-----------------------------------------------------------------------------

#include "SystemViewConfigurationValidator.h"

#include <KactusAPI/LibraryInterface.h>

#include <IPXACTmodels/Design/ComponentInstance.h>

#include <IPXACTmodels/designConfiguration/ViewConfiguration.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/kactusExtensions/SystemView.h>

#include <QRegularExpression>

//-----------------------------------------------------------------------------
// Function: SystemViewConfigurationValidator::SystemViewConfigurationValidator()
//-----------------------------------------------------------------------------
SystemViewConfigurationValidator::SystemViewConfigurationValidator(LibraryInterface* library,
    QSharedPointer<ExpressionParser> parser):
ViewConfigurationValidator(library, parser),
availableViews_()
{

}

//-----------------------------------------------------------------------------
// Function: SystemViewConfigurationValidator::~SystemViewConfigurationValidator()
//-----------------------------------------------------------------------------
SystemViewConfigurationValidator::~SystemViewConfigurationValidator()
{

}

//-----------------------------------------------------------------------------
// Function: SystemViewConfigurationValidator::hasValidViewReference()
//-----------------------------------------------------------------------------
bool SystemViewConfigurationValidator::hasValidViewReference(QSharedPointer<ViewConfiguration> configuration)
{
    changeAvailableSystemViews(configuration);

    if (!configuration->getViewReference().isEmpty() && !availableViews_.isEmpty())
    {
        foreach (QString currentView, availableViews_)
        {
            if (configuration->getViewReference() == currentView)
            {
                return true;
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: SystemViewConfigurationValidator::changeAvailableSystemViews()
//-----------------------------------------------------------------------------
void SystemViewConfigurationValidator::changeAvailableSystemViews(QSharedPointer<ViewConfiguration> configuration)
{
    if (!configuration->getInstanceName().isEmpty() && getAvailableInstances())
    {
        foreach (QSharedPointer<ComponentInstance> instance, *getAvailableInstances())
        {
            if (instance->getInstanceName() == configuration->getInstanceName())
            {
                if (instance->getComponentRef() && instance->getComponentRef()->isValid())
                {
                    QSharedPointer<Component> component = getLibraryHandler()->getModel(
                        *instance->getComponentRef().data()).dynamicCast<Component>();

                    if (component)
                    {
                        availableViews_ = component->getViewNames();
						availableViews_.append( component->getSystemViewNames() );
                        return;
                    }
                }

                break;
            }
        }
    }
    
    if (!availableViews_.isEmpty())
    {
        availableViews_.clear();
    }
}
