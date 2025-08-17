//-----------------------------------------------------------------------------
// File: FPGATreeFilter.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// The class that does the filtering and sorting of items for the FGPA tree view.
//-----------------------------------------------------------------------------

#include <QFileSystemModel>
#include <QFileInfo>

#include "FPGATreeFilter.h"


//-----------------------------------------------------------------------------
// Function: FPGATreeFilter::FPGATreeFilter()
//-----------------------------------------------------------------------------
FPGATreeFilter::FPGATreeFilter(QObject *parent):
QSortFilterProxyModel(parent)
{

}

//-----------------------------------------------------------------------------
// Function: FPGATreeFilter::filterAcceptsRow()
//-----------------------------------------------------------------------------
bool FPGATreeFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex itemIndex = sourceModel()->index(sourceRow, 0, sourceParent);

//    return true;

    QString filePath = sourceModel()->data(itemIndex, QFileSystemModel::FilePathRole).toString();

    QFileInfo fileInfo(filePath);

    // 디렉토리는 항상 수락
    if (fileInfo.isDir())
        return true;

    // 파일 확장자가 필터 리스트에 있는 경우 수락
    return m_filters.contains(fileInfo.suffix(), Qt::CaseInsensitive);
}

//-----------------------------------------------------------------------------
// Function: FPGATreeFilter::lessThan()
//-----------------------------------------------------------------------------
bool FPGATreeFilter::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QFileSystemModel *model = qobject_cast<QFileSystemModel *>(sourceModel());

    QFileInfo leftFileInfo = model->fileInfo(left);
    QFileInfo rightFileInfo = model->fileInfo(right);

    // 폴더를 먼저 정렬
    if (leftFileInfo.isDir() && !rightFileInfo.isDir()) {
        return true;
    } else if (!leftFileInfo.isDir() && rightFileInfo.isDir()) {
        return false;
    }

    // 폴더와 파일이 동일한 경우 이름으로 정렬
//    return leftFileInfo.fileName().toLower() < rightFileInfo.fileName().toLower();
    return QString::compare(leftFileInfo.fileName().toLower(), rightFileInfo.fileName().toLower()) < 0;
}

void FPGATreeFilter::setFilters(const QStringList &filters)
{
    m_filters = filters;
}
