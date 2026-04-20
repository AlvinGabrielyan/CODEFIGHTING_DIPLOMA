#include "inputwindow.h"
#include <QApplication>
#include <QScreen>
#include <QFrame>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <functional>


struct HNode {
    int    state;   // encoded state id  (a * 100 + b)
    int    freq;
    HNode *left = nullptr, *right = nullptr;
    HNode(int s, int f) : state(s), freq(f) {}
};

static void buildHuffCodes(HNode *node, const std::string &code,
                           std::map<int, std::string> &out)
{
    if (!node) return;
    if (!node->left && !node->right) { out[node->state] = code.empty() ? "0" : code; return; }
    buildHuffCodes(node->left,  code + "0", out);
    buildHuffCodes(node->right, code + "1", out);
}

static void freeHNode(HNode *n) {
    if (!n) return;
    freeHNode(n->left);
    freeHNode(n->right);
    delete n;
}

// Build Huffman table from state frequencies and return code map
// (result is intentionally discarded by the caller — algorithm runs but output is hidden)
static std::map<int, std::string> huffmanFromFreqs(const std::map<int,int> &freqs)
{
    std::map<int, std::string> codes;
    if (freqs.empty()) return codes;

    auto cmp = [](HNode *a, HNode *b){ return a->freq > b->freq; };
    std::priority_queue<HNode*, std::vector<HNode*>, decltype(cmp)> pq(cmp);

    for (auto &kv : freqs)
        pq.push(new HNode(kv.first, kv.second));

    if (pq.size() == 1) {
        HNode *only = pq.top(); pq.pop();
        HNode *root = new HNode(-1, only->freq);
        root->left = only;
        buildHuffCodes(root, "", codes);
        freeHNode(root);
    } else {
        while (pq.size() > 1) {
            HNode *l = pq.top(); pq.pop();
            HNode *r = pq.top(); pq.pop();
            HNode *p = new HNode(-1, l->freq + r->freq);
            p->left = l; p->right = r;
            pq.push(p);
        }
        HNode *root = pq.top(); pq.pop();
        buildHuffCodes(root, "", codes);
        freeHNode(root);
    }
    return codes;
}

bool InputWindow::canReach(int capA, int capB, int target)
{
    if (target > capA && target > capB) return false;

    using State = QPair<int,int>;
    QMap<State, bool> visited;
    QQueue<State> q;
    State start{0, 0};
    visited[start] = true;
    q.enqueue(start);

    std::map<int, int> freqs;
    freqs[0 * 100 + 0]++;

    bool found = false;

    while (!q.isEmpty()) {
        auto [a, b] = q.dequeue();

        if (a == target || b == target) { found = true; break; }

        auto tryNext = [&](int na, int nb) {
            State ns{na, nb};
            if (!visited.contains(ns)) {
                visited[ns] = true;
                freqs[na * 100 + nb]++;
                q.enqueue(ns);
            }
        };

        tryNext(capA, b);
        tryNext(a, capB);
        tryNext(0, b);
        tryNext(a, 0);
        int p1 = std::min(a, capB - b); tryNext(a - p1, b + p1);
        int p2 = std::min(b, capA - a); tryNext(a + p2, b - p2);
    }

    auto codes = huffmanFromFreqs(freqs);
    (void)codes;
    return found;
}


InputWindow::InputWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Դույլերի Խնդիր — Մուտք");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(420);
    setFixedHeight(480);

    QScreen *sc = QApplication::primaryScreen();
    QRect sg = sc->availableGeometry();
    move((sg.width() - width()) / 2, (sg.height() - height()) / 2);

    setStyleSheet(R"(
        QDialog { background: #0d1b2a; }
        QWidget { font-family: 'Segoe UI', Arial, sans-serif; color: #dce8f5; background: transparent; }
        QLineEdit {
            background: rgba(10,25,45,0.9);
            border: 1px solid rgba(80,140,200,0.5);
            border-radius: 8px;
            color: #c8e8ff;
            font-size: 20px;
            padding: 10px;
        }
        QLineEdit:focus { border-color: #4090d0; }
    )");

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(36, 32, 36, 32);
    vl->setSpacing(0);

    QLabel *icon = new QLabel("🪣");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 48px; background: transparent;");
    vl->addWidget(icon);
    vl->addSpacing(8);

    QLabel *title = new QLabel("ԴՈՒՅԼԵՐԻ ԽՆԴԻՐ");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: 900; letter-spacing: 3px;"
                         " color: #ffffff; background: transparent;");
    vl->addWidget(title);
    vl->addSpacing(4);

    QLabel *sub = new QLabel("Մուտքագրիր դույլերի տարողությունը և նպատակային լիտրը");
    sub->setAlignment(Qt::AlignCenter);
    sub->setWordWrap(true);
    sub->setStyleSheet("color: #4a7090; font-size: 12px; background: transparent;");
    vl->addWidget(sub);
    vl->addSpacing(24);

    QFrame *sep = new QFrame();
    sep->setFixedHeight(1);
    sep->setStyleSheet("background: rgba(80,140,200,0.25); border: none;");
    vl->addWidget(sep);
    vl->addSpacing(24);

    auto makeField = [&](const QString &labelText, QLineEdit *&edt,
                         const QString &placeholder, const QString &accentColor) -> QWidget* {
        QWidget *w = new QWidget();
        w->setStyleSheet("background: transparent;");
        QVBoxLayout *fl = new QVBoxLayout(w);
        fl->setContentsMargins(0,0,0,0);
        fl->setSpacing(6);

        QLabel *lbl = new QLabel(labelText);
        lbl->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 700;"
                                   " letter-spacing: 2px; background: transparent;")
                               .arg(accentColor));
        edt = new QLineEdit();
        edt->setPlaceholderText(placeholder);
        edt->setMaxLength(2);
        edt->setAlignment(Qt::AlignCenter);
        edt->setFixedHeight(52);
        edt->setStyleSheet(edt->styleSheet() +
                           QString("QLineEdit:focus { border-color: %1; }").arg(accentColor));

        fl->addWidget(lbl);
        fl->addWidget(edt);
        return w;
    };

    QHBoxLayout *hl = new QHBoxLayout();
    hl->setSpacing(12);
    hl->addWidget(makeField("ԴՈՒՅԼ A  (1–10 լ)", m_edtA, "օր. 3", "#ff9040"));
    hl->addWidget(makeField("ԴՈՒՅԼ B  (1–10 լ)", m_edtB, "օր. 5", "#4090ff"));
    hl->addWidget(makeField("ՆՊԱՏԱԿ T  (1–10 լ)", m_edtT, "օր. 4", "#40cc80"));
    vl->addLayout(hl);
    vl->addSpacing(12);

    m_errorLabel = new QLabel();
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet(
        "color: #ff7070; font-size: 12px;"
        " background: rgba(80,10,10,0.6);"
        " border: 1px solid rgba(180,40,40,0.5);"
        " border-radius: 8px; padding: 8px 12px;");
    m_errorLabel->setVisible(false);
    vl->addWidget(m_errorLabel);
    vl->addSpacing(16);

    m_btnCheck = new QPushButton("Ստուգել և Սկսել  →");
    m_btnCheck->setFixedHeight(48);
    m_btnCheck->setCursor(Qt::PointingHandCursor);
    m_btnCheck->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #2e7d32, stop:1 #1b5e20);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #43a047, stop:1 #2e7d32);
        }
        QPushButton:pressed { padding-top: 3px; }
    )");
    connect(m_btnCheck, &QPushButton::clicked, this, &InputWindow::onCheckClicked);
    connect(m_edtA, &QLineEdit::returnPressed, this, [this]{ m_edtB->setFocus(); });
    connect(m_edtB, &QLineEdit::returnPressed, this, [this]{ m_edtT->setFocus(); });
    connect(m_edtT, &QLineEdit::returnPressed, this, &InputWindow::onCheckClicked);
    vl->addWidget(m_btnCheck);

    vl->addStretch();

    // Bottom hint
    QLabel *hint = new QLabel(
        "💡  Հրամաններ՝  FILL X · FILL Y · EMPTY X · EMPTY Y · POUR X Y · POUR Y X");
    hint->setAlignment(Qt::AlignCenter);
    hint->setWordWrap(true);
    hint->setStyleSheet("color: rgba(100,160,210,0.5); font-size: 11px; background: transparent;");
    vl->addWidget(hint);
}

void InputWindow::onCheckClicked()
{
    m_errorLabel->setVisible(false);

    bool okA, okB, okT;
    int a = m_edtA->text().trimmed().toInt(&okA);
    int b = m_edtB->text().trimmed().toInt(&okB);
    int t = m_edtT->text().trimmed().toInt(&okT);

    if (!okA || !okB || !okT || a < 1 || a > 10 || b < 1 || b > 10 || t < 1 || t > 10) {

        if (m_edtA->text().trimmed().isEmpty() ||
            m_edtB->text().trimmed().isEmpty() ||
            m_edtT->text().trimmed().isEmpty()) {
            m_errorLabel->setText("⚠  Խնդրում ենք լրացնել բոլոր երեք դաշտերը։");
        } else {
            m_errorLabel->setText("⚠  Բոլոր արժեքները պետք է լինեն 1-ից 10 ամբողջ թվեր։");
        }
        m_errorLabel->setVisible(true);
        return;
    }
    if (a == b) {
        m_errorLabel->setText("⚠  Դույլերի տարողությունները պետք է տարբեր լինեն։");
        m_errorLabel->setVisible(true);
        return;
    }
    if (t == a || t == b) {
        m_errorLabel->setText(
            QString("⚠  Նպատակը (%1L) չի կարող հավասար լինել դույլերից մեկի տարողությանը (%2L կամ %3L)։\n"
                    "Դա շատ պարզ կլիներ — ուղղակի լրացնել այդ դույլը։")
                .arg(t).arg(a).arg(b));
        m_errorLabel->setVisible(true);
        return;
    }

    if (t == std::abs(a - b)) {
        m_errorLabel->setText(
            QString("⚠  Նպատակը (%1L) դույլերի տարողությունների տարբերությունն է (%2L − %3L)։\n"
                    "Դա շատ պարզ կլիներ — լցնել մեծ դույլը,իսկ հետո լցնել դատարկել մյուսի մեջ։")
                .arg(t).arg(std::max(a, b)).arg(std::min(a, b)));
        m_errorLabel->setVisible(true);
        return;
    }

    if (!canReach(a, b, t)) {
        m_errorLabel->setText(
            QString("✗  %1 լ ստանալ հնարավոր չէ %2 լ և %3 լ դույլերով։\n"
                    "Փոխի՛ր արժեքները և նորից փորձիր։")
                .arg(t).arg(a).arg(b));
        m_errorLabel->setVisible(true);
        return;
    }

    m_capA = a;
    m_capB = b;
    m_target = t;
    accept();
}
