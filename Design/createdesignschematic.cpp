#include "createdesignschematic.h"

#include "createdesigngeneral.h"
#include "createdesignfileset.h"
#include "createdesignportmap.h"
#include "createdesigndetail.h"
#include "schematictool.h"

#include <UI/HWDesign/HWDesignWidget.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDir>
#include <QSplitter>


CreateDesignSchematic::CreateDesignSchematic(QWidget *parent,
                                             CreateDesignGeneral* designGen,
                                             CreateDesignFileSet* designFileSet,
                                             CreateDesignPortMap* designPortMap,
                                             TabDocument* designSchematic)
    : QWidget{parent}
{
    _designGen = designGen;
    _designFileSet = designFileSet;
    _designPortMap = designPortMap;
    _designSchematic = designSchematic;


    connect(this, SIGNAL(createEmptyDesign()), this, SLOT(onCreateEmptyDesign()), Qt::UniqueConnection);




    setStyleSheet("CreateDesignSchematic { border: 1px solid black; }");
    QString borderStyle = "border: 1px solid black;";

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃

    mainLayout->addSpacing(30);

    // 기본 폰트 설정
    QFont baseFont;
    baseFont.setPointSize(10);
    baseFont.setBold(true);


    CreateDesignDetail *createDesignDetail = new CreateDesignDetail( this);

    //------------------------

    // 최종 레이아웃
    QSplitter* splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    if (_designSchematic) {
        splitter->addWidget(_designSchematic);
    }
    splitter->addWidget(createDesignDetail);
//    splitter->setStretchFactor(0, 2);
//    splitter->setStretchFactor(1, 2);
    splitter->setContentsMargins(0, 0, 0, 0);


    createDesignDetail->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addWidget(splitter, 1);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    //////////////////////////////////////////////////////
    /*  left layout    */
//    QVBoxLayout *leftLayout = new QVBoxLayout(); // 좌측 레이아웃 (입력폼)

//    leftLayout->addWidget(_designSchematic);

//    /*  right layout    */
//    QVBoxLayout *rightLayout = new QVBoxLayout(); //right layout

//    rightLayout->addWidget(createDesignDetail);

//    QHBoxLayout *middleLayout = new QHBoxLayout();

//    middleLayout->addLayout(leftLayout); // left layout into middle
//    middleLayout->addLayout(rightLayout); // right layout into middle

//    mainLayout->addLayout(middleLayout);

    //////////////////////////////////////////////////////

    // 레이아웃 설정
    this->setLayout(mainLayout);



//    setCentralWidget(splitter);
//    splitter->addWidget(elementWindow);
//    splitter->addWidget(mainWorkWindow);


//    // 위젯을 중앙에 배치하기 위한 그리드 레이아웃 생성
//    QGridLayout *gridLayout = new QGridLayout();
//    QWidget *centerWidget = new QWidget(this);
//    centerWidget->setLayout(mainLayout);
//    gridLayout->addWidget(centerWidget, 1, 1, 1, 1); // 중앙에 위젯을 배치합니다.
//    //gridLayout->setRowStretch(0, 1);  // 위쪽 여백
//    gridLayout->setRowStretch(2, 1);  // 아래쪽 여백
//    //gridLayout->setColumnStretch(0, 1);  // 왼쪽 여백
//    gridLayout->setColumnStretch(2, 1);  // 오른쪽 여백

//    this->setLayout(gridLayout);  // 위젯에 그리드 레이아웃 설정


    //..DEBUG
//    nameTxt->setText("myio");
//    versionTxt ->setText("1.0");
//    vendorTxt ->setText("asicland");
//    authorTxt ->setText("tester");
}


bool CreateDesignSchematic::onCreateEmptyDesign()
{
    return false;
}

void CreateDesignSchematic::CreateDesignXml()
{
    //..todo 할일
    // 폴더 경로를 io처럼 만들어낸다.
  //  _designFileSet->;//
 //   _designGen
}


QString CreateDesignSchematic::IoabsDefFile()
{

//    QString VendorValue = vendorTxt->text();
//    QString NameValue = nameTxt->text();
//    QString Author = authorTxt->text();
//    QString VersionValue = versionTxt->text();
//    QString DescriptionValue = descriptionTxt->toPlainText();
/*

    #ifdef DEBUG
    VendorValue="abc_vendor";
    NameValue ="abc_name";
    VersionValue="1.0";
    DescriptionValue="abc_description";
    Author = 'abc_author';
    #endif
*/
//    if(VendorValue.isEmpty())
//    {
//        QMessageBox::information(this, "General",  "vendor 정보가 없습니다.");
//        return "";
//    }
//    if(NameValue.isEmpty())
//    {
//        QMessageBox::information(this, "General",  "version 정보가 없습니다.");
//        return "";
//    }
//    if(Author.isEmpty())
//    {
//        QMessageBox::information(this, "General",  "author 정보가 없습니다.");
//        return "";
//    }
//    if(VersionValue.isEmpty())
//    {
//        QMessageBox::information(this, "General",  "name 정보가 없습니다.");
//        return "";
//    }


//    QString libraryPath = QDir::homePath() + "/AMagic/library";
//    QString savepath   = libraryPath +"/" +VendorValue + "/io/"+ NameValue + "/" + VersionValue + "/";
//    QString _IoabsDefFile = savepath + NameValue+".absDef."+VersionValue + ".xml";
//    return _IoabsDefFile;

    return QString();
}

