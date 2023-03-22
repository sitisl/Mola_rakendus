#include "FormName.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormName *formName = new FormName;
    formName->show();
    return a.exec();
}
