#include "createiomain.h"
#include <QMessageBox>

CreateIOMain::CreateIOMain(QTabWidget *parent) : QWidget(parent), parentTabWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout;

        // 여백 설정: 위, 오른쪽, 아래, 왼쪽
    layout->setContentsMargins(0, 0, 0, 0); // 모든 방향에 2px의 여백 설정
    layout->setSpacing(0); // 위젯들 사이의 간격을 1px로 설정

    // QStackedWidget 초기화
    stackedWidgets = new QStackedWidget(this);

    wirePortWidget = new CreateIOWirePort(this);
    generalWidget = new CreateIOGeneral(this);

        // 좌측 메뉴

    // std::bind를 사용할 때 _1, _2 등은 플레이스홀더로,  나중에 실제 인자로 대체됩니다.
    using namespace std::placeholders;
    CreateIOLeftMenu *leftMenu = new CreateIOLeftMenu(std::bind(&CreateIOMain::ChangeTabTitlebyCallback, this, _1), this);

    leftMenu->SetLinkWindow(generalWidget, wirePortWidget);

    layout->addWidget(leftMenu);

    leftMenu->SetTargetWiget(this);

    // QStackedWidget에 위젯 추가
    stackedWidgets->addWidget(generalWidget);
    stackedWidgets->addWidget(wirePortWidget);

    layout->addWidget(stackedWidgets);
//    layout->addStretch(1);

    //stackedWidgets->setCurrentIndex(0);

    this->setLayout(layout);
    ShowWidget(true);
}

void CreateIOMain::ShowWidget(bool isGeneral) {
    if (isGeneral) {
        stackedWidgets->setCurrentWidget(generalWidget);
    } else {
        stackedWidgets->setCurrentWidget(wirePortWidget);
    }
}

//...
void CreateIOMain::ChangeTabTitlebyCallback(QString tabName) {
        // 콜백으로 호출될 함수의 내용
    //QMessageBox::information(this, "ChangeTabTitlebyCallback", QString("You TabName: %1").arg(tabName));

    int index = this->getCurrentIndex(); // 이
    if(index != -1) { // -1은 탭이 존재하지 않는 것을 의미합니다.
         parentTabWidget->setTabText(index, tabName);
    }
}
