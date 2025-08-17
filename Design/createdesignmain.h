#ifndef CREATEDESIGNMAIN_H
#define CREATEDESIGNMAIN_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStackedWidget>
#include "createdesignleftmenu.h"
#include "createdesigngeneral.h"
#include "createdesignfileset.h"
#include "createdesignportmap.h"
#include "createdesignschematic.h"

enum DesignSubType
{
    Design_General,
    Design_FileSet,
    Design_PortMap,
    Design_Schematic
};

#if LIBRARYINTERFACE_TEST_20231206 == 0
class LibraryHandler;
#else
class LibraryInterface;
#endif
class TabDocument;
class CreateDesignMain : public QWidget
{
    Q_OBJECT

public:
#if LIBRARYINTERFACE_TEST_20231206 == 0
    CreateDesignMain(LibraryHandler* libInterface, TabDocument* dw, QWidget *parent);
    CreateDesignMain(LibraryHandler* libInterface, QWidget *parent);
#else
    CreateDesignMain(LibraryInterface* libInterface, TabDocument* dw, QWidget *parent);
    CreateDesignMain(LibraryInterface* libInterface, QWidget *parent);
#endif

private:
#if LIBRARYINTERFACE_TEST_20231206 == 0
    //! The library interface.
    LibraryHandler* library_;
#else
    //! The library interface.
    LibraryInterface* library_;
#endif

    DesignSubType type_;

    QStackedWidget *stackedWidgets;
    CreateDesignGeneral *generalWidget;
    CreateDesignFileSet *fileSetWidget;
    CreateDesignPortMap *portMapWidget;
    CreateDesignSchematic *schematicWidget;
//    TabDocument* schematicWidget;

signals:
    void createDesign(VLNV const& vlnv, QString const& directory);

    void createEmptyDesign();
    void createEmptyDesign_General();
    void createEmptyDesign_FileSet();
    void createEmptyDesign_PortMap();
    void createEmptyDesign_Schematic();

public slots:
//public:
    void onCreateEmptyDesign();


//    void createDesign(KactusAttribute::ProductHierarchy prodHier, KactusAttribute::Firmness firmness,
//        VLNV const& vlnv, QString const& directory);
    void onCreateDesign(VLNV const& vlnv, QString const& directory);

public:
    // 외부에서 호출할 함수
    void ShowWidget(enum DesignSubType type);
    DesignSubType GetCurrentWidgetType();

};

#endif // CREATEDESIGNMAIN_H
