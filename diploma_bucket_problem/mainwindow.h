// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include <QTextEdit>
// #include <QPushButton>
// #include <QLabel>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QTimer>
// #include <QComboBox>
// #include <QPropertyAnimation>
// #include <climits>
// #include <map>
// #include <string>
// #include "interpreter.h"
// #include "problem_definition.h"

// enum class PourDirection { None, XtoY, YtoX };

// class BucketWidget : public QWidget {
//     Q_OBJECT
//     Q_PROPERTY(float animatedWaterLevel READ getAnimatedWaterLevel WRITE setAnimatedWaterLevel)

// public:
//     explicit BucketWidget(const QString& name, int capacity, QWidget* parent = nullptr);
//     void setValue(int value);
//     void setCapacity(int capacity);
//     int getValue() const { return currentValue; }
//     void setPourDirection(PourDirection dir);

//     float getAnimatedWaterLevel() const;
//     void setAnimatedWaterLevel(float v);

// protected:
//     void paintEvent(QPaintEvent* event) override;

// private:
//     QString bucketName;
//     int maxCapacity;
//     int currentValue;
//     float animatedValue;
//     float ripplePhase = 0.0f;
//     PourDirection pourDirection;
//     QPropertyAnimation* waterAnim;
//     QTimer* rippleTimer;
// };

// class PourArrowWidget : public QWidget {
//     Q_OBJECT
// public:
//     explicit PourArrowWidget(QWidget* parent = nullptr);
//     void showPour(PourDirection dir);
//     void hidePour();

// protected:
//     void paintEvent(QPaintEvent* event) override;

// private:
//     PourDirection direction;
//     float animPhase;
//     bool visible;
//     QTimer* animTimer;
// };

// class MainWindow : public QMainWindow {
//     Q_OBJECT

// public:
//     // capA, capB, target come from InputWindow
//     explicit MainWindow(QWidget *parent, int capA, int capB, int target);
//     ~MainWindow();

// private slots:
//     void onRunClicked();
//     void onResetClicked();
//     void onStepClicked();
//     void executeNextStep();
//     void checkFinalResult();

// private:
//     void setupUI();
//     void updateBucketDisplay();
//     void updateDescription();
//     void showMessage(const QString& msg, const QString& color);
//     void triggerPourAnimation(const std::string& cmd);

//     QTextEdit* codeEditor;
//     QTextEdit* outputConsole;
//     BucketWidget* bucketX;
//     BucketWidget* bucketY;
//     PourArrowWidget* pourArrow;
//     QPushButton* runButton;
//     QPushButton* resetButton;
//     QPushButton* stepButton;
//     QLabel* statusLabel;
//     QLabel* descriptionLabel;

//     // The single problem built from user's input
//     ProblemDefinition* problem;
//     Interpreter* interpreter;
//     std::vector<std::string> commands;
//     int currentStep;
//     QTimer* stepTimer;
//     bool isRunning;
//     int errorCountThisRun = 0;

//     int m_capA, m_capB, m_target;
//     int m_bestSteps = INT_MAX;
//     bool m_solved   = false;

//     void showStarDialog(int stars);
//     int  computeStars(int usedSteps) const;
// };

// #endif


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QComboBox>
#include <QPropertyAnimation>
#include <climits>
#include <map>
#include <string>
#include "interpreter.h"
#include "problem_definition.h"

enum class PourDirection { None, XtoY, YtoX };

class BucketWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(float animatedWaterLevel READ getAnimatedWaterLevel WRITE setAnimatedWaterLevel)

public:
    explicit BucketWidget(const QString& name, int capacity, QWidget* parent = nullptr);
    void setValue(int value);
    void setCapacity(int capacity);
    int getValue() const { return currentValue; }
    void setPourDirection(PourDirection dir);

    float getAnimatedWaterLevel() const;
    void setAnimatedWaterLevel(float v);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString bucketName;
    int maxCapacity;
    int currentValue;
    float animatedValue;
    float ripplePhase = 0.0f;
    PourDirection pourDirection;
    QPropertyAnimation* waterAnim;
    QTimer* rippleTimer;
};

class PourArrowWidget : public QWidget {
    Q_OBJECT
public:
    explicit PourArrowWidget(QWidget* parent = nullptr);
    void showPour(PourDirection dir);
    void hidePour();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    PourDirection direction;
    float animPhase;
    bool visible;
    QTimer* animTimer;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // capA, capB, target come from InputWindow
    explicit MainWindow(QWidget *parent, int capA, int capB, int target);
    ~MainWindow();

private slots:
    void onRunClicked();
    void onResetClicked();
    void onStepClicked();
    void executeNextStep();
    void checkFinalResult();

private:
    void setupUI();
    void updateBucketDisplay();
    void updateDescription();
    void showMessage(const QString& msg, const QString& color);
    void triggerPourAnimation(const std::string& cmd);

    QTextEdit* codeEditor;
    QTextEdit* outputConsole;
    BucketWidget* bucketX;
    BucketWidget* bucketY;
    PourArrowWidget* pourArrow;
    QPushButton* runButton;
    QPushButton* resetButton;
    QPushButton* stepButton;
    QLabel* statusLabel;
    QLabel* descriptionLabel;

    // The single problem built from user's input
    ProblemDefinition* problem;
    Interpreter* interpreter;
    std::vector<std::string> commands;
    int currentStep;
    QTimer* stepTimer;
    bool isRunning;
    int errorCountThisRun = 0;

    int m_capA, m_capB, m_target;
    int m_optimalSteps = 0;
    int m_bestSteps = INT_MAX;
    bool m_solved   = false;

    void showStarDialog(int stars);
    int  computeStars(int usedSteps) const;
};

#endif
