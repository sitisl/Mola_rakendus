#include "FormName.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QStackedWidget>
#include <QFile>
#include <QIcon>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication application(argc, argv);
    QIcon windowIcon(":/myresources/icons/Mola.png");
    application.setWindowIcon(windowIcon);
    application.setApplicationName("M\u00D6LA");
    QFile f(":myresources/themes/darkstyle.qss");
    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        application.setStyleSheet(ts.readAll());
    }
    QStackedWidget stackedWidget;
    FormName formName;
    FormMessenger formMessenger;
    stackedWidget.addWidget(&formName);
    //QWidget* formN = stackedWidget.widget(0);
    //formN->setFixedSize(600, 400);
    stackedWidget.addWidget(&formMessenger);

    // Set the stacked widget as the main widget for the application
    application.setActiveWindow(&stackedWidget);
    stackedWidget.show();
    QObject::connect(&formName, &FormName::switchToMessenger, &stackedWidget, &QStackedWidget::setCurrentIndex);
    QObject::connect(&formName, &FormName::switchToMessenger, &formMessenger, &FormMessenger::handleClientData);

    return application.exec();
}
