#include "createiogeneral.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
//#include "Constructor/io_xmlmgr.h"
#include "Constructor/ioxmlgen.h"
#include "Constructor/ipxactparser.h"
#include <QMessageBox>

CreateIOGeneral::CreateIOGeneral(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("CreateIOWirePort { border: 1px solid black; }");

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

    QLabel *label2 = new QLabel("General information of I/O", this);
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

    QString borderStyle = "border: 1px solid black;";

    // Name 라벨 및 TextBox
    QLabel *nameLabel = new QLabel("Name", this);
    nameLabel->setFont(baseFont);
    nameLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    nameTxt = new QLineEdit(this);
    nameTxt->setFixedSize(400, 20);
    nameTxt->setAlignment(Qt::AlignLeft);
    nameTxt->setStyleSheet(borderStyle);

    QHBoxLayout *nameLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameTxt);
    mainLayout->addLayout(nameLayout);

    // Version 라벨 및 TextBox
    QLabel *versionLabel = new QLabel("Version", this);
    versionLabel->setFont(baseFont);
    versionLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    versionTxt = new QLineEdit(this);
    versionTxt->setFixedSize(400, 20);
    versionTxt->setAlignment(Qt::AlignLeft);
    versionTxt->setStyleSheet(borderStyle);

    QHBoxLayout *versionLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    versionLayout->addWidget(versionLabel);
    versionLayout->addWidget(versionTxt);
    mainLayout->addLayout(versionLayout);

    // Vender 라벨 및 TextBox
    QLabel *venderLabel = new QLabel("Vendor", this);
    venderLabel->setFont(baseFont);
    venderLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    venderTxt = new QLineEdit(this);
    venderTxt->setFixedSize(400, 20);
    venderTxt->setAlignment(Qt::AlignLeft);
    venderTxt->setStyleSheet(borderStyle);

    QHBoxLayout *venderLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    venderLayout->addWidget(venderLabel);
    venderLayout->addWidget(venderTxt);
    mainLayout->addLayout(venderLayout);


    // Author 라벨 및 TextBox
    QLabel *authorLabel = new QLabel("Author", this);
    authorLabel->setFont(baseFont);
    authorLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    authorTxt = new QLineEdit(this);
    authorTxt->setFixedSize(400, 20);
    authorTxt->setAlignment(Qt::AlignLeft);
    authorTxt->setStyleSheet(borderStyle);

    QHBoxLayout *authorLayout = new QHBoxLayout();
    authorLayout->addWidget(authorLabel);
    authorLayout->addWidget(authorTxt);
    mainLayout->addLayout(authorLayout);

    // Description 라벨 및 TextBox
    QLabel *descriptionLabel = new QLabel("Description", this);
    descriptionLabel->setFont(baseFont);
    descriptionLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    descriptionTxt = new QTextEdit(this);
    descriptionTxt->setFixedSize(400, 60);
    descriptionTxt->setAlignment(Qt::AlignLeft);
    descriptionTxt->setStyleSheet(borderStyle);

    QHBoxLayout *descriptLayout = new QHBoxLayout();  // 이 부분이 새로 추가된 것입니다.
    descriptLayout->addWidget(descriptionLabel);
    descriptLayout->addWidget(descriptionTxt);
    mainLayout->addLayout(descriptLayout);


    // 레이아웃 설정
    this->setLayout(mainLayout);
    //this->setFixedSize(900, 900);
    // 레이아웃 설정
    this->setLayout(mainLayout);
    //this->setFixedSize(800, 900); //0911 제거

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
}

QString CreateIOGeneral::MakeXML(QTableWidget *table)
{
    IOXmlGen mgr;
    QString VendorValue = venderTxt->text();
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
        return false;
    }
    if(NameValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "version 정보가 없습니다.");
        return false;
    }
    if(Author.isEmpty())
    {
        QMessageBox::information(this, "General",  "author 정보가 없습니다.");
        return false;
    }
    if(VersionValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
        return false;
    }

    mgr.MakeGenXml(VendorValue, NameValue, VersionValue, DescriptionValue, Author);
    strCurDir = mgr.GetSavePath(VendorValue, NameValue, VersionValue);

    mgr.MakeWireXml(table, VendorValue, NameValue, VersionValue, DescriptionValue, Author);

//    IPXACTParser parser;
//    parser.parse(wireXMLfile);


    QString sFile = mgr.GetGeneralXMLPath(VendorValue, NameValue, VersionValue);

    QFileInfo fileInfo(sFile);
    return fileInfo.fileName();
}
/*
void CreateIOGeneral::GetGenralInfo(QString &vendor, QString &name, QString &version, QString &description, QString& author)
{
    if(vendor.isEmpty())
    {
        QMessageBox::information(this, "확인",  "vendor 정보가 없습니다.");
        return;
    }
    if(version.isEmpty())
    {
        QMessageBox::information(this, "확인",  "version 정보가 없습니다.");
        return;
    }
    if(author.isEmpty())
    {
        QMessageBox::information(this, "확인",  "author 정보가 없습니다.");
        return;
    }
    if(name.isEmpty())
    {
        QMessageBox::information(this, "확인",  "name 정보가 없습니다.");
        return;
    }

    vendor = venderTxt->text();
    name = nameTxt->text();
    version = versionTxt->text();
    description = descriptionTxt->toPlainText();
    author = authorTxt->text();
}
*/

QString CreateIOGeneral::LoadXML(QString xmlFile)
{
    QString vendor, name, version, description, author;

    IOXmlGen mgr;
    mgr.LoadGenXmlInfo(xmlFile, vendor, name, version, description, author);

    venderTxt->setText(vendor);
    nameTxt->setText(name);
    authorTxt->setText(author);
    versionTxt->setText(version);
    descriptionTxt->setText(description);

    return mgr.GetWireXMLPath(vendor, name, version);
}

QString CreateIOGeneral::GetCurrentFileDir()
{
    IOXmlGen mgr;
    QString VendorValue = venderTxt->text();
    QString NameValue = nameTxt->text();
    QString VersionValue = versionTxt->text();
    return mgr.GetSavePath(VendorValue, NameValue, VersionValue);
}
