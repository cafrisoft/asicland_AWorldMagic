#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QTabWidget>
#include <QTabBar>
#include <QHBoxLayout>
#include <QPushButton>

class CustomTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CustomTabBar(QWidget *parent = nullptr);

signals:

};

#endif // CUSTOMTABBAR_H
