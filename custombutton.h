#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QObject>
#include <QPushButton>

class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit CustomButton(QObject *parent = nullptr);
    explicit CustomButton(const QString &iconPath, QWidget *parent = nullptr) : QPushButton(parent) {
        this->setIcon(QIcon(iconPath));  // Set the image
        this->setIconSize(QSize(24, 24));  // Set the image size
    }
signals:

};

#endif // CUSTOMBUTTON_H
