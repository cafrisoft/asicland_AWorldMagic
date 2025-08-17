#include "libwidget.h"
#include "iptreeview.h"
#include "iotreeview.h"
#include <QStandardItem>
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTreeView>
#include <QMessageBox>

#if LIBWIDGET_20231213 == 1
#include <KactusAPI/LibraryTreeModel.h>

//#include "LibraryTreeView.h"
#endif

#include "Constructor/basexmlmgr.h"
#include "mymainwindow.h"
#include "singleton.h"

QString getFileNameFromPath(const QString& fullPath) {
    QFileInfo fileInfo(fullPath);
    return fileInfo.fileName();
}


#if LIBWIDGET_20231213 == 0
LibWidget::LibWidget(QWidget *parent)
    : QWidget{parent}
#else
LibWidget::LibWidget(LibraryInterface* handler, LibraryTreeModel* dataModel, QWidget *parent)
    : QWidget{parent},
      filter_(new LibraryTreeFilter(handler, this)),
      view_(handler, filter_, this),
      dataModel_(dataModel)
#endif
{

#if LIBWIDGET_20231213 == 1
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0); // remove margins

    // Titlebar
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);

    titleLayout->setSpacing(0);
    QLabel* title = new QLabel("Library");
    QPushButton* menuButton = new QPushButton(QIcon("path_to_icon.png"), "");
    menuButton->setFixedSize(24, 24);  // Set size of menuButton to 16x16 pixels
    titleLayout->addWidget(title);
    titleLayout->addWidget(menuButton);
    mainLayout->addLayout(titleLayout);

    // Input TextBox and Search Button
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setContentsMargins(0, 0, 0, 0);
    searchLayout->setSpacing(0);
    QLineEdit* searchBox = new QLineEdit();
    QPushButton* searchButton = new QPushButton(QIcon("path_to_search_icon.png"), "");
    searchButton->setFixedSize(24, 24);  // Set size of searchButton to 16x16 pixels
    searchLayout->addWidget(searchBox);
    searchLayout->addWidget(searchButton);

    //.. temp for test.............
    QPushButton* xmlIPButton = new QPushButton("ip");
    xmlIPButton->setFixedSize(24, 24);  // Set size of searchButton to 16x16 pixels
    connect(xmlIPButton, &QPushButton::clicked, this, &LibWidget::onXmlButtonClicked);
    searchLayout->addWidget(xmlIPButton);

    QPushButton* xmlIOButton = new QPushButton("io");
    xmlIOButton->setFixedSize(24, 24);  // Set size of searchButton to 16x16 pixels
    connect(xmlIOButton, &QPushButton::clicked, this, &LibWidget::onXmlIoButtonClicked);
    searchLayout->addWidget(xmlIOButton);

    //.......................

    mainLayout->addLayout(searchLayout);

    // TabWidget
    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->setContentsMargins(0, 0, 0, 0);
    ipTreeView = new IPTreeView();
    ioTreeView = new IOTreeView(); //..<---

    tabWidget->addTab(ipTreeView, "IP");
    /*int ioTabIndex =*/ tabWidget->addTab(ioTreeView, "I/O");

    QStringList filters = {"*.xml", "*.c", "*.h", "*.tcl", "*.sdc", "*.rtl", ""};
    ipModel = createFilteredModel(filters);
    ioModel = createFilteredModel(filters);

     connect(ipTreeView, &QTreeView::doubleClicked, this, &LibWidget::handleIPTreeViewDoubleClick);
     connect(ioTreeView, &QTreeView::doubleClicked, this, &LibWidget::handleIOTreeViewDoubleClick);

    BaseXMLMgr mgr;

    ipModel->setRootPath(mgr.libraryPath);
    ioModel->setRootPath(mgr.libraryPath);

    ipTreeView->setModel(ipModel);
    ioTreeView->setModel(ioModel);

    ipTreeView->setRootIndex(ipModel->index(mgr.libraryPath));
    ioTreeView->setRootIndex(ioModel->index(mgr.libraryPath));

    // 열을 숨깁니다. 0번 열은 이름 열이므로 숨기지 않습니다.

    for (int i = 1; i < ipModel->columnCount(); ++i) {
        ipTreeView->hideColumn(i);
        ioTreeView->hideColumn(i);
    }



    tabWidget->addTab(ipTreeView, "IP");
    tabWidget->addTab(ioTreeView, "I/O");
    mainLayout->addWidget(tabWidget);

    this->setLayout(mainLayout);


    //connect(xmlButton, &QPushButton::clicked, this, &LibWidget::onXmlButtonClicked);
#else
    filter_->setSourceModel(dataModel);

    // set view to use LibraryTreeFilter as source model
    view_.setModel(filter_);

    // the layout to manage the visible items in the widget
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(&view_);
    layout->setContentsMargins(0, 0, 0, 0);

    setupConnections(dataModel);

    view_.sortByColumn(0, Qt::AscendingOrder);
#endif
}

QFileSystemModel* LibWidget::createFilteredModel(const QStringList& filters) {
    QFileSystemModel* model = new QFileSystemModel();
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    model->setNameFilters(filters);
    model->setNameFilterDisables(false);
    return model;
}

void LibWidget::populateTreeFromIPXML(QStandardItem *parent, QXmlStreamReader &xml)
{
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            QStandardItem *item = new QStandardItem(xml.name().toString());
            parent->appendRow(item);

            if (xml.attributes().size() > 0) {
                for (const QXmlStreamAttribute &attr : xml.attributes()) {
                    QStandardItem *attrName = new QStandardItem(attr.name().toString());
                    QStandardItem *attrValue = new QStandardItem(attr.value().toString());
                    item->appendRow({attrName, attrValue});
                }
            }

            if (!xml.isEndElement()) {
                populateTreeFromIPXML(item, xml);
            }
        }
        else if (xml.isEndElement()) {
            return;
        }
        else if (xml.isCharacters() && !xml.isWhitespace()) {
            QStandardItem *textItem = new QStandardItem(xml.text().toString());
            parent->appendRow(textItem);
        }
    }
}

#if LIBWIDGET_20231213 == 1

//-----------------------------------------------------------------------------
// Function: LibraryTreeWidget::setupConnections()
//-----------------------------------------------------------------------------
void LibWidget::setupConnections(LibraryTreeModel* dataModel)
{
//	connect(&view_, SIGNAL(errorMessage(QString const&)),
//        this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(noticeMessage(QString const&)),
//		this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);

    // connect the view to the tree model
    connect(&view_, SIGNAL(openComponent(QModelIndex const&)),
            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(openDesign(QModelIndex const&, QString const&)),
            dataModel, SLOT(onOpenDesign(QModelIndex const&, QString const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(openMemoryDesign(const QModelIndex&, QString const&)),
//        dataModel, SLOT(onOpenMemoryDesign(const QModelIndex&, QString const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(openSWDesign(QModelIndex const&)),
//            dataModel, SLOT(onOpenSWDesign(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(openSystemDesign(QModelIndex const&)),
//        dataModel, SLOT(onOpenSystemDesign(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(createNewComponent(QModelIndex const&)),
            dataModel, SLOT(onCreateNewComponent(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(createNewDesign(QModelIndex const&)),
            dataModel, SLOT(onCreateNewDesign(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(createNewSWDesign(QModelIndex const&)),
//            dataModel, SLOT(onCreateNewSWDesign(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(createNewSystemDesign(QModelIndex const&)),
//            dataModel, SLOT(onCreateNewSystemDesign(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(openBus(QModelIndex const&)),
//		    dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(openCatalog(QModelIndex const&)),
//        dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(createBus(QModelIndex const&)),
//		    dataModel, SLOT(onCreateBus(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(createAbsDef(QModelIndex const&)),
//		    dataModel, SLOT(onCreateAbsDef(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(openComDef(QModelIndex const&)),
//            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(createComDef(QModelIndex const&)),
//            dataModel, SLOT(onCreateComDef(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(openApiDef(QModelIndex const&)),
//            dataModel, SLOT(onOpenDocument(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(createApiDef(QModelIndex const&)),
//            dataModel, SLOT(onCreateApiDef(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(deleteItem(QModelIndex const&)),
//		    dataModel, SLOT(onDeleteItem(QModelIndex const&)), Qt::UniqueConnection);

//	connect(&view_, SIGNAL(exportItem(QModelIndex const&)),
//		dataModel, SLOT(onExportItem(QModelIndex const&)), Qt::UniqueConnection);

//    connect(&view_, SIGNAL(showErrors(QModelIndex const&)),
//        dataModel, SLOT(onShowErrors(QModelIndex const&)), Qt::UniqueConnection);

    connect(&view_, SIGNAL(itemSelected(const VLNV&)),
        this, SIGNAL(itemSelected(const VLNV&)), Qt::UniqueConnection);

    connect(dataModel, SIGNAL(invalidateFilter()), filter_, SLOT(invalidate()), Qt::UniqueConnection);
}

#endif

void LibWidget::onXmlButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*.xml)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QXmlStreamReader xml(&file);

        QStandardItemModel *model = new QStandardItemModel();
        model->setParent(ipTreeView);

        QStandardItem *rootItem = model->invisibleRootItem();

        populateTreeFromIPXML(rootItem, xml);

        ipTreeView->setModel(model);
        ipTreeView->expandAll();
    }
}

void LibWidget::onXmlIoButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*.xml)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QXmlStreamReader xml(&file);

        QStandardItemModel *model = new QStandardItemModel();
        model->setParent(ioTreeView);

        QStandardItem *rootItem = model->invisibleRootItem();

        populateTreeFromIOXML(rootItem, xml);

        ioTreeView->setModel(model);
        ioTreeView->expandAll();
    }
}

void LibWidget::populateTreeFromIOXML(QStandardItem *parent, QXmlStreamReader &xml)
{
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            QStandardItem *item = new QStandardItem(xml.name().toString());
            parent->appendRow(item);

            // XML의 속성 처리
            if (xml.attributes().size() > 0) {
                for (const QXmlStreamAttribute &attr : xml.attributes()) {
                    QStandardItem *attrName = new QStandardItem(attr.name().toString());
                    QStandardItem *attrValue = new QStandardItem(attr.value().toString());
                    item->appendRow({attrName, attrValue});
                }
            }

            // 일부 XML 태그는 텍스트 값을 가지고 있으므로 이를 처리합니다.
            if (xml.isCharacters() && !xml.isWhitespace()) {
                QStandardItem *textItem = new QStandardItem(xml.text().toString());
                item->appendRow(textItem);
            } else {
                // 중첩된 XML 요소 처리
                populateTreeFromIOXML(item, xml);
            }
        }
        else if (xml.isEndElement()) {
            return;
        }
        else if (xml.isCharacters() && !xml.isWhitespace()) {
            QStandardItem *textItem = new QStandardItem(xml.text().toString());
            parent->appendRow(textItem);
        }
        else if (xml.isComment()) {
            QStandardItem *commentItem = new QStandardItem("Comment: " + xml.text().toString());
            parent->appendRow(commentItem);
        }
    }
}

void LibWidget::handleIOTreeViewDoubleClick(const QModelIndex &index)
{
    QString filePath = ioModel->filePath(index);

    if(ipModel->isDir(index)) {
        QDir dir(filePath);
        QStringList xmlFiles = dir.entryList(QStringList() << "*.xml", QDir::Files);

        if(!xmlFiles.isEmpty()) {
                          // 폴더 내에 .xml 파일이 하나 이상 있음
            //QMessageBox::information(this, "XML Found", QString("Found .xml files in: %1").arg(filePath));

            MyMainWindow* mainWindow = Singleton::getInstance().getMainWorkWind();
            if(mainWindow) {
                QString firstXmlFile = xmlFiles.first();
                QString fileName = getFileNameFromPath(firstXmlFile); //순수한 파일이름 구함

                if(fileName.contains(".xml")){
                    mainWindow->AddTapWindow("unsavedio", filePath+"/"+firstXmlFile);
                    //QMetaObject::invokeMethod(mainWindow, "displayFilePathSlot", Q_ARG(QString, filePath));
                }
            }

        } else {
                        // 폴더 내에 .xml 파일이 없음
        }
    }
    else
    {
        MyMainWindow* mainWindow = Singleton::getInstance().getMainWorkWind();
        if(mainWindow) {
            QString fileName = getFileNameFromPath(filePath);
            if(fileName.contains(".xml")){
                mainWindow->AddTapWindow("unsavedio", filePath);
            }
        }
    }

/*

    // 이 부분에서 파일을 더블 클릭했을 때의 로직을 처리합니다.
    QMessageBox::information(this, "File Opened", QString("You opened: %1").arg(filePath));

    MyMainWindow* mainWindow = Singleton::getInstance().getMainWorkWind();
    if(mainWindow) {

        QString fileName = getFileNameFromPath(filePath);
        if(fileName.contains(".xml")){
            mainWindow->AddTapWindow("unsavedio", filePath);
            //QMetaObject::invokeMethod(mainWindow, "displayFilePathSlot", Q_ARG(QString, filePath));
        }

    } else {
        //qDebug() << "MyMainWindow instance is nullptr";
    }
*/
}



void LibWidget::handleIPTreeViewDoubleClick(const QModelIndex &index)
{
    QString filePath = ipModel->filePath(index);

    if(ipModel->isDir(index)) {
        QDir dir(filePath);
        QStringList xmlFiles = dir.entryList(QStringList() << "*.xml", QDir::Files);

        if(!xmlFiles.isEmpty()) {
            // 폴더 내에 .xml 파일이 하나 이상 있음
            QMessageBox::information(this, "XML Found", QString("Found .xml files in: %1").arg(filePath));
        } else {
            // 폴더 내에 .xml 파일이 없음
            QMessageBox::information(this, "No XML Found", QString("No .xml files found in: %1").arg(filePath));
        }
    } else {
        // 파일을 더블 클릭했을 때의 로직을 처리합니다.
        QMessageBox::information(this, "File Opened", QString("You opened: %1").arg(filePath));
    }
}
