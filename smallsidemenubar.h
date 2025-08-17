#ifndef SMALLSIDEMENUBAR_H
#define SMALLSIDEMENUBAR_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>


class SmallSideMenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit SmallSideMenuBar(QWidget *parent = nullptr);

private:
    QPushButton *fileBtn_;
//    QPushButton *projectBtn_;
    QPushButton *playBtn_;
    QPushButton *configBtn_;

private:

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        //painter.setBrush(QBrush(Qt::blue));
        painter.setBrush(QBrush(QColor("#2682ED")));
        //painter.drawRect(this->rect());
        painter.drawRect(this->rect().adjusted(-1, -1, 1, 1));
    }

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
    void onFileBtnClicked();
//    void onProjectBtnClicked();
    void onPlayBtnClicked();
    void onConfigBtnClicked();

    void onNewBusTriggered();
    void onNewComponentTriggered();
    void onNewProjectTriggered();
    void onNewDesignTriggered();

    void onOpenProjectTriggered();
    void onCloseProjectTriggered();

    void onImportProjectTriggered();
    void onExportProjectTriggered();

    void onSaveTriggered();
    void onSaveAllTriggered();

    void onRunSimulatePreTriggered();
    void onRunSimulateTriggered();
    void onRunSynthesisTriggered();
    void onRunFPGATriggered();

    void onPreferenceTriggered();
    void onHelpTriggered();
    void onAboutTriggered();
};

#endif // SMALLSIDEMENUBAR_H
