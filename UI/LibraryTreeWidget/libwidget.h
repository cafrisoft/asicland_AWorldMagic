#ifndef LIBWIDGET_H
#define LIBWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include "iptreeview.h"
#include "iotreeview.h"
#include <QXmlStreamReader>
#include <QStandardItemModel>

#if LIBWIDGET_20231213 == 1
#include <IPXACTmodels/common/VLNV.h>

#include <UI/LibraryTreeWidget/LibraryTreeView.h>
#endif

#if LIBWIDGET_20231213 == 1
class LibraryInterface;
class LibraryTreeModel;
class LibraryTreeFilter;
//class LibraryTreeView;
#endif

class LibWidget : public QWidget
{
    Q_OBJECT
public:
#if LIBWIDGET_20231213 == 0
    explicit LibWidget(QWidget *parent = nullptr);
#else
    explicit LibWidget(LibraryInterface* handler, LibraryTreeModel* dataModel, QWidget *parent = nullptr);
#endif

private:
#if LIBWIDGET_20231213 == 1
    //! The filter between the view and data model.
    LibraryTreeFilter* filter_;

    //! The table view to display the search results.
    LibraryTreeView view_;

    //! The model for the filter.
    LibraryTreeModel* dataModel_;
#endif

    IPTreeView* ipTreeView;
    IOTreeView* ioTreeView;
    QFileSystemModel* ipModel;
    QFileSystemModel* ioModel;

signals:

private slots:
    void handleIOTreeViewDoubleClick(const QModelIndex &index) ;
    void handleIPTreeViewDoubleClick(const QModelIndex &index) ;

private:
    QFileSystemModel* createFilteredModel(const QStringList& filters);

#if LIBWIDGET_20231213 == 1
    //! No copying.
//	LibraryTreeWidget(const LibraryTreeWidget& other);

    //! No assignment.
//	LibraryTreeWidget& operator=(const LibraryTreeWidget& other);

    //! Set up the connections between child widgets.
    void setupConnections(LibraryTreeModel* dataModel);
#endif

public:
    void onXmlButtonClicked();
    void onXmlIoButtonClicked();
    void populateTreeFromIPXML(QStandardItem *parent, QXmlStreamReader &xml);
    void populateTreeFromIOXML(QStandardItem *parent, QXmlStreamReader &xml);
};

#endif // LIBWIDGET_H
