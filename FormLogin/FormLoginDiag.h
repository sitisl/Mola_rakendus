#pragma once

#include <QDialog>
#include "ui_FormLoginDiag.h"
#include "FormMessenger.h"

class FormLoginDiag : public QDialog
{
	Q_OBJECT

public:
	FormLoginDiag(QWidget *parent = nullptr);
	~FormLoginDiag();

private slots:
	void on_btnLogin_clicked();
	void on_lineEditPassword_textEdited(const QString &arg1);

private:
	Ui::FormLoginDiagClass ui;
	FormMessenger* formMessenger;
};
