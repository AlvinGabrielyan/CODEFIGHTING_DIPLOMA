#include "flower.h"
#include <algorithm>

Flower::Flower(int id, const QString& type, double maxNectar, const QPointF& position)
    : id(id), type(type), nectarAmount(maxNectar), maxNectar(maxNectar),
    position(position), regenerationRate(0.5) { // 0.5 գ/վրկ
}

double Flower::collectNectar(double requestedAmount) {
    double collected = std::min(requestedAmount, nectarAmount);
    nectarAmount -= collected;
    return collected;
}

void Flower::regenerate(double deltaTime) {
    if (nectarAmount < maxNectar) {
        nectarAmount = std::min(nectarAmount + regenerationRate * deltaTime, maxNectar);
    }
}

void Flower::reset() {
    nectarAmount = maxNectar;
}

QString Flower::getStatusString() const {
    return QString("%1: %2/%3գ նեկտար")
        .arg(type)
        .arg(nectarAmount, 0, 'f', 1)
        .arg(maxNectar, 0, 'f', 1);
}
