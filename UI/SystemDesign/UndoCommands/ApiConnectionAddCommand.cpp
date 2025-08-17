//-----------------------------------------------------------------------------
// File: ApiConnectionAddCommand.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 16.02.2016
//
// Description:
// Undo add commands for API connections.
//-----------------------------------------------------------------------------

#include "ApiConnectionAddCommand.h"

#include <IPXACTmodels/Design/Design.h>
#include <IPXACTmodels/Design/ComponentInstance.h>

#include <UI/SystemDesign/ApiGraphicsConnection.h>
#include <UI/SystemDesign/SystemDesignDiagram.h>
#include <UI/SystemDesign/SWPortItem.h>

#include <UI/Common/graphicsItems/ConnectionEndpoint.h>
#include <UI/Common/graphicsItems/ComponentItem.h>


//-----------------------------------------------------------------------------
// Function: ApiConnectionAddCommand::ApiConnectionAddCommand()
//-----------------------------------------------------------------------------
ApiConnectionAddCommand::ApiConnectionAddCommand(QGraphicsScene* scene, ApiGraphicsConnection* connection,
                                                 QSharedPointer<Design> containingDesign, QUndoCommand* parent):
QUndoCommand(parent),
connection_(connection),
scene_(scene),
del_(false),
containingDesign_(containingDesign)
{

}

//-----------------------------------------------------------------------------
// Function: ApiConnectionAddCommand::~ApiConnectionAddCommand()
//-----------------------------------------------------------------------------
ApiConnectionAddCommand::~ApiConnectionAddCommand()
{
    if (del_)
    {
        delete connection_;
    }
}

//-----------------------------------------------------------------------------
// Function: ApiConnectionAddCommand::undo()
//-----------------------------------------------------------------------------
void ApiConnectionAddCommand::undo()
{
    scene_->removeItem(connection_);
    connection_->setSelected(false);

    connection_->disconnectEnds();

    QList<QSharedPointer<ApiInterconnection> > apiInterconnections = containingDesign_->getApiConnections();
    apiInterconnections.removeAll(connection_->getApiInterconnection());
    containingDesign_->setApiConnections(apiInterconnections);

    containingDesign_->removeRoute(connection_->getRouteExtension());

    del_ = true;
}

//-----------------------------------------------------------------------------
// Function: ApiConnectionAddCommand::redo()
//-----------------------------------------------------------------------------
void ApiConnectionAddCommand::redo()
{
    if (!scene_->items().contains(connection_))
    {
        scene_->addItem(connection_);
    }

    if (connection_->connectEnds())
    {
        ConnectionEndpoint* startPoint = connection_->endpoint1();
        ConnectionEndpoint* endPoint = connection_->endpoint2();

        if (startPoint->getType() == ConnectionEndpoint::ENDPOINT_TYPE_UNDEFINED)
        {
            changePortItemFromComToApi(startPoint);
        }
        else if (endPoint->getType() == ConnectionEndpoint::ENDPOINT_TYPE_UNDEFINED)
        {
            changePortItemFromComToApi(endPoint);
        }

        QList<QSharedPointer<ApiInterconnection> > apiInterconnections = containingDesign_->getApiConnections();
        apiInterconnections.append(connection_->getApiInterconnection());
        containingDesign_->setApiConnections(apiInterconnections);

        containingDesign_->addRoute(connection_->getRouteExtension());
    }

    del_ = false;
}

//-----------------------------------------------------------------------------
// Function: ApiConnectionAddCommand::changePortItemFromApiToCom()
//-----------------------------------------------------------------------------
void ApiConnectionAddCommand::changePortItemFromComToApi(ConnectionEndpoint* endPoint)
{
    SWPortItem* endPointPort = dynamic_cast<SWPortItem*>(endPoint);
    if (endPointPort)
    {
        QSharedPointer<ComponentInstance> containingInstance = endPoint->encompassingComp()->getComponentInstance();

        QMap<QString, QPointF> comPositions = containingInstance->getComInterfacePositions();

        if (comPositions.contains(endPoint->name()) && comPositions.value(endPoint->name()) == endPoint->pos())
        {
            containingInstance->removeComInterfacePosition(endPoint->name());
        }

        containingInstance->updateApiInterfacePosition(endPoint->name(), endPoint->pos());
    }
}
