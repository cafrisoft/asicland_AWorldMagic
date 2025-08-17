//-----------------------------------------------------------------------------
// File: BusInterfaceUtilities.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 15.02.2019
//
// Description:
// Utilities for bus interface mode compatibilities.
//-----------------------------------------------------------------------------

#include <KactusAPI/BusInterfaceUtilities.h>

#include <IPXACTmodels/BusDefinition/BusDefinition.h>

#include <KactusAPI/LibraryInterface.h>

//-----------------------------------------------------------------------------
// Function: BusInterfaceUtilities::hasMatchingBusDefinitions()
//-----------------------------------------------------------------------------
bool BusInterfaceUtilities::hasMatchingBusDefinitions(QSharedPointer<const BusDefinition> firstDefinition,
    VLNV const& secondDefinitionVLNV, LibraryInterface* library)
{
    if (firstDefinition)
    {
        QSharedPointer<const BusDefinition> comparisonDefinition =
            library->getModelReadOnly(secondDefinitionVLNV).dynamicCast<const BusDefinition>();
        if (comparisonDefinition)
        {
            if (firstDefinition == comparisonDefinition)
            {
                return true;
            }
            else
            {
                VLNV comparisonExtendVLNV = comparisonDefinition->getExtends();
                VLNV firstExtendVLNV = firstDefinition->getExtends();

                if ((comparisonExtendVLNV.isValid() &&
                    hasMatchingBusDefinitions(firstDefinition, comparisonExtendVLNV, library)) ||
                    (firstExtendVLNV.isValid() &&
                        hasMatchingBusDefinitions(comparisonDefinition, firstExtendVLNV, library)))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceUtilities::hasMatchingBusDefinitions()
//-----------------------------------------------------------------------------
bool BusInterfaceUtilities::busDefinitionVLNVsMatch(VLNV const& firstDefinitionVLNV,
    VLNV const& secondDefinitionVLNV, LibraryInterface* library)
{
    QSharedPointer<const BusDefinition> firstDefinition =
        library->getModelReadOnly(firstDefinitionVLNV).dynamicCast<const BusDefinition>();
    if (firstDefinition)
    {
        return BusInterfaceUtilities::hasMatchingBusDefinitions(firstDefinition, secondDefinitionVLNV, library);
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: BusInterfaceUtilities::getIconPath()
//-----------------------------------------------------------------------------
QString BusInterfaceUtilities::getIconPath(General::InterfaceMode busMode)
{
    QString iconPath("");
    if (busMode == General::MASTER)
    {
        iconPath = ":resources/Res/busInterfaceMaster.png";
    }
    else if (busMode == General::MIRROREDMASTER)
    {
        iconPath = ":resources/Res/busInterfaceMirroredMaster.png";
    }
    else if (busMode == General::SLAVE)
    {
        iconPath = ":resources/Res/busInterfaceSlave.png";
    }
    else if (busMode == General::MIRROREDSLAVE)
    {
        iconPath = ":resources/Res/busInterfaceMirroredSlave.png";
    }
    else if (busMode == General::SYSTEM)
    {
        iconPath = ":resources/Res/busInterfaceSystem.png";
    }
    else if (busMode == General::MIRROREDSYSTEM)
    {
        iconPath = ":resources/Res/busInterfaceMirroredSystem.png";
    }
    else if (busMode == General::MONITOR)
    {
        iconPath = ":resources/Res/busInterfaceMonitor.png";
    }

    return iconPath;
}
