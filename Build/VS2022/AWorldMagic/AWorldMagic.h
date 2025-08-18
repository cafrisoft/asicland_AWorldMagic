#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AWorldMagic.h"

class AWorldMagic : public QMainWindow
{
    Q_OBJECT

public:
    AWorldMagic(QWidget *parent = nullptr);
    ~AWorldMagic();

private:
    Ui::AWorldMagicClass ui;
};

