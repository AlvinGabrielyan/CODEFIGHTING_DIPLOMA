#include "beesimulatorwindow.h"
#include <QStyle>
#include "resultsaver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QToolBar>
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QSplitter>
#include <QMessageBox>
#include <QDialog>
#include <QTimer>
#include <QFrame>
#include <QSet>
#include <cmath>
#include "bee.h"

// Վիզուալիզացիայի վիջեթ
class BeeVisualizationWidget : public QWidget {
private:
    BeeSimulator* simulator;

public:
    explicit BeeVisualizationWidget(BeeSimulator* sim, QWidget* parent = nullptr)
        : QWidget(parent), simulator(sim)
    {
        setMinimumSize(800, 500);
        setStyleSheet("background-color: #FFF8E7; border: 2px solid #333;");
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Երկինք
        QLinearGradient sky(0, 0, 0, height()/2);
        sky.setColorAt(0, QColor("#E8F4F8"));
        sky.setColorAt(1, QColor("#FFF8DC"));
        painter.fillRect(0, 0, width(), height()/2, sky);

        // Խոտ
        QLinearGradient grass(0, height()/2, 0, height());
        grass.setColorAt(0, QColor("#E8F5E9"));
        grass.setColorAt(1, QColor("#A5D6A7"));
        painter.fillRect(0, height()/2, width(), height()/2, grass);

        // Արև
        QRadialGradient sun(width()-80, 60, 35);
        sun.setColorAt(0, QColor("#FFF9C4"));
        sun.setColorAt(1, QColor("#FFD54F"));
        painter.setBrush(sun);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(width()-80, 60), 35, 35);

        // Ծաղիկներ - աջ կողմում
        int flowerY = height() - 100;
        drawFlower(painter, width()-200, flowerY, QColor("#FF69B4"), "Վարդ", "FLOWER1");
        drawFlower(painter, width()-340, flowerY - 30, QColor("#9370DB"), "Մանուշակ", "FLOWER2");
        drawFlower(painter, width()-270, flowerY - 15, QColor("#FFD700"), "Երիցուկ", "FLOWER3");

        // Ամաններ - կենտրոնում
        for (int i = 0; i < 5; i++) {
            QString combId = QString("COMB%1").arg(i+1);
            auto info = simulator->getHoneycombInfo(combId);
            int x = 120 + (i % 3) * 110;
            int y = height() - 220 + (i / 3) * 110;
            drawHoneycomb(painter, x, y, info, combId);
        }

        // Տեղեկատվական պանել վերևում
        drawInfoPanel(painter);

        // Մեղուներ - վերջում նկարել որ ամեն ինչից վերևում լինեն
        auto bees = simulator->getAllBees();
        for (QObject* obj : bees) {
            Bee* bee = qobject_cast<Bee*>(obj);
            if (bee) {
                drawBee(painter, bee);
            }
        }
    }

private:
    QPointF getCombPosition(const QString& combId) {
        int idx = combId.mid(4).toInt() - 1;
        int x = 120 + (idx % 3) * 110;  // Ավելացրել տարածությունը 80→110
        int y = height() - 220 + (idx / 3) * 110;  // Ավելացրել տարածությունը 90→110
        return QPointF(x, y);
    }

    QPointF getFlowerPosition(const QString& flowerId) {
        int flowerY = height() - 100;
        if (flowerId == "FLOWER1") return QPointF(width()-200, flowerY);
        if (flowerId == "FLOWER2") return QPointF(width()-340, flowerY - 30);
        if (flowerId == "FLOWER3") return QPointF(width()-270, flowerY - 15);
        return QPointF(0, 0);
    }

    void drawInfoPanel(QPainter& p) {
        p.setBrush(QColor(255, 255, 255, 200));
        p.setPen(QPen(QColor("#2196F3"), 2));
        p.drawRoundedRect(10, 10, width() - 20, 80, 10, 10);

        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(20, 30, " Մեղուներ:");

        auto bees = simulator->getAllBees();
        int x = 120;
        for (QObject* obj : bees) {
            Bee* bee = qobject_cast<Bee*>(obj);
            if (bee) {
                QColor beeColor;
                if (bee->id() == "BEE1") beeColor = QColor("#FFE44D");
                else if (bee->id() == "BEE2") beeColor = QColor("#FFD700");
                else beeColor = QColor("#FFA500");

                // Մեղուի գունավոր կետ
                p.setBrush(beeColor);
                p.setPen(Qt::black);
                p.drawEllipse(x, 18, 14, 14);

                // Տեղեկատվություն կոմպակտ ձևով
                p.setPen(Qt::black);
                p.setFont(QFont("Arial", 9));

                // Առաջին տող: Նեկտար և էներգիա
                QString line1 = QString("🍯%1գ ⚡%2%")
                                    .arg(bee->currentNectar(), 0, 'f', 1)
                                    .arg(bee->energy());
                p.drawText(x, 42, line1);

                // Երկրորդ տող: Դիրք
                QString line2 = bee->location();
                if (bee->isFlying()) {
                    line2 = QString("→%1").arg(bee->targetLocation());
                }
                p.setFont(QFont("Arial", 8));
                p.drawText(x, 55, line2);

                x += 110;
            }
        }
    }

    void drawFlower(QPainter& p, int x, int y, QColor color, QString name, QString id) {

        if (id == "FLOWER1") {
            //  ՎԱՐԴ  լայն կլորավուն թերթեր
            QColor rose("#CC0000");
            QColor roseMid("#E8001A");
            QColor roseDark("#8B0000");

            p.setRenderHint(QPainter::Antialiasing, true);

            // Արտաքին շերտ
            for (int i = 0; i < 5; i++) {
                double angle = (2.0 * M_PI / 5.0) * i - M_PI / 2.0;
                p.save();
                p.translate(x, y);
                p.rotate(angle * 180.0 / M_PI);

                QPainterPath petal;
                // Ներքևից բարձրանում, ձախ-աջ բացվում, վերևում կլորանում
                petal.moveTo(0, 0);
                petal.cubicTo(-10, -8,   -18, -18,  -12, -30);
                petal.cubicTo( -6, -38,    6, -38,   12, -30);
                petal.cubicTo( 18, -18,   10,  -8,    0,   0);

                p.setBrush(rose);
                p.setPen(QPen(roseDark, 1.2));
                p.drawPath(petal);

                // Թերթի  (ծալք)
                p.setPen(QPen(roseDark.lighter(130), 0.8));
                p.drawLine(0, -2, 0, -28);

                p.restore();
            }

            // ── Միջին շերտ — 5 ավելի կարճ թերթ, պտտված ──
            for (int i = 0; i < 5; i++) {
                double angle = (2.0 * M_PI / 5.0) * i - M_PI / 2.0 + M_PI / 5.0;
                p.save();
                p.translate(x, y);
                p.rotate(angle * 180.0 / M_PI);

                QPainterPath petal;
                petal.moveTo(0, 0);
                petal.cubicTo(-8, -6,   -14, -14,  -9, -23);
                petal.cubicTo(-4, -29,   4,  -29,   9, -23);
                petal.cubicTo(14, -14,   8,   -6,   0,   0);

                p.setBrush(roseMid);
                p.setPen(QPen(roseDark, 1.0));
                p.drawPath(petal);
                p.restore();
            }

            //  Ներքին կենտրոն — գնդաձև  փակ բողբոջ
            QRadialGradient centerGrad(x, y-3, 9);
            centerGrad.setColorAt(0, QColor("#FF4444"));
            centerGrad.setColorAt(1, QColor("#990000"));
            p.setBrush(centerGrad);
            p.setPen(QPen(roseDark, 1));
            p.drawEllipse(QPointF(x, y-1), 8, 9);

        } else {
            //  ՄՅՈՒՍ ԾԱՂԻԿՆԵՐԸ — կլոր թերթեր
            QColor petalColor  = (id == "FLOWER3") ? QColor("#FFFFFF") : color;
            QColor petalBorder = (id == "FLOWER3") ? QColor("#CCCCCC") : color.darker(120);

            p.setBrush(petalColor);
            p.setPen(QPen(petalBorder, 2));
            for (int i = 0; i < 6; i++) {
                double angle = M_PI/3.0 * i;
                QPointF pos(x + 25*cos(angle), y + 25*sin(angle));
                p.drawEllipse(pos, 16, 16);
            }

            p.setBrush(QColor("#FFD700"));
            p.setPen(QPen(QColor("#DAA520"), 1));
            p.drawEllipse(QPointF(x, y), 12, 12);
        }

        // ══ Կոթ և տերևներ ══
        p.setPen(QPen(QColor("#228B22"), 4));
        p.drawLine(x, y, x, y+50);

        p.setBrush(QColor("#90EE90"));
        p.setPen(QPen(QColor("#228B22"), 1));
        QPolygon leaf1;
        leaf1 << QPoint(x-5, y+20) << QPoint(x-20, y+25) << QPoint(x-5, y+30);
        p.drawPolygon(leaf1);

        QPolygon leaf2;
        leaf2 << QPoint(x+5, y+35) << QPoint(x+20, y+40) << QPoint(x+5, y+45);
        p.drawPolygon(leaf2);

        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(x-25, y+70, name);
    }

    void drawHoneycomb(QPainter& p, int x, int y, const BeeSimulator::HoneycombInfo& info, QString id) {
        QPolygonF hex;
        double hexRadius = 45;
        for (int i = 0; i < 6; i++) {
            double angle = M_PI/3.0 * i;
            hex << QPointF(x + hexRadius*cos(angle), y + hexRadius*sin(angle));
        }

        double pct = (info.capacity > 0) ? (info.currentAmount / info.capacity * 100) : 0;

        // Գույնի սխեմա՝ ըստ լցվածության
        QColor fill;
        if (info.sealed) {
            fill = QColor("#D4AF37"); // Ոսկեգույն՝ փակված
        } else if (pct > 80) {
            fill = QColor("#FFD54F"); // Մուգ դեղին
        } else if (pct > 50) {
            fill = QColor("#FFE082"); // Միջին դեղին
        } else if (pct > 20) {
            fill = QColor("#FFECB3"); // Բաց դեղին
        } else {
            fill = QColor("#FFF9C4"); // Շատ բաց դեղին
        }

        // Նկարել վեցանկյունը
        p.setBrush(fill);
        p.setPen(QPen(QColor("#8B4513"), 3));
        p.drawPolygon(hex);

        // Ներքին դետալներ (մեղրի հոսքի էֆեկտ)
        if (pct > 10) {
            p.setPen(QPen(QColor("#FFA000"), 1, Qt::DotLine));
            for (int i = 0; i < 3; i++) {
                double innerRadius = 38 - i * 12;
                if (innerRadius > 0 && pct > i * 30) {
                    QPolygonF innerHex;
                    for (int j = 0; j < 6; j++) {
                        double angle = M_PI/3.0 * j;
                        innerHex << QPointF(x + innerRadius*cos(angle), y + innerRadius*sin(angle));
                    }
                    p.drawPolygon(innerHex);
                }
            }
        }

        // ID-ն ՎԵՐԵՎՈՒՄ (ոչ թե ներքևում)
        p.setPen(QColor("#D84315")); // Շագանակագույն կարմիր
        p.setFont(QFont("Arial", 11, QFont::Bold));
        QFontMetrics fm(p.font());
        // Ցուցադրել "ԱՄԱՆ1" փոխարեն "COMB1"
        QString displayId = id;
        if (id.startsWith("COMB")) {
            displayId = "ԱՄԱՆ" + id.mid(4);
        }
        int idWidth = fm.horizontalAdvance(displayId);
        p.drawText(x - idWidth/2, y - hexRadius - 8, displayId);

        // Տեղեկատվությունը ԱՄԱՆԻ ՆԵՐՍՈՒՄ
        // Քանակ
        p.setPen(QColor("#1976D2"));
        p.setFont(QFont("Arial", 10, QFont::Bold));
        QString amountText = QString("%1/%2գ")
                                 .arg(info.currentAmount, 0, 'f', 1)
                                 .arg(info.capacity, 0, 'f', 1);
        int amountWidth = fm.horizontalAdvance(amountText);
        p.drawText(x - 22, y - 5, amountText);

        // Տոկոս
        p.setPen(pct > 80 ? QColor("#F57C00") : QColor("#689F38")); // Նարնջագույն կամ կանաչ
        p.setFont(QFont("Arial", 12, QFont::Bold));
        QString pctText = QString("%1%").arg((int)pct);
        int pctWidth = fm.horizontalAdvance(pctText);
        p.drawText(x - 12, y + 12, pctText);

        // Փակված նշան
        if (info.sealed) {
            p.setFont(QFont("Arial", 18));
            p.drawText(x - 10, y + 32, "🔒");
        }
    }

    void drawBee(QPainter& p, Bee* bee) {
        double baseX = 0.0, baseY = 0.0;
        double angle = 0.0;

        QString loc = bee->location();
        QString target = bee->targetLocation();

        // Սկզբնական դիրք
        if (loc == "HIVE") {
            baseX = 0.45;
            baseY = 0.75;
            if (bee->id() == "BEE1") {
                baseX -= 0.015;
                baseY -= 0.02;
            } else if (bee->id() == "BEE2") {
                baseX += 0.00;
                baseY += 0.00;
            } else if (bee->id() == "BEE3") {
                baseX += 0.015;
                baseY += 0.02;
            }
        } else if (loc.startsWith("FLOWER")) {
            QPointF flowerPos = getFlowerPosition(loc);
            baseX = flowerPos.x() / width();
            baseY = flowerPos.y() / height();
        } else if (loc.startsWith("COMB")) {
            QPointF combPos = getCombPosition(loc);
            baseX = combPos.x() / width();
            baseY = combPos.y() / height();
        }

        if (bee->isFlying() && !target.isEmpty()) {
            double progress = bee->progress();

            double targetX = 0.0, targetY = 0.0;
            if (target == "HIVE") {
                targetX = 0.45;
                targetY = 0.75;
            } else if (target.startsWith("FLOWER")) {
                QPointF flowerPos = getFlowerPosition(target);
                targetX = flowerPos.x() / width();
                targetY = flowerPos.y() / height();
            } else if (target.startsWith("COMB")) {
                QPointF combPos = getCombPosition(target);
                targetX = combPos.x() / width();
                targetY = combPos.y() / height();
            }

            double dx = targetX - baseX;
            double dy = targetY - baseY;
            angle = atan2(dy, dx);

            double straightX = baseX + dx * progress;
            double straightY = baseY + dy * progress;

            double waveAmplitude = 0.03;
            double waveFrequency = 8.0;
            double wave = sin(progress * M_PI * waveFrequency) * waveAmplitude;

            baseX = straightX;
            baseY = straightY + wave;
        }

        // ՉԿԱՆ ԱՎԵԼԻ OFFSET-ներ - բոլոր մեղուները ճիշտ դիրքերում

        double px = baseX * width();
        double py = baseY * height();
        QPointF pos(px, py);

        QColor beeColor;
        if (bee->id() == "BEE1") beeColor = QColor("#FFE44D");
        else if (bee->id() == "BEE2") beeColor = QColor("#FFD700");
        else beeColor = QColor("#FFA500");

        p.save();
        p.translate(pos);
        p.rotate(angle * 180.0 / M_PI + 180.0);

        QPointF center(0, 0);

        p.setBrush(beeColor);
        p.setPen(QPen(Qt::black, 2));
        p.drawEllipse(center, 12, 12);
        p.drawEllipse(center + QPointF(18, 0), 16, 14);

        p.setBrush(Qt::black);
        p.drawEllipse(center + QPointF(12, 0), 4, 14);
        p.drawEllipse(center + QPointF(24, 0), 4, 14);

        p.setBrush(QColor(255, 255, 255, 180));
        p.setPen(QPen(QColor(150, 150, 150), 1));

        QPainterPath wing1;
        wing1.moveTo(center + QPointF(15, -8));
        wing1.quadTo(center + QPointF(5, -28), center + QPointF(-5, -12));
        wing1.quadTo(center + QPointF(8, -15), center + QPointF(15, -8));
        p.drawPath(wing1);

        QPainterPath wing2;
        wing2.moveTo(center + QPointF(15, 8));
        wing2.quadTo(center + QPointF(5, 28), center + QPointF(-5, 12));
        wing2.quadTo(center + QPointF(8, 15), center + QPointF(15, 8));
        p.drawPath(wing2);

        p.setBrush(Qt::black);
        p.drawEllipse(center + QPointF(-5, -5), 3, 3);
        p.drawEllipse(center + QPointF(-5, 5), 3, 3);

        p.setPen(QPen(Qt::black, 2));
        p.drawLine(center + QPointF(-8, -8), center + QPointF(-15, -15));
        p.drawLine(center + QPointF(-8, 8), center + QPointF(-15, 15));

        p.restore();

        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(pos + QPointF(-25, -40), bee->name());
    }
};

BeeSimulatorWindow::BeeSimulatorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Մեղուների կառավարման տրամաբանության սիմուլյատոր");
    resize(1400, 900);

    simulator = new BeeSimulator(this);

    simulator->addBee(new Bee("BEE1", "Մայա", "HIVE", 3.0, 3, 5, QColor("#FFE44D"), "Մայա մեղուն"));
    simulator->addBee(new Bee("BEE2", "Վիլի", "HIVE", 2.5, 3, 5, QColor("#FFD700"), "Վիլի մեղուն"));
    simulator->addBee(new Bee("BEE3", "Զիթա", "HIVE", 2.0, 3, 5, QColor("#FFA500"), "Զիթա մեղուն"));

    setupUI();
    connectSignals();
    showExampleScript();

    QTimer* vizTimer = new QTimer(this);
    connect(vizTimer, &QTimer::timeout, [this]() {
        if (vizWidget) vizWidget->update();
    });
    vizTimer->start(50);
}

void BeeSimulatorWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ՎԵՐԵՎԻ TOOLBAR
    QWidget* toolbar = new QWidget();
    toolbar->setStyleSheet("background-color: #34495E; padding: 5px;");
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);

    // Կոճակներ
    QPushButton* btnStart = new QPushButton("Մեկնարկ");
    btnStart->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    btnStart->setStyleSheet("QPushButton { background-color: #3498DB; color: white; padding: 10px 20px; font-size: 14px; font-weight: bold; border-radius: 5px; } QPushButton:hover { background-color: #2980B9; }");

    QPushButton* btnPause = new QPushButton("Դադար");
    btnPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    btnPause->setStyleSheet("QPushButton { background-color: #E67E22; color: white; padding: 10px 20px; font-size: 14px; font-weight: bold; border-radius: 5px; } QPushButton:hover { background-color: #D35400; }");
    btnPause->setEnabled(false);

    QPushButton* btnReset = new QPushButton("Վերականգնել");
    btnReset->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    btnReset->setStyleSheet("QPushButton { background-color: #9B59B6; color: white; padding: 10px 20px; font-size: 14px; font-weight: bold; border-radius: 5px; } QPushButton:hover { background-color: #8E44AD; }");

    QPushButton* btnHelp = new QPushButton("Օգնություն");
    btnHelp->setIcon(style()->standardIcon(QStyle::SP_MessageBoxQuestion));
    btnHelp->setStyleSheet("QPushButton { background-color: #7F8C8D; color: white; padding: 10px 20px; font-size: 14px; font-weight: bold; border-radius: 5px; } QPushButton:hover { background-color: #95A5A6; }");

    QPushButton* btnProblem = new QPushButton("Խնդրի նկարագրություն");
    btnProblem->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    btnProblem->setStyleSheet("QPushButton { background-color: #E74C3C; color: white; padding: 10px 20px; font-size: 14px; font-weight: bold; border-radius: 5px; } QPushButton:hover { background-color: #C0392B; }");

    // Ժամանակ
    timeLabel = new QLabel("Ժամանակ: 0.0 րոպե");
    timeLabel->setStyleSheet("color: white; font-size: 14px; font-weight: bold; padding: 5px;");

    toolbarLayout->addWidget(btnStart);
    toolbarLayout->addWidget(btnPause);
    toolbarLayout->addWidget(btnReset);
    toolbarLayout->addWidget(btnProblem);
    toolbarLayout->addWidget(btnHelp);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(timeLabel);

    mainLayout->addWidget(toolbar);

    // ՄԵՋՏԵՂԻ ՄԱՍԸ - Վիզուալիզացիա
    vizWidget = new BeeVisualizationWidget(simulator, this);
    mainLayout->addWidget(vizWidget, 1);

    // ՆԵՐՔԵՎԻ ՄԱՍԸ - Սցենար և Մատյան
    QWidget* bottomWidget = new QWidget();
    bottomWidget->setMaximumHeight(250);
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(5, 5, 5, 5);

    // Ձախ - Սցենար
    QWidget* scriptPanel = new QWidget();
    scriptPanel->setStyleSheet("background-color: #2C3E50; border-radius: 5px;");
    QVBoxLayout* scriptLayout = new QVBoxLayout(scriptPanel);

    QLabel* scriptLabel = new QLabel(" Սցենար");
    scriptLabel->setStyleSheet("color: #ECF0F1; font-size: 14px; font-weight: bold; padding: 5px;");
    scriptLayout->addWidget(scriptLabel);

    scriptInput = new QTextEdit();
    scriptInput->setFont(QFont("Courier New", 10));
    scriptInput->setStyleSheet("QTextEdit { background-color: #34495E; color: #ECF0F1; border: 1px solid #555; padding: 5px; }");
    scriptLayout->addWidget(scriptInput);

    // Աջ - Մատյան
    QWidget* logPanel = new QWidget();
    logPanel->setStyleSheet("background-color: #2C3E50; border-radius: 5px;");
    QVBoxLayout* logLayout = new QVBoxLayout(logPanel);

    QLabel* logLabel = new QLabel("📋 Մատյան");
    logLabel->setStyleSheet("color: #ECF0F1; font-size: 14px; font-weight: bold; padding: 5px;");
    logLayout->addWidget(logLabel);

    logOutput = new QTextEdit();
    logOutput->setReadOnly(true);
    logOutput->setFont(QFont("Consolas", 9));
    logOutput->setStyleSheet("QTextEdit { background-color: #34495E; color: #ECF0F1; border: 1px solid #555; padding: 5px; }");
    logLayout->addWidget(logOutput);

    bottomLayout->addWidget(scriptPanel, 1);
    bottomLayout->addWidget(logPanel, 1);

    mainLayout->addWidget(bottomWidget);

    // Պահպանել հղումները
    startButton = btnStart;
    pauseButton = btnPause;
    resetButton = btnReset;

    connect(btnStart, &QPushButton::clicked, this, &BeeSimulatorWindow::onStart);
    connect(btnPause, &QPushButton::clicked, this, &BeeSimulatorWindow::onPause);
    connect(btnReset, &QPushButton::clicked, this, &BeeSimulatorWindow::onReset);
    connect(btnProblem, &QPushButton::clicked, this, &BeeSimulatorWindow::onProblemDescription);
    connect(btnHelp, &QPushButton::clicked, this, &BeeSimulatorWindow::onHelp);
}

void BeeSimulatorWindow::connectSignals() {
    connect(simulator, &BeeSimulator::currentTimeChanged, this, &BeeSimulatorWindow::updateTime);
    connect(simulator, &BeeSimulator::logMessage, this, &BeeSimulatorWindow::onLogMessage);
    connect(simulator, &BeeSimulator::simulationFinished, this, &BeeSimulatorWindow::onSimulationFinished);
}

void BeeSimulatorWindow::showExampleScript() {
    scriptInput->clear();
}

void BeeSimulatorWindow::onStart() {
    // Ավտոմատ բեռնել սցենարը մեկնարկից առաջ
    QString script = scriptInput->toPlainText();
    if (script.trimmed().isEmpty()) {
        logOutput->append("<font color='#E74C3C'>Սցենարը դատարկ է!</font>");
        return;
    }
    bool success = simulator->loadScript(script);
    if (!success) {
        logOutput->append("<font color='#E74C3C'>Սցենարում սխալներ կան:</font>");
        const QStringList errors = simulator->getErrors();
        for (const QString& error : errors) {
            logOutput->append("<font color='#E74C3C'>  * " + error + "</font>");
        }
        return;
    }
    simulator->start();
    startButton->setEnabled(false);
    pauseButton->setEnabled(true);
}

void BeeSimulatorWindow::onPause() {
    simulator->pause();
    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
}

void BeeSimulatorWindow::onReset() {
    simulator->reset();
    logOutput->clear();
    logOutput->append("<font color='#3498DB'>Համակարգը վերականգնված է</font>");
    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
    if (vizWidget) vizWidget->update();
}

void BeeSimulatorWindow::updateTime() {
    timeLabel->setText(QString("Ժամանակ: %1 րոպե").arg(simulator->currentTime(), 0, 'f', 1));
}

void BeeSimulatorWindow::onLogMessage(const QString& message, const QString& type) {
    QString color = "#ECF0F1";
    if (type == "error") color = "#E74C3C";
    else if (type == "warning") color = "#F39C12";
    else if (type == "success") color = "#2ECC71";
    else if (type == "info") color = "#3498DB";
    else if (type == "command") color = "#9B59B6";

    logOutput->append(QString("<font color='%1'>%2</font>").arg(color, message));

    QScrollBar* scrollBar = logOutput->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void BeeSimulatorWindow::onSimulationFinished() {
    logOutput->append("<font color='#2ECC71' size='4'><b>Սիմուլյացիան ավարտված է!</b></font>");
    startButton->setEnabled(false);
    pauseButton->setEnabled(false);
    // Rating dialog — 600ms հետաձգումով որ UI-ն թարմացվի
    QTimer::singleShot(600, this, [this]() {
        RatingResult result = calculateRating();
        showRatingDialog(result);
    });
}

void BeeSimulatorWindow::onHelp() {
    QString help =
        "ՀՐԱՄԱՆՆԵՐ:\n"
        "• FLY(մեղու, սկիզբ->վերջ, ժամանակ)\n"
        "• COLLECT(մեղու, տեսակ, քանակ, ժամանակ)\n"
        "• DEPOSIT(մեղու, աման, ժամանակ)\n"
        "• REST(մեղու, տևողություն, ժամանակ)\n"
        "• CHECK_COMB(աման)\n\n"
        "ԿՈՃԱԿՆԵՐ:\n"
        "Մեկնարկ - Սկսել սիմուլյացիան\n"
        "Դադար - Դադարեցնել\n"
        "Վերականգնել - Վերականգնել համակարգը\n";

    QMessageBox::information(this, "Օգնություն", help);
}

void BeeSimulatorWindow::onProblemDescription() {
    // Ստեղծել custom dialog
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Խնդրի նկարագրություն");
    dialog->resize(700, 600);

    // Բեժ ֆոն
    dialog->setStyleSheet(
        "QDialog { background-color: #F5F5DC; }"  // Beige
        "QLabel { color: #2C3E50; }"
        "QTextEdit { background-color: #FFFEF0; border: 2px solid #D2B48C; border-radius: 5px; padding: 10px; }"
        );

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Վերնագիր
    QLabel* titleLabel = new QLabel("🍯 ՄԵՂՈՒՆԵՐԻ ԿԱՌԱՎԱՐՄԱՆ ԽՆԴԻՐ");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #8B4513; padding: 10px;");
    layout->addWidget(titleLabel);

    // Scrollable տեքստ
    QTextEdit* textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setHtml(
        "<div style='font-size: 11pt; line-height: 1.6;'>"

        "<p>Մի փոքր փեթակում ապրում են երեք աշխույժ մեղուներ՝ "
        "<b>Մայա</b>, <b>Վիլի</b> և <b>Զիթա</b>։ Նրանք պետք է հավաքեն նեկտար "
        "ծաղիկներից և լցնեն մեղրը հինգ մեղրային ամանների մեջ։</p>"

        "<p>Յուրաքանչյուր աշնանային առավոտ մեղուները սկսում են իրենց օրվա "
        "աշխատանքը՝ թռչելով դեպի գույնզգույն ծաղիկներ։ Բայց այս պարզ գործընթացը "
        "պահանջում է խելացի կազմակերպում։</p>"

        "<h3 style='color: #D2691E; border-bottom: 2px solid #D2B48C; margin-top: 15px;'>🐝 ՄԵՂՈՒՆԵՐԻ ՀԱՏԿՈՒԹՅՈՒՆՆԵՐԸ</h3>"

        "<table style='width: 100%; margin: 10px 0;'>"
        "<tr style='background-color: #FFE44D;'>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'><b>🟡 ՄԱՅԱ</b></td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'>Խիզախ և ուժեղ</td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'><b>3.0գ</b></td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'>3-5վրկ</td>"
        "</tr>"
        "<tr style='background-color: #FFD700;'>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'><b>🟠 ՎԻԼԻ</b></td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'>Արագ և ճարպիկ</td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'><b>2.5գ</b></td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'>3-5վրկ</td>"
        "</tr>"
        "<tr style='background-color: #FFA500;'>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'><b>🔶 ԶԻԹԱ</b></td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'>Փոքրիկ և շարժուն</td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'><b>2.0գ</b></td>"
        "<td style='padding: 8px; border: 1px solid #D2B48C;'>3-5վրկ</td>"
        "</tr>"
        "</table>"

        "<h3 style='color: #D2691E; border-bottom: 2px solid #D2B48C; margin-top: 15px;'> ԾԱՂԻԿՆԵՐԸ</h3>"
        "<p style='margin: 10px 0;'>"
        " <b style='color: #C71585;'>Վարդ</b> - Վարդագույն և բուրավետ<br>"
        " <b style='color: #9370DB;'>Մանուշակ</b> - Մանուշակագույն և քաղցր<br>"
        " <b style='color: #DAA520;'>Երիցուկ</b> - Արևային և պայծառ"
        "</p>"

        "<h3 style='color: #D2691E; border-bottom: 2px solid #D2B48C; margin-top: 15px;'> ԱՄԱՆՆԵՐԸ</h3>"
        "<p style='margin: 10px 0;'>"
        "• Քանակ՝ <b>5 աման</b><br>"
        "• Տարողություն՝ <b>5.0 գրամ</b> (յուրաքանչյուրը)<br>"
        "• Ընդհանուր՝ <b>25.0 գրամ</b><br>"
        "• Երբ ամանը լիքն է՝ այն ավտոմատ փակվում է։"
        "</p>"

        "<h3 style='color: #D2691E; border-bottom: 2px solid #D2B48C; margin-top: 15px;'> ԽՆԴՐԻ ՆՊԱՏԱԿԸ</h3>"
        "<p style='background-color: #FFF8DC; padding: 10px; border-left: 4px solid #D2691E; margin: 10px 0;'>"
        "Ձեր խնդիրն է գրել սցենար, որը կօպտիմալացնի մեղուների աշխատանքը այնպես, որ՝<br><br>"
        "✓ Բոլոր հինգ ամանները լցվեն,<br>"
        "✓ Մեղուները չբեռնվեն ավելի շատ նեկտարով, քան անհրաժեշտ է,<br>"
        "✓ Աշխատանքն իրականացնել հնարավորինս կարճ ժամանակահատվածում,<br>"
        "✓ Մեղուները չհյուծվեն,<br>"
        "✓ Աշխատանքը հեշտացնելու համար զուգահեռացնել նրանց թռիչքները։"
        "</p>"

        "<h3 style='color: #D2691E; border-bottom: 2px solid #D2B48C; margin-top: 15px;'>Աստղերի պայմանները</h3>"
        "<table style='width:100%; margin:8px 0; border-collapse:collapse;'>"
        "<tr style='background:#FFF8DC;'>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C; width:55px; text-align:center;'>⭐</td>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C;'>Բոլոր 5 ամանները լիարժեքորեն լցված են</td>"
        "</tr>"
        "<tr style='background:#FFFACD;'>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C; text-align:center;'>⭐⭐</td>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C;'>Ընդհանուր ժամանակ ≤ 70 վրկ</td>"
        "</tr>"
        "<tr style='background:#FFF8DC;'>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C; text-align:center;'>⭐⭐⭐</td>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C;'>Հավաքված նեկտար ≥ 25 գ (կորուստ չկա)</td>"
        "</tr>"
        "<tr style='background:#FFFACD;'>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C; text-align:center;'>⭐⭐⭐⭐</td>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C;'>Բոլոր մեղուների էներգիան &gt; 30% (ավելացրու REST)</td>"
        "</tr>"
        "<tr style='background:#FFF8DC;'>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C; text-align:center;'>⭐⭐⭐⭐⭐</td>"
        "<td style='padding:6px 10px; border:1px solid #D2B48C;'>Բոլոր 3 մեղուներն t=0-ին միաժամանակ մեկնել են</td>"
        "</tr>"
        "</table>"

        "<p style='text-align: center; margin-top: 20px; font-size: 14pt;'>"
        "<b>Հաջողություն! </b>"
        "</p>"

        "</div>"
        );

    layout->addWidget(textEdit);

    // Փակել կոճակ
    QPushButton* closeButton = new QPushButton("✓ Փակել");
    closeButton->setStyleSheet(
        "QPushButton { background-color: #8B4513; color: white; padding: 10px 30px; "
        "font-size: 12px; font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #A0522D; }"
        );
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    dialog->exec();
}


BeeSimulatorWindow::RatingResult BeeSimulatorWindow::calculateRating()
{
    RatingResult result;
    int stars = 0;

    // Բոլոր 5 ամանները լրիվ լցված
    int filled = 0;
    for (int i = 1; i <= 5; i++) {
        auto info = simulator->getHoneycombInfo(QString("COMB%1").arg(i));
        if (info.sealed || info.currentAmount >= info.capacity - 0.01) filled++;
    }
    bool allFilled = (filled == 5);
    if (allFilled) {
        stars++;
        result.criteria << "⭐ Բոլոր 5 ամանները լրիվ լցված են ✅";
    } else {
        result.criteria << QString("❌ Լցված ամաններ՝ %1/5 — բոլոր 5-ը պետք է լցված լինեն").arg(filled);
    }

    // Ժամանակ ≤ 70 վրկ
    double t = simulator->currentTime();
    if (t <= 70.0) {
        stars++;
        result.criteria << QString("⭐ Ժամանակ՝ %1 վրկ  (≤ 70 վրկ) ✅").arg(t, 0, 'f', 1);
    } else if (t <= 110.0) {
        result.criteria << QString("❌ Ժամանակ՝ %1 վրկ  (70 վրկ-ից ավելի — արագացրու)").arg(t, 0, 'f', 1);
    } else {
        result.criteria << QString("❌ Ժամանակ՝ %1 վրկ  (շատ դանդաղ է կազմակերպված)").arg(t, 0, 'f', 1);
    }

    // Հավաքված նեկտար 24.9–25.5գ
    double nectar = simulator->totalNectarCollected();
    if (nectar >= 24.9 && nectar <= 25.5) {
        stars++;
        result.criteria << QString("⭐ Կատարյալ նեկտար՝ %1 գ  (կորուստ չկա) ✅").arg(nectar, 0, 'f', 1);
    } else if (nectar < 24.9) {
        result.criteria << QString("❌ Հավաքված նեկտար՝ %1 գ  (25 գ-ից պակաս — ամանները լիարժեքորեն չեն լցվել)").arg(nectar, 0, 'f', 1);
    } else {
        result.criteria << QString("❌ Հավաքված նեկտար՝ %1 գ  (%2 գ ավելցուկ է եղել, մեղուն կերել է)")
                               .arg(nectar, 0, 'f', 1).arg(nectar - 25.0, 0, 'f', 1);
    }

    //Բոլոր մեղուների էներգիա > 30%
    auto bees = simulator->getAllBees();
    int healthy = 0;
    QStringList tired;
    for (QObject* obj : bees) {
        Bee* bee = qobject_cast<Bee*>(obj);
        if (!bee) continue;
        if (bee->energy() > 30) healthy++;
        else tired << QString("%1(%2%)").arg(bee->name()).arg(bee->energy());
    }
    if (healthy == 3) {
        stars++;
        result.criteria << "⭐ Բոլոր մեղուների էներգիան > 30% ✅";
    } else {
        result.criteria << QString("❌ Հյուծված մեղուներ՝ %1 — ավելացրու REST հրամաններ")
                               .arg(tired.join(", "));
    }

    // Զուգահեռ մեկնարկ — բոլոր 3 մեղուն t=0-ին FLY
    // m_commands-ից ստուգում ենք FLY հրամանները t=0-ին
    QSet<QString> parallelBees;
    for (Command* cmd : simulator->getCommands()) {
        if (cmd->type() == Command::FLY && cmd->executeTime() == 0.0) {
            parallelBees.insert(cmd->beeId());
        }
    }
    bool hasParallel = parallelBees.contains("BEE1") &&
                       parallelBees.contains("BEE2") &&
                       parallelBees.contains("BEE3");
    if (hasParallel) {
        stars++;
        result.criteria << "⭐ Բոլոր 3 մեղուն t=0-ին ՄԻԱԺԱՄԱՆԱԿ թռել են ✅";
    } else {
        QStringList missing;
        for (const QString& id : {"BEE1","BEE2","BEE3"}) {
            if (!parallelBees.contains(id)) missing << id;
        }
        result.criteria << QString("❌ %1-ը t=0-ին FLY չի սկսել — բոլոր 3 մեղուն պետք է ՄԻԱԺԱՄԱՆԱԿ մեկնեն")
                               .arg(missing.join(", "));
    }

    //ԱՄՓՈՓ
    result.stars = stars;
    switch (stars) {
    case 5: result.title = "ԿԱՏԱՐՅԱԼ";
        result.description = "Բոլոր 5 պայմանները կատարված են։\nՄեղուները աշխատել են արագ, արդյունավետ\nև առանց ոչ մի կաթիլ կորուստի։ Շնորհավորում եմ!"; break;
    case 4: result.title = "ԳԵՐԱԶԱՆՑ";
        result.description = "Շատ լավ արդյունք — մի փոքր բարելավումով\nկարող ես հասնել կատարյալ գնահատականի։"; break;
    case 3: result.title = "ԼԱՎ";
        result.description = "Հիմնական խնդիրը լուծված է։\nՈւշադրություն դարձրու ժամանակին,\nկորուստներին կամ էներգիային։"; break;
    case 2: result.title = "ԲԱՎԱՐԱՐ";
        result.description = "Մասամբ ճիշտ լուծում։\nՍտուգիր բոլոր ամաններն ու ժամանակը։"; break;
    default:result.title = "ԹՈՒՅԼ";
        result.description = "Խնդիրը դեռ լուծված չէ։\nԲոլոր 5 ամանները պետք է լցվեն\n70 վրկ-ի ընթացքում, առանց կորուստների։"; break;
    }
    return result;
}

void BeeSimulatorWindow::showRatingDialog(const RatingResult& result)
{
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Արդյունքների գնահատում");
    dlg->setFixedSize(560, 520);

    // Palette-ով ուղղակի ֆոն + տեքստ — stylesheet QLabel override-ից ավելի հուսալի
    QPalette pal = dlg->palette();
    pal.setColor(QPalette::Window,     QColor("#1a1a2e"));
    pal.setColor(QPalette::WindowText, QColor("#ffffff"));
    pal.setColor(QPalette::Base,       QColor("#1a1a2e"));
    pal.setColor(QPalette::Text,       QColor("#ffffff"));
    dlg->setPalette(pal);
    dlg->setAutoFillBackground(true);

    dlg->setStyleSheet(
        "QPushButton { background-color:#e2b714; color:#1a1a2e; font-weight:bold;"
        "  font-size:13px; padding:10px 40px; border-radius:8px; border:none; }"
        "QPushButton:hover { background-color:#f0c832; }");

    QVBoxLayout* lay = new QVBoxLayout(dlg);
    lay->setSpacing(10);
    lay->setContentsMargins(35, 28, 35, 28);

    // «ԳՆԱՀԱՏՈՒՄ» վերնագիր
    QLabel* head = new QLabel("ԳՆԱՀԱՏՈՒՄ");
    head->setAlignment(Qt::AlignCenter);
    head->setFont(QFont("Arial", 11, QFont::Bold));
    head->setStyleSheet("color:#888888; letter-spacing:5px; background:transparent;");
    lay->addWidget(head);

    // Աստղեր — մեծ
    QString ss;
    for (int i = 0; i < 5; i++) { ss += (i < result.stars) ? "★" : "☆"; if (i < 4) ss += " "; }
    QLabel* starsLbl = new QLabel(ss);
    starsLbl->setAlignment(Qt::AlignCenter);
    starsLbl->setStyleSheet("font-size:46px; color:#e2b714; letter-spacing:8px; padding:4px 0; background:transparent;");
    lay->addWidget(starsLbl);

    // Վերնագիր
    QLabel* titleLbl = new QLabel(result.title);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setFont(QFont("Arial", 18, QFont::Bold));
    titleLbl->setStyleSheet("color:#e2b714; background:transparent;");
    lay->addWidget(titleLbl);

    // Նկարագրություն
    QLabel* descLbl = new QLabel(result.description);
    descLbl->setAlignment(Qt::AlignCenter);
    descLbl->setWordWrap(true);
    descLbl->setFont(QFont("Arial", 10));
    descLbl->setStyleSheet("color:#bbbbbb; margin:2px 0 8px 0; background:transparent;");
    lay->addWidget(descLbl);

    // Բաժանիչ
    QFrame* sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("background-color:#3a3a6a; border:none; max-height:1px;");
    lay->addWidget(sep);

    // ՊԱՅՄԱՆՆԵՐ
    QLabel* critHead = new QLabel("ՊԱՅՄԱՆՆԵՐ");
    critHead->setFont(QFont("Arial", 9, QFont::Bold));
    critHead->setStyleSheet("color:#666666; letter-spacing:4px; margin-top:4px; background:transparent;");
    lay->addWidget(critHead);

    for (const QString& c : result.criteria) {
        QLabel* lbl = new QLabel(c);
        lbl->setWordWrap(true);
        lbl->setFont(QFont("Arial", 10));
        if (c.contains("✅"))
            lbl->setStyleSheet("color:#4ade80; padding:2px 0; background:transparent;");
        else
            lbl->setStyleSheet("color:#ff7070; padding:2px 0; background:transparent;");
        lay->addWidget(lbl);
    }

    lay->addStretch();

    QPushButton* retryBtn    = new QPushButton("↺  Կրկին փորձել");
    QPushButton* continueBtn = new QPushButton("→  Անցնել առաջ");

    retryBtn->setStyleSheet(
        "QPushButton { background-color:#2a2a4a; color:#ffffff; font-weight:bold;"
        "  font-size:13px; padding:10px 28px; border-radius:8px; border:1px solid #4a4a8a; }"
        "QPushButton:hover { background-color:#3a3a6a; }");
    continueBtn->setStyleSheet(
        "QPushButton { background-color:#e2b714; color:#1a1a2e; font-weight:bold;"
        "  font-size:13px; padding:10px 28px; border-radius:8px; border:none; }"
        "QPushButton:hover { background-color:#f0c832; }");

    connect(retryBtn,    &QPushButton::clicked, dlg, &QDialog::reject);
    connect(continueBtn, &QPushButton::clicked, dlg, &QDialog::accept);

    QHBoxLayout* br = new QHBoxLayout();
    br->addStretch();
    br->addWidget(retryBtn);
    br->addSpacing(12);
    br->addWidget(continueBtn);
    br->addStretch();
    lay->addLayout(br);

    // Պահպանել արդյունքը դատաբազայում
    ResultSaver::save("bee", result.stars);

    int choice = dlg->exec();
    delete dlg;
    if (choice == QDialog::Rejected) {
        onReset();
        scriptInput->clear();
        scriptInput->setFocus();
    } else {
        this->close();
    }
}
