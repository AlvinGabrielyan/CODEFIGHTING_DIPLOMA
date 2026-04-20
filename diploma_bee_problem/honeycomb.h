#ifndef HONEYCOMB_H
#define HONEYCOMB_H

#include <QString>
#include <QPointF>

class Honeycomb {
private:
    int id;
    double capacity;        // Ամանի տարողությունը (գրամ)
    double currentAmount;   // Ներկայիս լցվածությունը (գրամ)
    bool sealed;            // Փակված է արդյոք
    QPointF position;

public:
    Honeycomb(int id, double capacity, const QPointF& position);

    int getId() const { return id; }
    double getCapacity() const { return capacity; }
    double getCurrentAmount() const { return currentAmount; }
    bool isSealed() const { return sealed; }
    QPointF getPosition() const { return position; }
    double getAvailableSpace() const { return capacity - currentAmount; }
    double getFillPercentage() const { return (currentAmount / capacity) * 100.0; }
    bool isFull() const { return currentAmount >= capacity; }

    double addNectar(double amount);
    void seal() { sealed = true; }
    void unseal() { sealed = false; }
    void reset();
    QString getStatusString() const;
};

#endif
