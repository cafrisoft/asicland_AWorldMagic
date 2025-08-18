#include "createdesignportmap.h"
#include <QLabel>
#include <QVBoxLayout>

#include <QComboBox>
#include <QListView>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

#include "createdesignfileset.h"
#include "createdesigngeneral.h"
//#include "parsor/vhdlrecordparser.h"

//VhdlRecordParser vhdlParser;//("C:/QtWorks/atcbmc200_wrapper.v");
//QStringList includes;
//QStringList vhdls;

CreateDesignPortMap::CreateDesignPortMap(QWidget *parent, CreateDesignGeneral* designGen, CreateDesignFileSet* designFileSet)
    : QWidget{parent}
{
    _designGen = designGen;
    _designFileSet = designFileSet;    

    connect(this, SIGNAL(createEmptyDesign()), this, SLOT(onCreateEmptyDesign()), Qt::UniqueConnection);

    setStyleSheet("CreateDesignPortMap { border: 1px solid black; }");

    // 기본 폰트 설정
    QFont baseFont;
    baseFont.setPointSize(10);
    baseFont.setBold(true);

    QLabel *label1 = new QLabel("Port Map & Parameter", this);
    QFont font1;
    font1.setPointSize(16);
    font1.setBold(true);
    label1->setFont(font1);
    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label1->setContentsMargins(20, 0, 0, 0);  // 좌측 패딩을 20으로 설정

    QLabel *label2 = new QLabel("Port Map and Parameter information of Design", this);
    QFont font2;
    font2.setPointSize(12);
    label2->setFont(font2);
    label2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label2->setContentsMargins(20, 0, 0, 0);  // 좌측 패딩을 20으로 설정

    QLabel *label3 = new QLabel("Ports", this);
    label3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label3->setContentsMargins(10, 0, 0, 0);

    //---------------------------- Ports  테이블 ------------------


    porttable = new QTableWidget(this);
    QFont headerFont = porttable->horizontalHeader()->font();    
    QFont font = porttable->font();

    headerFont.setPixelSize(12);

    font.setPixelSize(12);

    porttable->horizontalHeader()->setFont(headerFont);
    porttable->horizontalHeader()->setFixedHeight(30);
    porttable->setFont(font);

        // 컬럼의 수 설정
    porttable->setColumnCount(14);
        // 헤더 설정
    QStringList headers = {"Name", "In/Out", "Left(Higher)\nBound,(fx)", "Right(Lower)\nBound,f(x)", "Width", "Type", "Default Value", "Logical Port", "Logical Left", "Logical Right",
"Physical Left", "Physical Right", "Invert", "Description"};
    porttable->setHorizontalHeaderLabels(headers);
    // QTableWidget에 우클릭 메뉴 이벤트 추가
    porttable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(porttable, &QTableWidget::customContextMenuRequested, this, &CreateDesignPortMap::showPortsContextMenu);


    // 테이블에 5개 행 추가 (예시로 5개를 설정했지만 필요한만큼 추가할 수 있습니다.)
    porttable->setRowCount(0);

    for (int i = 0; i < 0; i++) {
        porttable->setItem(i, 0, new QTableWidgetItem("Name " + QString::number(i)));

        // Mode에 QComboBox 추가
        QComboBox *modeComboBox = new QComboBox();
        modeComboBox->addItems({"In", "Out"});
        porttable->setCellWidget(i, 1, modeComboBox);


        QComboBox *portTypeComboBox = new QComboBox();
        portTypeComboBox->addItems({"Clock", "PAD", "PEN", "PRD"});
        porttable->setCellWidget(i, 7, portTypeComboBox);


        QComboBox *logicalPortComboBox = new QComboBox();
        logicalPortComboBox->addItems({"선택i/o", "port값", "PEN", "PRD"});
        porttable->setCellWidget(i, 8, logicalPortComboBox);

        //--------------------------------------------------------------------

        porttable->setItem(i, 2, new QTableWidgetItem("Presence " + QString::number(i)));

        // Direction에 아이콘과 텍스트 추가
        QTableWidgetItem *directionItem = new QTableWidgetItem(QIcon(":/path/to/your/icon.png"), "Direction " + QString::number(i));
        porttable->setItem(i, 3, directionItem);

                 // ... 나머지 컬럼들에 대한 아이템도 이런 식으로 추가할 수 있습니다.
        porttable->setItem(i, 4, new QTableWidgetItem("Presence " + QString::number(i)));
        porttable->setItem(i, 5, new QTableWidgetItem("Presence " + QString::number(i)));
        porttable->setItem(i, 6, new QTableWidgetItem("Presence " + QString::number(i)));


        porttable->setItem(i, 9, new QTableWidgetItem("Presence " + QString::number(i)));
        porttable->setItem(i, 10, new QTableWidgetItem("Presence " + QString::number(i)));
        porttable->setItem(i, 11, new QTableWidgetItem("Presence " + QString::number(i)));
        porttable->setItem(i, 12, new QTableWidgetItem("Presence " + QString::number(i)));

                 // 13번째 컬럼에 체크박스 추가
/*        QCheckBox *invertCheckBox = new QCheckBox();
        porttable->setCellWidget(i, 13, invertCheckBox);
        QTableWidgetItem *centeredItem = new QTableWidgetItem();
        centeredItem->setTextAlignment(Qt::AlignCenter);  // 셀의 내용을 중앙에 정렬합니다.
        porttable->setItem(i, 13, centeredItem);*/

        QCheckBox *invertCheckBox = new QCheckBox();

        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->addWidget(invertCheckBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0); // 여백을 0으로 설정하여 위젯이 셀 전체에 꽉 차도록 합니다.
        widget->setLayout(layout);

        porttable->setCellWidget(i, 13, widget);
        porttable->setItem(i, 14, new QTableWidgetItem("Presence " + QString::number(i)));
    }

    porttable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 컬럼의 크기를 테이블의 너비에 맞게 조정



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QLabel *label4 = new QLabel("Parameters", this);
    label4->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label4->setContentsMargins(10, 0, 0, 0);



    //-------------  Parameter 테이블

    paratable = new QTableWidget(this);
    paratable->setFont(font);
    paratable->horizontalHeader()->setFont(headerFont);


       // 컬럼의 수 설정
    paratable->setColumnCount(5);
       // 헤더 설정
    QStringList pheaders = {"Name", "Type", "Value, f(x)", "Resolve", "Description"};
    paratable->setHorizontalHeaderLabels(pheaders);
         // 테이블에 5개 행 추가 (예시로 5개를 설정했지만 필요한만큼 추가할 수 있습니다.)
    //paratable->setRowCount(5);
    /*
    for (int i = 0; i < 5; i++) {
        paratable->setItem(i, 0, new QTableWidgetItem("Name " + QString::number(i)));
        porttable->setItem(i, 1, new QTableWidgetItem("String " + QString::number(i)));
        porttable->setItem(i, 2, new QTableWidgetItem("a51c " + QString::number(i)));
        porttable->setItem(i, 3, new QTableWidgetItem("user " + QString::number(i)));
        porttable->setItem(i, 4, new QTableWidgetItem("Presence " + QString::number(i)));
    }
    */
        // 마지막 컬럼은 남은 공간으로 채움
    paratable->horizontalHeader()->setSectionResizeMode(paratable->columnCount() - 1, QHeaderView::Stretch);

    // QTableWidget에 우클릭 메뉴 이벤트 추가
    paratable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(paratable, &QTableWidget::customContextMenuRequested, this, &CreateDesignPortMap::showParametersContextMenu);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addSpacing(30);

    layout->addWidget(label1);
    layout->addSpacing(8); // label1과 label2 사이의 간격
    layout->addWidget(label2);
    layout->addSpacing(20);

    layout->addWidget(label3);
    layout->addSpacing(10);
    layout->addWidget(porttable);
    layout->addSpacing(20);
    layout->addWidget(label4);
    layout->addSpacing(10);
    layout->addWidget(paratable);

    layout->addSpacing(3); // QComboBox 아래의 간격
    layout->setContentsMargins(30, 0, 0, 0); // 좌우 패딩 없음

    this->setLayout(layout);

//    // 위젯을 중앙에 배치하기 위한 그리드 레이아웃 생성
//    QGridLayout *gridLayout = new QGridLayout();
//    QWidget *centerWidget = new QWidget(this);
//    centerWidget->setLayout(layout);
//    gridLayout->addWidget(centerWidget, 1, 1, 1, 1); // 중앙에 위젯을 배치합니다.
//    //gridLayout->setRowStretch(0, 1);  // 위쪽 여백
//    gridLayout->setRowStretch(2, 1);  // 아래쪽 여백
//    //gridLayout->setColumnStretch(0, 1);  // 왼쪽 여백
//    gridLayout->setColumnStretch(2, 1);  // 오른쪽 여백

//    this->setLayout(gridLayout);  // 위젯에 그리드 레이아웃 설정

    //includes.append("/home/dubi/AMagic/my_vhdl/include");
    //vhdls.append( "/home/dubi/AMagic/my_vhdl/atcbmc300.v");


    //vhdlParser.parse(vhdls.at(0));
}

bool CreateDesignPortMap::onCreateEmptyDesign()
{
    return false;
}

void CreateDesignPortMap::showPortsContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Add New Line", this);
    connect(&action1, &QAction::triggered, this, &CreateDesignPortMap::doPortsInsertRow);
    contextMenu.addAction(&action1);

    QAction action2("Delete Line", this);
    connect(&action2, &QAction::triggered, this, &CreateDesignPortMap::doPortsDeleteRow);
    contextMenu.addAction(&action2);

    // ... 다른 메뉴 아이템들을 추가 ...

    contextMenu.exec(porttable->mapToGlobal(pos));
}

void CreateDesignPortMap::showParametersContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Add New Line", this);
    connect(&action1, &QAction::triggered, this, &CreateDesignPortMap::doParameterInsertRow);
    contextMenu.addAction(&action1);

    QAction action2("Delete Line", this);
    connect(&action2, &QAction::triggered, this, &CreateDesignPortMap::doParameterDeleteRow);
    contextMenu.addAction(&action2);

    contextMenu.exec(paratable->mapToGlobal(pos));
}


void CreateDesignPortMap::doPortsInsertRow()
{

}

QStringList CreateDesignPortMap::getColumnTexts(QTableWidget *table, QComboBox *localCB, int column) {
    QStringList texts;
    localCB->clear();  // 기존에 콤보 박스에 있던 항목들을 모두 제거
    for (int i = 0; i < table->rowCount(); ++i) {
        QTableWidgetItem *item = table->item(i, column);
        if (item) {
            QString text = item->text();
            texts.append(text);
            localCB->addItem(text);  // 콤보 박스에 항목 추가
        }
    }
    return texts;
}

void CreateDesignPortMap::doPortsDeleteRow()
{
        // 현재 선택된 행들을 가져옴
    QList<QTableWidgetItem*> selectedItems = porttable->selectedItems();
    QSet<int> rowsToDelete;

        // 선택된 항목들을 반복하면서 어떤 행들이 선택되었는지 파악
    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

        // 삭제해야 하는 행들을 역순으로 삭제 (앞에서부터 삭제하면 행의 인덱스가 바뀌기 때문)
        QList<int> sortedRows(rowsToDelete.begin(), rowsToDelete.end());
        std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    for (int row : sortedRows) {
        porttable->removeRow(row);
    }
}

void CreateDesignPortMap::doParameterInsertRow()
{

}

void CreateDesignPortMap::doParameterDeleteRow()
{
    // 현재 선택된 행들을 가져옴
    QList<QTableWidgetItem*> selectedItems = paratable->selectedItems();
    QSet<int> rowsToDelete;

    // 선택된 항목들을 반복하면서 어떤 행들이 선택되었는지 파악
    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

     // 삭제해야 하는 행들을 역순으로 삭제 (앞에서부터 삭제하면 행의 인덱스가 바뀌기 때문)
     QList<int> sortedRows(rowsToDelete.begin(), rowsToDelete.end());
     std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    for (int row : sortedRows) {
        paratable->removeRow(row);
    }

}



void CreateDesignPortMap::LoadDesignPortMapXML(QString xmlFile)
{
/*
    table->clear();
    wireParser.parse(wireXMLfile);

    QStringList headers = {"Name", "Mode", "Presence", "Direction", "Width", "Default Value", "Driver", "Qualifier", "System Group", "Description"};
    table->setHorizontalHeaderLabels(headers);

    QString Value[11];


    for(const IPXACTPort& port : wireParser.ports) {
                // 여기서 port를 사용하여 필요한 작업을 수행합니다.

        Value[0] = port.logicalName;
        Value[5] = port.defaultValue;

        if(port.driver.isNull)
            Value[6] = "";
        else
            Value[6] = port.driver.typeAtt;

        if(port.qualifier.isNull)
            Value[7] = "";
        else
            Value[7] = port.qualifier.qulifierType;

        Value[8] = port.systemGroup;
        Value[9] = port.description;



                //--  주의 ... 무조건 마지막 부분에 있어야 한다.... 여러 줄 삽입이 필요 할 수 도

        int idx = table->rowCount();
        if(!port.onMaster.isNull )
        {
            Value[1] = "master";
            Value[2] = port.onMaster.presence;
            Value[3] = port.onMaster.direction;
            Value[4] =  QString::number(port.onMaster.width);
            Addrow(Value);
        }

        if(!port.onSlave.isNull )
        {
            Value[1] = "slave";
            Value[2] = port.onSlave.presence;
            Value[3] = port.onSlave.direction;
            Value[4] =  QString::number(port.onSlave.width);
            Addrow(Value);
        }

        if(!port.onSystem.isNull )
        {
            Value[1] = "system";
            Value[2] = port.onSystem.presence;
            Value[3] = port.onSystem.direction;
            Value[4] =  QString::number(port.onSystem.width);
            Addrow(Value);
        }


    } */
}

void CreateDesignPortMap::Addrow(QString Value[11])
{
/*

    QComboBox *modeComboBox = new QComboBox();
    modeComboBox->addItems({"master", "slave", "system"});
    QComboBox *directionComboBox = new QComboBox();
    directionComboBox->addItems({"out", "in", "inout"});
    QComboBox *driverComboBox = new QComboBox();
    driverComboBox->addItems({"clock", "singleShot", "any", "none"});
    QComboBox *qualifierComboBox = new QComboBox();
    qualifierComboBox->addItems({"address", "data", "address,data", "clock", "reset", "none"});

         // 현재 행의 수 가져오기
    int i = table->rowCount();
        // 행 추가
    table->insertRow(i);

        // 새로운 행에 데이터나 위젯을 추가하려면:
    table->setItem(i, 0, new QTableWidgetItem(Value[0]));
    table->setItem(i, 2, new QTableWidgetItem(Value[2]));
    table->setItem(i, 4, new QTableWidgetItem(Value[4]));
    table->setItem(i, 5, new QTableWidgetItem(Value[5]));
    table->setItem(i, 8, new QTableWidgetItem(Value[8]));
    table->setItem(i, 9, new QTableWidgetItem(Value[9]));

        //콤포박스

    int idx = modeComboBox->findText(Value[1]);
    if (idx != -1) {
        table->setCellWidget(i, 1, modeComboBox); //{"master", "slave", "system"}
        modeComboBox->setCurrentIndex(idx);
    } else {
        qDebug() << Value[1] << "not found in the combo box.";
    }

    idx = directionComboBox->findText(Value[3]);
    if (idx != -1) {
        table->setCellWidget(i, 3, directionComboBox);
        directionComboBox->setCurrentIndex(idx);
    } else {
        qDebug() << Value[3] << "not found in the combo box.";
    }

    idx = driverComboBox->findText(Value[6]);
    if (idx != -1) {
        table->setCellWidget(i, 6, driverComboBox);
        driverComboBox->setCurrentIndex(idx);
    } else {
        qDebug() << Value[6] << "not found in the combo box.";
    }

    idx = qualifierComboBox->findText(Value[7]);
    if (idx != -1) {
        table->setCellWidget(i, 7, qualifierComboBox);
        qualifierComboBox->setCurrentIndex(idx);
    } else {
        qDebug() << Value[7] << "not found in the combo box.";
    }
*/
}
