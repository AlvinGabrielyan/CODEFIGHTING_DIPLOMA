#include "interpreter.h"
#include <QRegularExpression>

Interpreter::Interpreter() {}

//Դիտարկում ենք հրամանների կատարման հնարավոր ելքերը՝ ճիշտ և սխալ հրամանների դեպքում հաղորդագրությունների տպում հաղորդագրությունների ցանկում
bool Interpreter::executeCommand(const QString& command, LibrarianState& state, QString& output) {
    QString cmd = command.trimmed().toUpper();

    if (cmd.startsWith("MOVE")) {
        QString from, to;
        if (!parseMoveCommand(cmd, from, to)) {
            output = "Սխալ MOVE հրաման";
            return false;
        }

        if (state.librarianRoom != from) {
            output = QString("Դուք գտնվում եք %1 սենյակում, ոչ թե %2-ում։ Ուշադիր եղեք։")
                         .arg(state.librarianRoom, from);
            return false;
        }

        if (!state.canMove(from, to)) {
            output = QString("Չի կարող անցնել %1 → %2").arg(from, to);
            return false;
        }

        state.moveToRoom(to);
        output = QString("✓ Շարժվեց %1 → %2").arg(from, to);
        return true;
    }

    if (cmd.startsWith("PICK")) {
        int bookId;
        if (!parsePickCommand(cmd, bookId)) {
            output = "Սխալ PICK հրաման";
            return false;
        }

        if (!state.canPickBook(bookId)) {
            output = QString("Չի կարող վերցնել %1 գիրքը").arg(bookId);
            return false;
        }

        state.pickBook(bookId);
        output = QString("✓ Վերցրեց %1 գիրքը").arg(bookId);
        return true;
    }

    if (cmd.startsWith("PLACE")) {
        int bookId;
        if (!parsePlaceCommand(cmd, bookId)) {
            output = "Սխալ PLACE հրաման";
            return false;
        }

        if (!state.canPlaceBook(bookId)) {
            output = QString("Չի կարող դնել %1 գիրքը").arg(bookId);
            return false;
        }

        state.placeBook(bookId);
        output = QString("✓ Դրեց գիրք %1").arg(bookId);
        return true;
    }

    output = "Անհայտ հրաման";
    return false;
}

//Սահմանված հրամանների PARSE, հրամանի ինչպիսի սահմանման դեպքում կիրականանա այն
bool Interpreter::parseMoveCommand(const QString& cmd, QString& from, QString& to) {
    // Ընդունել և՛ -> և՛ → նշանները
    QRegularExpression re("MOVE\\s*\\(\\s*([A-Z])\\s*(?:->|→|―>|=>|- >)\\s*([A-Z])\\s*\\)");
    QRegularExpressionMatch match = re.match(cmd);

    if (match.hasMatch()) {
        from = match.captured(1);
        to = match.captured(2);
        return true;
    }
    return false;
}

bool Interpreter::parsePickCommand(const QString& cmd, int& bookId) {
    QRegularExpression re("PICK\\s*\\(\\s*(\\d+)\\s*\\)");
    QRegularExpressionMatch match = re.match(cmd);

    if (match.hasMatch()) {
        bookId = match.captured(1).toInt();
        return true;
    }
    return false;
}

bool Interpreter::parsePlaceCommand(const QString& cmd, int& bookId) {
    QRegularExpression re("PLACE\\s*\\(\\s*(\\d+)\\s*\\)");
    QRegularExpressionMatch match = re.match(cmd);

    if (match.hasMatch()) {
        bookId = match.captured(1).toInt();
        return true;
    }
    return false;
}
