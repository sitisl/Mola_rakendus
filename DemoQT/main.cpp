#include "DemoQT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DemoQT w;
    w.show();
    return a.exec();
}
