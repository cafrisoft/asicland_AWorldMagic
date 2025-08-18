//-----------------------------------------------------------------------------
// File: FPGATreeView.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// The widget to display the FPGA in a tree.
//-----------------------------------------------------------------------------

#include <QHeaderView>
#include <QMimeData>
#include <QMouseEvent>
#include <QTreeView>
#include <QModelIndex>

#include <AWorldMagicAPI/FPGAHandler.h>

#include "FPGATreeFilter.h"

#include "FPGATreeView.h"

//-----------------------------------------------------------------------------
// Function: FPGATreeView::FPGATreeView()
//-----------------------------------------------------------------------------
FPGATreeView::FPGATreeView(FPGAHandler* handler, FPGATreeFilter* filter, QWidget* parent):
    QTreeView(parent),
    handler_(handler),
    filter_(filter)
{
    // 열 헤더 설정
//    header()->setSortIndicator(0, Qt::AscendingOrder); // 첫 번째 열을 오름차순으로 정렬
//    header()->setSortIndicatorShown(true);
//    header()->setSectionsClickable(true);

    // QTreeView의 정렬 기능 활성화
    setSortingEnabled(true);

    // 기본 정렬 설정
    sortByColumn(0, Qt::AscendingOrder); // 첫 번째 열(이름)을 기준으로 오름차순 정렬

    setupActions();
}


//-----------------------------------------------------------------------------
// Function: FPGATreeView::setupActions()
//-----------------------------------------------------------------------------
void FPGATreeView::setupActions()
{
}

//-----------------------------------------------------------------------------
// Function: FPGATreeView::mouseDoubleClickEvent()
//-----------------------------------------------------------------------------
void FPGATreeView::mouseDoubleClickEvent(QMouseEvent* event)
{
    // accept the event so it is not passed forwards
    event->accept();

    QModelIndex current = currentIndex();
    if (!current.isValid())
    {
        return;
    }

    emit openFPGA(filter_->mapToSource(current));

    // let the default handler process the event
    QTreeView::mouseDoubleClickEvent(event);
}

