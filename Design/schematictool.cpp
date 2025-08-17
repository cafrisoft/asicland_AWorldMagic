#include "schematictool.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>


SchematicTool::SchematicTool(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("SchematicTool { border: 1px solid black; }");
    QString borderStyle = "border: 1px solid black;";

    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // 주 레이아웃

    mainLayout->addSpacing(30);

    // 기본 폰트 설정
    QFont baseFont;
    baseFont.setPointSize(10);
    baseFont.setBold(true);

    //////////////////////////////////////////////////////

    // 레이아웃 설정
    this->setLayout(mainLayout);
}
