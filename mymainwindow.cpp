#include "mymainwindow.h"

#if LIBRARYINTERFACE_TEST_20231206 == 0
#include <KactusAPI/LibraryHandler.h>
#else
#include <KactusAPI/LibraryInterface.h>
#endif


#include <UI/DrawingBoard/DrawingBoard.h>
#include <UI/Widgets/TabDocument/TabDocument.h>

//#include "awcodeedittab.h"
#include "./Editor/awmedit.h"

#include "IO/createiomain.h"
#include "IP/createipmain.h"
#include "Design/createdesignmain.h"
#include "./Project/createprojectmain.h"

#include <QLabel>
#include "QMessageBox"
#include <QFileInfo>
#include <QMessageBox>
//..#include <QWebEngineView>

#if LIBRARYINTERFACE_TEST_20231206 == 0
MyMainWindow::MyMainWindow(LibraryHandler* libInterface, QWidget *parent)
#else
MyMainWindow::MyMainWindow(LibraryInterface* libInterface, QWidget *parent)
#endif
    : QWidget(parent) ,
      library_(libInterface)
{
    //setStyleSheet("background-color: cyan;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

#if MYMAINWINDOW_TEST == 1
    setupDrawBoard();

    // Add the tab widget to the layout
    layout->addWidget(designTabs_);
    this->setLayout(layout);

#else
    QTabWidget *tabWidget = new QTabWidget(this);

    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_pTabWidget = tabWidget;
    tabWidget->setTabsClosable(true);

    // 탭이 닫힐 때의 동작을 구현합니다.
    connect(tabWidget, &QTabWidget::tabCloseRequested, [tabWidget](int index) {
        // 인덱스를 사용하여 해당 탭을 삭제합니다.
        QWidget* widget = tabWidget->widget(index);
        tabWidget->removeTab(index);

        // 필요에 따라 위젯을 삭제합니다.
        // 이는 위젯의 동적 생성 방식과 관련되므로 주의가 필요합니다.
        delete widget;
    });

    //AWMEdit *awmEdit = new AWMEdit(this); // AWMEdit를 MainWindow의 자식으로 생성
    //QWidget *edit= static_cast<QWidget *>(awmEdit);
    //tabWidget->addTab(edit, "Cpp Code");
    //awmEdit->OpenFile("test.cpp");

   /*
    AWMEdit *awmEdit2 = new AWMEdit(this); // AWMEdit를 MainWindow의 자식으로 생성
    QWidget *edit2= static_cast<QWidget *>(awmEdit2);
    tabWidget->addTab(edit2, "Cpp2 Code");
*/

//    QWidget *welcomeTab = new QWidget();
//    tabWidget->addTab(welcomeTab, "Welcome");
//    welcomeTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //..실제릴리즈시에는 코멘트아웃
//     /*CreateIOMain **/ workTab = new CreateIOMain(tabWidget);
    workTab = new CreateDesignMain(library_, tabWidget);
//    CreateDesignMain *workTab = new CreateDesignMain(library_, m_pTabWidget);
     workTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     int tabindex = tabWidget->addTab(workTab, "Unsaved Design");

     QObject::connect(workTab, &CreateDesignMain::createDesign, this, &MyMainWindow::createDesign);

     //welcomeTab->setStyleSheet("border: 5px solid black;");  .. 내부에 border가 생김
     //workTab->setStyleSheet("border: 1px solid black;");

     //tabWidget->setStyleSheet("border: 1px solid black;");  ... 모든 콘트롤에 border가 생김

     //this->setCentralWidget(awmEdit); // AWMEdit를 중앙 위젯으로 설정
     // Add the tab widget to the layout
     layout->addWidget(tabWidget);
     this->setLayout(layout);
#endif

}

void MyMainWindow::displayFilePathSlot(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString fileExtension = fileInfo.suffix().toLower(); // 파일 확장자 가져오기

    if (fileExtension == "c" || fileExtension == "cpp" || fileExtension == "txt"
        || fileExtension == "h" || fileExtension == "hpp") {

        // 파일을 열거나, 다른 동작을 수행
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "파일 열기 오류", "파일을 열 수 없습니다.");
            return;
        }

#if MYMAINWINDOW_TEST == 1
        //QMessageBox::information(this, "File Path", filePath);
        AWMEdit *awmEdit = new AWMEdit(this); // AWMEdit를 MainWindow의 자식으로 생성
        QWidget *edit= static_cast<QWidget *>(awmEdit);
        designTabs_->addTab(edit, "Cpp");
        // Set the last tab as the current tab
        designTabs_->setCurrentIndex(designTabs_->count() - 1);
#else
        //QMessageBox::information(this, "File Path", filePath);
        AWMEdit *awmEdit = new AWMEdit(this); // AWMEdit를 MainWindow의 자식으로 생성
        QWidget *edit= static_cast<QWidget *>(awmEdit);
        m_pTabWidget->addTab(edit, "Cpp");
        // Set the last tab as the current tab
        m_pTabWidget->setCurrentIndex(m_pTabWidget->count() - 1);
#endif

        awmEdit->OpenFile(filePath);

        file.close();
    } else {
        if(fileExtension.length()>1)
        {
            // 지원하지 않는 확장자일 때 메시지 박스 표시
            QMessageBox::information(this, "지원하지 않는 확장자", "해당 파일 확장자는 지원하지 않습니다.");
        }
    }
}

#if MYMAINWINDOW_TEST == 1
//-----------------------------------------------------------------------------
// Function: MainWindow::setupDrawBoard()
//-----------------------------------------------------------------------------
void MyMainWindow::setupDrawBoard()
{
    designTabs_ = new DrawingBoard();
    designTabs_->setMovable(true);
    designTabs_->setTabsClosable(true);

//    designTabs_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    connect(designTabs_, SIGNAL(lastDocumentClosed()), this, SLOT(onLastDocumentClosed()), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(currentChanged(int)), this, SLOT(onDocumentChanged(int)), Qt::UniqueConnection);

    connect(designTabs_, SIGNAL(documentContentChanged()), this, SLOT(updateMenuStrip()), Qt::UniqueConnection);
    connect(designTabs_, SIGNAL(documentEditStateChanged()), this, SLOT(updateMenuStrip()));

    connect(designTabs_, SIGNAL(helpUrlRequested(QString const&)),
        this, SIGNAL(helpUrlRequested(QString const&)), Qt::UniqueConnection);

//    connect(designTabs_, SIGNAL(errorMessage(const QString&)),
//        dockHandler_, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
//    connect(designTabs_, SIGNAL(noticeMessage(const QString&)),
//        dockHandler_, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);

}
#endif



#if MYMAINWINDOW_TEST == 1
void MyMainWindow::AddTapWindow(const QString &type, QString xmlFullFile, TabDocument *tabDocument )
{
    if(type == "unsavedio")
    {
        CreateIOMain *workTab = new CreateIOMain(designTabs_);
        designTabs_->addTab(workTab, "Unsaved I/O");

        xmlFullFile.replace(".absDef", "");
        QString wireXmlFullPath = workTab->generalWidget->LoadXML(xmlFullFile);

        // .absDef 있는 이름으로 변경
         workTab->wirePortWidget->LoadWireXML(wireXmlFullPath);

        //파일명으로 탭의 제목을 바꾼다.
         QFileInfo fileInfo(xmlFullFile);
         workTab->ChangeTabTitlebyCallback(fileInfo.fileName());

/*
        if(xmlFullFile.contains("absDef")>0)
        {
            workTab->wirePortWidget->LoadXML(xmlFullFile);
        }
        else
        {
            workTab->generalWidget->LoadXML(xmlFullFile);
        }
*/

        designTabs_->setCurrentWidget(workTab);
    }

    if(type == "unsavedip")
    {
        CreateIPMain *workTab = new CreateIPMain(designTabs_);
        designTabs_->addTab(workTab, "Unsaved IP");

        if (xmlFullFile.contains("/ip/")) {
            //workTab->portMapWidget->LoadXML(xmlFullFile);
        }
        designTabs_->setCurrentWidget(workTab);
    }

    if(type == "unsavedproject")
    {
        CreateProjectMain *workTab = new CreateProjectMain(designTabs_);
        designTabs_->addTab(workTab, "Unsaved Project");
        designTabs_->setCurrentWidget(workTab);
    }

    if(type == "unsaveddesign")
    {
//        CreateDesignMain *workTab = new CreateDesignMain(designTabs_);
        CreateDesignMain *workTab = new CreateDesignMain(tabDocument, designTabs_);
        designTabs_->addTab(workTab, "Unsaved Design");
        designTabs_->setCurrentWidget(workTab);
    }
}

void MyMainWindow::AddTapWindow(TabDocument *tabDocument )
{
    CreateDesignMain *workTab = new CreateDesignMain(tabDocument, designTabs_);
    designTabs_->addTab(workTab, "Unsaved Design");
//    designTabs_->setCurrentWidget(workTab);

//    designTabs_->addAndOpenDocument(workTab);
    designTabs_->setCurrentWidget(workTab);

}

#else
void MyMainWindow::AddTapWindow(const QString &type, QString xmlFullFile )
{
    if(type == "unsavedio")
    {
        CreateIOMain *workTab = new CreateIOMain(m_pTabWidget);
        m_pTabWidget->addTab(workTab, "Unsaved I/O");

        xmlFullFile.replace(".absDef", "");
        QString wireXmlFullPath = workTab->generalWidget->LoadXML(xmlFullFile);

        // .absDef 있는 이름으로 변경
         workTab->wirePortWidget->LoadWireXML(wireXmlFullPath);

        //파일명으로 탭의 제목을 바꾼다.
         QFileInfo fileInfo(xmlFullFile);
         workTab->ChangeTabTitlebyCallback(fileInfo.fileName());

/*
        if(xmlFullFile.contains("absDef")>0)
        {
            workTab->wirePortWidget->LoadXML(xmlFullFile);
        }
        else
        {
            workTab->generalWidget->LoadXML(xmlFullFile);
        }
*/

        m_pTabWidget->setCurrentWidget(workTab);
    }

    if(type == "unsavedip")
    {
        CreateIPMain *workTab = new CreateIPMain(m_pTabWidget);
        m_pTabWidget->addTab(workTab, "Unsaved IP");

        if (xmlFullFile.contains("/ip/")) {
            //workTab->portMapWidget->LoadXML(xmlFullFile);
        }
        m_pTabWidget->setCurrentWidget(workTab);
    }

    if(type == "unsavedproject")
    {
        CreateProjectMain *workTab = new CreateProjectMain(m_pTabWidget);
        m_pTabWidget->addTab(workTab, "Unsaved Project");
        m_pTabWidget->setCurrentWidget(workTab);
    }

    if(type == "unsaveddesign")
    {
        CreateDesignMain *workTab = new CreateDesignMain(library_, m_pTabWidget);
        m_pTabWidget->addTab(workTab, "Unsaved Design");
        m_pTabWidget->setCurrentWidget(workTab);
    }
}

void MyMainWindow::AddTapWindow(TabDocument *tabDocument )
{
    CreateDesignMain *workTab = new CreateDesignMain(library_, tabDocument, m_pTabWidget);
    m_pTabWidget->addTab(workTab, "Unsaved Design");
    m_pTabWidget->setCurrentWidget(workTab);
}

#endif

