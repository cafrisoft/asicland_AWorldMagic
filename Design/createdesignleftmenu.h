#ifndef CREATEDESIGNLEFTMENU_H
#define CREATEDESIGNLEFTMENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>

// 전방 선언을 추가합니다.
class CreateDesignMain;
class CreateDesignGeneral;
class CreateDesignFileSet;
class CreateDesignPortMap;
class CreateDesignSchematic;
class TabDocument;

class CreateDesignLeftMenu : public QWidget
{
    Q_OBJECT
public:
//    explicit CreateDesignLeftMenu(QWidget *parent , TabDocument* designSchematic);
    explicit CreateDesignLeftMenu(QWidget *parent);

private:
    QLabel *createDesignLabel;

//    TabDocument* designSchematic_;

public:
    CreateDesignMain *designMain_;
    CreateDesignGeneral* generalWidget_;
    CreateDesignFileSet* fileSetWidget_;
    CreateDesignPortMap* portMapWidget_;
    CreateDesignSchematic* designSchematic_;

signals:
    void createEmptyDesign();

    void GeneralInfoInput();
    void FileSetInput();
    void PortMapInput();
    void SchematicInput();
    void CreateDesignInfo();
    void CancelDesign();

private slots:  // 슬롯 섹션을 추가합니다.
    void handleGeneralInfoInput();
    void handleFileSetInput();
    void handlePortMapInput();
    void handleSchematicInput();
    void handleCreateDesignInfo();
    void handleCancelDesign();

public :
    void SetTargetWiget(CreateDesignMain* pMain ,
                        CreateDesignGeneral* generalWidget,
                        CreateDesignFileSet* fileSetWidget,
                        CreateDesignPortMap* portMapWidget,
                        CreateDesignSchematic* designSchematic);
};

#endif // CREATEDESIGNLEFTMENU_H
