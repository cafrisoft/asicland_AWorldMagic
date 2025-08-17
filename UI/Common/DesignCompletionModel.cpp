//-----------------------------------------------------------------------------
// File: DesignCompletionModel.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Esko Pekkarinen
// Date: 13.01.2015
//
// Description:
// Completion model for parameter names in a design.
//-----------------------------------------------------------------------------

#include <QIcon>

#include <UI/ComponentEditor/parameters/ComponentParameterColumns.h>

#include "DesignCompletionModel.h"


//-----------------------------------------------------------------------------
// Function: DesignCompletionModel::DesignCompletionModel()
//-----------------------------------------------------------------------------
DesignCompletionModel::DesignCompletionModel(QSharedPointer<ParameterFinder> topComponentFinder, 
    QSharedPointer<ParameterFinder> parameterFinder, QObject *parent):
    ComponentParameterModel(parameterFinder, parent),
        topFinder_(topComponentFinder)
{

}

//-----------------------------------------------------------------------------
// Function: DesignCompletionModel::~DesignCompletionModel()
//-----------------------------------------------------------------------------
DesignCompletionModel::~DesignCompletionModel()
{

}

//-----------------------------------------------------------------------------
// Function: DesignCompletionModel::data()
//-----------------------------------------------------------------------------
QVariant DesignCompletionModel::data(QModelIndex const& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DecorationRole && index.column() == ComponentParameterColumns::NAME)
    {
        QString id = data(index.sibling(index.row(), ComponentParameterColumns::ID), Qt::DisplayRole).toString();
        if (topFinder_->hasId(id))
        {
            return QIcon(":resources/Res/external.png");
        }
        else
        {
            return QVariant();
        }
    }

    return ComponentParameterModel::data(index, role);
}
