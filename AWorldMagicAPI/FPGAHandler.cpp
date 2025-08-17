//-----------------------------------------------------------------------------
// File: FPGAHandler.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// FPGAHandler is the class that implements the services to manage the IP-XACT library.
//-----------------------------------------------------------------------------

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

//#include <KactusAPI/Project.h>

#include <KactusAPI/MessageMediator.h>

//#include <KactusAPI/utils.h>

//#include <KactusAPI/TagManager.h>

#include "FPGAHandler.h"


// Singleton instance.
FPGAHandler* FPGAHandler::FPGAHandler_instance_ = nullptr;

//-----------------------------------------------------------------------------
// Function: FPGAHandler::getInstance()
//-----------------------------------------------------------------------------
FPGAHandler* FPGAHandler::getInstance()
{
    if (FPGAHandler_instance_ == nullptr)
    {
        initialize(nullptr, nullptr);
    }

    return FPGAHandler::FPGAHandler_instance_;
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::initialize()
//-----------------------------------------------------------------------------
void FPGAHandler::initialize(MessageMediator* messageChannel, QObject* parent)
{
    if (FPGAHandler_instance_ == nullptr)
    {
        FPGAHandler::FPGAHandler_instance_ = new FPGAHandler(messageChannel, parent);
    }
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::FPGAHandler()
//-----------------------------------------------------------------------------
FPGAHandler::FPGAHandler(MessageMediator* messageChannel, QObject* parent):
QObject(parent),
    messageChannel_(messageChannel),
    loader_(messageChannel),
    treeModel_(new FPGATreeModel(this))
{
    // create the connections between models and library handler
    syncronizeModels();
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::searchForFPGAFiles()
//-----------------------------------------------------------------------------
void FPGAHandler::searchForFPGAFiles()
{
    loadFPGAFIles();

    resetModels();
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::getModel()
//-----------------------------------------------------------------------------
FPGATreeModel* FPGAHandler::getModel()
{
    return treeModel_;
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::onOpenFPGA()
//-----------------------------------------------------------------------------
void FPGAHandler::onOpenFPGA(QString const& filePath)
{
    emit openFPGA(filePath);
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::syncronizeModels()
//-----------------------------------------------------------------------------
void FPGAHandler::syncronizeModels()
{
    // signals from tree model to library handler
    connect(treeModel_, SIGNAL(openFPGA(QString const&)),
        this, SLOT(onOpenFPGA(QString const&)), Qt::UniqueConnection);

    connect(this, SIGNAL(resetModel()),
        treeModel_, SLOT(onResetModel()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::loadFPGAFIles()
//-----------------------------------------------------------------------------
void FPGAHandler::loadFPGAFIles()
{
//    messageChannel_->showStatusMessage(tr("Scanning FPGA. Please wait..."));

    emit resetModel();

//    messageChannel_->showStatusMessage(tr("Ready."));
}

//-----------------------------------------------------------------------------
// Function: FPGAHandler::resetModels()
//-----------------------------------------------------------------------------
void FPGAHandler::resetModels()
{
//    messageChannel_->showStatusMessage(tr("Updating FPGA view. Please wait..."));

    emit resetModel();

//    messageChannel_->showStatusMessage(tr("Ready."));
}
