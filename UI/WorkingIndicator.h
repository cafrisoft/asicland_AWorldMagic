#ifndef WORKINGINDICATOR_H
#define WORKINGINDICATOR_H

#include <QObject>
#include <QLabel>
#include <QImage>
#include <QColor>



class WorkingIndicator : public QLabel
{
    Q_OBJECT

public:
    explicit WorkingIndicator(QObject *parent = 0);

private:
    QImage image;
    QTimer *timer;

    int duration;
    int step;
    int maxSteps;

    QColor colorA;
    QColor colorB;
    QColor colorC;

signals:

public slots:

private slots:
    void updateColor();
};

#endif // WORKINGINDICATOR_H
