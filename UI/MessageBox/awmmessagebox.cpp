#include "awmmessagebox.h"

#include <QPushButton>

AWMMessageBox::AWMMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
    setWindowTitle("Custom Message Box");
    setText("Do you accept?");

    // 버튼 생성
    QPushButton *acceptButton = new QPushButton("Accept", this);
    addButton(acceptButton, QMessageBox::AcceptRole);

    // 버튼을 accept 슬롯에 연결
    connect(acceptButton, &QPushButton::clicked, this, &QMessageBox::accept);
}

AWMMessageBox::~AWMMessageBox()
{
}
