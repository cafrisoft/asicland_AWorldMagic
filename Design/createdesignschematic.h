#ifndef CREATEDESIGNSCHEMATIC_H
#define CREATEDESIGNSCHEMATIC_H

#include <QWidget>

class CreateDesignGeneral;
class CreateDesignFileSet;
class CreateDesignPortMap;
class SchematicTool;
class TabDocument;
class CreateDesignSchematic : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDesignSchematic(QWidget *parent,
                                   CreateDesignGeneral* designGen,
                                   CreateDesignFileSet* designFileSet,
                                   CreateDesignPortMap* designPortMap,
                                   TabDocument* designSchematic);

private:

private:
    CreateDesignGeneral* _designGen;
    CreateDesignFileSet* _designFileSet;
    CreateDesignPortMap* _designPortMap;
    TabDocument* _designSchematic;

signals:
    void createEmptyDesign();

public slots:
    bool onCreateEmptyDesign();

public:
    void CreateDesignXml();
    QString IoabsDefFile();

};

#endif // CREATEDESIGNSCHEMATIC_H
