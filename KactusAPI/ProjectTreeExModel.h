/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectTreeExModel.h
 *      Project: AWM
 *      Description :
 *          The model that contains the LibraryItems to display library hierarchically.
 */

#ifndef PROJECTTREEEXMODEL_H
#define PROJECTTREEEXMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QSharedPointer>
#include <QList>

#include <IPXACTmodels/common/VLNV.h>

#include <KactusAPI/KactusAPIGlobal.h>
#include <KactusAPI/LibraryItem.h>

class LibraryInterface;

//-----------------------------------------------------------------------------
//! The model that contains the LibraryItems to display library hierarchically.
//-----------------------------------------------------------------------------
//class KACTUS2_API ProjectTreeExModel : public QAbstractItemModel
class ProjectTreeExModel : public QAbstractItemModel
{
    Q_OBJECT

public:

	/*! The constructor
	*
	*      @param [in] handler          The instance that manages the library.
	*      @param [in] parent           The owner of this class
	*
	*/
    ProjectTreeExModel(LibraryInterface* handler, QObject* parent = 0);

	//! The destructor
    virtual ~ProjectTreeExModel();

	/*! Get the data for the headers of this model.
	*
	*      @param [in] section          Specifies the column of the header.
	*      @param [in] orientation      The orientation of the header, only Qt::Horizontal supported.
	*      @param [in] role             Specifies the role of the data.
	*
	*      @return QVariant containing the data.
	*/
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	/*! Get the number of columns in this model
	*
	*      @param [in] parent Model index identifying the item that's column count is  wanted.
	*
	*      @return Number of columns.
	*/
	virtual int columnCount(QModelIndex const& parent = QModelIndex()) const;

	/*! Get the number of rows an item has.
	*
	*      @param [in] parent Model index identifying the item that's row count is wanted
	*
	*      @return Number of rows the item has.
	*/
	virtual int rowCount(QModelIndex const& parent = QModelIndex()) const;

	/*! Get the model index of the specified object.
	*
	*      @param [in] row      Row number of the object.
	*      @param [in] column   Column number of the object.
	*      @param [in] parent   Model index of the parent of the object.
	*
	*      @return QModelIndex that identifies the object.
	*/
	virtual QModelIndex index(int row, int column, QModelIndex const& parent = QModelIndex()) const;

	/*! Get the model index of the parent of the object
	*
	*      @param [in] child Model index that identifies the child of the object.
	*
	*      @return QModelIndex that identifies the parent of the given object.
	*/
	virtual QModelIndex parent(QModelIndex const& child) const;

	/*! Get the data associated with given object.
	*
	*      @param [in] index    Model index that identifies the object that's data is wanted.
	*      @param [in] role     Specifies the type of data wanted.
	*
	*      @return QVariant Containing the requested data.
	*/
	virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const;

	/*! Does the specified item have child items or not.
	*
	*      @param [in] parent Model index identifying the object that's children are asked.
	*
	*      @return True if object has child objects.
	*/
	virtual bool hasChildren(QModelIndex const& parent = QModelIndex()) const;

	/*! Get the flags that identify possible methods for given object.
	*
	*      @param [in] index Model index identifying the object that's flags are requested.
	*
	*      @return Qt::ItemFlags that specify how the object can be handled.
	*/
	Qt::ItemFlags flags(QModelIndex const& index) const;

	/*! Get the root item of the tree.
	*
	*      @return The root item
    */
    LibraryItem* getRoot() const;

	/*! Create a model index for a library item
	*
    *      @param [in] item The LibraryItem that's model index is wanted
	*
    *      @return QModelIndex that identifies the LibraryItem.
	*/
    QModelIndex index(LibraryItem* item);

signals:

	//! Emit an error message to be printed to user.
	void errorMessage(const QString& message);

	//! Emit a notice message to be printed to user.
	void noticeMessage(const QString& message);

    //! Shows errors about the item at the given index.
    void showErrors(VLNV const& vlnv);

	//! Open the design of a component.
	void openDesign(VLNV const& vlnv, QString const& viewName);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the SW design of a component.
    void openSWDesign(VLNV const& vlnv);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the memory design of a HW design.
    void openMemoryDesign(VLNV const& vlnv, QString const& viewName);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SYSTEMMENU_SUPPORT_20231229 == 1
    //! Open the system design of a component.
    void openSystemDesign(VLNV const& vlnv);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

	//! Open the component in a component editor.
	void editItem(VLNV const& vlnv);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new SW design with given vlnv.
    void createSWDesign(VLNV const& vlnv);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new system design with given vlnv.
    void createSystemDesign(VLNV const& vlnv);
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if DELETEMENU_SUPPORT_20231229 == 1
    //! Remove the specified VLNV from the library
    void removeVLNV(QList<VLNV> vlnvs);
#endif // DELETEMENU_SUPPORT_20231229

	//! Export a list of vlnv items to a new location.
    void exportItems(const QString& name, const QList<VLNV> list);

	//! Refresh the item filtering because changes have been made
	void invalidateFilter();

	//! Items have changed so th VLNVDialer should refresh itself.
	void refreshDialer();

public slots:

    //! Shows errors about the item at the given index.
    void onShowErrors(QModelIndex const& index);

	//! Open the selected hierarchical design
	void onOpenDesign(QModelIndex const& index, QString const& viewName);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the selected hierarchical SW design
    void onOpenSWDesign(QModelIndex const& index);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if MEMORYDESIGNMENU_SUPPORT_20231229 == 1
    //! Open the selected memory design.
    void onOpenMemoryDesign(QModelIndex const& index, QString const& viewName);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

#if SYSTEMMENU_SUPPORT_20231229 == 1
    //! Open the selected hierarchical system design
    void onOpenSystemDesign(QModelIndex const& index);
#endif // MEMORYDESIGNMENU_SUPPORT_20231229

	//! Open the selected document e.g. component or bus definition.
	void onOpenDocument(QModelIndex const& index);

#if SWDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new SW design
    void onCreateNewSWDesign(QModelIndex const& index);
#endif // SWDESIGNMENU_SUPPORT_20231229

#if SYSTEMDESIGNMENU_SUPPORT_20231229 == 1
    //! Create new system design
    void onCreateNewSystemDesign(QModelIndex const& index);
#endif // SYSTEMDESIGNMENU_SUPPORT_20231229

#if DELETEMENU_SUPPORT_20231229 == 1
    //! When delete is selected in search view
    void onDeleteItem(QModelIndex const& index);
#endif // DELETEMENU_SUPPORT_20231229

	//! When export is selected in search view
    void onExportItem(QModelIndex const& index);

	//! Remove the specified vlnv branch from the tree.
	void onRemoveVLNV(VLNV const& vlnv);

	//! Add the specified vlnv branch to the tree.
	void onAddVLNV(VLNV const& vlnv);

	//! Reset the model.
	void onResetModel();

    /*! This function should be called when an IP-XACT document has changed.
     *
     * Function updates the hierarchical model so that changes made to the document are visible.
     *
     *      @param [in] vlnv Identifies the document that changed.
    */
    void onDocumentUpdated(VLNV const& vlnv);

private:
    //! No copying
    ProjectTreeExModel(const ProjectTreeExModel& other);

    //! No assignment
    ProjectTreeExModel& operator=(const ProjectTreeExModel& other);

    /*! Removes an item from the treeModel
     *
     *      @param [in] toRemove     The LibraryItem that is the highest-level item to remove.
    */
    void removeLibraryItem(LibraryItem* toRemove);
    
    /*!
     *  Validates the given item and possible sub-items.
     *
     *      @param [in] item   The item to validate.
     *
     *      @return True, if the item is valid, otherwise false.
     */
    bool validate(LibraryItem* item);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    /*! The root of the tree.
     *
     * The LibraryTreeFilter owns the root item and the root item owns it's child items.
    */
    LibraryItem* rootItem_;

    //! The instance that manages the library.
    LibraryInterface* handler_;
};

#endif // PROJECTTREEEXMODEL_H
