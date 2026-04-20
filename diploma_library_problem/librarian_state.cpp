#include "librarian_state.h"

LibrarianState::LibrarianState()
    : librarianRoom("S"), totalEnergy(0) {
    initialize();
}

void LibrarianState::initialize() {
    librarianRoom = "S";
    booksInHand.clear();
    deliveredBooks.clear();
    totalEnergy = 0;
    actionHistory.clear();
    initializeBooks();
    initializeGraph();
    initializeDoors();
}

//Սահմանում ենք սկզբնական ու նպատակային սենյակները
        void LibrarianState::initializeBooks() {

            books.clear();
            books[1] = Book(1, "S", "A");
            books[2] = Book(2, "S", "B");
            books[3] = Book(3, "A", "C");
            books[4] = Book(4, "C", "B");
            books[5] = Book(5, "S", "C");
            books[6] = Book(6, "B", "A");
            books[7] = Book(7, "S", "B");
            books[8] = Book(8, "C", "A");
            books[9] = Book(9, "S", "C");
        }

//կապում ենք համապատասխան սենյակները միմյանց՝ցույց տալով, որ նրանց միջև անցում կա
        void LibrarianState::initializeGraph() {

            graph.clear();
            graph["S"] = {"A", "B", "C"};
            graph["A"] = {"S", "C"};
            graph["B"] = {"S", "C"};
            graph["C"] = {"A", "B", "S"};
        }

//Սահմանում ենք միայն դատարկ ձեռքերով անցնող սենյակները
        void LibrarianState::initializeDoors() {

            emptyHandDoors.clear();
            emptyHandDoors.insert({"S", "B"});
            emptyHandDoors.insert({"B", "S"});
            emptyHandDoors.insert({"A", "C"});
            emptyHandDoors.insert({"C", "A"});
        }

//Ստուգում ենք, արդյոք կա անցում երկու սենյակների միջև՝ runtime
        bool LibrarianState::canMove(const QString& from, const QString& to) const {
            if (!graph.contains(from) || !graph[from].contains(to)) {
                return false;
            }

            QPair<QString, QString> door = {from, to};
            if (emptyHandDoors.contains(door)) {
                return booksInHand.isEmpty();
            }

            return true;
        }

//ստուգում ենք որ պայմանների դեպքում կարող է աշխատել PICK հրամանը և որ պայմանների դեպքում՝ոչ
        bool LibrarianState::canPickBook(int bookId) const {

            if (!books.contains(bookId)) return false;
            if (booksInHand.size() >= 2) return false;
            if (booksInHand.contains(bookId)) return false;
            if (deliveredBooks.contains(bookId)) return false;

            const Book& book = books[bookId];
            return book.currentRoom == librarianRoom;
        }


//Ստուգում ենք, եթե ձեռքին կա գիրք և դա պետք է դրվի համապատասխան սենյակում, դնում ենք
        bool LibrarianState::canPlaceBook(int bookId) const {
            return booksInHand.contains(bookId);
        }

//Սահմանում ենք, որ PICK, PLACE հրամանների դեպքում 2 էներգիա ավելանա, իսկ MOVE-ի դեպքում՝ 1։
        void LibrarianState::pickBook(int bookId) {

            if (!canPickBook(bookId)) return;
            booksInHand.append(bookId);
            totalEnergy += 2;
            QString action = QString("PICK(%1) [%2] - Էներգիա: +2 = %3")
                                 .arg(bookId).arg(librarianRoom).arg(totalEnergy);
            actionHistory.append(action);
        }


        void LibrarianState::placeBook(int bookId) {

            if (!canPlaceBook(bookId)) return;
            booksInHand.removeOne(bookId);
            books[bookId].currentRoom = librarianRoom;
            totalEnergy += 2;
            bool isDelivered = (books[bookId].targetRoom == librarianRoom);

            if (isDelivered) {
                books[bookId].delivered = true;
                deliveredBooks.insert(bookId);
            }

            QString action = QString("PLACE(%1) [%2]%3 - Էներգիա: +2 = %4")
                                 .arg(bookId)
                                 .arg(librarianRoom)
                                 .arg(isDelivered ? " ✓ ՀԱՍՑՎԵՑ" : "")
                                 .arg(totalEnergy);
            actionHistory.append(action);
        }

        void LibrarianState::moveToRoom(const QString& room) {

            if (!canMove(librarianRoom, room)) return;
            QString fromRoom = librarianRoom;
            librarianRoom = room;
            totalEnergy += 1;

            QString action = QString("MOVE(%1 → %2) - Էներգիա: +1 = %3")
                                 .arg(fromRoom).arg(room).arg(totalEnergy);
            actionHistory.append(action);
        }

//Ստուգում ենք, արդյո՞ք բոլոր հրքերը հասցված են
        bool LibrarianState::isComplete() const {
            return deliveredBooks.size() == books.size();
        }
//Տպում ենք էդ պահին որտեղ ա գտնվում, քանի գիրք կա և այլն
        QString LibrarianState::getStateString() const {
            QString state = QString("Սենյակ: %1 | Ձեռքին: %2 գիրք | Էներգիա: %3 | Հասցված: %4/%5")
                                .arg(librarianRoom)
                                .arg(booksInHand.size())
                                .arg(totalEnergy)
                                .arg(deliveredBooks.size())
                                .arg(books.size());
            return state;
        }

QVector<Book> LibrarianState::getBooksInRoom(const QString& room) const {

    QVector<Book> result;
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->currentRoom == room && !deliveredBooks.contains(it->id)) {
            result.append(*it);
        }
    }
    return result;
}
