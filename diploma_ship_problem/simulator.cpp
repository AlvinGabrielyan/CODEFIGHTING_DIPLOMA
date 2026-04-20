#include "simulator.h"
#include <QDebug>

Simulator::Simulator(QObject *parent)
    : QObject(parent),
    m_currentTime(0.0),
    m_isRunning(false),
    m_speed(1.0),
    m_bridgeHealth(100),
    m_bridgeOccupied(false),
    m_completedBoats(0),
    m_collisions(0),
    m_currentCommandIndex(0),
    m_totalScore(0),
    m_stars(0)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    connect(m_timer, &QTimer::timeout, this, &Simulator::update);

    m_interpreter = new Interpreter(this);

    m_currentOccupancy.boatId = "";
    m_currentOccupancy.startTime = 0.0;
    m_currentOccupancy.endTime = 0.0;

    initializeRequirements();
}

Simulator::~Simulator()
{
    qDeleteAll(m_boats);
    qDeleteAll(m_commands);
}

void Simulator::initializeRequirements()
{
    // Քաղաք A - Պահանջներ (Բ → Ա) (Ընդհանուր: 650 կգ)
    m_deliveries["A_MED_ANTIBIOTICS"] = CargoDelivery{"MED_ANTIBIOTICS", 0, 120, "A"};
    m_deliveries["A_MED_FIRSTAID"] = CargoDelivery{"MED_FIRSTAID", 0, 80, "A"};
    m_deliveries["A_MED_PAINKILLERS"] = CargoDelivery{"MED_PAINKILLERS", 0, 100, "A"};
    m_deliveries["A_FOOD_FRESH"] = CargoDelivery{"FOOD_FRESH", 0, 150, "A"};
    m_deliveries["A_WATER"] = CargoDelivery{"WATER", 0, 200, "A"};

    // Քաղաք B - Պահանջներ (Ա → Բ) (Ընդհանուր: 650 կգ)
    m_deliveries["B_BUILDING_MATERIALS"] = CargoDelivery{"BUILDING_MATERIALS", 0, 400, "B"};
    m_deliveries["B_EQUIPMENT"] = CargoDelivery{"EQUIPMENT", 0, 100, "B"};
    m_deliveries["B_MED_BANDAGES"] = CargoDelivery{"MED_BANDAGES", 0, 100, "B"};
    m_deliveries["B_FOOD_DRY"] = CargoDelivery{"FOOD_DRY", 0, 50, "B"};

}

void Simulator::setSpeed(double speed)
{
    if (!qFuzzyCompare(m_speed, speed)) {
        m_speed = qBound(0.1, speed, 5.0);
        emit speedChanged();
    }
}

void Simulator::addBoat(Boat *boat)
{
    m_boats.append(boat);
    m_boatMap[boat->id()] = boat;
    m_boatMovements[boat->id()] = 0;
    emit totalBoatsChanged();
}

Boat* Simulator::getBoat(const QString &id)
{
    return m_boatMap.value(id.toUpper(), nullptr);
}

QList<QObject*> Simulator::getAllBoats()
{
    QList<QObject*> result;
    for (Boat *boat : m_boats) {
        result.append(boat);
    }
    return result;
}

bool Simulator::loadScript(const QString &script)
{
    qDeleteAll(m_commands);
    m_commands.clear();

    m_commands = m_interpreter->parseScript(script);

    QStringList boatIds;
    for (Boat *boat : m_boats) {
        boatIds << boat->id();
    }

    for (Command *cmd : m_commands) {
        m_interpreter->validateCommand(cmd, boatIds);
    }

    m_interpreter->validateAllCommands(m_commands);

    return m_interpreter->errors().isEmpty();
}

QStringList Simulator::getErrors() const
{
    return m_interpreter->errors();
}

QStringList Simulator::getWarnings() const
{
    return m_interpreter->warnings();
}

void Simulator::start()
{
    if (m_commands.isEmpty()) {
        emit logMessage(" Սցենարը դատարկ է կամ ունի սխալներ", "error");
        return;
    }

    if (!m_interpreter->errors().isEmpty()) {
        emit logMessage(" Հնարավոր չէ սկսել սիմուլյացիան սխալների պատճառով", "error");
        return;
    }

    m_isRunning = true;
    m_timer->start();
    emit isRunningChanged();
    emit logMessage("▶ ՍԻՄՈՒԼՅԱՑԻԱՆ ՍԿՍՎԵՑ", "success");
}

void Simulator::pause()
{
    m_isRunning = false;
    m_timer->stop();
    emit isRunningChanged();
    emit logMessage(" ԴԱԴԱՐ", "info");
}

void Simulator::stop()
{
    m_isRunning = false;
    m_timer->stop();
    emit isRunningChanged();
    emit logMessage(" ԿԱՆԳ", "warning");
}

void Simulator::reset()
{
    stop();

    m_currentTime = 0.0;
    m_completedBoats = 0;
    m_collisions = 0;
    m_currentCommandIndex = 0;
    m_bridgeHealth = 100;
    m_bridgeOccupied = false;
    m_totalScore = 0;
    m_stars = 0;

    m_bridgeSchedule.clear();
    m_activeBoats.clear();
    m_currentOccupancy.boatId = "";
    m_boatMovements.clear();

    // Վերականգնել բեռների տվյալները
    initializeRequirements();

    for (Boat *boat : m_boats) {
        boat->setFuel(100);
        boat->setIsMoving(false);
        boat->setProgress(0.0);
        boat->unloadCargo();
        // Վերականգնել սկզբնական կողմը
        boat->setSide(boat->initialSide());
        boat->setPositionX(boat->initialSide() == "A" ? 0.0 : 1.0);
        boat->setTargetSide("");
        m_boatMovements[boat->id()] = 0;
    }

    emit currentTimeChanged();
    emit completedBoatsChanged();
    emit collisionsChanged();
    emit bridgeHealthChanged();
    emit bridgeOccupiedChanged();
    emit totalScoreChanged();
    emit starsChanged();
    emit logMessage("ՎԵՐԱԿԱՆԳՆՎԱԾ", "info");
}

void Simulator::step()
{
    if (!m_isRunning) {
        m_currentTime += 0.1 * m_speed;
        emit currentTimeChanged();
        updateBoatPositions();
        checkBridgeStatus();
    }
}

void Simulator::update()
{
    if (!m_isRunning) return;

    m_currentTime += 0.1 * m_speed;
    emit currentTimeChanged();

    // Կատարել հրամանները ըստ executeTime-ի
    while (m_currentCommandIndex < m_commands.size()) {
        Command *cmd = m_commands[m_currentCommandIndex];

        if (cmd->executeTime() <= m_currentTime && !cmd->executed()) {
            executeCommand(cmd);
            cmd->setExecuted(true);
            m_currentCommandIndex++;
        } else {
            break;
        }
    }

    updateBoatPositions();
    checkBridgeStatus();

    bool allCommandsExecuted = (m_currentCommandIndex >= m_commands.size());
    bool allBoatsFinished = m_activeBoats.isEmpty();
    bool allBoatsDelivered = checkAllBoatsCompleted();

    if (allCommandsExecuted && allBoatsFinished && allBoatsDelivered) {
        stop();
        calculateFinalScore();

        emit logMessage("\n ՍԻՄՈՒԼՅԱՑԻԱՆ ԱՎԱՐՏՎԵՑ", "success");
        emit simulationFinished();
    }
}

bool Simulator::checkAllBoatsCompleted()
{
    // Ստուգել որ բոլոր 7 նավակները գոնե մեկ անգամ անցել են
    int requiredBoats = 7;
    int boatsThatMoved = 0;

    for (auto it = m_boatMovements.begin(); it != m_boatMovements.end(); ++it) {
        if (it.value() > 0) {
            boatsThatMoved++;
        }
    }

    return (boatsThatMoved >= requiredBoats);
}

void Simulator::calculateFinalScore()
{
    m_totalScore = 0;

    emit logMessage(" ԲԵՌՆԵՐԻ ԱՌԱՔՄԱՆ ՎԵՐԼՈՒԾՈՒԹՅՈՒՆ", "info");
    emit logMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", "info");

    // Հաշվել ընդհանուր հասցված բեռը
    int totalDelivered = 0;

    // Հաշվել յուրաքանչյուր բեռի միավորները
    for (auto it = m_deliveries.begin(); it != m_deliveries.end(); ++it) {
        CargoDelivery &delivery = it.value();

        if (delivery.required == 0) continue;

        totalDelivered += delivery.delivered;

        double completionRate = qMin(1.0, (double)delivery.delivered / delivery.required);
        int points = (int)(completionRate * 100);

        m_totalScore += points;

        QString statusIcon = completionRate >= 1.0 ? "✅" : (completionRate >= 0.5 ? "⚠️" : "❌");

        emit logMessage(
            QString("%1 %2 → Քաղաք %3: %4/%5 կգ (%6%) | Միավոր: %7")
                .arg(statusIcon)
                .arg(delivery.cargoType)
                .arg(delivery.destination)
                .arg(delivery.delivered)
                .arg(delivery.required)
                .arg((int)(completionRate * 100))
                .arg(points),
            completionRate >= 1.0 ? "success" : (completionRate >= 0.5 ? "warning" : "error")
            );
    }

    // Հաշվել աստղերը
    m_stars = calculateStars();
    QString starDisplay = QString("⭐").repeated(m_stars);

    // Հաշվել միջին վառելիք
    double avgFuel = calculateAverageFuel();

    // Հաշվել նավակների օգտագործումը
    int boatsUsed = 0;
    for (auto it = m_boatMovements.begin(); it != m_boatMovements.end(); ++it) {
        if (it.value() > 0) boatsUsed++;
    }

    emit logMessage("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", "info");
    emit logMessage("🏆 ՎԵՐՋՆԱԿԱՆ ԱՐԴՅՈՒՆՔ", "success");
    emit logMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", "info");
    emit logMessage(QString(" ԸՆԴՀԱՆՈՒՐ ՀԱՍՑՎԱԾ ԲԵՌԸ: %1/1300 կգ (%2%%)").arg(totalDelivered).arg((int)(100.0 * totalDelivered / 1300)),
                    totalDelivered >= 1300 ? "success" : "warning");
    emit logMessage(QString(" Միավորներ: %1").arg(m_totalScore), "success");
    emit logMessage(QString(" Ընդհանուր ժամանակ: %1 րոպե").arg(m_currentTime, 0, 'f', 1), "info");
    emit logMessage(QString(" Բախումներ: %1").arg(m_collisions), m_collisions == 0 ? "success" : "error");
    emit logMessage(QString(" Միջին վառելիք: %1%%").arg(avgFuel, 0, 'f', 1), avgFuel > 50 ? "success" : "warning");
    emit logMessage(QString(" Կամրջի ամրություն: %1%%").arg(m_bridgeHealth), m_bridgeHealth == 100 ? "success" : (m_bridgeHealth >= 80 ? "warning" : "error"));
    emit logMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━", "info");

    if (boatsUsed < 7) {
        emit logMessage("ՁԱԽՈՂՎԱԾ: Բոլոր 7 նավակները ՊԵՏՔ Է հասցված լինեն!", "error");
        emit logMessage(QString("🌟 Գնահատական: 0 ԱՍՏՂ"), "error");
    } else {
        emit logMessage(QString("🌟 Գնահատական: %1 %2").arg(starDisplay).arg(getStarRating(m_stars)), "success");
    }
    emit logMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n", "info");

    // Խորհուրդներ բարելավման համար
    if (m_stars < 5 || boatsUsed < 7) {
        emit logMessage("💡 ԽՈՐՀՈՒՐԴՆԵՐ ԲԱՐԵԼԱՎՄԱՆ ՀԱՄԱՐ:", "info");

        if (boatsUsed < 7) {
            emit logMessage(" ՊԱՐՏԱԴԻՐ: Բոլոր 7 նավակները պետք է գոնե 1 անգամ հասցվեն", "error");
        }
        if (totalDelivered < 1300) {
            int needed = 1300 - totalDelivered;
            emit logMessage(QString(" 5 ԱՍՏՂԻ ՀԱՄԱՐ: Հասցրու ողջ 1300 կգ բեռը (պակասում է %1 կգ)").arg(needed), "warning");
        }
        if (totalDelivered >= 1300) {
            emit logMessage(" Շնորհավորում ենք! Հասցրել ես ողջ 1300 կգ բեռը!", "success");
        }
        if (avgFuel < 50) {
            emit logMessage(" Ավելացրու REFUEL հրամաններ վառելիքի համար", "info");
        }
    }

    emit totalScoreChanged();
    emit starsChanged();
}

int Simulator::calculateStars()
{
    // Ստուգել արդյոք բոլոր 7 նավակները հասցվել են (գոնե 1 անցում)
    int boatsCompleted = 0;
    for (auto it = m_boatMovements.begin(); it != m_boatMovements.end(); ++it) {
        if (it.value() > 0) boatsCompleted++;
    }

    // ԵԹԵ ԲՈԼՈՐ 7 ՆԱՎԱԿՆԵՐԸ ՉԵՆ ՀԱՍՑՎԵԼ → ԱՎՏՈՄԱՏ 0 ԱՍՏՂ
    if (boatsCompleted < 7) {
        return 0;
    }

    // Հաշվել ընդհանուր հասցված բեռի քանակը
    int totalDelivered = 0;
    for (auto it = m_deliveries.begin(); it != m_deliveries.end(); ++it) {
        totalDelivered += it.value().delivered;
    }

    if (totalDelivered >= 1300) {
        return 5;
    }
    else if (totalDelivered >= 1170) {
        return 4;
    }
    else if (totalDelivered >= 1040) {
        return 3;
    }
    else if (totalDelivered >= 910) {
        return 2;
    }
    else {
        return 1;
    }
}

QString Simulator::getStarRating(int stars)
{
    switch(stars) {
    case 5: return "⭐⭐⭐⭐⭐ ԿԱՏԱՐՅԱԼ - 1300 կգ (100%)";
    case 4: return "⭐⭐⭐⭐ ԳԵՐԱԶԱՆՑ - 1170+ կգ (90%)";
    case 3: return "⭐⭐⭐ ԼԱՎ - 1040+ կգ (80%)";
    case 2: return "⭐⭐ ԲԱՎԱՐԱՐ - 910+ կգ (70%)";
    case 1: return "⭐ ԹՈՒՅԼ - Բոլոր 7 նավակները հասցված են";
    case 0: return "ՁԱԽՈՂՎԱԾ - Պահանջվում է բոլոր 7 նավակների տեղափոխում";
    default: return "Անհայտ";
    }
}

double Simulator::calculateAverageFuel()
{
    if (m_boats.isEmpty()) return 0.0;

    int totalFuel = 0;
    for (Boat *boat : m_boats) {
        totalFuel += boat->fuel();
    }

    return (double)totalFuel / m_boats.size();
}

void Simulator::executeCommand(Command *cmd)
{
    switch (cmd->type()) {
    case Command::SEND:
        executeSend(cmd);
        break;
    case Command::REFUEL:
        executeRefuel(cmd);
        break;
    case Command::LOAD:
        executeLoad(cmd);
        break;
    case Command::UNLOAD:
        executeUnload(cmd);
        break;
    case Command::CHECK_BRIDGE:
        executeCheckBridge(cmd);
        break;
    default:
        break;
    }

    emit commandExecuted(cmd, true);
}

void Simulator::executeSend(Command *cmd)
{
    Boat *boat = getBoat(cmd->boatId());
    if (!boat) {
        emit logMessage(QString(" ՍԽԱԼ: '%1'նավակը չի գտնվել").arg(cmd->boatId()), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    QString from = cmd->params()["from"].toString().toUpper();
    QString to = cmd->params()["to"].toString().toUpper();

    if (boat->side() != from) {
        emit logMessage(QString(" ՍԽԱԼ: %1-ը գտնվում է %2 կողմում, ոչ թե %3-ում")
                            .arg(boat->name()).arg(boat->side()).arg(from), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    if (!boat->canMove()) {
        emit logMessage(QString(" ՍԽԱԼ: %1-ը չի կարող շարժվել (ցածր վառելիք)")
                            .arg(boat->name()), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    // Ստուգել բեռը, եթե կա
    if (boat->cargoWeight() > 0) {
        QString cargoType = boat->cargoType().toUpper();

        // Ստուգել, արդյոք նավակը կարող է տանել այս բեռը
        if (!boat->canCarry(cargoType)) {
            emit logMessage(QString(" ՍԽԱԼ: %1-ը ՉԻ ԿԱՐՈՂ տանել %2 տեսակի բեռ")
                                .arg(boat->name())
                                .arg(cargoType),
                            "error");
            emit logMessage(QString("   Թույլատրված բեռներ: %1")
                                .arg(boat->allowedCargoTypes().join(", ")),
                            "error");
            stop();
            emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
            return;
        }

        // Ստուգել արդյոք նպատակակետում պահանջվում է այս բեռը
        QString destinationKey = QString("%1_%2").arg(to).arg(cargoType);
        if (!m_deliveries.contains(destinationKey)) {
            emit logMessage(QString(" ՍԽԱԼ: Քաղաք %1-ում %2 տեսակի բեռ ՉԻ ՊԱՀԱՆՋՎՈՒՄ")
                                .arg(to)
                                .arg(cargoType),
                            "error");
            emit logMessage(QString("  %1-ը փորձում է տանել անպետք բեռ")
                                .arg(boat->name()),
                            "error");
            stop();
            emit logMessage("ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
            return;
        }
    }

    int crossingTime = boat->calculateCrossingTime();

    if (checkCollision(boat->id(), m_currentTime, crossingTime)) {
        m_collisions++;
        m_bridgeHealth = qMax(0, m_bridgeHealth - 10);
        emit collisionsChanged();
        emit bridgeHealthChanged();
        emit collisionDetected(boat->id(), m_currentOccupancy.boatId);
        emit logMessage(QString("️ ԶԳՈՒՇԱՑՈՒՄ! %1-ը բախվեց %2-ին! Կամուրջ: -%10%")
                            .arg(boat->name()).arg(m_currentOccupancy.boatId), "error");
        return;
    }

    boat->setIsMoving(true);
    boat->setProgress(0.0);
    boat->setTargetSide(to);

    // Գրանցել նավակի անցումը
    m_boatMovements[boat->id()]++;

    BridgeOccupancy occupancy;
    occupancy.boatId = boat->id();
    occupancy.startTime = m_currentTime;
    occupancy.endTime = m_currentTime + crossingTime;
    occupancy.direction = from + " -> " + to;
    occupancy.sourceSide = from;
    occupancy.targetSide = to;

    m_bridgeSchedule.append(occupancy);
    m_activeBoats[boat->id()] = occupancy;

    if (m_currentOccupancy.boatId.isEmpty()) {
        m_currentOccupancy = occupancy;
        m_bridgeOccupied = true;
        emit bridgeOccupiedChanged();
    }

    emit boatStartedCrossing(boat);

    // Ավելի մանրամասն լոգ
    QString cargoInfo = "";
    if (boat->cargoWeight() > 0) {
        cargoInfo = QString(" | %1 (%2 կգ)").arg(boat->cargoType()).arg(boat->cargoWeight());
    }

    emit logMessage(QString(" %1 սկսեց անցումը %2 |  %3 րոպե %4")
                        .arg(boat->name())
                        .arg(occupancy.direction)
                        .arg(crossingTime)
                        .arg(cargoInfo),
                    "info");

    boat->consumeFuel(crossingTime * 2);
}

void Simulator::executeWait(Command *cmd)
{
    Boat *boat = getBoat(cmd->boatId());
    if (!boat) {
        emit logMessage(QString(" ՍԽԱԼ: Նավակ '%1'-ը չի գտնվել").arg(cmd->boatId()), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    int duration = cmd->params()["duration"].toInt();
    emit logMessage(QString(" ՍՊԱՍՈՒՄ: %1 սպասում է %2 րոպե")
                        .arg(boat->name()).arg(duration), "info");
}

void Simulator::executeRefuel(Command *cmd)
{
    Boat *boat = getBoat(cmd->boatId());
    if (!boat) {
        emit logMessage(QString(" ՍԽԱԼ: Նավակ '%1'-ը չի գտնվել").arg(cmd->boatId()), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    int amount = cmd->params()["amount"].toInt();
    int oldFuel = boat->fuel();
    boat->setFuel(qMin(100, boat->fuel() + amount));
    int actualAdded = boat->fuel() - oldFuel;

    emit logMessage(QString(" ՎԱՌԵԼԻՔ: %1 +%2% (ընդհանուր %3%)")
                        .arg(boat->name()).arg(actualAdded).arg(boat->fuel()), "success");
}

void Simulator::executeLoad(Command *cmd)
{
    Boat *boat = getBoat(cmd->boatId());
    if (!boat) {
        emit logMessage(QString(" ՍԽԱԼ: Նավակը '%1' չի գտնվել").arg(cmd->boatId()), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    QString cargoType = cmd->params()["cargoType"].toString().toUpper();
    int weight = cmd->params()["weight"].toInt();

    //Նավակը արդեն բեռնված է
    if (boat->cargoWeight() > 0) {
        emit logMessage(QString(" ՍԽԱԼ: %1-ը ԱՐԴԵՆ ԻՍԿ ԲԵՌՆՎԱԾ է %2 կգ %3-ով")
                            .arg(boat->name())
                            .arg(boat->cargoWeight())
                            .arg(boat->cargoType()),
                        "error");
        emit logMessage(QString("   Նախ բեռնաթափիր (UNLOAD), ապա նոր բեռ բեռնիր"), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    //  Նավակը կարող է տանել այս բեռի տեսակը
    if (!boat->canCarry(cargoType)) {
        emit logMessage(QString(" ՍԽԱԼ: %1-ը ՉԻ ԿԱՐՈՂ տանել %2 տեսակի բեռ")
                            .arg(boat->name())
                            .arg(cargoType),
                        "error");
        emit logMessage(QString("   Թույլատրված բեռներ: %1")
                            .arg(boat->allowedCargoTypes().join(", ")),
                        "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    // Բեռը պետք է առկա լինի նավակի ընթացիկ քաղաքում
    QString sourceKey = QString("%1_%2").arg(boat->side()).arg(cargoType);

    // Ստուգել որ այս բեռը պահանջվում է այլ քաղաքում (ոչ թե սկզբնական քաղաքում)
    bool cargoAvailableInOtherCity = false;
    for (auto it = m_deliveries.begin(); it != m_deliveries.end(); ++it) {
        if (it.value().cargoType == cargoType && it.value().destination != boat->side()) {
            cargoAvailableInOtherCity = true;
            break;
        }
    }

    if (!cargoAvailableInOtherCity) {
        QString otherSide = (boat->side() == "A") ? "B" : "A";
        emit logMessage(QString(" ՍԽԱԼ: %1 բեռը ՉԿԱ քաղաք %2-ում")
                            .arg(cargoType)
                            .arg(boat->side()),
                        "error");
        emit logMessage(QString("   %1-ը պետք է լինի քաղաք %2-ում բեռ բեռնելու համար")
                            .arg(boat->name())
                            .arg(otherSide),
                        "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    // Քաշը տարողունակության սահմաններում է
    if (weight > boat->capacity()) {
        emit logMessage(QString(" ՍԽԱԼ: %1-ը չի կարող բեռնել %2 կգ (Առավելագույնը: %3 կգ)")
                            .arg(boat->name())
                            .arg(weight)
                            .arg(boat->capacity()),
                        "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    // Բեռնել բեռը
    if (boat->loadCargo(cargoType, weight)) {
        emit logMessage(QString(" ԲԵՌՆՈՒՄ: %1 բեռնեց %2 (%3 կգ / %4 կգ տարողունակություն)")
                            .arg(boat->name())
                            .arg(cargoType)
                            .arg(weight)
                            .arg(boat->capacity()),
                        "success");
    }
}

void Simulator::executeUnload(Command *cmd)
{
    Boat *boat = getBoat(cmd->boatId());
    if (!boat) {
        emit logMessage(QString(" ՍԽԱԼ: Նավակ '%1'-ը չի գտնվել").arg(cmd->boatId()), "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    // Նավակը ունի բեռ
    if (boat->cargoWeight() == 0) {
        emit logMessage(QString(" ՍԽԱԼ: %1-ը ԴԱՏԱՐԿ է, բեռնաթափելու բան չկա")
                            .arg(boat->name()),
                        "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    QString cargoType = boat->cargoType().toUpper();
    QString destinationKey = QString("%1_%2").arg(boat->side()).arg(cargoType);

    // Բեռը պահանջվում է ընթացիկ քաղաքում
    if (!m_deliveries.contains(destinationKey)) {
        QString otherSide = (boat->side() == "A") ? "B" : "A";
        emit logMessage(QString(" ՍԽԱԼ: Քաղաք %1-ում %2 տեսակի բեռ ՉԻ ՊԱՀԱՆՋՎՈՒՄ")
                            .arg(boat->side())
                            .arg(cargoType),
                        "error");
        emit logMessage(QString("   %1-ը պետք է հասնի քաղաք %2 բեռնաթափելու համար")
                            .arg(boat->name())
                            .arg(otherSide),
                        "error");
        stop();
        emit logMessage(" ՍԻՄՈՒԼՅԱՑԻԱՆ ԴԱԴԱՐԵՑՎԵՑ ՍԽԱԼԻ ՊԱՏՃԱՌՈՎ", "error");
        return;
    }

    m_deliveries[destinationKey].delivered += boat->cargoWeight();

    emit logMessage(
        QString(" ԱՌԱՔՎԱԾ: %1 կգ %2 → Քաղաք %3 | Ընդհանուր: %4/%5 կգ (%6%%)")
            .arg(boat->cargoWeight())
            .arg(cargoType)
            .arg(boat->side())
            .arg(m_deliveries[destinationKey].delivered)
            .arg(m_deliveries[destinationKey].required)
            .arg((int)(100.0 * m_deliveries[destinationKey].delivered / m_deliveries[destinationKey].required)),
        "success"
        );

    boat->unloadCargo();
}

void Simulator::executeCheckBridge(Command *cmd)
{
    Q_UNUSED(cmd)
    emit logMessage(QString(" ԿԱՄՐՋԻ ՍՏՈՒԳՈՒՄ: Վիճակ %1%% | %2")
                        .arg(m_bridgeHealth)
                        .arg(m_bridgeOccupied ? "🔴 ԶԲԱՂՎԱԾ" : "🟢 ԱԶԱՏ"),
                    "info");
}


bool Simulator::checkCollision(const QString &boatId, double startTime, double duration)
{
    double endTime = startTime + duration;

    if (m_bridgeOccupied && startTime < m_currentOccupancy.endTime && m_currentOccupancy.boatId != boatId) {
        return true;
    }

    for (const BridgeOccupancy &occ : m_bridgeSchedule) {
        if (occ.boatId == boatId) continue;

        if ((startTime >= occ.startTime && startTime < occ.endTime) ||
            (endTime > occ.startTime && endTime <= occ.endTime) ||
            (startTime <= occ.startTime && endTime >= occ.endTime)) {
            return true;
        }
    }

    return false;
}

void Simulator::updateBoatPositions()
{
    QStringList finishedBoats;

    for (auto it = m_activeBoats.begin(); it != m_activeBoats.end(); ++it) {
        QString boatId = it.key();
        BridgeOccupancy &occ = it.value();

        Boat *boat = getBoat(boatId);
        if (!boat) continue;

        double elapsed = m_currentTime - occ.startTime;
        double duration = occ.endTime - occ.startTime;

        if (duration <= 0) continue;

        double progress = elapsed / duration;
        progress = qBound(0.0, progress, 1.0);

        boat->setProgress(progress);

        if (occ.sourceSide == "A") {
            boat->setPositionX(progress);
        } else {
            boat->setPositionX(1.0 - progress);
        }

        if (m_currentTime >= occ.endTime) {
            boat->setIsMoving(false);
            boat->setProgress(0.0);

            boat->setSide(occ.targetSide);
            boat->setTargetSide("");

            boat->setPositionX(occ.targetSide == "A" ? 0.0 : 1.0);

            finishedBoats.append(boatId);

            m_completedBoats++;
            emit completedBoatsChanged();
            emit boatFinishedCrossing(boat);
            emit logMessage(QString(" %1-ը հասավ %2 կողմ |  Վառելիք: %3%")
                                .arg(boat->name())
                                .arg(boat->side())
                                .arg(boat->fuel()),
                            "success");
        }
    }

    for (const QString &boatId : finishedBoats) {
        m_activeBoats.remove(boatId);
    }
}

void Simulator::checkBridgeStatus()
{
    if (m_currentOccupancy.boatId.isEmpty()) {
        m_bridgeOccupied = false;
        emit bridgeOccupiedChanged();
        return;
    }

    if (m_currentTime >= m_currentOccupancy.endTime) {
        m_currentOccupancy.boatId = "";
        m_bridgeOccupied = false;
        emit bridgeOccupiedChanged();
    }
}

int Simulator::getRequiredAmount(const QString &side, const QString &cargoType)
{
    QString key = QString("%1_%2").arg(side).arg(cargoType);
    if (m_deliveries.contains(key)) {
        return m_deliveries[key].required;
    }
    return 0;
}
