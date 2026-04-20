#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QVBoxLayout>
#include <queue>
#include "mazelogic.h"


class MazeScene : public QGraphicsScene {
    Q_OBJECT
public:
    static const int CS = 52;
    static const int WT = 5;

    explicit MazeScene(MazeLogic* logic, QObject* parent = nullptr);
    void redraw();

private:
    MazeLogic* logic;
    void drawFloor();
    void drawAllWalls();
    void drawHWall(int wallRow, int col, bool moving);
    void drawVWall(int row, int wallCol, bool moving);
    void drawSpecial();
    void drawRobot();
    void drawStickFigure(qreal cx, qreal cy, Direction dir, int victoryFrame);
};


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void onRun();
    void onStep();
    void onReset();
    void onClearLog();
    void tick();
    void victoryTick();

private:
    MazeLogic*     logic;
    MazeScene*     scene;
    QGraphicsView* view;
    QTextEdit*     logBox;
    QTextEdit*     cmdBox;
    QLabel*        lblStatus;
    QLabel*        lblSteps;
    QLabel*        lblPos;
    QLabel*        lblDir;
    QPushButton*   btnRun;
    QPushButton*   btnStep;
    QPushButton*   btnReset;
    QPushButton*   btnClear;
    QTimer*        timer;
    std::queue<Command> cmdQ;
    bool  running  = false;
    int   speed    = 1200;
    QTimer* victoryTimer = nullptr;

    void setupUI();
    void buildLeftPanel (QVBoxLayout*);
    void buildRightPanel(QVBoxLayout*);
    QGroupBox* mkStatusBox();
    QGroupBox* mkRulesBox();
    QGroupBox* mkStoryBox();
    QGroupBox* mkCmdBox();
    QGroupBox* mkCtrlBox();
    QGroupBox* mkLogBox();

    bool isKnownCmd(const QString&);
    void parseInput();
    void log(const QString& msg, const QString& type = "info");
    void refreshStatus();
    Command strToCmd(const QString&);
    QString cmdToStr(Command);
};

#endif
