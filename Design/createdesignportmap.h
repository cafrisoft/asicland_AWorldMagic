#ifndef CREATEDESIGNPORTMAP_H
#define CREATEDESIGNPORTMAP_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>

#include "Constructor/ioportmapparser.h"

class CreateDesignGeneral;
class CreateDesignFileSet;

class CreateDesignPortMap : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDesignPortMap(QWidget *parent, CreateDesignGeneral* designGen, CreateDesignFileSet* designFileSet);

private:
    CreateDesignGeneral* _designGen;
    CreateDesignFileSet* _designFileSet;

signals:
    void createEmptyDesign();

public slots:
    bool onCreateEmptyDesign();

public:
    void showPortsContextMenu(const QPoint &pos);
    void showParametersContextMenu(const QPoint &pos);

    void doPortsInsertRow();
    void doPortsDeleteRow();

    void doParameterInsertRow();
    void doParameterDeleteRow();

    void LoadDesignPortMapXML(QString xmlFile);
    void Addrow(QString Value[11]);
    QStringList getColumnTexts(QTableWidget *table, QComboBox *localCB, int column);

    IOPortMapParser portMapParser;
    QTableWidget *porttable;
    QTableWidget *paratable;

};

#endif // CREATEDESIGNPORTMAP_H
