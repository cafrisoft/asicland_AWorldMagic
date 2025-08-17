/*
 *      Created on: 11.6.2024
 *      Author: P.H.G
 *      Filename: FPGATreeModel.cpp
 *      Project: AWM
 *      Description :
 *          The model that contains the FPGA Items to display.
 */

//#include <QBrush>
//#include <QIcon>

#include "FPGATreeModel.h"


//-----------------------------------------------------------------------------
// Function: FPGATreeModel::FPGATreeModel()
//-----------------------------------------------------------------------------
FPGATreeModel::FPGATreeModel(QObject* parent):
    QFileSystemModel (parent)
{
}

//-----------------------------------------------------------------------------
// Function: FPGATreeModel::~FPGATreeModel()
//-----------------------------------------------------------------------------
FPGATreeModel::~FPGATreeModel()
{
}

//-----------------------------------------------------------------------------
// Function: FPGATreeModel::onOpenFPGA()
//-----------------------------------------------------------------------------
void FPGATreeModel::onOpenFPGA(QModelIndex const& index)
{
    if (!index.isValid())
    {
        return;
    }

    QString filePath = this->filePath(index);

    if (!this->isDir(index)) {
        emit openFPGA(filePath);
    }
}

//-----------------------------------------------------------------------------
// Function: FPGATreeModel::onResetModel()
//-----------------------------------------------------------------------------
void FPGATreeModel::onResetModel()
{
    emit refresh();
}
