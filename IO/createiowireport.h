#ifndef CREATEIOWIREPORT_H
#define CREATEIOWIREPORT_H

#include <QWidget>
#include <QTableWidget>
#include "Constructor/ipxactparser.h"

class CreateIOWirePort : public QWidget
{
    Q_OBJECT
public:
    explicit CreateIOWirePort(QWidget *parent = nullptr);

    void showContextMenu(const QPoint &pos);
    void doInsertRow();
    void doDeleteRow();
    void LoadWireXML(QString xmlFile);
    void Addrow(QString Value[11]);

signals:
private:
    IPXACTParser wireParser;
public:
    QTableWidget *table;

};

#endif // CREATEIOWIREPORT_H
