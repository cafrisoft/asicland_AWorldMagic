#include "customtabbar.h"

CustomTabBar::CustomTabBar(QWidget *parent)
    : QTabBar{parent}
//    CustomTabBar(QWidget *parent = nullptr) : QTabBar(parent)
{
    // 우측 상단에 배치될 QPushButton을 생성합니다.
    QPushButton *button = new QPushButton(this);
    button->setText("Button");

    // QPushButton의 사이즈 정책을 설정합니다.
//0911    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // QPushButton을 QTabBar에 추가합니다.
   // setCornerWidget(button, Qt::TopRightCorner);
}
