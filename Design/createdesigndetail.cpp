#include "createdesigndetail.h"

#include <QLabel>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDir>

CreateDesignDetail::CreateDesignDetail(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("CreateDesignDetail { border: 1px solid black; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃

//    mainLayout->addSpacing(10);


    QLabel *designDetailLabel = new QLabel("Design Detail", this);
    QFont font1;
    font1.setPointSize(12);
    font1.setBold(true);
    designDetailLabel->setFont(font1);
    designDetailLabel->setContentsMargins(10, 0, 0, 0);
    designDetailLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
//    designDetailLabel->setContentsMargins(40, 0, 10, 0);

    mainLayout->addWidget(designDetailLabel);
//    mainLayout->addSpacing(30);


//    QLabel *label1 = new QLabel("General", this);
//    QFont font1;
//    font1.setPointSize(16);
//    font1.setBold(true);
//    label1->setFont(font1);
//    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
//    label1->setContentsMargins(60, 0, 0, 0);
//    mainLayout->addWidget(label1);
//    mainLayout->addSpacing(8); // label1과 label2 사이의 간격

//    QLabel *label2 = new QLabel("General information of Design", this);
//    QFont font2;
//    font2.setPointSize(12);
//    label2->setFont(font2);
//    label2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
//    label2->setContentsMargins(60, 0, 30, 0);
//    mainLayout->addWidget(label2);

//    // 50px의 공백 추가
//    mainLayout->addSpacing(50);

    designDetailtable = new QTableWidget(this);
    QFont headerFont = designDetailtable->horizontalHeader()->font();
    QFont font = designDetailtable->font();

    headerFont.setPixelSize(12);

    font.setPixelSize(12);

    designDetailtable->horizontalHeader()->setFont(headerFont);
    designDetailtable->horizontalHeader()->setFixedHeight(30);
    designDetailtable->setFont(font);

        // 컬럼의 수 설정
    designDetailtable->setColumnCount(1);
        // 헤더 설정
    QStringList headers = {"Design VLNV"};
    designDetailtable->setHorizontalHeaderLabels(headers);

    // 테이블에 5개 행 추가 (예시로 5개를 설정했지만 필요한만큼 추가할 수 있습니다.)
    designDetailtable->setRowCount(0);

    for (int i = 0; i < 0; i++) {
    }

    designDetailtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 컬럼의 크기를 테이블의 너비에 맞게 조정

    QHBoxLayout *detailLayout = new QHBoxLayout();
    detailLayout->addWidget(designDetailtable);
    mainLayout->addLayout(detailLayout);

    // 레이아웃 설정
    this->setLayout(mainLayout);
    //this->setFixedSize(900, 900);
    // 레이아웃 설정
    this->setLayout(mainLayout);

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
