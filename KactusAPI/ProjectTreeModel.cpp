/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectTreeModel.cpp
 *      Project: AWM
 *      Description :
 *          The model that contains the LibraryItems to display library hierarchically.
 */

#include "ProjectTreeModel.h"
#include "LibraryItem.h"
#include "LibraryInterface.h"

#include <IPXACTmodels/common/Document.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Design/Design.h>

#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>

#include <KactusAPI/KactusColors.h>

#include <QSharedPointer>
#include <QBrush>

#include <QIcon>

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::ProjectTreeModel()
//-----------------------------------------------------------------------------
ProjectTreeModel::ProjectTreeModel(LibraryInterface* handler, QObject* parent):
QAbstractItemModel(parent),
    rootItem_(),
    handler_(handler)
{
    onResetModel();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::~ProjectTreeModel()
//-----------------------------------------------------------------------------
ProjectTreeModel::~ProjectTreeModel()
{
    delete rootItem_;
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::headerData()
//-----------------------------------------------------------------------------
QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return tr("Project items");
    }
    
    return QVariant();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::columnCount()
//-----------------------------------------------------------------------------
int ProjectTreeModel::columnCount(QModelIndex const&) const
{
    return 1;
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::rowCount()
//-----------------------------------------------------------------------------
int ProjectTreeModel::rowCount(QModelIndex const& parent) const
{
    // only one column exists
    if (parent.column() > 0)
    {
        return 0;
    }

    LibraryItem *parentItem = 0;

    // if the given item is invalid, it is interpreted as root item
    if (!parent.isValid()) 
    {
        parentItem = rootItem_;
    }
    else
    {
        parentItem = static_cast<LibraryItem*>(parent.internalPointer());
    }

    // return how many children the parent has
    return parentItem->getNumberOfChildren();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::index()
//-----------------------------------------------------------------------------
QModelIndex ProjectTreeModel::index(int row, int column, QModelIndex const& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    LibraryItem* parentItem = 0;

    // if the index of the parent is invalid then it is the root item
    if (!parent.isValid()) 
    {
        parentItem = rootItem_;
    }
    else
    {
        parentItem = static_cast<LibraryItem*>(parent.internalPointer());
    }

    if (!parentItem)
    {
        return QModelIndex();
    }

    // find the child item of the parent
    LibraryItem *child = parentItem->child(row);

    // if the child is found
    if (child)
    {
        return createIndex(row, column, child);
    }
    else
    {
        return QModelIndex();
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::index()
//-----------------------------------------------------------------------------
QModelIndex ProjectTreeModel::index(LibraryItem* item)
{
    // if pointer is null
    if (!item)
    {
        return QModelIndex();
    }

    // if this item has no parent then it is the root item
    LibraryItem* parent = item->parent();
    if (!parent)
    {
        return QModelIndex();
    }

    // create the index for the child
    return createIndex(item->row(), 0, item);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::parent()
//-----------------------------------------------------------------------------
QModelIndex ProjectTreeModel::parent(QModelIndex const& child) const
{
    if(!child.isValid())
    {
        return QModelIndex();
    }

    LibraryItem *childItem = static_cast<LibraryItem*>(child.internalPointer());
    LibraryItem *parent = childItem->parent();

    // If the parent does not exist then this item is the root item.
    if (!parent)
    {
        return QModelIndex();
    }

    // If row is invalid then the grandparent does not exist and parent is the root.
    int row = parent->row();
    if (row == -1)
    {
        return QModelIndex();
    }

    return createIndex(row, 0, parent);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::data()
//-----------------------------------------------------------------------------
QVariant ProjectTreeModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    if (role == Qt::DisplayRole)
    {
        return item->name();
    }

    else if (role == Qt::ForegroundRole)
    {
        if (item->isValid())
        {
            return KactusColors::REGULAR_TEXT;
        }
        else
        {
            return KactusColors::ERROR;
        }
    }

    else if (role == Qt::ToolTipRole)
    {
        VLNV vlnv = item->getVLNV();

        // if item can identify a single library object
        if (vlnv.isValid())
        {
            QString text = "<b>Vendor:</b> " + vlnv.getVendor() + "<br>" +
                "<b>Library:</b> " + vlnv.getLibrary() + "<br>" + 
                "<b>Name:</b> " + vlnv.getName() + "<br>" +
                "<b>Version:</b> " + vlnv.getVersion() + "<br>";

            QSharedPointer<Document const> document = handler_->getModelReadOnly(vlnv);

            if (document != 0 && !document->getDescription().isEmpty())
            {
                text += "<br><b>Description:</b><br>" + document->getDescription();
            }

            text += "<br><b>File Path:</b><br>" + handler_->getPath(vlnv);
            return text;
        }

        // if item did not represent a single vlnv item
        return QVariant();
    }
    
    else if (role == Qt::DecorationRole)
    {        
        VLNV vlnv = item->getVLNV();
        
        // if item represents a single vlnv object
        if (vlnv.isValid())
        {
            VLNV::IPXactType documentType = vlnv.getType();
            if (documentType == VLNV::COMPONENT)
            {
                QSharedPointer<Component const> component = 
                    handler_->getModelReadOnly(vlnv).staticCast<Component const>();

                if (component != 0)
                {
                    if (component->getImplementation() == KactusAttribute::SYSTEM)
                    {
                        return QIcon(":/resources/Res/system-component.png");
                    }
                    else if (component->getImplementation() == KactusAttribute::SW)
                    {
                        if (component->isHierarchical())
                        {
                            return QIcon(":/resources/Res/hier-sw-component.png");
                        }
                        else
                        {
                            return QIcon(":/resources/Res/sw-component48x48.png");
                        }
                    }
                    else
                    {
                        if (component->isHierarchical())
                        {
                            return QIcon(":/resources/Res/hier-hw-component.png");
                        }
                        else
                        {
                            return QIcon(":/resources/Res/hw-component.png");
                        }
                    }
                }

                return QIcon(":/resources/Res/hw-component.png");
            }

            else if (documentType == VLNV::ABSTRACTIONDEFINITION)
            {
                return QIcon(":/resources/Res/abs-def.png");
            }

            else if (documentType == VLNV::BUSDEFINITION)
            {
                return QIcon(":/resources/Res/bus-def.png");
            }

            else if (documentType == VLNV::CATALOG)
            {
                return QIcon(":/resources/Res/catalog.png");
            }

            else if (documentType == VLNV::COMDEFINITION)
            {
                return QIcon(":/resources/Res/new-com_definition.png");
            }

            else if (documentType == VLNV::APIDEFINITION)
            {
                return QIcon(":/resources/Res/new-api_definition.png");
            }

            else if (documentType == VLNV::DESIGN)
            {
                // Determine the design type.
                QSharedPointer<const Design> design = handler_->getModelReadOnly<Design>(vlnv);
                if (design && (design->getImplementation() == KactusAttribute::SW ||
                    design->getImplementation() == KactusAttribute::SYSTEM))
                {
                    return QIcon(":/resources/Res/sw-design48x48.png");
                }
                else
                {               
                    return QIcon(":/resources/Res/hw-design.png");
                }
            }

            else if (documentType == VLNV::DESIGNCONFIGURATION)
            {
                return QIcon(":/resources/Res/configuration.png");
            }
        }
    }
    
    return QVariant();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::flags()
//-----------------------------------------------------------------------------
Qt::ItemFlags ProjectTreeModel::flags(QModelIndex const& index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }

    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::hasChildren()
//-----------------------------------------------------------------------------
bool ProjectTreeModel::hasChildren(QModelIndex const& parent) const
{
    // only one column exists
    if (parent.column() > 0)
    {
        return false;
    }

    LibraryItem *parentItem = 0;

    // if the given item is invalid, it is interpreted as root item
    if (!parent.isValid())
    {
        parentItem = rootItem_;
    }
    else
    {
        parentItem = static_cast<LibraryItem*>(parent.internalPointer());
    }

    // return how many children the parent has
    return parentItem->hasChildren();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onExportItem()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onExportItem(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    emit exportItems(item->name(), item->getVLNVs().toList());
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onDeleteItem()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onDeleteItem(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* child = static_cast<LibraryItem*>(index.internalPointer());
    LibraryItem* toRemove = child->findHighestUnique();

    // inform the library handler that these VLNVs should be removed
    emit removeVLNV(toRemove->getVLNVs().toList());
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onRemoveVLNV()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onRemoveVLNV(VLNV const& vlnv)
{
    if (!vlnv.isValid())
    {
        return;
    }

    LibraryItem* toRemove = rootItem_->findHighestUnique(vlnv);
    
    // Remove the item and it's sub-items.
    if (toRemove)
    {
        removeLibraryItem(toRemove);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onAddVLNV()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onAddVLNV(VLNV const& vlnv)
{
    if (!vlnv.isValid())
    {
        return;
    }

    LibraryItem* parentItem = rootItem_->findHighestUnique(vlnv);

    QModelIndex parentIndex;

    // if the highest unique is not the root
    if (parentItem != rootItem_)
    {
        int row = parentItem->row();
        Q_ASSERT(row >= 0);

        parentIndex = createIndex(row, 0, parentItem);
    }
    
    int rowToAdd = parentItem->getNumberOfChildren();

    beginInsertRows(parentIndex, rowToAdd, rowToAdd);
    rootItem_->createChild(vlnv, static_cast<LibraryItem::Level>(0));
    endInsertRows();

    emit refreshDialer();
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::removeLibraryItem()
//-----------------------------------------------------------------------------
void ProjectTreeModel::removeLibraryItem(LibraryItem* toRemove)
{
    int row = toRemove->parent()->getIndexOf(toRemove);

    QModelIndex removeIndex = createIndex(row, 0, toRemove);

    // get the model index of the parent
    QModelIndex parentIndex = parent(removeIndex);

     beginRemoveRows(parentIndex, row, row);
     toRemove->parent()->removeChild(toRemove);
     endRemoveRows();

    emit refreshDialer();
}

#if DOCKWIDGETHANDLER_20231228 == 0
//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onResetModel()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onResetModel()
{
#if OPEN_DESIGN_TEST == 0
    beginResetModel();

    if (!rootItem_)
    {
        rootItem_ = new LibraryItem(this);
    }
    else
    {
        rootItem_->clear();
    }

    // get the items to be displayed from the data source
    for (VLNV const& item : handler_->getAllVLNVs())
    {
        VLNV::IPXactType documentType = item.getType();
        if (documentType == VLNV::ABSTRACTIONDEFINITION || documentType == VLNV::BUSDEFINITION ||
            documentType == VLNV::CATALOG || documentType == VLNV::COMPONENT ||
            documentType == VLNV::DESIGN || documentType == VLNV::DESIGNCONFIGURATION ||
            documentType == VLNV::APIDEFINITION || documentType == VLNV::COMDEFINITION)
        {
            rootItem_->createChild(item, LibraryItem::Level::ROOT);
        }
    }

    validate(rootItem_);

    endResetModel();

    emit refreshDialer();
#else

    beginResetModel();

    if (!rootItem_)
    {
        rootItem_ = new LibraryItem(this);
    }
    else
    {
        rootItem_->clear();
    }

    VLNV item2;

    // get the items to be displayed from the data source
    for (VLNV const& item : handler_->getAllVLNVs())
    {
        VLNV::IPXactType documentType = item.getType();
        if (documentType == VLNV::ABSTRACTIONDEFINITION || documentType == VLNV::BUSDEFINITION ||
            documentType == VLNV::CATALOG || documentType == VLNV::COMPONENT ||
            documentType == VLNV::DESIGN || documentType == VLNV::DESIGNCONFIGURATION ||
            documentType == VLNV::APIDEFINITION || documentType == VLNV::COMDEFINITION)
        {
            rootItem_->createChild(item, LibraryItem::Level::ROOT);

            if (item.getVendor() == "asicland" && item.getName() == "example_chip_for_zigbee")
            {
                item2 = item;
            }

        }
    }

    validate(rootItem_);


    //. onOpenDesign 테스트
    LibraryItem* library_Item = rootItem_->findItem(item2);

    QModelIndex itemIndex = index(library_Item);
    onOpenDesign(itemIndex, "hierarchical");


    endResetModel();

    emit refreshDialer();
#endif
}

#else
//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onResetModel()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onResetModel()
{
    beginResetModel();

    if (!rootItem_)
    {
        rootItem_ = new LibraryItem(this);
    }
    else
    {
        rootItem_->clear();
    }

    // get the items to be displayed from the data source
    for (VLNV const& item : handler_->getAllVLNVs())
    {
        VLNV::IPXactType documentType = item.getType();
        if (documentType == VLNV::ABSTRACTIONDEFINITION || documentType == VLNV::BUSDEFINITION ||
            documentType == VLNV::CATALOG || documentType == VLNV::COMPONENT ||
            documentType == VLNV::DESIGN || documentType == VLNV::DESIGNCONFIGURATION ||
            documentType == VLNV::APIDEFINITION || documentType == VLNV::COMDEFINITION)
        {
            rootItem_->createChild(item, LibraryItem::Level::ROOT);
        }
    }

    validate(rootItem_);

    endResetModel();

    emit refreshDialer();
}
#endif

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onDocumentUpdated()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onDocumentUpdated(VLNV const& vlnv)
{
    LibraryItem* item = rootItem_->findHighestUnique(vlnv);
    if (item)
    {
        validate(item);

        QModelIndex itemIndex = index(item);
        emit dataChanged(itemIndex, itemIndex);
    }    
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::getRoot()
//-----------------------------------------------------------------------------
LibraryItem* ProjectTreeModel::getRoot() const
{
    return rootItem_;
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateBus()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateBus(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::BUSDEFINITION);
    item->setVlnv(vlnv);

    emit createBus(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateAbsDef()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateAbsDef(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::BUSDEFINITION);
    item->setVlnv(vlnv);

    emit createAbsDef(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateComDef()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateComDef(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::COMDEFINITION);
    item->setVlnv(vlnv);

    emit createComDef(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateApiDef()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateApiDef(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::APIDEFINITION);
    item->setVlnv(vlnv);

    emit createApiDef(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onOpenDocument()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onOpenDocument(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    VLNV vlnv = item->getVLNV();

    if (vlnv.isValid())
    {
        emit editItem(vlnv);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onOpenDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onOpenDesign(QModelIndex const& index, QString const& viewName)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    VLNV vlnv = item->getVLNV();

    if (vlnv.isValid())
    {
        emit openDesign(vlnv, viewName);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onOpenMemoryDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onOpenMemoryDesign(QModelIndex const& index, QString const& viewName)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    if (item)
    {
        VLNV vlnv = item->getVLNV();

        if (vlnv.isValid())
        {
            emit openMemoryDesign(vlnv, viewName);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onOpenSWDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onOpenSWDesign(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    VLNV vlnv = item->getVLNV();

    if (vlnv.isValid())
    {
        emit openSWDesign(vlnv);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onOpenSystemDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onOpenSystemDesign(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    VLNV vlnv = item->getVLNV();

    if (vlnv.isValid())
    {
        emit openSystemDesign(vlnv);
    }
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateNewComponent()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateNewComponent(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::COMPONENT);
    item->setVlnv(vlnv);

    emit createComponent(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateNewDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateNewDesign(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::COMPONENT);
    item->setVlnv(vlnv);

    emit createDesign(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateNewSWDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateNewSWDesign(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::COMPONENT);
    item->setVlnv(vlnv);

    emit createSWDesign(vlnv);
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::onCreateNewSystemDesign()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onCreateNewSystemDesign(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());

    VLNV vlnv;
    vlnv.setType(VLNV::COMPONENT);
    item->setVlnv(vlnv);

    emit createSystemDesign(vlnv);
}

//-----------------------------------------------------------------------------
// Function: HierarchyModel::onShowErrors()
//-----------------------------------------------------------------------------
void ProjectTreeModel::onShowErrors(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    LibraryItem* item = static_cast<LibraryItem*>(index.internalPointer());
    emit showErrors(item->getVLNV());
}

//-----------------------------------------------------------------------------
// Function: ProjectTreeModel::validate()
//-----------------------------------------------------------------------------
bool ProjectTreeModel::validate(LibraryItem* item)
{
    bool isValid = false;
    if (item->getLevel() == LibraryItem::Level::VERSION)
    {
        isValid = handler_->isValid(item->getVLNV());
    }
    else
    {
        bool hasOneValidChild = false;

        int childCount = item->getNumberOfChildren();        
        for (int i = 0; i < childCount; i++)
        {
            bool childIsValid = validate(item->child(i));
            hasOneValidChild |= childIsValid;
        }

        isValid = hasOneValidChild;
    }

    item->setValid(isValid);
    return isValid;
}
