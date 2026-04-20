#include "SpaceExplorerEngine.h"
#include <QDebug>
#include <cmath>

SpaceExplorerEngine::SpaceExplorerEngine(QObject *parent)
    : QObject(parent)
    , m_isRunning(false)
    , m_wastedMoves(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SpaceExplorerEngine::executeStep);

    // Define planets
    m_planets.append({2, 2, 3, "#FF8C42"});
    m_planets.append({5, 5, 2, "#9B59B6"});
    m_planets.append({6, 1, 2, "#3498DB"});

    calculatePlanetTerritories();
    resetMission();
}

void SpaceExplorerEngine::calculatePlanetTerritories()
{
    m_planetTerritories.clear();
    m_territoryPlanetMap.clear();
    m_planetCenters.clear();

    // Sectors explicitly excluded from territory (visually confusing positions)
    QSet<QString> excluded = {"0,4", "1,4", "2,4", "4,3", "5,7", "7,5", "2,5"};

    for (int planetId = 0; planetId < m_planets.size(); planetId++) {
        const Planet &planet = m_planets[planetId];

        QString centerKey = cellKey(planet.x, planet.y);
        m_planetCenters.insert(centerKey);

        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                if (x == planet.x && y == planet.y) continue;

                QString key = cellKey(x, y);
                if (excluded.contains(key)) continue;

                double dx = x - planet.x;
                double dy = y - planet.y;
                double distance = std::sqrt(dx * dx + dy * dy);

                if (distance <= planet.radius) {
                    m_planetTerritories.insert(key);
                    m_territoryPlanetMap[key] = planetId;
                }
            }
        }
    }

    emit planetTerritoriesChanged();
}

bool SpaceExplorerEngine::isInPlanetTerritory(int x, int y) const
{
    return m_planetTerritories.contains(cellKey(x, y));
}

int SpaceExplorerEngine::getCellPlanetId(int x, int y) const
{
    QString key = cellKey(x, y);
    return m_territoryPlanetMap.value(key, -1);
}

QString SpaceExplorerEngine::normalizeCommand(const QString &cmd) const
{
    // Remove extra whitespaces and convert to uppercase
    QString normalized = cmd.trimmed().toUpper();
    normalized = normalized.simplified(); // Replace multiple spaces with single space
    return normalized;
}

bool SpaceExplorerEngine::validateCommand(const QString &cmd) const
{
    QString normalized = normalizeCommand(cmd);
    QStringList parts = normalized.split(' ', Qt::SkipEmptyParts);

    if (parts.isEmpty()) {
        return false;
    }

    QString action = parts[0];

    if (action == "SCAN" || action == "WAIT" || action == "STOP") {
        return parts.size() == 1;
    }
    else if (action == "MOVE") {
        if (parts.size() != 2) {
            return false;
        }
        QString dir = parts[1];
        return (dir == "UP" || dir == "DOWN" || dir == "RIGHT" || dir == "LEFT");
    }

    return false;
}

void SpaceExplorerEngine::resetMission()
{
    m_timer->stop();
    m_isRunning = false;

    m_internalExplorers.clear();
    m_internalExplorers.append({1, 0, 0, 1, "#FF6B6B", QStringList(), 0, true, {}});  // կարմիր
    m_internalExplorers.append({2, 7, 0, 2, "#A8FF3E", QStringList(), 0, true, {}});  // կանաչ-դեղնավուն
    m_internalExplorers.append({3, 0, 7, 1, "#FF69B4", QStringList(), 0, true, {}});  // վարդագույն

    m_scannedCells.clear();
    m_visitedCells.clear();
    m_wastedMoves = 0;
    m_missionStatus = "Պատրաստ";

    updateExplorersVariant();
    emit scannedCellsChanged();
    emit visitedCellsChanged();
    emit wastedMovesChanged();
    emit missionStatusChanged();
    emit isRunningChanged();
}

void SpaceExplorerEngine::startMission()
{
    if (m_isRunning) return;

    // Check if all explorers have no commands
    bool hasCommands = false;
    for (const auto &explorer : m_internalExplorers) {
        if (explorer.currentCommand < explorer.commands.size()) {
            hasCommands = true;
            break;
        }
    }

    if (!hasCommands) {
        emit errorOccurred("Չկան կատարման ենթակա հրամաններ։ Խնդրում ենք ավելացնել հրամաններ։");
        return;
    }

    m_isRunning = true;
    m_missionStatus = "Աշխատում է...";
    m_timer->start(500);

    emit isRunningChanged();
    emit missionStatusChanged();
}

void SpaceExplorerEngine::pauseMission()
{
    m_isRunning = false;
    m_timer->stop();
    m_missionStatus = "Դադար";

    emit isRunningChanged();
    emit missionStatusChanged();
}

void SpaceExplorerEngine::setSpeed(int milliseconds)
{
    if (m_timer->isActive()) {
        m_timer->stop();
        m_timer->start(milliseconds);
    }
}

void SpaceExplorerEngine::addCommands(int explorerId, const QStringList &commands)
{
    QStringList validCommands;

    for (const auto &cmd : qAsConst(commands)) {
        QString normalized = normalizeCommand(cmd);

        if (!validateCommand(normalized)) {
            emit errorOccurred(QString("Սխալ հրաման #%1 տիեզերագնացի համար: '%2'").arg(explorerId).arg(cmd));
            continue;
        }

        validCommands.append(normalized);
    }

    if (validCommands.isEmpty()) {
        return;
    }

    for (auto &explorer : m_internalExplorers) {
        if (explorer.id == explorerId) {
            explorer.commands.append(validCommands);
            updateExplorersVariant();
            return;
        }
    }
}

void SpaceExplorerEngine::clearCommands(int explorerId)
{
    for (auto &explorer : m_internalExplorers) {
        if (explorer.id == explorerId) {
            explorer.commands.clear();
            explorer.currentCommand = 0;
            updateExplorersVariant();
            return;
        }
    }
}

QStringList SpaceExplorerEngine::getCommands(int explorerId) const
{
    for (const auto &explorer : m_internalExplorers) {
        if (explorer.id == explorerId) {
            return explorer.commands;
        }
    }
    return QStringList();
}

QVariantList SpaceExplorerEngine::getPathHistory(int explorerId) const
{
    QVariantList result;
    for (const auto &explorer : m_internalExplorers) {
        if (explorer.id == explorerId) {
            for (const auto &point : explorer.path) {
                QVariantMap pointMap;
                pointMap["x"] = point.first;
                pointMap["y"] = point.second;
                result.append(pointMap);
            }
            break;
        }
    }
    return result;
}

QVariantList SpaceExplorerEngine::planetTerritories() const
{
    QVariantList result;
    for (const QString &key : m_planetTerritories) {
        QStringList parts = key.split(',');
        QVariantMap cell;
        cell["x"] = parts[0].toInt();
        cell["y"] = parts[1].toInt();
        cell["planetId"] = m_territoryPlanetMap.value(key, -1);
        result.append(cell);
    }
    return result;
}

QVariantList SpaceExplorerEngine::visitedCells() const
{
    QVariantList result;
    for (const QString &key : m_visitedCells) {
        QStringList parts = key.split(',');
        QVariantMap cell;
        cell["x"] = parts[0].toInt();
        cell["y"] = parts[1].toInt();
        result.append(cell);
    }
    return result;
}

void SpaceExplorerEngine::executeStep()
{
    bool allStopped = true;

    // Store current positions before any moves
    QMap<int, QPair<int, int>> nextPositions;

    // First pass: calculate where each explorer will move
    for (auto &explorer : m_internalExplorers) {
        if (explorer.active && explorer.currentCommand < explorer.commands.size()) {
            QString command = explorer.commands[explorer.currentCommand];
            QStringList parts = command.split(' ', Qt::SkipEmptyParts);
            QString action = parts[0];

            if (action == "MOVE" && parts.size() > 1) {
                QString direction = parts[1];
                int steps = explorer.speed;

                int finalX = explorer.x;
                int finalY = explorer.y;

                for (int i = 0; i < steps; i++) {
                    if (direction == "UP") finalY -= 1;
                    else if (direction == "DOWN") finalY += 1;
                    else if (direction == "RIGHT") finalX += 1;
                    else if (direction == "LEFT") finalX -= 1;
                }

                nextPositions[explorer.id] = {finalX, finalY};
            } else {
                nextPositions[explorer.id] = {explorer.x, explorer.y};
            }
        } else {
            nextPositions[explorer.id] = {explorer.x, explorer.y};
        }
    }

    // Check for collisions at final positions
    QSet<QString> finalPositionSet;
    for (auto it = nextPositions.begin(); it != nextPositions.end(); ++it) {
        QString posKey = QString("%1,%2").arg(it.value().first).arg(it.value().second);
        if (finalPositionSet.contains(posKey)) {

            // Two explorers will end up in the same position
            m_missionStatus = QString("Ուղիների հատում հայտնաբերված սեկտորում (%1, %2)!")
                                  .arg(it.value().first).arg(it.value().second);
            emit errorOccurred(m_missionStatus);
            emit missionStatusChanged();
            pauseMission();
            return;
        }
        finalPositionSet.insert(posKey);
    }

    // Second pass: execute commands
    for (auto &explorer : m_internalExplorers) {
        if (explorer.active && explorer.currentCommand < explorer.commands.size()) {
            allStopped = false;
            QString command = explorer.commands[explorer.currentCommand];

            if (!executeCommand(explorer, command)) {
                pauseMission();
                return;
            }

            explorer.currentCommand++;
        }
    }

    if (allStopped) {
        pauseMission();
        int territoryCount = m_planetTerritories.size();
        int efficiency = territoryCount > 0 ? (m_scannedCells.size() * 100 / territoryCount) : 0;

        if (m_scannedCells.size() == territoryCount) {
            m_missionStatus = QString("Հաջողություն! Բոլոր %1 սեկտորներն ուսումնասիրված են! \n               Վատնած քայլեր: %2")
                                  .arg(territoryCount).arg(m_wastedMoves);
        } else {
            m_missionStatus = QString("Ավարտված:%1/%2 Ուսումնասիրված։(%3%) Վատնած քայլեր:%4")
                                  .arg(m_scannedCells.size())
                                  .arg(territoryCount)
                                  .arg(efficiency)
                                  .arg(m_wastedMoves);
        }
        emit missionStatusChanged();
    }

    updateExplorersVariant();
}


bool SpaceExplorerEngine::executeCommand(Explorer &explorer, const QString &command)
{
    QStringList parts = command.split(' ', Qt::SkipEmptyParts);
    QString action = parts[0];

    if (action == "MOVE" && parts.size() > 1) {
        QString direction = parts[1];
        int steps = explorer.speed;

        for (int i = 0; i < steps; i++) {
            int newX = explorer.x;
            int newY = explorer.y;

            if (direction == "UP") newY = newY - 1;
            else if (direction == "DOWN") newY = newY + 1;
            else if (direction == "RIGHT") newX = newX + 1;
            else if (direction == "LEFT") newX = newX - 1;

            // Check bounds
            if (newX < 0 || newX >= GRID_SIZE || newY < 0 || newY >= GRID_SIZE) {
                QString errorMsg = QString("Տիեզերագնաց #%1: Փորձ դուրս գալ սահմաններից։ Դիրք: (%2,%3) → Ուղղություն: %4")
                                       .arg(explorer.id).arg(explorer.x).arg(explorer.y).arg(direction);
                emit errorOccurred(errorMsg);
                m_missionStatus = errorMsg;
                emit missionStatusChanged();
                return false;
            }

            // Check if trying to enter planet center
            QString newCellKey = cellKey(newX, newY);
            if (m_planetCenters.contains(newCellKey)) {
                QString errorMsg = QString("Տիեզերագնաց #%1: Չի կարող մտնել մոլորակի կենտրոն (%2, %3)!")
                                       .arg(explorer.id).arg(newX).arg(newY);
                emit errorOccurred(errorMsg);
                m_missionStatus = errorMsg;
                emit missionStatusChanged();
                return false;
            }

            // Check if visiting already scanned cell
            if (m_scannedCells.contains(newCellKey)) {
                QString errorMsg = QString("Տիեզերագնաց #%1: Մտել է արդեն ուսումնասիրված սեկտոր (%2, %3)!")
                                       .arg(explorer.id).arg(newX).arg(newY);
                emit errorOccurred(errorMsg);
                m_missionStatus = errorMsg;
                emit missionStatusChanged();
                return false;
            }

            // Track visited cells only if this is NOT explorer #2
            // Explorer #2 doesn't mark cells as visited when just passing through
            if (explorer.id != 2) {
                // Check if another explorer is currently at this position
                for (const auto &other : m_internalExplorers) {
                    if (other.id != explorer.id && other.x == newX && other.y == newY) {
                        QString errorMsg = QString("Տիեզերագնաց #%1: Ուղիների հատում սեկտորում (%2, %3)!")
                                               .arg(explorer.id).arg(newX).arg(newY);
                        emit errorOccurred(errorMsg);
                        m_missionStatus = errorMsg;
                        emit missionStatusChanged();
                        return false;
                    }
                }

                m_visitedCells.insert(newCellKey);
                emit visitedCellsChanged();
            }

            // Track if wasted move
            if (!isInPlanetTerritory(newX, newY)) {
                m_wastedMoves++;
                emit wastedMovesChanged();
            }

            explorer.x = newX;
            explorer.y = newY;
            explorer.path.append({newX, newY});
        }
    }
    else if (action == "SCAN") {
        QString key = cellKey(explorer.x, explorer.y);

        // Check if trying to scan planet center
        if (m_planetCenters.contains(key)) {
            QString errorMsg = QString("Տիեզերագնաց #%1: Հնարավոր չէ սկանավորել մոլորակի կենտրոնը (%2, %3)!")
                                   .arg(explorer.id).arg(explorer.x).arg(explorer.y);
            emit errorOccurred(errorMsg);
            m_missionStatus = errorMsg;
            emit missionStatusChanged();
            return false;
        }

        if (m_scannedCells.contains(key)) {
            QString errorMsg = QString("Տիեզերագնաց #%1:(%2, %3)սեկտորը արդեն ուսումնասիրված է!")
                                   .arg(explorer.id).arg(explorer.x).arg(explorer.y);
            emit errorOccurred(errorMsg);
            m_missionStatus = errorMsg;
            emit missionStatusChanged();
            return false;
        }

        if (!isInPlanetTerritory(explorer.x, explorer.y)) {
            QString warningMsg = QString("Տիեզերագնաց #%1: Ուսումնասիրվում է մոլորակային տիրույթից դուրս սեկտոր (%2, %3)")
                                     .arg(explorer.id).arg(explorer.x).arg(explorer.y);
            emit errorOccurred(warningMsg);
        }

        m_scannedCells.insert(key);

        // Mark as visited when scanned (applies to all explorers including #2)
        m_visitedCells.insert(key);
        emit visitedCellsChanged();
        emit scannedCellsChanged();
    }
    else if (action == "WAIT") {
        // Do nothing
    }
    else if (action == "STOP") {
        explorer.active = false;
    }

    return true;
}

bool SpaceExplorerEngine::checkPathCollision(int explorerId, int x, int y)
{
    for (const auto &explorer : m_internalExplorers) {
        if (explorer.id != explorerId) {
            for (const auto &point : explorer.path) {
                if (point.first == x && point.second == y) {
                    return true;
                }
            }
        }
    }
    return false;
}

QString SpaceExplorerEngine::cellKey(int x, int y) const
{
    return QString("%1,%2").arg(x).arg(y);
}

QVariantList SpaceExplorerEngine::scannedCells() const
{
    QVariantList result;
    for (const QString &key : m_scannedCells) {
        QStringList parts = key.split(',');
        QVariantMap cell;
        cell["x"] = parts[0].toInt();
        cell["y"] = parts[1].toInt();
        result.append(cell);
    }
    return result;
}

void SpaceExplorerEngine::updateExplorersVariant()
{
    m_explorers.clear();
    for (const auto &explorer : m_internalExplorers) {
        QVariantMap map;
        map["id"] = explorer.id;
        map["x"] = explorer.x;
        map["y"] = explorer.y;
        map["speed"] = explorer.speed;
        map["color"] = explorer.color;
        map["active"] = explorer.active;
        map["currentCommand"] = explorer.currentCommand;
        map["totalCommands"] = explorer.commands.size();
        m_explorers.append(map);
    }
    emit explorersChanged();
}
