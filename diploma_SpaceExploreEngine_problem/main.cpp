#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Space Explorer Mission");
    app.setOrganizationName("YourUniversity");
    app.setApplicationVersion("1.0");

    MainWindow window;
    window.show();

    return app.exec();
}
