#include "bee.h"
#include <QtMath>

Bee::Bee(QObject *parent)
    : QObject(parent)
    , m_energy(100)
    , m_currentNectar(0.0)
    , m_isFlying(false)
    , m_progress(0.0)
    , m_positionX(0.0)
    , m_positionY(0.0)
{
}

Bee::Bee(const QString &id, const QString &name, const QString &location,
         double capacity, int minTime, int maxTime, const QColor &color,
         const QString &description, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
    , m_location(location)
    , m_initialLocation(location)
    , m_energy(100)
    , m_capacity(capacity)
    , m_currentNectar(0.0)
    , m_isFlying(false)
    , m_progress(0.0)
    , m_positionX(location == "HIVE" ? 0.1 : 0.5)
    , m_positionY(location == "HIVE" ? 0.8 : 0.2)
    , m_color(color)
    , m_description(description)
    , m_minTime(minTime)
    , m_maxTime(maxTime)
{
}

void Bee::setLocation(const QString &location)
{
    if (m_location != location) {
        m_location = location;
        emit locationChanged();
    }
}

void Bee::setTargetLocation(const QString &targetLocation)
{
    if (m_targetLocation != targetLocation) {
        m_targetLocation = targetLocation;
        emit targetLocationChanged();
    }
}

void Bee::setEnergy(int energy)
{
    if (m_energy != energy) {
        m_energy = qBound(0, energy, 100);
        emit energyChanged();
    }
}

void Bee::setCurrentNectar(double nectar)
{
    if (!qFuzzyCompare(m_currentNectar, nectar)) {
        m_currentNectar = qBound(0.0, nectar, m_capacity);
        emit currentNectarChanged();
    }
}

void Bee::setNectarType(const QString &type)
{
    if (m_nectarType != type) {
        m_nectarType = type;
        emit nectarTypeChanged();
    }
}

void Bee::setIsFlying(bool flying)
{
    if (m_isFlying != flying) {
        m_isFlying = flying;
        emit isFlyingChanged();
    }
}

void Bee::setProgress(double progress)
{
    if (!qFuzzyCompare(m_progress, progress)) {
        m_progress = qBound(0.0, progress, 1.0);
        emit progressChanged();
    }
}

void Bee::setPositionX(double x)
{
    if (!qFuzzyCompare(m_positionX, x)) {
        m_positionX = qBound(0.0, x, 1.0);
        emit positionXChanged();
    }
}

void Bee::setPositionY(double y)
{
    if (!qFuzzyCompare(m_positionY, y)) {
        m_positionY = qBound(0.0, y, 1.0);
        emit positionYChanged();
    }
}

bool Bee::canFly() const
{
    return m_energy >= 10 && !m_isFlying;
}

int Bee::calculateFlightTime() const
{
    // Հաշվարկել նեկտարի լցվածության գործակիցը
    double loadFactor = 0.0;
    if (m_capacity > 0 && m_currentNectar > 0) {
        loadFactor = m_currentNectar / m_capacity;
        loadFactor = qBound(0.0, loadFactor, 1.0);
    }

    // Էներգիայի գործակից
    double energyFactor = 1.0;
    if (m_energy < 30) {
        energyFactor = 1.0 + (30.0 - m_energy) / 30.0 * 0.3;
    }

    double baseTime = m_minTime + (m_maxTime - m_minTime) * loadFactor;

    int finalTime = static_cast<int>(baseTime * energyFactor);

    return finalTime;
}

void Bee::consumeEnergy(int amount)
{
    setEnergy(m_energy - amount);
}

bool Bee::collectNectar(const QString &type, double amount)
{
    if (m_currentNectar == 0 && amount <= m_capacity) {
        setNectarType(type);
        setCurrentNectar(amount);
        return true;
    }
    return false;
}

void Bee::depositNectar()
{
    setNectarType("");
    setCurrentNectar(0.0);
}

bool Bee::isEmpty() const
{
    return m_currentNectar <= 0.0;
}

bool Bee::isFull() const
{
    return m_currentNectar >= m_capacity;
}
