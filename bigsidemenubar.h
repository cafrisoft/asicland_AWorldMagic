#ifndef BIGSIDEMENUBAR_H
#define BIGSIDEMENUBAR_H

#include <QWidget>
#include "qdebug.h"
#include <QPainter>
#include <QVBoxLayout>

class BigSideMenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit BigSideMenuBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        //painter.setBrush(QBrush(Qt::blue));
        painter.setBrush(QBrush(QColor("#2682ED")));
        //painter.drawRect(this->rect());
        painter.drawRect(this->rect().adjusted(-1, -1, 1, 1));

        // 자식 위젯들의 사각형 부분을 그리지 않습니다.
        //        foreach (QWidget* w, this->findChildren<QWidget*>()) {
        //          painter.setCompositionMode(QPainter::CompositionMode_Clear);
        //        painter.drawRect(w->geometry());
        //      painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        // }
    }

//    void addFileBtnMenu(QVBoxLayout  *layout);
//    void addPlayBtnMenu(QVBoxLayout  *layout);
//    void addCfgBtnMenu(QVBoxLayout  *layout);
    void addMenu(QVBoxLayout  *layout, const QString &name, bool hasSubMenu);

signals:    
#if LOG_OUT_SUPPORT_20240104 == 1
    /*!
     *  Transfers error messages between the main window and the message log out console.
     *
     *      @param [in] message     The error message.
     */
    void errorMessage_LogOut(QString const& message);

    /*!
     *  Transfers notice messages between the main window and the message log out console.
     *
     *      @param [in] message     The notice message.
     */
    void noticeMessage_LogOut(QString const& message);

#endif // LOG_OUT_SUPPORT_20240104

    void newBus();
    void newComponent();
    void newProject();
    void newDesign();

    void openProject();
    void closeProject();

    void importProject();
    void exportProject();

    void saveDocument();
    void saveAllDocument();

    void runSimulatePre();
    void runSimulate();
    void runSynthesis();
    void runFPGA();

    void openPreference();
    void openHelp();
    void openAbout();

private slots:
    void onNewBusClicked();
    void onNewComponentClicked();
    void onNewProjectClicked();
    void onNewDesignClicked();

    void onOpenProjectClicked();
    void onCloseProjectClicked();

    void onImportProjectClicked();
    void onExportProjectClicked();

    void onSaveClicked();
    void onSaveAllClicked();

    void onRunSimulatePreClicked();
    void onRunSimulateClicked();
    void onRunSynthesisClicked();
    void onRunFPGAClicked();

    void onPreferenceClicked();
    void onHelpClicked();
    void onAboutClicked();
};

#endif // BIGSIDEMENUBAR_H
