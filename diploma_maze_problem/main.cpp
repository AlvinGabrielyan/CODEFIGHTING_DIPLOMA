#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Դինամիկ Լաբիրինթոս");
    w.showMaximized();
    return app.exec();
}
