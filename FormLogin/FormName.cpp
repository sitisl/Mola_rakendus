#include "FormName.h"
#include "FormMessenger.h"

FormName::FormName(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(":/myresources/icons/Mola.png"));
    //Create shortcut for return key, to login when pressing enter.
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    QObject::connect(shortcut, SIGNAL(activated()), ui.btnJatka, SLOT(click()));
    
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

