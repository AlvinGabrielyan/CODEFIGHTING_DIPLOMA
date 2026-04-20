#ifndef BEESYSTEM_H
#define BEESYSTEM_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QStringList>
#include <QMap>
#include "bee.h"
#include "honeycomb.h"
#include "flower.h"

class BeeSystem : public QObject {
    Q_OBJECT

private:
    QVector<Bee*> bees;
    QVector<Honeycomb*> honeycombs;
    QVector<Flower*> flowers;
    QTimer* updateTimer;
    QStringList messages;
    int maxMessages;

    double flowerCollectionTime;    // Ծաղկից հավաքելու ժամանակ (վրկ)
    double depositTime;             // Ամանի մեջ լցնելու ժամանակ (վրկ)
    double eatingTime;              // Ուտելու ժամանակ (վրկ)

    QMap<int, double> beeActionTimers; // Գործողությունների ժամանակաչափներ

public:
    explicit BeeSystem(QObject* parent = nullptr);
    ~BeeSystem();

    void initialize();
    void reset();

    const QVector<Bee*>& getBees() const { return bees; }
    const QVector<Honeycomb*>& getHoneycombs() const { return honeycombs; }
    const QVector<Flower*>& getFlowers() const { return flowers; }
    const QStringList& getMessages() const { return messages; }

    void start();
    void stop();
    void step();

    void addMessage(const QString& message);
    void clearMessages();

signals:
    void systemUpdated();
    void messageAdded(const QString& message);
    void errorOccurred(const QString& error);

private slots:
    void update();

private:
    void updateBees(double deltaTime);
    void updateFlowers(double deltaTime);
    void processBeeLogic(Bee* bee, double deltaTime);
    Flower* findAvailableFlower();
    Honeycomb* findAvailableHoneycomb();
    void handleFullHoneycombs(Bee* bee);
};

#endif
