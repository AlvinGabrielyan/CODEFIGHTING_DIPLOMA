#ifndef BEESIMULATORWINDOW_H
#define BEESIMULATORWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include "beesimulator.h"

class BeeSimulatorWindow : public QMainWindow {
    Q_OBJECT

private:
    BeeSimulator* simulator;
    QWidget* vizWidget;
    QTextEdit* scriptInput;
    QTextEdit* logOutput;
    QPushButton* startButton;
    QPushButton* pauseButton;
    QPushButton* resetButton;
    QLabel* timeLabel;

    struct RatingResult {
        int stars;
        QString title;
        QString description;
        QList<QString> criteria;
    };

    RatingResult calculateRating();
    void showRatingDialog(const RatingResult& result);

public:
    explicit BeeSimulatorWindow(QWidget* parent = nullptr);

private:
    void setupUI();
    void connectSignals();
    void showExampleScript();

private slots:
    void onStart();
    void onPause();
    void onReset();
    void updateTime();
    void onLogMessage(const QString& message, const QString& type);
    void onSimulationFinished();
    void onProblemDescription();
    void onHelp();
};

#endif
