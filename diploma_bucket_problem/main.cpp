#include "mainwindow.h"
#include "inputwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    InputWindow input;
    if (input.exec() != QDialog::Accepted)
        return 0;
    MainWindow window(nullptr, input.capA(), input.capB(), input.target());
    window.show();

    return app.exec();
}
