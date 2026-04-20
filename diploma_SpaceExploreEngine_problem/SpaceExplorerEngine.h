#ifndef SPACEEXPLORERENGINE_H
#define SPACEEXPLORERENGINE_H

#include <QObject>
#include <QTimer>
#include <QVariantList>
#include <QVariantMap>
#include <QSet>
#include <QString>
#include <QPair>
#include <QMap>

class SpaceExplorerEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList explorers READ explorers NOTIFY explorersChanged)
    Q_PROPERTY(QVariantList scannedCells READ scannedCells NOTIFY scannedCellsChanged)
    Q_PROPERTY(QVariantList visitedCells READ visitedCells NOTIFY visitedCellsChanged)
    Q_PROPERTY(QVariantList planetTerritories READ planetTerritories NOTIFY planetTerritoriesChanged)
    Q_PROPERTY(QString missionStatus READ missionStatus NOTIFY missionStatusChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(int gridSize READ gridSize CONSTANT)
    Q_PROPERTY(int scannedCount READ scannedCount NOTIFY scannedCellsChanged)
    Q_PROPERTY(int totalTerritories READ totalTerritories CONSTANT)
    Q_PROPERTY(int wastedMoves READ wastedMoves NOTIFY wastedMovesChanged)

public:
    explicit SpaceExplorerEngine(QObject *parent = nullptr);

    QVariantList explorers() const { return m_explorers; }
    QVariantList scannedCells() const;
    QVariantList visitedCells() const;
    QVariantList planetTerritories() const;
    QString missionStatus() const { return m_missionStatus; }
    bool isRunning() const { return m_isRunning; }
    int gridSize() const { return GRID_SIZE; }
    int scannedCount() const { return m_scannedCells.size(); }
    int totalTerritories() const { return m_planetTerritories.size(); }
    int wastedMoves() const { return m_wastedMoves; }

    Q_INVOKABLE void resetMission();
    Q_INVOKABLE void startMission();
    Q_INVOKABLE void pauseMission();
    Q_INVOKABLE void addCommands(int explorerId, const QStringList &commands);
    Q_INVOKABLE void clearCommands(int explorerId);
    Q_INVOKABLE QStringList getCommands(int explorerId) const;
    Q_INVOKABLE void setSpeed(int milliseconds);
    Q_INVOKABLE QVariantList getPathHistory(int explorerId) const;

signals:
    void explorersChanged();
    void scannedCellsChanged();
    void visitedCellsChanged();
    void planetTerritoriesChanged();
    void missionStatusChanged();
    void isRunningChanged();
    void wastedMovesChanged();
    void errorOccurred(const QString &error);

private slots:
    void executeStep();

private:
    struct Explorer {
        int id;
        int x;
        int y;
        int speed;
        QString color;
        QStringList commands;
        int currentCommand;
        bool active;
        QList<QPair<int, int>> path;
    };

    struct Planet {
        int x, y, radius;
        QString color;
    };

    bool executeCommand(Explorer &explorer, const QString &command);
    QString cellKey(int x, int y) const;
    bool checkPathCollision(int explorerId, int x, int y);
    void updateExplorersVariant();
    void calculatePlanetTerritories();
    bool isInPlanetTerritory(int x, int y) const;
    int getCellPlanetId(int x, int y) const;
    QString normalizeCommand(const QString &cmd) const;
    bool validateCommand(const QString &cmd) const;

    static const int GRID_SIZE = 8;
    QList<Explorer> m_internalExplorers;
    QList<Planet> m_planets;
    QVariantList m_explorers;
    QSet<QString> m_scannedCells;
    QSet<QString> m_visitedCells;
    QSet<QString> m_planetTerritories;
    QSet<QString> m_planetCenters;
    QMap<QString, int> m_territoryPlanetMap;
    QString m_missionStatus;
    bool m_isRunning;
    int m_wastedMoves;
    QTimer *m_timer;
};

#endif
