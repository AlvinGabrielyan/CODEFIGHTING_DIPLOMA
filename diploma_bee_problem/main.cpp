#include <QApplication>
#include <QFont>
#include "beesimulatorwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFont font = app.font();
    font.setFamily("Arial");
    font.setPointSize(10);
    app.setFont(font);

    BeeSimulatorWindow window;
    window.show();

    return app.exec();
}
