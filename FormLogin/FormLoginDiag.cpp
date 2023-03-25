#include "FormLoginDiag.h"
#include "FormMessenger.h"

FormLoginDiag::FormLoginDiag(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    ui.lblError->hide();
}

FormLoginDiag::~FormLoginDiag()
{}

void FormLoginDiag::on_btnLogin_clicked()
{
    QString strUserName = ui.lineEditUsername->text();
    QString strPassWord = ui.lineEditPassword->text();
    if (strUserName == "Pets" && strPassWord == "1234")
    {
        hide();
        formMessenger = new FormMessenger(this);
        formMessenger->show();
    }
    else
    {
        ui.lblError->show();
        ui.lblError->setStyleSheet("QLabel { color : red; }");
        //ui.lblError->setText("Vale kasutajanimi parool!");
        ui.lineEditPassword->clear();
        //ui.statusBar->showMessage("Vale kasutajanimi voi salasona", 3000);
    }

}


void FormLoginDiag::on_lineEditPassword_textEdited(const QString& arg1) 
{
    ui.lblError->hide();
}
