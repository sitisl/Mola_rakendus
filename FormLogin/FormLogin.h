#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FormLogin.h"

class FormLogin : public QMainWindow
{
    Q_OBJECT

public:
    FormLogin(QWidget *parent = nullptr);
    ~FormLogin();

private:
    Ui::FormLoginClass ui;
};
