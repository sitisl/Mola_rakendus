#include "FormLoginDiag.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormLoginDiag *formLogin = new FormLoginDiag();
    formLogin->show();
    return a.exec();
}
