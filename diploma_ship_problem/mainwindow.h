#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QListWidget>
#include <QSpinBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QMap>
#include "simulator.h"

class BoatGraphicsItem : public QGraphicsItem
{
public:
    BoatGraphicsItem(Boat *boat, QGraphicsItem *parent = nullptr);
    void updatePosition();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Boat *m_boat;
    QGraphicsTextItem *m_label;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Simulator *simulator, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onPauseClicked();
    void onResetClicked();
    void updateUI();
    void updateVisualization();
    void onLogMessage(const QString &message, const QString &type);
    void onSimulationFinished();
    void onShowHelp();
    void onShowProblem();

private:
    void setupUI();
    void setupVisualization();
    void updateBoatsList();
    void updateStatus();

    Simulator *m_simulator;

    QTextEdit *m_scriptEditor;
    QTextEdit *m_logViewer;
    QListWidget *m_boatsList;
    QPushButton *m_startButton;
    QPushButton *m_pauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_helpButton;
    QPushButton *m_problemButton;
    QLabel *m_timeLabel;
    QLabel *m_statusLabel;
    QLabel *m_completedLabel;
    QLabel *m_collisionsLabel;
    QProgressBar *m_bridgeHealthBar;

    // Visualization
    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_scene;
    QGraphicsRectItem *m_bridge;
    QGraphicsTextItem *m_sideALabel;
    QGraphicsTextItem *m_sideBLabel;
    QMap<QString, BoatGraphicsItem*> m_boatItems;
    QTimer *m_visualTimer;
};

#endif
