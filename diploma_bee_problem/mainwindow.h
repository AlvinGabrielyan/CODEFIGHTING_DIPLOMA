#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "beesystem.h"
#include "interpreter.h"

class VisualizationWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    // Համակարգ
    BeeSystem* beeSystem;
    Interpreter* interpreter;

    // GUI բաղադրիչներ
    VisualizationWidget* visualWidget;
    QTextEdit* messageLog;
    QTextEdit* descriptionText;
    QTextEdit* rulesText;
    QLineEdit* commandInput;
    QPushButton* executeButton;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* resetButton;
    QPushButton* stepButton;
    QPushButton* helpButton;

    void setupUI();
    void createMenuBar();
    void createDescriptionPanel();
    void createRulesPanel();
    void createCommandPanel();
    void createControlPanel();
    void createVisualizationPanel();
    void createMessagePanel();

    // Տեղեկատվական մեթոդներ
    QString getSystemStatus();
    QString getBeesStatus();
    QString getCombsStatus();
    QString getFlowersStatus();
    QString getBeeInfo(int index);
    QString getCombInfo(int index);
    QString getFlowerInfo(int index);

private slots:
    void onExecuteCommand();
    void onStart();
    void onStop();
    void onReset();
    void onStep();
    void onHelp();
    void onSystemUpdated();
    void onMessageAdded(const QString& message);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

// Վիզուալիզացիայի վիջեթ
class VisualizationWidget : public QWidget {
    Q_OBJECT

private:
    BeeSystem* system;

public:
    explicit VisualizationWidget(BeeSystem* sys, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawBee(QPainter& painter, const Bee* bee);
    void drawHoneycomb(QPainter& painter, const Honeycomb* comb);
    void drawFlower(QPainter& painter, const Flower* flower);
};

#endif // MAINWINDOW_H
