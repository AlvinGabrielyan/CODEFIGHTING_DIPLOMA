#include "mainwindow.h"
#include "resultsaver.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QSizePolicy>
#include <QResizeEvent>
#include <QShowEvent>
#include <QMessageBox>
#include <QDialog>
#include <QApplication>
#include <QStyle>
#include <cmath>

MazeScene::MazeScene(MazeLogic* l, QObject* parent)
    : QGraphicsScene(parent), logic(l)
{
    int px = CS * N + WT;
    setSceneRect(0, 0, px, px);
    setBackgroundBrush(QColor(14, 14, 28));
}

void MazeScene::redraw() {
    clear();
    drawFloor();
    drawSpecial();
    drawAllWalls();
    // Draw robot: always show during victory animation, hide only on non-victory game over
    if (!logic->isGameOver() || logic->isExitReached())
        drawRobot();
}

void MazeScene::drawFloor() {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            bool even = (r+c)%2==0;
            QColor col = even ? QColor(42,42,66) : QColor(35,35,55);
            // Slight stone texture: darker corners
            QGraphicsRectItem* tile = addRect(c*CS, r*CS, CS, CS, QPen(Qt::NoPen), QBrush(col));
            tile->setZValue(0);

            // very faint grid line
            addRect(c*CS, r*CS, CS, CS,
                    QPen(QColor(55,52,78,120),1), QBrush(Qt::NoBrush))->setZValue(1);

            // small coordinate label
            auto* lbl = addText(QString("%1,%2").arg(r+1).arg(c+1));
            lbl->setFont(QFont("Arial",6));
            lbl->setDefaultTextColor(QColor(80,78,105));
            lbl->setPos(c*CS+3, r*CS+1);
            lbl->setZValue(2);
        }
    }
}

// special cells (start, exit)
void MazeScene::drawSpecial() {
    auto mark = [&](Position pos, QColor glow, const QString& emoji) {
        int x = pos.col*CS, y = pos.row*CS;
        // Glow rect
        addRect(x+3, y+3, CS-6, CS-6,
                QPen(glow.lighter(160), 2), QBrush(glow.darker(250)))->setZValue(3);
        // Emoji
        auto* t = addText(emoji);
        t->setFont(QFont("Segoe UI Emoji", 22));
        t->setPos(x + CS/2 - 14, y + CS/2 - 15);
        t->setZValue(4);
    };
    mark(logic->getStartPos(), QColor(60,180,100), "🏁");
    mark(logic->getExitPos(),  QColor(220,170,30), "🚪");
}


// Returns currentDir for moving hWall row (0 if not moving)
static int movingHShift(MazeLogic* logic, int wallRow) {
    return logic->movingHDir(wallRow);
}
// Returns currentDir for moving vWall col (0 if not moving)
static int movingVShift(MazeLogic* logic, int wallCol) {
    return logic->movingVDir(wallCol);
}

void MazeScene::drawHWall(int wallRow, int col, bool moving) {
    int x1 = col * CS;
    int x2 = x1 + CS;
    int y  = wallRow * CS;

    bool overlap = moving && logic->isOverlap(true, wallRow, col);

    QColor base, hi, shadow;
    if (overlap) {
        // Double wall — bright magenta/electric
        base   = QColor(220,  40, 220);
        hi     = QColor(255, 150, 255);
        shadow = QColor( 80,   0,  80);
    } else if (moving) {
        base   = QColor(200,  60,  40);
        hi     = QColor(255, 140, 100);
        shadow = QColor(100,  20,  10);
    } else {
        base   = QColor( 70,  60,  85);
        hi     = QColor(115, 105, 135);
        shadow = QColor( 30,  25,  40);
    }

    auto* s = addLine(x1, y+2, x2, y+2, QPen(shadow, WT+2, Qt::SolidLine, Qt::FlatCap));
    s->setZValue(8);
    auto* b = addLine(x1, y, x2, y, QPen(base, WT, Qt::SolidLine, Qt::FlatCap));
    b->setZValue(9);
    auto* h = addLine(x1, y-1, x2, y-1, QPen(hi, 2, Qt::SolidLine, Qt::FlatCap));
    h->setZValue(10);

    if (!moving) {
        for (int jx = x1+CS/3; jx < x2; jx += CS/3) {
            auto* j = addLine(jx, y-3, jx, y+3, QPen(shadow.lighter(120), 2));
            j->setZValue(11);
        }
    } else if (overlap) {
        // Overlap: draw double line + ✕ symbol
        auto* b2 = addLine(x1, y-3, x2, y-3, QPen(hi, 2, Qt::SolidLine, Qt::FlatCap));
        b2->setZValue(10);
        int mx = (x1+x2)/2;
        auto* lbl = addText("✕✕");
        lbl->setFont(QFont("Arial", 7, QFont::Bold));
        lbl->setDefaultTextColor(QColor(255, 255, 80));
        lbl->setPos(mx-12, y-13);
        lbl->setZValue(12);
    } else {
        // Arrow indicator: show this segment's movement direction
        int shift = logic->segmentDir(true, wallRow, col);
        QString arrow = (shift >= 0) ? "→" : "←";
        int mx = (x1+x2)/2;
        auto* arr = addText(arrow);
        arr->setFont(QFont("Arial", 9, QFont::Bold));
        arr->setDefaultTextColor(QColor(255, 220, 80));
        arr->setPos(mx-7, y-13);
        arr->setZValue(12);
    }
}

void MazeScene::drawVWall(int row, int wallCol, bool moving) {
    int y1 = row * CS;
    int y2 = y1 + CS;
    int x  = wallCol * CS;

    bool overlap = moving && logic->isOverlap(false, wallCol, row);

    QColor base, hi, shadow;
    if (overlap) {
        base   = QColor(220,  40, 220);
        hi     = QColor(255, 150, 255);
        shadow = QColor( 80,   0,  80);
    } else if (moving) {
        base   = QColor(200,  60,  40);
        hi     = QColor(255, 140, 100);
        shadow = QColor(100,  20,  10);
    } else {
        base   = QColor( 70,  60,  85);
        hi     = QColor(115, 105, 135);
        shadow = QColor( 30,  25,  40);
    }

    auto* s = addLine(x+2, y1, x+2, y2, QPen(shadow, WT+2, Qt::SolidLine, Qt::FlatCap));
    s->setZValue(8);
    auto* b = addLine(x, y1, x, y2, QPen(base, WT, Qt::SolidLine, Qt::FlatCap));
    b->setZValue(9);
    auto* h = addLine(x-1, y1, x-1, y2, QPen(hi, 2, Qt::SolidLine, Qt::FlatCap));
    h->setZValue(10);

    if (!moving) {
        for (int jy = y1+CS/3; jy < y2; jy += CS/3) {
            auto* j = addLine(x-3, jy, x+3, jy, QPen(shadow.lighter(120), 2));
            j->setZValue(11);
        }
    } else if (overlap) {
        // Overlap: draw double line + ✕ symbol
        auto* b2 = addLine(x-3, y1, x-3, y2, QPen(hi, 2, Qt::SolidLine, Qt::FlatCap));
        b2->setZValue(10);
        int my = (y1+y2)/2;
        auto* lbl = addText("✕");
        lbl->setFont(QFont("Arial", 7, QFont::Bold));
        lbl->setDefaultTextColor(QColor(255, 255, 80));
        lbl->setPos(x+3, my-8);
        lbl->setZValue(12);
    } else {
        // Arrow indicator: show this segment's movement direction
        int shift = logic->segmentDir(false, wallCol, row);
        QString arrow = (shift >= 0) ? "↓" : "↑";
        int my = (y1+y2)/2;
        auto* arr = addText(arrow);
        arr->setFont(QFont("Arial", 9, QFont::Bold));
        arr->setDefaultTextColor(QColor(255, 220, 80));
        arr->setPos(x+3, my-10);
        arr->setZValue(12);
    }
}

void MazeScene::drawAllWalls() {
    for (int wr = 0; wr <= N; wr++) {
        for (int c = 0; c < N; c++) {
            if (logic->hWall[wr][c]) {
                // moving only if a segment specifically occupies this cell
                bool moving = (logic->segmentDir(true, wr, c) != 0);
                drawHWall(wr, c, moving);
            }
        }
    }
    for (int r = 0; r < N; r++) {
        for (int wc = 0; wc <= N; wc++) {
            if (logic->vWall[r][wc]) {
                // moving only if a segment specifically occupies this cell
                bool moving = (logic->segmentDir(false, wc, r) != 0);
                drawVWall(r, wc, moving);
            }
        }
    }

    // Corner caps (where walls meet)
    QColor capCol(50, 45, 65);
    for (int wr = 0; wr <= N; wr++) {
        for (int wc = 0; wc <= N; wc++) {
            // Draw a small square at every corner junction
            addRect(wc*CS - WT/2, wr*CS - WT/2, WT, WT,
                    QPen(Qt::NoPen), QBrush(capCol))->setZValue(13);
        }
    }
}

// ── Robot: full stick figure
void MazeScene::drawRobot() {
    Position pos = logic->getRobotPos();
    int frame    = logic->victoryFrame;
    bool victory = logic->isExitReached();

    qreal cx, cy;

    if (victory && frame > 0) {
        // Walk out to the right of the maze, then jump
        int exitCol = logic->getExitPos().col;
        int exitRow = logic->getExitPos().row;

        // Phase 1 (frames 1-8): walk from exit cell rightward off-screen
        // Phase 2 (frames 9-18): stand outside and celebrate (jump + arms up)
        qreal walkFrac = qMin(frame, 8) / 8.0;
        cx = exitCol * CS + CS/2.0 + walkFrac * (CS * 1.8);
        cy = exitRow * CS + CS/2.0;

        // Jump bounce in phase 2
        if (frame > 8) {
            int jf = frame - 8;          // 1..10
            qreal t = jf / 5.0;          // 0..2
            // parabola: y offset = -A * t*(2-t)
            cy += -28.0 * t * (2.0 - t); // arc up then down
        }

        drawStickFigure(cx, cy, Direction::RIGHT, frame);
    } else {
        cx = pos.col * CS + CS / 2.0;
        cy = pos.row * CS + CS / 2.0;
        drawStickFigure(cx, cy, logic->getRobotDir(), 0);
    }
}

void MazeScene::drawStickFigure(qreal cx, qreal cy, Direction dir, int victoryFrame) {
    qreal sc = CS / 80.0;

    const QColor skinColor (255, 210, 140);
    const QColor hairColor ( 60,  40,  20);
    const QColor shirtColor( 50, 130, 240);
    const QColor pantsColor( 35,  75, 155);
    const QColor shoeColor ( 25,  25,  30);
    const QColor outlineCol( 15,  15,  20);
    const QColor arrowCol  (255, 230,  50);

    bool celebrating = (victoryFrame > 8);

    // In celebration: face right, arms up
    qreal angle = 0;
    if (!celebrating) {
        switch (dir) {
        case Direction::RIGHT: angle =   0; break;
        case Direction::DOWN:  angle =  90; break;
        case Direction::LEFT:  angle = 180; break;
        case Direction::UP:    angle = -90; break;
        }
    }

    QTransform tf;
    tf.translate(cx, cy);
    tf.rotate(angle);

    auto addP = [&](QPainterPath& path, QPen pen, QBrush brush, int z) {
        auto* it = addPath(path, pen, brush);
        it->setTransform(tf);
        it->setZValue(z);
    };
    auto addL = [&](qreal x1,qreal y1,qreal x2,qreal y2, QPen pen, int z) {
        auto* it = addLine(x1,y1,x2,y2, pen);
        it->setTransform(tf);
        it->setZValue(z);
    };
    auto addE = [&](qreal x,qreal y,qreal w,qreal h, QPen pen, QBrush br, int z) {
        auto* it = addEllipse(x,y,w,h,pen,br);
        it->setTransform(tf);
        it->setZValue(z);
    };

    // ── Shadow ───────────────────────────────────────────────────────────────
    if (!celebrating) {
        addE(-14*sc, 22*sc, 28*sc, 8*sc, QPen(Qt::NoPen), QBrush(QColor(0,0,0,60)), 7);
    }

    // ── Legs ─────────────────────────────────────────────────────────────────
    if (celebrating) {
        // Legs apart, slightly bent — jumping pose
        for (int side : {-1, 1}) {
            QPainterPath leg;
            leg.moveTo(side*5*sc, 8*sc);
            leg.quadTo(side*14*sc, 16*sc, side*10*sc, 28*sc);
            addP(leg, QPen(pantsColor, 7*sc, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
                 QBrush(Qt::NoBrush), 8);
            addL(side*10*sc, 28*sc, side*16*sc, 30*sc,
                 QPen(shoeColor, 5*sc, Qt::SolidLine, Qt::RoundCap), 9);
        }
    } else {
        for (int side : {-1, 1}) {
            QPainterPath leg;
            leg.moveTo(side*5*sc, 8*sc);
            leg.quadTo(side*9*sc, 18*sc, side*7*sc, 27*sc);
            addP(leg, QPen(pantsColor, 7*sc, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
                 QBrush(Qt::NoBrush), 8);
            addL(side*7*sc, 27*sc, side*13*sc, 29*sc,
                 QPen(shoeColor, 5*sc, Qt::SolidLine, Qt::RoundCap), 9);
        }
    }

    // ── Torso ─────────────────────────────────────────────────────────────────
    QPainterPath torso;
    torso.moveTo(-10*sc, -8*sc);
    torso.lineTo(-11*sc,  8*sc);
    torso.lineTo( 11*sc,  8*sc);
    torso.lineTo( 10*sc, -8*sc);
    torso.closeSubpath();
    addP(torso, QPen(outlineCol, 1.5*sc), QBrush(shirtColor), 10);
    addL(-10*sc, -2*sc, 10*sc, -2*sc, QPen(shirtColor.lighter(145), 1.5*sc), 11);

    // ── Arms ──────────────────────────────────────────────────────────────────
    if (celebrating) {
        // Both arms raised in V shape
        for (int side : {-1, 1}) {
            QPainterPath arm;
            arm.moveTo(side*10*sc, -6*sc);
            arm.quadTo(side*20*sc, -18*sc, side*18*sc, -28*sc);
            addP(arm, QPen(skinColor, 5*sc, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
                 QBrush(Qt::NoBrush), 9);
            addE(side*18*sc - 3*sc, -28*sc - 3*sc, 6*sc, 6*sc,
                 QPen(outlineCol, 1*sc), QBrush(skinColor), 10);
        }
    } else {
        for (int side : {-1, 1}) {
            QPainterPath arm;
            arm.moveTo(side*10*sc, -6*sc);
            arm.quadTo(side*17*sc, 0, side*15*sc, 9*sc);
            addP(arm, QPen(skinColor, 5*sc, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
                 QBrush(Qt::NoBrush), 9);
            addE(side*15*sc - 3*sc, 9*sc - 3*sc, 6*sc, 6*sc,
                 QPen(outlineCol, 1*sc), QBrush(skinColor), 10);
        }
    }

    // ── Neck ──────────────────────────────────────────────────────────────────
    addL(0, -8*sc, 0, -12*sc, QPen(skinColor, 5*sc, Qt::SolidLine, Qt::RoundCap), 10);

    // ── Head ──────────────────────────────────────────────────────────────────
    qreal hW = 11*sc, hH = 9*sc;
    addE(-hW+1*sc, -21*sc+1*sc, 2*hW, 2*hH, QPen(Qt::NoPen), QBrush(QColor(0,0,0,60)), 9);
    addE(-hW, -21*sc, 2*hW, 2*hH, QPen(outlineCol, 2*sc), QBrush(skinColor), 11);

    QPainterPath hair;
    hair.moveTo(-hW, -12*sc);
    hair.arcTo(-hW, -21*sc, 2*hW, 2*hH, 0, 180);
    addP(hair, QPen(hairColor, 1*sc), QBrush(hairColor), 12);

    // Eyes
    for (int side : {-1, 1}) {
        qreal ex = side * 4*sc, ey = -14*sc;
        if (celebrating) {
            // Happy closed eyes (arcs)
            QPainterPath eye;
            eye.moveTo(ex-2.5*sc, ey-1*sc);
            eye.quadTo(ex, ey-4*sc, ex+2.5*sc, ey-1*sc);
            addP(eye, QPen(QColor(20,20,40), 2*sc, Qt::SolidLine, Qt::RoundCap),
                 QBrush(Qt::NoBrush), 13);
        } else {
            addE(ex-2*sc, ey-2*sc, 4*sc, 4.5*sc, QPen(Qt::NoPen), QBrush(QColor(20,20,40)), 13);
            addE(ex-0.5*sc, ey-1*sc, 1.5*sc, 1.5*sc, QPen(Qt::NoPen), QBrush(Qt::white), 14);
        }
    }

    // Mouth
    QPainterPath smile;
    if (celebrating) {
        // Big open smile
        smile.moveTo(-6*sc, -10*sc);
        smile.quadTo(0, -4*sc, 6*sc, -10*sc);
        addP(smile, QPen(QColor(180,60,50), 2.5*sc, Qt::SolidLine, Qt::RoundCap),
             QBrush(Qt::NoBrush), 13);
        // Teeth
        QPainterPath teeth;
        teeth.addRect(-4*sc, -9*sc, 8*sc, 2.5*sc);
        addP(teeth, QPen(Qt::NoPen), QBrush(Qt::white), 14);
    } else {
        smile.moveTo(-4*sc, -10*sc);
        smile.quadTo(0, -7*sc, 4*sc, -10*sc);
        addP(smile, QPen(QColor(180,80,60), 1.5*sc, Qt::SolidLine, Qt::RoundCap),
             QBrush(Qt::NoBrush), 13);
    }

    // ── Celebration sparkles ───────────────────────────────────────────────────
    if (celebrating) {
        // Stars / sparkles around the figure
        struct Star { qreal x, y; QColor c; };
        Star stars[] = {
                        {-30*sc, -30*sc, QColor(255,230,0)},
                        { 32*sc, -25*sc, QColor(100,255,150)},
                        {-25*sc,  10*sc, QColor(255,100,200)},
                        { 28*sc,  15*sc, QColor(100,200,255)},
                        {  0*sc, -40*sc, QColor(255,180,0)},
                        };
        for (auto& st : stars) {
            QPainterPath star;
            int pts = 5;
            for (int i = 0; i < pts*2; i++) {
                qreal a = i * M_PI / pts - M_PI/2;
                qreal r = (i%2==0) ? 7*sc : 3*sc;
                qreal sx = st.x + r*cos(a), sy = st.y + r*sin(a);
                if (i==0) star.moveTo(sx,sy); else star.lineTo(sx,sy);
            }
            star.closeSubpath();
            addP(star, QPen(Qt::NoPen), QBrush(st.c), 16);
        }

        auto* txt = addText(" ՈՒՌԱԱ! ");
        txt->setFont(QFont("Segoe UI Emoji", 12, QFont::Bold));
        txt->setDefaultTextColor(QColor(255,230,50));
        txt->setPos(cx - 55, cy - 80*sc - 15);
        txt->setZValue(20);
    }

    // ── Direction arrow (only in normal mode) ─────────────────────────────────
    if (!celebrating) {
        qreal ad = 30*sc;
        QPainterPath arrow;
        arrow.moveTo(ad,0); arrow.lineTo(ad-7*sc,-5*sc);
        arrow.moveTo(ad,0); arrow.lineTo(ad-7*sc,+5*sc);
        addP(arrow, QPen(arrowCol, 3*sc, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
             QBrush(Qt::NoBrush), 15);
        addL(20*sc,0,ad,0, QPen(arrowCol, 2.5*sc, Qt::SolidLine, Qt::RoundCap), 15);
    }
}


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    logic = new MazeLogic();
    scene = new MazeScene(logic, this);
    setupUI();
    scene->redraw();
    refreshStatus();
}

MainWindow::~MainWindow() { delete logic; }

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    if (view && scene)
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    if (view && scene)
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::setupUI() {
    auto* central = new QWidget(this);
    auto* root    = new QHBoxLayout(central);
    root->setSpacing(12);
    root->setContentsMargins(12,12,12,12);

    auto* left  = new QVBoxLayout();
    auto* right = new QVBoxLayout();
    buildLeftPanel(left);
    buildRightPanel(right);
    root->addLayout(left,  3);
    root->addLayout(right, 2);

    setCentralWidget(central);
    central->setStyleSheet(
        "QWidget{background:#0d0e1c;}"
        "QGroupBox{color:#ffd700;font-weight:bold;font-size:11pt;"
        "border:2px solid #2a2a48;border-radius:8px;margin-top:10px;padding-top:10px;}"
        "QGroupBox::title{subcontrol-origin:margin;left:10px;padding:0 5px;}"
        );
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::tick);

    victoryTimer = new QTimer(this);
    victoryTimer->setInterval(80);  // fast frames for smooth animation
    connect(victoryTimer, &QTimer::timeout, this, &MainWindow::victoryTick);
}

void MainWindow::buildLeftPanel(QVBoxLayout* l) {
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setStyleSheet("background:#0d0e1c;border:3px solid #363660;border-radius:10px;");
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setMinimumSize(200, 200);
    l->addWidget(view, 1);
    l->addWidget(mkStatusBox(), 0);
    l->addWidget(mkRulesBox(), 0);
    l->addWidget(mkStoryBox(), 0);
}

void MainWindow::buildRightPanel(QVBoxLayout* l) {
    l->addWidget(mkCmdBox(), 1);
    l->addWidget(mkCtrlBox(), 0);
    l->addWidget(mkLogBox(), 1);
}

QGroupBox* MainWindow::mkStatusBox() {
    auto* b = new QGroupBox("Ռոբոտի Վիճակ");
    auto* l = new QVBoxLayout();
    lblSteps  = new QLabel("Քայլ: 0");
    lblSteps->setStyleSheet("color:#4fc3f7;font-size:10pt;padding:3px;font-weight:bold;");
    lblPos    = new QLabel("Դիրք: (4,1)");
    lblPos->setStyleSheet("color:#81c784;font-size:10pt;padding:3px;");
    lblDir    = new QLabel("Ուղղ: → Աջ");
    lblDir->setStyleSheet("color:#ffb74d;font-size:10pt;padding:3px;");
    lblStatus = new QLabel("Պատրաստ");
    lblStatus->setStyleSheet("color:#a0ffa0;font-size:10pt;padding:6px;"
                             "background:#1a3a1a;border-radius:5px;margin:3px;");
    l->addWidget(lblSteps); l->addWidget(lblPos);
    l->addWidget(lblDir);   l->addWidget(lblStatus);
    b->setLayout(l); return b;
}

QGroupBox* MainWindow::mkRulesBox() {
    auto* b = new QGroupBox("Կանոններ");
    auto* l = new QVBoxLayout();
    auto* r = new QLabel(
        "🔸 Կարմիրով նշված պատերը շարժվում են նշված ուղղությամբ\n"
        "🔸 Ամեն FORWARD, WAIT քայլից հետո պատերը սահում են\n"
        "🔸 Պատերը չեն շարժվում ROTATE_RIGHT,ROTATE_LEFT հրամանների դեպքում\n"
        "🔸 Տողի կամ սյան ծայրակետին հասնելիս շարժվող պատերը հակառակ ուղղությամբ են գնում\n"
        "🔸 Ցանկացած պատին բախվելուց հետո համակարգը կդադարեցնի աշխատանքը\n"
        "🔸 Նպատակ։ Սկզբնական վիճակից հասնել ելքային դռանը\n"
        "🔸 Հրամանները զգայուն չեն մեծատառերի նկատմամբ"
        );
    r->setStyleSheet("color:#d0d0d0;font-size:10pt;padding:8px;line-height:1.5;");
    r->setWordWrap(true);
    l->addWidget(r); b->setLayout(l); return b;
}

QGroupBox* MainWindow::mkStoryBox() {
    auto* b = new QGroupBox();
    b->setStyleSheet("QGroupBox{border:none;margin:0;padding:0;}");
    auto* l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);

    auto* btn = new QPushButton("Խնդրի Նկարագրություն");
    btn->setStyleSheet(
        "QPushButton{"
        "  background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "    stop:0 #2a1a4a, stop:1 #1a2a4a);"
        "  color:#ffd700;"
        "  font-size:11pt;"
        "  font-weight:bold;"
        "  padding:10px 18px;"
        "  border-radius:8px;"
        "  border:2px solid #5a3a8a;"
        "  text-align:left;"
        "}"
        "QPushButton:hover{"
        "  background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "    stop:0 #3a2a5a, stop:1 #2a3a5a);"
        "  border-color:#9060cc;"
        "}"
        "QPushButton:pressed{background:#1a0a3a;}"
        );

    static const QString storyHtml =
        "<div style='color:#e8e0ff;font-size:10pt;line-height:1.8;'>"
        "<p style='font-size:14pt;color:#ffd700;font-weight:bold;margin-bottom:8px;'>"
        "Կախարդական Լաբիրինթոս</p>"

        "<p>Շրջմոլիկը հայտնվել է կախարդված լաբիրինթոսում, որի <b>պատերը կյանք ունեն</b>։ "
        "Կատարում է քայլ, և պատերի շերտերը սահում են, փոխում ուղղությունը, թաքցնում ելքը... "
        "Նա պետք է գտնի ելքը՝ <b> (4,1)-ից</b> մինչև <b> (4,7)</b>։</p>"

        "<hr style='border:1px solid #3a2a5a;margin:10px 0;'/>"

        "<p style='color:#ff9966;font-size:11pt;font-weight:bold;'>🔴 Շարժվող պատեր</p>"
        "<p style='color:#a0b0c0;font-size:9pt;margin-top:-6px;'>"
        "Համակարգի սկզբնական վիճակ։</p>"

        "<table style='width:100%;border-collapse:collapse;margin:6px 0;'>"
        "<tr style='background:#1e1040;'>"
        "  <td style='padding:5px 8px;color:#74c0fc;font-weight:bold;'>Հորիզոնական 1-ին շերտ</td>"
        "  <td style='padding:5px 8px;color:#69db7c;font-size:10pt;'>→ Աջ</td>"
        "</tr>"
        "<tr style='background:#16183a;'>"
        "  <td style='padding:5px 8px;color:#74c0fc;font-weight:bold;'>Հորիզոնական 2-րդ շերտ</td>"
        "  <td style='padding:5px 8px;color:#ff6b6b;font-size:10pt;'>← Ձախ</td>"
        "</tr>"
        "<tr style='background:#1e1040;'>"
        "  <td style='padding:5px 8px;color:#74c0fc;font-weight:bold;'>Հորիզոնական 4-րդ շերտ</td>"
        "  <td style='padding:5px 8px;color:#69db7c;font-size:10pt;'>→ Աջ</td>"
        "</tr>"
        "<tr style='background:#16183a;'>"
        "  <td style='padding:5px 8px;color:#74c0fc;font-weight:bold;'>Հորիզոնական 6-րդ շերտ</td>"
        "  <td style='padding:5px 8px;color:#69db7c;font-size:10pt;'>→ Աջ</td>"
        "</tr>"
        "<tr style='background:#1e1040;'>"
        "  <td style='padding:5px 8px;color:#c084fc;font-weight:bold;'>Ուղղահայաց 2-րդ սյուն</td>"
        "  <td style='padding:5px 8px;color:#69db7c;font-size:10pt;'>↓ Ներքև</td>"
        "</tr>"
        "<tr style='background:#16183a;'>"
        "  <td style='padding:5px 8px;color:#c084fc;font-weight:bold;'>Ուղղահայաց 5-րդ սյուն</td>"
        "  <td style='padding:5px 8px;color:#ff6b6b;font-size:10pt;'>↑ Վերև</td>"
        "</tr>"
        "</table>"

        "<hr style='border:1px solid #3a2a5a;margin:10px 0;'/>"

        "<p><b style='color:#ffd43b;'>💡 Խորհուրդ —</b> "
        "Ճիշտ ժամանակի <b style='color:#c084fc;'>WAIT</b>-ով կանգ առ։ "
        "Միգուցե հաջորդ պահին ճանապարհ բացվի, երբ պատերը շարժվեն...</p>"

        "<p style='color:#808090;font-size:8.5pt;margin-top:10px;'>"
        "Ունես 3 Վերսկսելու հնարավորություն։Նախ մտածիր — հետո գործարկիր։</p>"
        "</div>";

    connect(btn, &QPushButton::clicked, this, [this, storyHtml]() {
        auto* dlg = new QDialog(this);
        dlg->setWindowTitle(" Խնդրի Նկարագրություն");
        dlg->setMinimumSize(520, 480);
        dlg->setStyleSheet(
            "QDialog{background:#0d0e1c;}"
            "QScrollBar:vertical{background:#1a1a32;width:10px;border-radius:5px;}"
            "QScrollBar::handle:vertical{background:#5040a0;border-radius:5px;}"
            );

        auto* vl = new QVBoxLayout(dlg);
        vl->setContentsMargins(16, 16, 16, 12);

        auto* scroll = new QScrollArea();
        scroll->setWidgetResizable(true);
        scroll->setStyleSheet("QScrollArea{border:2px solid #3a2a6a;border-radius:8px;background:#111128;}");

        auto* lbl = new QLabel(storyHtml);
        lbl->setWordWrap(true);
        lbl->setTextFormat(Qt::RichText);
        lbl->setStyleSheet("background:transparent;padding:14px;");
        lbl->setOpenExternalLinks(false);

        scroll->setWidget(lbl);
        vl->addWidget(scroll, 1);

        auto* closeBtn = new QPushButton("✕  Փակել");
        closeBtn->setStyleSheet(
            "QPushButton{background:#3a1a5a;color:#e0d0ff;font-size:10pt;"
            "padding:8px 24px;border-radius:6px;border:2px solid #6040a0;font-weight:bold;}"
            "QPushButton:hover{background:#5a2a7a;}"
            );
        closeBtn->setFixedWidth(130);
        auto* br = new QHBoxLayout();
        br->addStretch(); br->addWidget(closeBtn);
        vl->addLayout(br);

        connect(closeBtn, &QPushButton::clicked, dlg, &QDialog::accept);
        dlg->exec();
    });

    l->addWidget(btn);
    b->setLayout(l);
    return b;
}

QGroupBox* MainWindow::mkCmdBox() {
    auto* b = new QGroupBox("Հրամաններ");
    auto* l = new QVBoxLayout();
    auto* h = new QLabel(
        "FORWARD կամ MOVE_FORWARD   — Առաջ\n"
        "RIGHT   կամ ROTATE_RIGHT   — Աջ պտույտ(90°)\n"
        "LEFT    կամ ROTATE_LEFT    — Ձախ պտույտ(90°)\n"
        "WAIT                       — Սպասել մեկ քայլ\n"
        "EXIT    կամ SENSE_EXIT     — Ստուգել ելքը"
        );
    h->setStyleSheet("color:#a0a0c0;font-size:9pt;padding:6px;"
                     "background:#14142a;border-radius:5px;font-family:'Courier New';");
    cmdBox = new QTextEdit();
    cmdBox->setPlaceholderText("Մուտքագրեք հրամանները (մեկ տողում մեկ հրաման):\nF\nF\nR\nF\nL\nF");
    cmdBox->setStyleSheet("background:#14142a;color:#e0e0f0;"
                          "border:2px solid #303060;font-size:10pt;"
                          "padding:7px;border-radius:5px;font-family:'Courier New';");
    cmdBox->setMinimumHeight(130);
    l->addWidget(h); l->addWidget(cmdBox);
    b->setLayout(l); return b;
}

QGroupBox* MainWindow::mkCtrlBox() {
    auto* b = new QGroupBox("Կառավարում");
    auto* l = new QVBoxLayout();

    auto mkBtn = [](const QString& txt, const QString& bg) {
        auto* btn = new QPushButton(txt);
        btn->setStyleSheet(QString(
                               "QPushButton{background:%1;color:white;font-size:11pt;"
                               "padding:10px;border-radius:6px;font-weight:bold;}"
                               "QPushButton:hover{filter:brightness(130%);opacity:0.9;}"
                               ).arg(bg));
        return btn;
    };
    btnRun   = mkBtn("Գործարկել", "#2060cc");
    btnStep  = mkBtn("Քայլ",     "#c07020");
    btnReset = mkBtn("Վերսկս. (3)", "#b03020");

    btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    btnStep->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaSkipForward));
    btnReset->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    btnRun->setIconSize(QSize(20, 20));
    btnStep->setIconSize(QSize(20, 20));
    btnReset->setIconSize(QSize(20, 20));
    connect(btnRun,  &QPushButton::clicked,this,&MainWindow::onRun);
    connect(btnStep, &QPushButton::clicked,this,&MainWindow::onStep);
    connect(btnReset,&QPushButton::clicked,this,&MainWindow::onReset);

    auto* brow = new QHBoxLayout();
    brow->addWidget(btnRun); brow->addWidget(btnStep); brow->addWidget(btnReset);
    l->addLayout(brow);
    b->setLayout(l); return b;
}

QGroupBox* MainWindow::mkLogBox() {
    auto* b = new QGroupBox("Պատմություն");
    auto* l = new QVBoxLayout();
    auto* hr = new QHBoxLayout();
    btnClear = new QPushButton("Մաքրել");
    btnClear->setStyleSheet("QPushButton{background:#506070;color:white;font-size:9pt;"
                            "padding:4px 10px;border-radius:4px;}"
                            "QPushButton:hover{background:#607080;}");
    btnClear->setMaximumWidth(90);
    connect(btnClear,&QPushButton::clicked,this,&MainWindow::onClearLog);
    hr->addStretch(); hr->addWidget(btnClear);
    logBox = new QTextEdit();
    logBox->setReadOnly(true);
    logBox->setStyleSheet("background:#0d0e1c;color:#d0d0d0;"
                          "border:2px solid #2a2a48;font-size:9pt;"
                          "padding:6px;border-radius:5px;font-family:'Consolas','Courier New';");
    logBox->setMinimumHeight(170);
    l->addLayout(hr); l->addWidget(logBox);
    b->setLayout(l); return b;
}

void MainWindow::onRun() {
    if (running) {
        timer->stop(); running = false;
        btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        btnRun->setText("Գործարկել");
        log("Դադարեցված","warning"); return;
    }
    parseInput();
    if (cmdQ.empty()) { log("Հրամաններ չկան!","error"); return; }
    running = true;
    btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaStop));
    btnRun->setText("Դադարեցնել");
    log("Սկսվում է...","success");
    timer->start(speed);
}

void MainWindow::onStep() {
    if (running) { log("Ավտո-ռեժիմ","warning"); return; }
    if (cmdQ.empty()) parseInput();
    if (!cmdQ.empty()) tick();
    else log("Հրամաններ չկան","warning");
}

void MainWindow::onReset() {
    if (logic->getResetCount() >= MazeLogic::MAX_RESETS) {
        return;
    }
    logic->incrementResetCount();
    logic->reset();
    logic->victoryFrame = 0;
    victoryTimer->stop();
    scene->redraw();
    while (!cmdQ.empty()) cmdQ.pop();
    running = false; timer->stop();
    btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    btnRun->setText("Գործարկել");
    logBox->clear();
    int remaining = MazeLogic::MAX_RESETS - logic->getResetCount();
    if (remaining == 0) {
        btnReset->setEnabled(false);
        btnReset->setText("Վերսկսել (0)");
        log(" Վերջին վերսկսումն օգտագործված է — ավելիի հնարավորություն չկա","warning");
    } else {
        btnReset->setText(QString("Վերսկսել (%1)").arg(remaining));
        log(QString(" Վերսկսված է — մնացել է %1 անգամ").arg(remaining),"warning");
    }
    refreshStatus();
}

void MainWindow::onClearLog() { logBox->clear(); }

void MainWindow::victoryTick() {
    logic->tickVictory();
    scene->redraw();
    if (logic->victoryFrame >= MazeLogic::VICTORY_FRAMES) {
        victoryTimer->stop();

        // ── Աստղերի հաշվարկ ──────────────────────────────────
        int steps  = logic->getStepCount();
        int resets = logic->getResetCount();
        int stars;
        if      (resets == 0 && steps <= 28) stars = 5;
        else if (resets == 0 && steps <= 35) stars = 4;
        else if (resets <= 1)                stars = 3;
        else if (resets <= 2)                stars = 2;
        else                                 stars = 1;

        // ── Պահպանել DB-ում ───────────────────────────────────
        ResultSaver::save("maze", stars);

        // ── Արդյունքի dialog (styled, yellow stars) ──────────
        QString grade;
        QString gradeColor;
        if      (stars == 5) { grade = "Գերազանց";  gradeColor = "#4ade80"; }
        else if (stars == 4) { grade = "Լավ";        gradeColor = "#facc15"; }
        else if (stars == 3) { grade = "Բավարար";   gradeColor = "#fb923c"; }
        else if (stars == 2) { grade = "Թույլ";      gradeColor = "#f87171"; }
        else                 { grade = "Անբավարար";      gradeColor = "#ef4444"; }

        // Build yellow star string via HTML (always renders colored)
        QString starsHtml;
        for (int i = 0; i < 5; i++)
            starsHtml += (i < stars)
                             ? "<span style='color:#facc15;font-size:28px;'>★</span>"
                             : "<span style='color:#3a3a5a;font-size:28px;'>☆</span>";

        auto* dlg = new QDialog(this);
        dlg->setWindowTitle(" Հաղթանակ!");
        dlg->setFixedSize(360, 260);
        dlg->setStyleSheet(
            "QDialog { background:#0d0d18; border-radius:16px; }"
            "QLabel  { background:transparent; }"
            "QPushButton { background:#1a1a2e; border:1px solid #3a3a5c;"
            "              border-radius:8px; color:#a0a0d0;"
            "              padding:8px 28px; font-size:14px; font-weight:600; }"
            "QPushButton:hover { background:#7c6af7; color:#fff; border-color:#7c6af7; }"
            );

        auto* lay = new QVBoxLayout(dlg);
        lay->setContentsMargins(28, 24, 28, 24);
        lay->setSpacing(10);

        auto* titleLbl = new QLabel("Ելքը գտնվեց!");
        titleLbl->setAlignment(Qt::AlignCenter);
        titleLbl->setStyleSheet("color:#ffffff;font-size:20px;font-weight:800;");
        lay->addWidget(titleLbl);

        auto* starsLbl = new QLabel(starsHtml);
        starsLbl->setAlignment(Qt::AlignCenter);
        starsLbl->setTextFormat(Qt::RichText);
        lay->addWidget(starsLbl);

        auto* gradeLbl = new QLabel(grade);
        gradeLbl->setAlignment(Qt::AlignCenter);
        gradeLbl->setStyleSheet(
            QString("color:%1;font-size:22px;font-weight:800;").arg(gradeColor));
        lay->addWidget(gradeLbl);

        auto* infoLbl = new QLabel(
            QString("<span style='color:#5a5a7a;font-size:13px;'>"
                    "Քայլ: <b style='color:#c0c0d0;'>%1</b>&nbsp;&nbsp;"
                    "Reset: <b style='color:#c0c0d0;'>%2</b></span>")
                .arg(steps).arg(resets));
        infoLbl->setAlignment(Qt::AlignCenter);
        infoLbl->setTextFormat(Qt::RichText);
        lay->addWidget(infoLbl);

        lay->addStretch();

        auto* closeBtn = new QPushButton("Փակել");
        closeBtn->setCursor(Qt::PointingHandCursor);
        connect(closeBtn, &QPushButton::clicked, dlg, &QDialog::accept);
        lay->addWidget(closeBtn, 0, Qt::AlignCenter);

        dlg->exec();
    }
}

void MainWindow::tick() {
    if (cmdQ.empty()) {
        timer->stop(); running = false;
        btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        btnRun->setText("Գործարկել");
        if (logic->isExitReached())
            log(QString(" ՀԱՋՈՂՈՒԹՅՈՒՆ! Քայլ: %1").arg(logic->getStepCount()),"success");
        else if (logic->isGameOver())
            log(" Ձախողված: " + logic->getLastError(),"error");
        else
            log(" Ավարտ","info");
        return;
    }

    Command cmd = cmdQ.front(); cmdQ.pop();
    log("▶ " + cmdToStr(cmd), "command");

    QString msg;
    bool ok = logic->executeCommand(cmd, msg);
    log(msg, ok ? "info" : "error");

    if (!ok) {
        timer->stop(); running = false;
        btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        btnRun->setText("Գործարկել");
        scene->redraw(); refreshStatus();
        if (logic->isGameOver() && logic->getResetCount() >= MazeLogic::MAX_RESETS) {
            QMessageBox::critical(this, "Ձախողում",
                                  "Վերսկսելու հնարավորությունները սպառվել են:\nԴուք Պարտվել եք։");
            QApplication::quit();
        }
        return;
    }

    if (cmd == Command::MOVE_FORWARD || cmd == Command::WAIT) {
        logic->moveWalls();
        if (logic->isGameOver()) {
            timer->stop(); running = false;
            btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
            btnRun->setText("Գործարկել");
            log("❌ " + logic->getLastError(),"error");
            if (logic->getResetCount() >= MazeLogic::MAX_RESETS) {
                QMessageBox::critical(this, "Ձախողում",
                                      "Վերսկսելու հնարավորությունները սպառվել են:\nԴուք Պարտվել եք։");
                QApplication::quit();
            }
        }
    }
    scene->redraw(); refreshStatus();

    if (logic->isExitReached() && logic->victoryFrame == 0) {
        timer->stop(); running = false;
        btnRun->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        btnRun->setText("Գործարկել");
        log(QString("ՀԱՋՈՂՈՒԹՅՈՒՆ! Քայլ: %1").arg(logic->getStepCount()),"success");
        victoryTimer->start();
    }
}

void MainWindow::parseInput() {
    while (!cmdQ.empty()) cmdQ.pop();
    for (const QString& line : cmdBox->toPlainText().split('\n',Qt::SkipEmptyParts)) {
        QString t = line.trimmed().toUpper();
        if (t.isEmpty()||t.startsWith("//")||t.startsWith("#")) continue;
        if (!isKnownCmd(t)) {
            log(QString("Անհայտ հրաման է մուտքագրված՝ \"%1\"").arg(line.trimmed()), "warning");
            continue;
        }
        cmdQ.push(strToCmd(t));
    }
    log(QString("%1 հրաման").arg(cmdQ.size()),"info");
}

bool MainWindow::isKnownCmd(const QString& s) {
    return s=="FORWARD"||s=="MOVE_FORWARD"
           ||s=="LEFT" ||s=="ROTATE_LEFT"
           ||s=="RIGHT"||s=="ROTATE_RIGHT"
           ||s=="WAIT"
           ||s=="SENSE_EXIT"||s=="EXIT";
}

Command MainWindow::strToCmd(const QString& s) {
    if (s=="FORWARD"||s=="MOVE_FORWARD")
        return Command::MOVE_FORWARD;
    if (s=="LEFT"||s=="ROTATE_LEFT")
        return Command::TURN_LEFT;
    if (s=="RIGHT"||s=="ROTATE_RIGHT")
        return Command::TURN_RIGHT;
    if (s=="WAIT")
        return Command::WAIT;
    if (s=="SENSE_EXIT"||s=="EXIT")
        return Command::SENSE_EXIT;
    return Command::MOVE_FORWARD;
}

QString MainWindow::cmdToStr(Command c) {
    switch(c){
    case Command::MOVE_FORWARD: return "MOVE_FORWARD";
    case Command::TURN_LEFT:    return "TURN_LEFT";
    case Command::TURN_RIGHT:   return "TURN_RIGHT";
    case Command::WAIT:         return "WAIT";
    case Command::SENSE_EXIT:   return "SENSE_EXIT";
    }
    return "?";
}

void MainWindow::log(const QString& msg, const QString& type) {
    QString col="#d0d0d0";
    if      (type=="error")   col="#ff6b6b";
    else if (type=="success") col="#69db7c";
    else if (type=="warning") col="#ffd43b";
    else if (type=="system")  col="#74c0fc";
    else if (type=="command") col="#c084fc";

    logBox->append(QString("<span style='color:%1;'>%2</span>").arg(col,msg));
    logBox->verticalScrollBar()->setValue(logBox->verticalScrollBar()->maximum());
}

void MainWindow::refreshStatus() {
    lblSteps->setText(QString("Քայլ: %1").arg(logic->getStepCount()));
    QString di,ds;
    switch(logic->getRobotDir()){
    case Direction::UP:    di="↑";ds="Վերև"; break;
    case Direction::RIGHT: di="→";ds="Աջ";   break;
    case Direction::DOWN:  di="↓";ds="Ներք"; break;
    case Direction::LEFT:  di="←";ds="Ձախ";  break;
    }
    Position p=logic->getRobotPos();
    lblPos->setText(QString("Դիրք: (%1,%2)").arg(p.row+1).arg(p.col+1));
    lblDir->setText(QString("Ուղղ: %1 %2").arg(di,ds));

    if (logic->isExitReached())
        lblStatus->setText(" ՀԱՋՈՂՈՒԹՅՈՒՆ!"),
            lblStatus->setStyleSheet("color:#ffeb3b;font-size:11pt;padding:7px;"
                                     "background:#1a4a1a;border-radius:5px;margin:3px;font-weight:bold;");
    else if (logic->isGameOver())
        lblStatus->setText(" Ձախողված"),
            lblStatus->setStyleSheet("color:#ff6b6b;font-size:11pt;padding:7px;"
                                     "background:#4a1a1a;border-radius:5px;margin:3px;font-weight:bold;");
    else if (running)
        lblStatus->setText("Ընթացիկ վիճակում է..."),
            lblStatus->setStyleSheet("color:#4fc3f7;font-size:11pt;padding:7px;"
                                     "background:#1a2a4a;border-radius:5px;margin:3px;font-weight:bold;");
    else
        lblStatus->setText("✓ Պատրաստ"),
            lblStatus->setStyleSheet("color:#a0ffa0;font-size:11pt;padding:7px;"
                                     "background:#1a3a1a;border-radius:5px;margin:3px;");
}
