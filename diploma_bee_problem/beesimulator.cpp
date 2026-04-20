#include "beesimulator.h"
#include <QDebug>

BeeSimulator::BeeSimulator(QObject *parent)
    : QObject(parent)
    , m_currentTime(0.0)
    , m_isRunning(false)
    , m_speed(1.0)
    , m_completedTrips(0)
    , m_totalNectarCollected(0.0)
    , m_currentCommandIndex(0)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // 50մվ
    connect(m_timer, &QTimer::timeout, this, &BeeSimulator::update);

    m_interpreter = new Interpreter(this);

    // Ինիցիալիզացնել ամանները
    addHoneycomb("COMB1", 5.0);
    addHoneycomb("COMB2", 5.0);
    addHoneycomb("COMB3", 5.0);
    addHoneycomb("COMB4", 5.0);
    addHoneycomb("COMB5", 5.0);
}

BeeSimulator::~BeeSimulator()
{
    qDeleteAll(m_bees);
    qDeleteAll(m_commands);
}

void BeeSimulator::setSpeed(double speed)
{
    if (!qFuzzyCompare(m_speed, speed)) {
        m_speed = qBound(0.1, speed, 5.0);
        emit speedChanged();
    }
}

void BeeSimulator::addBee(Bee *bee)
{
    m_bees.append(bee);
    m_beeMap[bee->id()] = bee;
    emit totalBeesChanged();
}

Bee* BeeSimulator::getBee(const QString &id)
{
    return m_beeMap.value(id, nullptr);
}

QList<QObject*> BeeSimulator::getAllBees()
{
    QList<QObject*> list;
    for (Bee *bee : m_bees) {
        list.append(bee);
    }
    return list;
}

void BeeSimulator::addHoneycomb(const QString &id, double capacity)
{
    HoneycombInfo info;
    info.id = id;
    info.capacity = capacity;
    info.currentAmount = 0.0;
    info.sealed = false;
    m_honeycombs[id] = info;
}

BeeSimulator::HoneycombInfo BeeSimulator::getHoneycombInfo(const QString &id)
{
    return m_honeycombs.value(id, HoneycombInfo());
}

QStringList BeeSimulator::getAllHoneycombIds()
{
    return m_honeycombs.keys();
}

bool BeeSimulator::loadScript(const QString &script)
{
    emit logMessage("Սկրիպտի վերլուծում...", "info");

    qDeleteAll(m_commands);
    m_commands.clear();
    m_currentCommandIndex = 0;

    m_commands = m_interpreter->parseScript(script);

    if (!m_interpreter->errors().isEmpty()) {
        for (const QString &error : m_interpreter->errors()) {
            emit logMessage(error, "error");
        }
        return false;
    }

    if (!m_interpreter->warnings().isEmpty()) {
        for (const QString &warning : m_interpreter->warnings()) {
            emit logMessage(warning, "warning");
        }
    }

    QStringList beeIds;
    for (Bee *bee : m_bees) {
        beeIds << bee->id();
    }

    for (Command *cmd : m_commands) {
        if (!m_interpreter->validateCommand(cmd, beeIds)) {
            return false;
        }
    }

    if (!m_interpreter->validateAllCommands(m_commands)) {
        for (const QString &error : m_interpreter->errors()) {
            emit logMessage(error, "error");
        }
        return false;
    }

    double estimatedTime = m_interpreter->estimateTotalTime(m_commands);
    emit logMessage(QString("Սկրիպտը վերլուծված է։ %1 հրաման, ~%2 վրկ")
                        .arg(m_commands.size())
                        .arg(estimatedTime, 0, 'f', 1), "success");

    QStringList suggestions = m_interpreter->suggestOptimizations(m_commands);
    for (const QString &suggestion : suggestions) {
        emit logMessage(suggestion, "info");
    }

    return true;
}

QStringList BeeSimulator::getErrors() const
{
    return m_interpreter->errors();
}

QStringList BeeSimulator::getWarnings() const
{
    return m_interpreter->warnings();
}

void BeeSimulator::start()
{
    m_isRunning = true;
    m_timer->start();
    emit isRunningChanged();
    emit logMessage("Սիմուլյացիան սկսված է", "info");
}

void BeeSimulator::pause()
{
    m_isRunning = false;
    m_timer->stop();
    emit isRunningChanged();
    emit logMessage("Սիմուլյացիան դադարեցված է", "info");
}


void BeeSimulator::stop()
{
    m_isRunning = false;
    m_timer->stop();
    emit isRunningChanged();
    emit logMessage("Սիմուլյացիան կանգ է առել", "info");
}

void BeeSimulator::reset()
{
    stop();

    m_currentTime = 0.0;
    m_currentCommandIndex = 0;
    m_completedTrips = 0;
    m_totalNectarCollected = 0.0;
    m_activeFlights.clear();

    // Վերականգնել մեղուների դիրքերը
    for (Bee *bee : m_bees) {
        bee->setLocation(bee->initialLocation());
        bee->setEnergy(100);
        bee->setCurrentNectar(0.0);
        bee->setNectarType("");
        bee->setIsFlying(false);
        bee->setProgress(0.0);

        if (bee->initialLocation() == "HIVE") {
            bee->setPositionX(0.45);
            bee->setPositionY(0.75);
        }
    }

    // Վերականգնել ամանները
    for (auto it = m_honeycombs.begin(); it != m_honeycombs.end(); ++it) {
        it->currentAmount = 0.0;
        it->sealed = false;
    }

    for (Command *cmd : m_commands) {
        cmd->setExecuted(false);
    }

    emit currentTimeChanged();
    emit completedTripsChanged();
    emit totalNectarCollectedChanged();
    emit logMessage("Համակարգը վերականգնված է", "info");
}

void BeeSimulator::step()
{
    // Մեկ քայլ առաջ
    if (m_currentCommandIndex < m_commands.size()) {
        Command *cmd = m_commands[m_currentCommandIndex];
        executeCommand(cmd);
        m_currentCommandIndex++;
    }
}

void BeeSimulator::update()
{
    // Ժամանակի ավելացում
    m_currentTime += 0.05 * m_speed;  // 50մվ
    emit currentTimeChanged();

    // Կատարել հրամանները որոնց ժամանակը հասել է
    while (m_currentCommandIndex < m_commands.size()) {
        Command *cmd = m_commands[m_currentCommandIndex];

        if (cmd->executeTime() <= m_currentTime) {
            executeCommand(cmd);
            m_currentCommandIndex++;
        } else {
            break;  // Հաջորդ հրամանը դեռ շուտ է
        }
    }

    // Թարմացնել մեղուների դիրքերը
    updateBeePositions();

    // Ստուգել արդյոք սիմուլյացիան ավարտված է
    if (m_currentCommandIndex >= m_commands.size() && m_activeFlights.isEmpty()) {
        stop();
        emit logMessage("Սիմուլյացիան ավարտված է!", "success");
        emit simulationFinished();
    }
}

void BeeSimulator::updateBeePositions()
{
    for (auto it = m_activeFlights.begin(); it != m_activeFlights.end();) {
        QString beeId = it.key();
        FlightInfo info = it.value();

        Bee *bee = m_beeMap[beeId];
        if (!bee) {
            ++it;
            continue;
        }

        // Հաշվարկել ընթացիկ progress
        double elapsed = m_currentTime - info.startTime;
        double duration = info.endTime - info.startTime;
        double progress = elapsed / duration;

        if (progress >= 1.0) {
            // Թռիչքն ավարտված է
            bee->setProgress(1.0);
            bee->setLocation(info.to);
            bee->setIsFlying(false);

            // Թարմացնել դիրքը
            if (info.to == "HIVE") {
                bee->setPositionX(0.45);
                bee->setPositionY(0.75);
            } else if (info.to.startsWith("COMB")) {
                // Ամանների դիրքեր
                int combNum = info.to.mid(4).toInt() - 1;
                bee->setPositionX(0.15 + (combNum % 3) * 0.15);
                bee->setPositionY(0.6 + (combNum / 3) * 0.15);
            } else {
                // Ծաղիկների դիրքեր
                bee->setPositionX(0.5);
                bee->setPositionY(0.2);
            }

            emit beeFinishedFlying(bee);
            emit logMessage(QString("%1-ն հասավ %2")
                                .arg(bee->name()).arg(info.to), "info");

            // Եթե կա հետաձգված հրաման, կատարել այն
            if (info.pendingCommand) {
                emit logMessage(QString("Կատարվում է հետաձգված հրաման..."), "info");
                executeCommand(info.pendingCommand);
            }

            it = m_activeFlights.erase(it);
            m_completedTrips++;
            emit completedTripsChanged();
        } else {
            // Թարմացնել progress-ը
            bee->setProgress(progress);

            // Ինտերպոլյացիա դիրքի համար
            double startX = 0.5, startY = 0.2;
            double endX = 0.5, endY = 0.2;

            // Սկզբնական դիրք
            if (info.from == "HIVE") {
                startX = 0.45;
                startY = 0.75;
            } else if (info.from.startsWith("COMB")) {
                int combNum = info.from.mid(4).toInt() - 1;
                startX = 0.15 + (combNum % 3) * 0.15;
                startY = 0.6 + (combNum / 3) * 0.15;
            } else if (info.from.startsWith("FLOWER")) {
                startX = 0.5;
                startY = 0.2;
            }

            if (info.to == "HIVE") {
                endX = 0.45;
                endY = 0.75;
            } else if (info.to.startsWith("COMB")) {
                int combNum = info.to.mid(4).toInt() - 1;
                endX = 0.15 + (combNum % 3) * 0.15;
                endY = 0.6 + (combNum / 3) * 0.15;
            } else if (info.to.startsWith("FLOWER")) {
                endX = 0.5;
                endY = 0.2;
            }

            bee->setPositionX(startX + (endX - startX) * progress);
            bee->setPositionY(startY + (endY - startY) * progress);

            ++it;
        }
    }
}


void BeeSimulator::executeCommand(Command *cmd)
{
    if (!cmd || cmd->executed()) return;

    emit logMessage(QString("⚙Տող %1: %2")
                        .arg(cmd->lineNumber())
                        .arg(cmd->toString()), "command");

    bool success = false;
    bool shouldMarkExecuted = true;

    switch (cmd->type()) {
    case Command::FLY:
        executeFly(cmd);
        success = true;
        break;
    case Command::COLLECT:
        executeCollect(cmd);
        success = true;
        break;
    case Command::DEPOSIT:
        // DEPOSIT-ը կարող է ստեղծել թռիչք և հետաձգել կատարումը
        // Այդ դեպքում չպետք է նշել որպես executed
        {
            QString beeId = cmd->beeId();
            Bee *bee = getBee(beeId);
            QString combId = cmd->params()["combId"].toString();

            // Եթե մեղուն պետք է թռչի, հրամանը հետաձգվում է
            if (bee && bee->location() != combId) {
                shouldMarkExecuted = false;
            }
        }
        executeDeposit(cmd);
        success = true;
        break;
    case Command::REST:
        executeRest(cmd);
        success = true;
        break;
    case Command::CHECK_COMB:
        executeCheckComb(cmd);
        success = true;
        break;
    default:
        emit logMessage(QString("Անհայտ հրամանի տեսակ"), "error");
        break;
    }

    if (shouldMarkExecuted) {
        cmd->setExecuted(true);
    }
    emit commandExecuted(cmd, success);
}

void BeeSimulator::executeFly(Command *cmd)
{
    QString beeId = cmd->beeId();
    Bee *bee = getBee(beeId);

    if (!bee) {
        emit logMessage(QString("Մեղու '%1' չի գտնվել").arg(beeId), "error");
        return;
    }

    if (!bee->canFly()) {
        emit logMessage(QString("%1-ը չի կարող թռչել (էներգիա: %2)")
                            .arg(bee->name()).arg(bee->energy()), "error");
        return;
    }

    if (bee->isFlying()) {
        emit logMessage(QString("%1-ը արդեն թռչում է").arg(bee->name()), "error");
        return;
    }

    QString from = cmd->params()["from"].toString();
    QString to = cmd->params()["to"].toString();

    if (bee->location() != from) {
        emit logMessage(QString("%1-ը գտնվում է %2-ում, ոչ թե %3-ում")
                            .arg(bee->name()).arg(bee->location()).arg(from), "warning");
    }

    // Հաշվարկել թռիչքի ժամանակը
    int flightTime = bee->calculateFlightTime();

    // Ստեղծել թռիչքի տեղեկատվություն
    FlightInfo info;
    info.beeId = beeId;
    info.startTime = m_currentTime;
    info.endTime = m_currentTime + flightTime;
    info.from = from;
    info.to = to;
    info.pendingCommand = nullptr;

    m_activeFlights[beeId] = info;

    bee->setIsFlying(true);
    bee->setTargetLocation(to);
    bee->setProgress(0.0);
    bee->consumeEnergy(10);

    emit beeStartedFlying(bee);
    emit logMessage(QString("%1-ը սկսեց թռչել %2→%3 (%4վրկ)")
                        .arg(bee->name()).arg(from).arg(to).arg(flightTime), "success");
}

void BeeSimulator::executeCollect(Command *cmd)
{
    QString beeId = cmd->beeId();
    Bee *bee = getBee(beeId);

    if (!bee) {
        emit logMessage(QString("Մեղու '%1'-ն չի գտնվել").arg(beeId), "error");
        return;
    }

    QString nectarType = cmd->params()["nectarType"].toString();
    double amount = cmd->params()["amount"].toDouble();

    if (!bee->isEmpty()) {
        emit logMessage(QString("%1-ն արդեն ունի նեկտար").arg(bee->name()), "error");
        return;
    }

    if (amount > bee->capacity()) {
        emit logMessage(QString("Քանակը (%1գ) գերազանցում է տարողությունը (%2գ)")
                            .arg(amount).arg(bee->capacity()), "warning");
        amount = bee->capacity();
    }

    if (bee->collectNectar(nectarType, amount)) {
        emit logMessage(QString("%1-ն հավաքեց %2գ %3")
                            .arg(bee->name()).arg(amount).arg(nectarType), "success");
    } else {
        emit logMessage(QString("%1-ն չկարողացավ հավաքել նեկտար")
                            .arg(bee->name()), "error");
    }
}

void BeeSimulator::executeDeposit(Command *cmd)
{
    QString beeId = cmd->beeId();
    Bee *bee = getBee(beeId);

    if (!bee) {
        emit logMessage(QString("Մեղու '%1'-ը չի գտնվել").arg(beeId), "error");
        return;
    }

    if (bee->isEmpty()) {
        emit logMessage(QString("%1-ը դատարկ է (չունի նեկտար)")
                            .arg(bee->name()), "error");
        return;
    }

    QString combId = cmd->params()["combId"].toString();

    if (!m_honeycombs.contains(combId)) {
        emit logMessage(QString("Աման '%1'-ը չի գտնվել").arg(combId), "error");
        return;
    }

    HoneycombInfo &comb = m_honeycombs[combId];

    if (comb.sealed) {
        emit logMessage(QString("Աման %1-ը փակված է").arg(combId), "error");
        return;
    }

    // Եթե մեղուն չէ ամանի մոտ, ավտոմատ թռցնել
    if (bee->location() != combId) {
        QString from = bee->location();

        // Հաշվարկել թռիչքի ժամանակը
        int flightTime = bee->calculateFlightTime();

        //Սկսում ենք ԱՆՄԻՋԱՊԵՍ (m_currentTime), ոչ թե executeTime-ին
        FlightInfo info;
        info.beeId = beeId;
        info.startTime = m_currentTime;
        info.endTime = m_currentTime + flightTime;
        info.from = from;
        info.to = combId;
        info.pendingCommand = cmd;

        m_activeFlights[beeId] = info;

        bee->setIsFlying(true);
        bee->setTargetLocation(combId);
        bee->setProgress(0.0);
        bee->consumeEnergy(10);

        emit beeStartedFlying(bee);
        emit logMessage(QString("%1-ն ավտոմատ թռչում է %2→%3 (%4վրկ)")
                            .arg(bee->name()).arg(from).arg(combId).arg(flightTime), "info");

        // ՉԿԱՏԱՐԵԼ DEPOSIT-ը հիմա - այն կկատարվի երբ մեղուն հասնի
        return;
    }

    // Հիմա մեղուն ամանի մոտ է, կարող է լցնել
    double nectarAmount = bee->currentNectar();
    double availableSpace = comb.capacity - comb.currentAmount;

    if (availableSpace >= nectarAmount) {
        // Ամբողջ նեկտարը տեղավորվում է
        comb.currentAmount += nectarAmount;
        m_totalNectarCollected += nectarAmount;

        bee->depositNectar();

        emit logMessage(QString("%1-ը լցրեց %2գ նեկտար %3-ում (%4/%5գ)")
                            .arg(bee->name())
                            .arg(nectarAmount, 0, 'f', 1)
                            .arg(combId)
                            .arg(comb.currentAmount, 0, 'f', 1)
                            .arg(comb.capacity, 0, 'f', 1), "success");

        // Ստուգել արդյոք ամանը լցվել է
        if (comb.currentAmount >= comb.capacity) {
            comb.sealed = true;
            emit logMessage(QString("Աման %1-ը լիքն է և փակվեց").arg(combId), "info");
        }

        emit totalNectarCollectedChanged();
    } else {
        // Մասամբ լցնել
        double deposited = availableSpace;
        double remaining = nectarAmount - deposited;

        comb.currentAmount = comb.capacity;
        comb.sealed = true;
        m_totalNectarCollected += deposited;

        bee->setCurrentNectar(remaining);

        emit logMessage(QString("%1-ն լցրեց %2գ, մնաց %3գ (Աման %4 լցվեց և փակվեց)")
                            .arg(bee->name())
                            .arg(deposited, 0, 'f', 1)
                            .arg(remaining, 0, 'f', 1)
                            .arg(combId), "warning");

        emit logMessage(QString("%1-ը կուտակեց %2գ նեկտար (տեղ չկար)")
                            .arg(bee->name())
                            .arg(remaining, 0, 'f', 1), "info");

        // Մեղուն ուտում է մնացած նեկտարը
        bee->depositNectar();

        emit totalNectarCollectedChanged();
    }
}

void BeeSimulator::executeRest(Command *cmd)
{
    QString beeId = cmd->beeId();
    Bee *bee = getBee(beeId);

    if (!bee) {
        emit logMessage(QString("Մեղու '%1'-ն չի գտնվել").arg(beeId), "error");
        return;
    }

    int duration = cmd->params()["duration"].toInt();

    // Վերականգնել էներգիան
    int energyRestored = duration * 2;
    bee->setEnergy(qMin(100, bee->energy() + energyRestored));

    emit logMessage(QString("%1-ն հանգստացավ %2վրկ (էներգիա: %3)")
                        .arg(bee->name()).arg(duration).arg(bee->energy()), "info");
}

void BeeSimulator::executeCheckComb(Command *cmd)
{
    QString combId = cmd->params()["combId"].toString();

    if (!m_honeycombs.contains(combId)) {
        emit logMessage(QString("Աման '%1-ը' չի գտնվել").arg(combId), "error");
        return;
    }

    const HoneycombInfo &comb = m_honeycombs[combId];

    QString status = comb.sealed ? "փակված " : "բաց ";
    double percentage = (comb.currentAmount / comb.capacity) * 100.0;

    emit logMessage(QString(" Աման %1: %2/%3գ (%4%) - %5")
                        .arg(combId)
                        .arg(comb.currentAmount, 0, 'f', 1)
                        .arg(comb.capacity, 0, 'f', 1)
                        .arg(percentage, 0, 'f', 0)
                        .arg(status), "info");
}

bool BeeSimulator::checkBeeAvailable(const QString &beeId)
{
    Bee *bee = getBee(beeId);
    if (!bee) return false;

    return !bee->isFlying();
}
