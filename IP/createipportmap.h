#ifndef CREATEIPPORTMAP_H
#define CREATEIPPORTMAP_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>

#include "Constructor/ioportmapparser.h"

class CreateIpGeneral;
class CreateIpFileSet;

class CreateIpPortMap : public QWidget
{
    Q_OBJECT
public:
    explicit CreateIpPortMap(QWidget *parent, CreateIpGeneral* ipGen, CreateIpFileSet* ipFileSet );
    void CreateIpXml();

signals:
private:
    CreateIpGeneral* _ipGen;
    CreateIpFileSet* _ipFileSet;

public:
    void showPortsContextMenu(const QPoint &pos);
    void showIoContextMenu(const QPoint &pos);
    void showParametersContextMenu(const QPoint &pos);

    void doPortsInsertRow();
    void doPortsDeleteRow();

    void doIOInsertRow();
    void doIODeleteRow();
    void doParameterInsertRow();
    void doParameterDeleteRow();

    void LoadIpPortMapXML(QString xmlFile);
    void Addrow(QString Value[11]);
    QStringList getColumnTexts(QTableWidget *table, QComboBox *localCB, int column);

    IOPortMapParser portMapParser;
    QTableWidget *iotable;
    QTableWidget *porttable;
    QTableWidget *paratable;

};

#endif // CREATEIPPORTMAP_H
