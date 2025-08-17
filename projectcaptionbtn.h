#ifndef PROJECTCAPTIONBTN_H
#define PROJECTCAPTIONBTN_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QPainter>

class ProjectCaptionBtn : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectCaptionBtn(QWidget *parent = nullptr);

signals:

private slots:
    void button1Clicked() {
        QMessageBox::information(this, "Button Clicked", "Button 1 was clicked!");
    }

    void button2Clicked() {
        QMessageBox::information(this, "Button Clicked", "Button 2 was clicked!");
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event);

        QPainter painter(this);
        painter.drawText(rect(), Qt::AlignCenter, tr("Project"));

        painter.setPen(QPen(Qt::black, 1)); // Set the pen to draw the rectangle

        // Draw a rectangle with the current size of the widget
        painter.drawRect(0, 0, width() - 1, height() - 1);

        // Draw the text in the center of the widget
        painter.drawText(rect(), Qt::AlignCenter, tr("Project"));
    }
};

#endif // PROJECTCAPTIONBTN_H
