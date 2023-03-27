#include "FormName.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    //QFont amazingFont("Consolas", 8, QFont::Monospace, true);
    //QApplication::setFont(amazingFont);
    QApplication application(argc, argv);
    QFile f(":myresources/themes/darkstyle.qss");
    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        application.setStyleSheet(ts.readAll());
    }
    FormName formName;
    formName.show();

    return application.exec();
}
