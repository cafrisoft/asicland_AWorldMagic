#include "createdesignfileset.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QCheckBox>
#include <QTextEdit>
#include <QDebug>
#include "folderselector.h"
#include "designfoldermanager.h"

CreateDesignFileSet::CreateDesignFileSet(QWidget *parent, CreateDesignGeneral*designGen)
    : QWidget{parent}
{
    _designGen = designGen;

    connect(this, SIGNAL(createEmptyDesign()), this, SLOT(onCreateEmptyDesign()), Qt::UniqueConnection);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addSpacing(30);

    QLabel *label1 = new QLabel("File Set", this);
    QFont font1;
    font1.setPointSize(16);
    font1.setBold(true);
    label1->setFont(font1);
    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label1->setContentsMargins(10, 0, 0, 0);
    mainLayout->addWidget(label1);

    QLabel *label2 = new QLabel("File Set information of Design", this);
    QFont font2;
    font2.setPointSize(12);
    label2->setFont(font2);
    label2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label2->setContentsMargins(10, 0, 30, 0);
    mainLayout->addWidget(label2);


    // "Include" label and IncludeAdd button
    QHBoxLayout *includeLayout = new QHBoxLayout();
    QLabel *includeLabel = new QLabel("Include", this);
    QPushButton *includeAdd = new QPushButton("+", this);
    includeAdd->setFixedSize(25, 25); // 크기 설정
    includeAdd->setObjectName("IncludeAdd");
    includeLayout->addWidget(includeLabel);
    includeLayout->addWidget(includeAdd);
    mainLayout->addLayout(includeLayout);

    // IncludeListCtrl with checkboxes
    /*DesignFolderManager */includeListCtrl = new DesignFolderManager(this);
    includeListCtrl->setObjectName("IncludeListCtrl");
    mainLayout->addWidget(includeListCtrl);

    // "Files" label and FileAdd button
    QHBoxLayout *filesLayout = new QHBoxLayout();
    QLabel *filesLabel = new QLabel("Files", this);
    QPushButton *fileAdd = new QPushButton("+", this);
    fileAdd->setFixedSize(25, 25); // 크기 설정
    fileAdd->setObjectName("FileAdd");
    filesLayout->addWidget(filesLabel);
    filesLayout->addWidget(fileAdd);
    mainLayout->addLayout(filesLayout);

    // IncludeListCtrl with checkboxes
    filestCtrl = new DesignFolderManager(this, true);
    filestCtrl->setObjectName("FilesListCtrl");
    mainLayout->addWidget(filestCtrl);


    connect(includeAdd, &QPushButton::clicked, this, &CreateDesignFileSet::onIncludeAddClicked);
    connect(fileAdd, &QPushButton::clicked, this, &CreateDesignFileSet::onFileAddClicked);

    // "Preview" label
    QLabel *previewLabel = new QLabel("Preview", this);
    mainLayout->addWidget(previewLabel);

    // Multiline TextBox
    QTextEdit *multilineTextBox = new QTextEdit(this);
    multilineTextBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 윈도우의 폭과 높이를 모두 사용
    mainLayout->addWidget(multilineTextBox);


    this->setLayout(mainLayout);



}

void CreateDesignFileSet::onIncludeAddClicked() {
    qDebug() << "Include Add button clicked!";

    QString homeDir = QDir::homePath();
    homeDir = homeDir + "/AMagic";

    QString dir = QFileDialog::getExistingDirectory(this, "Select a directory",
                                                    homeDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()) {
        includeListCtrl->addFolder(dir);
        //curInclude = dir;
    }
}

void CreateDesignFileSet::onFileAddClicked() {

    qDebug() << "File Add button clicked!";

    QString homeDir = QDir::homePath();
    homeDir = homeDir + "/AMagic";

    QStringList filters;

    filters << "files (*.v *.vh *.h)";

    /*
    filters //<< "C/C++ files (*.c *.cpp *.h)"
            << "Verilog files (*.v)"
            << "HDL files (*.hdl)";
*/
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Select a file",
                                                    homeDir,
                                                    filters.join(";;"),
                                                    nullptr,
                                                    QFileDialog::DontResolveSymlinks);

    if(!fileName.isEmpty()) {
        filestCtrl->addFolder(fileName); // 변경 필요: 파일 이름을 추가하는 메서드 이름으로 변경
        //curVHDLFile = fileName;
    }
}

bool CreateDesignFileSet::onCreateEmptyDesign()
{
    return false;
}

