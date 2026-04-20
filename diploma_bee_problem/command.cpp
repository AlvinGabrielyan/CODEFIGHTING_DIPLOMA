#include "command.h"

Command::Command(QObject *parent)
    : QObject(parent)
    , m_type(UNKNOWN)
    , m_executeTime(0.0)
    , m_lineNumber(0)
    , m_executed(false)
{
}

Command::Command(CommandType type, const QString &beeId, const QVariantMap &params,
                 double executeTime, int lineNumber, QObject *parent)
    : QObject(parent)
    , m_type(type)
    , m_beeId(beeId)
    , m_params(params)
    , m_executeTime(executeTime)
    , m_lineNumber(lineNumber)
    , m_executed(false)
{
}

void Command::setExecuted(bool executed)
{
    if (m_executed != executed) {
        m_executed = executed;
        emit executedChanged();
    }
}

QString Command::toString() const
{
    QString result = typeToString(m_type) + "(";

    if (!m_beeId.isEmpty()) {
        result += m_beeId;
    }

    if (!m_params.isEmpty()) {
        if (!m_beeId.isEmpty()) result += ", ";
        QStringList paramStrs;
        for (auto it = m_params.begin(); it != m_params.end(); ++it) {
            paramStrs << it.key() + "=" + it.value().toString();
        }
        result += paramStrs.join(", ");
    }

    result += ")";
    return result;
}

QString Command::typeToString(CommandType type)
{
    switch (type) {
    case FLY: return "FLY";
    case COLLECT: return "COLLECT";
    case DEPOSIT: return "DEPOSIT";
    case REST: return "REST";
    case WAIT: return "WAIT";
    case CHECK_COMB: return "CHECK_COMB";
    default: return "UNKNOWN";
    }
}
