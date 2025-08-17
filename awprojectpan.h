#ifndef AWPROJECTPAN_H
#define AWPROJECTPAN_H

#include "qpushbutton.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class AWProjectPan : public QPushButton
{
    Q_OBJECT
public:
    explicit AWProjectPan(QWidget *parent = nullptr);
signals:

protected:
    void mousePressEvent(QMouseEvent *event);
//    void resizeEvent(QResizeEvent *event);


    QLabel *label; // Add QLabel here
};

#endif // AWPROJECTPAN_H
