#include "createioleftmenu.h"
#include <QVBoxLayout>
#include "ClickableLabel.h"
#include "createiomain.h"
#include "createiogeneral.h"
#include "createiowireport.h"
#include <QMessageBox>
#include <QDir>

CreateIOLeftMenu::CreateIOLeftMenu(CallbackType callback, QWidget *parent) : QWidget(parent), m_callback(callback) {

    setStyleSheet("CreateIOLeftMenu { border: 1px solid black; }");

    //setStyleSheet("border: 1px solid black;");


    QVBoxLayout *layout = new QVBoxLayout;

    createIOLabel = new QLabel("Create\nI/O", this);
    createIOLabel->setAlignment(Qt::AlignCenter);  // 텍스트를 중앙에 정렬합니다.
    QFont font = createIOLabel->font();  // 현재 폰트를 가져옵니다.
    font.setBold(true);                  // Bold로 설정합니다.
    font.setPointSize(16);               // 폰트 크기를 20px로 설정합니다.
    createIOLabel->setFont(font);        // 라벨에 폰트를 설정합니다.


    layout->addSpacing(10);
    layout->addWidget(createIOLabel, 0, Qt::AlignCenter);

    ClickableLabel *generalLabel = new ClickableLabel("General", this);
    connect(generalLabel, &ClickableLabel::clicked, this, &CreateIOLeftMenu::handleGeneralInfoInput);
    layout->addSpacing(35);
    layout->addWidget(generalLabel, 0, Qt::AlignCenter);

    ClickableLabel *wirePortLabel = new ClickableLabel("Wire Port", this);
    connect(wirePortLabel, &ClickableLabel::clicked, this, &CreateIOLeftMenu::handleWirePortInput);
    layout->addSpacing(15);
    layout->addWidget(wirePortLabel, 0, Qt::AlignCenter);


    layout->addStretch(1);

    QPushButton *createButton = new QPushButton("Create", this);
    connect(createButton, &QPushButton::clicked, this, &CreateIOLeftMenu::handleCreateIoInfo);
    layout->addWidget(createButton);

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    layout->addWidget(cancelButton);
    layout->setContentsMargins(5, 0, 5, 10);
    layout->setSpacing(5);
    connect(cancelButton, &QPushButton::clicked, this, &CreateIOLeftMenu::handleCancelIO);


    this->setLayout(layout);

    //connect(this, &CreateIOLeftMenu::GeneralInfoInput, this, &CreateIOLeftMenu::handleGeneralInfoInput);
    //connect(this, &CreateIOLeftMenu::WirePortInput, this, &CreateIOLeftMenu::handleWirePortInput);
    connect(this, &CreateIOLeftMenu::GeneralInfoInput, this, &CreateIOLeftMenu::handleCreateIoInfo);
    connect(this, &CreateIOLeftMenu::CancelIO, this, &CreateIOLeftMenu::handleCancelIO);

}

void CreateIOLeftMenu::handleGeneralInfoInput()
{
    createIOLabel->setText("Create\nI/O");
    ioMain->ShowWidget(true);
}

void CreateIOLeftMenu::handleWirePortInput()
{
    createIOLabel->setText("I/O\nDetail");
    ioMain->ShowWidget(false);
}

void CreateIOLeftMenu::handleCreateIoInfo() //..<--
{
    //General의 MakeXML()은 io.xml이고
    QString sTitle = generalWidget->MakeXML(wirePortWidget->table);
    //..todo 폴더보이기
    m_callback(sTitle); //"MakeXML  완료... 탭 이름 바꿔야되");
}

void CreateIOLeftMenu::handleCancelIO()
{
    QString curDir = generalWidget->strCurDir;
    //if(StandardButton.Yes == )

    QMessageBox::information(this, "삭제",  curDir+"내에 생성된 파일을 삭제합니다.");
    removeDir(curDir);
}

bool CreateIOLeftMenu::removeDir(const QString &dirPath) {
    QDir dir(dirPath);

    if (!dir.exists()) {
        return false;
    }

    // 폴더 내의 각 항목에 대한 정보를 얻기
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::AllEntries);
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list[i];
        if (fileInfo.isDir()) {
            // 재귀적으로 하위 폴더 삭제
            removeDir(fileInfo.absoluteFilePath());
        } else {
            // 파일 삭제
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }

    // 이제 폴더 내의 모든 항목이 삭제되었으므로 폴더 자체를 삭제
    return dir.rmdir(dir.absolutePath());
}

void CreateIOLeftMenu::SetLinkWindow(CreateIOGeneral* _generalWidget, CreateIOWirePort* _wirePortWidget)
{
    generalWidget   = _generalWidget;
    wirePortWidget  = _wirePortWidget;
}
