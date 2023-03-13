#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DemoQT.h"

class DemoQT : public QMainWindow
{
    Q_OBJECT

public:
    DemoQT(QWidget *parent = nullptr);
    ~DemoQT();

private:
    Ui::DemoQTClass ui;
};
