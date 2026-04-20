#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QVariantMap>
#include "boat.h"
#include "command.h"
#include "interpreter.h"

class Simulator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int bridgeHealth READ bridgeHealth NOTIFY bridgeHealthChanged)
    Q_PROPERTY(bool bridgeOccupied READ bridgeOccupied NOTIFY bridgeOccupiedChanged)
    Q_PROPERTY(int completedBoats READ completedBoats NOTIFY completedBoatsChanged)
    Q_PROPERTY(int totalBoats READ totalBoats NOTIFY totalBoatsChanged)
    Q_PROPERTY(int collisions READ collisions NOTIFY collisionsChanged)
    Q_PROPERTY(int totalScore READ totalScore NOTIFY totalScoreChanged)
    Q_PROPERTY(int stars READ stars NOTIFY starsChanged)

public:
    struct CargoDelivery {
        QString cargoType;
        int delivered;
        int required;
        QString destination;
    };

    explicit Simulator(QObject *parent = nullptr);
    ~Simulator();

    double currentTime() const { return m_currentTime; }
    bool isRunning() const { return m_isRunning; }
    double speed() const { return m_speed; }
    int bridgeHealth() const { return m_bridgeHealth; }
    bool bridgeOccupied() const { return m_bridgeOccupied; }
    int completedBoats() const { return m_completedBoats; }
    int totalBoats() const { return m_boats.size(); }
    int collisions() const { return m_collisions; }
    int totalScore() const { return m_totalScore; }
    int stars() const { return m_stars; }

    void setSpeed(double speed);

    Q_INVOKABLE void addBoat(Boat *boat);
    Q_INVOKABLE Boat* getBoat(const QString &id);
    Q_INVOKABLE QList<QObject*> getAllBoats();
    Q_INVOKABLE bool loadScript(const QString &script);
    Q_INVOKABLE QStringList getErrors() const;
    Q_INVOKABLE QStringList getWarnings() const;
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void step();

signals:
    void currentTimeChanged();
    void isRunningChanged();
    void speedChanged();
    void bridgeHealthChanged();
    void bridgeOccupiedChanged();
    void completedBoatsChanged();
    void totalBoatsChanged();
    void collisionsChanged();
    void totalScoreChanged();
    void starsChanged();
    void logMessage(const QString &message, const QString &type);
    void commandExecuted(Command *command, bool success);
    void boatStartedCrossing(Boat *boat);
    void boatFinishedCrossing(Boat *boat);
    void collisionDetected(const QString &boat1, const QString &boat2);
    void simulationFinished();

private slots:
    void update();

private:
    struct BridgeOccupancy {
        QString boatId;
        double startTime;
        double endTime;
        QString direction;
        QString sourceSide;
        QString targetSide;
    };

    QTimer *m_timer;
    Interpreter *m_interpreter;
    QList<Boat*> m_boats;
    QList<Command*> m_commands;
    QMap<QString, Boat*> m_boatMap;
    QList<BridgeOccupancy> m_bridgeSchedule;
    QMap<QString, BridgeOccupancy> m_activeBoats;
    BridgeOccupancy m_currentOccupancy;

    QMap<QString, CargoDelivery> m_deliveries;
    QMap<QString, int> m_boatMovements;
    int m_totalScore;
    int m_stars;

    double m_currentTime;
    bool m_isRunning;
    double m_speed;
    int m_bridgeHealth;
    bool m_bridgeOccupied;
    int m_completedBoats;
    int m_collisions;
    int m_currentCommandIndex;

    bool checkCollision(const QString &boatId, double startTime, double duration);
    void executeCommand(Command *cmd);
    void updateBoatPositions();
    void checkBridgeStatus();
    void executeSend(Command *cmd);
    void executeWait(Command *cmd);
    void executeRefuel(Command *cmd);
    void executeLoad(Command *cmd);
    void executeUnload(Command *cmd);
    void executeCheckBridge(Command *cmd);
    void executeRepairBridge(Command *cmd);

    void initializeRequirements();
    int getRequiredAmount(const QString &side, const QString &cargoType);
    bool checkAllBoatsCompleted();
    void calculateFinalScore();
    int calculateStars();
    QString getStarRating(int stars);
    double calculateAverageFuel();
};

#endif
