#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>
#include "librarian_state.h"

class Interpreter {
public:
    Interpreter();

    bool executeCommand(const QString& command, LibrarianState& state, QString& output);

private:
    bool parseMoveCommand(const QString& cmd, QString& from, QString& to);
    bool parsePickCommand(const QString& cmd, int& bookId);
    bool parsePlaceCommand(const QString& cmd, int& bookId);
};

#endif
