#include "awsidetoolbar.h"

#include <QDebug>
#include <QPushButton>
#include <QSplitter>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QFileDialog>

#include <UI/SynthesisDialog/SynthesisDialog.h>

#include "./Python/pythonscriptrunner.h"
#include "./Python/PythonSimulate.h"
#include "./Python/PythonFPGA.h"
#include "./Python/PythonFPGADialog.h"

#include "smallsidemenubar.h"
#include "bigsidemenubar.h"

#include "singleton.h"

#include "mainwindow.h"



QStackedLayout *stackedLayout;

AWSideToolBar::AWSideToolBar(QWidget *parent)
    : QWidget{parent}
{
    // Set the background color to blue
    setAutoFillBackground(true);
    setStyleSheet("background-color: #2682ED;"); //<- sky Blue

    smallSideMenuBar_ = new SmallSideMenuBar(this);
    bigSideMenuBar_ = new BigSideMenuBar(this);
    // 자식 위젯의 배경을 투명하게 만듭니다.
  //  smallSideMenuBar_->setAttribute(Qt::WA_TranslucentBackground, true);
  //  bigSideMenuBar_->setAttribute(Qt::WA_TranslucentBackground, true);

    // Position and size the sidebars
    smallSideMenuBar_->setFixedWidth(30);
    bigSideMenuBar_->setFixedWidth(100);

    resizeBtn = new QPushButton(this);
    resizeBtn->setStyleSheet("border: none;");
    resizeBtn->setIcon(QIcon(":/resources/Res/Expand.png"));
    resizeBtn->setIconSize(QSize(32, 32));
    resizeBtn->setStyleSheet("background-color: #2682ED;");

    // Position and size the widgets
    resizeBtn->setFixedHeight(30);

/*
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(smallSideMenuBar_);
    layout->addWidget(bigSideMenuBar_);
    layout->addWidget(resizeBtn);
    layout->setAlignment(resizeBtn, Qt::AlignBottom|Qt::AlignHCenter);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
*/

    QVBoxLayout *layout = new QVBoxLayout(this);

    stackedLayout = new QStackedLayout; // Create a stacked layout
    stackedLayout->addWidget(smallSideMenuBar_); // Add the sidebars to the stacked layout
    stackedLayout->addWidget(bigSideMenuBar_);
    stackedLayout->setCurrentIndex(0); // Display the first sidebar initially

    layout->addLayout(stackedLayout, 1); // Add the stacked layout to the main layout with stretch factor 1
    layout->addWidget(resizeBtn, 0); // 0 stretch value, this widget will not consume space
    layout->setAlignment(resizeBtn, Qt::AlignBottom|Qt::AlignHCenter);
    layout->setSpacing(0);
    layout->setContentsMargins(1, 1, 1, 1);
    this->setLayout(layout);

    connect(resizeBtn, &QPushButton::clicked, this, &AWSideToolBar::resizeBtnClicked);

    connectMenuBar();

  //  smallSideMenuBar_->show();
  //  bigSideMenuBar_->hide();
}



//-----------------------------------------------------------------------------
// Function: AWSideToolBar::connectMenuBar()
//-----------------------------------------------------------------------------
void AWSideToolBar::connectMenuBar()
{

#if DOCKWIDGETHANDLER_20231228 == 0
#else

#if LOG_OUT_SUPPORT_20240104 == 1
    connect(smallSideMenuBar_, SIGNAL(inputMessage(QString const&)), this, SIGNAL(inputMessage(QString const&)), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(noticeMessage(QString const&)), this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(errorMessage(QString const&)), this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(undefinedMessage(QString const&)), this, SIGNAL(undefinedMessage(QString const&)), Qt::UniqueConnection);

    connect(bigSideMenuBar_, SIGNAL(inputMessage(QString const&)), this, SIGNAL(inputMessage(QString const&)), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(noticeMessage(QString const&)), this, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(errorMessage(QString const&)), this, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(undefinedMessage(QString const&)), this, SIGNAL(undefinedMessage(QString const&)), Qt::UniqueConnection);

//    connect(this, SIGNAL(errorMessage(QString const&)),
//        dockHandler_, SIGNAL(errorMessage(QString const&)), Qt::UniqueConnection);
//    connect(this, SIGNAL(noticeMessage(QString const&)),
//        dockHandler_, SIGNAL(noticeMessage(QString const&)), Qt::UniqueConnection);

//    connect(this, SIGNAL(helpUrlRequested(QString const&)),
//        dockHandler_, SIGNAL(helpUrlRequested(QString const&)), Qt::UniqueConnection);

#endif // LOG_OUT_SUPPORT_20240104

#endif // #if DOCKWIDGETHANDLER_20231228 == 0

    // smallSideMenuBar connent
    connect(smallSideMenuBar_, SIGNAL(newBus()), this, SLOT(onNewBus()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(newComponent()), this, SLOT(onNewComponent()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(newProject()), this, SLOT(onNewProject()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(newDesign()), this, SLOT(onNewDesign()), Qt::UniqueConnection);

    connect(smallSideMenuBar_, SIGNAL(openProject()), this, SLOT(onOpenProject()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(closeProject()), this, SLOT(onCloseProject()), Qt::UniqueConnection);

    connect(smallSideMenuBar_, SIGNAL(importProject()), this, SLOT(onImportProject()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(exportProject()), this, SLOT(onExportProject()), Qt::UniqueConnection);

    connect(smallSideMenuBar_, SIGNAL(saveDocument()), this, SLOT(onSave()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(saveAllDocument()), this, SLOT(onSaveAll()), Qt::UniqueConnection);

    connect(smallSideMenuBar_, SIGNAL(runSimulatePre()), this, SLOT(onRunSimulatePre()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(runSimulate()), this, SLOT(onSRunimulate()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(runSynthesis()), this, SLOT(onRunSynthesis()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(runFPGA()), this, SLOT(onRunFPGA()), Qt::UniqueConnection);

    connect(smallSideMenuBar_, SIGNAL(openPreference()), this, SLOT(onPreference()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(openHelp()), this, SLOT(onHelp()), Qt::UniqueConnection);
    connect(smallSideMenuBar_, SIGNAL(openAbout()), this, SLOT(onAbout()), Qt::UniqueConnection);

    // bigSideMenuBar connent
    connect(bigSideMenuBar_, SIGNAL(newBus()), this, SLOT(onNewBus()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(newComponent()), this, SLOT(onNewComponent()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(newProject()), this, SLOT(onNewProject()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(newDesign()), this, SLOT(onNewDesign()), Qt::UniqueConnection);

    connect(bigSideMenuBar_, SIGNAL(openProject()), this, SLOT(onOpenProject()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(closeProject()), this, SLOT(onCloseProject()), Qt::UniqueConnection);

    connect(bigSideMenuBar_, SIGNAL(importProject()), this, SLOT(onImportProject()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(exportProject()), this, SLOT(onExportProject()), Qt::UniqueConnection);

    connect(bigSideMenuBar_, SIGNAL(saveDocument()), this, SLOT(onSave()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(saveAllDocument()), this, SLOT(onSaveAll()), Qt::UniqueConnection);

    connect(bigSideMenuBar_, SIGNAL(runSimulatePre()), this, SLOT(onRunSimulatePre()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(runSimulate()), this, SLOT(onSRunimulate()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(runSynthesis()), this, SLOT(onRunSynthesis()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(runFPGA()), this, SLOT(onRunFPGA()), Qt::UniqueConnection);

    connect(bigSideMenuBar_, SIGNAL(openPreference()), this, SLOT(onPreference()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(openHelp()), this, SLOT(onHelp()), Qt::UniqueConnection);
    connect(bigSideMenuBar_, SIGNAL(openAbout()), this, SLOT(onAbout()), Qt::UniqueConnection);

}



void AWSideToolBar::resizeBtnClicked() {
    if(this->width()>60)
    {
        this->setFixedWidth(30);
        resizeBtn->setFixedWidth(30);
        stackedLayout->setCurrentWidget(smallSideMenuBar_);
    }
    else
    {
        this->setFixedWidth(100);
        resizeBtn->setFixedWidth(100);
        stackedLayout->setCurrentWidget(bigSideMenuBar_);
    }
 //   qDebug() << "Open button clicked.";
}



void AWSideToolBar::onActionNewTriggered()
{

}



void AWSideToolBar::onActionSaveTriggered()
{
}



void AWSideToolBar::onActionSaveAllTriggered()
{

}



void AWSideToolBar::onNewBus()
{
    // I/O Magic 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        mainWindow->OnCreateNewBus();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onNewComponent()
{
    // IP 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        mainWindow->onCreateNewHWComponent();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onNewProject()
{
    // Project 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        mainWindow->onCreateNewProject();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onNewDesign()
{
    // Design 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        mainWindow->onCreateNewDesign();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onOpenProject()
{
    QString location = QSettings().value(QStringLiteral("ROOT/Location")).toString();

    location = location + "/project";

    QFileDialog browseDialog(this, tr("Select proejct folder to open"), location);
    browseDialog.setFileMode(QFileDialog::Directory);
    if (browseDialog.exec() == QDialog::Accepted)
    {
        QStringList selectedFiles = browseDialog.selectedFiles();

        if (!selectedFiles.isEmpty())
        {
            QString selectedProejctFolder = selectedFiles.first();
            QString selectedScriptFolder = selectedProejctFolder + "/Script";

            QSettings().setValue("Project/DefaultLocation", selectedProejctFolder);

            QSettings().setValue("Script/Location", selectedScriptFolder);

            MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
            if(mainWindow) {
                QString selectedProjectName = selectedProejctFolder.replace(location + "/", "");

                emit inputMessage("open project " + selectedProjectName);

                emit noticeMessage("[ SYSTEM ] | Open | Project opened.");

                mainWindow->onProjectSearch();
                mainWindow->onProjectLibrarySearch();
            } else {
                qDebug() << "MainWindow instance is nullptr";
            }
        }
    }
}



void AWSideToolBar::onCloseProject()
{
    QSettings().setValue("Project/DefaultLocation", "");

    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        emit inputMessage("close project");

        emit noticeMessage("[ SYSTEM ] | Close | Project closed.");

        mainWindow->onProjectSearch();
        mainWindow->onProjectLibrarySearch();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onImportProject()
{
    // Save All 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        mainWindow->onImportProject();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onExportProject()
{
    // Save All 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        mainWindow->onExportProject();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onSave()
{
    // Save 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        emit inputMessage("save");
        mainWindow->saveCurrentDocument();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onSaveAll()
{
    // Save All 메뉴 항목을 클릭했을 때 실행될 코드
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();
    if(mainWindow) {
        emit inputMessage("saveall");
        mainWindow->saveAll();
    } else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onRunSimulatePre()
{
    // Sumulate 메뉴 항목을 클릭했을 때 실행될 코드
    PythonSimulate *dlg = new PythonSimulate(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);  // 창이 닫힐 때 객체가 자동으로 삭제되도록 설정합니다.

    connect(dlg, SIGNAL(inputMessage(const QString&)), this, SIGNAL(inputMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(undefinedMessage(const QString&)), this, SIGNAL(undefinedMessage(const QString&)), Qt::UniqueConnection);

    dlg->show();
    dlg->raise();
}



void AWSideToolBar::onRunSimulate()
{
    // Design 메뉴 항목을 클릭했을 때 실행될 코드
    PythonScriptRunner *dlg = new PythonScriptRunner(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);  // 창이 닫힐 때 객체가 자동으로 삭제되도록 설정합니다.
    //dialog->Create();

    connect(dlg, SIGNAL(inputMessage(const QString&)), this, SIGNAL(inputMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(undefinedMessage(const QString&)), this, SIGNAL(undefinedMessage(const QString&)), Qt::UniqueConnection);

    dlg->show();
    dlg->raise();
}



void AWSideToolBar::onRunSynthesis()
{
    // Synthesis 메뉴 항목을 클릭했을 때 실행될 코드
    SynthesisDialog* dlg = new SynthesisDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);  // 창이 닫힐 때 객체가 자동으로 삭제되도록 설정합니다.

    connect(dlg, SIGNAL(inputMessage(const QString&)), this, SIGNAL(inputMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(undefinedMessage(const QString&)), this, SIGNAL(undefinedMessage(const QString&)), Qt::UniqueConnection);

    dlg->show();
    dlg->raise();
}



void AWSideToolBar::onRunFPGA()
{
    // FPGA 메뉴 항목을 클릭했을 때 실행될 코드
    PythonFPGADialog* dlg = new PythonFPGADialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);  // 창이 닫힐 때 객체가 자동으로 삭제되도록 설정합니다.

    connect(dlg, SIGNAL(inputMessage(const QString&)), this, SIGNAL(inputMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
    connect(dlg, SIGNAL(undefinedMessage(const QString&)), this, SIGNAL(undefinedMessage(const QString&)), Qt::UniqueConnection);

    dlg->show();
    dlg->raise();
}



void AWSideToolBar::onPreference()
{
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();

    if(mainWindow) {
        mainWindow->onOpenSettings();
    }

    else {
        qDebug() << "MainWindow instance is nullptr";
    }
}



void AWSideToolBar::onHelp()
{

}



void AWSideToolBar::onAbout()
{
    MainWindow* mainWindow = Singleton::getInstance().getMainWindow();

    if(mainWindow) {
        mainWindow->onShowAbout();
    }

    else {
        qDebug() << "MainWindow instance is nullptr";
    }

}
