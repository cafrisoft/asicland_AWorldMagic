#include "awelementwindow.h"

#include <KactusAPI/LibraryHandler.h>

//#include <UI/LibraryTreeWidget/LibraryTreeWidget.h>
//#include <UI/HierarchyWidget/hierarchywidget.h>

#if AWELEMENTWINDOW_20231213 == 1
#include <UI/LibraryTreeWidget/libwidget.h>
#endif


//#include <AWProjectPan.h>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "projecttreewidget.h"
#include <QResizeEvent>

#if AWELEMENTWINDOW_20231213 == 0
#include "libwidget.h"
#endif


AWElementWindow::AWElementWindow(LibraryHandler* libraryHandler, QWidget *parent)
    : QWidget{parent},
      libraryHandler_(libraryHandler)
{
/*
    // Create List and Stacked Widget
    QListWidget* listWidget = new QListWidget(this);
    //-----------------------------------------
    // Create Items for the ListWidget and add custom QWidget to each item
    QListWidgetItem* item1 = new QListWidgetItem;
    item1->setSizeHint(QSize(0, 25));  // 0 width, 200 height
    listWidget->addItem(item1);
    QPushButton *button1 = new QPushButton("Project");
    listWidget->setItemWidget(item1, button1);

    //_____________________________

    // Create an empty item with a label widget
    QListWidgetItem* emptyItem = new QListWidgetItem;
    emptyItem->setSizeHint(QSize(100, 400));
    listWidget->addItem(emptyItem);

    ProjectTreeWidget *projectpan = new ProjectTreeWidget(QDir::currentPath());
    //AWProjectPan *projectpan = new AWProjectPan();
    //projectpan->show(); // make projectpan visible
    listWidget->setItemWidget(emptyItem, projectpan);
    //projectpan->raise();


    //-----------------------------------------

    QListWidgetItem* item2 = new QListWidgetItem;
    listWidget->addItem(item2);
    QPushButton *button2 = new QPushButton("Library");
    listWidget->setItemWidget(item2, button2);

    // Create an empty item with a label widget
    QListWidgetItem* emptyItemLibrary = new QListWidgetItem;
    listWidget->addItem(emptyItemLibrary);
    QLabel *labelLibs = new QLabel("");
    listWidget->setItemWidget(emptyItemLibrary, labelLibs);

    // Set the size of the emptyItem
    emptyItemLibrary->setSizeHint(QSize(0, 200));  // 0 width, 200 height

    //-----------------------------------------

    QListWidgetItem* item3 = new QListWidgetItem;
    listWidget->addItem(item3);
    QPushButton *button3 = new QPushButton("Preview");
    listWidget->setItemWidget(item3, button3);


    // Layout for the widgets
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(listWidget);

    layout->setContentsMargins(0, 0, 0, 0); // remove margins
    layout->setSpacing(0); // remove spacing

    // Set layout to this widget
    setContentsMargins(0, 0, 0, 0); // remove margins
   // setSpacing(0); // remove spacing
    this->setLayout(layout);

*/

    setMinimumSize(800, 600);  // set minimum size of the window
    //setStyleSheet("background-color: yellow;");

    QSplitter *splitter = new QSplitter(Qt::Vertical, this); // Create a splitter



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //. 2023-11-27 phg 프로젝트 윈도우 테스트
//    hierarchyWidget_ = new HierarchyWidget(libraryHandler_, libraryHandler_->getHierarchyModel(), this);
//    hierarchyWidget_->setMinimumWidth(200); // setMinimumWidth를 사용
//    splitter->addWidget(hierarchyWidget_);


//    treeWidget_ = new LibraryTreeWidget(libraryHandler_, libraryHandler_->getTreeModel(), this);
//    treeWidget_->setMinimumWidth(200); // setMinimumWidth를 사용
//    splitter->addWidget(treeWidget_);

    ProjectTreeWidgetEx *projectpan = new ProjectTreeWidgetEx("" /*QDir::currentPath()*/);
    projectpan->setMinimumHeight(200); // Set minimum height of the widget
    //projectpan->setStyleSheet("background-color: green;"); // Set background color
    splitter->addWidget(projectpan);
    //projectpan->raise();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if AWELEMENTWINDOW_20231213 == 0
    LibWidget *libraryWidget = new LibWidget();
#else
    LibWidget *libraryWidget = new LibWidget(libraryHandler_, libraryHandler_->getTreeModel(), this);;
#endif

    libraryWidget->setMinimumHeight(200); // Set minimum height of the widget
  //  libraryWidget->setStyleSheet("background-color: red;"); // Set background color
    splitter->addWidget(libraryWidget);



    QWidget *previewWidget = new QWidget();
    previewWidget->setMinimumHeight(200); // Set minimum height of the widget
    previewWidget->setStyleSheet("background-color: blue;"); // Set background color
    splitter->addWidget(previewWidget);

    // Make the last widget fill the remaining space
    splitter->setStretchFactor(2, 1);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 5); // Set padding to 0
    layout->addWidget(splitter);  // add the splitter to the layout

    setLayout(layout);  // set the layout of the window
}

void AWElementWindow::resizeEvent(QResizeEvent *event) {
    layout()->setGeometry(QRect(QPoint(0, 0), event->size()));  // resize the layout
}
