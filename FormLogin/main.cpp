#include "FormLogin.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormLogin *formLogin = new FormLogin;

    formLogin->show();
    return a.exec();
}
