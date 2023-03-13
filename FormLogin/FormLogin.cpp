#include "FormLogin.h"
#include "FormMessenger.h"

FormLogin::FormLogin(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

FormLogin::~FormLogin()
{}

void FormLogin::on_btnLogin_clicked()
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
        ui.statusBar->showMessage("Vale kasutajanimi voi salasona",3000);
    }
    
}
