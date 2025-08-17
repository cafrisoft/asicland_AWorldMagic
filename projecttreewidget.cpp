#include "projecttreewidget.h"
#include <QPushButton>
#include <QFileDialog>
#include "singleton.h"
#include "mymainwindow.h"
//#include <QStackedWidget>
#include <QAction>
#include <QMenu>
#include <QResizeEvent>
#include <QStackedLayout>
#include <projectcaptionbtn.h>


ProjectTreeWidgetEx::ProjectTreeWidgetEx(const QString& path, QWidget *parent)
    : QWidget{parent},
      m_model(new QFileSystemModel(this)),
      m_tree(new QTreeView(this))
{
    /*
    QDir dir(path.isEmpty() ? QDir::currentPath() : path);
    QString displayPath = dir.absolutePath();
    if (dir.cdUp()) { // Move to the parent directory
        displayPath = dir.absolutePath();
    }*/

    QDir dir(path.isEmpty() ? QDir::homePath() : path);
    QString displayPath = dir.absolutePath();

    //"Project" 트리안의 보여줄 내용 지정

    if (dir.cdUp()) { // Move to the parent directory
        displayPath = dir.absolutePath();
    }

    m_model->setRootPath(displayPath);

    // ipTreeView와 동일한 방식으로 보이게 설정
    QStringList filters = {"*.c", "*.h", "*.tcl", "*.sdc", "*.rtl", ""};
    m_model->setNameFilters(filters);
    m_model->setNameFilterDisables(false);

    m_tree->setModel(m_model);
    m_tree->setRootIndex(m_model->index(displayPath));

    // 열을 숨깁니다. 0번 열은 이름 열이므로 숨기지 않습니다.
    for (int i = 1; i < m_model->columnCount(); ++i) {
        m_tree->hideColumn(i);
    }

    m_emptyWidget = new QWidget(this);
    m_emptyWidget->setStyleSheet("border: 1px solid black; background-color: white;");

    ProjectCaptionBtn* pCaptionBtn = new ProjectCaptionBtn();
    pCaptionBtn->setStyleSheet("border: 1px solid black; background-color: white;");
    pCaptionBtn->setContentsMargins(0, 0, 0, 0); // Set padding to 0
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pCaptionBtn);

    QStackedLayout* stackedLayout = new QStackedLayout;
    // Set up m_tree as before...
    stackedLayout->addWidget(m_emptyWidget);
    stackedLayout->addWidget(m_tree);

    mainLayout->addLayout(stackedLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0); // remove margins
    mainLayout->setSpacing(0); // remove spacing
    setLayout(mainLayout);

    m_emptyWidget->setContextMenuPolicy(Qt::CustomContextMenu); // Enable custom context menu for this widget
    connect(m_emptyWidget, &QWidget::customContextMenuRequested, this, &ProjectTreeWidgetEx::showContextMenu);

    //if(path.isEmpty())
    //    stackedLayout->setCurrentWidget(m_emptyWidget);  // Hide m_tree
    //else
        stackedLayout->setCurrentWidget(m_tree);  // Show m_tree

    connect(m_tree->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ProjectTreeWidgetEx::displayFilePath);
    //connect(m_changeFolderButton, &QPushButton::clicked, this, &ProjectTreeWidgetEx::changeFolder);

}

void ProjectTreeWidgetEx::changeFolder()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                  "/home",
                                                  QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty())
  {
      m_model->setRootPath(dir);
      m_tree->setRootIndex(m_model->index(dir));
  }
}

void ProjectTreeWidgetEx::displayFilePath(const QItemSelection &selected)
{
    // Get the index of the selected item
    QModelIndex index = selected.indexes().first();

    // Use the model to get the file path
    QString filePath = m_model->filePath(index);

    // Display the file path in a message box
    //QMessageBox::information(this, "File Path", filePath);

    // Send event to MyMainWindow via the Singleton
    MyMainWindow* mainWindow = Singleton::getInstance().getMainWorkWind();
    if(mainWindow) {
      QMetaObject::invokeMethod(mainWindow, "displayFilePathSlot", Q_ARG(QString, filePath));
    } else {
      //....qDebug() << "MyMainWindow instance is nullptr";
    }
}
void ProjectTreeWidgetEx::resizeEvent(QResizeEvent *event)
{
    //m_changeFolderButton->setFixedWidth(width() / 3);
    QWidget::resizeEvent(event);
}

void ProjectTreeWidgetEx::showContextMenu(const QPoint &pos) {
    return; //우측마우스는 일단 안쓴다.
    QMenu contextMenu(this);

    QAction action1("Menu Item 1", this);
    connect(&action1, &QAction::triggered, this, &ProjectTreeWidgetEx::menuItem1Clicked);
    contextMenu.addAction(&action1);

    QAction action2("Menu Item 2", this);
    connect(&action2, &QAction::triggered, this, &ProjectTreeWidgetEx::menuItem2Clicked);
    contextMenu.addAction(&action2);

    contextMenu.exec(m_emptyWidget->mapToGlobal(pos));
}

void ProjectTreeWidgetEx::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && m_emptyWidget->geometry().contains(event->pos())) {
      QMenu contextMenu(this);

      QAction action1("Menu Item 1", this);
      connect(&action1, &QAction::triggered, this, &ProjectTreeWidgetEx::menuItem1Clicked);
      contextMenu.addAction(&action1);

      QAction action2("Menu Item 2", this);
      connect(&action2, &QAction::triggered, this, &ProjectTreeWidgetEx::menuItem2Clicked);
      contextMenu.addAction(&action2);

      contextMenu.exec(event->globalPos());
    } else {
      QWidget::mousePressEvent(event);
    }
}

void ProjectTreeWidgetEx::menuItem1Clicked() {
    // Do something when Menu Item 1 is clicked
}

void ProjectTreeWidgetEx::menuItem2Clicked() {
    // Do something when Menu Item 2 is clicked
}
