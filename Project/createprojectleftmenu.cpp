#include "createprojectleftmenu.h"
#include <QVBoxLayout>
#include "ClickableLabel.h"
#include "createprojectmain.h"
#include "createprojectgeneral.h"
//#include "createiowireport.h"
#include <QMessageBox>
#include <QDir>

CreateProjectLeftMenu::CreateProjectLeftMenu(CallbackType callback, QWidget *parent) : QWidget(parent), m_callback(callback) {

    setStyleSheet("CreateIOLeftMenu { border: 1px solid black; }");

    //setStyleSheet("border: 1px solid black;");


    QVBoxLayout *layout = new QVBoxLayout;

    createIOLabel = new QLabel("Create\nProject", this);
    createIOLabel->setAlignment(Qt::AlignCenter);  // 텍스트를 중앙에 정렬합니다.
    QFont font = createIOLabel->font();  // 현재 폰트를 가져옵니다.
    font.setBold(true);                  // Bold로 설정합니다.
    font.setPointSize(16);               // 폰트 크기를 20px로 설정합니다.
    createIOLabel->setFont(font);        // 라벨에 폰트를 설정합니다.


    layout->addSpacing(10);
    layout->addWidget(createIOLabel, 0, Qt::AlignCenter);

    ClickableLabel *generalLabel = new ClickableLabel("General", this);
    connect(generalLabel, &ClickableLabel::clicked, this, &CreateProjectLeftMenu::handleGeneralInfoInput);
    layout->addSpacing(35);
    layout->addWidget(generalLabel, 0, Qt::AlignCenter);

    layout->addStretch(1);

    QPushButton *createButton = new QPushButton("Create", this);
    connect(createButton, &QPushButton::clicked, this, &CreateProjectLeftMenu::handleCreateIoInfo);
    layout->addWidget(createButton);

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    layout->addWidget(cancelButton);
    layout->setContentsMargins(5, 0, 5, 10);
    layout->setSpacing(5);
    connect(cancelButton, &QPushButton::clicked, this, &CreateProjectLeftMenu::handleCancelIO);


    this->setLayout(layout);

    //connect(this, &CreateIOLeftMenu::GeneralInfoInput, this, &CreateIOLeftMenu::handleGeneralInfoInput);
    //connect(this, &CreateIOLeftMenu::WirePortInput, this, &CreateIOLeftMenu::handleWirePortInput);
    connect(this, &CreateProjectLeftMenu::GeneralInfoInput, this, &CreateProjectLeftMenu::handleCreateIoInfo);
    connect(this, &CreateProjectLeftMenu::CancelIO, this, &CreateProjectLeftMenu::handleCancelIO);

}

void CreateProjectLeftMenu::handleGeneralInfoInput()
{
    createIOLabel->setText("Create\nI/O");
    _Main->ShowWidget(true);
}

void CreateProjectLeftMenu::handleWirePortInput()
{
    createIOLabel->setText("I/O\nDetail");
    _Main->ShowWidget(false);
}

void CreateProjectLeftMenu::handleCreateIoInfo() //..<--
{
    QString sTitle = generalWidget->GetProjectFileName();
    m_callback(sTitle); // " 탭 이름 바꿈

    // 폴더이름을 구해서 폴더 생성
    QString sDir = generalWidget->GetCurrentFileDir();

    // 폴더 생성
    QDir dir;
    if ( !sDir.isEmpty() ){
        if(!dir.mkpath(sDir))
            return ;  // 폴더 생성에 실패한 경우 false 반환
        else{
            QString  sPrjDir =  sDir+"/"+sTitle;
            if(dir.mkpath(sPrjDir))
            {
                dir.mkpath(sPrjDir+"/design");
                dir.mkpath(sPrjDir+"/ip");
                dir.mkpath(sPrjDir+"/io");
            }
        }

        //파일복사
}

}

void CreateProjectLeftMenu::handleCancelIO()
{
    QString curDir = generalWidget->strCurDir;
    //if(StandardButton.Yes == )

    QMessageBox::information(this, "삭제",  curDir+"내에 생성된 파일을 삭제합니다.");
    removeDir(curDir);
}

bool CreateProjectLeftMenu::removeDir(const QString &dirPath) {
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

void CreateProjectLeftMenu::SetLinkWindow(CreateProjectGeneral* _generalWidget/*, CreateIOWirePort* _wirePortWidget*/)
{
    generalWidget   = _generalWidget;
    //wirePortWidget  = _wirePortWidget;
}
