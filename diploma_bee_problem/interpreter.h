#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include "command.h"

class Interpreter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList errors READ errors NOTIFY errorsChanged)
    Q_PROPERTY(QStringList warnings READ warnings NOTIFY warningsChanged)

public:
    explicit Interpreter(QObject *parent = nullptr);

    Q_INVOKABLE QList<Command*> parseScript(const QString &script);
    Q_INVOKABLE bool validateCommand(Command *cmd, const QStringList &availableBees);
    Q_INVOKABLE bool validateAllCommands(const QList<Command*> &commands);
    Q_INVOKABLE double estimateTotalTime(const QList<Command*> &commands);
    Q_INVOKABLE QStringList suggestOptimizations(const QList<Command*> &commands);

    QStringList errors() const { return m_errors; }
    QStringList warnings() const { return m_warnings; }

signals:
    void errorsChanged();
    void warningsChanged();
    void parsingProgress(int percent);

private:
    QStringList m_errors;
    QStringList m_warnings;

    Command* parseFlyCommand(const QString &line, int lineNum);
    Command* parseCollectCommand(const QString &line, int lineNum);
    Command* parseDepositCommand(const QString &line, int lineNum);
    Command* parseRestCommand(const QString &line, int lineNum);
    Command* parseWaitCommand(const QString &line, int lineNum);
    Command* parseCheckCombCommand(const QString &line, int lineNum);

    void addError(const QString &error);
    void addWarning(const QString &warning);
    void clearMessages();

    QRegularExpression m_flyRegex;
    QRegularExpression m_collectRegex;
    QRegularExpression m_depositRegex;
    QRegularExpression m_restRegex;
    QRegularExpression m_waitRegex;
    QRegularExpression m_checkCombRegex;

    void initRegexPatterns();
};

#endif
