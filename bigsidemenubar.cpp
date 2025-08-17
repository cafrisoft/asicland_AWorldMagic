#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "bigsidemenubar.h"


BigSideMenuBar::BigSideMenuBar(QWidget *parent)
    : QWidget{parent}
{
    // 중앙 위젯과 레이아웃 설정
    //QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0); // 간격을 0으로 설정
    layout->setContentsMargins(0, 0, 0, 0); // 좌, 상, 우, 하 여백을 0으로 설정

   // centralWidget->setLayout(layout);
   // setCentralWidget(centralWidget);

    // y좌표가 30인 위치에서 시작하도록 빈 공간 추가
    QLabel *spacer = new QLabel();
    spacer->setFixedHeight(10);
    layout->addWidget(spacer);

    // 메뉴 추가
    addMenu(layout, "File", true);
//    addMenu(layout, "Project", true);
    addMenu(layout, "Run", false);
    addMenu(layout, "System", false);
    layout->addStretch(); // 이 부분이 레이아웃의 나머지 공간을 채움
    this->setLayout(layout);
}

void BigSideMenuBar::addMenu(QVBoxLayout  *layout, const QString &name, bool hasSubMenu)
{
    QVBoxLayout *spaceLayout = new QVBoxLayout();
    spaceLayout->setContentsMargins(0, 0, 0, 0); // 위쪽 마진을 30으로 설정
    layout->addLayout(spaceLayout);

    QVBoxLayout *subMenuLayout = new QVBoxLayout();
    subMenuLayout->setSpacing(0); // 서브메뉴 레이아웃의 간격도 0으로 설정

    if(name=="File")
    {
        QPushButton *fileBtn = new QPushButton(this);
        fileBtn->setStyleSheet("border: none;");
        //fileBtn->setStyleSheet("border: none; color: white;"); // 텍스트 색상을 흰색으로 설정
        fileBtn->setStyleSheet("CreateIOWirePort { border: 1px solid black; color: white;}");
        fileBtn->setText("  File");
        fileBtn->setIcon(QIcon(":/resources/Res/File.png"));
        fileBtn->setIconSize(QSize(24, 24));
       // fileBtn->hide();
        subMenuLayout->addWidget(fileBtn);

        QVBoxLayout *additionalSubMenuLayout = new QVBoxLayout();
        additionalSubMenuLayout->setSpacing(0);

        // NEW Button Group start
        QPushButton *newBtn = new QPushButton("New");
        newBtn->setStyleSheet("background-color:white; color:black;");
        newBtn->hide();
        additionalSubMenuLayout->addWidget(newBtn);

        QVBoxLayout *subNewMenuLayout = new QVBoxLayout();
        subNewMenuLayout->setSpacing(0); // 서브메뉴 레이아웃의 간격도 0으로 설정

        QPushButton *newBusBtn = new QPushButton("I/O Magic");
        newBusBtn->setStyleSheet("background-color:white; color:black;");
        newBusBtn->hide();
        subNewMenuLayout->addWidget(newBusBtn);

        QPushButton *newComponetBtn = new QPushButton("IP");
        newComponetBtn->setStyleSheet("background-color:white; color:black;");
        newComponetBtn->hide();
        subNewMenuLayout->addWidget(newComponetBtn);

        QPushButton *newProjectBtn = new QPushButton("Project");
        newProjectBtn->setStyleSheet("background-color:white; color:black;");
        newProjectBtn->hide();
        subNewMenuLayout->addWidget(newProjectBtn);

        QPushButton *newDesignBtn = new QPushButton("Design");
        newDesignBtn->setStyleSheet("background-color:white; color:black;");
        newDesignBtn->hide();
        subNewMenuLayout->addWidget(newDesignBtn);

        additionalSubMenuLayout->addLayout(subNewMenuLayout);

        connect(newBtn, &QPushButton::clicked, [=](){
            for (int i = 0; i < subNewMenuLayout->count(); ++i) {
                QWidget *widget = subNewMenuLayout->itemAt(i)->widget();
                if (widget) {
                    widget->setVisible(!widget->isVisible());
                }
            }
        });

        // addSeparator
        additionalSubMenuLayout->addSpacing(5);

        // Open project
        QPushButton *openProjectBtn = new QPushButton("Open Project");
        openProjectBtn->setStyleSheet("background-color:white; color:black;");
        openProjectBtn->hide();
        additionalSubMenuLayout->addWidget(openProjectBtn);

        // Close project
        QPushButton *closeProjectBtn = new QPushButton("Close Project");
        closeProjectBtn->setStyleSheet("background-color:white; color:black;");
        closeProjectBtn->hide();
        additionalSubMenuLayout->addWidget(closeProjectBtn);

        // Import project
        QPushButton *importProjectBtn = new QPushButton("Import project");
        importProjectBtn->setStyleSheet("background-color:white; color:black;");
        importProjectBtn->hide();
        additionalSubMenuLayout->addWidget(importProjectBtn);

        // Export project
        QPushButton *exportProjectBtn = new QPushButton("export project");
        exportProjectBtn->setStyleSheet("background-color:white; color:black;");
        exportProjectBtn->hide();
        additionalSubMenuLayout->addWidget(exportProjectBtn);

        // addSeparator
//        QFrame *projectLline = new QFrame(this);
//        projectLline->setFrameShape(QFrame::HLine);
//        projectLline->setFrameShadow(QFrame::Plain);
//        additionalSubMenuLayout->addWidget(projectLline);

        additionalSubMenuLayout->addSpacing(5);

        // SAVE
        QPushButton *saveBtn = new QPushButton("Save");
        saveBtn->setStyleSheet("background-color:white; color:black;");
        saveBtn->hide();
        additionalSubMenuLayout->addWidget(saveBtn);

        // SAVE ALL
        QPushButton *saveBtnAll = new QPushButton("Save All");
        saveBtnAll->setStyleSheet("background-color:white; color:black;");
        saveBtnAll->hide();
        additionalSubMenuLayout->addWidget(saveBtnAll);

        connect(fileBtn, &QPushButton::clicked, [=](){
            for (int i = 0; i < additionalSubMenuLayout->count(); ++i) {
                QWidget *widget = additionalSubMenuLayout->itemAt(i)->widget();
                if (widget) {
                    widget->setVisible(!widget->isVisible());
                }
            }
        });

        subMenuLayout->addLayout(additionalSubMenuLayout);

        connect(newBusBtn, &QPushButton::clicked, this, &BigSideMenuBar::onNewBusClicked);
        connect(newComponetBtn, &QPushButton::clicked, this, &BigSideMenuBar::onNewComponentClicked);
        connect(newProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onNewProjectClicked);
        connect(newDesignBtn, &QPushButton::clicked, this, &BigSideMenuBar::onNewDesignClicked);

        connect(openProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onOpenProjectClicked);
        connect(closeProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onCloseProjectClicked);

        connect(importProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onImportProjectClicked);
        connect(exportProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onExportProjectClicked);

        connect(saveBtn, &QPushButton::clicked, this, &BigSideMenuBar::onSaveClicked);
        connect(saveBtnAll, &QPushButton::clicked, this, &BigSideMenuBar::onSaveAllClicked);
    }

//    if(name=="Project")
//    {
//        QPushButton *ProjectBtn = new QPushButton(this);
//        ProjectBtn->setStyleSheet("CreateIOWirePort { border: 1px solid black; color: white;}");
//        ProjectBtn->setIcon(QIcon(":/resources/Res/Project.png"));
//        ProjectBtn->setIconSize(QSize(24, 24));
//        ProjectBtn->setText("  Project");
//        subMenuLayout->addWidget(ProjectBtn);

//        QVBoxLayout *addSubProjectMenuLayout = new QVBoxLayout();
//        addSubProjectMenuLayout->setSpacing(0);

//        QPushButton *openProjectBtn = new QPushButton("Open Project");
//        openProjectBtn->setStyleSheet("background-color:white; color:black;");
//        openProjectBtn->hide();
//        addSubProjectMenuLayout->addWidget(openProjectBtn);

//        QPushButton *closeProjectBtn = new QPushButton("Close Project");
//        closeProjectBtn->setStyleSheet("background-color:white; color:black;");
//        closeProjectBtn->hide();
//        addSubProjectMenuLayout->addWidget(closeProjectBtn);

//        subMenuLayout->addLayout(addSubProjectMenuLayout);

//        connect(ProjectBtn, &QPushButton::clicked, [=](){
//            for (int i = 0; i < addSubProjectMenuLayout->count(); ++i) {
//                QWidget *widget = addSubProjectMenuLayout->itemAt(i)->widget();
//                if (widget) {
//                    widget->setVisible(!widget->isVisible());
//                }
//            }
//        });

//        connect(openProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onOpenProjectClicked);
//        connect(closeProjectBtn, &QPushButton::clicked, this, &BigSideMenuBar::onCloseProjectClicked);
//    }

    if(name=="Run")
    {
        QPushButton *runBtn = new QPushButton(this);
        runBtn->setStyleSheet("CreateIOWirePort { border: 1px solid black; color: white;}");
        runBtn->setIcon(QIcon(":/resources/Res/Play.png"));
        runBtn->setIconSize(QSize(24, 24));
        runBtn->setText("  Run");
        //runBtn->hide();
        subMenuLayout->addWidget(runBtn);

        QVBoxLayout *addSubRunMenuLayout = new QVBoxLayout();
        addSubRunMenuLayout->setSpacing(0);

        QPushButton *runSimulateBtn = new QPushButton("Simulate");
        runSimulateBtn->setStyleSheet("background-color:white; color:black;");
        runSimulateBtn->hide();
        addSubRunMenuLayout->addWidget(runSimulateBtn);

        QPushButton *runSynthesisBtn = new QPushButton("Synthesis");
        runSynthesisBtn->setStyleSheet("background-color:white; color:black;");
        runSynthesisBtn->hide();
        addSubRunMenuLayout->addWidget(runSynthesisBtn);

        QPushButton *runFPGABtn = new QPushButton("FPGA");
        runFPGABtn->setStyleSheet("background-color:white; color:black;");
        runFPGABtn->hide();
        addSubRunMenuLayout->addWidget(runFPGABtn);

        subMenuLayout->addLayout(addSubRunMenuLayout);

        connect(runBtn, &QPushButton::clicked, [=](){
            for (int i = 0; i < addSubRunMenuLayout->count(); ++i) {
                QWidget *widget = addSubRunMenuLayout->itemAt(i)->widget();
                if (widget) {
                    widget->setVisible(!widget->isVisible());
                }
            }
        });

        connect(runSimulateBtn, &QPushButton::clicked, this, &BigSideMenuBar::onRunSimulatePreClicked);
        connect(runSynthesisBtn, &QPushButton::clicked, this, &BigSideMenuBar::onRunSynthesisClicked);
        connect(runFPGABtn, &QPushButton::clicked, this, &BigSideMenuBar::onRunFPGAClicked);
    }

    if(name=="System")
    {
        QPushButton *configBtn = new QPushButton(this);
        configBtn->setStyleSheet("CreateIOWirePort { border: 1px solid black; color: white;}");
        configBtn->setIcon(QIcon(":/resources/Res/Config.png"));
        configBtn->setText("  System");
        configBtn->setIconSize(QSize(24, 24));
        //configBtn->hide();
        subMenuLayout->addWidget(configBtn);

        QVBoxLayout *addSubSystemMenuLayout = new QVBoxLayout();
        addSubSystemMenuLayout->setSpacing(0);

        QPushButton *preferenceBtn = new QPushButton("Preference");
        preferenceBtn->setStyleSheet("background-color:white; color:black;");
        preferenceBtn->hide();
        addSubSystemMenuLayout->addWidget(preferenceBtn);

        QPushButton *helpBtn = new QPushButton("Help");
        helpBtn->setStyleSheet("background-color:white; color:black;");
        helpBtn->hide();
        addSubSystemMenuLayout->addWidget(helpBtn);

        QPushButton *aboutBtn = new QPushButton("About");
        aboutBtn->setStyleSheet("background-color:white; color:black;");
        aboutBtn->hide();
        addSubSystemMenuLayout->addWidget(aboutBtn);

        subMenuLayout->addLayout(addSubSystemMenuLayout);

        connect(configBtn, &QPushButton::clicked, [=](){
            for (int i = 0; i < addSubSystemMenuLayout->count(); ++i) {
                QWidget *widget = addSubSystemMenuLayout->itemAt(i)->widget();
                if (widget) {
                    widget->setVisible(!widget->isVisible());
                }
            }
        });

        connect(preferenceBtn, &QPushButton::clicked, this, &BigSideMenuBar::onPreferenceClicked);
        connect(helpBtn, &QPushButton::clicked, this, &BigSideMenuBar::onHelpClicked);
        connect(aboutBtn, &QPushButton::clicked, this, &BigSideMenuBar::onAboutClicked);
    }

    layout->addLayout(subMenuLayout);
}

void BigSideMenuBar::onNewBusClicked()
{
    emit newBus();
}

void BigSideMenuBar::onNewComponentClicked()
{
    emit newComponent();
}

void BigSideMenuBar::onNewProjectClicked()
{
    emit newProject();
}

void BigSideMenuBar::onNewDesignClicked()
{
    emit newDesign();
}

void BigSideMenuBar::onOpenProjectClicked()
{
    emit openProject();
}

void BigSideMenuBar::onCloseProjectClicked()
{
    emit closeProject();
}

void BigSideMenuBar::onImportProjectClicked()
{
    emit importProject();
}

void BigSideMenuBar::onExportProjectClicked()
{
    emit exportProject();
}
void BigSideMenuBar::onSaveClicked()
{
    emit saveDocument();
}

void BigSideMenuBar::onSaveAllClicked()
{
    emit saveAllDocument();
}

void BigSideMenuBar::onRunSimulatePreClicked()
{
    emit runSimulatePre();
}

void BigSideMenuBar::onRunSimulateClicked()
{
    emit runSimulate();
}

void BigSideMenuBar::onRunSynthesisClicked()
{
    emit runSynthesis();
}

void BigSideMenuBar::onRunFPGAClicked()
{
    emit runFPGA();
}

void BigSideMenuBar::onPreferenceClicked()
{
    emit openPreference();
}

void BigSideMenuBar::onHelpClicked()
{
    emit openHelp();
}

void BigSideMenuBar::onAboutClicked()
{
    emit openAbout();
}

