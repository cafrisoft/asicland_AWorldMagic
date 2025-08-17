#ifndef CREATEIPMAIN_H
#define CREATEIPMAIN_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStackedWidget>
#include "createipleftmenu.h"
#include "createipgeneral.h"
#include "createipfileset.h"
#include "createipportmap.h"

enum IpSubType
{
    __General,
    FileSet,
    PortMap
};

class CreateIPMain : public QWidget {

public:
    CreateIPMain(QWidget *parent);
    // 외부에서 호출할 함수
    void ShowWidget(enum IpSubType type);

private:
    QStackedWidget *stackedWidgets;
    CreateIpGeneral *generalWidget;
    CreateIpFileSet *fileSetWidget;
    CreateIpPortMap *portMapWidget;

};

#endif // CREATEIPMAIN_H
