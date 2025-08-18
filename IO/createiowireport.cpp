#include "createiowireport.h"
#include <QLabel>
#include <QVBoxLayout>

#include <QComboBox>
#include <QListView>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include "Constructor/ioxmlgen.h"


CreateIOWirePort::CreateIOWirePort(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("CreateIOWirePort { border: 1px solid black; }");

    QLabel *label1 = new QLabel("Wire Port", this);
    QFont font1;
    font1.setPointSize(16);
    font1.setBold(true);
    label1->setFont(font1);
    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label1->setContentsMargins(20, 0, 0, 0);  // 좌측 패딩을 20으로 설정

    QLabel *label2 = new QLabel("Wire Port information of I/O", this);
    QFont font2;
    font2.setPointSize(12);
    label2->setFont(font2);
    label2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label2->setContentsMargins(20, 0, 0, 0);  // 좌측 패딩을 20으로 설정

    table = new QTableWidget(this);

    // 컬럼의 수 설정
    table->setColumnCount(10);

    // 헤더 설정
    QStringList headers = {"Name", "Mode", "Presence", "Direction", "Width", "Default Value", "Driver", "Qualifier", "System Group", "Description"};
    table->setHorizontalHeaderLabels(headers);

    // 테이블에 5개 행 추가 (예시로 5개를 설정했지만 필요한만큼 추가할 수 있습니다.)
/*    table->setRowCount(5);

    for (int i = 0; i < 5; i++) {
        table->setItem(i, 0, new QTableWidgetItem("Name " + QString::number(i)));

        // Mode에 QComboBox 추가
        QComboBox *modeComboBox = new QComboBox();
        modeComboBox->addItems({"Mode 1", "Mode 2", "Mode 3"});
        table->setCellWidget(i, 1, modeComboBox);

        table->setItem(i, 2, new QTableWidgetItem("Presence " + QString::number(i)));

        // Direction에 아이콘과 텍스트 추가
        QTableWidgetItem *directionItem = new QTableWidgetItem(QIcon(":/path/to/your/icon.png"), "Direction " + QString::number(i));
        table->setItem(i, 3, directionItem);

        // ... 나머지 컬럼들에 대한 아이템도 이런 식으로 추가할 수 있습니다.
        table->setItem(i, 4, new QTableWidgetItem("Presence " + QString::number(i)));
        table->setItem(i, 5, new QTableWidgetItem("Presence " + QString::number(i)));
        table->setItem(i, 6, new QTableWidgetItem("Presence " + QString::number(i)));
        table->setItem(i, 7, new QTableWidgetItem("Presence " + QString::number(i)));
        table->setItem(i, 8, new QTableWidgetItem("Presence " + QString::number(i)));
        table->setItem(i, 9, new QTableWidgetItem("Presence " + QString::number(i)));
    }
*/


    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 컬럼의 크기를 테이블의 너비에 맞게 조정

    // QTableWidget에 우클릭 메뉴 이벤트 추가
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableWidget::customContextMenuRequested, this, &CreateIOWirePort::showContextMenu);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addSpacing(30);

    layout->addWidget(label1);
    layout->addSpacing(8); // label1과 label2 사이의 간격
    layout->addWidget(label2);
    layout->addSpacing(10); // label2와 QComboBox 사이의 간격
    layout->addWidget(table);
    layout->addSpacing(3); // QComboBox 아래의 간격
    layout->setContentsMargins(60, 0, 0, 0); // 좌우 패딩 없음

    this->setLayout(layout);
}


void CreateIOWirePort::showContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Add New Line", this);
    connect(&action1, &QAction::triggered, this, &CreateIOWirePort::doInsertRow);
    contextMenu.addAction(&action1);

    QAction action2("Delete Line", this);
    connect(&action2, &QAction::triggered, this, &CreateIOWirePort::doDeleteRow);
    contextMenu.addAction(&action2);

    // ... 다른 메뉴 아이템들을 추가 ...

    contextMenu.exec(table->mapToGlobal(pos));
}

void CreateIOWirePort::doInsertRow() {

    QComboBox *modeComboBox = new QComboBox();
    modeComboBox->addItems({"master", "slave", "system"});
    QComboBox *directionComboBox = new QComboBox();
    directionComboBox->addItems({"out", "in", "inout"});
    QComboBox *driverComboBox = new QComboBox();
    driverComboBox->addItems({"none","clock", "singleShot", "any"});
    QComboBox *qualifierComboBox = new QComboBox();
    qualifierComboBox->addItems({"none","address", "data", "address,data", "clock", "reset"});

    QComboBox *preperenceComboBox = new QComboBox();
    preperenceComboBox->addItems({"none","illegal", "required", "optional"});

         // 현재 행의 수 가져오기
    int i = table->rowCount();

        // 행 추가
    table->insertRow(i);

        // 새로운 행에 데이터나 위젯을 추가하려면:
    table->setItem(i, 0, new QTableWidgetItem(""));
    //table->setItem(i, 2, new QTableWidgetItem(""));
    table->setItem(i, 4, new QTableWidgetItem(""));
    table->setItem(i, 5, new QTableWidgetItem(""));
    table->setItem(i, 8, new QTableWidgetItem(""));
    table->setItem(i, 9, new QTableWidgetItem(""));

        //콤포박스
    table->setCellWidget(i, 1, modeComboBox);
    table->setCellWidget(i, 2, preperenceComboBox);
    table->setCellWidget(i, 3, directionComboBox);
    table->setCellWidget(i, 6, driverComboBox);
    table->setCellWidget(i, 7, qualifierComboBox);

}

void CreateIOWirePort::doDeleteRow() {
        // 현재 선택된 행들을 가져옴
    QList<QTableWidgetItem*> selectedItems = table->selectedItems();
    QSet<int> rowsToDelete;

        // 선택된 항목들을 반복하면서 어떤 행들이 선택되었는지 파악
    for (QTableWidgetItem* item : selectedItems) {
        rowsToDelete.insert(item->row());
    }

        // 삭제해야 하는 행들을 역순으로 삭제 (앞에서부터 삭제하면 행의 인덱스가 바뀌기 때문)
        QList<int> sortedRows(rowsToDelete.begin(), rowsToDelete.end());
        std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    for (int row : sortedRows) {
        table->removeRow(row);
    }
}

void CreateIOWirePort::LoadWireXML(QString wireXMLfile)
{
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


    }

}

void CreateIOWirePort::Addrow(QString Value[11])
{

    QComboBox *modeComboBox = new QComboBox();
    modeComboBox->addItems({"master", "slave", "system"});
    QComboBox *directionComboBox = new QComboBox();
    directionComboBox->addItems({"out", "in", "inout"});
    QComboBox *driverComboBox = new QComboBox();
    driverComboBox->addItems({"clock", "singleShot", "any", "none"});
    QComboBox *qualifierComboBox = new QComboBox();
    qualifierComboBox->addItems({"address", "data", "address,data", "clock", "reset", "none"});

    QComboBox *preperenceComboBox = new QComboBox();
    preperenceComboBox->addItems({"none","illegal", "required", "optional"});

    // 현재 행의 수 가져오기
    int i = table->rowCount();
        // 행 추가
    table->insertRow(i);

        // 새로운 행에 데이터나 위젯을 추가하려면:
    table->setItem(i, 0, new QTableWidgetItem(Value[0]));
    //table->setItem(i, 2, new QTableWidgetItem(Value[2]));
    table->setItem(i, 4, new QTableWidgetItem(Value[4]));
    table->setItem(i, 5, new QTableWidgetItem(Value[5]));
    table->setItem(i, 8, new QTableWidgetItem(Value[8]));
    table->setItem(i, 9, new QTableWidgetItem(Value[9]));

        //콤포박스

    int idx = modeComboBox->findText(Value[1]);
    if (idx != -1) {
        table->setCellWidget(i, 1, modeComboBox); //{"master", "slave", "system"}
        modeComboBox->setCurrentIndex(idx);
    }

    idx = preperenceComboBox->findText(Value[2]);
    if (idx != -1) {
        table->setCellWidget(i, 2, preperenceComboBox);
        preperenceComboBox->setCurrentIndex(idx);
    }

    idx = directionComboBox->findText(Value[3]);
    if (idx != -1) {
        table->setCellWidget(i, 3, directionComboBox);
        directionComboBox->setCurrentIndex(idx);
    }

    idx = driverComboBox->findText(Value[6]);
    if (idx != -1) {
        table->setCellWidget(i, 6, driverComboBox);
        driverComboBox->setCurrentIndex(idx);
    }

    idx = qualifierComboBox->findText(Value[7]);
    if (idx != -1) {
        table->setCellWidget(i, 7, qualifierComboBox);
        qualifierComboBox->setCurrentIndex(idx);
    }

}
