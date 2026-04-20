#ifndef FLOWER_H
#define FLOWER_H

#include <QString>
#include <QPointF>

class Flower {
private:
    int id;
    QString type;           // Ծաղկի տեսակը
    double nectarAmount;    // Առկա նեկտարի քանակը
    double maxNectar;       // Առավելագույն նեկտար
    QPointF position;
    double regenerationRate; // Վերականգնման արագություն (գ/վայրկյան)

public:
    Flower(int id, const QString& type, double maxNectar, const QPointF& position);

    // Getters
    int getId() const { return id; }
    QString getType() const { return type; }
    double getNectarAmount() const { return nectarAmount; }
    double getMaxNectar() const { return maxNectar; }
    QPointF getPosition() const { return position; }
    double getRegenerationRate() const { return regenerationRate; }
    bool hasNectar() const { return nectarAmount > 0.0; }

    // Operations
    double collectNectar(double requestedAmount);
    void regenerate(double deltaTime);
    void reset();
    QString getStatusString() const;
};

#endif
