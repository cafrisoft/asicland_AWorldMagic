#include "createdesignmain.h"
#include "createdesignleftmenu.h"

#if LIBRARYINTERFACE_TEST_20231206 == 0
#include <KactusAPI/LibraryHandler.h>
#else
#include <KactusAPI/LibraryInterface.h>
#endif


#include <UI/HWDesign/HWDesignWidget.h>

#include <QStackedWidget>
#include <QWidget>


#if LIBRARYINTERFACE_TEST_20231206 == 0
CreateDesignMain::CreateDesignMain(LibraryHandler* libInterface, TabDocument* dw, QWidget *parent)
#else
CreateDesignMain::CreateDesignMain(LibraryInterface* libInterface, TabDocument* dw, QWidget *parent)
#endif
    : QWidget(parent) ,
      library_(libInterface)
{

    // QStackedWidget 초기화
    stackedWidgets = new QStackedWidget(this);

    generalWidget = new CreateDesignGeneral(library_, this);
    fileSetWidget = new CreateDesignFileSet(this, generalWidget);
    portMapWidget = new CreateDesignPortMap(this, generalWidget, fileSetWidget);
    schematicWidget = new CreateDesignSchematic(this, generalWidget, fileSetWidget, portMapWidget, dw);
//    schematicWidget = dw;


//    connect(this, SIGNAL(createEmptyDesign), this, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);

//    connect(this, SIGNAL(createEmptyDesign_General), generalWidget, SLOT(CreateDesignGeneral::onCreateEmptyDesign));
    connect(this, SIGNAL(createEmptyDesign_FileSet), fileSetWidget, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);
    connect(this, SIGNAL(createEmptyDesign_PortMap), portMapWidget, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);
    connect(this, SIGNAL(createEmptyDesign_Schematic), schematicWidget, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);

//    connect(this, SIGNAL(createEmptyDesign_General), generalWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_FileSet), fileSetWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_PortMap), portMapWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_Schematic), schematicWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);


//    connect(generalWidget, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
//            VLNV const&, QString const&)),
//        this, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
//            VLNV const&, QString const&)));

    connect(generalWidget, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            VLNV const&, QString const&)),
        this, SLOT(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
            VLNV const&, QString const&)));



    QHBoxLayout *layout = new QHBoxLayout;

    // 여백 설정: 위, 오른쪽, 아래, 왼쪽
    layout->setContentsMargins(0, 0, 0, 0); // 모든 방향에 2px의 여백 설정
    layout->setSpacing(0); // 위젯들 사이의 간격을 1px로 설정


    // 좌측 메뉴
    CreateDesignLeftMenu *leftMenu = new CreateDesignLeftMenu(this);
    layout->addWidget(leftMenu);


    leftMenu->SetTargetWiget(this,
                             generalWidget,
                             fileSetWidget,
                             portMapWidget,
                             schematicWidget);

    // QStackedWidget에 위젯 추가
    stackedWidgets->addWidget(generalWidget);
    stackedWidgets->addWidget(fileSetWidget);
    stackedWidgets->addWidget(portMapWidget);
    stackedWidgets->addWidget(schematicWidget);

    layout->addWidget(stackedWidgets);
    //    layout->addStretch(1);

    //stackedWidgets->setCurrentIndex(0);
//    fileSetWidget->AddTestData(); //..디버그용

    this->setLayout(layout);
    ShowWidget(DesignSubType::Design_General);
}


#if LIBRARYINTERFACE_TEST_20231206 == 0
CreateDesignMain::CreateDesignMain(LibraryHandler* libInterface, QWidget *parent)
#else
CreateDesignMain::CreateDesignMain(LibraryInterface* libInterface, QWidget *parent)
#endif
  : QWidget(parent),
    library_(libInterface),
    type_(Design_General)
{
    QHBoxLayout *layout = new QHBoxLayout;

    // 여백 설정: 위, 오른쪽, 아래, 왼쪽
    layout->setContentsMargins(0, 0, 0, 0); // 모든 방향에 2px의 여백 설정
    layout->setSpacing(0); // 위젯들 사이의 간격을 1px로 설정


    // QStackedWidget 초기화
    stackedWidgets = new QStackedWidget(this);

    generalWidget = new CreateDesignGeneral(library_, this);
    fileSetWidget = new CreateDesignFileSet(this, generalWidget);
    portMapWidget = new CreateDesignPortMap(this, generalWidget, fileSetWidget);
    schematicWidget = new CreateDesignSchematic(this, generalWidget, fileSetWidget, portMapWidget, nullptr);
//    schematicWidget = dw;

//    if (connect(this, SIGNAL(createEmptyDesign), this, SLOT(onCreateEmptyDesign) ) ) {
//        qDebug("");

//    }
//    else {
//        qDebug("");

//    }


//    connect(this, SIGNAL(createEmptyDesign_General), generalWidget, SLOT(CreateDesignGeneral::onCreateEmptyDesign));
//    connect(this, SIGNAL(createEmptyDesign_FileSet), fileSetWidget, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_PortMap), portMapWidget, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_Schematic), schematicWidget, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);

//    connect(this, SIGNAL(createEmptyDesign_General), generalWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_FileSet), fileSetWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_PortMap), portMapWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign_Schematic), schematicWidget, SIGNAL(createEmptyDesign), Qt::UniqueConnection);


//    connect(generalWidget, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
//            VLNV const&, QString const&)),
//        this, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
//            VLNV const&, QString const&)));

    QObject::connect(generalWidget, &CreateDesignGeneral::createDesign, this, &CreateDesignMain::createDesign);

//    connect(generalWidget, SIGNAL(createDesign(VLNV const&, QString const&)),
//        this, SLOT(onCreateDesign(VLNV const&, QString const&)));

//    connect(generalWidget, SIGNAL(createDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
//            VLNV const&, QString const&)),
//        this, SLOT(onCreateDesign(KactusAttribute::ProductHierarchy, KactusAttribute::Firmness,
//            VLNV const&, QString const&)));



    // 좌측 메뉴
    CreateDesignLeftMenu *leftMenu = new CreateDesignLeftMenu(this);
    layout->addWidget(leftMenu);


    leftMenu->SetTargetWiget(this,
                             generalWidget,
                             fileSetWidget,
                             portMapWidget,
                             schematicWidget);

    // QStackedWidget에 위젯 추가
    stackedWidgets->addWidget(generalWidget);
    stackedWidgets->addWidget(fileSetWidget);
    stackedWidgets->addWidget(portMapWidget);
    stackedWidgets->addWidget(schematicWidget);

    layout->addWidget(stackedWidgets);
    //    layout->addStretch(1);

    //stackedWidgets->setCurrentIndex(0);
//    fileSetWidget->AddTestData(); //..디버그용

    this->setLayout(layout);
    ShowWidget(DesignSubType::Design_General);

}


//-----------------------------------------------------------------------------
// Function: mainwindow::createDesign()
//-----------------------------------------------------------------------------
//void CreateDesignMain::createDesign(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
void CreateDesignMain::onCreateDesign(VLNV const& vlnv, QString const& directory)
{
    Q_ASSERT(vlnv.isValid());
//    VLNV vlnv2 = vlnv;

}

void CreateDesignMain::onCreateEmptyDesign()
{
    switch (this->GetCurrentWidgetType())
    {
    case DesignSubType::Design_General:
//        emit createEmptyDesign_General();
        generalWidget->onCreateEmptyDesign();
        break;
    case DesignSubType::Design_FileSet:
        emit createEmptyDesign_FileSet();
        break;
    case DesignSubType::Design_PortMap:
        emit createEmptyDesign_PortMap();
        break;
    case DesignSubType::Design_Schematic:
        emit createEmptyDesign_Schematic();
        break;
    }
}

void CreateDesignMain::ShowWidget(enum DesignSubType type)
{
    type_ = type;

    switch(type)
    {
    case DesignSubType::Design_General:
        stackedWidgets->setCurrentWidget(generalWidget);
        break;
    case DesignSubType::Design_FileSet:
        stackedWidgets->setCurrentWidget(fileSetWidget);
        break;
    case DesignSubType::Design_PortMap:
        stackedWidgets->setCurrentWidget(portMapWidget);
        break;
    case DesignSubType::Design_Schematic:
        stackedWidgets->setCurrentWidget(schematicWidget);
        break;
    }
}

DesignSubType CreateDesignMain::GetCurrentWidgetType()
{
    return type_;
}
