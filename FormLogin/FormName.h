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
    void on_btnAvatar_clicked();
    void insertAvatar(const QString& avatarIconPath);

private:
    QString avatarPath;
    Ui::QMainWindow ui;
    FormMessenger* formMessenger;

};
