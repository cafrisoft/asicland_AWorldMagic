#ifndef CREATEPROJECTMAIN_H
#define CREATEPROJECTMAIN_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStackedWidget>
#include "createprojectgeneral.h"
#include "createprojectleftmenu.h"

#include <QTabWidget>

class CreateProjectMain : public QWidget {
    Q_OBJECT
public:
    explicit CreateProjectMain(QTabWidget *parent = nullptr);

    // 외부에서 호출할 함수
    void ShowWidget(bool isGeneral);

    QStackedWidget *stackedWidgets;

public:
    CreateProjectGeneral *generalWidget;
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



#endif // CREATEPROJECTMAIN_H
