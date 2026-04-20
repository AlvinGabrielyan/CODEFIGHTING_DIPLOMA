#ifndef BEESIMULATOR_H
#define BEESIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QVariantMap>
#include "bee.h"
#include "command.h"
#include "interpreter.h"

class BeeSimulator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int completedTrips READ completedTrips NOTIFY completedTripsChanged)
    Q_PROPERTY(int totalBees READ totalBees NOTIFY totalBeesChanged)
    Q_PROPERTY(double totalNectarCollected READ totalNectarCollected NOTIFY totalNectarCollectedChanged)

public:
    // Թռիչքի տեղեկատվություն
    struct FlightInfo {
        QString beeId;
        double startTime;
        double endTime;
        QString from;
        QString to;
        Command *pendingCommand;  // Հրաման որը պետք է կատարվի հասնելուց հետո
    };

    // Ամանի տեղեկատվություն
    struct HoneycombInfo {
        QString id;
        double capacity;
        double currentAmount;
        bool sealed;
    };

    explicit BeeSimulator(QObject *parent = nullptr);
    ~BeeSimulator();

    double currentTime() const { return m_currentTime; }
    bool isRunning() const { return m_isRunning; }
    double speed() const { return m_speed; }
    int completedTrips() const { return m_completedTrips; }
    int totalBees() const { return m_bees.size(); }
    double totalNectarCollected() const { return m_totalNectarCollected; }

    void setSpeed(double speed);

    Q_INVOKABLE void addBee(Bee *bee);
    Q_INVOKABLE Bee* getBee(const QString &id);
    Q_INVOKABLE QList<QObject*> getAllBees();
    Q_INVOKABLE bool loadScript(const QString &script);
    Q_INVOKABLE QStringList getErrors() const;
    Q_INVOKABLE QStringList getWarnings() const;
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void step();

    Q_INVOKABLE void addHoneycomb(const QString &id, double capacity);
    Q_INVOKABLE HoneycombInfo getHoneycombInfo(const QString &id);
    Q_INVOKABLE QStringList getAllHoneycombIds();
    Q_INVOKABLE const QList<Command*>& getCommands() const { return m_commands; }

signals:
    void currentTimeChanged();
    void isRunningChanged();
    void speedChanged();
    void completedTripsChanged();
    void totalBeesChanged();
    void totalNectarCollectedChanged();
    void logMessage(const QString &message, const QString &type);
    void commandExecuted(Command *command, bool success);
    void beeStartedFlying(Bee *bee);
    void beeFinishedFlying(Bee *bee);
    void simulationFinished();

private slots:
    void update();

private:
    QTimer *m_timer;
    Interpreter *m_interpreter;
    QList<Bee*> m_bees;
    QList<Command*> m_commands;
    QMap<QString, Bee*> m_beeMap;
    QMap<QString, FlightInfo> m_activeFlights;
    QMap<QString, HoneycombInfo> m_honeycombs;

    double m_currentTime;
    bool m_isRunning;
    double m_speed;
    int m_completedTrips;
    double m_totalNectarCollected;
    int m_currentCommandIndex;

    void executeCommand(Command *cmd);
    void updateBeePositions();

    // Հրամանների կատարում
    void executeFly(Command *cmd);
    void executeCollect(Command *cmd);
    void executeDeposit(Command *cmd);
    void executeRest(Command *cmd);
    void executeCheckComb(Command *cmd);
    void executeEmergency(Command *cmd);

    bool checkBeeAvailable(const QString &beeId);
};

#endif // BEESIMULATOR_H
