#ifndef IPTREEVIEW_H
#define IPTREEVIEW_H

#include <QWidget>
#include <QTreeWidget>

class IPTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit IPTreeView(QWidget *parent = nullptr);

signals:

};

#endif // IPTREEVIEW_H
