#ifndef BOAT_H
#define BOAT_H

#include <QObject>
#include <QString>
#include <QColor>

class Boat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString side READ side WRITE setSide NOTIFY sideChanged)
    Q_PROPERTY(QString targetSide READ targetSide WRITE setTargetSide NOTIFY targetSideChanged)
    Q_PROPERTY(int minSpeed READ minSpeed CONSTANT)
    Q_PROPERTY(int maxSpeed READ maxSpeed CONSTANT)
    Q_PROPERTY(int capacity READ capacity CONSTANT)
    Q_PROPERTY(int fuel READ fuel WRITE setFuel NOTIFY fuelChanged)
    Q_PROPERTY(QColor color READ color CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString cargoType READ cargoType WRITE setCargoType NOTIFY cargoTypeChanged)
    Q_PROPERTY(int cargoWeight READ cargoWeight WRITE setCargoWeight NOTIFY cargoWeightChanged)
    Q_PROPERTY(bool isMoving READ isMoving WRITE setIsMoving NOTIFY isMovingChanged)
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(double positionX READ positionX WRITE setPositionX NOTIFY positionXChanged)

public:
    explicit Boat(QObject *parent = nullptr);

    Boat(const QString &id, const QString &name, const QString &side,
         int minSpeed, int maxSpeed, int capacity, const QColor &color,
         const QString &description = QString(), QObject *parent = nullptr);

    QString id() const { return m_id; }
    QString name() const { return m_name; }
    QString side() const { return m_side; }
    QString initialSide() const { return m_initialSide; }
    QString targetSide() const { return m_targetSide; }
    int minSpeed() const { return m_minSpeed; }
    int maxSpeed() const { return m_maxSpeed; }
    int capacity() const { return m_capacity; }
    int fuel() const { return m_fuel; }
    QColor color() const { return m_color; }
    QString description() const { return m_description; }
    QString cargoType() const { return m_cargoType; }
    int cargoWeight() const { return m_cargoWeight; }
    bool isMoving() const { return m_isMoving; }
    double progress() const { return m_progress; }
    double positionX() const { return m_positionX; }
    QStringList allowedCargoTypes() const { return m_allowedCargoTypes; }

    void setSide(const QString &side);
    void setTargetSide(const QString &targetSide);
    void setFuel(int fuel);
    void setCargoType(const QString &type);
    void setCargoWeight(int weight);
    void setIsMoving(bool moving);
    void setProgress(double progress);
    void setPositionX(double x);
    void setAllowedCargoTypes(const QStringList &types);

    Q_INVOKABLE bool canMove() const;
    Q_INVOKABLE int calculateCrossingTime() const;
    Q_INVOKABLE void consumeFuel(int amount);
    Q_INVOKABLE bool loadCargo(const QString &type, int weight);
    Q_INVOKABLE void unloadCargo();
    Q_INVOKABLE bool canCarry(const QString &cargoType) const;

signals:
    void sideChanged();
    void targetSideChanged();
    void fuelChanged();
    void cargoTypeChanged();
    void cargoWeightChanged();
    void isMovingChanged();
    void progressChanged();
    void positionXChanged();

private:
    QString m_id;
    QString m_name;
    QString m_side;
    QString m_initialSide;
    QString m_targetSide;
    int m_minSpeed;
    int m_maxSpeed;
    int m_capacity;
    int m_fuel;
    QColor m_color;
    QString m_description;
    QString m_cargoType;
    int m_cargoWeight;
    bool m_isMoving;
    double m_progress;
    double m_positionX;
    QStringList m_allowedCargoTypes;
};

#endif
