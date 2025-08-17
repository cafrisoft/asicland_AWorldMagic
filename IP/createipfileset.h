#ifndef CREATEIPFILESET_H
#define CREATEIPFILESET_H

#include <QWidget>
#include <QString>

#include "foldermanager.h"

class CreateIpGeneral;

class CreateIpFileSet : public QWidget
{
    Q_OBJECT
public:
    explicit CreateIpFileSet(QWidget *parent, CreateIpGeneral* ipGen);

    CreateIpGeneral* _ipGen;

    FolderManager *includeListCtrl;
    FolderManager *filestCtrl;
    //QStringList Includes;
    //QStringList VHDLFiles;

    QStringList GetIncludes() {
        return includeListCtrl->getAll3rdItemTextList();
    }

    QStringList GetVHDLFiles() {
        return filestCtrl->getAll3rdItemTextList();
    }

    void AddTestData()
    {
        //#ifndef DEBUG
            includeListCtrl->addFolder(QString("/home/dubi/AMagic/library/asicland/ip/atcbmc300/3003/native/hdl"));
            filestCtrl->addFolder(QString("/home/dubi/AMagic/library/asicland/ip/atcbmc300/3003/native/hdl/atcbmc300.v")); // 변경 필요: 파일 이름을 추가하는 메서드 이름으로 변경
            filestCtrl->addFolder(QString("/home/dubi/AMagic/library/asicland/ip/atcbmc300/3003/native/hdl/atcbmc300_ds_addr_ctrl.v")); // 변경 필요: 파일 이름을 추가하는 메서드 이름으로 변경
        //#endif

    }

signals:
private slots:
    void onIncludeAddClicked();
    void onFileAddClicked();

};

#endif // CREATEIPFILESET_H
