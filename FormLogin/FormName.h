#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FormName.h"
#include "FormMessenger.h"
#include <QShortcut>

class FormName : public QMainWindow
{
    Q_OBJECT

public:
    FormName(QWidget *parent = nullptr);
    ~FormName();

private slots:
    void on_btnJatka_clicked();

private:
    Ui::QMainWindow ui;
    FormMessenger* formMessenger;

};
