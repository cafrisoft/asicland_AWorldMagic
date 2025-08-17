#include "createipmain.h"
#include "createipleftmenu.h"
#include <QStackedWidget>
#include <QWidget>


CreateIPMain::CreateIPMain(QWidget *parent) : QWidget(parent) {

    QHBoxLayout *layout = new QHBoxLayout;

    // 여백 설정: 위, 오른쪽, 아래, 왼쪽
    layout->setContentsMargins(0, 0, 0, 0); // 모든 방향에 2px의 여백 설정
    layout->setSpacing(0); // 위젯들 사이의 간격을 1px로 설정


    // QStackedWidget 초기화
    stackedWidgets = new QStackedWidget(this);

    generalWidget = new CreateIpGeneral(this);
    fileSetWidget = new CreateIpFileSet(this, generalWidget);
    portMapWidget = new CreateIpPortMap(this, generalWidget, fileSetWidget );


        // 좌측 메뉴
    CreateIpLeftMenu *leftMenu = new CreateIpLeftMenu(this, portMapWidget);
    layout->addWidget(leftMenu);


    leftMenu->SetTargetWiget(this);

    // QStackedWidget에 위젯 추가
    stackedWidgets->addWidget(generalWidget);
    stackedWidgets->addWidget(fileSetWidget);
    stackedWidgets->addWidget(portMapWidget);

    layout->addWidget(stackedWidgets);
    //    layout->addStretch(1);

    //stackedWidgets->setCurrentIndex(0);
    fileSetWidget->AddTestData(); //..디버그용

    this->setLayout(layout);
    ShowWidget(IpSubType::__General);
}

void CreateIPMain::ShowWidget(enum IpSubType type) {
    switch(type)
    {
    case __General:
        stackedWidgets->setCurrentWidget(generalWidget);
        break;
    case FileSet:
        stackedWidgets->setCurrentWidget(fileSetWidget);
        break;
    case PortMap:
        stackedWidgets->setCurrentWidget(portMapWidget);
        break;
    }
}
