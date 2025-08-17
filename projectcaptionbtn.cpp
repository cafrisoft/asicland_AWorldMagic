#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QSpacerItem>

#include "projectcaptionbtn.h"
#include "custombutton.h"


ProjectCaptionBtn::ProjectCaptionBtn(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("border: 1px solid black;");
    setStyleSheet("background-color: green;");

    //this->setText("Project");
    //QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QPushButton *button1 = new QPushButton(this);
    button1->setIcon(QIcon(":/resources/Res/import.png"));  // Set the image
    button1->setIconSize(QSize(16, 16));  // Set the image size
    button1->show();  // Show the button

    QPushButton *button2 = new QPushButton(this);
    button2->setIcon(QIcon(":/resources/Res/export.png"));  // Set the image
    button2->setIconSize(QSize(16, 16));  // Set the image size
    button2->show();  // Show the button

    // Connect the button's clicked signal to our slot functions
    connect(button1, &QPushButton::clicked, this, &ProjectCaptionBtn::button1Clicked);
    connect(button2, &QPushButton::clicked, this, &ProjectCaptionBtn::button2Clicked);


    mainLayout->setAlignment(Qt::AlignRight);

    mainLayout->addWidget(button1);
    mainLayout->addWidget(button2);
    this->setLayout(mainLayout);

}

/*
ProjectCaptionBtn::ProjectCaptionBtn(QWidget *parent)
    : QPushButton{parent}
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    CustomButton *button1 = new CustomButton(":/resources/Res/import.png", this);
    CustomButton *button2 = new CustomButton(":/resources/Res/export.png", this);

    // Connect the button's clicked signal to our slot functions
    connect(button1, &QPushButton::clicked, this, &ProjectCaptionBtn::button1Clicked);
    connect(button2, &QPushButton::clicked, this, &ProjectCaptionBtn::button2Clicked);

    // Set alignment to the right
    mainLayout->setAlignment(Qt::AlignRight);

    mainLayout->addWidget(button1);
    mainLayout->addWidget(button2);

    this->setLayout(mainLayout);
}
*/
