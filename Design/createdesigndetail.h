#ifndef CREATEDESIGNDETAIL_H
#define CREATEDESIGNDETAIL_H

#include <QWidget>
#include <QTableWidget>

class CreateDesignDetail : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDesignDetail(QWidget *parent);

private:
    QTableWidget *designDetailtable;

signals:

private:

};

#endif // CREATEDESIGNDETAIL_H
