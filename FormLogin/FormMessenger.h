#pragma once

#include <QMainWindow>
#include "ui_FormMessenger.h"

class FormMessenger : public QMainWindow
{
	Q_OBJECT

public:
	FormMessenger(QWidget *parent = nullptr);
	~FormMessenger();

private:
	Ui::FormMessengerClass ui;
};
