// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include <QWidget>
// #include <QPushButton>
// #include <QComboBox>
// #include <QTextEdit>
// #include <QLabel>
// #include <QSlider>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QGroupBox>
// #include <QTimer>
// #include <QListWidget>
// #include "SpaceExplorerEngine.h"

// class GridWidget;

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private slots:
//     void onStartPause();
//     void onReset();
//     void onAddCommands();
//     void onClearCommands();
//     void onStepByStep();
//     void onExplorerChanged(int index);
//     void updateVisualization();
//     void onSpeedChanged(int value);
//     void onErrorOccurred(const QString &error);

// private:
//     void setupUI();
//     QWidget* createLeftPanel();
//     QWidget* createCenterPanel();
//     QWidget* createRightPanel();

//     // Backend
//     SpaceExplorerEngine *engine;

//     // UI Components
//     GridWidget *gridWidget;
//     QLabel *statusLabel;
//     QLabel *scanCountLabel;
//     QPushButton *startPauseBtn;
//     QPushButton *resetBtn;
//     QComboBox *explorerSelector;
//     QTextEdit *commandInput;
//     QTextEdit *currentCommands;
//     QSlider *speedSlider;
//     QLabel *speedLabel;
//     QListWidget *errorLog;
// };

// // Custom Widget for Grid Visualization
// class GridWidget : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit GridWidget(SpaceExplorerEngine *engine, QWidget *parent = nullptr);
//     void updateGrid();

// protected:
//     void paintEvent(QPaintEvent *event) override;

// private:
//     SpaceExplorerEngine *m_engine;
//     int cellSize;

//     struct Planet {
//         int x, y;
//         double size;
//         QColor color;
//     };
//     QList<Planet> planets;
// };

// #endif // MAINWINDOW_H




#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTimer>
#include <QListWidget>
#include "SpaceExplorerEngine.h"

class GridWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartPause();
    void onReset();
    void onAddCommands();
    void onClearCommands();
    void onStepByStep();
    void onExplorerChanged(int index);
    void updateVisualization();
    void onSpeedChanged(int value);
    void onErrorOccurred(const QString &error);

private:
    void setupUI();
    QWidget* createLeftPanel();
    QWidget* createCenterPanel();
    QWidget* createRightPanel();
    void showStarRatingDialog();

    // Backend
    SpaceExplorerEngine *engine;

    // UI Components
    GridWidget *gridWidget;
    QLabel *statusLabel;
    QLabel *scanCountLabel;
    QPushButton *startPauseBtn;
    QPushButton *resetBtn;
    QComboBox *explorerSelector;
    QTextEdit *commandInput;
    QTextEdit *currentCommands;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QListWidget *errorLog;
};

// Custom Widget for Grid Visualization
class GridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GridWidget(SpaceExplorerEngine *engine, QWidget *parent = nullptr);
    void updateGrid();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    SpaceExplorerEngine *m_engine;
    int cellSize;

    struct Planet {
        int x, y;
        double size;
        QColor color;
    };
    QList<Planet> planets;
};

#endif // MAINWINDOW_H
