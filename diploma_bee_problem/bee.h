#ifndef BEE_H
#define BEE_H

#include <QObject>
#include <QString>
#include <QColor>

class Bee : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString targetLocation READ targetLocation WRITE setTargetLocation NOTIFY targetLocationChanged)
    Q_PROPERTY(int energy READ energy WRITE setEnergy NOTIFY energyChanged)
    Q_PROPERTY(double capacity READ capacity CONSTANT)
    Q_PROPERTY(double currentNectar READ currentNectar WRITE setCurrentNectar NOTIFY currentNectarChanged)
    Q_PROPERTY(QString nectarType READ nectarType WRITE setNectarType NOTIFY nectarTypeChanged)
    Q_PROPERTY(bool isFlying READ isFlying WRITE setIsFlying NOTIFY isFlyingChanged)
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(double positionX READ positionX WRITE setPositionX NOTIFY positionXChanged)
    Q_PROPERTY(double positionY READ positionY WRITE setPositionY NOTIFY positionYChanged)
    Q_PROPERTY(QColor color READ color CONSTANT)

public:
    explicit Bee(QObject *parent = nullptr);

    Bee(const QString &id, const QString &name, const QString &location,
        double capacity, int minTime, int maxTime, const QColor &color,
        const QString &description = QString(), QObject *parent = nullptr);

    QString id() const { return m_id; }
    QString name() const { return m_name; }
    QString location() const { return m_location; }
    QString initialLocation() const { return m_initialLocation; }
    QString targetLocation() const { return m_targetLocation; }
    int energy() const { return m_energy; }
    double capacity() const { return m_capacity; }
    double currentNectar() const { return m_currentNectar; }
    QString nectarType() const { return m_nectarType; }
    bool isFlying() const { return m_isFlying; }
    double progress() const { return m_progress; }
    double positionX() const { return m_positionX; }
    double positionY() const { return m_positionY; }
    QColor color() const { return m_color; }
    QString description() const { return m_description; }
    int minTime() const { return m_minTime; }
    int maxTime() const { return m_maxTime; }

    void setLocation(const QString &location);
    void setTargetLocation(const QString &targetLocation);
    void setEnergy(int energy);
    void setCurrentNectar(double nectar);
    void setNectarType(const QString &type);
    void setIsFlying(bool flying);
    void setProgress(double progress);
    void setPositionX(double x);
    void setPositionY(double y);

    Q_INVOKABLE bool canFly() const;
    Q_INVOKABLE int calculateFlightTime() const;
    Q_INVOKABLE void consumeEnergy(int amount);
    Q_INVOKABLE bool collectNectar(const QString &type, double amount);
    Q_INVOKABLE void depositNectar();
    Q_INVOKABLE bool isEmpty() const;
    Q_INVOKABLE bool isFull() const;

signals:
    void locationChanged();
    void targetLocationChanged();
    void energyChanged();
    void currentNectarChanged();
    void nectarTypeChanged();
    void isFlyingChanged();
    void progressChanged();
    void positionXChanged();
    void positionYChanged();

private:
    QString m_id;
    QString m_name;
    QString m_location;
    QString m_initialLocation;
    QString m_targetLocation;
    int m_energy;
    double m_capacity;
    double m_currentNectar;
    QString m_nectarType;
    bool m_isFlying;
    double m_progress;
    double m_positionX;
    double m_positionY;
    QColor m_color;
    QString m_description;
    int m_minTime;
    int m_maxTime;
};

#endif // BEE_H
