#include "beesystem.h"
#include <QDebug>
#include <cmath>

BeeSystem::BeeSystem(QObject* parent)
    : QObject(parent), updateTimer(new QTimer(this)), maxMessages(100),
    flowerCollectionTime(2.0), depositTime(1.5), eatingTime(1.0) {

    connect(updateTimer, &QTimer::timeout, this, &BeeSystem::update);
}

BeeSystem::~BeeSystem() {
    qDeleteAll(bees);
    qDeleteAll(honeycombs);
    qDeleteAll(flowers);
}

void BeeSystem::initialize() {
    // Ստեղծել մեղուներ - using the correct constructor
    bees.append(new Bee("BEE1", "Մայա", "HIVE", 3.0, 3, 5, QColor("#FFD700"), "Մայա մեղուն"));
    bees.append(new Bee("BEE2", "Վիլի", "HIVE", 2.5, 3, 5, QColor("#FFA500"), "Վիլի մեղուն"));
    bees.append(new Bee("BEE3", "Զիթա", "HIVE", 2.0, 3, 5, QColor("#FF69B4"), "Զիթա մեղուն"));

    // Դիրքավորել մեղուներին սկզբում
    for (int i = 0; i < bees.size(); ++i) {
        bees[i]->setPositionX(0.1 + i * 0.05);
        bees[i]->setPositionY(0.8);
    }

    // Ստեղծել ամաններ
    for (int i = 0; i < 5; ++i) {
        QPointF pos(80 + (i % 3) * 40, 350 + (i / 3) * 45);
        honeycombs.append(new Honeycomb(i + 1, 5.0, pos));
    }

    // Ստեղծել ծաղիկներ
    flowers.append(new Flower(1, "Վարդ", 8.0, QPointF(400, 100)));
    flowers.append(new Flower(2, "Մանուշակ", 6.0, QPointF(500, 150)));
    flowers.append(new Flower(3, "Երիցուկ", 7.0, QPointF(600, 120)));

    addMessage("Համակարգը նախապատրաստված է։ 3 մեղու, 5 աման, 3 ծաղիկ։");
}

void BeeSystem::reset() {
    stop();

    for (Bee* bee : bees) {
        bee->setLocation("HIVE");
        bee->setEnergy(100);
        bee->setCurrentNectar(0.0);
        bee->setNectarType("");
        bee->setIsFlying(false);
    }

    for (Honeycomb* comb : honeycombs) {
        comb->reset();
    }

    for (Flower* flower : flowers) {
        flower->reset();
    }

    beeActionTimers.clear();
    clearMessages();

    // Վերադիրքավորել մեղուները
    for (int i = 0; i < bees.size(); ++i) {
        bees[i]->setPositionX(0.1 + i * 0.05);
        bees[i]->setPositionY(0.8);
    }

    addMessage("Համակարգը վերականգնվեց։");
    emit systemUpdated();
}

void BeeSystem::start() {
    // Սա այլևս չի սկսում ավտոմատ մեղուների աշխատանքը
    // Միայն թարմացնում է վիզուալիզացիան
    updateTimer->start(50); // 50մվ
    addMessage("Համակարգը պատրաստ է հրամանների համար։");
}

void BeeSystem::stop() {
    updateTimer->stop();
    addMessage("Համակարգը դադարեցված է։");
}

void BeeSystem::step() {
    update();
}

void BeeSystem::update() {
    double deltaTime = 0.05; // 50մվ
    // Միայն ծաղիկները թարմացվում են (վերականգնվում)
    // Մեղուները չեն աշխատում ավտոմատ - միայն հրամանների միջոցով
    updateFlowers(deltaTime);

    emit systemUpdated();
}

void BeeSystem::updateBees(double deltaTime) {
    Q_UNUSED(deltaTime);
}

void BeeSystem::updateFlowers(double deltaTime) {
    for (Flower* flower : flowers) {
        flower->regenerate(deltaTime);
    }
}

void BeeSystem::processBeeLogic(Bee* bee, double deltaTime) {
    Q_UNUSED(bee);
    Q_UNUSED(deltaTime);
}

Flower* BeeSystem::findAvailableFlower() {
    for (Flower* flower : flowers) {
        if (flower->hasNectar()) {
            return flower;
        }
    }
    return nullptr;
}

Honeycomb* BeeSystem::findAvailableHoneycomb() {
    for (Honeycomb* comb : honeycombs) {
        if (!comb->isSealed() && !comb->isFull()) {
            return comb;
        }
    }
    return nullptr;
}

void BeeSystem::handleFullHoneycombs(Bee* bee) {
    double remaining = bee->currentNectar();

    // Փորձել լցնել առաջին հասանելի ամանի մեջ
    Honeycomb* comb = findAvailableHoneycomb();

    if (comb) {
        double space = comb->getAvailableSpace();

        if (space >= remaining) {
            // Ամբողջ նեկտարը տեղավորվում է
            comb->addNectar(remaining);
            bee->depositNectar();
            addMessage(QString("%1-ը լցրեց %2գ նեկտար աման №%3-ում։")
                           .arg(bee->name()).arg(remaining, 0, 'f', 1).arg(comb->getId()));
        } else {
            // Ամանը լցվում է մասամբ
            comb->addNectar(space);
            comb->seal();
            addMessage(QString("%1-ը լցրեց %2գ և փակեց աման №%3-ը։")
                           .arg(bee->name()).arg(space, 0, 'f', 1).arg(comb->getId()));

            bee->setCurrentNectar(remaining - space);
            remaining = bee->currentNectar();

            // Փնտրել հաջորդ ամանը
            Honeycomb* nextComb = findAvailableHoneycomb();

            if (nextComb) {
                // Կան ազատ ամաններ
                nextComb->addNectar(remaining);
                bee->depositNectar();
                addMessage(QString("%1-ը տեղափոխեց մնացած %2գ-ը աման №%3։")
                               .arg(bee->name()).arg(remaining, 0, 'f', 1).arg(nextComb->getId()));
            } else {
                // Ազատ ամաններ չկան - պետք է ուտի
                addMessage(QString(" %1-ը ստիպված է ուտել %2գ նեկտար (տեղ չկա)։")
                               .arg(bee->name()).arg(remaining, 0, 'f', 1));
                bee->depositNectar();
            }
        }
    } else {
        // Ամբողջովին ազատ ամաններ չկան
        addMessage(QString("%1-ը ուտում է բոլոր %2գ նեկտարը (բոլոր ամանները լցված են)։")
                       .arg(bee->name()).arg(remaining, 0, 'f', 1));
        bee->depositNectar();
    }
}

void BeeSystem::addMessage(const QString& message) {
    messages.prepend(message);
    if (messages.size() > maxMessages) {
        messages.removeLast();
    }
    emit messageAdded(message);
}

void BeeSystem::clearMessages() {
    messages.clear();
}
