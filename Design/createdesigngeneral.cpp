#include <GlobalBuildOptions.h>

#include "createdesigngeneral.h"

#if LIBRARYINTERFACE_TEST_20231206 == 0
#include <KactusAPI/LibraryHandler.h>
#else
#include <KactusAPI/LibraryInterface.h>
#endif


#include <IPXACTmodels/Component/Component.h>

#include <QCoreApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#ifdef CREATE_PROJECT_TEST
#include <QPushButton>
#include <QFileDialog>
#endif

#include <QMessageBox>
#include <QDir>


#if LIBRARYINTERFACE_TEST_20231206 == 0
CreateDesignGeneral::CreateDesignGeneral(LibraryHandler* libInterface, QWidget *parent)
#else
CreateDesignGeneral::CreateDesignGeneral(LibraryInterface* libInterface, QWidget *parent)
#endif
    : QWidget(parent),
      library_(libInterface),
      designExt_(),
      designConfExt_()
{
    designExt_ = ".design";
    designConfExt_ = ".designcfg";


//    connect(this, SIGNAL(createEmptyDesign()), this, SLOT(onCreateEmptyDesign()), Qt::UniqueConnection);


    setStyleSheet("CreateDesignGeneral { border: 1px solid black; }");

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
    mainLayout->addSpacing(8); // label1과 label2 사이의 간격

    QLabel *label2 = new QLabel("General information of Design", this);
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

    //    QVBoxLayout *leftLayout = new QVBoxLayout(); // 좌측 레이아웃 (입력폼)

#ifdef CREATE_PROJECT_TEST
    // ProjectDirectory 추가
    QLabel *projectLabel = new QLabel("Directory", this);
    projectLabel->setFont(baseFont);
    projectLabel->setFixedSize(160, 20);
    projectLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    projectDirTxt = new QLineEdit(this);
    projectDirTxt->setFixedSize(325, 20);
    projectDirTxt->setAlignment(Qt::AlignLeft);
    projectDirTxt->setReadOnly(true);
    projectDirTxt->setStyleSheet(borderStyle);
    projectDirTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    QPushButton *browiseButton = new QPushButton("Browse", this);
    browiseButton->setFixedSize(60, 20);
//    browiseButton->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백
    connect(browiseButton, &QPushButton::clicked, this, &CreateDesignGeneral::onHandleBrowse);

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *projectDirLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    projectDirLayoutWidget->setFixedSize(600, 30);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *projectDirSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    projectDirSubLayout->addWidget(projectLabel);
    projectDirSubLayout->addWidget(projectDirTxt);
    projectDirSubLayout->addWidget(browiseButton);

    // 3. 위젯에 레이아웃을 설정합니다.
    projectDirLayoutWidget->setLayout(projectDirSubLayout);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *projectDirLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    projectDirLayout->addWidget(projectDirLayoutWidget);

//    projectDirLayout->addWidget(projectLabel);
//    projectDirLayout->addWidget(projectDirTxt);
//    projectDirLayout->addWidget(browiseButton);

//    projectDirLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(projectDirLayout);
#else
    // Project 입력폼
    QLabel *projectLabel = new QLabel("Project", this);
    projectLabel->setFont(baseFont);
    projectLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    projectComboBox = new QComboBox(this);
    projectComboBox->setFixedSize(400, 25);
//    projectComboBox->setAlignment(Qt::AlignLeft);
//    projectComboBox->setStyleSheet(borderStyle);
//    projectComboBox->setStyleSheet("QComboBox QAbstractItemView { align-items: center; }");


    QHBoxLayout *projectLayout = new QHBoxLayout();
    projectLayout->addWidget(projectLabel);
    projectLayout->addWidget(projectComboBox);
    mainLayout->addLayout(projectLayout);
//    leftLayout->addLayout(projectLayout);
#endif


#if CREATEDESIGNGENERAL_20231206 == 1

    // Vendor 입력폼
    QLabel *vendorLabel = new QLabel("Vendor", this);
    vendorLabel->setFont(baseFont);
    vendorLabel->setFixedSize(160, 20);
    vendorLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    vendorTxt = new QLineEdit(this);
    vendorTxt->setFixedSize(400, 20);
    vendorTxt->setAlignment(Qt::AlignLeft);
    vendorTxt->setStyleSheet(borderStyle);

    vendorTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *vendorLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    vendorLayoutWidget->setFixedSize(600, 30);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *vendorSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    vendorSubLayout->addWidget(vendorLabel);
    vendorSubLayout->addWidget(vendorTxt);

    // 3. 위젯에 레이아웃을 설정합니다.
    vendorLayoutWidget->setLayout(vendorSubLayout);

    QHBoxLayout *vendorLayout = new QHBoxLayout();
    vendorLayout->addWidget(vendorLayoutWidget);
//    vendorLayout->addWidget(vendorLabel);
//    vendorLayout->addWidget(vendorTxt);

    mainLayout->addLayout(vendorLayout);
//    leftLayout->addLayout(vendorLayout);

    //Llibrary 입력폼
    QLabel *libraryLabel = new QLabel("Library", this);
    libraryLabel->setFont(baseFont);
    libraryLabel->setFixedSize(160, 20);
    libraryLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    libraryTxt = new QLineEdit(this);
    libraryTxt->setFixedSize(400, 20);
    libraryTxt->setAlignment(Qt::AlignLeft);
    libraryTxt->setStyleSheet(borderStyle);

    libraryTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *libraryLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    libraryLayoutWidget->setFixedSize(600, 30);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *librarySubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    librarySubLayout->addWidget(libraryLabel);
    librarySubLayout->addWidget(libraryTxt);

    // 3. 위젯에 레이아웃을 설정합니다.
    libraryLayoutWidget->setLayout(librarySubLayout);

    QHBoxLayout *libraryLayout = new QHBoxLayout();
    libraryLayout->addWidget(libraryLayoutWidget);
//    libraryLayout->addWidget(libraryLabel);
//    libraryLayout->addWidget(libraryTxt);

    mainLayout->addLayout(libraryLayout);
//    leftLayout->addLayout(libraryLayout);

#endif



    // Name 라벨 및 TextBox
    QLabel *nameLabel = new QLabel("Name", this);
    nameLabel->setFont(baseFont);
    nameLabel->setFixedSize(160, 20);
    nameLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    nameTxt = new QLineEdit(this);
    nameTxt->setFixedSize(400, 20);
    nameTxt->setAlignment(Qt::AlignLeft);
    nameTxt->setStyleSheet(borderStyle);

    nameTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *nameLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    nameLayoutWidget->setFixedSize(600, 30);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *nameSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    nameSubLayout->addWidget(nameLabel);
    nameSubLayout->addWidget(nameTxt);

    // 3. 위젯에 레이아웃을 설정합니다.
    nameLayoutWidget->setLayout(nameSubLayout);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameLayoutWidget);
//    nameLayout->addWidget(nameLabel);
//    nameLayout->addWidget(nameTxt);

//    nameLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(nameLayout);
//    leftLayout->addLayout(nameLayout);

    // Version 입력폼
    QLabel *versionLabel = new QLabel("Version", this);
    versionLabel->setFont(baseFont);
    versionLabel->setFixedSize(160, 20);
    versionLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    versionTxt = new QLineEdit(this);
    versionTxt->setFixedSize(400, 20);
    versionTxt->setAlignment(Qt::AlignLeft);
    versionTxt->setStyleSheet(borderStyle);

    versionTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *versionLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    versionLayoutWidget->setFixedSize(600, 30);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *versionSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    versionSubLayout->addWidget(versionLabel);
    versionSubLayout->addWidget(versionTxt);

    // 3. 위젯에 레이아웃을 설정합니다.
    versionLayoutWidget->setLayout(versionSubLayout);

    QHBoxLayout *versionLayout = new QHBoxLayout();
    versionLayout->addWidget(versionLayoutWidget);
//    versionLayout->addWidget(versionLabel);
//    versionLayout->addWidget(versionTxt);

    mainLayout->addLayout(versionLayout);
//    leftLayout->addLayout(versionLayout);

#if CREATEDESIGNGENERAL_20231206 == 0

    // Author 입력폼
    QLabel *authorLabel = new QLabel("Author", this);
    authorLabel->setFont(baseFont);
    authorLabel->setFixedSize(160, 20);
    authorLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    authorTxt = new QLineEdit(this);
    authorTxt->setFixedSize(400, 20);
    authorTxt->setAlignment(Qt::AlignLeft);
    authorTxt->setStyleSheet(borderStyle);

    authorTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *authorLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    authorLayoutWidget->setFixedSize(600, 30);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *authorSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    authorSubLayout->addWidget(authorLabel);
    authorSubLayout->addWidget(authorTxt);

    // 3. 위젯에 레이아웃을 설정합니다.
    authorLayoutWidget->setLayout(authorSubLayout);

    QHBoxLayout *authorLayout = new QHBoxLayout();
    authorLayout->addWidget(authorLayoutWidget);
//    authorLayout->addWidget(authorLabel);
//    authorLayout->addWidget(authorTxt);

    mainLayout->addLayout(authorLayout);
//    leftLayout->addLayout(authorLayout);

    // Description 입력폼
    QLabel *descriptionLabel = new QLabel("Description", this);
    descriptionLabel->setFont(baseFont);
    descriptionLabel->setFixedSize(160, 20);
    descriptionLabel->setContentsMargins(60, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    descriptionTxt = new QTextEdit(this);
    descriptionTxt->setFixedSize(395, 60);
    descriptionTxt->setAlignment(Qt::AlignLeft);
    descriptionTxt->setStyleSheet(borderStyle);

    descriptionTxt->setContentsMargins(5, 0, 0, 0);  // 좌측 패딩 및 상단 여백

    // 1. 레이아웃을 갖는 위젯을 만듭니다.
    QWidget *descriptionLayoutWidget = new QWidget();

    // 2. 이 위젯의 크기를 원하는 크기로 설정합니다.
    descriptionLayoutWidget->setFixedSize(600, 80);

    // 위에서 주어진 코드를 바탕으로 QHBoxLayout를 생성합니다.
    QHBoxLayout *descriptionSubLayout = new QHBoxLayout();

    // 위젯들을 레이아웃에 추가합니다.
    descriptionSubLayout->addWidget(descriptionLabel);
    descriptionSubLayout->addWidget(descriptionTxt);

    // 3. 위젯에 레이아웃을 설정합니다.
    descriptionLayoutWidget->setLayout(descriptionSubLayout);

    QHBoxLayout *descriptionLayout = new QHBoxLayout();
    descriptionLayout->addWidget(descriptionLayoutWidget);
//    descriptionLayout->addWidget(descriptionLabel);
//    descriptionLayout->addWidget(descriptionTxt);

    mainLayout->addLayout(descriptionLayout);
//    leftLayout->addLayout(descriptionLayout);
#endif


//    // 우측 요약 텍스트 박스
//    QTextEdit *summaryTxt = new QTextEdit(this);
//    summaryTxt->setFixedSize(400, 200); // 크기를 조절할 수 있습니다.

//    // 최종 레이아웃
//    QHBoxLayout *middleLayout = new QHBoxLayout();
//    middleLayout->addLayout(leftLayout); // 좌측 레이아웃 추가

//    /*  right layout    */

//    QVBoxLayout *rightLayout = new QVBoxLayout(); //right layout
//    QLabel *previewLabel = new QLabel("Preview", this);
//    previewLabel->setContentsMargins(40, 0, 10, 0);
//    rightLayout->addWidget(previewLabel);
//    rightLayout->addSpacing(10);
//    summaryTxt->setContentsMargins(40, 0, 10, 0);
//    rightLayout->addWidget(summaryTxt);
//    middleLayout->addLayout(rightLayout); // right layout into middle

//    mainLayout->addLayout(middleLayout);

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
    gridLayout->addWidget(centerWidget, 1, 1, 1, 1); //projectComboBox 중앙에 위젯을 배치합니다.
    //gridLayout->setRowStretch(0, 1);  // 위쪽 여백
    gridLayout->setRowStretch(2, 1);  // 아래쪽 여백
    //gridLayout->setColumnStretch(0, 1);  // 왼쪽 여백
    gridLayout->setColumnStretch(2, 1);  // 오른쪽 여백

    this->setLayout(gridLayout);  // 위젯에 그리드 레이아웃 설정


    //..DEBUG
    projectDirTxt ->setText("/home/user1/AMagic/Project/test4/design");
    vendorTxt->setText("jws");
//    libraryTxt->setText("ip");
    libraryTxt->setText("flat");
    nameTxt->setText("mydesign");
    versionTxt ->setText("1.0");


}

#ifdef CREATE_PROJECT_TEST
void CreateDesignGeneral::onHandleBrowse()
{
    QString folderName = selectDirectory(this);
    if(!folderName.isEmpty()) {
        // 폴더를 선택한 경우의 처리
        projectDirTxt->setText(folderName);
    } else {
        // 폴더를 선택하지 않은 경우의 처리
    }
}


QString CreateDesignGeneral::selectDirectory(QWidget *parent)
{
    // QFileDialog를 사용하여 폴더 선택 대화상자를 엽니다.
    // 이 함수는 선택한 폴더의 경로를 반환하거나, 선택하지 않으면 빈 문자열을 반환합니다.
    QString dir = QFileDialog::getExistingDirectory(parent, "Select a folder", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // 선택한 폴더의 이름만을 반환하려면 QFileInfo를 사용하여 폴더명을 추출할 수 있습니다.
    return QFileInfo(dir).filePath();
}
#endif

QString CreateDesignGeneral::designcfgFile()
{

#ifdef CREATE_PROJECT_TEST
    QString ProjectValue = projectDirTxt->text();
#else
    QString ProjectValue = projectComboBox->currentText();
#endif


#if CREATEDESIGNGENERAL_20231206 == 0

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

    if(ProjectValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "project 정보가 없습니다.");
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
    QString savepath   = libraryPath +"/" +ProjectValue + "/io/"+ NameValue + "/" + VersionValue + "/";
    QString _IoabsDefFile = savepath + NameValue+".absDef."+VersionValue + ".xml";
    return _IoabsDefFile;

#else
    QString VendorValue = vendorTxt->text();
    QString LibraryValue = libraryTxt->text();
    QString NameValue = nameTxt->text();
    QString VersionValue = versionTxt->text();

    if(ProjectValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "project 정보가 없습니다.");
        return "";
    }

    if(VendorValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "vendor 정보가 없습니다.");
        return "";
    }

    if(LibraryValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "library 정보가 없습니다.");
        return "";
    }

    if(NameValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
        return "";
    }

    if(VersionValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
        return "";
    }

    QString savepath   = ProjectValue + "/Design/"  + LibraryValue + "/"  + NameValue + "/" + VersionValue + "/";
    QString _designcfgFile = savepath + NameValue+".designcfg.."+VersionValue + ".xml";
    return _designcfgFile;
#endif

}

void CreateDesignGeneral::onCreateEmptyDesign()
{
#ifdef CREATE_PROJECT_TEST
    QString ProjectValue = projectDirTxt->text();
#else
    QString ProjectValue = projectComboBox->currentText();
#endif

    QString VendorValue = vendorTxt->text();
    QString LibraryValue = libraryTxt->text();
    QString NameValue = nameTxt->text();
    QString VersionValue = versionTxt->text();

    if(ProjectValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "project 정보가 없습니다.");
//        return false;
        return;
    }

    if(VendorValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "vendor 정보가 없습니다.");
//        return false;
        return;
    }

    if(LibraryValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "library 정보가 없습니다.");
//        return false;
        return;
    }

    if(NameValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
//        return false;
        return;
    }

    if(VersionValue.isEmpty())
    {
        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
//        return false;
        return;
    }


//    QSharedPointer<Component> newComponent;

//    VLNV vlnv = this->getVLNV();

////    if (library_->contains(vlnv))
////    {
////        // find the component
////        QSharedPointer<Document> libComp = library_->getModel(vlnv);
////        newComponent = libComp.staticCast<Component>();

////        Q_ASSERT_X(newComponent, "HWDesignWidget::createEmptyDesign",
////            "The selected library item has to be component");
////    }
////    else
////    {
////        // create the component
////        newComponent = QSharedPointer<Component>(new Component(vlnv));
////    }

//    // create the component
//    newComponent = QSharedPointer<Component>(new Component(vlnv));


////    QString savepath   = ProjectValue + "/Design/"  + LibraryValue + "/"  + NameValue + "/" + VersionValue + "/";
//    QString savepath   = ProjectValue + "/"  + LibraryValue + "/"  + NameValue + "/" + VersionValue + "/";
////    QString _designcfgFile = savepath + NameValue+".designcfg.."+VersionValue + ".xml";
////    QString _designcfgFile = savepath + NameValue+".designcfg.."+VersionValue + ".xml";

//    library_->writeModelToFile(savepath, newComponent);

////    createDesignForComponent(newComponent, path);

////    return newComponent;

////    return true;


//    QString savepath   = ProjectValue + "/Design/"  + LibraryValue + "/"  + NameValue + "/" + VersionValue + "/";
    QString savepath   = ProjectValue + "/"  + LibraryValue + "/"  + NameValue + "/" + VersionValue + "/";
//    QString _designcfgFile = savepath + NameValue+".designcfg.."+VersionValue + ".xml";
//    QString _designcfgFile = savepath + NameValue+".designcfg.."+VersionValue + ".xml";

//    emit createDesign(KactusAttribute::FLAT, KactusAttribute::MUTABLE, this->getVLNV(), savepath);
    emit createDesign(this->getVLNV(), savepath);

}


//-----------------------------------------------------------------------------
// Function: CreateDesignGeneral::getVLNV()
//-----------------------------------------------------------------------------
VLNV CreateDesignGeneral::getVLNV() const
{
    return VLNV(VLNV::COMPONENT, vendorTxt->text(), libraryTxt->text(), nameTxt->text(), versionTxt->text());
//    return VLNV(VLNV::DESIGN, vendorTxt->text(), libraryTxt->text(), nameTxt->text(), versionTxt->text());
}

////-----------------------------------------------------------------------------
//// Function: NewDesignDialog::getDesignVLNV()
////-----------------------------------------------------------------------------
//VLNV CreateDesignGeneral::getDesignVLNV() const
//{
//    VLNV vlnv = this->getVLNV();
//    vlnv.setType(VLNV::DESIGN);
//    vlnv.setName(vlnv.getName().remove(".comp") + designExt_);
//    return vlnv;
//}

////-----------------------------------------------------------------------------
//// Function: CreateDesignGeneral::getDesignConfVLNV()
////-----------------------------------------------------------------------------
//VLNV CreateDesignGeneral::getDesignConfVLNV() const
//{
//    VLNV vlnv = this->getVLNV();
//    vlnv.setType(VLNV::DESIGNCONFIGURATION);

//    QString name = vlnv.getName().remove(".comp");

////    if (!qualifierEdit_->text().isEmpty())
////    {
////        name.append("."  + qualifierEdit_->text());
////    }

//    name.append(designConfExt_);

//    vlnv.setName(name);
//    return vlnv;
//}

