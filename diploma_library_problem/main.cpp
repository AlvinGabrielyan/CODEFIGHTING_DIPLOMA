#include "mainwindow.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Librarian Problem Solver");
    a.setOrganizationName("Diploma Project");
    a.setApplicationVersion("2.0");
    QFont font("Segoe UI", 10);
    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
