#include "honeycomb.h"
#include <algorithm>

Honeycomb::Honeycomb(int id, double capacity, const QPointF& position)
    : id(id), capacity(capacity), currentAmount(0.0), sealed(false), position(position) {
}

double Honeycomb::addNectar(double amount) {
    if (sealed) {
        return 0.0; // Չի կարող ավելացնել եթե փակված է
    }

    double canAdd = std::min(amount, capacity - currentAmount);
    currentAmount += canAdd;

    return canAdd; // Վերադարձնում է ինչքան իրականում ավելացվեց
}

void Honeycomb::reset() {
    currentAmount = 0.0;
    sealed = false;
}

QString Honeycomb::getStatusString() const {
    if (sealed) {
        return QString("Փակված (%1/%2գ)").arg(currentAmount, 0, 'f', 1).arg(capacity, 0, 'f', 1);
    } else {
        return QString("Բաց (%1/%2գ)").arg(currentAmount, 0, 'f', 1).arg(capacity, 0, 'f', 1);
    }
}
