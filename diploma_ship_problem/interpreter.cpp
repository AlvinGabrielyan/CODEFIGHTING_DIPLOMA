#include "interpreter.h"
#include <QDebug>

Interpreter::Interpreter(QObject *parent)
    : QObject(parent)
{
    initRegexPatterns();
}

void Interpreter::initRegexPatterns()
{
    m_sendRegex = QRegularExpression(R"(SEND\s*\(\s*(\w+)\s*,\s*([AB])\s*(?:->|=>)\s*([AB])\s*,\s*(\d+)\s*\))",
                                     QRegularExpression::CaseInsensitiveOption);
    // REFUEL(boat, amount, time)
    m_refuelRegex = QRegularExpression(R"(REFUEL\s*\(\s*(\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))",
                                       QRegularExpression::CaseInsensitiveOption);
    // LOAD(boat, cargoType, weight, time)
    m_loadRegex = QRegularExpression(R"(LOAD\s*\(\s*(\w+)\s*,\s*([A-Za-z0-9_]+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))",
                                     QRegularExpression::CaseInsensitiveOption);
    // UNLOAD(boat, time)
    m_unloadRegex = QRegularExpression(R"(UNLOAD\s*\(\s*(\w+)\s*,\s*(\d+)\s*\))",
                                       QRegularExpression::CaseInsensitiveOption);
    m_checkBridgeRegex = QRegularExpression(R"(CHECK_BRIDGE\s*\(\s*\))",
                                            QRegularExpression::CaseInsensitiveOption);
    m_repairBridgeRegex = QRegularExpression(R"(REPAIR_BRIDGE\s*\(\s*(\d+)\s*\))",
                                             QRegularExpression::CaseInsensitiveOption);
}

QList<Command*> Interpreter::parseScript(const QString &script)
{
    clearMessages();
    QList<Command*> commands;

    QStringList lines = script.split('\n', Qt::SkipEmptyParts);
    int totalLines = lines.size();

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();

        if (line.isEmpty() || line.startsWith("//") || line.startsWith("#")) {
            continue;
        }

        Command *cmd = nullptr;

        if (m_sendRegex.match(line).hasMatch()) {
            cmd = parseSendCommand(line, i + 1);
        }
        else if (m_refuelRegex.match(line).hasMatch()) {
            cmd = parseRefuelCommand(line, i + 1);
        }
        else if (m_loadRegex.match(line).hasMatch()) {
            cmd = parseLoadCommand(line, i + 1);
        }
        else if (m_unloadRegex.match(line).hasMatch()) {
            cmd = parseUnloadCommand(line, i + 1);
        }
        else if (m_checkBridgeRegex.match(line).hasMatch()) {
            cmd = parseCheckBridgeCommand(line, i + 1);
        }
        else if (m_repairBridgeRegex.match(line).hasMatch()) {
            cmd = parseRepairBridgeCommand(line, i + 1);
        }
        else {
            addError(QString("Տող %1: Անհայտ հրաման '%2'").arg(i + 1).arg(line));
        }

        if (cmd) {
            commands.append(cmd);
        }

        emit parsingProgress((i + 1) * 100 / totalLines);
    }

    return commands;
}

Command* Interpreter::parseSendCommand(const QString &line, int lineNum)
{
    auto match = m_sendRegex.match(line);
    if (!match.hasMatch()) {
        addError(QString("Տող %1: Սխալ SEND հրամանի ձևաչափ '%2'").arg(lineNum).arg(line));
        return nullptr;
    }

    QString boatId = match.captured(1).toUpper();
    QString from = match.captured(2).toUpper();
    QString to = match.captured(3).toUpper();
    int time = match.captured(4).toInt();

    if (from == to) {
        addError(QString("Տող %1: Նավակը չի կարող անցնել նույն ափ (%2->%3)")
                     .arg(lineNum).arg(from).arg(to));
        return nullptr;
    }

    QVariantMap params;
    params["from"] = from;
    params["to"] = to;
    params["startTime"] = time;

    return new Command(Command::SEND, boatId, params, time, lineNum);
}

Command* Interpreter::parseRefuelCommand(const QString &line, int lineNum)
{
    auto match = m_refuelRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString boatId = match.captured(1).toUpper();
    int amount = match.captured(2).toInt();
    int executeTime = match.captured(3).toInt();

    if (amount < 0 || amount > 100) {
        addWarning(QString("Տող %1: Վառելիքի քանակը պետք է լինի 0-100 միջակայքում").arg(lineNum));
        amount = qBound(0, amount, 100);
    }

    QVariantMap params;
    params["amount"] = amount;

    return new Command(Command::REFUEL, boatId, params, executeTime, lineNum);
}

Command* Interpreter::parseLoadCommand(const QString &line, int lineNum)
{
    auto match = m_loadRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString boatId = match.captured(1).toUpper();
    QString cargoType = match.captured(2).toUpper();
    int weight = match.captured(3).toInt();
    int executeTime = match.captured(4).toInt();

    QStringList validTypes = {
        "FOOD_DRY", "FOOD_FRESH", "WATER",
        "MED_ANTIBIOTICS", "MED_PAINKILLERS", "MED_BANDAGES", "MED_FIRSTAID",
        "EQUIPMENT", "FUEL", "BUILDING_MATERIALS"
    };

    if (!validTypes.contains(cargoType)) {
        addWarning(QString("Տող %1: Անհայտ բեռի տիպ '%2' (կօգտագործվի այնուամենայնիվ)")
                       .arg(lineNum).arg(cargoType));
    }

    QVariantMap params;
    params["cargoType"] = cargoType;
    params["weight"] = weight;

    return new Command(Command::LOAD, boatId, params, executeTime, lineNum);
}

Command* Interpreter::parseUnloadCommand(const QString &line, int lineNum)
{
    auto match = m_unloadRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString boatId = match.captured(1).toUpper();
    int executeTime = match.captured(2).toInt();
    return new Command(Command::UNLOAD, boatId, QVariantMap(), executeTime, lineNum);
}

Command* Interpreter::parseCheckBridgeCommand(const QString &line, int lineNum)
{
    return new Command(Command::CHECK_BRIDGE, "", QVariantMap(), 0, lineNum);
}

Command* Interpreter::parseRepairBridgeCommand(const QString &line, int lineNum)
{
    auto match = m_repairBridgeRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    int duration = match.captured(1).toInt();

    QVariantMap params;
    params["duration"] = duration;

    return new Command(Command::REPAIR_BRIDGE, "", params, 0, lineNum);
}

bool Interpreter::validateCommand(Command *cmd, const QStringList &availableBoats)
{
    if (!cmd) return false;

    if (!cmd->boatId().isEmpty() && !availableBoats.contains(cmd->boatId())) {
        addError(QString("Տող %1: Նավակը '%2' չի գտնվել")
                     .arg(cmd->lineNumber()).arg(cmd->boatId()));
        return false;
    }

    return true;
}

bool Interpreter::validateAllCommands(const QList<Command*> &commands)
{
    QMap<double, Command*> timeMap;

    for (Command *cmd : commands) {
        if (cmd->type() == Command::SEND) {
            double time = cmd->executeTime();
            if (timeMap.contains(time)) {
                addError(QString("Բախում: Երկու հրաման նույն ժամանակին (տող %1 և %2)")
                             .arg(timeMap[time]->lineNumber())
                             .arg(cmd->lineNumber()));
                return false;
            }
            timeMap[time] = cmd;
        }
    }

    return m_errors.isEmpty();
}

double Interpreter::estimateTotalTime(const QList<Command*> &commands)
{
    double maxTime = 0.0;

    for (Command *cmd : commands) {
        if (cmd->type() == Command::SEND) {
            double endTime = cmd->executeTime() + 10;
            maxTime = qMax(maxTime, endTime);
        }
    }

    return maxTime;
}


void Interpreter::addError(const QString &error)
{
    m_errors.append(error);
    emit errorsChanged();
}

void Interpreter::addWarning(const QString &warning)
{
    m_warnings.append(warning);
    emit warningsChanged();
}

void Interpreter::clearMessages()
{
    m_errors.clear();
    m_warnings.clear();
    emit errorsChanged();
    emit warningsChanged();
}
