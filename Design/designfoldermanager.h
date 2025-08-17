#ifndef DESIGNFOLDERMANAGER_H
#define DESIGNFOLDERMANAGER_H

//#include <QListWidget>
//#include <QListWidgetItem>

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCheckBox>
#include <QDir>

class DesignFolderManager : public QWidget {
    Q_OBJECT

public:
    DesignFolderManager(QWidget* parent = nullptr, bool IsFileCtrl=false)
        : QWidget(parent) {


        layout = new QVBoxLayout(this);
        folderTree = new QTreeWidget(this);
        folderTree->setColumnCount(3);
        if(IsFileCtrl)
            folderTree->setHeaderLabels(QStringList() << "Del" << "Select" << "File Name");
        else
            folderTree->setHeaderLabels(QStringList() << "Del" << "Select" << "Folder Name");

        connect(folderTree, &QTreeWidget::itemChanged, this, &DesignFolderManager::onItemChanged);
        //항목클릭시
        connect(folderTree, &QTreeWidget::itemClicked, this, &DesignFolderManager::onItemClicked);

        layout->addWidget(folderTree);

        folderTree->setRootIsDecorated(false); // 루트의 장식(라인 등)을 숨김
        folderTree->setAlternatingRowColors(false); // 교대 행 색상을 비활성화

        setLayout(layout);
    }

    void addFolder(const QString& folderName) {
        QTreeWidgetItem* item = new QTreeWidgetItem(folderTree);
        item->setCheckState(0, Qt::Unchecked); // 첫 번째 컬럼에 체크박스 추가
       // item->setCheckState(1, Qt::Checked);   // 두 번째 컬럼에 체크박스 추가
        item->setText(2, folderName); // 세 번째 컬럼에 폴더 이름 추가
    }

    void addFile(const QString& fileName) {
        QTreeWidgetItem* item = new QTreeWidgetItem(folderTree);
        item->setCheckState(0, Qt::Unchecked); // 첫 번째 컬럼에 체크박스 추가
       // item->setCheckState(1, Qt::Checked);   // 두 번째 컬럼에 체크박스 추가
        item->setText(2, fileName); // 세 번째 컬럼에 폴더 이름 추가
    }

    //세버째 컬럼 텍스를 가져옴
    QStringList getAll3rdItemTextList() const {
        QStringList folderNames;
        int itemCount = folderTree->topLevelItemCount();
        for (int i = 0; i < itemCount; ++i) {
            QTreeWidgetItem* item = folderTree->topLevelItem(i);

            if (item->checkState(1) == Qt::Checked) {  // 두 번째 컬럼의 체크 상태가 Checked인지 확인
                folderNames.append(item->text(2));  // 세 번째 컬럼의 텍스트를 가져옵니다.
            }

            //folderNames.append(item->text(2));  // 세 번째 컬럼의 텍스트를 가져옵니다.
        }
        return folderNames;
    }

private slots:
    void onItemChanged(QTreeWidgetItem* item, int column) {
        if(column == 0 && item->checkState(0) == Qt::Checked) {
            delete item; // 첫 번째 컬럼의 체크박스가 체크되면 항목 삭제
        }

        //..할일 두 번째 컬럼의 체크박스 상태 변경에 따른 로직을 여기에 추가 - 선택이 하나만 되어야 할 때,

        if(column == 1 && item->checkState(0) == Qt::Checked) {

            //다른 항목은 모두 uncheck
            if(objectName()=="IncludeListCtrl"){

            }

            if(objectName()=="FilesListCtrl"){

            }
        }
    }

    void onItemClicked(QTreeWidgetItem* item, int column) {

       // if(objectName()=="FilesListCtrl"){

            if (column == 1) {  // 두 번째 컬럼이 클릭된 경우

                if (item->checkState(1) == Qt::Checked) {  // 클릭된 항목이 체크된 경우
                    item->setCheckState(1, Qt::Unchecked);  // 체크 해제

                } else {  // 클릭된 항목이 체크되지 않은 경우
                    item->setCheckState(1, Qt::Checked);  // 체크 설정

                    int itemCount = folderTree->topLevelItemCount();
                    for (int i = 0; i < itemCount; ++i) {
                        QTreeWidgetItem* otherItem = folderTree->topLevelItem(i);
                        if (otherItem != item) {  // 클릭된 항목을 제외한 다른 항목들
                            otherItem->setCheckState(1, Qt::Unchecked);  // 체크 해제
                        }
                    }
                } //else
            } // if


   //     }

    }

    QString getSelectedItemText3rd() const {
        int itemCount = folderTree->topLevelItemCount();
        for (int i = 0; i < itemCount; ++i) {
            QTreeWidgetItem* item = folderTree->topLevelItem(i);
            if (item->checkState(1) == Qt::Checked) {  // 두 번째 컬럼의 체크 상태가 Checked인지 확인
                return item->text(2);  // 해당 항목의 세 번째 컬럼의 텍스트를 반환합니다.  - 파일명 또는 폴더명 임
            }
        }
        return QString();  // 선택된 항목이 없으면 빈 QString을 반환합니다.
    }

private:
    QVBoxLayout* layout;
    QTreeWidget* folderTree;
    bool IsFileCtrl;
};


/*
class DesignFolderManager : public QWidget {
    Q_OBJECT

public:
    DesignFolderManager(QWidget* parent = nullptr)
        : QWidget(parent) {

        layout = new QVBoxLayout(this);
        folderList = new QListWidget(this);
        connect(folderList, &QListWidget::itemChanged, this, &DesignFolderManager::onItemChanged);
        layout->addWidget(folderList);

        setLayout(layout);
    }

    void addFolder(const QString& folderName) {
        QListWidgetItem* item = new QListWidgetItem(folderName, folderList);
        QCheckBox* checkbox = new QCheckBox(this);
        folderList->addItem(item);
        folderList->setItemWidget(item, checkbox);
        connect(checkbox, &QCheckBox::toggled, [this, item](bool checked) {
            if (checked) {
                int row = folderList->row(item);
                folderList->takeItem(row);
                delete item; // 삭제할 때 항목 메모리 해제
            }
        });
    }

private slots:
    void onItemChanged(QListWidgetItem* item) {
        QCheckBox* checkbox = qobject_cast<QCheckBox*>(folderList->itemWidget(item));
        if (!checkbox->isChecked()) {
            checkbox->setChecked(true);
        }
    }

private:
    QVBoxLayout* layout;
    QListWidget* folderList;
};
*/
#endif // DESIGNFOLDERMANAGER_H
