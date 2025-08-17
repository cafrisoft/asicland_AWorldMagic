#include "CommandWindow.h"

#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QLineEdit>

#include <QTabWidget>
#include <QVBoxLayout>
#include "ConsolLogTab.h"
#include "LogOutTab.h"

CommandWindow::CommandWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QTabWidget *tabWidget = new QTabWidget(this);

    // 우측 상단에 배치될 위젯들을 가질 QHBoxLayout를 생성합니다.
    QHBoxLayout *topRightLayout = new QHBoxLayout();

    QLineEdit *lineEdit = new QLineEdit(this);
    topRightLayout->addWidget(lineEdit);

    QPushButton *button1 = new QPushButton(this);
    button1->setText("검색");
    //0911 button1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    topRightLayout->addWidget(button1);

    QPushButton *button2 = new QPushButton(this);
    button2->setText("일반 버튼");
    //0911 button2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    topRightLayout->addWidget(button2);

    topRightLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *cornerWidget = new QWidget(this);
    cornerWidget->setLayout(topRightLayout);
/*
    cornerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //welcomeTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  */

    // Initialize widgets visibility to false
    lineEdit->setVisible(false);
    button1->setVisible(false);
    button2->setVisible(false);

    // QHBoxLayout을 QTabWidget에 추가합니다.
    tabWidget->setCornerWidget(cornerWidget, Qt::TopRightCorner);

    // Create Console tab
    consolLogTab_ = new ConsolLogTab();
    tabWidget->addTab(consolLogTab_, "Console");

    // Create Log tab
    logOutTab_ = new LogOutTab();
    tabWidget->addTab(logOutTab_, "Log");

    // Create Report tab
    QWidget *reportTab = new QWidget();
    tabWidget->addTab(reportTab, "Report");

    // Add the tab widget to the layout
    layout->addWidget(tabWidget);

    this->setLayout(layout);

    // Connect signal
    connect(tabWidget, &QTabWidget::currentChanged, this, [lineEdit, button1, button2, tabWidget](int index){
        // Only when the tab titled "Report" is selected
        if (tabWidget->tabText(index) == "Report") {
            lineEdit->setVisible(true);
            button1->setVisible(true);
            button2->setVisible(true);
        } else {
            lineEdit->setVisible(false);
            button1->setVisible(false);
            button2->setVisible(false);
        }
    });
}

void CommandWindow::LogOut(QString log, bool isErr)
{
    if(logOutTab_)
        logOutTab_->LogPrint(log, isErr);
}
