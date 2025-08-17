#ifndef CREATEDESIGNFILESET_H
#define CREATEDESIGNFILESET_H

#include <QWidget>
#include <QString>

#include "designfoldermanager.h"

class CreateDesignGeneral;

class CreateDesignFileSet : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDesignFileSet(QWidget *parent, CreateDesignGeneral* designGen);

public:
    CreateDesignGeneral* _designGen;

    DesignFolderManager *includeListCtrl;
    DesignFolderManager *filestCtrl;
    //QStringList Includes;
    //QStringList VHDLFiles;

signals:
    void createEmptyDesign();

private slots:
    void onIncludeAddClicked();
    void onFileAddClicked();

public slots:
    bool onCreateEmptyDesign();

public:
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

};

#endif // CREATEDESIGNFILESET_H
