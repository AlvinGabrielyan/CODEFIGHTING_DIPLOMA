// #include "mainwindow.h"
// #include "resultsaver.h"
// #include <QPainter>
// #include <QScrollArea>
// #include <QFont>
// #include <QPainterPath>
// #include <QTime>
// #include <QDialog>
// #include <QStyle>
// #include <QApplication>
// #include <cmath>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
// {
//     setWindowTitle("Տիեզերագնացների Հետազոտական Առաքելություն");
//     setMinimumSize(1400, 900);
//     engine = new SpaceExplorerEngine(this);
//     setupUI();

//     connect(engine, &SpaceExplorerEngine::explorersChanged, this, &MainWindow::updateVisualization);
//     connect(engine, &SpaceExplorerEngine::scannedCellsChanged, this, &MainWindow::updateVisualization);
//     connect(engine, &SpaceExplorerEngine::errorOccurred, this, &MainWindow::onErrorOccurred);
//     connect(engine, &SpaceExplorerEngine::missionStatusChanged, this, [this]() {
//         statusLabel->setText(engine->missionStatus());
//         scanCountLabel->setText(QString("%1/%2").arg(engine->scannedCount()).arg(engine->totalTerritories()));
//         if (engine->isRunning()) {
//             startPauseBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
//             startPauseBtn->setText("Դադար");
//         } else {
//             startPauseBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
//             startPauseBtn->setText("Սկսել");
//         }

//         QString status = engine->missionStatus();
//         if (!engine->isRunning() && status.contains("Հաջողություն")) {
//             QTimer::singleShot(300, this, &MainWindow::showStarRatingDialog);
//         }    });
// }

// MainWindow::~MainWindow() {}

// void MainWindow::setupUI()
// {
//     QWidget *centralWidget = new QWidget(this);
//     QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
//     mainLayout->setSpacing(15);
//     mainLayout->setContentsMargins(20, 20, 20, 20);

//     QPalette pal = palette();
//     pal.setColor(QPalette::Window, QColor(30, 58, 138));
//     setPalette(pal);
//     setAutoFillBackground(true);

//     mainLayout->addWidget(createLeftPanel(), 1);
//     mainLayout->addWidget(createCenterPanel(), 2);
//     mainLayout->addWidget(createRightPanel(), 1);
//     setCentralWidget(centralWidget);
// }

// QWidget* MainWindow::createLeftPanel()
// {
//     QGroupBox *leftPanel = new QGroupBox("Ընդհանուր նկարագրություն");
//     leftPanel->setStyleSheet(
//         "QGroupBox { background-color: rgba(255, 255, 255, 30); "
//         "border: 1px solid rgba(255, 255, 255, 64); "
//         "border-radius: 10px; color: lightblue; font-size: 12pt; font-weight: bold; padding: 8px; }"
//         "QLabel { color: lightgrey; }");

//     QVBoxLayout *layout = new QVBoxLayout(leftPanel);

//     QLabel *desc1 = new QLabel(QString("<b>Խնդրի նկարագրություն:</b> Տիեզերագնացները ստացել են առաջադրանք՝ ուսումնասիրել մոլորակների ազդեցության գոտիները։ Որոնք բաժանված են սեկտորների և տարբերվում են սովորական մոլորակների ազդեցության գոտիներից։ Մոլորակները ու տիեզերագնացները 3ն են։ Փորձեք կանոններին հետևելով կատարել խնդիրը, հնարավորինս քիչ անցնելով մոլորակների չպատկանող տարածքները (%1×%1)").arg(engine->gridSize()));
//     desc1->setWordWrap(true);
//     desc1->setStyleSheet("font-size: 11pt;");

//     QLabel *desc4 = new QLabel(" <b>Կանոններ:</b>");
//     desc4->setStyleSheet("font-size: 11pt;");

//     QLabel *rule1 = new QLabel("  • Նշված տարածքից դուրս գալ չի կարելի");
//     rule1->setStyleSheet("font-size: 10pt;");

//     QLabel *rule2 = new QLabel("  • Ուրաքանչյուր սեկտոր պետք է մուտք գործել ճիշտ 1 անգամ");
//     rule2->setStyleSheet("font-size: 10pt;");

//     QLabel *rule3 = new QLabel("  • Ուղիները չպետք է հատվեն");
//     rule3->setStyleSheet("font-size: 10pt;");

//     QLabel *rule4 = new QLabel("  • Հրամանները զգայուն չեն մեծատառերին");
//     rule4->setStyleSheet("font-size: 10pt;");

//     QLabel *rule5 = new QLabel("  • 2-րդ տիեզերագնացը երկու սեկտոր է անցնում միանգամից");
//     rule5->setStyleSheet("font-size: 10pt;");

//     QLabel *rule6 = new QLabel("  • Մոլորակի կենտրոն մտնել հնարավոր չէ");
//     rule6->setStyleSheet("font-size: 10pt;");

//     QLabel *rule7 = new QLabel("  • 2-րդ տիեզերագնացը առանց սկանավորելու անցնում է մի սեկտորի վրայով, որը համարվում է չայցելաց");
//     rule7->setStyleSheet("font-size: 10pt;font-weight: bold;");

//     QLabel *errorLabel = new QLabel(" <b>Սխալների գրանցում:</b>");
//     errorLabel->setStyleSheet("font-size: 11pt; margin-top: 10px;");

//     errorLog = new QListWidget();
//     errorLog->setStyleSheet(
//         "QListWidget { background-color: rgba(255, 100, 100, 30); "
//         "border: 1px solid rgba(255, 100, 100, 80); "
//         "border-radius: 8px; color: lightgrey; font-size: 9pt; padding: 5px; }");
//     errorLog->setMaximumHeight(150);

//     layout->addWidget(desc1);
//     layout->addWidget(desc4);
//     layout->addWidget(rule1);
//     layout->addWidget(rule2);
//     layout->addWidget(rule3);
//     layout->addWidget(rule4);
//     layout->addWidget(rule5);
//     layout->addWidget(rule6);
//     layout->addWidget(rule7);

//     // Star rating info table
//     QLabel *starTitle = new QLabel("⭐ Գնահատման համակարգ");
//     starTitle->setStyleSheet("font-size: 11pt; font-weight: bold; color: #ffd700; margin-top: 8px;");
//     layout->addWidget(starTitle);

//     QLabel *starNote = new QLabel("Նախ պետք է բոլոր սեկտորները սկանավորել (⭐1)։\nՄնացած աստղերը կախված են վատնած քայլերից.");
//     starNote->setStyleSheet("font-size: 9pt; color: lightgrey;");
//     starNote->setWordWrap(true);
//     layout->addWidget(starNote);

//     QWidget *starTableWidget = new QWidget();
//     QGridLayout *starTable = new QGridLayout(starTableWidget);
//     starTable->setSpacing(3);
//     starTable->setContentsMargins(0, 2, 0, 2);

//     auto makeCell = [](const QString &text, const QString &color = "lightgrey", bool bold = false) {
//         QLabel *l = new QLabel(text);
//         l->setAlignment(Qt::AlignCenter);
//         l->setStyleSheet(QString("font-size: 9pt; color: %1; %2 "
//                                  "background-color: rgba(255,255,255,15); "
//                                  "border-radius: 3px; padding: 3px;")
//                              .arg(color, bold ? "font-weight: bold;" : ""));
//         return l;
//     };

//     starTable->addWidget(makeCell("Գնահատ.",    "#aaddff", true), 0, 0);
//     starTable->addWidget(makeCell("Պայման",     "#aaddff", true), 0, 1);

//     starTable->addWidget(makeCell("⭐",         "lightgrey"), 1, 0);
//     starTable->addWidget(makeCell("Բոլոր սեկտ. սկանավ.", "white"), 1, 1);

//     starTable->addWidget(makeCell("⭐⭐",       "lightgrey"), 2, 0);
//     starTable->addWidget(makeCell("Վատնած ≤ 18", "white"),   2, 1);

//     starTable->addWidget(makeCell("⭐⭐⭐",     "lightgrey"), 3, 0);
//     starTable->addWidget(makeCell("Վատնած ≤ 10", "white"),   3, 1);

//     starTable->addWidget(makeCell("⭐⭐⭐⭐",   "#ffa500"),   4, 0);
//     starTable->addWidget(makeCell("Վատնած ≤ 6",  "#ffa500"), 4, 1);

//     starTable->addWidget(makeCell("⭐⭐⭐⭐⭐", "#ffd700"),   5, 0);
//     starTable->addWidget(makeCell("Վատնած ≤ 5",  "#ffd700"), 5, 1);

//     starTableWidget->setStyleSheet("background-color: transparent;");
//     layout->addWidget(starTableWidget);

//     layout->addWidget(errorLabel);
//     layout->addWidget(errorLog);
//     layout->addStretch();

//     return leftPanel;
// }

// QWidget* MainWindow::createCenterPanel()
// {
//     QGroupBox *centerPanel = new QGroupBox("Մոլորակների Քարտեզ");
//     centerPanel->setStyleSheet(
//         "QGroupBox { background-color: rgba(255, 255, 255, 30); "
//         "border: 1px solid rgba(255, 255, 255, 64); "
//         "border-radius: 12px; color: lightblue; font-size: 14pt; font-weight: bold; padding: 10px; }");

//     QVBoxLayout *layout = new QVBoxLayout(centerPanel);
//     QHBoxLayout *statsLayout = new QHBoxLayout();

//     scanCountLabel = new QLabel("0/0");
//     scanCountLabel->setStyleSheet(
//         "background-color: rgba(255, 255, 255, 64); "
//         "border-radius: 15px; color: lightblue; font-size: 12pt; "
//         "font-weight: bold; padding: 5px 15px;");
//     scanCountLabel->setAlignment(Qt::AlignCenter);

//     QLabel *wastedLabel = new QLabel();
//     wastedLabel->setObjectName("wastedLabel");
//     wastedLabel->setStyleSheet(
//         "background-color: rgba(255, 140, 66, 128); "
//         "border-radius: 12px; color: lightblue; font-size: 11pt; "
//         "font-weight: bold; padding: 5px 12px;");
//     wastedLabel->setAlignment(Qt::AlignCenter);

//     connect(engine, &SpaceExplorerEngine::wastedMovesChanged, [this, wastedLabel]() {
//         wastedLabel->setText(QString(" Վատնած: %1").arg(engine->wastedMoves()));
//     });
//     wastedLabel->setText("Վատնած: 0");

//     statsLayout->addStretch();
//     statsLayout->addWidget(wastedLabel);
//     statsLayout->addWidget(scanCountLabel);
//     layout->addLayout(statsLayout);

//     gridWidget = new GridWidget(engine, this);
//     layout->addWidget(gridWidget, 0, Qt::AlignCenter);

//     statusLabel = new QLabel("Պատրաստ");
//     statusLabel->setStyleSheet("color: grey; font-size: 12pt; font-weight: bold;");
//     statusLabel->setAlignment(Qt::AlignCenter);
//     layout->addWidget(statusLabel);

//     QHBoxLayout *btnLayout = new QHBoxLayout();

//     startPauseBtn = new QPushButton("Սկսել");
//     startPauseBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
//     startPauseBtn->setIconSize(QSize(20, 20));
//     startPauseBtn->setStyleSheet(
//         "QPushButton { background-color: #10b981; color: darkblue; "
//         "font-size: 11pt; padding: 10px 20px; border-radius: 8px; font-weight: bold; }"
//         "QPushButton:hover { background-color: #6EE7B7; }");
//     connect(startPauseBtn, &QPushButton::clicked, this, &MainWindow::onStartPause);

//     QPushButton *stepBtn = new QPushButton("Քայլ առ քայլ");
//     stepBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaSkipForward));
//     stepBtn->setIconSize(QSize(20, 20));
//     stepBtn->setStyleSheet(
//         "QPushButton { background-color: #f59e0b; color: darkblue; "
//         "font-size: 11pt; padding: 8px 15px; border-radius: 8px; font-weight: bold;}"
//         "QPushButton:hover { background-color: #FCD34D; }");
//     connect(stepBtn, &QPushButton::clicked, this, &MainWindow::onStepByStep);

//     resetBtn = new QPushButton("Վերսկսել");
//     resetBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
//     resetBtn->setIconSize(QSize(20, 20));
//     resetBtn->setStyleSheet(
//         "QPushButton { background-color: #ef4444; color: darkblue; "
//         "font-size: 11pt; padding: 8px 15px; border-radius: 8px; font-weight: bold;}"
//         "QPushButton:hover { background-color: #FECACA; }");
//     connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onReset);

//     btnLayout->addStretch();
//     btnLayout->addWidget(stepBtn);
//     btnLayout->addWidget(startPauseBtn);
//     btnLayout->addWidget(resetBtn);
//     btnLayout->addStretch();

//     layout->addLayout(btnLayout);
//     return centerPanel;
// }


// QWidget* MainWindow::createRightPanel()
// {
//     QGroupBox *rightPanel = new QGroupBox("Հրամանների կառավարում");
//     rightPanel->setStyleSheet(
//         "QGroupBox { background-color: rgba(255, 255, 255, 30); "
//         "border: 1px solid rgba(255, 255, 255, 64); "
//         "border-radius: 9px; color: lightblue; font-size: 11pt; font-weight: bold; padding: 7px; }"
//         "QLabel { color: white; font-size: 11pt; }");

//     QVBoxLayout *layout = new QVBoxLayout(rightPanel);

//     QLabel *selectorLabel = new QLabel("Ընտրել տիեզերագնաց:");
//     explorerSelector = new QComboBox();
//     explorerSelector->addItem("Տիեզերագնաց #1 🔴");
//     explorerSelector->addItem("Տիեզերագնաց #2 🟢");
//     explorerSelector->addItem("Տիեզերագնաց #3 🩷");
//     explorerSelector->setStyleSheet(
//         "QComboBox { background-color: rgba(255, 255, 255, 64); color: lightgrey; "
//         "border: 1px solid rgba(255, 255, 255, 96); border-radius: px; padding: 4px; }");
//     connect(explorerSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
//             this, &MainWindow::onExplorerChanged);

//     QLabel *availLabel = new QLabel("Հասանելի հրամաններ:");
//     QLabel *commands = new QLabel(
//         "• MOVE UP/DOWN/RIGHT/LEFT - Շարժում\n"
//         "• SCAN - Սկանավորել սեկտորը\n"
//         "• WAIT - Սպասել\n"
//         "• STOP - Ավարտել");
//     commands->setStyleSheet(
//         "background-color: rgb(120, 180, 255); "
//         "border-radius: 8px; padding: 10px; font-size: 10pt;");

//     QLabel *inputLabel = new QLabel("Մուտքագրել հրամաններ:");
//     commandInput = new QTextEdit();
//     commandInput->setPlaceholderText("SCAN\nMOVE RIGHT\nSCAN\nMOVE RIGHT\n...");
//     commandInput->setMaximumHeight(120);
//     commandInput->setStyleSheet(
//         "QTextEdit { background-color: rgba(255, 255, 255, 64); color: lightblue; "
//         "border: 1px solid rgba(255, 255, 255, 96); border-radius: 8px; "
//         "padding: 5px; font-family: Courier; font-size: 10pt; }");

//     QHBoxLayout *cmdBtnLayout = new QHBoxLayout();

//     QPushButton *addBtn = new QPushButton("Ավելացնել հրամաններ");
//     addBtn->setStyleSheet(
//         "QPushButton { background-color: #3b82f6; color: lightblue; "
//         "font-size: 11pt; padding: 8px; border-radius: 8px; }"
//         "QPushButton:hover { background-color: #2563eb; }");
//     connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddCommands);

//     QPushButton *clearBtn = new QPushButton("Մաքրել");
//     clearBtn->setStyleSheet(
//         "QPushButton { background-color: #ef4444; color: lightblue; "
//         "font-size: 10pt; padding: 8px; border-radius: 8px; }"
//         "QPushButton:hover { background-color: #dc2626; }");
//     connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearCommands);

//     cmdBtnLayout->addWidget(addBtn, 3);
//     cmdBtnLayout->addWidget(clearBtn, 1);

//     QLabel *currentLabel = new QLabel("Ընթացիկ հրամաններ:");
//     currentCommands = new QTextEdit();
//     currentCommands->setReadOnly(true);
//     currentCommands->setStyleSheet(
//         "QTextEdit { background-color: rgba(255, 255, 255, 64); color: white; "
//         "border: 1px solid rgba(255, 255, 255, 96); border-radius: 8px; "
//         "padding: 5px; font-family: Courier; font-size: 10pt; }");

//     layout->addWidget(selectorLabel);
//     layout->addWidget(explorerSelector);
//     layout->addWidget(availLabel);
//     layout->addWidget(commands);
//     layout->addWidget(inputLabel);
//     layout->addWidget(commandInput);
//     layout->addLayout(cmdBtnLayout);
//     layout->addWidget(currentLabel);
//     layout->addWidget(currentCommands);
//     layout->addStretch();

//     return rightPanel;
// }

// void MainWindow::onStartPause()
// {
//     if (engine->isRunning()) {
//         engine->pauseMission();
//     } else {
//         engine->startMission();
//     }
// }

// void MainWindow::onReset()
// {
//     engine->resetMission();
//     commandInput->clear();
//     currentCommands->clear();
//     errorLog->clear();
//     onExplorerChanged(explorerSelector->currentIndex());
//     gridWidget->update();
// }

// void MainWindow::onStepByStep()
// {
//     if (!engine->isRunning()) {
//         engine->startMission();
//         QTimer::singleShot(100, [this]() {
//             engine->pauseMission();
//         });
//     }
// }

// void MainWindow::onAddCommands()
// {
//     QString text = commandInput->toPlainText();
//     QStringList commands = text.split('\n', Qt::SkipEmptyParts);

//     if (commands.isEmpty()) {
//         return;
//     }

//     int explorerId = explorerSelector->currentIndex() + 1;
//     engine->addCommands(explorerId, commands);
//     onExplorerChanged(explorerSelector->currentIndex());
//     commandInput->clear();
// }

// void MainWindow::onClearCommands()
// {
//     int explorerId = explorerSelector->currentIndex() + 1;
//     engine->clearCommands(explorerId);
//     onExplorerChanged(explorerSelector->currentIndex());
// }

// void MainWindow::onExplorerChanged(int index)
// {
//     int explorerId = index + 1;
//     QStringList allCommands = engine->getCommands(explorerId);
//     QString display;
//     for (int i = 0; i < allCommands.size(); i++) {
//         display += QString("%1. %2\n").arg(i + 1).arg(allCommands[i]);
//     }
//     currentCommands->setText(display.isEmpty() ? "Հրամաններ չկան" : display);
// }

// void MainWindow::updateVisualization()
// {
//     gridWidget->update();
// }

// void MainWindow::onSpeedChanged(int value)
// {
//     speedLabel->setText(QString("%1ms").arg(value));
//     engine->setSpeed(value);
// }

// void MainWindow::onErrorOccurred(const QString &error)
// {
//     QTime currentTime = QTime::currentTime();
//     QString timeStr = currentTime.toString("hh:mm:ss");
//     errorLog->addItem(QString("[%1] %2").arg(timeStr).arg(error));
//     errorLog->scrollToBottom();
// }

// // GridWidget Implementation
// GridWidget::GridWidget(SpaceExplorerEngine *engine, QWidget *parent)
//     : QWidget(parent), m_engine(engine), cellSize(60)
// {
//     setMinimumSize(cellSize * 8, cellSize * 8);
//     setMaximumSize(cellSize * 8, cellSize * 8);

//     planets.append({2, 2, 30, QColor(255, 140, 66)});
//     planets.append({5, 5, 25, QColor(155, 89, 182)});
//     planets.append({6, 1, 20, QColor(52, 152, 219)});
// }

// void GridWidget::updateGrid()
// {
//     update();
// }

// void GridWidget::paintEvent(QPaintEvent *event)
// {
//     Q_UNUSED(event);

//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);

//     int gridSize = m_engine->gridSize();

//     // Background
//     painter.fillRect(rect(), QColor(20, 20, 46));

//     // Draw stars
//     painter.setPen(Qt::NoPen);
//     for (int i = 0; i < 50; i++) {
//         int x = (i * 73) % width();
//         int y = (i * 59) % height();
//         int r = (i % 3) + 1;
//         painter.setBrush(QColor(255, 255, 255, 100));
//         painter.drawEllipse(x, y, r, r);
//     }

//     // Draw grid
//     painter.setPen(QPen(QColor(255, 255, 255, 50), 1));
//     for (int i = 0; i <= gridSize; i++) {
//         painter.drawLine(i * cellSize, 0, i * cellSize, gridSize * cellSize);
//         painter.drawLine(0, i * cellSize, gridSize * cellSize, i * cellSize);
//     }

//     // Planet territory overlay removed intentionally — all sectors look the same,
//     // explorers are not tied to planet colors.

//     // Draw planet territories
//     QVariantList territories = m_engine->planetTerritories();
//     QList<QColor> planetColors = {
//         QColor(255, 140, 66, 60),
//         QColor(155, 89, 182, 60),
//         QColor(52, 152, 219, 60)
//     };

//     for (const auto &terr : territories) {
//         QVariantMap territory = terr.toMap();
//         int x = territory["x"].toInt();
//         int y = territory["y"].toInt();
//         int planetId = territory["planetId"].toInt();

//         if (planetId >= 0 && planetId < planetColors.size()) {
//             painter.setBrush(planetColors[planetId]);
//             painter.setPen(Qt::NoPen);
//             painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
//         }
//     }

//     // Draw visited but not scanned cells (wasted moves)
//     QVariantList visited = m_engine->visitedCells();
//     QVariantList scanned = m_engine->scannedCells();
//     QSet<QString> scannedSet;
//     for (const auto &cell : scanned) {
//         QVariantMap cellMap = cell.toMap();
//         scannedSet.insert(QString("%1,%2").arg(cellMap["x"].toInt()).arg(cellMap["y"].toInt()));
//     }

//     painter.setBrush(QColor(255, 100, 100, 80));
//     for (const auto &cell : visited) {
//         QVariantMap cellMap = cell.toMap();
//         int x = cellMap["x"].toInt();
//         int y = cellMap["y"].toInt();
//         QString key = QString("%1,%2").arg(x).arg(y);

//         if (!scannedSet.contains(key)) {
//             painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
//         }
//     }

//     // Draw planets with realistic appearance
//     for (const auto &planet : planets) {
//         int px = planet.x * cellSize + cellSize / 2;
//         int py = planet.y * cellSize + cellSize / 2;
//         QColor col = planet.color;

//         // Glow
//         QRadialGradient gradient(px, py, planet.size * 1.2);
//         col.setAlpha(40);
//         gradient.setColorAt(0, col);
//         col.setAlpha(0);
//         gradient.setColorAt(1, col);
//         painter.setBrush(gradient);
//         painter.setPen(Qt::NoPen);
//         painter.drawEllipse(QPoint(px, py), static_cast<int>(planet.size * 1.2), static_cast<int>(planet.size * 1.2));

//         // Planet body
//         QRadialGradient planetGradient(px - planet.size * 0.3, py - planet.size * 0.3, planet.size * 1.5);
//         col = planet.color;
//         col.setAlpha(200);
//         planetGradient.setColorAt(0, col.lighter(150));
//         col.setAlpha(220);
//         planetGradient.setColorAt(0.6, col);
//         col.setAlpha(180);
//         planetGradient.setColorAt(1, col.darker(120));

//         painter.setBrush(planetGradient);
//         painter.setPen(QPen(col.darker(150), 1));
//         painter.drawEllipse(QPoint(px, py), static_cast<int>(planet.size), static_cast<int>(planet.size));

//         // Craters
//         painter.setPen(Qt::NoPen);
//         QColor craterColor = col.darker(130);
//         craterColor.setAlpha(80);
//         painter.setBrush(craterColor);
//         painter.drawEllipse(QPoint(px + planet.size * 0.3, py - planet.size * 0.2), static_cast<int>(planet.size * 0.2), static_cast<int>(planet.size * 0.2));
//         painter.drawEllipse(QPoint(px - planet.size * 0.4, py + planet.size * 0.3), static_cast<int>(planet.size * 0.15), static_cast<int>(planet.size * 0.15));

//         // Highlight
//         QRadialGradient highlight(px - planet.size * 0.4, py - planet.size * 0.4, planet.size * 0.5);
//         highlight.setColorAt(0, QColor(255, 255, 255, 100));
//         highlight.setColorAt(1, QColor(255, 255, 255, 0));
//         painter.setBrush(highlight);
//         painter.drawEllipse(QPoint(px - planet.size * 0.3, py - planet.size * 0.3), static_cast<int>(planet.size * 0.4), static_cast<int>(planet.size * 0.4));
//     }

//     // Draw scanned cells
//     painter.setBrush(QColor(74, 222, 128, 120));
//     for (const auto &cell : scanned) {
//         QVariantMap cellMap = cell.toMap();
//         int x = cellMap["x"].toInt();
//         int y = cellMap["y"].toInt();
//         painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
//     }

//     // Draw paths
//     QVariantList explorers = m_engine->explorers();
//     for (const auto &exp : explorers) {
//         QVariantMap explorer = exp.toMap();
//         int id = explorer["id"].toInt();
//         QColor color(explorer["color"].toString());
//         color.setAlpha(100);
//         painter.setBrush(color);

//         QVariantList path = m_engine->getPathHistory(id);
//         for (const auto &p : path) {
//             QVariantMap point = p.toMap();
//             int px = point["x"].toInt() * cellSize + cellSize / 2;
//             int py = point["y"].toInt() * cellSize + cellSize / 2;
//             painter.drawEllipse(QPoint(px, py), 3, 3);
//         }
//     }

//     // Draw footprints
//     painter.setPen(QPen(QColor(255, 255, 255, 100), 1));
//     for (const auto &cell : visited) {
//         QVariantMap cellMap = cell.toMap();
//         int x = cellMap["x"].toInt();
//         int y = cellMap["y"].toInt();

//         int fx = x * cellSize + cellSize / 2;
//         int fy = y * cellSize + cellSize / 2;

//         painter.setBrush(QColor(200, 200, 200, 80));
//         painter.drawEllipse(QPoint(fx - 5, fy), 4, 6);
//         painter.drawEllipse(QPoint(fx + 5, fy), 4, 6);
//     }

//     // Draw astronauts
//     for (const auto &exp : explorers) {
//         QVariantMap explorer = exp.toMap();
//         int x = explorer["x"].toInt();
//         int y = explorer["y"].toInt();
//         int id = explorer["id"].toInt();
//         QColor color(explorer["color"].toString());

//         int cx = x * cellSize + cellSize / 2;
//         int cy = y * cellSize + cellSize / 2;

//         // Helmet
//         color.setAlpha(220);
//         painter.setBrush(color);
//         painter.setPen(QPen(QColor(255, 255, 255, 150), 2));
//         painter.drawEllipse(QPoint(cx, cy - 5), 16, 16);

//         // Visor
//         painter.setBrush(QColor(100, 150, 200, 180));
//         painter.drawEllipse(QPoint(cx, cy - 5), 10, 10);

//         // Body
//         painter.setBrush(color);
//         painter.setPen(Qt::NoPen);
//         painter.drawRoundedRect(cx - 10, cy + 8, 20, 15, 3, 3);

//         // Arms
//         painter.drawEllipse(QPoint(cx - 14, cy + 12), 4, 8);
//         painter.drawEllipse(QPoint(cx + 14, cy + 12), 4, 8);

//         // ID badge
//         painter.setBrush(QColor(255, 255, 255, 200));
//         painter.drawEllipse(QPoint(cx, cy + 14), 6, 6);
//         painter.setPen(Qt::black);
//         painter.setFont(QFont("Arial", 8, QFont::Bold));
//         painter.drawText(QRect(cx - 6, cy + 8, 12, 12), Qt::AlignCenter, QString::number(id));
//     }
// }

// void MainWindow::showStarRatingDialog()
// {
//     int wasted = engine->wastedMoves();
//     int scanned = engine->scannedCount();
//     int total = engine->totalTerritories();
//     int scanPct = (total > 0) ? (scanned * 100 / total) : 0;
//     bool fullScan = (scanned == total);

//     // 1 star = minimum: all territories scanned (prerequisite)
//     // 2-5 stars = based purely on wasted moves (moves outside planet territories)
//     bool c1 = fullScan;              // ⭐1: բոլոր սեկտորները սկանավորված
//     bool c2 = (c1 && wasted <= 18); // ⭐2
//     bool c3 = (c1 && wasted <= 10); // ⭐3
//     bool c4 = (c1 && wasted <= 6);  // ⭐4
//     bool c5 = (c1 && wasted <= 5);  // ⭐5: գրեթե կատարյալ

//     int stars = 0;
//     if (c1) stars = 1;
//     if (c2) stars = 2;
//     if (c3) stars = 3;
//     if (c4) stars = 4;
//     if (c5) stars = 5;

//     QDialog *dialog = new QDialog(this);
//     dialog->setWindowTitle("Առաքելության արդյունք");
//     dialog->setFixedSize(400, 320);
//     dialog->setStyleSheet(R"(
//         QDialog { background-color: #0d1b4b; }
//         QLabel  { color: #eaeaea; background-color: transparent; }
//         QPushButton {
//             border: none; border-radius: 8px;
//             font-size: 11pt; font-weight: bold; padding: 10px 22px;
//         }
//     )");

//     QVBoxLayout *layout = new QVBoxLayout(dialog);
//     layout->setSpacing(10);
//     layout->setContentsMargins(30, 22, 30, 22);

//     // Title
//     QString titleText;
//     switch (stars) {
//     case 5: titleText = "🏆 ԿԱՏԱՐՅԱԼ ԱՌԱՔԵԼՈՒԹՅՈՒՆ!"; break;
//     case 4: titleText = " ԳԵՐԱԶԱՆՑ!";               break;
//     case 3: titleText = " ԼԱՎ ԱՐԴՅՈՒՆՔ";            break;
//     case 2: titleText = " ԲԱՎԱՐԱՐ";                  break;
//     case 1: titleText = " ԱՎԱՐՏՎԱԾ";                  break;
//     default: titleText = "Բոլոր սեկտորները չեն սկանավորված"; break;
//     }
//     QLabel *titleLabel = new QLabel(titleText);
//     titleLabel->setAlignment(Qt::AlignCenter);
//     titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #ffd700;");
//     layout->addWidget(titleLabel);

//     // Stars
//     QString starsHtml;
//     for (int i = 0; i < 5; i++)
//         starsHtml += (i < stars)
//                          ? "<span style='color:#ffd700; font-size:28pt;'>★</span>"
//                          : "<span style='color:#333;   font-size:28pt;'>★</span>";
//     QLabel *starsLabel = new QLabel(starsHtml);
//     starsLabel->setAlignment(Qt::AlignCenter);
//     starsLabel->setTextFormat(Qt::RichText);
//     layout->addWidget(starsLabel);

//     QLabel *scoreLabel = new QLabel(QString("%1 / 5 աստղ").arg(stars));
//     scoreLabel->setAlignment(Qt::AlignCenter);
//     scoreLabel->setStyleSheet("font-size: 10pt; color: #aaa;");
//     layout->addWidget(scoreLabel);

//     // Stats
//     QLabel *statsLabel = new QLabel(
//         QString("Վատնած քայլեր: <b style='color:#ff8c42;'>%1</b>"
//                 "&nbsp;&nbsp;|&nbsp;&nbsp;"
//                 "Սկանավորված: <b style='color:#4ade80;'>%2/%3</b>")
//             .arg(wasted).arg(scanned).arg(total));
//     statsLabel->setAlignment(Qt::AlignCenter);
//     statsLabel->setTextFormat(Qt::RichText);
//     statsLabel->setStyleSheet("font-size: 11pt; margin-bottom: 4px;");
//     layout->addWidget(statsLabel);

//     layout->addStretch();

//     bool shouldRetry = false;

//     if (stars < 5) {
//         QLabel *retryLabel = new QLabel("Դուր եկա՞վ ձեր ստացած արդյունքը,\nթե՞ ուզում եք կրկին փորձել։");
//         retryLabel->setAlignment(Qt::AlignCenter);
//         retryLabel->setStyleSheet("font-size: 11pt; color: #eaeaea;");
//         layout->addWidget(retryLabel);

//         QHBoxLayout *btnRow = new QHBoxLayout();
//         btnRow->setSpacing(12);

//         QPushButton *closeBtn = new QPushButton("Այո, փակել");
//         closeBtn->setStyleSheet(
//             "QPushButton { background-color: #1e3a8a; color: #eaeaea; "
//             "border: 2px solid #2563eb; border-radius: 8px; "
//             "font-size: 11pt; font-weight: bold; padding: 10px 20px; }"
//             "QPushButton:hover { background-color: #2563eb; }");

//         QPushButton *retryBtn = new QPushButton("Կրկին փորձել");
//         retryBtn->setStyleSheet(
//             "QPushButton { background-color: #10b981; color: #0d1b4b; "
//             "border-radius: 8px; font-size: 11pt; font-weight: bold; padding: 10px 20px; }"
//             "QPushButton:hover { background-color: #6ee7b7; }");

//         btnRow->addStretch();
//         btnRow->addWidget(closeBtn);
//         btnRow->addWidget(retryBtn);
//         btnRow->addStretch();
//         layout->addLayout(btnRow);

//         connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::reject);
//         connect(retryBtn, &QPushButton::clicked, [&]() {
//             shouldRetry = true;
//             dialog->accept();
//         });
//     } else {
//         QPushButton *closeBtn = new QPushButton(" Փակել");
//         closeBtn->setStyleSheet(
//             "QPushButton { background-color: #ffd700; color: #0d1b4b; "
//             "border-radius: 8px; font-size: 12pt; font-weight: bold; padding: 10px 30px; }"
//             "QPushButton:hover { background-color: #fbbf24; }");
//         QHBoxLayout *btnRow = new QHBoxLayout();
//         btnRow->addStretch();
//         btnRow->addWidget(closeBtn);
//         btnRow->addStretch();
//         layout->addLayout(btnRow);
//         connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
//     }

//     // Պահպանել արդյունքը shared DB-ում
//     if (stars >= 1) ResultSaver::save("space", stars);

//     dialog->exec();
//     delete dialog;

//     if (shouldRetry) {
//         engine->resetMission();
//         commandInput->clear();
//         currentCommands->clear();
//         errorLog->clear();
//         onExplorerChanged(explorerSelector->currentIndex());
//         gridWidget->update();
//     }
// }



#include "mainwindow.h"
#include "resultsaver.h"
#include <QPainter>
#include <QScrollArea>
#include <QFont>
#include <QPainterPath>
#include <QTime>
#include <QDialog>
#include <QStyle>
#include <QApplication>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Տիեզերագնացների Հետազոտական Առաքելություն");
    setMinimumSize(1400, 900);
    engine = new SpaceExplorerEngine(this);
    setupUI();

    connect(engine, &SpaceExplorerEngine::explorersChanged, this, &MainWindow::updateVisualization);
    connect(engine, &SpaceExplorerEngine::scannedCellsChanged, this, &MainWindow::updateVisualization);
    connect(engine, &SpaceExplorerEngine::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(engine, &SpaceExplorerEngine::missionStatusChanged, this, [this]() {
        statusLabel->setText(engine->missionStatus());
        scanCountLabel->setText(QString("%1/%2").arg(engine->scannedCount()).arg(engine->totalTerritories()));
        if (engine->isRunning()) {
            startPauseBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
            startPauseBtn->setText("Դադար");
        } else {
            startPauseBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
            startPauseBtn->setText("Սկսել");
        }

        QString status = engine->missionStatus();
        if (!engine->isRunning() && status.contains("Հաջողություն")) {
            QTimer::singleShot(300, this, &MainWindow::showStarRatingDialog);
        }    });
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(30, 58, 138));
    setPalette(pal);
    setAutoFillBackground(true);

    mainLayout->addWidget(createLeftPanel(), 1);
    mainLayout->addWidget(createCenterPanel(), 2);
    mainLayout->addWidget(createRightPanel(), 1);
    setCentralWidget(centralWidget);
}

QWidget* MainWindow::createLeftPanel()
{
    QGroupBox *leftPanel = new QGroupBox("Ընդհանուր նկարագրություն");
    leftPanel->setStyleSheet(
        "QGroupBox { background-color: rgba(255, 255, 255, 30); "
        "border: 1px solid rgba(255, 255, 255, 64); "
        "border-radius: 10px; color: lightblue; font-size: 12pt; font-weight: bold; padding: 8px; }"
        "QLabel { color: lightgrey; }");

    QVBoxLayout *layout = new QVBoxLayout(leftPanel);

    QLabel *desc1 = new QLabel(QString("<b>Խնդրի նկարագրություն:</b> Տիեզերագնացները ստացել են առաջադրանք՝ ուսումնասիրել մոլորակների ազդեցության գոտիները։ Որոնք բաժանված են սեկտորների և տարբերվում են սովորական մոլորակների ազդեցության գոտիներից։ Մոլորակները ու տիեզերագնացները 3ն են։ Փորձեք կանոններին հետևելով կատարել խնդիրը, հնարավորինս քիչ անցնելով մոլորակների չպատկանող տարածքները (%1×%1)").arg(engine->gridSize()));
    desc1->setWordWrap(true);
    desc1->setStyleSheet("font-size: 11pt;");

    QLabel *desc4 = new QLabel(" <b>Կանոններ:</b>");
    desc4->setStyleSheet("font-size: 11pt;");

    QLabel *rule1 = new QLabel("  • Նշված տարածքից դուրս գալ չի կարելի");
    rule1->setStyleSheet("font-size: 10pt;");

    QLabel *rule2 = new QLabel("  • Ուրաքանչյուր սեկտոր պետք է մուտք գործել ճիշտ 1 անգամ");
    rule2->setStyleSheet("font-size: 10pt;");

    QLabel *rule3 = new QLabel("  • Ուղիները չպետք է հատվեն");
    rule3->setStyleSheet("font-size: 10pt;");

    QLabel *rule4 = new QLabel("  • Հրամանները զգայուն չեն մեծատառերին");
    rule4->setStyleSheet("font-size: 10pt;");

    QLabel *rule5 = new QLabel("  • 2-րդ տիեզերագնացը երկու սեկտոր է անցնում միանգամից");
    rule5->setStyleSheet("font-size: 10pt;");

    QLabel *rule6 = new QLabel("  • Մոլորակի կենտրոն մտնել հնարավոր չէ");
    rule6->setStyleSheet("font-size: 10pt;");

    QLabel *rule7 = new QLabel("  • 2-րդ տիեզերագնացը առանց սկանավորելու անցնում է մի սեկտորի վրայով, որը համարվում է չայցելաց");
    rule7->setStyleSheet("font-size: 10pt;font-weight: bold;");

    QLabel *errorLabel = new QLabel(" <b>Սխալների գրանցում:</b>");
    errorLabel->setStyleSheet("font-size: 11pt; margin-top: 10px;");

    errorLog = new QListWidget();
    errorLog->setStyleSheet(
        "QListWidget { background-color: rgba(255, 100, 100, 30); "
        "border: 1px solid rgba(255, 100, 100, 80); "
        "border-radius: 8px; color: lightgrey; font-size: 9pt; padding: 5px; }");
    errorLog->setMaximumHeight(150);

    layout->addWidget(desc1);
    layout->addWidget(desc4);
    layout->addWidget(rule1);
    layout->addWidget(rule2);
    layout->addWidget(rule3);
    layout->addWidget(rule4);
    layout->addWidget(rule5);
    layout->addWidget(rule6);
    layout->addWidget(rule7);

    // Star rating info table
    QLabel *starTitle = new QLabel("⭐ Գնահատման համակարգ");
    starTitle->setStyleSheet("font-size: 11pt; font-weight: bold; color: #ffd700; margin-top: 8px;");
    layout->addWidget(starTitle);

    QLabel *starNote = new QLabel("Նախ պետք է բոլոր սեկտորները սկանավորել (⭐1)։\nՄնացած աստղերը կախված են վատնած քայլերից.");
    starNote->setStyleSheet("font-size: 9pt; color: lightgrey;");
    starNote->setWordWrap(true);
    layout->addWidget(starNote);

    QWidget *starTableWidget = new QWidget();
    QGridLayout *starTable = new QGridLayout(starTableWidget);
    starTable->setSpacing(3);
    starTable->setContentsMargins(0, 2, 0, 2);

    auto makeCell = [](const QString &text, const QString &color = "lightgrey", bool bold = false) {
        QLabel *l = new QLabel(text);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(QString("font-size: 9pt; color: %1; %2 "
                                 "background-color: rgba(255,255,255,15); "
                                 "border-radius: 3px; padding: 3px;")
                             .arg(color, bold ? "font-weight: bold;" : ""));
        return l;
    };

    starTable->addWidget(makeCell("Գնահատ.",    "#aaddff", true), 0, 0);
    starTable->addWidget(makeCell("Պայման",     "#aaddff", true), 0, 1);

    starTable->addWidget(makeCell("⭐",         "lightgrey"), 1, 0);
    starTable->addWidget(makeCell("Բոլոր սեկտ. սկանավ.", "white"), 1, 1);

    starTable->addWidget(makeCell("⭐⭐",       "lightgrey"), 2, 0);
    starTable->addWidget(makeCell("Վատնած ≤ 18", "white"),   2, 1);

    starTable->addWidget(makeCell("⭐⭐⭐",     "lightgrey"), 3, 0);
    starTable->addWidget(makeCell("Վատնած ≤ 10", "white"),   3, 1);

    starTable->addWidget(makeCell("⭐⭐⭐⭐",   "#ffa500"),   4, 0);
    starTable->addWidget(makeCell("Վատնած ≤ 6",  "#ffa500"), 4, 1);

    starTable->addWidget(makeCell("⭐⭐⭐⭐⭐", "#ffd700"),   5, 0);
    starTable->addWidget(makeCell("Վատնած ≤ 5",  "#ffd700"), 5, 1);

    starTableWidget->setStyleSheet("background-color: transparent;");
    layout->addWidget(starTableWidget);

    layout->addWidget(errorLabel);
    layout->addWidget(errorLog);
    layout->addStretch();

    return leftPanel;
}

QWidget* MainWindow::createCenterPanel()
{
    QGroupBox *centerPanel = new QGroupBox("Մոլորակների Քարտեզ");
    centerPanel->setStyleSheet(
        "QGroupBox { background-color: rgba(255, 255, 255, 30); "
        "border: 1px solid rgba(255, 255, 255, 64); "
        "border-radius: 12px; color: lightblue; font-size: 14pt; font-weight: bold; padding: 10px; }");

    QVBoxLayout *layout = new QVBoxLayout(centerPanel);
    QHBoxLayout *statsLayout = new QHBoxLayout();

    scanCountLabel = new QLabel("0/0");
    scanCountLabel->setStyleSheet(
        "background-color: rgba(255, 255, 255, 64); "
        "border-radius: 15px; color: lightblue; font-size: 12pt; "
        "font-weight: bold; padding: 5px 15px;");
    scanCountLabel->setAlignment(Qt::AlignCenter);

    QLabel *wastedLabel = new QLabel();
    wastedLabel->setObjectName("wastedLabel");
    wastedLabel->setStyleSheet(
        "background-color: rgba(255, 140, 66, 128); "
        "border-radius: 12px; color: lightblue; font-size: 11pt; "
        "font-weight: bold; padding: 5px 12px;");
    wastedLabel->setAlignment(Qt::AlignCenter);

    connect(engine, &SpaceExplorerEngine::wastedMovesChanged, [this, wastedLabel]() {
        wastedLabel->setText(QString(" Վատնած: %1").arg(engine->wastedMoves()));
    });
    wastedLabel->setText("Վատնած: 0");

    statsLayout->addStretch();
    statsLayout->addWidget(wastedLabel);
    statsLayout->addWidget(scanCountLabel);
    layout->addLayout(statsLayout);

    gridWidget = new GridWidget(engine, this);
    layout->addWidget(gridWidget, 0, Qt::AlignCenter);

    statusLabel = new QLabel("Պատրաստ");
    statusLabel->setStyleSheet("color: grey; font-size: 12pt; font-weight: bold;");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    startPauseBtn = new QPushButton("Սկսել");
    startPauseBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    startPauseBtn->setIconSize(QSize(20, 20));
    startPauseBtn->setStyleSheet(
        "QPushButton { background-color: #10b981; color: darkblue; "
        "font-size: 11pt; padding: 10px 20px; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #6EE7B7; }");
    connect(startPauseBtn, &QPushButton::clicked, this, &MainWindow::onStartPause);

    QPushButton *stepBtn = new QPushButton("Քայլ առ քայլ");
    stepBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaSkipForward));
    stepBtn->setIconSize(QSize(20, 20));
    stepBtn->setStyleSheet(
        "QPushButton { background-color: #f59e0b; color: darkblue; "
        "font-size: 11pt; padding: 8px 15px; border-radius: 8px; font-weight: bold;}"
        "QPushButton:hover { background-color: #FCD34D; }");
    connect(stepBtn, &QPushButton::clicked, this, &MainWindow::onStepByStep);

    resetBtn = new QPushButton("Վերսկսել");
    resetBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    resetBtn->setIconSize(QSize(20, 20));
    resetBtn->setStyleSheet(
        "QPushButton { background-color: #ef4444; color: darkblue; "
        "font-size: 11pt; padding: 8px 15px; border-radius: 8px; font-weight: bold;}"
        "QPushButton:hover { background-color: #FECACA; }");
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onReset);

    btnLayout->addStretch();
    btnLayout->addWidget(stepBtn);
    btnLayout->addWidget(startPauseBtn);
    btnLayout->addWidget(resetBtn);
    btnLayout->addStretch();

    layout->addLayout(btnLayout);
    return centerPanel;
}


QWidget* MainWindow::createRightPanel()
{
    QGroupBox *rightPanel = new QGroupBox("Հրամանների կառավարում");
    rightPanel->setStyleSheet(
        "QGroupBox { background-color: rgba(255, 255, 255, 30); "
        "border: 1px solid rgba(255, 255, 255, 64); "
        "border-radius: 9px; color: lightblue; font-size: 11pt; font-weight: bold; padding: 7px; }"
        "QLabel { color: white; font-size: 11pt; }");

    QVBoxLayout *layout = new QVBoxLayout(rightPanel);

    QLabel *selectorLabel = new QLabel("Ընտրել տիեզերագնաց:");
    explorerSelector = new QComboBox();
    explorerSelector->addItem("Տիեզերագնաց #1 🔴");
    explorerSelector->addItem("Տիեզերագնաց #2 🟢");
    explorerSelector->addItem("Տիեզերագնաց #3 🩷");
    explorerSelector->setStyleSheet(
        "QComboBox { background-color: rgba(255, 255, 255, 64); color: lightgrey; "
        "border: 1px solid rgba(255, 255, 255, 96); border-radius: px; padding: 4px; }");
    connect(explorerSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onExplorerChanged);

    QLabel *availLabel = new QLabel("Հասանելի հրամաններ:");
    QLabel *commands = new QLabel(
        "• MOVE UP/DOWN/RIGHT/LEFT - Շարժում\n"
        "• SCAN - Սկանավորել սեկտորը\n"
        "• WAIT - Սպասել\n"
        "• STOP - Ավարտել");
    commands->setStyleSheet(
        "background-color: rgb(120, 180, 255); "
        "border-radius: 8px; padding: 10px; font-size: 10pt;");

    QLabel *inputLabel = new QLabel("Մուտքագրել հրամաններ:");
    commandInput = new QTextEdit();
    commandInput->setPlaceholderText("SCAN\nMOVE RIGHT\nSCAN\nMOVE RIGHT\n...");
    commandInput->setMaximumHeight(120);
    commandInput->setStyleSheet(
        "QTextEdit { background-color: rgba(255, 255, 255, 64); color: lightblue; "
        "border: 1px solid rgba(255, 255, 255, 96); border-radius: 8px; "
        "padding: 5px; font-family: Courier; font-size: 10pt; }");

    QHBoxLayout *cmdBtnLayout = new QHBoxLayout();

    QPushButton *addBtn = new QPushButton("Ավելացնել հրամաններ");
    addBtn->setStyleSheet(
        "QPushButton { background-color: #3b82f6; color: lightblue; "
        "font-size: 11pt; padding: 8px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #2563eb; }");
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddCommands);

    QPushButton *clearBtn = new QPushButton("Մաքրել");
    clearBtn->setStyleSheet(
        "QPushButton { background-color: #ef4444; color: lightblue; "
        "font-size: 10pt; padding: 8px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #dc2626; }");
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearCommands);

    cmdBtnLayout->addWidget(addBtn, 3);
    cmdBtnLayout->addWidget(clearBtn, 1);

    QLabel *currentLabel = new QLabel("Ընթացիկ հրամաններ:");
    currentCommands = new QTextEdit();
    currentCommands->setReadOnly(true);
    currentCommands->setStyleSheet(
        "QTextEdit { background-color: rgba(255, 255, 255, 64); color: white; "
        "border: 1px solid rgba(255, 255, 255, 96); border-radius: 8px; "
        "padding: 5px; font-family: Courier; font-size: 10pt; }");

    layout->addWidget(selectorLabel);
    layout->addWidget(explorerSelector);
    layout->addWidget(availLabel);
    layout->addWidget(commands);
    layout->addWidget(inputLabel);
    layout->addWidget(commandInput);
    layout->addLayout(cmdBtnLayout);
    layout->addWidget(currentLabel);
    layout->addWidget(currentCommands);
    layout->addStretch();

    return rightPanel;
}

void MainWindow::onStartPause()
{
    if (engine->isRunning()) {
        engine->pauseMission();
    } else {
        engine->startMission();
    }
}

void MainWindow::onReset()
{
    engine->resetMission();
    commandInput->clear();
    currentCommands->clear();
    errorLog->clear();
    onExplorerChanged(explorerSelector->currentIndex());
    gridWidget->update();
}

void MainWindow::onStepByStep()
{
    if (!engine->isRunning()) {
        engine->startMission();
        QTimer::singleShot(100, [this]() {
            engine->pauseMission();
        });
    }
}

void MainWindow::onAddCommands()
{
    QString text = commandInput->toPlainText();
    QStringList commands = text.split('\n', Qt::SkipEmptyParts);

    if (commands.isEmpty()) {
        return;
    }

    int explorerId = explorerSelector->currentIndex() + 1;
    engine->addCommands(explorerId, commands);
    onExplorerChanged(explorerSelector->currentIndex());
    commandInput->clear();
}

void MainWindow::onClearCommands()
{
    int explorerId = explorerSelector->currentIndex() + 1;
    engine->clearCommands(explorerId);
    onExplorerChanged(explorerSelector->currentIndex());
}

void MainWindow::onExplorerChanged(int index)
{
    int explorerId = index + 1;
    QStringList allCommands = engine->getCommands(explorerId);
    QString display;
    for (int i = 0; i < allCommands.size(); i++) {
        display += QString("%1. %2\n").arg(i + 1).arg(allCommands[i]);
    }
    currentCommands->setText(display.isEmpty() ? "Հրամաններ չկան" : display);
}

void MainWindow::updateVisualization()
{
    gridWidget->update();
}

void MainWindow::onSpeedChanged(int value)
{
    speedLabel->setText(QString("%1ms").arg(value));
    engine->setSpeed(value);
}

void MainWindow::onErrorOccurred(const QString &error)
{
    QTime currentTime = QTime::currentTime();
    QString timeStr = currentTime.toString("hh:mm:ss");
    errorLog->addItem(QString("[%1] %2").arg(timeStr).arg(error));
    errorLog->scrollToBottom();
}

// GridWidget Implementation
GridWidget::GridWidget(SpaceExplorerEngine *engine, QWidget *parent)
    : QWidget(parent), m_engine(engine), cellSize(60)
{
    setMinimumSize(cellSize * 8, cellSize * 8);
    setMaximumSize(cellSize * 8, cellSize * 8);

    planets.append({2, 2, 30, QColor(255, 140, 66)});
    planets.append({5, 5, 25, QColor(155, 89, 182)});
    planets.append({6, 1, 20, QColor(52, 152, 219)});
}

void GridWidget::updateGrid()
{
    update();
}

void GridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int gridSize = m_engine->gridSize();

    // Background
    painter.fillRect(rect(), QColor(20, 20, 46));

    // Draw stars
    painter.setPen(Qt::NoPen);
    for (int i = 0; i < 50; i++) {
        int x = (i * 73) % width();
        int y = (i * 59) % height();
        int r = (i % 3) + 1;
        painter.setBrush(QColor(255, 255, 255, 100));
        painter.drawEllipse(x, y, r, r);
    }

    // Draw grid
    painter.setPen(QPen(QColor(255, 255, 255, 50), 1));
    for (int i = 0; i <= gridSize; i++) {
        painter.drawLine(i * cellSize, 0, i * cellSize, gridSize * cellSize);
        painter.drawLine(0, i * cellSize, gridSize * cellSize, i * cellSize);
    }

    // Planet territory overlay removed intentionally — all sectors look the same,
    // explorers are not tied to planet colors.

    // Draw planet territories
    QVariantList territories = m_engine->planetTerritories();
    QList<QColor> planetColors = {
        QColor(255, 140, 66, 60),
        QColor(155, 89, 182, 60),
        QColor(52, 152, 219, 60)
    };

    for (const auto &terr : territories) {
        QVariantMap territory = terr.toMap();
        int x = territory["x"].toInt();
        int y = territory["y"].toInt();
        int planetId = territory["planetId"].toInt();

        if (planetId >= 0 && planetId < planetColors.size()) {
            painter.setBrush(planetColors[planetId]);
            painter.setPen(Qt::NoPen);
            painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
        }
    }

    // Draw visited but not scanned cells (wasted moves)
    QVariantList visited = m_engine->visitedCells();
    QVariantList scanned = m_engine->scannedCells();
    QSet<QString> scannedSet;
    for (const auto &cell : scanned) {
        QVariantMap cellMap = cell.toMap();
        scannedSet.insert(QString("%1,%2").arg(cellMap["x"].toInt()).arg(cellMap["y"].toInt()));
    }

    painter.setBrush(QColor(255, 100, 100, 80));
    for (const auto &cell : visited) {
        QVariantMap cellMap = cell.toMap();
        int x = cellMap["x"].toInt();
        int y = cellMap["y"].toInt();
        QString key = QString("%1,%2").arg(x).arg(y);

        if (!scannedSet.contains(key)) {
            painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
        }
    }

    // Draw planets with realistic appearance
    for (const auto &planet : planets) {
        int px = planet.x * cellSize + cellSize / 2;
        int py = planet.y * cellSize + cellSize / 2;
        QColor col = planet.color;

        // Glow
        QRadialGradient gradient(px, py, planet.size * 1.2);
        col.setAlpha(40);
        gradient.setColorAt(0, col);
        col.setAlpha(0);
        gradient.setColorAt(1, col);
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(px, py), static_cast<int>(planet.size * 1.2), static_cast<int>(planet.size * 1.2));

        // Planet body
        QRadialGradient planetGradient(px - planet.size * 0.3, py - planet.size * 0.3, planet.size * 1.5);
        col = planet.color;
        col.setAlpha(200);
        planetGradient.setColorAt(0, col.lighter(150));
        col.setAlpha(220);
        planetGradient.setColorAt(0.6, col);
        col.setAlpha(180);
        planetGradient.setColorAt(1, col.darker(120));

        painter.setBrush(planetGradient);
        painter.setPen(QPen(col.darker(150), 1));
        painter.drawEllipse(QPoint(px, py), static_cast<int>(planet.size), static_cast<int>(planet.size));

        // Craters
        painter.setPen(Qt::NoPen);
        QColor craterColor = col.darker(130);
        craterColor.setAlpha(80);
        painter.setBrush(craterColor);
        painter.drawEllipse(QPoint(px + planet.size * 0.3, py - planet.size * 0.2), static_cast<int>(planet.size * 0.2), static_cast<int>(planet.size * 0.2));
        painter.drawEllipse(QPoint(px - planet.size * 0.4, py + planet.size * 0.3), static_cast<int>(planet.size * 0.15), static_cast<int>(planet.size * 0.15));

        // Highlight
        QRadialGradient highlight(px - planet.size * 0.4, py - planet.size * 0.4, planet.size * 0.5);
        highlight.setColorAt(0, QColor(255, 255, 255, 100));
        highlight.setColorAt(1, QColor(255, 255, 255, 0));
        painter.setBrush(highlight);
        painter.drawEllipse(QPoint(px - planet.size * 0.3, py - planet.size * 0.3), static_cast<int>(planet.size * 0.4), static_cast<int>(planet.size * 0.4));
    }

    // Draw scanned cells
    painter.setBrush(QColor(74, 222, 128, 120));
    for (const auto &cell : scanned) {
        QVariantMap cellMap = cell.toMap();
        int x = cellMap["x"].toInt();
        int y = cellMap["y"].toInt();
        painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
    }

    // Draw paths
    QVariantList explorers = m_engine->explorers();
    for (const auto &exp : explorers) {
        QVariantMap explorer = exp.toMap();
        int id = explorer["id"].toInt();
        QColor color(explorer["color"].toString());
        color.setAlpha(100);
        painter.setBrush(color);

        QVariantList path = m_engine->getPathHistory(id);
        for (const auto &p : path) {
            QVariantMap point = p.toMap();
            int px = point["x"].toInt() * cellSize + cellSize / 2;
            int py = point["y"].toInt() * cellSize + cellSize / 2;
            painter.drawEllipse(QPoint(px, py), 3, 3);
        }
    }

    // Draw footprints
    painter.setPen(QPen(QColor(255, 255, 255, 100), 1));
    for (const auto &cell : visited) {
        QVariantMap cellMap = cell.toMap();
        int x = cellMap["x"].toInt();
        int y = cellMap["y"].toInt();

        int fx = x * cellSize + cellSize / 2;
        int fy = y * cellSize + cellSize / 2;

        painter.setBrush(QColor(200, 200, 200, 80));
        painter.drawEllipse(QPoint(fx - 5, fy), 4, 6);
        painter.drawEllipse(QPoint(fx + 5, fy), 4, 6);
    }

    // Draw astronauts
    for (const auto &exp : explorers) {
        QVariantMap explorer = exp.toMap();
        int x = explorer["x"].toInt();
        int y = explorer["y"].toInt();
        int id = explorer["id"].toInt();
        QColor color(explorer["color"].toString());

        int cx = x * cellSize + cellSize / 2;
        int cy = y * cellSize + cellSize / 2;

        // Helmet
        color.setAlpha(220);
        painter.setBrush(color);
        painter.setPen(QPen(QColor(255, 255, 255, 150), 2));
        painter.drawEllipse(QPoint(cx, cy - 5), 16, 16);

        // Visor
        painter.setBrush(QColor(100, 150, 200, 180));
        painter.drawEllipse(QPoint(cx, cy - 5), 10, 10);

        // Body
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(cx - 10, cy + 8, 20, 15, 3, 3);

        // Arms
        painter.drawEllipse(QPoint(cx - 14, cy + 12), 4, 8);
        painter.drawEllipse(QPoint(cx + 14, cy + 12), 4, 8);

        // ID badge
        painter.setBrush(QColor(255, 255, 255, 200));
        painter.drawEllipse(QPoint(cx, cy + 14), 6, 6);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        painter.drawText(QRect(cx - 6, cy + 8, 12, 12), Qt::AlignCenter, QString::number(id));
    }
}

void MainWindow::showStarRatingDialog()
{
    int wasted = engine->wastedMoves();
    int scanned = engine->scannedCount();
    int total = engine->totalTerritories();
    int scanPct = (total > 0) ? (scanned * 100 / total) : 0;
    bool fullScan = (scanned == total);

    // 1 star = minimum: all territories scanned (prerequisite)
    // 2-5 stars = based purely on wasted moves (moves outside planet territories)
    bool c1 = fullScan;              // ⭐1: բոլոր սեկտորները սկանավորված
    bool c2 = (c1 && wasted <= 18); // ⭐2
    bool c3 = (c1 && wasted <= 10); // ⭐3
    bool c4 = (c1 && wasted <= 6);  // ⭐4
    bool c5 = (c1 && wasted <= 5);  // ⭐5: գրեթե կատարյալ

    int stars = 0;
    if (c1) stars = 1;
    if (c2) stars = 2;
    if (c3) stars = 3;
    if (c4) stars = 4;
    if (c5) stars = 5;

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Առաքելության արդյունք");
    dialog->setFixedSize(400, 320);
    dialog->setStyleSheet(R"(
        QDialog { background-color: #0d1b4b; }
        QLabel  { color: #eaeaea; background-color: transparent; }
        QPushButton {
            border: none; border-radius: 8px;
            font-size: 11pt; font-weight: bold; padding: 10px 22px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(10);
    layout->setContentsMargins(30, 22, 30, 22);

    // Title
    QString titleText;
    switch (stars) {
    case 5: titleText = "🏆 ԿԱՏԱՐՅԱԼ ԱՌԱՔԵԼՈՒԹՅՈՒՆ!"; break;
    case 4: titleText = " ԳԵՐԱԶԱՆՑ!";               break;
    case 3: titleText = " ԼԱՎ ԱՐԴՅՈՒՆՔ";            break;
    case 2: titleText = " ԲԱՎԱՐԱՐ";                  break;
    case 1: titleText = " ԱՎԱՐՏՎԱԾ";                  break;
    default: titleText = "Բոլոր սեկտորները չեն սկանավորված"; break;
    }
    QLabel *titleLabel = new QLabel(titleText);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #ffd700;");
    layout->addWidget(titleLabel);

    // Stars
    QString starsHtml;
    for (int i = 0; i < 5; i++)
        starsHtml += (i < stars)
                         ? "<span style='color:#ffd700; font-size:28pt;'>★</span>"
                         : "<span style='color:#333;   font-size:28pt;'>★</span>";
    QLabel *starsLabel = new QLabel(starsHtml);
    starsLabel->setAlignment(Qt::AlignCenter);
    starsLabel->setTextFormat(Qt::RichText);
    layout->addWidget(starsLabel);

    QLabel *scoreLabel = new QLabel(QString("%1 / 5 աստղ").arg(stars));
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 10pt; color: #aaa;");
    layout->addWidget(scoreLabel);

    // Stats
    QLabel *statsLabel = new QLabel(
        QString("Վատնած քայլեր: <b style='color:#ff8c42;'>%1</b>"
                "&nbsp;&nbsp;|&nbsp;&nbsp;"
                "Սկանավորված: <b style='color:#4ade80;'>%2/%3</b>")
            .arg(wasted).arg(scanned).arg(total));
    statsLabel->setAlignment(Qt::AlignCenter);
    statsLabel->setTextFormat(Qt::RichText);
    statsLabel->setStyleSheet("font-size: 11pt; margin-bottom: 4px;");
    layout->addWidget(statsLabel);

    layout->addStretch();

    bool shouldRetry = false;

    if (stars < 5) {
        QLabel *retryLabel = new QLabel("Դուր եկա՞վ ձեր ստացած արդյունքը,\nթե՞ ուզում եք կրկին փորձել։");
        retryLabel->setAlignment(Qt::AlignCenter);
        retryLabel->setStyleSheet("font-size: 11pt; color: #eaeaea;");
        layout->addWidget(retryLabel);

        QHBoxLayout *btnRow = new QHBoxLayout();
        btnRow->setSpacing(12);

        QPushButton *closeBtn = new QPushButton("Այո, փակել");
        closeBtn->setStyleSheet(
            "QPushButton { background-color: #1e3a8a; color: #eaeaea; "
            "border: 2px solid #2563eb; border-radius: 8px; "
            "font-size: 11pt; font-weight: bold; padding: 10px 20px; }"
            "QPushButton:hover { background-color: #2563eb; }");

        QPushButton *retryBtn = new QPushButton("Կրկին փորձել");
        retryBtn->setStyleSheet(
            "QPushButton { background-color: #10b981; color: #0d1b4b; "
            "border-radius: 8px; font-size: 11pt; font-weight: bold; padding: 10px 20px; }"
            "QPushButton:hover { background-color: #6ee7b7; }");

        btnRow->addStretch();
        btnRow->addWidget(closeBtn);
        btnRow->addWidget(retryBtn);
        btnRow->addStretch();
        layout->addLayout(btnRow);

        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::reject);
        connect(retryBtn, &QPushButton::clicked, [&]() {
            shouldRetry = true;
            dialog->accept();
        });
    } else {
        QPushButton *closeBtn = new QPushButton(" Փակել");
        closeBtn->setStyleSheet(
            "QPushButton { background-color: #ffd700; color: #0d1b4b; "
            "border-radius: 8px; font-size: 12pt; font-weight: bold; padding: 10px 30px; }"
            "QPushButton:hover { background-color: #fbbf24; }");
        QHBoxLayout *btnRow = new QHBoxLayout();
        btnRow->addStretch();
        btnRow->addWidget(closeBtn);
        btnRow->addStretch();
        layout->addLayout(btnRow);
        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    }

    // Պահպանել արդյունքը shared DB-ում
    if (stars >= 1) ResultSaver::save("space", stars);

    dialog->exec();
    delete dialog;

    if (shouldRetry) {
        engine->resetMission();
        commandInput->clear();
        currentCommands->clear();
        errorLog->clear();
        onExplorerChanged(explorerSelector->currentIndex());
        gridWidget->update();
    } else {
        QApplication::quit();
    }
}
