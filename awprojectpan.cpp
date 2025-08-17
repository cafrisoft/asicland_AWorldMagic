#include "awprojectpan.h"

#include <QApplication>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QVBoxLayout>

AWProjectPan::AWProjectPan(QWidget *parent)
    : QPushButton{parent}
{
    label = new QLabel("Hello World!", this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);  // add label to the layout
    layout->setStretchFactor(label, 1);  // Set the stretch factor to 1
    this->setLayout(layout);   // set this layout to AWProjectPan
}

void AWProjectPan::mousePressEvent(QMouseEvent *event)
{
    /*
    QMessageBox::information(
        nullptr,
        "메시지 박스 제목",
        "여기에 메시지 내용을 입력하세요.");
*/

    if(event->button() == Qt::LeftButton) { // Left mouse button is clicked
        QMenu menu(this);
        menu.addAction("Action 1");
        menu.addAction("Action 2");
        menu.addAction("Action 3");
        menu.exec(event->globalPos());
    }
}

/*
void AWProjectPan::resizeEvent(QResizeEvent* event)
{
    //QRect rc = parentWidget()->geometry();
    //QRect rc1 = this->geometry();
    this->setGeometry(parentWidget()->geometry());
}
*/
