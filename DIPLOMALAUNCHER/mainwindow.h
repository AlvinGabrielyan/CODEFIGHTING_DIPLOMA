#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QFrame>
#include <QScrollArea>
#include <QStandardPaths>
#include <QTimer>
#include <QSet>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLevel(int level);
    void showHome();
    void showProgress();
    void refreshProgress();

    // Called when a monitored child process finishes
    void onAppFinished(const QString &projectId, int level);

private:
    QStackedWidget *stackedWidget;
    QWidget        *m_progressPage = nullptr;

    QPushButton *m_levelBtn[4] = { nullptr, nullptr, nullptr, nullptr };
    QTimer  *m_pollTimer   = nullptr;
    QSet<QString> m_handledProjects;

    QWidget* createHomePage();
    QWidget* createLevelPage(int level);
    void     rebuildLevelPage(int level);
    void     rebuildProgressPage();

    QPushButton* createLevelButton(const QString &title, const QString &subtitle, int level);
    QPushButton* createAppButton(const QString &title, const QString &description,
                                 const QString &icon, const QString &exePath,
                                 const QString &projectId, int level);

    void    applyGlobalStyle();
    bool    initDatabase();

    void    updateLevelButtons();

    void    launchApp(const QString &exePath, const QString &appName,
                   const QString &projectId, int level);

    int     getStars(const QString &projectId);
    QString getGradeLetter(double avg);
    QString getGradeColor(double avg);

    bool    isLevelComplete(int level);

    void    showLevelCompleteDialog(int completedLevel);

    struct ProjectInfo {
        QString id;
        QString name;
        QString icon;
        QString exe;
        int     level;
    };
    QList<ProjectInfo> allProjects();
};

#endif
