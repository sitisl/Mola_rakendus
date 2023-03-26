#include "FormName.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QFont amazingFont("Consolas", 8, QFont::Monospace, true);
    QApplication::setFont(amazingFont);
    QApplication a(argc, argv);
    FormName w;
    w.show();

    return a.exec();
}
