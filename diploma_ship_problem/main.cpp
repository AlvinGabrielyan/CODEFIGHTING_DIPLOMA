#include <QApplication>
#include "mainwindow.h"
#include "boat.h"
#include "command.h"
#include "interpreter.h"
#include "simulator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Հումանիտար կամրջային լոգիստիկայի սիմուլյատոր");

    Simulator *simulator = new Simulator(&app);

    // Ա->Բ ուղղությամբ գնացող նավակներ
    Boat *b1 = new Boat("B1", "Swift-A", "A", 8, 12, 150,
                        QColor("#ef4444"),
                        "Արագ նավակ, չոր սննդի համար (8-12ր)",
                        simulator);
    b1->setAllowedCargoTypes({"FOOD_DRY"});
    simulator->addBoat(b1);

    Boat *b2 = new Boat("B2", "Cargo-A", "A", 12, 18, 300,
                        QColor("#f59e0b"),
                        "Ծանր բեռներ կրող նավակ շինանյութերի համար (12-18ր)",
                        simulator);
    b2->setAllowedCargoTypes({"BUILDING_MATERIALS"});
    simulator->addBoat(b2);

    Boat *b3 = new Boat("B3", "Multi-A", "A", 6, 9, 120,
                        QColor("#10b981"),
                        "Բազմաֆունկցիոնալ՝ սարքավորումների և վիրակապերի համար (6-9ր)",
                        simulator);
    b3->setAllowedCargoTypes({"EQUIPMENT", "MED_BANDAGES"});
    simulator->addBoat(b3);

    //B->A ուղղությամբ գնացող նավակներ

    Boat *b4 = new Boat("B4", "Fresh-B", "B", 7, 11, 150,
                        QColor("#3b82f6"),
                        "Սառնարանով նավակ - թարմ սննդի համար(7-11ր)",
                        simulator);
    b4->setAllowedCargoTypes({"FOOD_FRESH"});
    simulator->addBoat(b4);

    Boat *b5 = new Boat("B5", "Tank-B", "B", 10, 15, 250,
                        QColor("#06b6d4"),
                        "Ջրի փոխադրման նավակ (10-15ր)",
                        simulator);
    b5->setAllowedCargoTypes({"WATER"});
    simulator->addBoat(b5);

    Boat *b6 = new Boat("B6", "Pharma-B", "B", 5, 8, 120,
                        QColor("#8b5cf6"),
                        "Նավակ՝ հակաբիոտիկների և առաջին բուժօգնության համար(5-8ր)",
                        simulator);
    b6->setAllowedCargoTypes({"MED_ANTIBIOTICS", "MED_FIRSTAID"});
    simulator->addBoat(b6);

    Boat *b7 = new Boat("B7", "Express-B", "B", 4, 6, 100,
                        QColor("#ec4899"),
                        "Նավակ՝ցավազրկողների համար(4-6ր)",
                        simulator);
    b7->setAllowedCargoTypes({"MED_PAINKILLERS"});
    simulator->addBoat(b7);

    MainWindow w(simulator);
    w.show();

    return app.exec();
}
