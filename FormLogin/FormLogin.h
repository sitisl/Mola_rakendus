#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FormLogin.h"
#include "FormMessenger.h"

class FormLogin : public QMainWindow
{
    Q_OBJECT

public:
    FormLogin(QWidget *parent = nullptr);
    ~FormLogin();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::CFormLogin ui;
    FormMessenger* formMessenger;
};
