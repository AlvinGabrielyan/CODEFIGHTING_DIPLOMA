#ifndef RESULTSAVER_H
#define RESULTSAVER_H

#include <QString>
#include <QList>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

struct ProjectResult {
    QString projectId;
    QString starsStr;
    int     starsInt;
    QString savedAt;
};

class ResultSaver {
public:

    // Բոլոր պրոյեկտները կարդում են ՆՈՒՅՆ DB ֆայլը
    static QString dbPath() {
        QString dir = QStandardPaths::writableLocation(
            QStandardPaths::GenericDataLocation);
        dir += "/DiplomaApp";
        QDir().mkpath(dir);
        return dir + "/results.db";
    }

    static bool init() {
        if (!QSqlDatabase::contains(CONN)) {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", CONN);
            db.setDatabaseName(dbPath());
        }
        QSqlDatabase db = QSqlDatabase::database(CONN);
        if (!db.open()) {
            qWarning() << "ResultSaver init error:" << db.lastError().text();
            return false;
        }
        QSqlQuery q(db);
        return q.exec(
            "CREATE TABLE IF NOT EXISTS results ("
            "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  project_id  TEXT    NOT NULL UNIQUE,"
            "  stars       INTEGER NOT NULL CHECK(stars BETWEEN 1 AND 5),"
            "  saved_at    TEXT    NOT NULL"
            ")"
            );
    }

    // Պահպանում է ՄԻԱՅՆ եթե նոր արդյունքը >= նախկինից
    static bool save(const QString &projectId, int stars) {
        if (stars < 1 || stars > 5) return false;
        if (!ensureOpen())          return false;

        int existing = getStarsInt(projectId);
        if (existing >= stars) return true;

        QSqlDatabase db = QSqlDatabase::database(CONN);
        QSqlQuery q(db);
        q.prepare(
            "INSERT INTO results (project_id, stars, saved_at) "
            "VALUES (:p, :s, :t) "
            "ON CONFLICT(project_id) DO UPDATE SET "
            "  stars    = excluded.stars, "
            "  saved_at = excluded.saved_at"
            );
        q.bindValue(":p", projectId);
        q.bindValue(":s", stars);
        q.bindValue(":t", QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm"));
        if (!q.exec()) {
            qWarning() << "ResultSaver save error:" << q.lastError().text();
            return false;
        }
        return true;
    }

    // Վերադարձնում է -1 եթե դեռ չի ավարտվել
    static int getStarsInt(const QString &projectId) {
        if (!ensureOpen()) return -1;
        QSqlQuery q(QSqlDatabase::database(CONN));
        q.prepare("SELECT stars FROM results WHERE project_id = :p");
        q.bindValue(":p", projectId);
        q.exec();
        return q.next() ? q.value(0).toInt() : -1;
    }

    static QList<ProjectResult> getAllResults() {
        QList<ProjectResult> list;
        if (!ensureOpen()) return list;
        QSqlQuery q(QSqlDatabase::database(CONN));
        q.exec("SELECT project_id, stars, saved_at FROM results "
               "ORDER BY saved_at DESC");
        while (q.next()) {
            ProjectResult r;
            r.projectId = q.value(0).toString();
            r.starsInt  = q.value(1).toInt();
            r.starsStr  = starsToString(r.starsInt);
            r.savedAt   = q.value(2).toString();
            list.append(r);
        }
        return list;
    }

    struct Summary {
        int     completedCount;
        int     totalProblems;
        int     progressPercent;
        double  averageStars;
        QString grade;
        QString gradeColor;
        QString lastActivity;
    };

    static Summary getSummary(int totalProblems = 6) {
        Summary s;
        s.totalProblems   = totalProblems;
        s.completedCount  = 0;
        s.progressPercent = 0;
        s.averageStars    = 0.0;
        s.grade           = "—";
        s.gradeColor      = "#888888";
        s.lastActivity    = "—";

        QList<ProjectResult> results = getAllResults();
        if (results.isEmpty()) return s;

        int sum = 0;
        for (const ProjectResult &r : results) {
            sum += r.starsInt;
            s.completedCount++;
        }
        s.progressPercent = (s.completedCount * 100) / totalProblems;
        s.averageStars    = static_cast<double>(sum) / s.completedCount;
        s.lastActivity    = results.first().savedAt;

        if      (s.averageStars >= 4.5) { s.grade = "A+"; s.gradeColor = "#22c55e"; }
        else if (s.averageStars >= 4.0) { s.grade = "A";  s.gradeColor = "#4ade80"; }
        else if (s.averageStars >= 3.5) { s.grade = "B+"; s.gradeColor = "#facc15"; }
        else if (s.averageStars >= 3.0) { s.grade = "B";  s.gradeColor = "#fb923c"; }
        else if (s.averageStars >= 2.0) { s.grade = "C";  s.gradeColor = "#f87171"; }
        else                             { s.grade = "D";  s.gradeColor = "#ef4444"; }

        return s;
    }

    static void clearAll() {
        if (!ensureOpen()) return;
        QSqlQuery q(QSqlDatabase::database(CONN));
        q.exec("DELETE FROM results");
    }

    static QString starsToString(int n) {
        return QString("★").repeated(qBound(0, n, 5)) +
               QString("☆").repeated(5 - qBound(0, n, 5));
    }

    static bool isLevelUnlocked(int level) {
        if (level <= 1) return true;
        if (level == 2) {
            return getStarsInt("bucket")  >= 1 &&
                   getStarsInt("library") >= 1;
        }
        if (level == 3) {
            return getStarsInt("bee")   >= 1 &&
                   getStarsInt("space") >= 1;
        }
        return false;
    }

private:
    static constexpr const char* CONN = "diploma_db";

    static bool ensureOpen() {
        QSqlDatabase db = QSqlDatabase::database(CONN);
        if (db.isValid() && db.isOpen()) return true;
        return init();
    }
};

#endif
