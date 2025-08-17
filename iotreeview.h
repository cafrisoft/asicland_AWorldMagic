#ifndef IOTREEVIEW_H
#define IOTREEVIEW_H

#include <QTreeView>

class IOTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit IOTreeView(QWidget *parent = nullptr);

signals:

};

#endif // IOTREEVIEW_H
