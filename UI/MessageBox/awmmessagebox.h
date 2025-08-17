#ifndef AWMMESSAGEBOX_H
#define AWMMESSAGEBOX_H

#include <QMessageBox>

class AWMMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    explicit AWMMessageBox(QWidget *parent = 0);
    ~AWMMessageBox();

};

#endif // AWMMESSAGEBOX_H
