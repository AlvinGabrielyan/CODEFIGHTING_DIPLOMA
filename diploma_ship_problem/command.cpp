#include "command.h"

Command::Command(QObject *parent)
    : QObject(parent),
    m_type(UNKNOWN),
    m_executeTime(0.0),
    m_lineNumber(0),
    m_executed(false)
{
}

Command::Command(CommandType type, const QString &boatId, const QVariantMap &params,
                 double executeTime, int lineNumber, QObject *parent)
    : QObject(parent),
    m_type(type),
    m_boatId(boatId),
    m_params(params),
    m_executeTime(executeTime),
    m_lineNumber(lineNumber),
    m_executed(false)
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

    if (!m_boatId.isEmpty()) {
        result += m_boatId;
    }

    if (!m_params.isEmpty()) {
        if (!m_boatId.isEmpty()) result += ", ";
        QStringList paramStrs;
        for (auto it = m_params.begin(); it != m_params.end(); ++it) {
            paramStrs << it.value().toString();
        }
        result += paramStrs.join(", ");
    }

    result += ")";
    return result;
}

QString Command::typeToString(CommandType type)
{
    switch (type) {
    case SEND: return "SEND";
    case REFUEL: return "REFUEL";
    case LOAD: return "LOAD";
    case UNLOAD: return "UNLOAD";
    case CHECK_BRIDGE: return "CHECK_BRIDGE";
    case REPAIR_BRIDGE: return "REPAIR_BRIDGE";
    default: return "UNKNOWN";
    }
}
