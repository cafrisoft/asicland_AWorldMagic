#ifndef PROJECTITEMSELECTIONFACTORY_H
#define PROJECTITEMSELECTIONFACTORY_H

#include <KactusAPI/LibraryInterface.h>

#include <IPXACTmodels/common/VLNV.h>

#include <UI/Dialogs/ProjectSelectionDialog/ProjectSelectionDialog.h>

#include <QList>

namespace ProjectItemSelectionFactory
{
    /*!
     *  Construct the items for the selection dialog.
     *
     *      @param [in] dialog    The selection dialog.
     *      @param [in] vlvns     List of selectable VLNVs.
     */
//   void constructItemsForSelectionDialog(LibraryInterface* library, ProjectSelectionDialog* dialog,
//                                         QList<VLNV> const& vlvns);

   namespace Details
   {
//        void addComponentFilesAndDesignItems(LibraryInterface *library, ProjectSelectionDialog* dialog,
//            VLNV const& componentVLNV);

//        void addDesignItemForConfiguration(LibraryInterface* library, ProjectSelectionDialog* dialog,
//            VLNV const& configurationVLNV);

//        void addAbstractionDefinitionItems(LibraryInterface* library, ProjectSelectionDialog* dialog,
//            VLNV const& busDefinitionVLNV);

//        void addBusDefintionItemIfOnlyAbstraction(LibraryInterface* library, ProjectSelectionDialog* dialog,
//            VLNV const& abstractionVLNV);
   }
}

#endif // PROJECTITEMSELECTIONFACTORY_H
