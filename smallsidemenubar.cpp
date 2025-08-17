#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>
#include <QMenu>

#include "smallsidemenubar.h"


SmallSideMenuBar::SmallSideMenuBar(QWidget *parent)
    : QWidget{parent}
{
    // Enable the widget's auto-fill background property
    setAutoFillBackground(true);
    setStyleSheet("SmallSideMenuBar { background-color: yellow; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 120, 1, 1); // Set all margins to 0 pixels

    QLabel *label = new QLabel("");
    label->setFixedHeight(24);
    layout->addSpacing(15);

    fileBtn_ = new QPushButton(this);
    fileBtn_->setStyleSheet("border: none;");
    fileBtn_->setIcon(QIcon(":/resources/Res/File.png"));
    fileBtn_->setIconSize(QSize(24, 24));
    fileBtn_->setFixedHeight(24);
    layout->addWidget(fileBtn_);
    layout->addSpacing(15);

//    projectBtn_ = new QPushButton(this);
//    projectBtn_->setStyleSheet("border: none;");
//    projectBtn_->setIcon(QIcon(":/resources/Res/File.png"));
//    projectBtn_->setIconSize(QSize(24, 24));
//    projectBtn_->setFixedHeight(24);
//    layout->addWidget(projectBtn_);
//    layout->addSpacing(15);

    playBtn_ = new QPushButton(this);
    playBtn_->setStyleSheet("border: none;");
    playBtn_->setIcon(QIcon(":/resources/Res/Play.png"));
    playBtn_->setIconSize(QSize(24, 24));
    playBtn_->setFixedHeight(24);
    layout->addWidget(playBtn_);
    layout->addSpacing(15);

    configBtn_ = new QPushButton(this);
    configBtn_->setStyleSheet("border: none;");
    configBtn_->setIcon(QIcon(":/resources/Res/Config.png"));
    configBtn_->setIconSize(QSize(24, 24));
    configBtn_->setFixedHeight(24);
    layout->addWidget(configBtn_);

    layout->addStretch(1); // Add a stretchable space at the end //<----
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(fileBtn_, &QPushButton::clicked, this, &SmallSideMenuBar::onFileBtnClicked);
//    connect(projectBtn_, &QPushButton::clicked, this, &SmallSideMenuBar::onProjectBtnClicked);
    connect(playBtn_, &QPushButton::clicked, this, &SmallSideMenuBar::onPlayBtnClicked);
    connect(configBtn_, &QPushButton::clicked, this, &SmallSideMenuBar::onConfigBtnClicked);

    this->setLayout(layout);
}

void SmallSideMenuBar::onFileBtnClicked() {

    QMenu contextMenu(tr("File"), this);

    QAction* newAction = contextMenu.addAction(tr("New"));
    QMenu* newSubMenu = new QMenu(tr("New"), &contextMenu);
    newAction->setMenu(newSubMenu);

    QAction* newBusAction = newSubMenu->addAction(tr("I/O Magic"));
    connect(newBusAction, &QAction::triggered, this, &SmallSideMenuBar::onNewBusTriggered);

    QAction* newComponentAction = newSubMenu->addAction(tr("IP"));
    connect(newComponentAction, &QAction::triggered, this, &SmallSideMenuBar::onNewComponentTriggered);

    QAction* newProjectAction = newSubMenu->addAction(tr("Project"));
    connect(newProjectAction, &QAction::triggered, this, &SmallSideMenuBar::onNewProjectTriggered);

    QAction* newDesignAction = newSubMenu->addAction(tr("Design"));
    connect(newDesignAction, &QAction::triggered, this, &SmallSideMenuBar::onNewDesignTriggered);

    contextMenu.addSeparator();

    QAction* openProjectAction = contextMenu.addAction(tr("Open Project"));
    connect(openProjectAction, &QAction::triggered, this, &SmallSideMenuBar::onOpenProjectTriggered);

    QAction* closeProjectAction = contextMenu.addAction(tr("Close Project"));
    connect(closeProjectAction, &QAction::triggered, this, &SmallSideMenuBar::onCloseProjectTriggered);

    QAction* importProejctAction = contextMenu.addAction(tr("Import Project"));
    connect(importProejctAction, &QAction::triggered, this, &SmallSideMenuBar::onImportProjectTriggered);

    QAction* exportProjectAction = contextMenu.addAction(tr("Export Project"));
    connect(exportProjectAction, &QAction::triggered, this, &SmallSideMenuBar::onExportProjectTriggered);

    contextMenu.addSeparator();

    QAction* saveAction = contextMenu.addAction(tr("Save"));
    connect(saveAction, &QAction::triggered, this, &SmallSideMenuBar::onSaveTriggered);

    QAction* saveAllAction = contextMenu.addAction(tr("Save All"));
    connect(saveAllAction, &QAction::triggered, this, &SmallSideMenuBar::onSaveAllTriggered);

    // 스타일 설정
    contextMenu.setStyleSheet(
        "QMenu {"
        "background-color: #f0f0f0;"  // 일반적인 회색 배경
        "border: 1px solid #888;"     // 테두리 색상 설정
        "}"
        "QMenu::item {"
        "padding: 2px 25px 2px 20px;" // 패딩 설정
        "}"
        "QMenu::item:selected {"
        "background-color: #308cc6;"  // 선택된 항목의 배경색 설정
        "color: white;"               // 선택된 항목의 글자색 설정
        "}"
        );

    contextMenu.exec(QCursor::pos());

    /* 오른쪽 마우스 용으로 할 때
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            [this, contextMenu](const QPoint& pos){
                contextMenu->exec(this->mapToGlobal(pos));
            });
    */
}

//void SmallSideMenuBar::onProjectBtnClicked()
//{
//    QMenu contextMenu(tr("Project"), this);

//    QAction* openProjectAction = contextMenu.addAction(tr("Open Project"));
//    connect(openProjectAction, &QAction::triggered, this, &SmallSideMenuBar::onOpenProjectTriggered);

//    QAction* closeProjectAction = contextMenu.addAction(tr("Close Project"));
//    connect(closeProjectAction, &QAction::triggered, this, &SmallSideMenuBar::onCloseProjectTriggered);

//    // 스타일 설정
//    contextMenu.setStyleSheet(
//        "QMenu {"
//        "background-color: #f0f0f0;"  // 일반적인 회색 배경
//        "border: 1px solid #888;"     // 테두리 색상 설정
//        "}"
//        "QMenu::item {"
//        "padding: 2px 25px 2px 20px;" // 패딩 설정
//        "}"
//        "QMenu::item:selected {"
//        "background-color: #308cc6;"  // 선택된 항목의 배경색 설정
//        "color: white;"               // 선택된 항목의 글자색 설정
//        "}"
//        );

//    contextMenu.exec(QCursor::pos());
//}

void SmallSideMenuBar::onPlayBtnClicked() {
    QMenu contextMenu(tr("Run"), this);

   // QAction* simulatePre = contextMenu.addAction(tr("Merge Script"));
   // connect(simulatePre, &QAction::triggered, this, &SmallSideMenuBar::onRunSimulatePreTriggered);

    QAction* simulate = contextMenu.addAction(tr("Simulate"));
    connect(simulate, &QAction::triggered, this, &SmallSideMenuBar::onRunSimulatePreTriggered);

    QAction* synthesis = contextMenu.addAction(tr("Synthesis"));
    connect(synthesis, &QAction::triggered, this, &SmallSideMenuBar::onRunSynthesisTriggered);

    QAction* fpga = contextMenu.addAction(tr("FPGA"));
    connect(fpga, &QAction::triggered, this, &SmallSideMenuBar::onRunFPGATriggered);

    // 스타일 설정
    contextMenu.setStyleSheet(
        "QMenu {"
        "background-color: #f0f0f0;"  // 일반적인 회색 배경
        "border: 1px solid #888;"     // 테두리 색상 설정
        "}"
        "QMenu::item {"
        "padding: 2px 25px 2px 20px;" // 패딩 설정
        "}"
        "QMenu::item:selected {"
        "background-color: #308cc6;"  // 선택된 항목의 배경색 설정
        "color: white;"               // 선택된 항목의 글자색 설정
        "}"
        );

    contextMenu.exec(QCursor::pos());
}

void SmallSideMenuBar::onConfigBtnClicked()
{
    qDebug() << "Config Button Clicked";

    QMenu contextMenu(tr("System"), this);

    QAction* preference = contextMenu.addAction(tr("Preference"));
    connect(preference, &QAction::triggered, this, &SmallSideMenuBar::onPreferenceTriggered);

    QAction* help = contextMenu.addAction(tr("Help"));
    connect(help, &QAction::triggered, this, &SmallSideMenuBar::onHelpTriggered);

    QAction* about = contextMenu.addAction(tr("About"));
    connect(about, &QAction::triggered, this, &SmallSideMenuBar::onAboutTriggered);

    // 스타일 설정
    contextMenu.setStyleSheet(
        "QMenu {"
        "background-color: #f0f0f0;"  // 일반적인 회색 배경
        "border: 1px solid #888;"     // 테두리 색상 설정
        "}"
        "QMenu::item {"
        "padding: 2px 25px 2px 20px;" // 패딩 설정
        "}"
        "QMenu::item:selected {"
        "background-color: #308cc6;"  // 선택된 항목의 배경색 설정
        "color: white;"               // 선택된 항목의 글자색 설정
        "}"
        );

    contextMenu.exec(QCursor::pos());
}

void SmallSideMenuBar::onNewBusTriggered()
{
    emit newBus();
}

void SmallSideMenuBar::onNewComponentTriggered()
{
    emit newComponent();
}

void SmallSideMenuBar::onNewProjectTriggered()
{
    emit newProject();
}

void SmallSideMenuBar::onNewDesignTriggered()
{
    emit newDesign();
}

void SmallSideMenuBar::onOpenProjectTriggered()
{
    emit openProject();
}

void SmallSideMenuBar::onCloseProjectTriggered()
{
    emit closeProject();
}

void SmallSideMenuBar::onImportProjectTriggered()
{
    emit importProject();
}

void SmallSideMenuBar::onExportProjectTriggered()
{
    emit exportProject();
}

void SmallSideMenuBar::onSaveTriggered()
{
    emit saveDocument();
}

void SmallSideMenuBar::onSaveAllTriggered()
{
    emit saveAllDocument();
}

void SmallSideMenuBar::onRunSimulatePreTriggered()
{
    emit runSimulatePre();
}

void SmallSideMenuBar::onRunSimulateTriggered()
{
    emit runSimulate();
}

void SmallSideMenuBar::onRunSynthesisTriggered()
{
    emit runSynthesis();
}

void SmallSideMenuBar::onRunFPGATriggered()
{
    emit runFPGA();
}

void SmallSideMenuBar::onPreferenceTriggered()
{
    emit openPreference();
}

void SmallSideMenuBar::onHelpTriggered()
{
    emit openHelp();
}

void SmallSideMenuBar::onAboutTriggered()
{
    emit openAbout();
}

