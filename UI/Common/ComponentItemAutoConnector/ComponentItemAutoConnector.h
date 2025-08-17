//-----------------------------------------------------------------------------
// File: ComponentItemAutoConnector.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 23.01.2019
//
// Description:
// Automatically connects the ports and bus interfaces of two component items.
//-----------------------------------------------------------------------------

#ifndef COMPONENTITEMAUTOCONNECTOR_H
#define COMPONENTITEMAUTOCONNECTOR_H

#include <UI/Common/ComponentItemAutoConnector/AutoConnectorItem.h>

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QTabWidget>

class ComponentItem;
class AutoConnector;
class Component;
class TableItemMatcher;
class TableAutoConnector;

//-----------------------------------------------------------------------------
//! Automatically connects the ports and bus interfaces of two component items.
//-----------------------------------------------------------------------------
class ComponentItemAutoConnector : public QDialog
{
    Q_OBJECT

public:

    //! Holder for item container data.
    struct AutoContainer
    {
        //! Component containing the items.
        QSharedPointer<Component> component_;

        //! Name of the container.
        QString name_;

        //! Visible name of the container.
        QString visibleName_;
    };

    //! Holder for required auto connector tools.
    struct TableTools 
    {
        //! Table Item matcher.
        TableItemMatcher* itemMatcher_;

        //! Table auto connector.
        TableAutoConnector* tableConnector_;
    };

    /*!
     *  The constructor.
     *
     *      @param [in] firstContainer      Data for the first container.
     *      @param [in] secondContainer     Data for the second container.
     *      @param [in] busTableTools       Auto connector tools for the bus interface item connector.
     *      @param [in] portTableTools      Auto connector tools for the port item connector.
     *      @param [in] secondItemType      Type of the second item container.
     *      @param [in] parent              The parent of this widget.
     */
    ComponentItemAutoConnector(AutoContainer const& firstContainer, AutoContainer const& secondContainer,
        TableTools const& busTableTools, TableTools const& portTableTools,
        AutoConnectorItem::ContainerType secondItemType, QWidget* parent = 0);

    /*!
     *  Destructor.
     */
    ~ComponentItemAutoConnector();

    /*!
     *  Get the list of connected items.
     *
     *      @return A list of item pairs.
     */
    QVector<QPair<AutoConnectorItem*, AutoConnectorItem*> > getConnectedItems() const;

    // No copying. No assignments.
    ComponentItemAutoConnector(ComponentItemAutoConnector const& rhs) = delete;
    ComponentItemAutoConnector& operator=(ComponentItemAutoConnector const& rhs) = delete;

private slots:

    /*!
     *  Connect the selected items from the item lists.
     */
    void connectSelectedItems();

    /*!
     *  Clear the connected items from the currently selected table.
     */
    void clearConnections();

    /*!
     *  Connect the items of the currently selected table automatically.
     */
    void autoConnectItems();

private:

    /*!
     *  Setup the layout.
     */
    void setupLayout();

    /*!
     *  Create connected item pairs from the selected connector table.
     *
     *      @param [in] connector   The selected connector table.
     *      @param [in] itemType    Type for the items.
     *
     *      @return List of connected item pairs.
     */
    QVector<QPair<AutoConnectorItem*, AutoConnectorItem*> > createItemPairs(AutoConnector* connector,
        AutoConnectorItem::ItemType itemType) const;

    /*!
     *  Create the intro widget.
     *
     *      @param [in] introName   Header for the intro.
     *      @param [in] introText   Text body for the intro.
     *
     *      @return The intro widget.
     */
    QWidget* setupIntroWidget(QString const& introName, QString const& introText) const;

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Name of the first component item.
    QString firstItemName_;

    //! Name of the second component item.
    QString secondItemName_;

    //! Button for connecting two items.
    QPushButton* connectButton_;

    //! Button for connecting items automatically.
    QPushButton* autoConnectButton_;

    //! Button for clearing the connected items table.
    QPushButton* clearButton_;

    //! Connection editor for bus interfaces.
    AutoConnector* busInterfaceConnector_;

    //! Connection editor for ports.
    AutoConnector* portConnector_;

    //! Holds the different editors.
    QTabWidget tabs_;

    //! Type of the second item container.
    AutoConnectorItem::ContainerType secondContainerType_;
};

//-----------------------------------------------------------------------------

#endif // COMPONENTITEMAUTOCONNECTOR_H
