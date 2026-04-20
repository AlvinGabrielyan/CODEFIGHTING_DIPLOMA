#include "mainwindow.h"
#include "resultsaver.h"
#include <QStyle>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QGroupBox>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QDialog>
#include <QFrame>
#include <QQueue>
#include <QMap>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <climits>
using namespace std;


BucketWidget::BucketWidget(const QString& name, int capacity, QWidget* parent)
    : QWidget(parent), bucketName(name), maxCapacity(capacity),
    currentValue(0), animatedValue(0.0f), pourDirection(PourDirection::None) {
    setMinimumSize(140, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    waterAnim = new QPropertyAnimation(this, "animatedWaterLevel", this);
    waterAnim->setDuration(1400);
    waterAnim->setEasingCurve(QEasingCurve::OutCubic);

    rippleTimer = new QTimer(this);
    rippleTimer->setInterval(30);
    connect(rippleTimer, &QTimer::timeout, this, [this]() {
        ripplePhase += 0.18f;
        update();
    });
    rippleTimer->start();
}

void BucketWidget::setValue(int value) {
    int clamped = qBound(0, value, maxCapacity);
    float targetLevel = maxCapacity > 0 ? (float)clamped / maxCapacity : 0.0f;
    waterAnim->stop();
    waterAnim->setStartValue(animatedValue);
    waterAnim->setEndValue(targetLevel);
    waterAnim->start();
    currentValue = clamped;
}

void BucketWidget::setCapacity(int capacity) {
    maxCapacity = capacity;
    currentValue = qBound(0, currentValue, maxCapacity);
    animatedValue = maxCapacity > 0 ? (float)currentValue / maxCapacity : 0.0f;
    update();
}

float BucketWidget::getAnimatedWaterLevel() const { return animatedValue; }
void  BucketWidget::setAnimatedWaterLevel(float v) { animatedValue = v; update(); }

void BucketWidget::setPourDirection(PourDirection dir) {
    pourDirection = dir;
    update();
}

void BucketWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width(), h = height();

    float capRatio = (maxCapacity > 0) ? qBound(0.3f, (float)maxCapacity / 10.0f, 1.0f) : 0.4f;
    int topOffset = (int)(8 + capRatio * 8);

    int bH = qMin((int)(100 + capRatio * 120), h - 80);
    int bW = qMin((int)(70  + capRatio * 60),  w - 24);

    int bX = (w - bW) / 2;
    int bY = qMax(50, h - bH - 44); // առնվազն 50px վերևից՝ handle-ի համար

    // շողք
    p.setPen(Qt::NoPen);
    QRadialGradient shadow(w / 2, bY + bH + 18, 55);
    shadow.setColorAt(0, QColor(0, 0, 0, 60));
    shadow.setColorAt(1, QColor(0, 0, 0, 0));
    p.setBrush(shadow);
    p.drawEllipse(bX - 20, bY + bH + 5, bW + 40, 28);

    // դույլ
    QPainterPath bucketPath;
    bucketPath.moveTo(bX - topOffset, bY);
    bucketPath.lineTo(bX + bW + topOffset, bY);
    bucketPath.lineTo(bX + bW, bY + bH);
    bucketPath.quadTo(bX + bW, bY + bH + 12, bX + bW / 2, bY + bH + 12);
    bucketPath.quadTo(bX, bY + bH + 12, bX, bY + bH);
    bucketPath.closeSubpath();

    p.setBrush(QColor(188, 202, 216));
    p.setPen(QPen(QColor(95, 118, 142), 2));
    p.drawPath(bucketPath);

    // ջուր
    if (animatedValue > 0.001f) {
        p.setClipPath(bucketPath);
        int waterH = (int)(bH * animatedValue);
        int waterY = bY + bH - waterH;

        auto bucketLeftAt  = [&](float y) { float t = (y - bY) / bH; return bX - topOffset + t * topOffset; };
        auto bucketRightAt = [&](float y) { float t = (y - bY) / bH; return bX + bW + topOffset - t * topOffset; };

        QPainterPath waterPath;
        waterPath.moveTo(bucketLeftAt(bY + bH), bY + bH + 12);
        waterPath.lineTo(bucketRightAt(bY + bH), bY + bH + 12);
        waterPath.lineTo(bucketRightAt(waterY), (float)waterY);
        int wavePoints = 12;
        float waveAmp = 3.5f * animatedValue;
        for (int i = wavePoints; i >= 0; i--) {
            float xFrac = (float)i / wavePoints;
            float wx = bucketLeftAt(waterY) + xFrac * (bucketRightAt(waterY) - bucketLeftAt(waterY));
            float wy = waterY + waveAmp * sinf(ripplePhase + xFrac * 5.0f);
            waterPath.lineTo(wx, wy);
        }
        waterPath.lineTo(bucketLeftAt(bY + bH), bY + bH + 12);
        waterPath.closeSubpath();

        QLinearGradient waterGrad(0, waterY, 0, bY + bH);
        if (animatedValue > 0.9f) {
            waterGrad.setColorAt(0, QColor(30, 130, 210, 220));
            waterGrad.setColorAt(1, QColor(10, 80, 160, 240));
        } else {
            waterGrad.setColorAt(0, QColor(60, 160, 240, 210));
            waterGrad.setColorAt(0.5, QColor(40, 130, 210, 220));
            waterGrad.setColorAt(1, QColor(20, 90, 175, 235));
        }
        p.setBrush(waterGrad);
        p.setPen(Qt::NoPen);
        p.drawPath(waterPath);
        p.setClipping(false);
    }

    // դույլի եզրերը
    p.setPen(QPen(QColor(130, 155, 175), 2.5));
    p.setBrush(QColor(175, 190, 205));
    p.drawRoundedRect(bX - topOffset - 4, bY - 10, bW + topOffset * 2 + 8, 14, 4, 4);

    // դույլի բռնակները
    QPen handlePen(QColor(120, 145, 168), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(handlePen);
    p.setBrush(Qt::NoBrush);
    p.drawArc(bX + bW / 2 - 28, bY - 42, 56, 42, 0, 180 * 16);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(100, 125, 150));
    p.drawEllipse(bX + bW / 2 - 29, bY - 8, 8, 8);
    p.drawEllipse(bX + bW / 2 + 21, bY - 8, 8, 8);

    // անունը
    bool isX = bucketName.contains("X");
    QColor pillColor = isX ? QColor(255, 120, 60, 200) : QColor(60, 140, 255, 200);
    p.setBrush(pillColor);
    p.setPen(Qt::NoPen);
    int lblW = 90, lblH = 28;
    int lblX = (w - lblW) / 2;
    p.drawRoundedRect(lblX, 10, lblW, lblH, 14, 14);
    p.setPen(Qt::white);
    QFont labelFont("Georgia", 12, QFont::Bold);
    p.setFont(labelFont);
    p.drawText(QRect(lblX, 10, lblW, lblH), Qt::AlignCenter, bucketName);

    // լցվածույունը
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(230, 240, 252, 200));
    p.drawRoundedRect((w - 80) / 2, h - 42, 80, 24, 12, 12);
    p.setPen(QColor(50, 70, 90));
    QFont valFont("Courier New", 11, QFont::Bold);
    p.setFont(valFont);
    p.drawText(QRect((w - 80) / 2, h - 42, 80, 24), Qt::AlignCenter,
               QString("%1L / %2L").arg(currentValue).arg(maxCapacity));
}


PourArrowWidget::PourArrowWidget(QWidget* parent)
    : QWidget(parent), direction(PourDirection::None), animPhase(0.0f), visible(false) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    animTimer = new QTimer(this);
    animTimer->setInterval(25);
    connect(animTimer, &QTimer::timeout, this, [this]() {
        animPhase += 0.08f;
        if (animPhase > 2 * M_PI) animPhase -= 2 * M_PI;
        update();
    });
}

void PourArrowWidget::showPour(PourDirection dir) {
    direction = dir; visible = true; animPhase = 0.0f;
    animTimer->start(); show(); update();
    QTimer::singleShot(900, this, [this]() { hidePour(); });
}

void PourArrowWidget::hidePour() {
    visible = false; animTimer->stop(); update();
}

void PourArrowWidget::paintEvent(QPaintEvent*) {
    if (!visible || direction == PourDirection::None) return;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int w = width(), h = height();
    bool goingRight = (direction == PourDirection::XtoY);
    QString label = goingRight ? "POUR X→Y" : "POUR Y→X";
    QFont lf("Arial", 8, QFont::Bold);
    p.setFont(lf);
    p.setPen(QColor(100, 160, 230));
    p.drawText(QRect(0, 2, w, 14), Qt::AlignCenter, label);
    int cy = 18 + (h - 26) / 2;
    int startX = goingRight ? 10 : w - 10;
    int endX   = goingRight ? w - 10 : 10;
    p.setPen(QPen(QColor(60, 140, 255, 200), 3, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(Qt::NoBrush);
    p.drawLine(startX, cy, endX, cy);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(60, 140, 255, 210));
    float d2 = goingRight ? 1.0f : -1.0f;
    QPointF tip(endX, cy);
    QPolygonF head;
    head << tip << QPointF(tip.x() - d2*12, tip.y()-7) << QPointF(tip.x() - d2*12, tip.y()+7);
    p.drawPolygon(head);
    int numDrops = 4;
    for (int i = 0; i < numDrops; i++) {
        float t = fmod(animPhase / (2*M_PI) + (float)i / numDrops, 1.0f);
        float dropX = startX + t * (endX - startX);
        float alpha = qBound(0.0f, 1.0f - fabsf(t - 0.5f) * 1.8f, 1.0f);
        QRadialGradient dg(dropX, cy, 5);
        dg.setColorAt(0, QColor(100, 200, 255, (int)(200*alpha)));
        dg.setColorAt(1, QColor(30, 100, 200, 0));
        p.setBrush(dg);
        p.drawEllipse(QPointF(dropX, cy), 5, 5);
    }
}

static int calcOptimalSteps(int capA, int capB, int target);


static ProblemDefinition buildDynamicProblem(int capA, int capB, int target)
{
    QString title = QString("Դույլերի խնդիր (%1L + %2L → %3L)").arg(capA).arg(capB).arg(target);
    ProblemDefinition prob(
        QString("buckets_%1_%2_%3").arg(capA).arg(capB).arg(target).toStdString(),
        title.toStdString()
        );

    prob.addVariable("bucketX", "X դույլ", 0, 0, capA);
    prob.addVariable("bucketY", "Y դույլ", 0, 0, capB);

    int cA = capA, cB = capB;

    prob.addCommand("FILL X", "Լցնել X դույլը",
                    [cA](std::map<std::string,int> &s){ s["bucketX"] = cA; });
    prob.addCommand("FILL Y", "Լցնել Y դույլը",
                    [cB](std::map<std::string,int> &s){ s["bucketY"] = cB; });
    prob.addCommand("EMPTY X", "Դատարկել X դույլը",
                    [](std::map<std::string,int> &s){ s["bucketX"] = 0; });
    prob.addCommand("EMPTY Y", "Դատարկել Y դույլը",
                    [](std::map<std::string,int> &s){ s["bucketY"] = 0; });
    prob.addCommand("POUR X Y", "Լցնել X-ից Y",
                    [cB](std::map<std::string,int> &s){
                        int x = s["bucketX"], y = s["bucketY"];
                        int amt = std::min(x, cB - y);
                        s["bucketX"] -= amt; s["bucketY"] += amt;
                    });
    prob.addCommand("POUR Y X", "Լցնել Y-ից X",
                    [cA](std::map<std::string,int> &s){
                        int x = s["bucketX"], y = s["bucketY"];
                        int amt = std::min(y, cA - x);
                        s["bucketY"] -= amt; s["bucketX"] += amt;
                    });

    int tgt = target;
    prob.setGoal([tgt](const std::map<std::string,int> &s){
        return s.at("bucketX") == tgt || s.at("bucketY") == tgt;
    });

    return prob;
}


MainWindow::MainWindow(QWidget *parent, int capA, int capB, int target)
    : QMainWindow(parent),
    problem(nullptr), interpreter(nullptr),
    currentStep(0), isRunning(false), errorCountThisRun(0),
    m_capA(capA), m_capB(capB), m_target(target)
{
    static ProblemDefinition dynProb = buildDynamicProblem(capA, capB, target);
    problem = &dynProb;

    // BFS-ով հաշվել օպտիմալ քայլերի քանակը (հետագայում կօգտագործվի աստղերի համար)
    m_optimalSteps = calcOptimalSteps(capA, capB, target);

    interpreter = new Interpreter(problem);
    stepTimer   = new QTimer(this);
    stepTimer->setInterval(2200);

    setupUI();
    updateBucketDisplay();
    updateDescription();
    connect(stepTimer, &QTimer::timeout, this, &MainWindow::executeNextStep);
}

MainWindow::~MainWindow() {
    delete interpreter;
}


void MainWindow::setupUI() {
    setWindowTitle(QString("Դույլերի Խնդիր — %1L + %2L → %3L")
                       .arg(m_capA).arg(m_capB).arg(m_target));
    resize(1100, 750);

    setStyleSheet(R"(
        QMainWindow { background: #0d1b2a; }
        QWidget { background: transparent; color: #dce8f5;
                  font-family: 'Segoe UI', Arial, sans-serif; }
        QGroupBox {
            border: 1px solid rgba(100,160,220,0.3); border-radius: 10px;
            margin-top: 14px; padding: 10px;
            background: rgba(20,40,65,0.85);
            color: #90c8f0; font-weight: bold; font-size: 12px; }
        QGroupBox::title { subcontrol-origin: margin; padding: 0 8px; color: #6ab4e8; }
        QTextEdit {
            background: rgba(10,20,35,0.9); border: 1px solid rgba(80,140,200,0.4);
            border-radius: 8px; color: #c8e8ff;
            font-family: 'Courier New', monospace; font-size: 12px; padding: 6px;
            selection-background-color: #1a4f80; }
        QLabel { color: #c0d8f0; }
        QScrollBar:vertical { background: rgba(10,25,45,0.6); width: 8px; border-radius: 4px; }
        QScrollBar::handle:vertical { background: rgba(80,140,200,0.5); border-radius: 4px; }
    )");

    QWidget *central = new QWidget(this);
    central->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #0d1b2a,stop:0.5 #0f2235,stop:1 #0a1a30);");
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(16,16,16,16);
    mainLayout->setSpacing(16);

    //  ձախ հատված
    QVBoxLayout *leftPanel = new QVBoxLayout();
    leftPanel->setSpacing(12);

    QGroupBox *descGroup = new QGroupBox("Խնդրի Նկարագրություն");
    QVBoxLayout *descLayout = new QVBoxLayout(descGroup);
    descriptionLabel = new QLabel();
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color:#a8d0f0; font-size:12px; line-height:1.5;");
    descLayout->addWidget(descriptionLabel);
    leftPanel->addWidget(descGroup);

    QPushButton *starsInfoBtn = new QPushButton("★  Ինչպես վաստակել աստղեր");
    starsInfoBtn->setFixedHeight(32);
    starsInfoBtn->setStyleSheet(R"(
        QPushButton { background:rgba(245,197,24,0.12); border:1px solid rgba(245,197,24,0.4);
            border-radius:8px; color:#f5c518; font-size:12px; font-weight:bold; }
        QPushButton:hover { background:rgba(245,197,24,0.25); border-color:#f5c518; }
    )");
    connect(starsInfoBtn, &QPushButton::clicked, this, [this](){
        int opt = m_optimalSteps;
        int s5 = opt;
        int s4 = opt * 3 / 2;
        int s3 = opt * 2;
        int s2 = opt * 3;

        QDialog *dlg = new QDialog(this);
        dlg->setWindowTitle("Աստղերի Պայմաններ");
        dlg->setMinimumWidth(380);
        dlg->setStyleSheet("QDialog{background:#0d1b2a;} QLabel{color:#dce8f5;}"
                           "QPushButton{background:#1a4060;color:white;border-radius:8px;"
                           "padding:8px 24px;font-size:13px;font-weight:bold;}"
                           "QPushButton:hover{background:#225080;}");
        QVBoxLayout *lay = new QVBoxLayout(dlg);
        lay->setContentsMargins(24,20,24,20); lay->setSpacing(10);

        QLabel *title = new QLabel(
            QString("<h3 style='color:#f5c518;text-align:center;'>⭐ Աստղերի պայմաններ</h3>"
                    "<p style='color:#4a7090;font-size:11px;text-align:center;'>"
                    "Օպտիմալ լուծում՝ <b style='color:#f5c518;'>%1 քայլ</b></p>").arg(opt));
        title->setTextFormat(Qt::RichText);
        title->setAlignment(Qt::AlignCenter);
        lay->addWidget(title);

        QString table = QString(R"(
<table style='font-size:12px;line-height:2.4;width:100%;'>
  <tr>
    <td><span style='color:#f5c518;font-size:16px;'>★★★★★</span></td>
    <td style='color:#c8e8ff;'>&nbsp;= %1 քայլ &nbsp;<span style='color:#4a7090;'>(օպտիմալ)</span></td>
  </tr>
  <tr>
    <td><span style='color:#f5c518;font-size:16px;'>★★★★</span><span style='color:#2a3a4a;font-size:16px;'>★</span></td>
    <td style='color:#c8e8ff;'>&nbsp;≤ %2 քայլ</td>
  </tr>
  <tr>
    <td><span style='color:#f5c518;font-size:16px;'>★★★</span><span style='color:#2a3a4a;font-size:16px;'>★★</span></td>
    <td style='color:#c8e8ff;'>&nbsp;≤ %3 քայլ</td>
  </tr>
  <tr>
    <td><span style='color:#f5c518;font-size:16px;'>★★</span><span style='color:#2a3a4a;font-size:16px;'>★★★</span></td>
    <td style='color:#c8e8ff;'>&nbsp;≤ %4 քայլ</td>
  </tr>
  <tr>
    <td><span style='color:#f5c518;font-size:16px;'>★</span><span style='color:#2a3a4a;font-size:16px;'>★★★★</span></td>
    <td style='color:#c8e8ff;'>&nbsp;&gt; %4 քայլ</td>
  </tr>
</table>)").arg(s5).arg(s4).arg(s3).arg(s2);

        QLabel *c = new QLabel(table);
        c->setTextFormat(Qt::RichText);
        c->setWordWrap(true);
        lay->addWidget(c);

        QPushButton *ok = new QPushButton("Հասկացա");
        ok->setFixedHeight(36);
        connect(ok, &QPushButton::clicked, dlg, &QDialog::accept);
        lay->addWidget(ok, 0, Qt::AlignCenter);
        dlg->exec(); delete dlg;
    });
    leftPanel->addWidget(starsInfoBtn);

    QGroupBox *codeGroup = new QGroupBox("Ձեր Լուծումը  (մեկ հրաման / տող)");
    QVBoxLayout *codeLayout = new QVBoxLayout(codeGroup);
    codeEditor = new QTextEdit();
    codeEditor->setPlaceholderText(
        "Օրինակ:\nFILL Y\nPOUR Y X\nEMPTY X\nFILL Y\nPOUR Y X");
    codeEditor->setMinimumHeight(140);
    codeLayout->addWidget(codeEditor);
    leftPanel->addWidget(codeGroup);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    auto makeBtn = [](const QString &text, const QString &color) {
        QPushButton *btn = new QPushButton(text);
        btn->setStyleSheet(QString(R"(
            QPushButton { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);
                color:white; border:none; border-radius:8px; font-size:13px;
                font-weight:bold; padding:10px 16px; min-height:36px; }
            QPushButton:hover { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 %3,stop:1 %1); }
            QPushButton:pressed { padding-top:12px; padding-bottom:8px; }
            QPushButton:disabled { background:#2a3a4a; color:#556677; }
        )").arg(color,
                                    QColor(color).darker(140).name(),
                                    QColor(color).lighter(120).name()));
        return btn;
    };

    runButton   = makeBtn("Կատարել",       "#2e7d32");
    stepButton  = makeBtn("Քայլ առ Քայլ",  "#1565c0");
    resetButton = makeBtn("Վերսկսել",      "#e65100");
    runButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    stepButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    resetButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    btnLayout->addWidget(runButton);
    btnLayout->addWidget(stepButton);
    btnLayout->addWidget(resetButton);
    leftPanel->addLayout(btnLayout);

    QGroupBox *outGroup = new QGroupBox("Ելք");
    QVBoxLayout *outLayout = new QVBoxLayout(outGroup);
    outputConsole = new QTextEdit();
    outputConsole->setReadOnly(true);
    outputConsole->setMaximumHeight(150);
    outLayout->addWidget(outputConsole);
    leftPanel->addWidget(outGroup);

    mainLayout->addLayout(leftPanel, 2);

    // աջ հատված
    QVBoxLayout *rightPanel = new QVBoxLayout();
    rightPanel->setSpacing(10);

    QGroupBox *vizGroup = new QGroupBox("Տեսողական Ցուցադրում");
    QVBoxLayout *vizLayout = new QVBoxLayout(vizGroup);
    vizLayout->setSpacing(10);

    statusLabel = new QLabel("💧 Սկզբնական վիճակ");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "QLabel { background:rgba(25,55,90,0.8); border:1px solid rgba(80,160,240,0.4);"
        " border-radius:10px; padding:8px; font-size:13px; font-weight:bold; color:#90d0ff; }");

    QWidget *bucketsContainer = new QWidget();
    bucketsContainer->setMinimumHeight(380);
    bucketsContainer->setMinimumWidth(260);
    bucketsContainer->setStyleSheet("background:transparent;");
    QHBoxLayout *bucketsLayout = new QHBoxLayout(bucketsContainer);
    bucketsLayout->setContentsMargins(8, 8, 8, 8);
    bucketsLayout->setSpacing(0);

    bucketX = new BucketWidget("X դույլ", m_capA);
    bucketX->setMinimumWidth(110);
    bucketY = new BucketWidget("Y դույլ", m_capB);
    bucketY->setMinimumWidth(110);

    pourArrow = new PourArrowWidget(bucketsContainer);
    pourArrow->setFixedSize(60, 50);
    pourArrow->hide();

    bucketsLayout->addWidget(bucketX, 1);
    bucketsLayout->addWidget(pourArrow, 0, Qt::AlignCenter);
    bucketsLayout->addWidget(bucketY, 1);

    vizLayout->addWidget(bucketsContainer);
    vizLayout->addWidget(statusLabel);
    rightPanel->addWidget(vizGroup);
    mainLayout->addLayout(rightPanel, 1);

    connect(runButton,   &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(stepButton,  &QPushButton::clicked, this, &MainWindow::onStepClicked);
}

void MainWindow::updateBucketDisplay() {
    auto state = interpreter->getState();
    bucketX->setValue(state["bucketX"]);
    bucketY->setValue(state["bucketY"]);
    bucketX->setCapacity(problem->variables[0].maxValue);
    bucketY->setCapacity(problem->variables[1].maxValue);
}

void MainWindow::triggerPourAnimation(const std::string &cmd) {
    QString qcmd = QString::fromStdString(cmd).toUpper().simplified();
    if      (qcmd == "POUR X Y") pourArrow->showPour(PourDirection::XtoY);
    else if (qcmd == "POUR Y X") pourArrow->showPour(PourDirection::YtoX);
}

void MainWindow::updateDescription() {
    QString desc = QString(
                       "<div style='line-height:1.7; color:#a8d0f0;'>"
                       "<p>Տրված են երկու դույլ <b style='color:#ff9040'>%1L</b> "
                       "և <b style='color:#40a0ff'>%2L</b> տարողությամբ։<br>"
                       "Նպատակ՝ ստանալ <b style='color:#50e080'>%3L</b> ջուր X կամ Y դույլում։</p>"
                       "<p><b style='color:#80c0ff'>Հրամաններ:</b><br>"
                       "<span style='color:#ffd080'>FILL X / FILL Y</span> — լցնել դույլը ամբողջությամբ<br>"
                       "<span style='color:#ffd080'>EMPTY X / EMPTY Y</span> — դատարկել դույլը<br>"
                       "<span style='color:#ffd080'>POUR X Y / POUR Y X</span> — լցնել մեկից մյուսը</p>"
                       "</div>"
                       ).arg(m_capA).arg(m_capB).arg(m_target);
    descriptionLabel->setText(desc);
}

void MainWindow::showMessage(const QString &msg, const QString &color) {
    outputConsole->append(
        QString("<span style='color:%1;font-family:Courier New;'>%2</span>")
            .arg(color, msg));
}

void MainWindow::onRunClicked() {
    if (isRunning) return;
    onResetClicked();
    QString code = codeEditor->toPlainText();
    istringstream stream(code.toStdString());
    string line; commands.clear();
    while (getline(stream, line)) {
        size_t s = line.find_first_not_of(" \t\r\n");
        if (s == string::npos) continue;
        line = line.substr(s);
        size_t e = line.find_last_not_of(" \t\r\n");
        line = line.substr(0, e + 1);
        if (!line.empty() && line[0] != '#'
            && !(line.size() >= 2 && line[0] == '/' && line[1] == '/'))
            commands.push_back(line);
    }
    if (commands.empty()) { QMessageBox::warning(this, "Սխալ", "Մուտքագրեք հրամաններ!"); return; }
    outputConsole->clear();
    showMessage("Կատարում...", "#4090d0");
    currentStep = 0; isRunning = true; errorCountThisRun = 0;
    runButton->setEnabled(false); stepButton->setEnabled(false);
    codeEditor->setEnabled(false);
    stepTimer->start();
}

void MainWindow::onStepClicked() {
    if (isRunning) return;
    if (currentStep == 0) {
        onResetClicked();
        QString code = codeEditor->toPlainText();
        istringstream stream(code.toStdString());
        string line; commands.clear();
        while (getline(stream, line)) {
            size_t s = line.find_first_not_of(" \t\r\n");
            if (s == string::npos) continue;
            line = line.substr(s);
            size_t e = line.find_last_not_of(" \t\r\n");
            line = line.substr(0, e + 1);
            if (!line.empty() && line[0] != '#'
                && !(line.size() >= 2 && line[0] == '/' && line[1] == '/'))
                commands.push_back(line);
        }
        if (commands.empty()) { QMessageBox::warning(this, "Սխալ", "Մուտքագրեք հրամաններ!"); return; }
        outputConsole->clear(); errorCountThisRun = 0;
        showMessage("Քայլ առ Քայլ", "#4090d0");
    }
    if (currentStep < (int)commands.size() && !interpreter->isSolved())
        executeNextStep();
}

void MainWindow::onResetClicked() {
    stepTimer->stop(); isRunning = false;
    currentStep = 0; commands.clear();
    delete interpreter;
    interpreter = new Interpreter(problem);
    updateBucketDisplay();
    outputConsole->clear();
    statusLabel->setText("💧 Սկզբնական վիճակ");
    statusLabel->setStyleSheet(
        "QLabel { background:rgba(25,55,90,0.8); border:1px solid rgba(80,160,240,0.4);"
        " border-radius:10px; padding:12px; font-size:15px; font-weight:bold; color:#90d0ff; }");
    runButton->setEnabled(true); stepButton->setEnabled(true);
    codeEditor->setEnabled(true);
}

void MainWindow::executeNextStep() {
    if (currentStep >= (int)commands.size()) {
        stepTimer->stop(); isRunning = false;
        checkFinalResult(); return;
    }

    string cmd = commands[currentStep];
    triggerPourAnimation(cmd);

    auto state = interpreter->getState();
    int xCap = problem->variables[0].maxValue;
    int yCap = problem->variables[1].maxValue;
    int xVal = state["bucketX"], yVal = state["bucketY"];
    QString qcmdU = QString::fromStdString(cmd).toUpper().simplified();
    QString warn;

    if      (qcmdU=="FILL X"  && xVal==xCap) warn = QString("FILL X — X դույլն արդեն լիքն է (%1L)!").arg(xCap);
    else if (qcmdU=="FILL Y"  && yVal==yCap) warn = QString("FILL Y — Y դույլն արդեն լիքն է (%1L)!").arg(yCap);
    else if (qcmdU=="EMPTY X" && xVal==0)    warn = "EMPTY X — X դույլն արդեն դատարկ է!";
    else if (qcmdU=="EMPTY Y" && yVal==0)    warn = "EMPTY Y — Y դույլն արդեն դատարկ է!";
    else if (qcmdU=="POUR X Y"&& xVal==0)    warn = "POUR X Y — X դույլը դատարկ է!";
    else if (qcmdU=="POUR X Y"&& yVal==yCap) warn = "POUR X Y — Y դույլն արդեն լիքն է!";
    else if (qcmdU=="POUR Y X"&& yVal==0)    warn = "POUR Y X — Y դույլը դատարկ է!";
    else if (qcmdU=="POUR Y X"&& xVal==xCap) warn = "POUR Y X — X դույլն արդեն լիքն է!";

    if (!warn.isEmpty()) {
        stepTimer->stop(); isRunning = false;
        runButton->setEnabled(true); stepButton->setEnabled(true);
        codeEditor->setEnabled(true);
        errorCountThisRun++;
        showMessage("⚠ Քայլ " + QString::number(currentStep+1) + ": " + warn, "#ff9944");
        statusLabel->setText("⚠ Անիմաստ հրաման");
        statusLabel->setStyleSheet(
            "QLabel { background:rgba(80,50,10,0.8); border:1px solid #aa6622;"
            " border-radius:10px; padding:12px; font-size:15px; font-weight:bold; color:#ffb070; }");
        return;
    }

    if (interpreter->executeCommand(cmd)) {
        updateBucketDisplay();
        auto ns = interpreter->getState();
        showMessage(QString("  Քայլ %1: %2  → X=%3L  Y=%4L")
                        .arg(currentStep+1)
                        .arg(QString::fromStdString(cmd))
                        .arg(ns["bucketX"]).arg(ns["bucketY"]), "#70a8d0");
    } else {
        stepTimer->stop(); isRunning = false;
        runButton->setEnabled(true); stepButton->setEnabled(true);
        codeEditor->setEnabled(true);
        showMessage("❌ Անհայտ հրաման: " + QString::fromStdString(cmd), "#ff5555");
        statusLabel->setText("❌ Սխալ հրաման");
        statusLabel->setStyleSheet(
            "QLabel { background:rgba(80,10,10,0.8); border:1px solid #aa2222;"
            " border-radius:10px; padding:12px; font-size:15px; font-weight:bold; color:#ff7070; }");
        currentStep++; return;
    }

    currentStep++;
    if (currentStep >= (int)commands.size()) {
        stepTimer->stop(); isRunning = false;
        checkFinalResult();
    }
}

//  BFS — հաշվել օպտիմալ քայլերի քանակը

static int calcOptimalSteps(int capA, int capB, int target)
{
    if (capA == target || capB == target) return 1;

    using State = QPair<int,int>;
    QMap<State, int> dist;
    QQueue<State> q;
    State start{0, 0};
    dist[start] = 0;
    q.enqueue(start);

    while (!q.isEmpty()) {
        auto [a, b] = q.dequeue();
        int d = dist[{a, b}];

        auto tryNext = [&](int na, int nb) {
            State ns{na, nb};
            if (!dist.contains(ns)) {
                dist[ns] = d + 1;
                if (na == target || nb == target) return;
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

    // Գտնել կարճագույն հեռավորությունը դեպի target
    int best = INT_MAX;
    for (auto it = dist.begin(); it != dist.end(); ++it) {
        if (it.key().first == target || it.key().second == target)
            best = std::min(best, it.value());
    }
    return best == INT_MAX ? -1 : best;
}


int MainWindow::computeStars(int usedSteps) const {
    int opt = m_optimalSteps;
    if (opt <= 0) return 1;

    if (usedSteps <= opt)           return 5;
    if (usedSteps <= opt * 3 / 2)   return 4;
    if (usedSteps <= opt * 2)        return 3;
    if (usedSteps <= opt * 3)        return 2;
    return 1;
}

void MainWindow::checkFinalResult() {
    runButton->setEnabled(true); stepButton->setEnabled(true);
    codeEditor->setEnabled(true);

    if (!interpreter->isSolved()) {
        auto fs = interpreter->getState();
        showMessage(QString("❌ Կոդի կատարումն ավարտվեց, բայց %1L-ը չի ստացվել։ "
                            "X=%2L, Y=%3L").arg(m_target).arg(fs["bucketX"]).arg(fs["bucketY"]),
                    "#ff4444");
        statusLabel->setText("❌ Ձախողում");
        statusLabel->setStyleSheet(
            "QLabel { background:rgba(80,10,10,0.8); border:1px solid #aa2222;"
            " border-radius:10px; padding:12px; font-size:15px; font-weight:bold; color:#ff7070; }");
        return;
    }

    int usedSteps = (int)commands.size();
    if (usedSteps < m_bestSteps) m_bestSteps = usedSteps;
    m_solved = true;

    int stars = computeStars(usedSteps);
    showMessage(QString("✅ Շնորհավորում եմ! %1L ստացվեց %2 քայլով!")
                    .arg(m_target).arg(usedSteps), "#44ee88");
    statusLabel->setText("✅ Լուծված!");
    statusLabel->setStyleSheet(
        "QLabel { background:rgba(10,60,25,0.9); border:1px solid #30cc66;"
        " border-radius:10px; padding:12px; font-size:15px; font-weight:bold; color:#50ff90; }");

    showStarDialog(stars);
}

void MainWindow::showStarDialog(int stars) {
    // Save to shared DB
    ResultSaver::save("bucket", stars);

    QString starRow;
    for (int i = 0; i < 5; i++)
        starRow += (i < stars)
                       ? "<span style='color:#f5c518;font-size:48px;'>★</span>"
                       : "<span style='color:#2a3a4a;font-size:48px;'>★</span>";

    QStringList msgs = { "",
        "Լուծված է։ Փորձեք ավելի կարճ ճանապարհ գտնել։",
        "Ճիշտ ուղղությամբ եք, բայց կա բարելավման տեղ։",
        "Լավ փորձ։ Փորձեք ավելի արդյունավետ կոդ գրել։",
        "Շատ լավ! Մի փոքր կրճատումով՝ 5 աստղ կստանայիք։",
        "Կատարյալ լուծում! Չի կարող ավելի լավ լինել! 🏆"
    };

    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle(stars == 5 ? "🏆 ԿԱՏԱՐՅԱԼ!" : "Հաջողություն!");
    dlg->setMinimumWidth(320);
    dlg->setStyleSheet(
        "QDialog{background:#0d1b2a;} QLabel{color:#dce8f5;}"
        "QPushButton{background:#1a6040;color:white;border-radius:8px;"
        "padding:8px 28px;font-size:14px;font-weight:bold;}"
        "QPushButton:hover{background:#22804e;}");

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    lay->setContentsMargins(28,22,28,22); lay->setSpacing(10);

    auto mkL = [&](const QString &html){
        QLabel *l = new QLabel(html);
        l->setTextFormat(Qt::RichText);
        l->setAlignment(Qt::AlignCenter);
        l->setWordWrap(true);
        return l;
    };

    lay->addWidget(mkL(starRow));
    lay->addWidget(mkL(QString("<p style='font-size:22px;font-weight:bold;color:#f5c518;'>%1 / 5</p>").arg(stars)));
    lay->addWidget(mkL(QString("<p style='color:#90c8f0;font-size:13px;'>Ձեր քայլերը՝ <b>%1</b> &nbsp;|&nbsp; Օպտիմալ՝ <b>%2</b></p>")
                           .arg((int)commands.size()).arg(m_optimalSteps)));
    QFrame *sep = new QFrame(); sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:rgba(80,140,200,0.3);");
    lay->addWidget(sep);
    lay->addWidget(mkL(QString("<p style='color:#a8d0f0;font-size:13px;'>%1</p>").arg(msgs[qBound(1,stars,5)])));

    QPushButton *retryBtn = new QPushButton("Կրկին փորձել");
    QPushButton *closeBtn = new QPushButton("Փակել");
    retryBtn->setFixedHeight(40);
    closeBtn->setFixedHeight(40);
    retryBtn->setStyleSheet(
        "QPushButton{background:#0d2a40;color:#90c8f0;border-radius:8px;"
        "padding:8px 24px;font-size:13px;font-weight:bold;border:1px solid #1a5070;}"
        "QPushButton:hover{background:#1a3a50;}");
    connect(retryBtn, &QPushButton::clicked, dlg, &QDialog::reject);
    connect(closeBtn, &QPushButton::clicked, dlg, &QDialog::accept);

    QHBoxLayout *bl = new QHBoxLayout();
    bl->addStretch();
    bl->addWidget(retryBtn);
    bl->addSpacing(12);
    bl->addWidget(closeBtn);
    bl->addStretch();
    lay->addLayout(bl);

    int choice = dlg->exec();
    delete dlg;

    if (choice == QDialog::Rejected) {
        codeEditor->clear();
        onResetClicked();
    } else {
        close();
    }
}
