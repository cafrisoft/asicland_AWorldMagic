#ifndef CREATEIOMAIN_H
#define CREATEIOMAIN_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStackedWidget>
#include "createioleftmenu.h"
#include "createiogeneral.h"
#include "createiowireport.h"
#include <QTabWidget>

class CreateIOMain : public QWidget {
    Q_OBJECT
public:
    explicit CreateIOMain(QTabWidget *parent = nullptr);

    // 외부에서 호출할 함수
    void ShowWidget(bool isGeneral);


    QStackedWidget *stackedWidgets;
public:
    CreateIOWirePort *wirePortWidget;
    CreateIOGeneral *generalWidget;
private:
    QTabWidget* parentTabWidget;

public :
    int getCurrentIndex() {
        if (parentTabWidget) {
            return parentTabWidget->indexOf(this);
        }
        return -1;
    }
    void ChangeTabTitlebyCallback(QString tabName);
};

#endif // CREATEIOMAIN_H
