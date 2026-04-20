#ifndef LIBRARIAN_STATE_H
#define LIBRARIAN_STATE_H
#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QPair>

struct Book {
    int id;
    QString startRoom;
    QString targetRoom;
    QString currentRoom;
    bool delivered;

    Book() : id(0), delivered(false) {}
    Book(int i, QString s, QString t)
        : id(i), startRoom(s), targetRoom(t), currentRoom(s), delivered(false) {}

};


class LibrarianState {
public:

    LibrarianState();
    QString librarianRoom;
    QVector<int> booksInHand;
    QMap<int, Book> books;
    QSet<int> deliveredBooks;
    int totalEnergy;
    QVector<QString> actionHistory;
    QMap<QString, QVector<QString>> graph;
    QSet<QPair<QString, QString>> emptyHandDoors;
    void initialize();
    bool canMove(const QString& from, const QString& to) const;
    bool canPickBook(int bookId) const;
    bool canPlaceBook(int bookId) const;
    void pickBook(int bookId);
    void placeBook(int bookId);
    void moveToRoom(const QString& room);
    bool isComplete() const;
    QString getStateString() const;
    QVector<Book> getBooksInRoom(const QString& room) const;

private:
    void initializeBooks();
    void initializeGraph();
    void initializeDoors();
};


inline uint qHash(const QPair<QString, QString> &key, uint seed = 0) {
    return qHash(key.first, seed) ^ qHash(key.second, seed + 1);
}
#endif
