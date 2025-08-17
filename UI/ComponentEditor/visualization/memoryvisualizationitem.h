//-----------------------------------------------------------------------------
// File: memoryvisualizationitem.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 15.10.2012
//
// Description:
// A base class for graphics items to visualize memory objects
//-----------------------------------------------------------------------------

#ifndef MEMORYVISUALIZATIONITEM_H
#define MEMORYVISUALIZATIONITEM_H

#include <UI/Common/graphicsItems/expandableitem.h>
#include <UI/Common/graphicsItems/graphicsexpandcollapseitem.h>

#include <QMultiMap>
#include <QGraphicsSceneMouseEvent>

class MemoryGapItem;
class ExpressionParser;
//-----------------------------------------------------------------------------
//! A base class for graphics items to visualize memory objects.
//-----------------------------------------------------------------------------
class MemoryVisualizationItem : public ExpandableItem
{
	Q_OBJECT

public:

	//! The indentation of each child under its parent.
	enum Indentation 
    {
		CHILD_INDENTATION = GraphicsExpandCollapseItem::SIDE
	};

	/*!
     *  The constructor.
	 *
     *      @param [in] expressionParser    The used expression parser.
	 *      @param [in] parent              The owner of the item.
	 */
	MemoryVisualizationItem(QSharedPointer<ExpressionParser> expressionParser, QGraphicsItem* parent = 0);
	
   
    //! The destructor.
	virtual ~MemoryVisualizationItem() = default;
    
    virtual  QRectF itemTotalRect() const override final;

    //! No copying.
    MemoryVisualizationItem(const MemoryVisualizationItem& other) = delete;
    MemoryVisualizationItem& operator=(const MemoryVisualizationItem& other) = delete;

    //! Updates the labels and tooltip for the item.
    virtual void updateDisplay() = 0;

	/*!
     *  Get the offset of the item. 
	 *
	 *      @return The offset of the item from the parent item's base address.
	 */
	virtual quint64 getOffset() const = 0;

	/*!
     *  Get the last address contained in the item.
	 *
	 *      @return The last address.
	 */
	virtual quint64 getLastAddress() const = 0;

	/*!
     *  Get the bit width of the item.
	 * 
	 *      @return The bit width of the item.
	 */
	virtual int getBitWidth() const = 0;

	/*!
     *  Get number of bits the addressable unit contains.
	 *
	 *      @return The size of least addressable unit.
	 */
	virtual unsigned int getAddressUnitSize() const = 0;

    //! Re-layouts the child items.
    virtual void redoChildLayout();

	/*!
     *  Add a child visualization item for this item.
	 *
	 *       @param [in] childItem Pointer to the child to add.
	 */
	virtual void addChild(MemoryVisualizationItem* childItem);

    /*!
     *  Remove a child visualization item from this item.
	 *
	 *       @param [in] childItem Pointer to the child to remove.
	 */
	virtual void removeChild(MemoryVisualizationItem* childItem);

    int getChildCount() const;

    /*!
     *  Set the width for the item.
	 *
	 *       @param [in] width The new width of the item.
	 */
	virtual void setWidth(qreal width);

	/*!
     *  The bounding rect of the item.
	 *
	 *      @return The rectangle that bounds the item and possible sub items.
	 */
	virtual QRectF boundingRect() const;

	/*!
     *  Sets the first non-overlapping address to display.
	 *
	 *       @param [in] The first address to set.
	 */
    virtual void setDisplayOffset(quint64 const& address);

	/*!
     *  Get the first non-overlapping address of the item.
	 *
	 *      @return The first non-overlapping address.
	 */
    virtual quint64 getDisplayOffset();

	/*!
     *  Sets the last non-overlapping address to display.
	 *
	 *       @param [in] The last address to set.
	 */
    virtual void setDisplayLastAddress(quint64 const& address);

    /*!
     *  Get the last non-overlapping address of the item.
     *
     *      @return The last non-overlapping address.
     */
    virtual quint64 getDisplayLastAddress();

    //! Set the item into conflicted (overlapping memory) state.
    virtual void setConflicted(bool conflicted);

    /*!
     *  Checks if the item is conflicted (overlapping with others).
     *
     *      @return True, if the item is conflicted, otherwise false.
     */
    bool isConflicted() const;

signals:

	//! Emitted when this item's editor should be displayed.
	void selectEditor();

protected slots:

    
    /*!
     *  Repositions the child items within this item.
     */
    virtual void repositionChildren();

protected:
   
    /*!
     *  Parse a given expression.
     *
     *      @param [in] expression  The expression to be parsed.
     */
    quint64 parseExpression(QString const& expression) const;

    /*!
     *  Get the used expression parser.
     *
     *      @return Pointer to the expression parser.
     */
    QSharedPointer<ExpressionParser> getExpressionParser() const;

    //! Shows the expand/collapse icon if the item has any children. Otherwise the icon is hidden.
    void showExpandIconIfHasChildren();

    //! Update the offsets of the child items in the map and fills the empty gaps between them.
    void updateChildMap();

    //! Removes current gaps between child items and re-sorts items by offset.
    virtual void removeGapsAndSortChildren();

    /*!
     * Fills the gaps between child items with gap items.
     */
    virtual void fillGapsBetweenChildren();

    //! Mark all invalid children outside item boundaries.
    virtual void markConflictingChildren();
 
	//! Handler for mouse press events
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

	/*!
     *  Set text to the top left corner.
	 *
	 * The function groups the hexadecimal digit into groups of 4 digits.
	 *
	 *       @param [in] text The text to display in the corner.
	 */
	virtual void setTopLabelText(QString const& text);

	/*!
     *  Set the address to be shown on the top left corner.
	 *
	 *       @param [in] address The address to be shown in hexadecimal form.
	 */
	virtual void setTopLabelText(quint64 address);

    /*!
     *  Converts an address to hexadecimal string.
     *
     *      @param [in] address   The address to convert.
     *
     *      @return The hexadecimal representation.
     */
    QString toHexString(quint64 address);

	/*!
     *  Set text to the bottom left corner.
	 *
	 *  The function groups the hexadecimal digit into groups of 4 digits.
	 *
	 *       @param [in] text The text to display in the corner.
	 */
	virtual void setBottomLabelText(QString const& text);

	/*!
     *  Set the address to be shown on the bottom left corner.
	 *
	 *       @param [in] address The address to be shown in hexadecimal form.
	 */
	virtual void setBottomLabelText(quint64 address);

	//! Contains the child memory items. The offset of the child is the key.
	QMultiMap<quint64, MemoryVisualizationItem*> childItems_;

    //! The first address of the item.
    quint64 firstAddress_ = 0;

    //! The last address of the item.
    quint64 lastAddress_ = 0;

private:
    
    /*!
     *  Checks if there is empty memory space between the given child and the last known used address.
     *
     *      @param [in] current             The currently iterated child.
     *      @param [in] lastAddressInUse    The last known used address.
     *
     *      @return True, if there is empty space, otherwise false.
     */
    bool emptySpaceBeforeChild(MemoryVisualizationItem const* current, quint64 lastAddressInUse) const;

    /*!
     *  Creates a new child for representing a free memory slot.
     *
     *      @param [in] offset          The offset of the free memory slot.
     *      @param [in] lastAddress     The last address of the free memory slot.
     *
     *      @return The created child item.
     */
    QMultiMap<quint64, MemoryVisualizationItem*>::iterator createMemoryGap(quint64 offset, quint64 lastAddress);

    /*!
     *  Groups a given address text to groups of four digits.
     *
     *      @param [in] text   The address to group.
     *
     *      @return The grouped address text.
     */
    QString groupByFourDigits(QString const& text) const;

    //! comparison function for two equal offsets.
    static bool compareItems(const MemoryVisualizationItem* s1, const MemoryVisualizationItem* s2);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

	//! Conflicted state. Item is conflicted if it overlaps with other items.
    bool conflicted_ = false;

    //! Width for child items.
    qreal childWidth_ = VisualizerItem::DEFAULT_WIDTH;

    //! The used expression parser.
    QSharedPointer<ExpressionParser> expressionParser_;

};

#endif // MEMORYVISUALIZATIONITEM_H
