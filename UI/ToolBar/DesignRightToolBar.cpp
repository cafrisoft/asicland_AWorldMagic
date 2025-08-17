#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "DesignRightToolBar.h"

DesignRightToolBar::DesignRightToolBar(QWidget *parent)
    : QWidget{parent}
{
    // Set the background color to blue
    setAutoFillBackground(true);
    setStyleSheet("background-color: #2682ED;"); //<- sky Blue

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 120, 1, 1); // Set all margins to 0 pixels

    QLabel *label = new QLabel("");
    label->setFixedHeight(24);
    layout->addSpacing(15);

    ProjectBtn = new QPushButton(this);
    ProjectBtn->setStyleSheet("border: none;");
    ProjectBtn->setIcon(QIcon(":/resources/Res/File.png"));
    ProjectBtn->setIconSize(QSize(24, 24));
    ProjectBtn->setFixedHeight(24);
    layout->addWidget(ProjectBtn);
    layout->addSpacing(15);

    this->setLayout(layout);

    connect(ProjectBtn, &QPushButton::clicked, this, &DesignRightToolBar::projectBtnClicked);
}

void DesignRightToolBar::projectBtnClicked()
{
}
