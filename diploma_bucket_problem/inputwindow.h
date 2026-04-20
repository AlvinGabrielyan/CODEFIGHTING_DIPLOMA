#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QPair>
#include <QQueue>
#include <QVector>

class InputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InputWindow(QWidget *parent = nullptr);

    // Results after accepted
    int capA() const { return m_capA; }
    int capB() const { return m_capB; }
    int target() const { return m_target; }

private slots:
    void onCheckClicked();

private:
    QLineEdit *m_edtA, *m_edtB, *m_edtT;
    QLabel    *m_errorLabel;
    QPushButton *m_btnCheck;

    int m_capA = 0, m_capB = 0, m_target = 0;

    // BFS reachability check (Huffman runs internally, result not shown)
    bool canReach(int capA, int capB, int target);
};

#endif
