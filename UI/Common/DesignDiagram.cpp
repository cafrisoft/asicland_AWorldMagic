//-----------------------------------------------------------------------------
// File: DesignDiagram.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Joni-Matti Maatta
// Date: 24.4.2012
//
// Description:
// Base class for all design diagrams.
//-----------------------------------------------------------------------------

#include "DesignDiagram.h"

#include "DesignWidget.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Design/Design.h>
#include <IPXACTmodels/common/GenericVendorExtension.h>
#include <IPXACTmodels/kactusExtensions/Kactus2Group.h>
#include <IPXACTmodels/kactusExtensions/Kactus2Position.h>

#include <KactusAPI/LibraryInterface.h>

#include <UI/Common/IEditProvider.h>

#include <UI/Common/graphicsItems/ComponentItem.h>
#include <UI/Common/graphicsItems/GraphicsColumnLayout.h>
#include <UI/Common/graphicsItems/ConnectionEndpoint.h>
#include <UI/Common/graphicsItems/GraphicsColumnConstants.h>

#include <UI/Common/Association/Association.h>
#include <UI/Common/diagramgrid.h>
#include <UI/Common/StickyNote/StickyNote.h>
#include <UI/Common/StickyNote/StickyNoteAddCommand.h>
#include <UI/Common/Association/AssociationAddCommand.h>

#include <UI/HWDesign/AdhocEditor/AdHocVisibilityEditor.h>

#include <QApplication>
#include <QGraphicsItem>
#include <QMenu>
#include <QPainter>
#include <QSharedPointer>
#include <QWidget>

//-----------------------------------------------------------------------------
// Function: DesignDiagram::DesignDiagram()
//-----------------------------------------------------------------------------
DesignDiagram::DesignDiagram(LibraryInterface *designInterface,
                             LibraryInterface* libraryInterface,
                             QSharedPointer<IEditProvider> editProvider,
                             DesignWidget* parent) :
    QGraphicsScene(parent),
    parent_(parent),
    designInterface_(designInterface),
    libraryInterface_(libraryInterface),
    editProvider_(editProvider),
    topView_(""),
    component_(),
    design_(),
    designConf_(),
    layout_(new GraphicsColumnLayout(this)),
    mode_(MODE_SELECT),
    loading_(false),
    locked_(false),
    interactionMode_(NORMAL),
    associationLine_(0)
{
    setSceneRect(0, 0, 100000, 100000);

    connect(this, SIGNAL(componentInstantiated(ComponentItem*)),
        this, SLOT(onComponentInstanceAdded(ComponentItem*)), Qt::UniqueConnection);
    connect(this, SIGNAL(componentInstanceRemoved(ComponentItem*)),
        this, SLOT(onComponentInstanceRemoved(ComponentItem*)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::~DesignDiagram()
//-----------------------------------------------------------------------------
DesignDiagram::~DesignDiagram()
{
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::clearScene()
//-----------------------------------------------------------------------------
void DesignDiagram::clearScene()
{
    clearLayout();
    clear();    
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setDesign()
//-----------------------------------------------------------------------------
bool DesignDiagram::setDesign(QSharedPointer<Component> component, QString const& selectedView,
    QSharedPointer<Design> design, QSharedPointer<DesignConfiguration> designConf)
{
    // Deselect items.
    emit clearItemSelection();
    clearSelection();

    // Clear the edit provider.
    editProvider_->clear();

    for (auto instanceItem : getInstances())
    {
        instanceItem->disconnect();
    }

    // Clear the scene.
    clearScene();
    getParent()->clearRelatedVLNVs();
    getParent()->addRelatedVLNV(component->getVlnv());

    // Set the new component and open the design.
    topView_ = selectedView;
    component_ = component;
    design_ = design;
    designConf_ = designConf;

    loading_ = true;
    loadDesign(design);
    loadStickyNotes();

    resetSceneRectangleForItems();

    loading_ = false;

    return true;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::resetSceneRectangleForItems()
//-----------------------------------------------------------------------------
void DesignDiagram::resetSceneRectangleForItems()
{
    QRectF requiredRectangle = itemsBoundingRect();
    qreal requiredWidth = requiredRectangle.width() + GraphicsColumnConstants::MIN_Y_PLACEMENT;
    qreal requiredHeight = requiredRectangle.height() + GraphicsColumnConstants::MIN_Y_PLACEMENT;

    setSceneRect(0, 0, requiredWidth, requiredHeight + GraphicsColumnConstants::MIN_Y_PLACEMENT);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getTopView()
//-----------------------------------------------------------------------------
QString DesignDiagram::getTopView() const
{
    return topView_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::attach()
//-----------------------------------------------------------------------------
void DesignDiagram::attach(AdHocVisibilityEditor* editor)
{
    connect(this, SIGNAL(contentChanged()), editor, SLOT(onContentChanged()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::detach()
//-----------------------------------------------------------------------------
void DesignDiagram::detach(AdHocVisibilityEditor* editor)
{
    disconnect(editor);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setMode()
//-----------------------------------------------------------------------------
void DesignDiagram::setMode(DrawMode mode)
{
    if (mode_ != mode)
    {
        mode_ = mode;

        if (mode_ != MODE_SELECT)
        {
            emit clearItemSelection();
        }

        emit modeChanged(mode);
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setProtection()
//-----------------------------------------------------------------------------
void DesignDiagram::setProtection(bool locked)
{
    locked_ = locked;
    clearSelection();
    emit clearItemSelection();

    setProtectionForStickyNotes();
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getMode()
//-----------------------------------------------------------------------------
DrawMode DesignDiagram::getMode() const
{
    return mode_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::isProtected()
//-----------------------------------------------------------------------------
bool DesignDiagram::isProtected() const
{
    return locked_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getInterface()
//-----------------------------------------------------------------------------
LibraryInterface* DesignDiagram::getInterface(const VLNV& vlnv) const
{
    LibraryInterface* libraryInterface = getDesignInterface();

    if (!libraryInterface->contains(vlnv))
    {
        libraryInterface = getLibraryInterface();
    }

    return libraryInterface;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getDesignInterface()
//-----------------------------------------------------------------------------
LibraryInterface* DesignDiagram::getDesignInterface() const
{
    return designInterface_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getLibraryInterface()
//-----------------------------------------------------------------------------
LibraryInterface* DesignDiagram::getLibraryInterface() const
{
    return libraryInterface_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getEditProvider()
//-----------------------------------------------------------------------------
QSharedPointer<IEditProvider> DesignDiagram::getEditProvider() const
{
    return editProvider_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getEditedComponent()
//-----------------------------------------------------------------------------
QSharedPointer<Component> DesignDiagram::getEditedComponent() const
{
    return component_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getDesign()
//-----------------------------------------------------------------------------
QSharedPointer<Design> DesignDiagram::getDesign() const
{
    return design_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getDesignConfiguration()
//-----------------------------------------------------------------------------
QSharedPointer<DesignConfiguration> DesignDiagram::getDesignConfiguration() const
{
    return designConf_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onComponentInstanceAdded()
//-----------------------------------------------------------------------------
void DesignDiagram::onComponentInstanceAdded(ComponentItem* item)
{
    getParent()->addRelatedVLNV(item->componentModel()->getVlnv());
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onComponentInstanceRemoved()
//-----------------------------------------------------------------------------
void DesignDiagram::onComponentInstanceRemoved(ComponentItem* item)
{
    getParent()->removeRelatedVLNV(item->componentModel()->getVlnv());
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onVendorExtensionAdded()
//-----------------------------------------------------------------------------
void DesignDiagram::onVendorExtensionAdded(QSharedPointer<VendorExtension> extension)
{
    if (design_ && !design_->getVendorExtensions()->contains(extension))
    {
        design_->getVendorExtensions()->append(extension);
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onVendorExtensionRemoved()
//-----------------------------------------------------------------------------
void DesignDiagram::onVendorExtensionRemoved(QSharedPointer<VendorExtension> extension)
{
    if (design_)
    {
        design_->getVendorExtensions()->removeAll(extension);
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onBeginAssociation()
//-----------------------------------------------------------------------------
void DesignDiagram::onBeginAssociation(Associable* startingPoint)
{
    setInteractionMode(ASSOCIATE);
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QPointF start = startingPoint->connectionPoint();
    associationLine_ = new QGraphicsLineItem(QLineF(start, start));
    addItem(associationLine_);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onItemModified()
//-----------------------------------------------------------------------------
void DesignDiagram::onItemModified(QUndoCommand* undoCommand)
{
    getEditProvider()->addCommand(QSharedPointer<QUndoCommand>(undoCommand));
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getUsedInstanceNames()
//-----------------------------------------------------------------------------
QStringList DesignDiagram::getUsedInstanceNames() const
{
    QStringList usedNames;
    for (QSharedPointer<ComponentInstance> instance : *getDesign()->getComponentInstances())
    {
        usedNames.append(instance->getInstanceName());
    }

    return usedNames;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::drawBackground()
//-----------------------------------------------------------------------------
void DesignDiagram::drawBackground(QPainter* painter, QRectF const& rect)
{
    painter->setWorldMatrixEnabled(true);
    painter->setPen(QPen(Qt::gray, 0));

    qreal left = int(rect.left()) - (int(rect.left()) % GridSize );
    qreal top = int(rect.top()) - (int(rect.top()) % GridSize );

    for (qreal x = left; x < rect.right(); x += GridSize )
    {
        for (qreal y = top; y < rect.bottom(); y += GridSize )
        {
            painter->drawPoint(x, y);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getTopmostComponent()
//-----------------------------------------------------------------------------
ComponentItem* DesignDiagram::getTopmostComponent(QPointF const& pos)
{
    QList<QGraphicsItem*> itemList = items(pos);

    for (QGraphicsItem* item : itemList)
    {
        ComponentItem* compItem = dynamic_cast<ComponentItem*>(item);

        if (compItem != 0)
        {
            return compItem;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getBaseItemOf()
//-----------------------------------------------------------------------------
QGraphicsItem* DesignDiagram::getBaseItemOf(QGraphicsItem* item) const
{
    QGraphicsItem* baseItem = item;

    while (baseItem != 0 && baseItem->parentItem() != 0 &&
        (baseItem->type() == QGraphicsTextItem::Type || baseItem->type() == QGraphicsPixmapItem::Type))
    {
        baseItem = baseItem->parentItem();
    }	

    return baseItem;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::createNote()
//-----------------------------------------------------------------------------
void DesignDiagram::createNoteAt(QPointF const& position)
{
    StickyNote* note = createStickyNote();
    note->setPos(position);

    QSharedPointer<StickyNoteAddCommand> cmd = createNoteAddCommand(note);
    cmd->redo(); 
    getEditProvider()->addCommand(cmd);

    clearSelection();
    emit clearItemSelection();
    note->setSelected(true);
    onSelected(note);

    note->beginEditing();
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::createContextMenu()
//-----------------------------------------------------------------------------
QMenu* DesignDiagram::createContextMenu(QPointF const&)
{
	return 0;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::contextMenuEvent()
//-----------------------------------------------------------------------------
void DesignDiagram::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	QMenu* menu = createContextMenu(event->scenePos());
	if (menu != 0)
	{
	    menu->exec(event->screenPos());
	    delete menu;
	}
	event->accept();
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::onShow()
//-----------------------------------------------------------------------------
void DesignDiagram::onShow()
{
    // Retrieve the new selection.
    QGraphicsItem *newSelection = 0;

    if (!selectedItems().isEmpty())
    {
        newSelection = selectedItems().front();
    }

    onSelected(newSelection);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::selectAll()
//-----------------------------------------------------------------------------
void DesignDiagram::selectAll()
{
    clearSelection();

    for (GraphicsColumn* column : getLayout()->getColumns())
    {
        column->setSelected(true);
    }
}

//-----------------------------------------------------------------------------
// Function: onVerticalScroll()
//-----------------------------------------------------------------------------
void DesignDiagram::onVerticalScroll(qreal y)
{
    getLayout()->setOffsetY(y);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setVisibilityControlState()
//-----------------------------------------------------------------------------
void DesignDiagram::setVisibilityControlState(QString const& name, bool state)
{
    if (name == QLatin1String("Sticky Notes"))
    { 
        for (QGraphicsItem* item: items())
        {       
            if (item->type() == StickyNote::Type || item->type() == Association::Type)
            {
                item->setVisible(state);
            }            
        }
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getInstances()
//-----------------------------------------------------------------------------
QList<ComponentItem*> DesignDiagram::getInstances() const
{
    // the list to store the diagram components to
    QList<ComponentItem*> instances;

    // ask for all graphics items.
    QList<QGraphicsItem*> graphItems = items();
    for (QGraphicsItem* graphItem : graphItems)
    {
        // make dynamic type conversion
        ComponentItem* diagComp = dynamic_cast<ComponentItem*>(graphItem);
        // if the item was a diagram component then add it to the list.
        if (diagComp) 
        {
            instances.append(diagComp);
        }
    }
    return instances;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::isLoading()
//-----------------------------------------------------------------------------
bool DesignDiagram::isLoading() const
{
    return loading_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getParent()
//-----------------------------------------------------------------------------
DesignWidget* DesignDiagram::getParent() const
{
    return parent_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::ensureOneTypeSelected()
//-----------------------------------------------------------------------------
void DesignDiagram::ensureOneTypeSelected(QList<QGraphicsItem*> const& items)
{
    QList<QGraphicsItem*> currentlySelectedItems = selectedItems();
    if (currentlySelectedItems.size() > 1)
    {
        // Check if all selected items have the same type.
        int type = getCommonItemType(currentlySelectedItems);
        if (type == -1)
        {
            // If not, deselect those that are in the new selection but no in the old one.
            for (QGraphicsItem* selectedItem : currentlySelectedItems)
            {
                if (!items.contains(selectedItem))
                {
                    selectedItem->setSelected(false);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getCommonType()
//-----------------------------------------------------------------------------
int DesignDiagram::getCommonItemType(QList<QGraphicsItem*> const& items)
{
    if (items.empty())
    {
        return -1;
    }

    int type = items.first()->type();

    for (int i = 1; i < items.size(); i++)
    {
        if (type != items.at(i)->type())
        {
            return -1;
        }
    }

    return type;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::sortByX()
//-----------------------------------------------------------------------------
bool DesignDiagram::sortByX(QGraphicsItem* lhs, QGraphicsItem* rhs)
{
    return lhs->x() < rhs->x();
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::getLayout()
//-----------------------------------------------------------------------------
QSharedPointer<GraphicsColumnLayout> DesignDiagram::getLayout() const
{
    return layout_;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::createInstanceName()
// Forms a unique identifier for a component instance
//-----------------------------------------------------------------------------
QString DesignDiagram::createInstanceName(QString const& baseName)
{
    QSettings settings; // this reads the application settings automatically
    QString format = settings.value("Policies/InstanceNames", "").toString();
    if (format.isEmpty())
    {
        format = "$ComponentName$_$InstanceNumber$";
    }

    // Determine a unique name by using a running number.
    int runningNumber = 0;

    QStringList instanceNames = getUsedInstanceNames();

    QString name = format;
    name.replace("$ComponentName$", baseName);
    name.replace("$InstanceNumber$", QString::number(runningNumber));

    while (instanceNames.contains(name))
    {
        runningNumber++;

        name = format;
        name.replace("$ComponentName$", baseName);
        name.replace("$InstanceNumber$", QString::number(runningNumber));
    }

    return name;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setLayout()
//-----------------------------------------------------------------------------
void DesignDiagram::clearLayout()
{
    layout_.clear();
    layout_ = QSharedPointer<GraphicsColumnLayout>(new GraphicsColumnLayout(this));
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::loadStickyNotes()
//-----------------------------------------------------------------------------
void DesignDiagram::loadStickyNotes()
{
    for (QSharedPointer<VendorExtension> extension : *design_->getVendorExtensions())
    {
        if (extension->type() == "kactus2:note")
        {
            QSharedPointer<Kactus2Group> noteExtension = extension.dynamicCast<Kactus2Group>();
            StickyNote* note = createStickyNote();
            note->setVendorExtension(noteExtension);

            QSharedPointer<StickyNoteAddCommand> cmd = createNoteAddCommand(note);
            cmd->redo();
                
            loadNoteAssociations(note);          
        }
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::createStickyNote()
//-----------------------------------------------------------------------------
StickyNote* DesignDiagram::createStickyNote()
{
    StickyNote* note = new StickyNote();
    connect(note, SIGNAL(beginAssociation(Associable*)), 
        this, SLOT(onBeginAssociation(Associable*)), Qt::UniqueConnection);
    connect(note, SIGNAL(modified(QUndoCommand*)), 
        this, SLOT(onItemModified(QUndoCommand*)), Qt::UniqueConnection);

    return note;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::loadNoteAssociations()
//-----------------------------------------------------------------------------
void DesignDiagram::loadNoteAssociations(StickyNote* note)
{
    QSharedPointer<Kactus2Group> associations = note->getAssociationExtensions();

    for (QSharedPointer<VendorExtension> endpoint : associations->getByType("kactus2:position"))
    {
        QSharedPointer<Kactus2Position> extension = endpoint.dynamicCast<Kactus2Position>();

        QSharedPointer<QUndoCommand> addCommand = createAssociationAddCommand(note, extension);
        addCommand->redo();
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setProtectionForStickyNotes()
//-----------------------------------------------------------------------------
void DesignDiagram::setProtectionForStickyNotes()
{
    for (QGraphicsItem* item : items())
    {
        if (item->type() == StickyNote::Type)
        {
            item->setEnabled(!locked_);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::adHocIdentifier()
//-----------------------------------------------------------------------------
QString DesignDiagram::adHocIdentifier() const
{
    return tr("top-level");
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::updateAssociationCursor()
//-----------------------------------------------------------------------------
void DesignDiagram::updateAssociationCursor(QPointF const& cursorPosition)
{
    bool possibleEndpoint = dynamic_cast<Associable*>(getBaseItemOf(itemAt(cursorPosition, QTransform())));
    if (possibleEndpoint)
    {
        QApplication::changeOverrideCursor(Qt::CrossCursor);
    }
    else
    {
        QApplication::changeOverrideCursor(Qt::ArrowCursor);
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::updateAssociationLine()
//-----------------------------------------------------------------------------
void DesignDiagram::updateAssociationLineDraw(QPointF const& cursorPosition)
{
    QLineF line = associationLine_->line();
    line.setP2(cursorPosition);
    associationLine_->setLine(line);    
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::endAssociation()
//-----------------------------------------------------------------------------
void DesignDiagram::endAssociation(QPointF const& endpoint)
{
    QPointF startingPoint = associationLine_->line().p1();

    endAssociationLineDraw();

    QGraphicsItem* item = getBaseItemOf(itemAt(startingPoint, QTransform()));
    Associable* startItem = dynamic_cast<Associable*>(item);

    QSharedPointer<Kactus2Position> endPointExtension(new Kactus2Position(endpoint));

    QSharedPointer<QUndoCommand> addCommand = createAssociationAddCommand(startItem, endPointExtension);
    addCommand->redo();
    getEditProvider()->addCommand(addCommand);

    QApplication::restoreOverrideCursor();
    setInteractionMode(NORMAL);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::createAssociation()
//-----------------------------------------------------------------------------
QSharedPointer<QUndoCommand> DesignDiagram::createAssociationAddCommand(Associable* startItem, 
    QSharedPointer<Kactus2Position> endPointExtension)
{
    QGraphicsItem* item = getBaseItemOf(itemAt(endPointExtension->position(), QTransform()));
    Associable* endItem = dynamic_cast<Associable*>(item);

    if (!endItem && item && item->parentItem())
    {
        QGraphicsItem* associableParentItem = item->parentItem();
        endItem = dynamic_cast<Associable*>(associableParentItem);
    }

    QSharedPointer<QUndoCommand> addCommand(new QUndoCommand());

    if (canAssociateItems(startItem, endItem))
    {
        Association* association = new Association(startItem, endItem, endPointExtension);
        addCommand = QSharedPointer<AssociationAddCommand>(new AssociationAddCommand(association, this));       
    }

    return addCommand;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::canAssociateItems()
//-----------------------------------------------------------------------------
bool DesignDiagram::canAssociateItems(Associable* startItem, Associable* endItem)
{
    return startItem && endItem && startItem != endItem;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::inAssociationMode()
//-----------------------------------------------------------------------------
bool DesignDiagram::inAssociationMode() const
{
    return interactionMode_ == ASSOCIATE;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::associationEnds()
//-----------------------------------------------------------------------------
bool DesignDiagram::canEndAssociation() const
{
    return inAssociationMode() && associationLine_->line().p1() != associationLine_->line().p2();
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::contextMenuEnabled()
//-----------------------------------------------------------------------------
bool DesignDiagram::contextMenuEnabled() const
{
    return getMode() == MODE_SELECT && interactionMode_ == CONTEXT_MENU;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::inReplaceMode()
//-----------------------------------------------------------------------------
bool DesignDiagram::inReplaceMode() const
{
    return interactionMode_ == REPLACE;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::setInteractionMode()
//-----------------------------------------------------------------------------
void DesignDiagram::setInteractionMode(interactionMode mode)
{
    interactionMode_ = mode;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::createNoteAddCommand()
//-----------------------------------------------------------------------------
QSharedPointer<StickyNoteAddCommand> DesignDiagram::createNoteAddCommand(StickyNote* note)
{
    QSharedPointer<StickyNoteAddCommand> cmd(new StickyNoteAddCommand(note, this, note->pos()));

    connect(cmd.data(), SIGNAL(addVendorExtension(QSharedPointer<VendorExtension>)),
        this, SLOT(onVendorExtensionAdded(QSharedPointer<VendorExtension>)), Qt::UniqueConnection);

    connect(cmd.data(), SIGNAL(removeVendorExtension(QSharedPointer<VendorExtension>)),
        this, SLOT(onVendorExtensionRemoved(QSharedPointer<VendorExtension>)), Qt::UniqueConnection);

    return cmd;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::wheelEvent()
//-----------------------------------------------------------------------------
void DesignDiagram::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (event->modifiers() == Qt::CTRL)
    {
        // Set the zoom level and center the view.
        getParent()->setZoomLevel(getParent()->getZoomLevel() + event->delta() / 12);

        event->accept();
    }
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::endInteraction()
//-----------------------------------------------------------------------------
void DesignDiagram::endInteraction()
{
    if (inAssociationMode())
    {
        endAssociationLineDraw();
    }

    setInteractionMode(NORMAL);
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::endAssociationLineDraw()
//-----------------------------------------------------------------------------
void DesignDiagram::endAssociationLineDraw()
{
    removeItem(associationLine_);
    delete associationLine_;
    associationLine_ = 0;
}

//-----------------------------------------------------------------------------
// Function: DesignDiagram::inOffPageMode()
//-----------------------------------------------------------------------------
bool DesignDiagram::inOffPageMode() const
{
    return interactionMode_ == OFFPAGE;
}

