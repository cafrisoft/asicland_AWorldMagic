//-----------------------------------------------------------------------------
// File: librarytreefilter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Antti Kamppi
// Date: 20.12.2010
//
// Description:
// The class that does the filtering and sorting of items for the library tree view.
//-----------------------------------------------------------------------------

#include "LibraryTreeFilter.h"

//#include "LibraryItem.h"
//#include "LibraryInterface.h"

#include <KactusAPI/LibraryItem.h>
#include <KactusAPI/LibraryInterface.h>

#include <IPXACTmodels/common/Document.h>
#include <IPXACTmodels/Design/Design.h>

//-----------------------------------------------------------------------------
// Function: LibraryTreeFilter::LibraryTreeFilter()
//-----------------------------------------------------------------------------
LibraryTreeFilter::LibraryTreeFilter(LibraryInterface* handler, QObject *parent):
LibraryFilter(handler, parent)
{

}

//-----------------------------------------------------------------------------
// Function: LibraryTreeFilter::filterAcceptsRow()
//-----------------------------------------------------------------------------
bool LibraryTreeFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex itemIndex = sourceModel()->index(sourceRow, 0, sourceParent);

    // root item is always ok.
    if (!itemIndex.isValid())
    {
        return true;
    }

    LibraryItem* item = static_cast<LibraryItem*>(itemIndex.internalPointer());

    // get all vlnvs for the item
    QVector<VLNV> list = item->getVLNVs();

    // if none of the vlnvs match the search rules
    if (!checkVLNVs(list))
    {
        return false;
    }

    LibraryInterface* libraryAccess = getLibraryInterface();

    for (VLNV const& vlnv : list)
    {
        VLNV::IPXactType documentType = libraryAccess->getDocumentType(vlnv);
        QSharedPointer<Document const> document = libraryAccess->getModelReadOnly(vlnv);

        if (documentType == VLNV::COMPONENT)
        {
            if (type().components_)
            {
                QSharedPointer<Component const> component = document.staticCast<Component const>();

                if (component.isNull() ||
                    (checkImplementation(component) && checkHierarchy(component) && 
                        checkFirmness(component) && checkTags(document)))
                {
                    return true;
                }                   
            }
        }

        else if (documentType == VLNV::CATALOG)
        {
            if (type().catalogs_ && (document.isNull() || checkTags(document)))
            {
                return true;
            }
        }

        else if (documentType == VLNV::BUSDEFINITION || documentType == VLNV::ABSTRACTIONDEFINITION)
        {
            if (type().buses_ && implementation().hw_ && (document.isNull() || checkTags(document)))
            {
                return true;
            }
        }

        else if ((documentType == VLNV::COMDEFINITION || documentType == VLNV::APIDEFINITION))
        {
            if (type().apis_ && (document.isNull() || checkTags(document)))
            {
                return true;
            }
        }

        else if (documentType == VLNV::DESIGN)
        {
            QSharedPointer<Design> design = libraryAccess->getDesign(vlnv);

            if ((type().advanced_ || (type().components_ && implementation().sw_ &&
                (design.isNull() || design->getImplementation() == KactusAttribute::SW))) && (design.isNull() || checkTags(document)))
            {
                return true;
            }
        }

        else if (type().advanced_ && (document.isNull() || checkTags(document)))
        {
            return true;
        }
    }

    return false;
}
