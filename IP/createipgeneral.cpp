#include "createipgeneral.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QDir>

CreateIpGeneral::CreateIpGeneral(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("CreateIpGeneral { border: 1px solid black; }");
    QString borderStyle = "border: 1px solid black;";

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃

    mainLayout->addSpacing(30);

    QLabel *label1 = new QLabel("General", this);
    QFont font1;
    font1.setPointSize(16);
    font1.setBold(true);
    label1->setFont(font1);
    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label1->setContentsMargins(60, 0, 0, 0);
    mainLayout->addWidget(label1);

    QLabel *label2 = new QLabel("General information of IP", this);
    QFont font2;
    font2.setPointSize(12);
    label2->setFont(font2);
    label2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label2->setContentsMargins(60, 0, 30, 0);
    mainLayout->addWidget(label2);

    // 50px의 공백 추가
    mainLayout->addSpacing(50);

    // 기본 폰트 설정
    QFont baseFont;
    baseFont.setPointSize(10);
    baseFont.setBold(true);

    //------------------------

    QVBoxLayout *leftLayout = new QVBoxLayout(); // 좌측 레이아웃 (입력폼)

    // Name 입력폼
    QLabel *nameLabel = new QLabel("Name", this);
    nameTxt = new QLineEdit(this);
    nameTxt->setStyleSheet(borderStyle);
    nameTxt->setFixedSize(400, 20);
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameTxt);
    leftLayout->addLayout(nameLayout);

    // Version 입력폼
    QLabel *versionLabel = new QLabel("Version", this);
    versionTxt = new QLineEdit(this);
    versionTxt->setFixedSize(400, 20);
    versionTxt->setStyleSheet(borderStyle);
    QHBoxLayout *versionLayout = new QHBoxLayout();
    versionLayout->addWidget(versionLabel);
    versionLayout->addWidget(versionTxt);
    leftLayout->addLayout(versionLayout);

    // Vendor 입력폼
    QLabel *vendorLabel = new QLabel("Vendor", this);
    vendorTxt = new QLineEdit(this);
    vendorTxt->setFixedSize(400, 20);
    vendorTxt->setStyleSheet(borderStyle);
    QHBoxLayout *vendorLayout = new QHBoxLayout();
    vendorLayout->addWidget(vendorLabel);
    vendorLayout->addWidget(vendorTxt);
    leftLayout->addLayout(vendorLayout);

    // Author 입력폼
    QLabel *authorLabel = new QLabel("Author", this);
    authorTxt = new QLineEdit(this);
    authorTxt->setFixedSize(400, 20);
    authorTxt->setStyleSheet(borderStyle);
    QHBoxLayout *authorLayout = new QHBoxLayout();
    authorLayout->addWidget(authorLabel);
    authorLayout->addWidget(authorTxt);
    leftLayout->addLayout(authorLayout);

    // Description 입력폼
    QLabel *descriptionLabel = new QLabel("Description", this);
    descriptionTxt = new QTextEdit(this);
    descriptionTxt->setFixedSize(400, 60);
    QHBoxLayout *descriptionLayout = new QHBoxLayout();
    descriptionTxt->setStyleSheet(borderStyle);
    descriptionLayout->addWidget(descriptionLabel);
    descriptionLayout->addWidget(descriptionTxt);
    leftLayout->addLayout(descriptionLayout);

    // 우측 요약 텍스트 박스
    QTextEdit *summaryTxt = new QTextEdit(this);
    summaryTxt->setFixedSize(400, 200); // 크기를 조절할 수 있습니다.

    // 최종 레이아웃
    QHBoxLayout *middleLayout = new QHBoxLayout();
    middleLayout->addLayout(leftLayout); // 좌측 레이아웃 추가


    /*  right layout    */

    QVBoxLayout *rightLayout = new QVBoxLayout(); //right layout
    QLabel *previewLabel = new QLabel("Preview", this);
    previewLabel->setContentsMargins(40, 0, 10, 0);
    rightLayout->addWidget(previewLabel);
    rightLayout->addSpacing(10);
    summaryTxt->setContentsMargins(40, 0, 10, 0);
    rightLayout->addWidget(summaryTxt);
    middleLayout->addLayout(rightLayout); // right layout into middle

    mainLayout->addLayout(middleLayout);

    //////////////////////////////////////////////////////

    // 레이아웃 설정
    this->setLayout(mainLayout);

    //this->setFixedSize(900, 900);
    // 레이아웃 설정
    this->setLayout(mainLayout);
    //this->setFixedSize(1000, 900);  //0911제거

    // 위젯을 중앙에 배치하기 위한 그리드 레이아웃 생성
    QGridLayout *gridLayout = new QGridLayout();
    QWidget *centerWidget = new QWidget(this);
    centerWidget->setLayout(mainLayout);
    gridLayout->addWidget(centerWidget, 1, 1, 1, 1); // 중앙에 위젯을 배치합니다.
    //gridLayout->setRowStretch(0, 1);  // 위쪽 여백
    gridLayout->setRowStretch(2, 1);  // 아래쪽 여백
    //gridLayout->setColumnStretch(0, 1);  // 왼쪽 여백
    gridLayout->setColumnStretch(2, 1);  // 오른쪽 여백

    this->setLayout(gridLayout);  // 위젯에 그리드 레이아웃 설정


    //..DEBUG
    nameTxt->setText("myio");
    versionTxt ->setText("1.0");
    vendorTxt ->setText("asicland");
    authorTxt ->setText("tester");
}


QString CreateIpGeneral::IoabsDefFile()
{

    QString VendorValue = vendorTxt->text();
    QString NameValue = nameTxt->text();
    QString Author = authorTxt->text();
    QString VersionValue = versionTxt->text();
    QString DescriptionValue = descriptionTxt->toPlainText();
/*

    #ifdef DEBUG
    VendorValue="abc_vendor";
    NameValue ="abc_name";
    VersionValue="1.0";
    DescriptionValue="abc_description";
    Author = 'abc_author';
    #endif
*/
    if(VendorValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "vendor 정보가 없습니다.");
        return "";
    }
    if(NameValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "version 정보가 없습니다.");
        return "";
    }
    if(Author.isEmpty())
    {
        QMessageBox::information(this, "General",  "author 정보가 없습니다.");
        return "";
    }
    if(VersionValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
        return "";
    }


    QString libraryPath = QDir::homePath() + "/AMagic/library";
    QString savepath   = libraryPath +"/" +VendorValue + "/io/"+ NameValue + "/" + VersionValue + "/";
    QString _IoabsDefFile = savepath + NameValue+".absDef."+VersionValue + ".xml";
    return _IoabsDefFile;
}

