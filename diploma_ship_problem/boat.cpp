#include "boat.h"
#include <QtMath>

Boat::Boat(QObject *parent): QObject(parent),
    m_fuel(100),
    m_cargoWeight(0),
    m_isMoving(false),
    m_progress(0.0),
    m_positionX(0.0)
{
}

Boat::Boat(const QString &id, const QString &name, const QString &side,
           int minSpeed, int maxSpeed, int capacity, const QColor &color,
           const QString &description, QObject *parent)
    : QObject(parent),
    m_id(id),
    m_name(name),
    m_side(side),
    m_initialSide(side),
    m_minSpeed(minSpeed),
    m_maxSpeed(maxSpeed),
    m_capacity(capacity),
    m_fuel(100),
    m_color(color),
    m_description(description),
    m_cargoWeight(0),
    m_isMoving(false),
    m_progress(0.0),
    m_positionX(side == "A" ? 0.0 : 1.0)
{
}

void Boat::setSide(const QString &side)
{
    if (m_side != side) {
        m_side = side;
        emit sideChanged();
    }
}

void Boat::setTargetSide(const QString &targetSide)
{
    if (m_targetSide != targetSide) {
        m_targetSide = targetSide;
        emit targetSideChanged();
    }
}

void Boat::setFuel(int fuel)
{
    if (m_fuel != fuel) {
        m_fuel = qBound(0, fuel, 100);
        emit fuelChanged();
    }
}

void Boat::setCargoType(const QString &type)
{
    if (m_cargoType != type) {
        m_cargoType = type;
        emit cargoTypeChanged();
    }
}

void Boat::setCargoWeight(int weight)
{
    if (m_cargoWeight != weight) {
        m_cargoWeight = weight;
        emit cargoWeightChanged();
    }
}

void Boat::setIsMoving(bool moving)
{
    if (m_isMoving != moving) {
        m_isMoving = moving;
        emit isMovingChanged();
    }
}

void Boat::setProgress(double progress)
{
    if (!qFuzzyCompare(m_progress, progress)) {
        m_progress = qBound(0.0, progress, 1.0);
        emit progressChanged();
    }
}

void Boat::setPositionX(double x)
{
    if (!qFuzzyCompare(m_positionX, x)) {
        m_positionX = qBound(0.0, x, 1.0);
        emit positionXChanged();
    }
}

bool Boat::canMove() const
{
    return m_fuel >= 10 && !m_isMoving;
}

int Boat::calculateCrossingTime() const
{
    // Calculate cargo load factor (0.0 = empty, 1.0 = full)
    double cargoLoadFactor = 0.0;
    if (m_capacity > 0 && m_cargoWeight > 0) {
        cargoLoadFactor = static_cast<double>(m_cargoWeight) / m_capacity;
        cargoLoadFactor = qBound(0.0, cargoLoadFactor, 1.0);
    }

    // Calculate fuel factor (1.0 = normal, up to 1.3 = low fuel)
    double fuelFactor = 1.0;
    if (m_fuel < 30) {
        // Linear interpolation: fuel 30 -> factor 1.0, fuel 0 -> factor 1.3
        fuelFactor = 1.0 + (30.0 - m_fuel) / 30.0 * 0.3;
    }

    // Interpolate between minSpeed and maxSpeed based on cargo load
    double baseTime = m_minSpeed + (m_maxSpeed - m_minSpeed) * cargoLoadFactor;

    // Apply fuel penalty
    int finalTime = static_cast<int>(baseTime * fuelFactor);

    return finalTime;
}

void Boat::consumeFuel(int amount)
{
    setFuel(m_fuel - amount);
}

bool Boat::loadCargo(const QString &type, int weight)
{
    if (weight <= m_capacity && m_cargoWeight == 0) {
        setCargoType(type);
        setCargoWeight(weight);
        return true;
    }
    return false;
}

void Boat::unloadCargo()
{
    setCargoType("");
    setCargoWeight(0);
}

void Boat::setAllowedCargoTypes(const QStringList &types)
{
    m_allowedCargoTypes = types;
}

bool Boat::canCarry(const QString &cargoType) const
{
    if (m_allowedCargoTypes.isEmpty()) {
        return true;  // Եթե սահմանված չեն, կարող է տանել ցանկացած բեռ
    }

    for (const QString &allowed : m_allowedCargoTypes) {
        if (allowed.compare(cargoType, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}
