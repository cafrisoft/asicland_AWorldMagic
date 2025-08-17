#include "createprojectgeneral.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>

QString selectDirectory(QWidget *parent = nullptr) {
    // QFileDialog를 사용하여 폴더 선택 대화상자를 엽니다.
    // 이 함수는 선택한 폴더의 경로를 반환하거나, 선택하지 않으면 빈 문자열을 반환합니다.
    QString dir = QFileDialog::getExistingDirectory(parent, "Select a folder", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 선택한 폴더의 이름만을 반환하려면 QFileInfo를 사용하여 폴더명을 추출할 수 있습니다.
    return QFileInfo(dir).filePath();
}

CreateProjectGeneral::CreateProjectGeneral(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("CreateProjectGeneral { border: 1px solid black; }");

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

    QLabel *label2 = new QLabel("General information of Project", this);
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


    // SaveDirectory 추가

    QLabel *savedirLabel = new QLabel("Save Directory", this);
    savedirLabel->setFont(baseFont);
    savedirLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    saveDirTxt = new QLineEdit(this);
    saveDirTxt->setFixedSize(300, 20);
    saveDirTxt->setAlignment(Qt::AlignLeft);
    saveDirTxt->setStyleSheet(borderStyle);

    QPushButton *browiseButton = new QPushButton("Browse", this);
    connect(browiseButton, &QPushButton::clicked, this, &CreateProjectGeneral::handleBrowse);


    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *layoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    layoutWidget->setFixedSize(400, 35);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *saveDirSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    saveDirSubLayout->addWidget(saveDirTxt);
    saveDirSubLayout->addWidget(browiseButton);

    // 3. 위젯에 레이아웃을 설정합니다.
    layoutWidget->setLayout(saveDirSubLayout);

    QHBoxLayout *saveDirLayout = new QHBoxLayout();
    saveDirLayout->addWidget(savedirLabel);
    saveDirLayout->addWidget(layoutWidget);

    mainLayout->addLayout(saveDirLayout);



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

QString CreateProjectGeneral::MakeXML(QTableWidget *table)
{
    //IOXmlGen mgr;
    QString VendorValue = venderTxt->text();
    QString NameValue = nameTxt->text();
    QString Author = authorTxt->text();
    QString saveDirValue = saveDirTxt->text();
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
    if(saveDirValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "saveDirValue 정보가 없습니다.");
        return false;
    }

//    QString sFile = mgr.GetGeneralXMLPath(VendorValue, NameValue, VersionValue);
//    QFileInfo fileInfo(sFile);
//    return fileInfo.fileName();
    return  "";
}

QString CreateProjectGeneral::LoadXML(QString xmlFile)
{
    QString vendor, name, version, description, author;
/*
    IOXmlGen mgr;
    mgr.LoadGenXmlInfo(xmlFile, vendor, name, version, description, author);

    venderTxt->setText(vendor);
    nameTxt->setText(name);
    authorTxt->setText(author);
    versionTxt->setText(version);
    descriptionTxt->setText(description);

    return mgr.GetWireXMLPath(vendor, name, version);
    */
    return "";
}

QString CreateProjectGeneral::GetCurrentFileDir()
{
    //IOXmlGen mgr;
    QString VendorValue = venderTxt->text();
    QString NameValue = nameTxt->text();
    QString saveDirValue = saveDirTxt->text();
   //return mgr.GetSavePath(VendorValue, NameValue, VersionValue);
    return saveDirValue;
}

//프로젝트 이름을 구함
QString CreateProjectGeneral::GetProjectFileName()
{
    //IOXmlGen mgr;
    QString VendorValue = venderTxt->text();
    QString NameValue = nameTxt->text();
    QString saveDirValue = saveDirTxt->text();
   //return mgr.GetSavePath(VendorValue, NameValue, VersionValue);
    return NameValue;
}

void CreateProjectGeneral::handleBrowse()
{
    QString folderName = selectDirectory(this);
    if(!folderName.isEmpty()) {
        // 폴더를 선택한 경우의 처리
        saveDirTxt->setText(folderName);
    } else {
        // 폴더를 선택하지 않은 경우의 처리
    }
}
