#ifndef AWSIDETOOLBAR_H
#define AWSIDETOOLBAR_H


#include <QWidget>
#include <QPushButton>

#include "smallsidemenubar.h"
#include "bigsidemenubar.h"

#include <QPainter>

class AWSideToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit AWSideToolBar(QWidget *parent = nullptr);

    SmallSideMenuBar *smallSideMenuBar_;
    BigSideMenuBar *bigSideMenuBar_;
    QPushButton *resizeBtn;

private:
    void connectMenuBar();

protected:
    /*
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::blue));
        painter.drawRect(this->rect());

        // 자식 위젯들의 사각형 부분을 그리지 않습니다.
        foreach (QWidget* w, this->findChildren<QWidget*>()) {
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            painter.drawRect(w->geometry());
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
    }*/

signals:

#if LOG_OUT_SUPPORT_20240104 == 1
    /*!
     *  Transfers error messages between the main window and the message log out console.
     *
     *      @param [in] message     The error message.
     */
    void inputMessage_LogOut(QString const& message);

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

    //! Print a message to the user.
    void inputMessage(const QString& msg);
    void noticeMessage(const QString& msg);
    void errorMessage(const QString& msg);
    void undefinedMessage(const QString& msg);

public slots:
    void resizeBtnClicked();

    void onActionNewTriggered();
    void onActionSaveTriggered();
    void onActionSaveAllTriggered();

    void onNewBus();
    void onNewComponent();
    void onNewProject();
    void onNewDesign();

    void onOpenProject();
    void onCloseProject();

    void onImportProject();
    void onExportProject();

    void onSave();
    void onSaveAll();

    void onRunSimulatePre();
    void onRunSimulate();
    void onRunSynthesis();
    void onRunFPGA();

    void onPreference();
    void onHelp();
    void onAbout();
};

#endif // AWSIDETOOLBAR_H
