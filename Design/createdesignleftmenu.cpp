#include "createdesignleftmenu.h"

#include <QVBoxLayout>
#include "ClickableLabel.h"

#include <UI/HWDesign/HWDesignWidget.h>

#include "createdesignmain.h"


CreateDesignLeftMenu::CreateDesignLeftMenu(QWidget *parent)
    : QWidget(parent)
//CreateDesignLeftMenu::CreateDesignLeftMenu(QWidget *parent, TabDocument* designSchematic) : QWidget{parent}
{

    setStyleSheet("CreateIOLeftMenu { border: 1px solid black; }");

    //setStyleSheet("border: 1px solid black;");


    QVBoxLayout *layout = new QVBoxLayout;

    createDesignLabel = new QLabel("Create\nDesign", this);
    createDesignLabel->setAlignment(Qt::AlignCenter);  // 텍스트를 중앙에 정렬합니다.
    QFont font = createDesignLabel->font();  // 현재 폰트를 가져옵니다.
    font.setBold(true);                  // Bold로 설정합니다.
    font.setPointSize(16);               // 폰트 크기를 20px로 설정합니다.
    createDesignLabel->setFont(font);        // 라벨에 폰트를 설정합니다.


    layout->addSpacing(10);
    layout->addWidget(createDesignLabel, 0, Qt::AlignCenter);

    ClickableLabel *generalLabel = new ClickableLabel("General", this);
    connect(generalLabel, &ClickableLabel::clicked, this, &CreateDesignLeftMenu::GeneralInfoInput);
    layout->addSpacing(35);
    layout->addWidget(generalLabel, 0, Qt::AlignCenter);

    ClickableLabel *fileSetLabel = new ClickableLabel("File Set", this);
    connect(fileSetLabel, &ClickableLabel::clicked, this, &CreateDesignLeftMenu::FileSetInput);
    layout->addSpacing(15);
    layout->addWidget(fileSetLabel, 0, Qt::AlignCenter);

    ClickableLabel *portMapLabel = new ClickableLabel("PortMap &\nParameter", this);
    connect(portMapLabel, &ClickableLabel::clicked, this, &CreateDesignLeftMenu::PortMapInput);
    layout->addSpacing(15);
    layout->addWidget(portMapLabel, 0, Qt::AlignCenter);

    ClickableLabel *SchematicLabel = new ClickableLabel("Schematic", this);
    connect(SchematicLabel, &ClickableLabel::clicked, this, &CreateDesignLeftMenu::SchematicInput);
    layout->addSpacing(15);
    layout->addWidget(SchematicLabel, 0, Qt::AlignCenter);

    layout->addStretch(1);

    QPushButton *createButton = new QPushButton("Create", this);
    connect(createButton, &QPushButton::clicked, this, &CreateDesignLeftMenu::handleCreateDesignInfo);
    layout->addWidget(createButton);

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    layout->addWidget(cancelButton);
    layout->setContentsMargins(5, 0, 5, 10);
    layout->setSpacing(5);
    connect(cancelButton, &QPushButton::clicked, this, &CreateDesignLeftMenu::handleCancelDesign);

    this->setLayout(layout);

    connect(this, &CreateDesignLeftMenu::GeneralInfoInput, this, &CreateDesignLeftMenu::handleGeneralInfoInput);
    connect(this, &CreateDesignLeftMenu::FileSetInput, this, &CreateDesignLeftMenu::handleFileSetInput);
    connect(this, &CreateDesignLeftMenu::PortMapInput, this, &CreateDesignLeftMenu::handlePortMapInput);
    connect(this, &CreateDesignLeftMenu::SchematicInput, this, &CreateDesignLeftMenu::handleSchematicInput);
    connect(this, &CreateDesignLeftMenu::CreateDesignInfo, this, &CreateDesignLeftMenu::handleCreateDesignInfo);
    connect(this, &CreateDesignLeftMenu::CancelDesign, this, &CreateDesignLeftMenu::handleCancelDesign);

//    connect(this, SIGNAL(createEmptyDesign), parent, SIGNAL(createEmptyDesign), Qt::UniqueConnection);


}

void CreateDesignLeftMenu::handleGeneralInfoInput()
{
    createDesignLabel->setText("Create\nDesign");
    designMain_->ShowWidget(DesignSubType::Design_General);
}

void CreateDesignLeftMenu::handleFileSetInput()
{
    //createDesignLabel->setText("File Set");
    designMain_->ShowWidget(DesignSubType::Design_FileSet);

}

void CreateDesignLeftMenu::handlePortMapInput()
{
    //createDesignLabel->setText("PortMap &\nParameter");
    designMain_->ShowWidget(DesignSubType::Design_PortMap);
}

void CreateDesignLeftMenu::handleSchematicInput()
{
    //createDesignLabel->setText("Schematic");
    designMain_->ShowWidget(DesignSubType::Design_Schematic);
}

void CreateDesignLeftMenu::handleCreateDesignInfo()
{
    designMain_->onCreateEmptyDesign();
//    emit createEmptyDesign();
}

void CreateDesignLeftMenu::handleCancelDesign()
{

}

void CreateDesignLeftMenu::SetTargetWiget(CreateDesignMain* pMain ,
                                          CreateDesignGeneral* generalWidget,
                                          CreateDesignFileSet* fileSetWidget,
                                          CreateDesignPortMap* portMapWidget,
                                          CreateDesignSchematic* designSchematic)
{
    designMain_ = pMain;
    generalWidget_ = generalWidget;
    fileSetWidget_ = fileSetWidget;
    portMapWidget_ = portMapWidget;
    designSchematic_ = designSchematic;

//    connect(this, SIGNAL(createEmptyDesign), designMain_, SLOT(onCreateEmptyDesign), Qt::UniqueConnection);
//    connect(this, SIGNAL(createEmptyDesign), pMain, SIGNAL(createEmptyDesign), Qt::UniqueConnection);
}
