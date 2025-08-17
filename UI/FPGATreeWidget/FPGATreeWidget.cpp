//-----------------------------------------------------------------------------
// File: FPGATreeWidget.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// FPGATreeWidget is the widget that displays the library content in a tree.
//-----------------------------------------------------------------------------

#include <QObject>
#include <QSettings>
#include <QVBoxLayout>

#include "FPGATreeFilter.h"
#include "FPGATreeModel.h"
//#include "FPGATreeView.h"
#include "FPGATreeWidget.h"



//-----------------------------------------------------------------------------
// Function: FPGATreeWidget::FPGATreeWidget()
//-----------------------------------------------------------------------------
FPGATreeWidget::FPGATreeWidget(FPGAHandler* handler, FPGATreeModel* dataModel, QWidget* parent):
QWidget(parent),
    filter_(new FPGATreeFilter(this)),
    view_(handler, filter_, this),
    dataModel_(dataModel)
{
    QString location = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

//    location = location + "/fpga";

//    dataModel_->setFilters(filters);
    dataModel_->setRootPath(location);

    QStringList filters;
    filters << "v" << "vh" << "sv" << "c" << "h" << "sdc" << "tcl" << "py" << "bit" << "xml" << "txt";

    filter_->setFilters(filters);
    filter_->setSourceModel(dataModel);

    // set view to use ProjectLibraryTreeFilter as source model
    view_.setModel(filter_);
//    view_.setRootIndex(dataModel_->index(location));
    view_.setRootIndex(filter_->mapFromSource(dataModel_->index(location)));

    // 파일 이름 열만 표시
    for (int i = 1; i < dataModel_->columnCount(); ++i) {
        view_.hideColumn(i);
    }

	// the layout to manage the visible items in the widget
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(&view_);
	layout->setContentsMargins(0, 0, 0, 0);

    setupConnections(dataModel_);

	view_.sortByColumn(0, Qt::AscendingOrder);
    view_.setHeaderHidden(true);
}

//-----------------------------------------------------------------------------
// Function: FPGATreeWidget::~FPGATreeWidget()
//-----------------------------------------------------------------------------
FPGATreeWidget::~FPGATreeWidget()
{

}

//-----------------------------------------------------------------------------
// Function: FPGATreeWidget::setupConnections()
//-----------------------------------------------------------------------------
void FPGATreeWidget::setupConnections(FPGATreeModel* dataModel)
{
	// connect the view to the tree model
    connect(&view_, SIGNAL(openFPGA(QModelIndex const&)),
            dataModel, SLOT(onOpenFPGA(QModelIndex const&)), Qt::UniqueConnection);

    // refresh the view to the tree model
    connect(dataModel, SIGNAL(refresh()),
            this, SLOT(onRefresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: FPGATreeModel::onOpenFPGA()
//-----------------------------------------------------------------------------
void FPGATreeWidget::onOpenFPGA(QModelIndex const& index)
{
    emit openFPGA(index);
}

//-----------------------------------------------------------------------------
// Function: FPGATreeModel::onRefresh()
//-----------------------------------------------------------------------------
void FPGATreeWidget::onRefresh()
{
    QString location = QSettings().value(QStringLiteral("Project/DefaultLocation")).toString();

//    location = location + "/fpga";

//    dataModel_->setFilters(filters);
    dataModel_->setRootPath(location);

    QStringList filters;
    filters << "v" << "vh" << "sv" << "c" << "h" << "sdc" << "tcl" << "py" << "bit" << "xml" << "txt";

    filter_->setFilters(filters);
    filter_->setSourceModel(dataModel_);

    // set view to use ProjectLibraryTreeFilter as source model
    view_.setModel(filter_);
//    view_.setRootIndex(dataModel_->index(location));
    view_.setRootIndex(filter_->mapFromSource(dataModel_->index(location)));

    // 파일 이름 열만 표시
    for (int i = 1; i < dataModel_->columnCount(); ++i) {
        view_.hideColumn(i);
    }

    view_.sortByColumn(0, Qt::AscendingOrder);
    view_.setHeaderHidden(true);
}
