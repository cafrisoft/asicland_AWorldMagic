#include "WorkingIndicator.h"

#include <QLabel>
#include <QImage>
#include <QTimer>



WorkingIndicator::WorkingIndicator(QObject *parent)
{
    colorA = QColor(50, 50, 50);
    colorB = QColor(150, 150, 150);
    colorC = QColor(255, 255, 255);

    image = QImage("/resources/Res/Waiting.png");

    duration = 6000;
    step = 0;
    maxSteps = duration / 2000;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WorkingIndicator::updateColor);
    timer->start(2000);
}



void WorkingIndicator::updateColor()
{
    if (step >= maxSteps) {
        timer->stop();
        return;
    }

    double progress = static_cast<double>(step) / maxSteps;

    QImage newImage = image;

    for (int y = 0; y < newImage.height(); ++y)
    {
        for (int x = 0; x < newImage.width(); ++x)
        {
            QColor originalColor = newImage.pixelColor(x, y);

            int r = colorA.red() * (1 - progress) + colorB.red() * progress;
            int g = colorA.green() * (1 - progress) + colorB.green() * progress;
            int b = colorA.blue() * (1 - progress) + colorB.blue() * progress;

            newImage.setPixelColor(x, y, QColor(r, g, b, originalColor.alpha()));
        }
    }

    setPixmap(QPixmap::fromImage(newImage));
    step++;
}
