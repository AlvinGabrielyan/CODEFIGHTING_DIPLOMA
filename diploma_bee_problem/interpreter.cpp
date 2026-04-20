#include "interpreter.h"
#include <QDebug>

Interpreter::Interpreter(QObject *parent)
    : QObject(parent)
{
    initRegexPatterns();
}

void Interpreter::initRegexPatterns()
{
    // FLY(bee, from->to, time)
    m_flyRegex = QRegularExpression(
        R"(FLY\s*\(\s*(\w+)\s*,\s*(\w+)\s*(?:->|=>|→)\s*(\w+)\s*,\s*(\d+)\s*\))",
        QRegularExpression::CaseInsensitiveOption
        );

    // COLLECT(bee, nectarType, amount, time)
    m_collectRegex = QRegularExpression(
        R"(COLLECT\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*([\d.]+)\s*,\s*(\d+)\s*\))",
        QRegularExpression::CaseInsensitiveOption
        );

    // DEPOSIT(bee, combId, time)
    m_depositRegex = QRegularExpression(
        R"(DEPOSIT\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(\d+)\s*\))",
        QRegularExpression::CaseInsensitiveOption
        );

    // REST(bee, duration, time)
    m_restRegex = QRegularExpression(
        R"(REST\s*\(\s*(\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\))",
        QRegularExpression::CaseInsensitiveOption
        );

    // WAIT(duration)
    m_waitRegex = QRegularExpression(
        R"(WAIT\s*\(\s*(\d+)\s*\))",
        QRegularExpression::CaseInsensitiveOption
        );

    // CHECK_COMB(combId)
    m_checkCombRegex = QRegularExpression(
        R"(CHECK_COMB\s*\(\s*(\w+)\s*\))",
        QRegularExpression::CaseInsensitiveOption
        );
}

QList<Command*> Interpreter::parseScript(const QString &script)
{
    clearMessages();
    QList<Command*> commands;

    QStringList lines = script.split('\n', Qt::SkipEmptyParts);
    int totalLines = lines.size();

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i].trimmed();

        // Բաց թողնել դատարկ տողերը և մեկնաբանությունները
        if (line.isEmpty() || line.startsWith("//") || line.startsWith("#")) {
            continue;
        }

        Command *cmd = nullptr;

        if (m_flyRegex.match(line).hasMatch()) {
            cmd = parseFlyCommand(line, i + 1);
        }
        else if (m_collectRegex.match(line).hasMatch()) {
            cmd = parseCollectCommand(line, i + 1);
        }
        else if (m_depositRegex.match(line).hasMatch()) {
            cmd = parseDepositCommand(line, i + 1);
        }
        else if (m_restRegex.match(line).hasMatch()) {
            cmd = parseRestCommand(line, i + 1);
        }
        else if (m_waitRegex.match(line).hasMatch()) {
            cmd = parseWaitCommand(line, i + 1);
        }
        else if (m_checkCombRegex.match(line).hasMatch()) {
            cmd = parseCheckCombCommand(line, i + 1);
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

Command* Interpreter::parseFlyCommand(const QString &line, int lineNum)
{
    auto match = m_flyRegex.match(line);
    if (!match.hasMatch()) {
        addError(QString("Տող %1: Սխալ FLY հրամանի ձևաչափ '%2'").arg(lineNum).arg(line));
        return nullptr;
    }

    QString beeId = match.captured(1).toUpper();
    QString from = match.captured(2).toUpper();
    QString to = match.captured(3).toUpper();
    int time = match.captured(4).toInt();

    if (from == to) {
        addError(QString("Տող %1: Մեղուն չի կարող թռչել նույն վայր (%2->%3)")
                     .arg(lineNum).arg(from).arg(to));
        return nullptr;
    }

    QVariantMap params;
    params["from"] = from;
    params["to"] = to;
    params["startTime"] = time;

    return new Command(Command::FLY, beeId, params, time, lineNum);
}

Command* Interpreter::parseCollectCommand(const QString &line, int lineNum)
{
    auto match = m_collectRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString beeId = match.captured(1).toUpper();
    QString nectarType = match.captured(2).toUpper();
    double amount = match.captured(3).toDouble();
    int executeTime = match.captured(4).toInt();

    if (amount <= 0) {
        addError(QString("Տող %1: Նեկտարի քանակը պետք է դրական լինի").arg(lineNum));
        return nullptr;
    }

    QVariantMap params;
    params["nectarType"] = nectarType;
    params["amount"] = amount;

    return new Command(Command::COLLECT, beeId, params, executeTime, lineNum);
}

Command* Interpreter::parseDepositCommand(const QString &line, int lineNum)
{
    auto match = m_depositRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString beeId = match.captured(1).toUpper();
    QString combId = match.captured(2).toUpper();
    int executeTime = match.captured(3).toInt();

    QVariantMap params;
    params["combId"] = combId;

    return new Command(Command::DEPOSIT, beeId, params, executeTime, lineNum);
}

Command* Interpreter::parseRestCommand(const QString &line, int lineNum)
{
    auto match = m_restRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString beeId = match.captured(1).toUpper();
    int duration = match.captured(2).toInt();
    int executeTime = match.captured(3).toInt();

    QVariantMap params;
    params["duration"] = duration;

    return new Command(Command::REST, beeId, params, executeTime, lineNum);
}

Command* Interpreter::parseWaitCommand(const QString &line, int lineNum)
{
    auto match = m_waitRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    int duration = match.captured(1).toInt();

    QVariantMap params;
    params["duration"] = duration;

    return new Command(Command::WAIT, "", params, 0, lineNum);
}

Command* Interpreter::parseCheckCombCommand(const QString &line, int lineNum)
{
    auto match = m_checkCombRegex.match(line);
    if (!match.hasMatch()) return nullptr;

    QString combId = match.captured(1).toUpper();

    QVariantMap params;
    params["combId"] = combId;

    return new Command(Command::CHECK_COMB, "", params, 0, lineNum);
}

bool Interpreter::validateCommand(Command *cmd, const QStringList &availableBees)
{
    if (!cmd) return false;

    if (!cmd->beeId().isEmpty() && !availableBees.contains(cmd->beeId())) {
        addError(QString("Տող %1: Մեղու '%2' չի գտնվել")
                     .arg(cmd->lineNumber()).arg(cmd->beeId()));
        return false;
    }

    return true;
}

bool Interpreter::validateAllCommands(const QList<Command*> &commands)
{
    // Ստուգել ժամանակների հերթականությունը յուրաքանչյուր մեղվի համար
    QMap<QString, double> lastBeeTime;  // Մեղվի վերջին հրամանի ժամանակ

    for (Command *cmd : commands) {
        QString beeId = cmd->beeId();

        // Բաց թողնել հրամանները առանց մեղվի
        if (beeId.isEmpty()) continue;

        if (cmd->type() == Command::FLY || cmd->type() == Command::COLLECT ||
            cmd->type() == Command::DEPOSIT || cmd->type() == Command::REST) {

            double time = cmd->executeTime();

            // Ստուգել արդյոք այս մեղուն արդեն ունի հրաման
            if (lastBeeTime.contains(beeId)) {
                double prevTime = lastBeeTime[beeId];

                // Եթե ժամանակը անցել է
                if (time < prevTime) {
                    addError(QString("Տող %1: %2-ի հրամանի ժամանակը (%3վրկ) անցել է նախորդ հրամանից (%4վրկ). "
                                     "Հրամանները պետք է լինեն ժամանակագրական հերթականությամբ")
                                 .arg(cmd->lineNumber())
                                 .arg(beeId)
                                 .arg(time)
                                 .arg(prevTime));
                }
                // Եթե ժամանակը շատ մոտ է (փոքր է քան 2 վրկ)
                else if (time - prevTime < 2.0 && time != prevTime) {
                    addWarning(QString("Տող %1: %2-ի հրամանները շատ մոտ են (%3վրկ տարբերություն). "
                                       "Նախորդ հրամանը կարող է դեռ ավարտված չլինել")
                                   .arg(cmd->lineNumber())
                                   .arg(beeId)
                                   .arg(time - prevTime));
                }
            }

            lastBeeTime[beeId] = time;
        }
    }

    // Ստուգել տարբեր մեղուների միաժամանակյա հրամանները
    QMap<double, QStringList> timeMap;

    for (Command *cmd : commands) {
        if (cmd->type() == Command::FLY || cmd->type() == Command::COLLECT ||
            cmd->type() == Command::DEPOSIT) {
            double time = cmd->executeTime();
            QString beeId = cmd->beeId();

            if (!beeId.isEmpty()) {
                timeMap[time].append(QString("%1 (տող %2)").arg(beeId).arg(cmd->lineNumber()));
            }
        }
    }

    // Տեղեկացնել միաժամանակյա հրամանների մասին
    for (auto it = timeMap.begin(); it != timeMap.end(); ++it) {
        if (it.value().size() > 1) {
            addWarning(QString("Ժամանակ %1վրկ: Միաժամանակ աշխատում են՝ %2")
                           .arg(it.key())
                           .arg(it.value().join(", ")));
        }
    }

    return m_errors.isEmpty();
}

double Interpreter::estimateTotalTime(const QList<Command*> &commands)
{
    double maxTime = 0.0;

    for (Command *cmd : commands) {
        if (cmd->type() == Command::FLY) {
            // FLY-ը տևում է ~3-5 վրկ
            double endTime = cmd->executeTime() + 5;
            maxTime = qMax(maxTime, endTime);
        }
        else if (cmd->type() == Command::COLLECT) {
            double endTime = cmd->executeTime() + 2;
            maxTime = qMax(maxTime, endTime);
        }
        else if (cmd->type() == Command::DEPOSIT) {
            double endTime = cmd->executeTime() + 1.5;
            maxTime = qMax(maxTime, endTime);
        }
    }

    return maxTime;
}

QStringList Interpreter::suggestOptimizations(const QList<Command*> &commands)
{
    QStringList suggestions;

    int flyCount = 0;
    for (Command *cmd : commands) {
        if (cmd->type() == Command::FLY) flyCount++;
    }

    if (flyCount > 10) {
        suggestions << "💡 Շատ թռիչքներ կան - փորձեք զուգահեռացնել մեղուներին";
    }

    suggestions << "💡 Օգտագործեք REST հրամանը էներգիա վերականգնելու համար";
    suggestions << "💡 CHECK_COMB-ը օգնում է ստուգել ամանի վիճակը";

    return suggestions;
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
