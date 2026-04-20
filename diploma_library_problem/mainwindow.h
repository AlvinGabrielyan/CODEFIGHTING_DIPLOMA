#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QListWidget>
#include "interpreter.h"
#include "librarian_state.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onResetClicked();
    void onExecuteCommandClicked();
    void updateDisplay();

private:
    QTextEdit *outputText;
    QTextEdit *problemDescriptionText;
    QTextEdit *rulesText;
    QTextEdit *commandsText;
    QTableWidget *booksTable;
    QLineEdit *commandInput;
    QListWidget *commandHistoryList;
    QLabel *statusLabel;
    QLabel *energyLabel;
    QLabel *roomLabel;
    QLabel *booksInHandLabel;
    QPushButton *resetButton;
    QPushButton *executeCommandButton;
    QTabWidget *tabWidget;

    Interpreter *interpreter;
    LibrarianState currentState;
    QVector<QString> commandHistory;

    void setupUI();
    void initializeState();
    void updateBooksTable();
    void setupProblemDescription();
    void setupRules();
    void setupCommands();
    void logOutput(const QString& message, bool isError = false);
    void addToHistory(const QString& command);

    void applyStyles();
    QString getBookStatusIcon(bool delivered, bool inHand);
    void showStarRatingDialog();
};

#endif
