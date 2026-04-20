#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QString>
#include <QVariantMap>

class Command : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CommandType type READ type CONSTANT)
    Q_PROPERTY(QString boatId READ boatId CONSTANT)
    Q_PROPERTY(QVariantMap params READ params CONSTANT)
    Q_PROPERTY(double executeTime READ executeTime CONSTANT)
    Q_PROPERTY(int lineNumber READ lineNumber CONSTANT)
    Q_PROPERTY(bool executed READ executed WRITE setExecuted NOTIFY executedChanged)

public:
    enum CommandType {
        SEND,
        REFUEL,
        LOAD,
        UNLOAD,
        CHECK_BRIDGE,
        REPAIR_BRIDGE,
        UNKNOWN
    };
    Q_ENUM(CommandType)

    explicit Command(QObject *parent = nullptr);
    Command(CommandType type, const QString &boatId, const QVariantMap &params,
            double executeTime, int lineNumber, QObject *parent = nullptr);

    CommandType type() const { return m_type; }
    QString boatId() const { return m_boatId; }
    QVariantMap params() const { return m_params; }
    double executeTime() const { return m_executeTime; }
    int lineNumber() const { return m_lineNumber; }
    bool executed() const { return m_executed; }

    void setExecuted(bool executed);

    Q_INVOKABLE QString toString() const;
    Q_INVOKABLE static QString typeToString(CommandType type);

signals:
    void executedChanged();

private:
    CommandType m_type;
    QString m_boatId;
    QVariantMap m_params;
    double m_executeTime;
    int m_lineNumber;
    bool m_executed;
};

#endif
