// #include "mainwindow.h"
// #include "resultsaver.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QGroupBox>
// #include <QSplitter>
// #include <QMessageBox>
// #include <QFont>
// #include <QGraphicsDropShadowEffect>
// #include <QtMath>
// #include <QPainterPath>
// #include <QPainter>
// #include <QMap>
// #include <QScrollArea>
// #include <QDialog>
// #include <QLabel>
// #include <QFrame>
// #include <QApplication>

// BoatGraphicsItem::BoatGraphicsItem(Boat *boat, QGraphicsItem *parent)
//     : QGraphicsItem(parent)
//     , m_boat(boat)
// {
//     setZValue(10);
//     m_label = new QGraphicsTextItem(boat->name(), this);
//     m_label->setDefaultTextColor(Qt::white);
//     QFont font = m_label->font();
//     font.setBold(true);
//     font.setPointSize(7);
//     m_label->setFont(font);

//     QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect();
//     textShadow->setBlurRadius(6);
//     textShadow->setColor(QColor(0, 0, 0, 200));
//     textShadow->setOffset(1, 1);
//     m_label->setGraphicsEffect(textShadow);

//     updatePosition();
// }

// void BoatGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
// {
//     Q_UNUSED(option)
//     Q_UNUSED(widget)

//     painter->setRenderHint(QPainter::Antialiasing);

//     qreal boatLength = 70;
//     qreal boatWidth = 30;

//     bool movingRight = true;
//     if (m_boat->isMoving()) {
//         if (!m_boat->targetSide().isEmpty()) {
//             movingRight = (m_boat->targetSide() == "B");
//         } else {
//             movingRight = (m_boat->side() == "A");
//         }
//     } else {
//         movingRight = (m_boat->side() == "A");
//     }

//     painter->save();

//     if (!movingRight) {
//         painter->scale(-1, 1);
//     }

//     // Ստվեր
//     painter->setBrush(QBrush(QColor(0, 0, 0, 80)));
//     painter->setPen(Qt::NoPen);
//     QRectF shadowRect(-boatLength/2 + 5, boatWidth/2 + 3, boatLength - 10, 8);
//     painter->drawEllipse(shadowRect);

//     // Նավակի արտաքին տեսքը
//     QPainterPath hull;
//     hull.moveTo(-boatLength/2 + 5, 0);
//     hull.quadTo(-boatLength/2, -boatWidth/3, -boatLength/2 + 5, -boatWidth/2);
//     hull.lineTo(boatLength/2 - 8, -boatWidth/2);
//     hull.quadTo(boatLength/2 - 3, -boatWidth/3, boatLength/2, 0);
//     hull.quadTo(boatLength/2 - 3, boatWidth/3, boatLength/2 - 8, boatWidth/2);
//     hull.lineTo(-boatLength/2 + 5, boatWidth/2);
//     hull.quadTo(-boatLength/2, boatWidth/3, -boatLength/2 + 5, 0);

//     // Նավակի հիմնական գույնը
//     QLinearGradient hullGradient(-boatLength/2, -boatWidth/2, -boatLength/2, boatWidth/2);
//     hullGradient.setColorAt(0, m_boat->color().lighter(140));
//     hullGradient.setColorAt(0.3, m_boat->color().lighter(120));
//     hullGradient.setColorAt(0.7, m_boat->color());
//     hullGradient.setColorAt(1, m_boat->color().darker(120));

//     painter->setBrush(QBrush(hullGradient));
//     painter->setPen(QPen(m_boat->color().darker(160), 2.5));
//     painter->drawPath(hull);

//     // Սպիտակ գծիկները
//     painter->setPen(QPen(QColor(255, 255, 255, 200), 2));
//     painter->drawLine(QPointF(-boatLength/2 + 10, -boatWidth/2 + 3),
//                       QPointF(boatLength/2 - 10, -boatWidth/2 + 3));

//     //պատուհանների տիրույթը
//     QRectF cabin(-15, -boatWidth/2.5, 30, boatWidth/1.5);
//     QLinearGradient cabinGradient(-15, -boatWidth/2.5, -15, boatWidth/3);
//     cabinGradient.setColorAt(0, m_boat->color().lighter(110));
//     cabinGradient.setColorAt(1, m_boat->color().darker(130));
//     painter->setBrush(QBrush(cabinGradient));
//     painter->setPen(QPen(m_boat->color().darker(180), 2));
//     painter->drawRoundedRect(cabin, 4, 4);

//     // պատուհանները
//     painter->setBrush(QBrush(QColor(150, 200, 255, 220)));
//     painter->setPen(QPen(QColor(100, 150, 200), 1));
//     painter->drawRoundedRect(QRectF(-10, -5, 7, 7), 2, 2);
//     painter->drawRoundedRect(QRectF(3, -5, 7, 7), 2, 2);

//     // ծխնելույզը
//     QRectF chimney(boatLength/2 - 18, -boatWidth/2 - 8, 4, 10);
//     painter->setBrush(QBrush(m_boat->color().darker(140)));
//     painter->setPen(QPen(m_boat->color().darker(200), 1));
//     painter->drawRect(chimney);

//     // ծուխը
//     if (m_boat->isMoving()) {
//         painter->setBrush(QBrush(QColor(200, 200, 200, 150)));
//         painter->setPen(Qt::NoPen);
//         painter->drawEllipse(QPointF(boatLength/2 - 16, -boatWidth/2 - 12), 3, 3);
//         painter->drawEllipse(QPointF(boatLength/2 - 14, -boatWidth/2 - 16), 4, 4);
//     }

//     // Բեռնված լինելու դեպքում բեռը պետք է արկղերով երևա
//     if (m_boat->cargoWeight() > 0) {
//         int numCrates = qMin(3, m_boat->cargoWeight() / 50); // 1-3 տուփ, կախված բեռների քաշից

//         for (int i = 0; i < numCrates; i++) {
//             double crateX = -28 - (i * 12);
//             double crateY = -6;

//             // տուփը
//             painter->setBrush(QBrush(QColor(160, 120, 60)));
//             painter->setPen(QPen(QColor(120, 90, 40), 1.5));
//             painter->drawRect(QRectF(crateX, crateY, 10, 10));

//             // փայտե տախտակները
//             painter->setPen(QPen(QColor(80, 60, 30), 1));
//             painter->drawLine(QPointF(crateX, crateY + 5), QPointF(crateX + 10, crateY + 5));
//             painter->drawLine(QPointF(crateX + 5, crateY), QPointF(crateX + 5, crateY + 10));
//         }

//         //բեռի տեսակը՝ փոքր կլոր գույնով
//         QColor cargoColor;
//         QString cargoType = m_boat->cargoType();
//         if (cargoType.contains("FOOD")) cargoColor = QColor(255, 200, 100);
//         else if (cargoType.contains("MED")) cargoColor = QColor(255, 100, 100);
//         else if (cargoType.contains("WATER")) cargoColor = QColor(100, 150, 255);
//         else if (cargoType.contains("BUILDING")) cargoColor = QColor(150, 150, 150);
//         else cargoColor = QColor(200, 200, 0);

//         painter->setBrush(QBrush(cargoColor));
//         painter->setPen(Qt::NoPen);
//         painter->drawEllipse(QPointF(-23, 0), 4, 4);
//     }

//     // նավի հետևի գծիկները շարժվելու ժամանակ
//     if (m_boat->isMoving()) {
//         painter->setPen(QPen(QColor(255, 255, 255, 120), 2));
//         painter->setBrush(Qt::NoBrush);

//         painter->drawArc(QRectF(-boatLength/2 - 10, -10, 20, 20), 0, 180 * 16);
//         painter->drawArc(QRectF(-boatLength/2 - 15, -8, 16, 16), 0, 180 * 16);
//     }

//     painter->restore();

//     if (m_boat->isMoving()) { // եթե ծարժվում է, անունը գլխին ա գրված լինում
//         m_label->setPos(-m_label->boundingRect().width() / 2, -35);
//     } else {                  //նորմալ ժամանակ կողքին ա գրված լինում
//         m_label->setPos(40, -8);
//     }
// }

// QRectF BoatGraphicsItem::boundingRect() const
// {
//     return QRectF(-50, -40, 150, 80);
// }

// void BoatGraphicsItem::updatePosition()
// {
//     // Նավակի ինդեքսը օգտագործում ենք իր դիրքը ուղղղահայաց ուղղությամբ ունենալու համար
//     static QMap<QString, int> boatIndex;

//     QString boatId = m_boat->id();

//     // Սահմանում ենք նավակի ինդեքսները սկզբնական վիճակի համար
//     if (!boatIndex.contains(boatId)) {
//         int idx = 0;
//         if      (boatId == "B1") idx = 0;
//         else if (boatId == "B2") idx = 1;
//         else if (boatId == "B3") idx = 2;
//         else if (boatId == "B4") idx = 0;
//         else if (boatId == "B5") idx = 1;
//         else if (boatId == "B6") idx = 2;
//         else if (boatId == "B7") idx = 3;
//         boatIndex[boatId] = idx;
//     }

//     int idx = boatIndex[boatId];

//     if (!m_boat->isMoving()) {
//         double x, y;
//         QString currentSide = m_boat->side();
//         double verticalSpacing = 75;

//         QString boatId = m_boat->id();
//         bool isFromSideA = (boatId == "B1" || boatId == "B2" || boatId == "B3");

//         if (currentSide == "A") {
//             x = 60;
//         } else {
//             x = 910;
//         }

//         if (isFromSideA)
//         {
//             y = 248 + idx * verticalSpacing;
//         } else {
//             y = 210 + idx * verticalSpacing;
//         }

//         setPos(x, y);
//         setRotation(0);
//         m_label->setPos(45, -8);
//     }
//     else {
//         double progress = m_boat->progress();
//         double posX = m_boat->positionX();

//         double leftX = 60;
//         double rightX = 910;
//         double x = leftX + posX * (rightX - leftX);

//         QString boatId = m_boat->id();
//         double baseY = 210;

//         if (boatId == "B1" || boatId == "B2" || boatId == "B3") {
//             baseY = 248;
//         }

//         double laneY = baseY + idx * 75;

//         // ալիքաձև անցում
//         double y = laneY + qSin(progress * M_PI * 4) * 5;

//         setPos(x, y);
//         setRotation(qSin(progress * M_PI * 8) * 3);
//         m_label->setPos(-m_label->boundingRect().width() / 2, -35);
//     }

//     update();
// }

// MainWindow::MainWindow(Simulator *simulator, QWidget *parent)
//     : QMainWindow(parent)
//     , m_simulator(simulator)
// {
//     setWindowTitle("🌉 Հումանիտար Կամրջային Լոգիստիկայի Սիմուլյատոր");
//     setMinimumSize(1100, 700);
//     resize(1200, 800);

//     setStyleSheet(
//         "QMainWindow { background-color: #0f1419; }"
//         "QGroupBox { "
//         "   color: #cdd6f4; "
//         "   border: 2px solid #89b4fa; "
//         "   border-radius: 8px; "
//         "   margin-top: 10px; "
//         "   font-weight: bold; "
//         "   padding-top: 10px; "
//         "}"
//         "QGroupBox::title { "
//         "   subcontrol-origin: margin; "
//         "   left: 10px; "
//         "   padding: 0 5px; "
//         "}"
//         "QPushButton { "
//         "   background-color: #89b4fa; "
//         "   color: #1e1e2e; "
//         "   border: none; "
//         "   border-radius: 6px; "
//         "   padding: 8px 16px; "
//         "   font-weight: bold; "
//         "   font-size: 13px; "
//         "}"
//         "QPushButton:hover { background-color: #b4befe; }"
//         "QPushButton:pressed { background-color: #74c7ec; }"
//         "QPushButton:disabled { background-color: #45475a; color: #6c7086; }"
//         "QTextEdit, QListWidget { "
//         "   background-color: #181825; "
//         "   color: #cdd6f4; "
//         "   border: 2px solid #313244; "
//         "   border-radius: 6px; "
//         "   padding: 8px; "
//         "}"
//         "QLabel { color: #cdd6f4; font-size: 13px; }"
//         "QSpinBox { "
//         "   background-color: #313244; "
//         "   color: #cdd6f4; "
//         "   border: 2px solid #45475a; "
//         "   border-radius: 4px; "
//         "   padding: 4px; "
//         "}"
//         "QProgressBar { "
//         "   border: 2px solid #45475a; "
//         "   border-radius: 6px; "
//         "   text-align: center; "
//         "   background-color: #313244; "
//         "   color: #cdd6f4; "
//         "}"
//         "QProgressBar::chunk { "
//         "   background-color: #a6e3a1; "
//         "   border-radius: 4px; "
//         "}"
//         );

//     setupUI();
//     setupVisualization();

//     connect(m_simulator, &Simulator::currentTimeChanged, this, &MainWindow::updateUI);
//     connect(m_simulator, &Simulator::logMessage, this, &MainWindow::onLogMessage);
//     connect(m_simulator, &Simulator::simulationFinished, this, &MainWindow::onSimulationFinished);
//     connect(m_simulator, &Simulator::isRunningChanged, this, &MainWindow::updateUI);
//     connect(m_simulator, &Simulator::completedBoatsChanged, this, &MainWindow::updateUI);
//     connect(m_simulator, &Simulator::collisionsChanged, this, &MainWindow::updateUI);
//     connect(m_simulator, &Simulator::bridgeHealthChanged, this, &MainWindow::updateUI);

//     m_visualTimer = new QTimer(this);
//     connect(m_visualTimer, &QTimer::timeout, this, &MainWindow::updateVisualization);
//     m_visualTimer->start(30);

//     updateBoatsList();
//     updateUI();
// }

// MainWindow::~MainWindow()
// {
// }

// void MainWindow::setupUI()
// {
//     QWidget *centralWidget = new QWidget(this);
//     setCentralWidget(centralWidget);

//     QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
//     mainLayout->setSpacing(8);
//     mainLayout->setContentsMargins(8, 8, 8, 8);

//     QHBoxLayout *controlLayout = new QHBoxLayout();
//     controlLayout->setSpacing(10);

//     m_startButton = new QPushButton(" Մեկնարկ", this);
//     m_pauseButton = new QPushButton(" Դադար", this);
//     m_resetButton = new QPushButton(" Վերսկսել", this);
//     m_helpButton = new QPushButton(" Օգնություն", this);
//     m_problemButton = new QPushButton(" Խնդրի Նկարագրություն", this);

//     QStyle *appStyle = QApplication::style();
//     m_startButton->setIcon(appStyle->standardIcon(QStyle::SP_MediaPlay));
//     m_pauseButton->setIcon(appStyle->standardIcon(QStyle::SP_MediaPause));
//     m_resetButton->setIcon(appStyle->standardIcon(QStyle::SP_BrowserReload));
//     m_helpButton->setIcon(appStyle->standardIcon(QStyle::SP_DialogHelpButton));
//     m_problemButton->setIcon(appStyle->standardIcon(QStyle::SP_FileDialogDetailedView));

//     for (QPushButton *btn : {m_startButton, m_pauseButton,
//                              m_resetButton, m_helpButton, m_problemButton}) {
//         btn->setIconSize(QSize(18, 18));
//     }


//     m_startButton->setMinimumHeight(35);
//     m_pauseButton->setMinimumHeight(35);
//     m_resetButton->setMinimumHeight(35);
//     m_helpButton->setMinimumHeight(35);
//     m_problemButton->setMinimumHeight(35);

//     connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
//     connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
//     connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
//     connect(m_helpButton, &QPushButton::clicked, this, &MainWindow::onShowHelp);
//     connect(m_problemButton, &QPushButton::clicked, this, &MainWindow::onShowProblem);

//     controlLayout->addWidget(m_startButton);
//     controlLayout->addWidget(m_pauseButton);
//     controlLayout->addWidget(m_resetButton);
//     controlLayout->addWidget(m_helpButton);
//     controlLayout->addWidget(m_problemButton);
//     controlLayout->addStretch();

//     mainLayout->addLayout(controlLayout);

//     QHBoxLayout *statusLayout = new QHBoxLayout();
//     statusLayout->setSpacing(20);

//     m_timeLabel = new QLabel("Ժամանակ: 0.0 րոպե", this);
//     m_statusLabel = new QLabel("Կարգավիճակ: Պատրաստ", this);
//     m_completedLabel = new QLabel("Ավարտված: 0/0", this);
//     m_collisionsLabel = new QLabel("Բախումներ: 0", this);

//     QFont boldFont;
//     boldFont.setBold(true);
//     boldFont.setPointSize(11);
//     m_timeLabel->setFont(boldFont);
//     m_statusLabel->setFont(boldFont);
//     m_completedLabel->setFont(boldFont);
//     m_collisionsLabel->setFont(boldFont);

//     statusLayout->addWidget(m_timeLabel);
//     statusLayout->addWidget(m_statusLabel);
//     statusLayout->addWidget(m_completedLabel);
//     statusLayout->addWidget(m_collisionsLabel);
//     statusLayout->addStretch();

//     QLabel *bridgeLabel = new QLabel("Կամուրջ:", this);
//     bridgeLabel->setStyleSheet("font-weight: bold;");
//     m_bridgeHealthBar = new QProgressBar(this);
//     m_bridgeHealthBar->setRange(0, 100);
//     m_bridgeHealthBar->setValue(100);
//     m_bridgeHealthBar->setTextVisible(true);
//     m_bridgeHealthBar->setMaximumWidth(200);
//     m_bridgeHealthBar->setMinimumHeight(20);

//     statusLayout->addWidget(bridgeLabel);
//     statusLayout->addWidget(m_bridgeHealthBar);

//     mainLayout->addLayout(statusLayout);

//     QGroupBox *visualGroup = new QGroupBox("Արտապատկերում", this);
//     QVBoxLayout *visualLayout = new QVBoxLayout(visualGroup);

//     m_graphicsView = new QGraphicsView(this);
//     m_graphicsView->setRenderHint(QPainter::Antialiasing);
//     m_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
//     m_graphicsView->setStyleSheet(
//         "QGraphicsView { "
//         "   background: #1e3a5f; "
//         "   border: 3px solid #89b4fa; "
//         "   border-radius: 10px; "
//         "}"
//         );
//     m_graphicsView->setMinimumHeight(300);
//     m_graphicsView->setMaximumHeight(400);
//     m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//     m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//     visualLayout->addWidget(m_graphicsView);
//     mainLayout->addWidget(visualGroup);

//     QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

//     QGroupBox *boatsGroup = new QGroupBox("Նավակներ", this);
//     QVBoxLayout *boatsLayout = new QVBoxLayout(boatsGroup);
//     m_boatsList = new QListWidget(this);
//     m_boatsList->setWordWrap(true);
//     m_boatsList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//     m_boatsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//     boatsLayout->addWidget(m_boatsList);
//     splitter->addWidget(boatsGroup);

//     QGroupBox *scriptGroup = new QGroupBox("📝 Սցենար ", this);
//     QVBoxLayout *scriptLayout = new QVBoxLayout(scriptGroup);
//     m_scriptEditor = new QTextEdit(this);
//     m_scriptEditor->setPlaceholderText("Այստեղ մուտքագրեք սցենարը:\n");

//     QFont monoFont("Courier New", 10);
//     m_scriptEditor->setFont(monoFont);
//     m_scriptEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//     m_scriptEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//     scriptLayout->addWidget(m_scriptEditor);
//     splitter->addWidget(scriptGroup);

//     QGroupBox *logGroup = new QGroupBox("📋 Մատյան", this);
//     QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
//     m_logViewer = new QTextEdit(this);
//     m_logViewer->setReadOnly(true);
//     m_logViewer->setFont(monoFont);
//     m_logViewer->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//     m_logViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//     logLayout->addWidget(m_logViewer);
//     splitter->addWidget(logGroup);

//     splitter->setStretchFactor(0, 2);
//     splitter->setStretchFactor(1, 2);
//     splitter->setStretchFactor(2, 1);

//     mainLayout->addWidget(splitter, 1);
// }

// void MainWindow::setupVisualization()
// {
//     m_scene = new QGraphicsScene(this);
//     m_scene->setSceneRect(0, 0, 1000, 550);
//     m_graphicsView->setScene(m_scene);

//     // Երկինքը
//     QGraphicsRectItem *sky = m_scene->addRect(0, 0, 1000, 180);
//     QLinearGradient skyGradient(0, 0, 0, 180);
//     skyGradient.setColorAt(0, QColor("#4a5f7f"));
//     skyGradient.setColorAt(0.5, QColor("#3d5470"));
//     skyGradient.setColorAt(1, QColor("#2c4661"));
//     sky->setBrush(QBrush(skyGradient));
//     sky->setPen(Qt::NoPen);
//     sky->setZValue(-20);

//     // ջուրը
//     QGraphicsRectItem *waterBase = m_scene->addRect(0, 180, 1000, 370);
//     QLinearGradient waterGradient(0, 180, 0, 550);
//     waterGradient.setColorAt(0, QColor("#2563eb"));
//     waterGradient.setColorAt(0.3, QColor("#1e40af"));
//     waterGradient.setColorAt(0.7, QColor("#1e3a8a"));
//     waterGradient.setColorAt(1, QColor("#1a2f5a"));
//     waterBase->setBrush(QBrush(waterGradient));
//     waterBase->setPen(Qt::NoPen);
//     waterBase->setZValue(-10);

//     //Ջրի ալիքները
//     for (int layer = 0; layer < 20; layer++) {
//         QPainterPath wavePath;
//         double waveY = 180 + layer * 19;
//         wavePath.moveTo(0, waveY);

//         for (int x = 0; x <= 1000; x += 10) {
//             double y = waveY + qSin(x * 0.05 + layer * 0.6) * (5 - layer * 0.15);
//             wavePath.lineTo(x, y);
//         }

//         QGraphicsPathItem *wave = m_scene->addPath(wavePath);
//         int alpha = qMax(8, 55 - layer * 3);
//         wave->setPen(QPen(QColor(255, 255, 255, alpha), 1.5));
//         wave->setZValue(-9 + (layer % 10));
//     }

//     // քաղաք Ա-ի կանաչ հատվածը
//     QGraphicsRectItem *leftShore = m_scene->addRect(0, 105, 100, 445);
//     QLinearGradient leftGradient(0, 105, 100, 105);
//     leftGradient.setColorAt(0, QColor("#166534"));
//     leftGradient.setColorAt(0.5, QColor("#15803d"));
//     leftGradient.setColorAt(1, QColor("#16a34a"));
//     leftShore->setBrush(QBrush(leftGradient));
//     leftShore->setPen(Qt::NoPen);
//     leftShore->setZValue(5);

//     // Ձախ կողմում գտնվող փայտե նավամատույցը
//     QGraphicsRectItem *leftPier = m_scene->addRect(85, 180, 50, 370);
//     QLinearGradient pierGradient(85, 180, 135, 180);
//     pierGradient.setColorAt(0, QColor("#92400e"));
//     pierGradient.setColorAt(0.5, QColor("#b45309"));
//     pierGradient.setColorAt(1, QColor("#78350f"));
//     leftPier->setBrush(QBrush(pierGradient));
//     leftPier->setPen(QPen(QColor("#78350f"), 2));
//     leftPier->setZValue(6);

//     m_sideALabel = m_scene->addText("ՔԱՂԱՔ Ա", QFont("Segoe UI", 18, QFont::Bold));
//     m_sideALabel->setDefaultTextColor(QColor("#86efac"));
//     m_sideALabel->setPos(8, 140);
//     m_sideALabel->setZValue(20);
//     QGraphicsDropShadowEffect *shadowA = new QGraphicsDropShadowEffect();
//     shadowA->setBlurRadius(14);
//     shadowA->setColor(QColor(0, 0, 0, 200));
//     shadowA->setOffset(2, 2);
//     m_sideALabel->setGraphicsEffect(shadowA);

//     // Քաղաք Բ-ի կանաչ հատվածը
//     QGraphicsRectItem *rightShore = m_scene->addRect(900, 105, 100, 445);
//     QLinearGradient rightGradient(900, 105, 1000, 105);
//     rightGradient.setColorAt(0, QColor("#16a34a"));
//     rightGradient.setColorAt(0.5, QColor("#15803d"));
//     rightGradient.setColorAt(1, QColor("#166534"));
//     rightShore->setBrush(QBrush(rightGradient));
//     rightShore->setPen(Qt::NoPen);
//     rightShore->setZValue(5);

//     // փայտե նավամատույցը Բ-ի
//     QGraphicsRectItem *rightPier = m_scene->addRect(865, 180, 50, 370);
//     rightPier->setBrush(QBrush(pierGradient));
//     rightPier->setPen(QPen(QColor("#78350f"), 2));
//     rightPier->setZValue(6);

//     m_sideBLabel = m_scene->addText("ՔԱՂԱՔ Բ", QFont("Segoe UI", 18, QFont::Bold));
//     m_sideBLabel->setDefaultTextColor(QColor("#fca5a5"));
//     m_sideBLabel->setPos(870, 140);
//     m_sideBLabel->setZValue(20);
//     QGraphicsDropShadowEffect *shadowB = new QGraphicsDropShadowEffect();
//     shadowB->setBlurRadius(14);
//     shadowB->setColor(QColor(0, 0, 0, 200));
//     shadowB->setOffset(2, 2);
//     m_sideBLabel->setGraphicsEffect(shadowB);

//     QList<QObject*> boats = m_simulator->getAllBoats();

//     QList<Boat*> sideABoats;
//     QList<Boat*> sideBBoats;

//     for (QObject *obj : boats) {
//         Boat *boat = qobject_cast<Boat*>(obj);
//         if (boat) {
//             if (boat->side() == "A") {
//                 sideABoats.append(boat);
//             } else {
//                 sideBBoats.append(boat);
//             }
//         }
//     }

//     double startXA = 60;
//     double startYA = 248;
//     double spacingA = 75;

//     for (int i = 0; i < sideABoats.size(); i++) {
//         Boat *boat = sideABoats[i];
//         double xPos = startXA;
//         double yPos = startYA + (i * spacingA);

//         boat->setPositionX(xPos / 1000.0);

//         BoatGraphicsItem *item = new BoatGraphicsItem(boat);
//         m_scene->addItem(item);
//         m_boatItems[boat->id()] = item;
//         item->setPos(xPos, yPos);
//     }

//     double startXB = 910;
//     double startYB = 210;
//     double spacingB = 75;

//     for (int i = 0; i < sideBBoats.size(); i++) {
//         Boat *boat = sideBBoats[i];
//         double xPos = startXB;
//         double yPos = startYB + (i * spacingB);

//         boat->setPositionX(xPos / 1000.0);

//         BoatGraphicsItem *item = new BoatGraphicsItem(boat);
//         m_scene->addItem(item);
//         m_boatItems[boat->id()] = item;
//         item->setPos(xPos, yPos);
//     }
// }

// void MainWindow::updateVisualization()
// {
//     for (auto it = m_boatItems.begin(); it != m_boatItems.end(); ++it) {
//         it.value()->updatePosition();
//     }
// }

// void MainWindow::onStartClicked()
// {
//     if (m_simulator->isRunning()) {
//         return;
//     }

//     QString script = m_scriptEditor->toPlainText().trimmed();

//     if (script.isEmpty()) {
//         QMessageBox::warning(this, "Դատարկ սցենար",
//                              "Խնդրում ենք մուտքագրել սցենար սիմուլյացիան սկսելուց առաջ:");
//         return;
//     }

//     bool success = m_simulator->loadScript(script);

//     if (!success) {
//         QStringList errors = m_simulator->getErrors();
//         QString errorMsg = "Սցենարում սխալներ կան:\n\n" + errors.join("\n");
//         QMessageBox::critical(this, "Սխալներ", errorMsg);
//         return;
//     }

//     QStringList warnings = m_simulator->getWarnings();
//     if (!warnings.isEmpty()) {
//         QString warnMsg = "Սցենարի զգուշացումներ:\n\n" + warnings.join("\n");
//         QMessageBox::warning(this, "Զգուշացումներ", warnMsg);
//     }

//     m_simulator->start();
// }

// void MainWindow::onPauseClicked()
// {
//     m_simulator->pause();
// }


// void MainWindow::onResetClicked()
// {
//     m_simulator->reset();
//     m_logViewer->clear();
//     updateBoatsList();
//     updateVisualization();
// }


// void MainWindow::updateUI()
// {
//     m_timeLabel->setText(QString("Ժամանակ: %1 րոպե").arg(m_simulator->currentTime(), 0, 'f', 1));

//     if (m_simulator->isRunning()) {
//         m_statusLabel->setText("Կարգավիճակ: Աշխատում է։");
//         m_statusLabel->setStyleSheet("color: #a6e3a1; font-weight: bold;");
//         m_startButton->setEnabled(false);
//         m_pauseButton->setEnabled(true);
//     } else {
//         m_statusLabel->setText("Կարգավիճակ: Կանգնած։");
//         m_statusLabel->setStyleSheet("color: #f38ba8; font-weight: bold;");
//         m_startButton->setEnabled(true);
//         m_pauseButton->setEnabled(false);
//     }

//     m_completedLabel->setText(QString("Ավարտված: %1/%2")
//                                   .arg(m_simulator->completedBoats())
//                                   .arg(m_simulator->totalBoats()));

//     m_collisionsLabel->setText(QString("Բախումներ: %1").arg(m_simulator->collisions()));
//     if (m_simulator->collisions() > 0) {
//         m_collisionsLabel->setStyleSheet("color: #f38ba8; font-weight: bold;");
//     } else {
//         m_collisionsLabel->setStyleSheet("color: #a6e3a1; font-weight: bold;");
//     }

//     m_bridgeHealthBar->setValue(m_simulator->bridgeHealth());
//     if (m_simulator->bridgeHealth() < 30) {
//         m_bridgeHealthBar->setStyleSheet(
//             "QProgressBar::chunk { background-color: #f38ba8; border-radius: 4px; }"
//             );
//     } else if (m_simulator->bridgeHealth() < 60) {
//         m_bridgeHealthBar->setStyleSheet(
//             "QProgressBar::chunk { background-color: #fab387; border-radius: 4px; }"
//             );
//     } else {
//         m_bridgeHealthBar->setStyleSheet(
//             "QProgressBar::chunk { background-color: #a6e3a1; border-radius: 4px; }"
//             );
//     }

//     updateBoatsList();
// }

// void MainWindow::updateBoatsList()
// {
//     m_boatsList->clear();

//     QList<QObject*> boats = m_simulator->getAllBoats();

//     QList<Boat*> sideA, sideB;
//     for (QObject *obj : boats) {
//         Boat *boat = qobject_cast<Boat*>(obj);
//         if (boat) {
//             if (boat->side() == "A") sideA.append(boat);
//             else sideB.append(boat);
//         }
//     }

//     QListWidgetItem *headerA = new QListWidgetItem("═══ ՔԱՂԱՔ Ա ═══");
//     headerA->setForeground(QColor("#a6e3a1"));
//     QFont headerFont;
//     headerFont.setBold(true);
//     headerFont.setPointSize(10);
//     headerA->setFont(headerFont);
//     m_boatsList->addItem(headerA);

//     // Ա քաղաքի նավակները
//     for (Boat *boat : sideA) {
//         QString status = boat->isMoving() ? "Շարժվում է" : "Նավահանգիստ";
//         QString cargo = boat->cargoWeight() > 0 ?
//                             QString("Բեռ: %1 (%2 կգ)").arg(boat->cargoType()).arg(boat->cargoWeight()) : "Բեռ: Դատարկ";

//         QString allowedCargo = "✓ Թույլատրված: ";
//         QStringList allowed = boat->allowedCargoTypes();
//         if (allowed.isEmpty()) {
//             allowedCargo += "Բոլոր տեսակները";
//         } else {
//             allowedCargo += allowed.join(", ");
//         }

//         QString info = QString("%1 [%2]\n   %3\n   Վառելիք: %4%% | Արագություն: %5 րոպե\n   Կարողություն: %6 կգ\n   %7\n   %8\n   %9")
//                            .arg(boat->name())
//                            .arg(boat->id())
//                            .arg(status)
//                            .arg(boat->fuel())
//                            .arg(QString("%1-%2").arg(boat->minSpeed()).arg(boat->maxSpeed()))
//                            .arg(boat->capacity())
//                            .arg(cargo)
//                            .arg(allowedCargo)
//                            .arg(boat->description());

//         QListWidgetItem *item = new QListWidgetItem(info);
//         item->setForeground(boat->color());
//         QFont itemFont = item->font();
//         itemFont.setBold(false);
//         itemFont.setPointSize(9);
//         item->setFont(itemFont);
//         m_boatsList->addItem(item);

//         QListWidgetItem *separator = new QListWidgetItem("───────────────");
//         separator->setForeground(QColor("#45475a"));
//         m_boatsList->addItem(separator);
//     }

//     // Բ քաղաք
//     QListWidgetItem *headerB = new QListWidgetItem("\n═══ ՔԱՂԱՔ Բ ═══");
//     headerB->setForeground(QColor("#89b4fa"));
//     headerB->setFont(headerFont);
//     m_boatsList->addItem(headerB);

//     //Բ քաղաքի նավեր, լրիվ նույն լոգիկան ա
//     for (Boat *boat : sideB) {
//         QString status = boat->isMoving() ? "Շարժվում է" : "Նավահանգիստ";
//         QString cargo = boat->cargoWeight() > 0 ?
//                             QString("Բեռ: %1 (%2 կգ)").arg(boat->cargoType()).arg(boat->cargoWeight()) : "Բեռ: Դատարկ";

//         // Թույլատրված բեռների ցուցադրում
//         QString allowedCargo = "✓ Թույլատրված: ";
//         QStringList allowed = boat->allowedCargoTypes();
//         if (allowed.isEmpty()) {
//             allowedCargo += "Բոլոր տեսակները";
//         } else {
//             allowedCargo += allowed.join(", ");
//         }

//         QString info = QString("%1 [%2]\n   %3\n   Վառելիք: %4% | Արագություն: %5 րոպե\n  Կարողություն: %6 կգ\n   %7\n   %8\n   %9")
//                            .arg(boat->name())
//                            .arg(boat->id())
//                            .arg(status)
//                            .arg(boat->fuel())
//                            .arg(QString("%1-%2").arg(boat->minSpeed()).arg(boat->maxSpeed()))
//                            .arg(boat->capacity())
//                            .arg(cargo)
//                            .arg(allowedCargo)
//                            .arg(boat->description());

//         QListWidgetItem *item = new QListWidgetItem(info);
//         item->setForeground(boat->color());
//         QFont itemFont = item->font();
//         itemFont.setBold(false);
//         itemFont.setPointSize(9);
//         item->setFont(itemFont);
//         m_boatsList->addItem(item);

//         QListWidgetItem *separator = new QListWidgetItem("───────────────");
//         separator->setForeground(QColor("#45475a"));
//         m_boatsList->addItem(separator);
//     }
// }


// void MainWindow::onLogMessage(const QString &message, const QString &type)
// {
//     QString color = "#cdd6f4";
//     if (type == "error") color = "#f38ba8";
//     else if (type == "warning") color = "#fab387";
//     else if (type == "success") color = "#a6e3a1";
//     else if (type == "info") color = "#89b4fa";

//     QString html = QString("<span style='color: %1;'>[%2] %3</span>")
//                        .arg(color)
//                        .arg(m_simulator->currentTime(), 0, 'f', 1)
//                        .arg(message);

//     m_logViewer->append(html);
// }

// void MainWindow::onSimulationFinished()
// {
//     QString rating;
//     int collisions = m_simulator->collisions();
//     double time = m_simulator->currentTime();
//     int health = m_simulator->bridgeHealth();
//     int completedBoats = m_simulator->completedBoats();
//     int totalBoats = m_simulator->totalBoats();
//     int score = m_simulator->totalScore();
//     int stars = m_simulator->stars();

//     switch (stars) {
//     case 5:
//         rating = "⭐⭐⭐⭐⭐ ԿԱՏԱՐՅԱԼ!";
//         break;
//     case 4:
//         rating = "⭐⭐⭐⭐ ԳԵՐԱԶԱՆՑ!";
//         break;
//     case 3:
//         rating = "⭐⭐⭐ ԼԱՎ";
//         break;
//     case 2:
//         rating = "⭐⭐ ԲԱՎԱՐԱՐ";
//         break;
//     case 1:
//         rating = "⭐ ԹՈՒՅԼ";
//         break;
//     default:
//         rating = "ՁԱԽՈՂՎԱԾ\n ԲՈԼՈՐ 7 ՆԱՎԱԿՆԵՐԸ ՊԵՏՔ Է ՀԱՍՑՎԵՆ!";
//         break;
//     }

//     QString msg = QString(
//                       "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
//                       "    ՍԻՄՈՒԼՅԱՑԻԱՆ ԱՎԱՐՏՎԵՑ\n"
//                       "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
//                       " ԱՐԴՅՈՒՆՔՆԵՐ:\n"
//                       "─────────────────────────────\n"
//                       "⏱️  Ժամանակ:        %1 րոպե\n"
//                       "🚢 Նավակներ:       %2/%3\n"
//                       "💥 Բախումներ:       %4\n"
//                       "🌉 Կամուրջ:         %5%%\n"
//                       "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
//                       "🏆 ԳՆԱՀԱՏԱԿԱՆ:\n"
//                       "%6\n"
//                       "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
//                       ).arg(time, 0, 'f', 1)
//                       .arg(completedBoats)
//                       .arg(totalBoats)
//                       .arg(collisions)
//                       .arg(health)
//                       .arg(rating);

//     // Պահպանել արդյունքը shared DB-ում
//     if (stars >= 1) ResultSaver::save("ship", stars);

//     QMessageBox msgBox(this);
//     msgBox.setWindowTitle("Ավարտված");
//     msgBox.setText(msg);
//     msgBox.setIcon(QMessageBox::Information);
//     msgBox.setStyleSheet(
//         "QMessageBox { background-color: #1e1e2e; min-width: 450px; }"
//         "QLabel { color: #cdd6f4; font-size: 12px; font-family: 'Courier New'; }"
//         "QPushButton { "
//         "   background-color: #89b4fa; "
//         "   color: #1e1e2e; "
//         "   border-radius: 6px; "
//         "   padding: 8px 16px; "
//         "   font-weight: bold; "
//         "}"
//         );
//     msgBox.exec();
// }

// void MainWindow::onShowHelp()
// {
//     QString helpText =
//         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
//         "        📚 ՀՐԱՄԱՆՆԵՐԻ ՈՒՂԵՑՈՒՅՑ \n"
//         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
//         " SEND(նավակ, սկիզբ, վերջ, ժամանակ)\n"
//         "  -> Նավակը ուղարկում է կամրջով\n"
//         "   • նավակ: Նավակի ID (B1-B7)\n"
//         "   • սկիզբ: Սկզբնական կողմ (A կամ B)\n"
//         "   • վերջ: Նպատակային կողմ (A կամ B)\n"
//         "   • ժամանակ: Երբ սկսել շարժումը (րոպե)\n"
//         "   Օր․՝ SEND(B1, A->B, 0)\n"

//         " LOAD(նավակ, բեռի_տեսակ, քաշ, ժամանակ)\n"
//         "  -> Բեռը բեռնում է նավակի վրա\n"
//         "   • նավակ: Նավակի ID (B1-B7)\n"
//         "   • բեռի_տեսակ:(տես ցանկը ներքևում)\n"
//         "   • քաշ: Քաշը կիլոգրամներով\n"
//         "   • ժամանակ: Երբ բեռնել (րոպե) \n"
//         "   Օր․՝ LOAD(B1, FOOD_DRY, 100, 0)\n"

//         " UNLOAD(նավակ, ժամանակ)\n"
//         "  -> Բեռը իջեցնում է նավակից\n"
//         "   • նավակ: Նավակի ID (B1-B7)\n"
//         "   • ժամանակ: Երբ բեռնաթափել (րոպե)\n"
//         "   Օր․՝ UNLOAD(B1, 12)\n"

//         " REFUEL(նավակ, քանակ, ժամանակ)\n"
//         "  -> Վառելիքի լիցքավորում (0-100%%)\n"
//         "   • նավակ: Նավակի ID (B1-B7)\n"
//         "   • քանակ: Վառելիքի քանակ (0-100)\n"
//         "   • ժամանակ: Երբ լիցքավորել (րոպե) \n"
//         "   Օր․՝ REFUEL(B1, 50, 10)\n"

//         " CHECK_BRIDGE()\n"
//         "  -> Ստուգում է կամրջի ընթացիկ վիճակը\n"
//         "   • Ցուցադրում է ամրությունը (%%)\n"
//         "   • Ցուցադրում է կամուրջն ազատ է թե զբաղված\n"
//         "   Օր․՝ CHECK_BRIDGE()\n\n"

//         "  REPAIR_BRIDGE(տևողություն)\n"
//         "  -> Կամուրջը վերանորոգում է\n"
//         "   • տևողություն: Վերանորոգման տևողությունը (րոպե)\n"
//         "   • Կամուրջն անգործ է վերանորոգման ընթացքում\n"
//         "   Օր․՝ REPAIR_BRIDGE(5)\n\n"


//         "  ՕՐԻՆԱԿ ՍՑԵՆԱՐ\n"

//         "// B1 նավակ: բեռնվում, շարժվում, բեռնաթափվում, լիցքավորում\n"
//         "LOAD(B1, FOOD_DRY, 100, 0)      // Բեռնվում է սկզբնական պահին\n"
//         "SEND(B1, A->B, 1)               // Սկսում է 1 րոպեին \n"
//         "UNLOAD(B1, 13)                  // Բեռնաթափվում է 13 րոպեին \n"
//         "REFUEL(B1, 30, 17)              // Լիցքավորվում է 17 րոպեին\n";


//     // Scroll անելու հնարավորություն ենք տալիս
//     QDialog *dialog = new QDialog(this);
//     dialog->setWindowTitle("Հրամանների Ուղեցույց");
//     dialog->setMinimumSize(600, 500);
//     dialog->setMaximumSize(650, 700);
//     dialog->resize(620, 600);

//     QVBoxLayout *layout = new QVBoxLayout(dialog);
//     layout->setContentsMargins(0, 0, 0, 10);
//     layout->setSpacing(0);

//     QScrollArea *scrollArea = new QScrollArea(dialog);
//     scrollArea->setWidgetResizable(true);
//     scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//     scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//     scrollArea->setFrameShape(QFrame::NoFrame);

//     QLabel *label = new QLabel(helpText, dialog);
//     label->setWordWrap(true);
//     label->setTextInteractionFlags(Qt::TextSelectableByMouse);
//     label->setFont(QFont("Courier New", 10));
//     label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//     label->setStyleSheet("QLabel { color: #11111b; padding: 20px; background-color: transparent; }");
//     label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//     scrollArea->setWidget(label);
//     layout->addWidget(scrollArea);

//     QPushButton *closeButton = new QPushButton("✖ Փակել", dialog);
//     closeButton->setMinimumHeight(40);
//     closeButton->setMaximumHeight(40);
//     connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
//     layout->addWidget(closeButton);

//     dialog->setStyleSheet(
//         "QDialog { "
//         "   background-color: #89dceb; "
//         "}"
//         "QPushButton { "
//         "   background-color: #1e1e2e; "
//         "   color: #89dceb; "
//         "   border: 2px solid #1e1e2e; "
//         "   border-radius: 8px; "
//         "   padding: 8px 16px; "
//         "   font-weight: bold; "
//         "   font-size: 12px; "
//         "}"
//         "QPushButton:hover { "
//         "   background-color: #313244; "
//         "   border-color: #313244; "
//         "}"
//         "QScrollArea { "
//         "   border: none; "
//         "   background-color: #89dceb; "
//         "}"
//         "QScrollBar:vertical { "
//         "   background-color: #74c7ec; "
//         "   width: 12px; "
//         "   border-radius: 6px; "
//         "}"
//         "QScrollBar::handle:vertical { "
//         "   background-color: #1e1e2e; "
//         "   border-radius: 6px; "
//         "   min-height: 20px; "
//         "}"
//         "QScrollBar::handle:vertical:hover { "
//         "   background-color: #313244; "
//         "}"
//         );

//     dialog->exec();
//     delete dialog;
// }
// void MainWindow::onShowProblem()
// {
//     QString problemText =

//         "📋 ԻՐԱՎԻՃԱԿ\n"
//         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
//         "Ա և Բ քաղաքները գտնվում են Երկրի հյուսիսային հատվածում \n"
//         "և իրարից բաժանված են Հյուսիսային նեղուցով։Քաղաքների միջև\n"
//         "անցկացված է կամուրջ՝ հատուկ նավարկության աշխատանքները   \n"
//         "հեշտացնելու համար։Օդերևութաբանները նախազգուշացրել են,որ \n"
//         "նեղուցի այն հատվածը, որն ընկած է քաղաքների միջև,շուտով  \n"
//         "սառելու է և նավային հաղորդակցությունը հնարավոր կլինի    \n"
//         "վերսկսել միայն գարնանը։   \n"

//         " ՆՊԱՏԱԿ: ՏԵՂԱՓՈԽԵԼ ԸՆԴԱՄԵՆԸ 1300 ԿԳ բեռ։\n"
//         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"

//         "🔴 Բ → Ա (ԱՐՏԱԿԱՐԳ): 650 ԿԳ\n"
//         "─────────────────────────────────────\n"
//         "  🏥 MED_ANTIBIOTICS    120 կգ\n"
//         "  🏥 MED_FIRSTAID        80 կգ\n"
//         "  💊 MED_PAINKILLERS    100 կգ\n"
//         "  🍎 FOOD_FRESH         150 կգ\n"
//         "  💧 WATER              200 կգ\n\n"

//         "🔵 Ա → Բ (ՎԵՐԱԿԱՆԳՆՈՒՄ): 650 ԿԳ\n"
//         "─────────────────────────────────────\n"
//         "  🏗️ BUILDING_MATERIALS 400 կգ\n"
//         "  ⚙️ EQUIPMENT          100 կգ\n"
//         "  🩹 MED_BANDAGES       100 կգ\n"
//         "  🌾 FOOD_DRY            50 կգ\n\n"


//         " ԿԱՆՈՆՆԵՐ\n"
//         "✓ Միաժամանակ 1 նավակ կամրջով\n"
//         "✓ Ծանր բեռ → դանդաղ շարժում(դրանով պայմանավորված սահմանված է ժամանակային միջակայք)\n"
//         "✓ Քիչ վառելիք (<30%) → +30% ժամանակ\n"
//         "✓ Բախումներ → կամուրջը վնասվում է(-10% ամրություն)\n"
//         "✓ Ժամանակները պետք է ճիշտ հաշվարկել\n\n"

//         "⭐ ԳՆԱՀԱՏԱԿԱՆ\n"
//         "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
//         "ԵԹԵ ԲՈԼՈՐ 7 ՆԱՎԱԿՆԵՐԸ ՉԵՆ ՀԱՍՑՎԵԼ:\n"
//         "   → ԱՎՏՈՄԱՏ 0 ԱՍՏՂ (անկախ այլ ցուցանիշներից)\n\n"

//         "⭐⭐⭐⭐⭐ ԿԱՏԱՐՅԱԼ:\n"
//         "   • 7/7 նավակ հասցված\n"
//         "   • 1300 կգ բեռ\n"
//         "   • Կամուրջ 100%\n"
//         "   • 0 բախում\n\n"

//         "⭐⭐⭐⭐ ԳԵՐԱԶԱՆՑ:\n"
//         "   • 7/7 նավակ հասցված\n"
//         "   • 1300 կգ բեռ\n"
//         "   • Կամուրջ 80%+\n"
//         "   • Ամենաշատը 1 բախում\n\n"

//         "⭐⭐⭐ ԼԱՎ:\n"
//         "   • 7/7 նավակ հասցված\n"
//         "   • 1170+ կգ (90%)\n"
//         "   • Կամուրջ 60%+\n\n"

//         "⭐⭐ ԲԱՎԱՐԱՐ:\n"
//         "   • 7/7 նավակ հասցված\n"
//         "   • 910+ կգ (70%)\n\n"

//         "⭐ ԹՈՒՅԼ:\n"
//         "   • 7/7 նավակ հասցված\n"
//         "   • Ցանկացած քանակ\n\n";



//     QDialog *dialog = new QDialog(this);
//     dialog->setWindowTitle(" Միսիայի Նկարագրություն");
//     dialog->setMinimumSize(600, 500);
//     dialog->setMaximumSize(650, 700);
//     dialog->resize(620, 600);

//     QVBoxLayout *layout = new QVBoxLayout(dialog);
//     layout->setContentsMargins(0, 0, 0, 10);
//     layout->setSpacing(0);

//     QScrollArea *scrollArea = new QScrollArea(dialog);
//     scrollArea->setWidgetResizable(true);
//     scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//     scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//     scrollArea->setFrameShape(QFrame::NoFrame);

//     QLabel *label = new QLabel(problemText, dialog);
//     label->setWordWrap(true);
//     label->setTextInteractionFlags(Qt::TextSelectableByMouse);
//     label->setFont(QFont("Courier New", 9));
//     label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//     label->setStyleSheet("QLabel { color: #11111b; padding: 15px; background-color: transparent; }");
//     label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//     scrollArea->setWidget(label);
//     layout->addWidget(scrollArea);

//     QPushButton *closeButton = new QPushButton("✖ Փակել", dialog);
//     closeButton->setMinimumHeight(35);
//     closeButton->setMaximumHeight(35);
//     connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
//     layout->addWidget(closeButton);

//     dialog->setStyleSheet(
//         "QDialog { "
//         "   background-color: #89dceb; "
//         "}"
//         "QPushButton { "
//         "   background-color: #1e1e2e; "
//         "   color: #89dceb; "
//         "   border: 2px solid #1e1e2e; "
//         "   border-radius: 8px; "
//         "   padding: 6px 12px; "
//         "   font-weight: bold; "
//         "   font-size: 11px; "
//         "}"
//         "QPushButton:hover { "
//         "   background-color: #313244; "
//         "   border-color: #313244; "
//         "}"
//         "QScrollArea { "
//         "   border: none; "
//         "   background-color: #89dceb; "
//         "}"
//         "QScrollBar:vertical { "
//         "   background-color: #74c7ec; "
//         "   width: 12px; "
//         "   border-radius: 6px; "
//         "}"
//         "QScrollBar::handle:vertical { "
//         "   background-color: #1e1e2e; "
//         "   border-radius: 6px; "
//         "   min-height: 20px; "
//         "}"
//         "QScrollBar::handle:vertical:hover { "
//         "   background-color: #313244; "
//         "}"
//         );

//     dialog->exec();
//     delete dialog;
// }



#include "mainwindow.h"
#include "resultsaver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QtMath>
#include <QPainterPath>
#include <QPainter>
#include <QMap>
#include <QScrollArea>
#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QApplication>

BoatGraphicsItem::BoatGraphicsItem(Boat *boat, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_boat(boat)
{
    setZValue(10);
    m_label = new QGraphicsTextItem(boat->name(), this);
    m_label->setDefaultTextColor(Qt::white);
    QFont font = m_label->font();
    font.setBold(true);
    font.setPointSize(7);
    m_label->setFont(font);

    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect();
    textShadow->setBlurRadius(6);
    textShadow->setColor(QColor(0, 0, 0, 200));
    textShadow->setOffset(1, 1);
    m_label->setGraphicsEffect(textShadow);

    updatePosition();
}

void BoatGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::Antialiasing);

    qreal boatLength = 70;
    qreal boatWidth = 30;

    bool movingRight = true;
    if (m_boat->isMoving()) {
        if (!m_boat->targetSide().isEmpty()) {
            movingRight = (m_boat->targetSide() == "B");
        } else {
            movingRight = (m_boat->side() == "A");
        }
    } else {
        movingRight = (m_boat->side() == "A");
    }

    painter->save();

    if (!movingRight) {
        painter->scale(-1, 1);
    }

    // Ստվեր
    painter->setBrush(QBrush(QColor(0, 0, 0, 80)));
    painter->setPen(Qt::NoPen);
    QRectF shadowRect(-boatLength/2 + 5, boatWidth/2 + 3, boatLength - 10, 8);
    painter->drawEllipse(shadowRect);

    // Նավակի արտաքին տեսքը
    QPainterPath hull;
    hull.moveTo(-boatLength/2 + 5, 0);
    hull.quadTo(-boatLength/2, -boatWidth/3, -boatLength/2 + 5, -boatWidth/2);
    hull.lineTo(boatLength/2 - 8, -boatWidth/2);
    hull.quadTo(boatLength/2 - 3, -boatWidth/3, boatLength/2, 0);
    hull.quadTo(boatLength/2 - 3, boatWidth/3, boatLength/2 - 8, boatWidth/2);
    hull.lineTo(-boatLength/2 + 5, boatWidth/2);
    hull.quadTo(-boatLength/2, boatWidth/3, -boatLength/2 + 5, 0);

    // Նավակի հիմնական գույնը
    QLinearGradient hullGradient(-boatLength/2, -boatWidth/2, -boatLength/2, boatWidth/2);
    hullGradient.setColorAt(0, m_boat->color().lighter(140));
    hullGradient.setColorAt(0.3, m_boat->color().lighter(120));
    hullGradient.setColorAt(0.7, m_boat->color());
    hullGradient.setColorAt(1, m_boat->color().darker(120));

    painter->setBrush(QBrush(hullGradient));
    painter->setPen(QPen(m_boat->color().darker(160), 2.5));
    painter->drawPath(hull);

    // Սպիտակ գծիկները
    painter->setPen(QPen(QColor(255, 255, 255, 200), 2));
    painter->drawLine(QPointF(-boatLength/2 + 10, -boatWidth/2 + 3),
                      QPointF(boatLength/2 - 10, -boatWidth/2 + 3));

    //պատուհանների տիրույթը
    QRectF cabin(-15, -boatWidth/2.5, 30, boatWidth/1.5);
    QLinearGradient cabinGradient(-15, -boatWidth/2.5, -15, boatWidth/3);
    cabinGradient.setColorAt(0, m_boat->color().lighter(110));
    cabinGradient.setColorAt(1, m_boat->color().darker(130));
    painter->setBrush(QBrush(cabinGradient));
    painter->setPen(QPen(m_boat->color().darker(180), 2));
    painter->drawRoundedRect(cabin, 4, 4);

    // պատուհանները
    painter->setBrush(QBrush(QColor(150, 200, 255, 220)));
    painter->setPen(QPen(QColor(100, 150, 200), 1));
    painter->drawRoundedRect(QRectF(-10, -5, 7, 7), 2, 2);
    painter->drawRoundedRect(QRectF(3, -5, 7, 7), 2, 2);

    // ծխնելույզը
    QRectF chimney(boatLength/2 - 18, -boatWidth/2 - 8, 4, 10);
    painter->setBrush(QBrush(m_boat->color().darker(140)));
    painter->setPen(QPen(m_boat->color().darker(200), 1));
    painter->drawRect(chimney);

    // ծուխը
    if (m_boat->isMoving()) {
        painter->setBrush(QBrush(QColor(200, 200, 200, 150)));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(boatLength/2 - 16, -boatWidth/2 - 12), 3, 3);
        painter->drawEllipse(QPointF(boatLength/2 - 14, -boatWidth/2 - 16), 4, 4);
    }

    // Բեռնված լինելու դեպքում բեռը պետք է արկղերով երևա
    if (m_boat->cargoWeight() > 0) {
        int numCrates = qMin(3, m_boat->cargoWeight() / 50); // 1-3 տուփ, կախված բեռների քաշից

        for (int i = 0; i < numCrates; i++) {
            double crateX = -28 - (i * 12);
            double crateY = -6;

            // տուփը
            painter->setBrush(QBrush(QColor(160, 120, 60)));
            painter->setPen(QPen(QColor(120, 90, 40), 1.5));
            painter->drawRect(QRectF(crateX, crateY, 10, 10));

            // փայտե տախտակները
            painter->setPen(QPen(QColor(80, 60, 30), 1));
            painter->drawLine(QPointF(crateX, crateY + 5), QPointF(crateX + 10, crateY + 5));
            painter->drawLine(QPointF(crateX + 5, crateY), QPointF(crateX + 5, crateY + 10));
        }

        //բեռի տեսակը՝ փոքր կլոր գույնով
        QColor cargoColor;
        QString cargoType = m_boat->cargoType();
        if (cargoType.contains("FOOD")) cargoColor = QColor(255, 200, 100);
        else if (cargoType.contains("MED")) cargoColor = QColor(255, 100, 100);
        else if (cargoType.contains("WATER")) cargoColor = QColor(100, 150, 255);
        else if (cargoType.contains("BUILDING")) cargoColor = QColor(150, 150, 150);
        else cargoColor = QColor(200, 200, 0);

        painter->setBrush(QBrush(cargoColor));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(-23, 0), 4, 4);
    }

    // նավի հետևի գծիկները շարժվելու ժամանակ
    if (m_boat->isMoving()) {
        painter->setPen(QPen(QColor(255, 255, 255, 120), 2));
        painter->setBrush(Qt::NoBrush);

        painter->drawArc(QRectF(-boatLength/2 - 10, -10, 20, 20), 0, 180 * 16);
        painter->drawArc(QRectF(-boatLength/2 - 15, -8, 16, 16), 0, 180 * 16);
    }

    painter->restore();

    if (m_boat->isMoving()) { // եթե ծարժվում է, անունը գլխին ա գրված լինում
        m_label->setPos(-m_label->boundingRect().width() / 2, -35);
    } else {                  //նորմալ ժամանակ կողքին ա գրված լինում
        m_label->setPos(40, -8);
    }
}

QRectF BoatGraphicsItem::boundingRect() const
{
    return QRectF(-50, -40, 150, 80);
}

void BoatGraphicsItem::updatePosition()
{
    // Նավակի ինդեքսը օգտագործում ենք իր դիրքը ուղղղահայաց ուղղությամբ ունենալու համար
    static QMap<QString, int> boatIndex;

    QString boatId = m_boat->id();

    // Սահմանում ենք նավակի ինդեքսները սկզբնական վիճակի համար
    if (!boatIndex.contains(boatId)) {
        int idx = 0;
        if      (boatId == "B1") idx = 0;
        else if (boatId == "B2") idx = 1;
        else if (boatId == "B3") idx = 2;
        else if (boatId == "B4") idx = 0;
        else if (boatId == "B5") idx = 1;
        else if (boatId == "B6") idx = 2;
        else if (boatId == "B7") idx = 3;
        boatIndex[boatId] = idx;
    }

    int idx = boatIndex[boatId];

    if (!m_boat->isMoving()) {
        double x, y;
        QString currentSide = m_boat->side();
        double verticalSpacing = 75;

        QString boatId = m_boat->id();
        bool isFromSideA = (boatId == "B1" || boatId == "B2" || boatId == "B3");

        if (currentSide == "A") {
            x = 60;
        } else {
            x = 910;
        }

        if (isFromSideA)
        {
            y = 248 + idx * verticalSpacing;
        } else {
            y = 210 + idx * verticalSpacing;
        }

        setPos(x, y);
        setRotation(0);
        m_label->setPos(45, -8);
    }
    else {
        double progress = m_boat->progress();
        double posX = m_boat->positionX();

        double leftX = 60;
        double rightX = 910;
        double x = leftX + posX * (rightX - leftX);

        QString boatId = m_boat->id();
        double baseY = 210;

        if (boatId == "B1" || boatId == "B2" || boatId == "B3") {
            baseY = 248;
        }

        double laneY = baseY + idx * 75;

        // ալիքաձև անցում
        double y = laneY + qSin(progress * M_PI * 4) * 5;

        setPos(x, y);
        setRotation(qSin(progress * M_PI * 8) * 3);
        m_label->setPos(-m_label->boundingRect().width() / 2, -35);
    }

    update();
}

MainWindow::MainWindow(Simulator *simulator, QWidget *parent)
    : QMainWindow(parent)
    , m_simulator(simulator)
{
    setWindowTitle("🌉 Հումանիտար Կամրջային Լոգիստիկայի Սիմուլյատոր");
    setMinimumSize(1100, 700);
    resize(1200, 800);

    setStyleSheet(
        "QMainWindow { background-color: #0f1419; }"
        "QGroupBox { "
        "   color: #cdd6f4; "
        "   border: 2px solid #89b4fa; "
        "   border-radius: 8px; "
        "   margin-top: 10px; "
        "   font-weight: bold; "
        "   padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px; "
        "}"
        "QPushButton { "
        "   background-color: #89b4fa; "
        "   color: #1e1e2e; "
        "   border: none; "
        "   border-radius: 6px; "
        "   padding: 8px 16px; "
        "   font-weight: bold; "
        "   font-size: 13px; "
        "}"
        "QPushButton:hover { background-color: #b4befe; }"
        "QPushButton:pressed { background-color: #74c7ec; }"
        "QPushButton:disabled { background-color: #45475a; color: #6c7086; }"
        "QTextEdit, QListWidget { "
        "   background-color: #181825; "
        "   color: #cdd6f4; "
        "   border: 2px solid #313244; "
        "   border-radius: 6px; "
        "   padding: 8px; "
        "}"
        "QLabel { color: #cdd6f4; font-size: 13px; }"
        "QSpinBox { "
        "   background-color: #313244; "
        "   color: #cdd6f4; "
        "   border: 2px solid #45475a; "
        "   border-radius: 4px; "
        "   padding: 4px; "
        "}"
        "QProgressBar { "
        "   border: 2px solid #45475a; "
        "   border-radius: 6px; "
        "   text-align: center; "
        "   background-color: #313244; "
        "   color: #cdd6f4; "
        "}"
        "QProgressBar::chunk { "
        "   background-color: #a6e3a1; "
        "   border-radius: 4px; "
        "}"
        );

    setupUI();
    setupVisualization();

    connect(m_simulator, &Simulator::currentTimeChanged, this, &MainWindow::updateUI);
    connect(m_simulator, &Simulator::logMessage, this, &MainWindow::onLogMessage);
    connect(m_simulator, &Simulator::simulationFinished, this, &MainWindow::onSimulationFinished);
    connect(m_simulator, &Simulator::isRunningChanged, this, &MainWindow::updateUI);
    connect(m_simulator, &Simulator::completedBoatsChanged, this, &MainWindow::updateUI);
    connect(m_simulator, &Simulator::collisionsChanged, this, &MainWindow::updateUI);
    connect(m_simulator, &Simulator::bridgeHealthChanged, this, &MainWindow::updateUI);

    m_visualTimer = new QTimer(this);
    connect(m_visualTimer, &QTimer::timeout, this, &MainWindow::updateVisualization);
    m_visualTimer->start(30);

    updateBoatsList();
    updateUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(10);

    m_startButton = new QPushButton(" Մեկնարկ", this);
    m_pauseButton = new QPushButton(" Դադար", this);
    m_resetButton = new QPushButton(" Վերսկսել", this);
    m_helpButton = new QPushButton(" Օգնություն", this);
    m_problemButton = new QPushButton(" Խնդրի Նկարագրություն", this);

    QStyle *appStyle = QApplication::style();
    m_startButton->setIcon(appStyle->standardIcon(QStyle::SP_MediaPlay));
    m_pauseButton->setIcon(appStyle->standardIcon(QStyle::SP_MediaPause));
    m_resetButton->setIcon(appStyle->standardIcon(QStyle::SP_BrowserReload));
    m_helpButton->setIcon(appStyle->standardIcon(QStyle::SP_DialogHelpButton));
    m_problemButton->setIcon(appStyle->standardIcon(QStyle::SP_FileDialogDetailedView));

    for (QPushButton *btn : {m_startButton, m_pauseButton,
                             m_resetButton, m_helpButton, m_problemButton}) {
        btn->setIconSize(QSize(18, 18));
    }


    m_startButton->setMinimumHeight(35);
    m_pauseButton->setMinimumHeight(35);
    m_resetButton->setMinimumHeight(35);
    m_helpButton->setMinimumHeight(35);
    m_problemButton->setMinimumHeight(35);

    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(m_helpButton, &QPushButton::clicked, this, &MainWindow::onShowHelp);
    connect(m_problemButton, &QPushButton::clicked, this, &MainWindow::onShowProblem);

    controlLayout->addWidget(m_startButton);
    controlLayout->addWidget(m_pauseButton);
    controlLayout->addWidget(m_resetButton);
    controlLayout->addWidget(m_helpButton);
    controlLayout->addWidget(m_problemButton);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->setSpacing(20);

    m_timeLabel = new QLabel("Ժամանակ: 0.0 րոպե", this);
    m_statusLabel = new QLabel("Կարգավիճակ: Պատրաստ", this);
    m_completedLabel = new QLabel("Ավարտված: 0/0", this);
    m_collisionsLabel = new QLabel("Բախումներ: 0", this);

    QFont boldFont;
    boldFont.setBold(true);
    boldFont.setPointSize(11);
    m_timeLabel->setFont(boldFont);
    m_statusLabel->setFont(boldFont);
    m_completedLabel->setFont(boldFont);
    m_collisionsLabel->setFont(boldFont);

    statusLayout->addWidget(m_timeLabel);
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addWidget(m_completedLabel);
    statusLayout->addWidget(m_collisionsLabel);
    statusLayout->addStretch();

    QLabel *bridgeLabel = new QLabel("Կամուրջ:", this);
    bridgeLabel->setStyleSheet("font-weight: bold;");
    m_bridgeHealthBar = new QProgressBar(this);
    m_bridgeHealthBar->setRange(0, 100);
    m_bridgeHealthBar->setValue(100);
    m_bridgeHealthBar->setTextVisible(true);
    m_bridgeHealthBar->setMaximumWidth(200);
    m_bridgeHealthBar->setMinimumHeight(20);

    statusLayout->addWidget(bridgeLabel);
    statusLayout->addWidget(m_bridgeHealthBar);

    mainLayout->addLayout(statusLayout);

    QGroupBox *visualGroup = new QGroupBox("Արտապատկերում", this);
    QVBoxLayout *visualLayout = new QVBoxLayout(visualGroup);

    m_graphicsView = new QGraphicsView(this);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_graphicsView->setStyleSheet(
        "QGraphicsView { "
        "   background: #1e3a5f; "
        "   border: 3px solid #89b4fa; "
        "   border-radius: 10px; "
        "}"
        );
    m_graphicsView->setMinimumHeight(300);
    m_graphicsView->setMaximumHeight(400);
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    visualLayout->addWidget(m_graphicsView);
    mainLayout->addWidget(visualGroup);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    QGroupBox *boatsGroup = new QGroupBox("Նավակներ", this);
    QVBoxLayout *boatsLayout = new QVBoxLayout(boatsGroup);
    m_boatsList = new QListWidget(this);
    m_boatsList->setWordWrap(true);
    m_boatsList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_boatsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boatsLayout->addWidget(m_boatsList);
    splitter->addWidget(boatsGroup);

    QGroupBox *scriptGroup = new QGroupBox("📝 Սցենար ", this);
    QVBoxLayout *scriptLayout = new QVBoxLayout(scriptGroup);
    m_scriptEditor = new QTextEdit(this);
    m_scriptEditor->setPlaceholderText("Այստեղ մուտքագրեք սցենարը:\n");

    QFont monoFont("Courier New", 10);
    m_scriptEditor->setFont(monoFont);
    m_scriptEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scriptEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    scriptLayout->addWidget(m_scriptEditor);
    splitter->addWidget(scriptGroup);

    QGroupBox *logGroup = new QGroupBox("📋 Մատյան", this);
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    m_logViewer = new QTextEdit(this);
    m_logViewer->setReadOnly(true);
    m_logViewer->setFont(monoFont);
    m_logViewer->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_logViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    logLayout->addWidget(m_logViewer);
    splitter->addWidget(logGroup);

    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);

    mainLayout->addWidget(splitter, 1);
}

void MainWindow::setupVisualization()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1000, 550);
    m_graphicsView->setScene(m_scene);

    // Երկինքը
    QGraphicsRectItem *sky = m_scene->addRect(0, 0, 1000, 180);
    QLinearGradient skyGradient(0, 0, 0, 180);
    skyGradient.setColorAt(0, QColor("#4a5f7f"));
    skyGradient.setColorAt(0.5, QColor("#3d5470"));
    skyGradient.setColorAt(1, QColor("#2c4661"));
    sky->setBrush(QBrush(skyGradient));
    sky->setPen(Qt::NoPen);
    sky->setZValue(-20);

    // ջուրը
    QGraphicsRectItem *waterBase = m_scene->addRect(0, 180, 1000, 370);
    QLinearGradient waterGradient(0, 180, 0, 550);
    waterGradient.setColorAt(0, QColor("#2563eb"));
    waterGradient.setColorAt(0.3, QColor("#1e40af"));
    waterGradient.setColorAt(0.7, QColor("#1e3a8a"));
    waterGradient.setColorAt(1, QColor("#1a2f5a"));
    waterBase->setBrush(QBrush(waterGradient));
    waterBase->setPen(Qt::NoPen);
    waterBase->setZValue(-10);

    //Ջրի ալիքները
    for (int layer = 0; layer < 20; layer++) {
        QPainterPath wavePath;
        double waveY = 180 + layer * 19;
        wavePath.moveTo(0, waveY);

        for (int x = 0; x <= 1000; x += 10) {
            double y = waveY + qSin(x * 0.05 + layer * 0.6) * (5 - layer * 0.15);
            wavePath.lineTo(x, y);
        }

        QGraphicsPathItem *wave = m_scene->addPath(wavePath);
        int alpha = qMax(8, 55 - layer * 3);
        wave->setPen(QPen(QColor(255, 255, 255, alpha), 1.5));
        wave->setZValue(-9 + (layer % 10));
    }

    // քաղաք Ա-ի կանաչ հատվածը
    QGraphicsRectItem *leftShore = m_scene->addRect(0, 105, 100, 445);
    QLinearGradient leftGradient(0, 105, 100, 105);
    leftGradient.setColorAt(0, QColor("#166534"));
    leftGradient.setColorAt(0.5, QColor("#15803d"));
    leftGradient.setColorAt(1, QColor("#16a34a"));
    leftShore->setBrush(QBrush(leftGradient));
    leftShore->setPen(Qt::NoPen);
    leftShore->setZValue(5);

    // Ձախ կողմում գտնվող փայտե նավամատույցը
    QGraphicsRectItem *leftPier = m_scene->addRect(85, 180, 50, 370);
    QLinearGradient pierGradient(85, 180, 135, 180);
    pierGradient.setColorAt(0, QColor("#92400e"));
    pierGradient.setColorAt(0.5, QColor("#b45309"));
    pierGradient.setColorAt(1, QColor("#78350f"));
    leftPier->setBrush(QBrush(pierGradient));
    leftPier->setPen(QPen(QColor("#78350f"), 2));
    leftPier->setZValue(6);

    m_sideALabel = m_scene->addText("ՔԱՂԱՔ Ա", QFont("Segoe UI", 18, QFont::Bold));
    m_sideALabel->setDefaultTextColor(QColor("#86efac"));
    m_sideALabel->setPos(8, 140);
    m_sideALabel->setZValue(20);
    QGraphicsDropShadowEffect *shadowA = new QGraphicsDropShadowEffect();
    shadowA->setBlurRadius(14);
    shadowA->setColor(QColor(0, 0, 0, 200));
    shadowA->setOffset(2, 2);
    m_sideALabel->setGraphicsEffect(shadowA);

    // Քաղաք Բ-ի կանաչ հատվածը
    QGraphicsRectItem *rightShore = m_scene->addRect(900, 105, 100, 445);
    QLinearGradient rightGradient(900, 105, 1000, 105);
    rightGradient.setColorAt(0, QColor("#16a34a"));
    rightGradient.setColorAt(0.5, QColor("#15803d"));
    rightGradient.setColorAt(1, QColor("#166534"));
    rightShore->setBrush(QBrush(rightGradient));
    rightShore->setPen(Qt::NoPen);
    rightShore->setZValue(5);

    // փայտե նավամատույցը Բ-ի
    QGraphicsRectItem *rightPier = m_scene->addRect(865, 180, 50, 370);
    rightPier->setBrush(QBrush(pierGradient));
    rightPier->setPen(QPen(QColor("#78350f"), 2));
    rightPier->setZValue(6);

    m_sideBLabel = m_scene->addText("ՔԱՂԱՔ Բ", QFont("Segoe UI", 18, QFont::Bold));
    m_sideBLabel->setDefaultTextColor(QColor("#fca5a5"));
    m_sideBLabel->setPos(870, 140);
    m_sideBLabel->setZValue(20);
    QGraphicsDropShadowEffect *shadowB = new QGraphicsDropShadowEffect();
    shadowB->setBlurRadius(14);
    shadowB->setColor(QColor(0, 0, 0, 200));
    shadowB->setOffset(2, 2);
    m_sideBLabel->setGraphicsEffect(shadowB);

    QList<QObject*> boats = m_simulator->getAllBoats();

    QList<Boat*> sideABoats;
    QList<Boat*> sideBBoats;

    for (QObject *obj : boats) {
        Boat *boat = qobject_cast<Boat*>(obj);
        if (boat) {
            if (boat->side() == "A") {
                sideABoats.append(boat);
            } else {
                sideBBoats.append(boat);
            }
        }
    }

    double startXA = 60;
    double startYA = 248;
    double spacingA = 75;

    for (int i = 0; i < sideABoats.size(); i++) {
        Boat *boat = sideABoats[i];
        double xPos = startXA;
        double yPos = startYA + (i * spacingA);

        boat->setPositionX(xPos / 1000.0);

        BoatGraphicsItem *item = new BoatGraphicsItem(boat);
        m_scene->addItem(item);
        m_boatItems[boat->id()] = item;
        item->setPos(xPos, yPos);
    }

    double startXB = 910;
    double startYB = 210;
    double spacingB = 75;

    for (int i = 0; i < sideBBoats.size(); i++) {
        Boat *boat = sideBBoats[i];
        double xPos = startXB;
        double yPos = startYB + (i * spacingB);

        boat->setPositionX(xPos / 1000.0);

        BoatGraphicsItem *item = new BoatGraphicsItem(boat);
        m_scene->addItem(item);
        m_boatItems[boat->id()] = item;
        item->setPos(xPos, yPos);
    }
}

void MainWindow::updateVisualization()
{
    for (auto it = m_boatItems.begin(); it != m_boatItems.end(); ++it) {
        it.value()->updatePosition();
    }
}

void MainWindow::onStartClicked()
{
    if (m_simulator->isRunning()) {
        return;
    }

    QString script = m_scriptEditor->toPlainText().trimmed();

    if (script.isEmpty()) {
        QMessageBox::warning(this, "Դատարկ սցենար",
                             "Խնդրում ենք մուտքագրել սցենար սիմուլյացիան սկսելուց առաջ:");
        return;
    }

    bool success = m_simulator->loadScript(script);

    if (!success) {
        QStringList errors = m_simulator->getErrors();
        QString errorMsg = "Սցենարում սխալներ կան:\n\n" + errors.join("\n");
        QMessageBox::critical(this, "Սխալներ", errorMsg);
        return;
    }

    QStringList warnings = m_simulator->getWarnings();
    if (!warnings.isEmpty()) {
        QString warnMsg = "Սցենարի զգուշացումներ:\n\n" + warnings.join("\n");
        QMessageBox::warning(this, "Զգուշացումներ", warnMsg);
    }

    m_simulator->start();
}

void MainWindow::onPauseClicked()
{
    m_simulator->pause();
}


void MainWindow::onResetClicked()
{
    m_simulator->reset();
    m_logViewer->clear();
    updateBoatsList();
    updateVisualization();
}


void MainWindow::updateUI()
{
    m_timeLabel->setText(QString("Ժամանակ: %1 րոպե").arg(m_simulator->currentTime(), 0, 'f', 1));

    if (m_simulator->isRunning()) {
        m_statusLabel->setText("Կարգավիճակ: Աշխատում է։");
        m_statusLabel->setStyleSheet("color: #a6e3a1; font-weight: bold;");
        m_startButton->setEnabled(false);
        m_pauseButton->setEnabled(true);
    } else {
        m_statusLabel->setText("Կարգավիճակ: Կանգնած։");
        m_statusLabel->setStyleSheet("color: #f38ba8; font-weight: bold;");
        m_startButton->setEnabled(true);
        m_pauseButton->setEnabled(false);
    }

    m_completedLabel->setText(QString("Ավարտված: %1/%2")
                                  .arg(m_simulator->completedBoats())
                                  .arg(m_simulator->totalBoats()));

    m_collisionsLabel->setText(QString("Բախումներ: %1").arg(m_simulator->collisions()));
    if (m_simulator->collisions() > 0) {
        m_collisionsLabel->setStyleSheet("color: #f38ba8; font-weight: bold;");
    } else {
        m_collisionsLabel->setStyleSheet("color: #a6e3a1; font-weight: bold;");
    }

    m_bridgeHealthBar->setValue(m_simulator->bridgeHealth());
    if (m_simulator->bridgeHealth() < 30) {
        m_bridgeHealthBar->setStyleSheet(
            "QProgressBar::chunk { background-color: #f38ba8; border-radius: 4px; }"
            );
    } else if (m_simulator->bridgeHealth() < 60) {
        m_bridgeHealthBar->setStyleSheet(
            "QProgressBar::chunk { background-color: #fab387; border-radius: 4px; }"
            );
    } else {
        m_bridgeHealthBar->setStyleSheet(
            "QProgressBar::chunk { background-color: #a6e3a1; border-radius: 4px; }"
            );
    }

    updateBoatsList();
}

void MainWindow::updateBoatsList()
{
    m_boatsList->clear();

    QList<QObject*> boats = m_simulator->getAllBoats();

    QList<Boat*> sideA, sideB;
    for (QObject *obj : boats) {
        Boat *boat = qobject_cast<Boat*>(obj);
        if (boat) {
            if (boat->side() == "A") sideA.append(boat);
            else sideB.append(boat);
        }
    }

    QListWidgetItem *headerA = new QListWidgetItem("═══ ՔԱՂԱՔ Ա ═══");
    headerA->setForeground(QColor("#a6e3a1"));
    QFont headerFont;
    headerFont.setBold(true);
    headerFont.setPointSize(10);
    headerA->setFont(headerFont);
    m_boatsList->addItem(headerA);

    // Ա քաղաքի նավակները
    for (Boat *boat : sideA) {
        QString status = boat->isMoving() ? "Շարժվում է" : "Նավահանգիստ";
        QString cargo = boat->cargoWeight() > 0 ?
                            QString("Բեռ: %1 (%2 կգ)").arg(boat->cargoType()).arg(boat->cargoWeight()) : "Բեռ: Դատարկ";

        QString allowedCargo = "✓ Թույլատրված: ";
        QStringList allowed = boat->allowedCargoTypes();
        if (allowed.isEmpty()) {
            allowedCargo += "Բոլոր տեսակները";
        } else {
            allowedCargo += allowed.join(", ");
        }

        QString info = QString("%1 [%2]\n   %3\n   Վառելիք: %4%% | Արագություն: %5 րոպե\n   Կարողություն: %6 կգ\n   %7\n   %8\n   %9")
                           .arg(boat->name())
                           .arg(boat->id())
                           .arg(status)
                           .arg(boat->fuel())
                           .arg(QString("%1-%2").arg(boat->minSpeed()).arg(boat->maxSpeed()))
                           .arg(boat->capacity())
                           .arg(cargo)
                           .arg(allowedCargo)
                           .arg(boat->description());

        QListWidgetItem *item = new QListWidgetItem(info);
        item->setForeground(boat->color());
        QFont itemFont = item->font();
        itemFont.setBold(false);
        itemFont.setPointSize(9);
        item->setFont(itemFont);
        m_boatsList->addItem(item);

        QListWidgetItem *separator = new QListWidgetItem("───────────────");
        separator->setForeground(QColor("#45475a"));
        m_boatsList->addItem(separator);
    }

    // Բ քաղաք
    QListWidgetItem *headerB = new QListWidgetItem("\n═══ ՔԱՂԱՔ Բ ═══");
    headerB->setForeground(QColor("#89b4fa"));
    headerB->setFont(headerFont);
    m_boatsList->addItem(headerB);

    //Բ քաղաքի նավեր, լրիվ նույն լոգիկան ա
    for (Boat *boat : sideB) {
        QString status = boat->isMoving() ? "Շարժվում է" : "Նավահանգիստ";
        QString cargo = boat->cargoWeight() > 0 ?
                            QString("Բեռ: %1 (%2 կգ)").arg(boat->cargoType()).arg(boat->cargoWeight()) : "Բեռ: Դատարկ";

        // Թույլատրված բեռների ցուցադրում
        QString allowedCargo = "✓ Թույլատրված: ";
        QStringList allowed = boat->allowedCargoTypes();
        if (allowed.isEmpty()) {
            allowedCargo += "Բոլոր տեսակները";
        } else {
            allowedCargo += allowed.join(", ");
        }

        QString info = QString("%1 [%2]\n   %3\n   Վառելիք: %4% | Արագություն: %5 րոպե\n  Կարողություն: %6 կգ\n   %7\n   %8\n   %9")
                           .arg(boat->name())
                           .arg(boat->id())
                           .arg(status)
                           .arg(boat->fuel())
                           .arg(QString("%1-%2").arg(boat->minSpeed()).arg(boat->maxSpeed()))
                           .arg(boat->capacity())
                           .arg(cargo)
                           .arg(allowedCargo)
                           .arg(boat->description());

        QListWidgetItem *item = new QListWidgetItem(info);
        item->setForeground(boat->color());
        QFont itemFont = item->font();
        itemFont.setBold(false);
        itemFont.setPointSize(9);
        item->setFont(itemFont);
        m_boatsList->addItem(item);

        QListWidgetItem *separator = new QListWidgetItem("───────────────");
        separator->setForeground(QColor("#45475a"));
        m_boatsList->addItem(separator);
    }
}


void MainWindow::onLogMessage(const QString &message, const QString &type)
{
    QString color = "#cdd6f4";
    if (type == "error") color = "#f38ba8";
    else if (type == "warning") color = "#fab387";
    else if (type == "success") color = "#a6e3a1";
    else if (type == "info") color = "#89b4fa";

    QString html = QString("<span style='color: %1;'>[%2] %3</span>")
                       .arg(color)
                       .arg(m_simulator->currentTime(), 0, 'f', 1)
                       .arg(message);

    m_logViewer->append(html);
}

void MainWindow::onSimulationFinished()
{
    QString rating;
    int collisions = m_simulator->collisions();
    double time = m_simulator->currentTime();
    int health = m_simulator->bridgeHealth();
    int completedBoats = m_simulator->completedBoats();
    int totalBoats = m_simulator->totalBoats();
    int score = m_simulator->totalScore();
    int stars = m_simulator->stars();

    switch (stars) {
    case 5:
        rating = "⭐⭐⭐⭐⭐ ԿԱՏԱՐՅԱԼ!";
        break;
    case 4:
        rating = "⭐⭐⭐⭐ ԳԵՐԱԶԱՆՑ!";
        break;
    case 3:
        rating = "⭐⭐⭐ ԼԱՎ";
        break;
    case 2:
        rating = "⭐⭐ ԲԱՎԱՐԱՐ";
        break;
    case 1:
        rating = "⭐ ԹՈՒՅԼ";
        break;
    default:
        rating = "ՁԱԽՈՂՎԱԾ\n ԲՈԼՈՐ 7 ՆԱՎԱԿՆԵՐԸ ՊԵՏՔ Է ՀԱՍՑՎԵՆ!";
        break;
    }

    QString msg = QString(
                      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                      "    ՍԻՄՈՒԼՅԱՑԻԱՆ ԱՎԱՐՏՎԵՑ\n"
                      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
                      " ԱՐԴՅՈՒՆՔՆԵՐ:\n"
                      "─────────────────────────────\n"
                      "⏱️  Ժամանակ:        %1 րոպե\n"
                      "🚢 Նավակներ:       %2/%3\n"
                      "💥 Բախումներ:       %4\n"
                      "🌉 Կամուրջ:         %5%%\n"
                      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                      "🏆 ԳՆԱՀԱՏԱԿԱՆ:\n"
                      "%6\n"
                      "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
                      ).arg(time, 0, 'f', 1)
                      .arg(completedBoats)
                      .arg(totalBoats)
                      .arg(collisions)
                      .arg(health)
                      .arg(rating);

    // Պահպանել արդյունքը shared DB-ում
    if (stars >= 1) ResultSaver::save("ship", stars);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Ավարտված");
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #1e1e2e; min-width: 450px; }"
        "QLabel { color: #cdd6f4; font-size: 12px; font-family: 'Courier New'; }"
        "QPushButton { "
        "   background-color: #89b4fa; "
        "   color: #1e1e2e; "
        "   border-radius: 6px; "
        "   padding: 8px 16px; "
        "   font-weight: bold; "
        "}"
        );
    msgBox.exec();
    QApplication::quit();
}

void MainWindow::onShowHelp()
{
    QString helpText =
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        "        📚 ՀՐԱՄԱՆՆԵՐԻ ՈՒՂԵՑՈՒՅՑ \n"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
        " SEND(նավակ, սկիզբ, վերջ, ժամանակ)\n"
        "  -> Նավակը ուղարկում է կամրջով\n"
        "   • նավակ: Նավակի ID (B1-B7)\n"
        "   • սկիզբ: Սկզբնական կողմ (A կամ B)\n"
        "   • վերջ: Նպատակային կողմ (A կամ B)\n"
        "   • ժամանակ: Երբ սկսել շարժումը (րոպե)\n"
        "   Օր․՝ SEND(B1, A->B, 0)\n"

        " LOAD(նավակ, բեռի_տեսակ, քաշ, ժամանակ)\n"
        "  -> Բեռը բեռնում է նավակի վրա\n"
        "   • նավակ: Նավակի ID (B1-B7)\n"
        "   • բեռի_տեսակ:(տես ցանկը ներքևում)\n"
        "   • քաշ: Քաշը կիլոգրամներով\n"
        "   • ժամանակ: Երբ բեռնել (րոպե) \n"
        "   Օր․՝ LOAD(B1, FOOD_DRY, 100, 0)\n"

        " UNLOAD(նավակ, ժամանակ)\n"
        "  -> Բեռը իջեցնում է նավակից\n"
        "   • նավակ: Նավակի ID (B1-B7)\n"
        "   • ժամանակ: Երբ բեռնաթափել (րոպե)\n"
        "   Օր․՝ UNLOAD(B1, 12)\n"

        " REFUEL(նավակ, քանակ, ժամանակ)\n"
        "  -> Վառելիքի լիցքավորում (0-100%%)\n"
        "   • նավակ: Նավակի ID (B1-B7)\n"
        "   • քանակ: Վառելիքի քանակ (0-100)\n"
        "   • ժամանակ: Երբ լիցքավորել (րոպե) \n"
        "   Օր․՝ REFUEL(B1, 50, 10)\n"

        " CHECK_BRIDGE()\n"
        "  -> Ստուգում է կամրջի ընթացիկ վիճակը\n"
        "   • Ցուցադրում է ամրությունը (%%)\n"
        "   • Ցուցադրում է կամուրջն ազատ է թե զբաղված\n"
        "   Օր․՝ CHECK_BRIDGE()\n\n"

        "  REPAIR_BRIDGE(տևողություն)\n"
        "  -> Կամուրջը վերանորոգում է\n"
        "   • տևողություն: Վերանորոգման տևողությունը (րոպե)\n"
        "   • Կամուրջն անգործ է վերանորոգման ընթացքում\n"
        "   Օր․՝ REPAIR_BRIDGE(5)\n\n"


        "  ՕՐԻՆԱԿ ՍՑԵՆԱՐ\n"

        "// B1 նավակ: բեռնվում, շարժվում, բեռնաթափվում, լիցքավորում\n"
        "LOAD(B1, FOOD_DRY, 100, 0)      // Բեռնվում է սկզբնական պահին\n"
        "SEND(B1, A->B, 1)               // Սկսում է 1 րոպեին \n"
        "UNLOAD(B1, 13)                  // Բեռնաթափվում է 13 րոպեին \n"
        "REFUEL(B1, 30, 17)              // Լիցքավորվում է 17 րոպեին\n";


    // Scroll անելու հնարավորություն ենք տալիս
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Հրամանների Ուղեցույց");
    dialog->setMinimumSize(600, 500);
    dialog->setMaximumSize(650, 700);
    dialog->resize(620, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(0, 0, 0, 10);
    layout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QLabel *label = new QLabel(helpText, dialog);
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->setFont(QFont("Courier New", 10));
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setStyleSheet("QLabel { color: #11111b; padding: 20px; background-color: transparent; }");
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollArea->setWidget(label);
    layout->addWidget(scrollArea);

    QPushButton *closeButton = new QPushButton("✖ Փակել", dialog);
    closeButton->setMinimumHeight(40);
    closeButton->setMaximumHeight(40);
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog->setStyleSheet(
        "QDialog { "
        "   background-color: #89dceb; "
        "}"
        "QPushButton { "
        "   background-color: #1e1e2e; "
        "   color: #89dceb; "
        "   border: 2px solid #1e1e2e; "
        "   border-radius: 8px; "
        "   padding: 8px 16px; "
        "   font-weight: bold; "
        "   font-size: 12px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #313244; "
        "   border-color: #313244; "
        "}"
        "QScrollArea { "
        "   border: none; "
        "   background-color: #89dceb; "
        "}"
        "QScrollBar:vertical { "
        "   background-color: #74c7ec; "
        "   width: 12px; "
        "   border-radius: 6px; "
        "}"
        "QScrollBar::handle:vertical { "
        "   background-color: #1e1e2e; "
        "   border-radius: 6px; "
        "   min-height: 20px; "
        "}"
        "QScrollBar::handle:vertical:hover { "
        "   background-color: #313244; "
        "}"
        );

    dialog->exec();
    delete dialog;
}
void MainWindow::onShowProblem()
{
    QString problemText =

        "📋 ԻՐԱՎԻՃԱԿ\n"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        "Ա և Բ քաղաքները գտնվում են Երկրի հյուսիսային հատվածում \n"
        "և իրարից բաժանված են Հյուսիսային նեղուցով։Քաղաքների միջև\n"
        "անցկացված է կամուրջ՝ հատուկ նավարկության աշխատանքները   \n"
        "հեշտացնելու համար։Օդերևութաբանները նախազգուշացրել են,որ \n"
        "նեղուցի այն հատվածը, որն ընկած է քաղաքների միջև,շուտով  \n"
        "սառելու է և նավային հաղորդակցությունը հնարավոր կլինի    \n"
        "վերսկսել միայն գարնանը։   \n"

        " ՆՊԱՏԱԿ: ՏԵՂԱՓՈԽԵԼ ԸՆԴԱՄԵՆԸ 1300 ԿԳ բեռ։\n"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"

        "🔴 Բ → Ա (ԱՐՏԱԿԱՐԳ): 650 ԿԳ\n"
        "─────────────────────────────────────\n"
        "  🏥 MED_ANTIBIOTICS    120 կգ\n"
        "  🏥 MED_FIRSTAID        80 կգ\n"
        "  💊 MED_PAINKILLERS    100 կգ\n"
        "  🍎 FOOD_FRESH         150 կգ\n"
        "  💧 WATER              200 կգ\n\n"

        "🔵 Ա → Բ (ՎԵՐԱԿԱՆԳՆՈՒՄ): 650 ԿԳ\n"
        "─────────────────────────────────────\n"
        "  🏗️ BUILDING_MATERIALS 400 կգ\n"
        "  ⚙️ EQUIPMENT          100 կգ\n"
        "  🩹 MED_BANDAGES       100 կգ\n"
        "  🌾 FOOD_DRY            50 կգ\n\n"


        " ԿԱՆՈՆՆԵՐ\n"
        "✓ Միաժամանակ 1 նավակ կամրջով\n"
        "✓ Ծանր բեռ → դանդաղ շարժում(դրանով պայմանավորված սահմանված է ժամանակային միջակայք)\n"
        "✓ Քիչ վառելիք (<30%) → +30% ժամանակ\n"
        "✓ Բախումներ → կամուրջը վնասվում է(-10% ամրություն)\n"
        "✓ Ժամանակները պետք է ճիշտ հաշվարկել\n\n"

        "⭐ ԳՆԱՀԱՏԱԿԱՆ\n"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        "ԵԹԵ ԲՈԼՈՐ 7 ՆԱՎԱԿՆԵՐԸ ՉԵՆ ՀԱՍՑՎԵԼ:\n"
        "   → ԱՎՏՈՄԱՏ 0 ԱՍՏՂ (անկախ այլ ցուցանիշներից)\n\n"

        "⭐⭐⭐⭐⭐ ԿԱՏԱՐՅԱԼ:\n"
        "   • 7/7 նավակ հասցված\n"
        "   • 1300 կգ բեռ\n"
        "   • Կամուրջ 100%\n"
        "   • 0 բախում\n\n"

        "⭐⭐⭐⭐ ԳԵՐԱԶԱՆՑ:\n"
        "   • 7/7 նավակ հասցված\n"
        "   • 1300 կգ բեռ\n"
        "   • Կամուրջ 80%+\n"
        "   • Ամենաշատը 1 բախում\n\n"

        "⭐⭐⭐ ԼԱՎ:\n"
        "   • 7/7 նավակ հասցված\n"
        "   • 1170+ կգ (90%)\n"
        "   • Կամուրջ 60%+\n\n"

        "⭐⭐ ԲԱՎԱՐԱՐ:\n"
        "   • 7/7 նավակ հասցված\n"
        "   • 910+ կգ (70%)\n\n"

        "⭐ ԹՈՒՅԼ:\n"
        "   • 7/7 նավակ հասցված\n"
        "   • Ցանկացած քանակ\n\n";



    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(" Միսիայի Նկարագրություն");
    dialog->setMinimumSize(600, 500);
    dialog->setMaximumSize(650, 700);
    dialog->resize(620, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(0, 0, 0, 10);
    layout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QLabel *label = new QLabel(problemText, dialog);
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->setFont(QFont("Courier New", 9));
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setStyleSheet("QLabel { color: #11111b; padding: 15px; background-color: transparent; }");
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollArea->setWidget(label);
    layout->addWidget(scrollArea);

    QPushButton *closeButton = new QPushButton("✖ Փակել", dialog);
    closeButton->setMinimumHeight(35);
    closeButton->setMaximumHeight(35);
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog->setStyleSheet(
        "QDialog { "
        "   background-color: #89dceb; "
        "}"
        "QPushButton { "
        "   background-color: #1e1e2e; "
        "   color: #89dceb; "
        "   border: 2px solid #1e1e2e; "
        "   border-radius: 8px; "
        "   padding: 6px 12px; "
        "   font-weight: bold; "
        "   font-size: 11px; "
        "}"
        "QPushButton:hover { "
        "   background-color: #313244; "
        "   border-color: #313244; "
        "}"
        "QScrollArea { "
        "   border: none; "
        "   background-color: #89dceb; "
        "}"
        "QScrollBar:vertical { "
        "   background-color: #74c7ec; "
        "   width: 12px; "
        "   border-radius: 6px; "
        "}"
        "QScrollBar::handle:vertical { "
        "   background-color: #1e1e2e; "
        "   border-radius: 6px; "
        "   min-height: 20px; "
        "}"
        "QScrollBar::handle:vertical:hover { "
        "   background-color: #313244; "
        "}"
        );

    dialog->exec();
    delete dialog;
}
