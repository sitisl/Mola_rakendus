#include "FormName.h"
#include "FormMessenger.h"

FormName::FormName(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(":/myresources/icons/Mola.png"));
}

FormName::~FormName()
{}

void FormName::on_btnJatka_clicked()
{
    QString strUserName = ui.lineNimi->text();
    if (strUserName != "")
    {
        hide();
        formMessenger = new FormMessenger(strUserName, this);
        formMessenger->show();
    }
    else
    {
        ui.statusBar->showMessage("Sisesta kasutajanimi.", 3000);
    }

}

