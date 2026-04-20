#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Algorithm Challenges Launcher");
    a.setApplicationVersion("2.0");
    MainWindow w;
    w.show();
    return a.exec();
}
