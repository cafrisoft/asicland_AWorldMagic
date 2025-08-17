#include "createipleftmenu.h"
#include <QVBoxLayout>
#include "ClickableLabel.h"
#include "createipmain.h"

CreateIpLeftMenu::CreateIpLeftMenu(QWidget *parent, CreateIpPortMap* ipPortMap) : QWidget{parent}
{

    _ipPortMap = ipPortMap;

    setStyleSheet("CreateIOLeftMenu { border: 1px solid black; }");

    //setStyleSheet("border: 1px solid black;");


    QVBoxLayout *layout = new QVBoxLayout;

    createIPLabel = new QLabel("Create\nIP", this);
    createIPLabel->setAlignment(Qt::AlignCenter);  // 텍스트를 중앙에 정렬합니다.
    QFont font = createIPLabel->font();  // 현재 폰트를 가져옵니다.
    font.setBold(true);                  // Bold로 설정합니다.
    font.setPointSize(16);               // 폰트 크기를 20px로 설정합니다.
    createIPLabel->setFont(font);        // 라벨에 폰트를 설정합니다.


    layout->addSpacing(10);
    layout->addWidget(createIPLabel, 0, Qt::AlignCenter);

    ClickableLabel *generalLabel = new ClickableLabel("General", this);
    connect(generalLabel, &ClickableLabel::clicked, this, &CreateIpLeftMenu::GeneralInfoInput);
    layout->addSpacing(35);
    layout->addWidget(generalLabel, 0, Qt::AlignCenter);

    ClickableLabel *fileSetLabel = new ClickableLabel("File Set", this);
    connect(fileSetLabel, &ClickableLabel::clicked, this, &CreateIpLeftMenu::FileSetInput);
    layout->addSpacing(15);
    layout->addWidget(fileSetLabel, 0, Qt::AlignCenter);

    ClickableLabel *portortMapLabel = new ClickableLabel("PortMap &\nParameter", this);
    connect(portortMapLabel, &ClickableLabel::clicked, this, &CreateIpLeftMenu::PortMapInput);
    layout->addSpacing(15);
    layout->addWidget(portortMapLabel, 0, Qt::AlignCenter);

    layout->addStretch(1);

    QPushButton *createButton = new QPushButton("Create", this);
    connect(createButton, &QPushButton::clicked, this, &CreateIpLeftMenu::handleCreateIPInfo);
    layout->addWidget(createButton);

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    layout->addWidget(cancelButton);
    layout->setContentsMargins(5, 0, 5, 10);
    layout->setSpacing(5);
    connect(cancelButton, &QPushButton::clicked, this, &CreateIpLeftMenu::handleCancelIP);

    this->setLayout(layout);

    connect(this, &CreateIpLeftMenu::GeneralInfoInput, this, &CreateIpLeftMenu::handleGeneralInfoInput);
    connect(this, &CreateIpLeftMenu::FileSetInput, this, &CreateIpLeftMenu::handleFileSetInput);
    connect(this, &CreateIpLeftMenu::PortMapInput, this, &CreateIpLeftMenu::handlePortMapInput);
    connect(this, &CreateIpLeftMenu::CreateIPInfo, this, &CreateIpLeftMenu::handleCreateIPInfo);
    connect(this, &CreateIpLeftMenu::CancelIP, this, &CreateIpLeftMenu::handleCancelIP);

}

void CreateIpLeftMenu::handleGeneralInfoInput()
{
    createIPLabel->setText("Create\nIP");
    ipMain->ShowWidget(IpSubType::__General);
}

void CreateIpLeftMenu::handleFileSetInput()
{
    //createIPLabel->setText("File Set");
    ipMain->ShowWidget(IpSubType::FileSet);

}

void CreateIpLeftMenu::handlePortMapInput()
{
    //createIPLabel->setText("PortMap &\nParameter");
    ipMain->ShowWidget(IpSubType::PortMap);
}

void CreateIpLeftMenu::handleCreateIPInfo()
{

    _ipPortMap->CreateIpXml();
}

void CreateIpLeftMenu::handleCancelIP()
{

}
