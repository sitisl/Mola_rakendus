#include "FormLoginDiag.h"
#include <QtWidgets/QApplication>
#include <QFile>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(":/qdarkstyle/qdarkstyle/theme/darkstyle.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet. File not found!\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		a.setStyleSheet(ts.readAll());
    }
    FormLoginDiag *formLogin = new FormLoginDiag();
    formLogin->show();
    return a.exec();
}
