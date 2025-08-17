#ifndef DESIGNRIGHTTOOLBAR_H
#define DESIGNRIGHTTOOLBAR_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>

class DesignRightToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit DesignRightToolBar(QWidget *parent = nullptr);

private:
    QPushButton *ProjectBtn;

signals:
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


//public slots:
private slots:
    void projectBtnClicked();

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);
        //painter.setBrush(QBrush(Qt::blue));
        painter.setBrush(QBrush(QColor("#2682ED")));
        //painter.drawRect(this->rect());
        painter.drawRect(this->rect().adjusted(-1, -1, 1, 1));
    }

};

#endif // DESIGNRIGHTTOOLBAR_H
