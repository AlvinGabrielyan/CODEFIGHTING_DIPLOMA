#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSplitter>
#include <QPainter>
#include <QFont>
#include <QPainterPath>
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Ստեղծել համակարգը
    beeSystem = new BeeSystem(this);
    beeSystem->initialize();

    interpreter = new Interpreter(this);

    setupUI();
    createMenuBar();

    // Կապել սիգնալները
    connect(beeSystem, &BeeSystem::systemUpdated, this, &MainWindow::onSystemUpdated);
    connect(beeSystem, &BeeSystem::messageAdded, this, &MainWindow::onMessageAdded);

    setWindowTitle("Մեղուների Համակարգ");
    resize(1400, 900);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Ձախ վահանակ
    QWidget* leftPanel = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);

    createDescriptionPanel();
    createRulesPanel();
    createCommandPanel();
    createControlPanel();

    leftLayout->addWidget(descriptionText);
    leftLayout->addWidget(rulesText);
    leftLayout->addWidget(new QLabel("<b>Հրամանների կատարում:</b>"));
    leftLayout->addWidget(commandInput);
    leftLayout->addWidget(executeButton);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(resetButton);
    controlLayout->addWidget(stepButton);
    controlLayout->addWidget(helpButton);
    leftLayout->addLayout(controlLayout);

    leftPanel->setMaximumWidth(400);

    // Աջ վահանակ
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    createVisualizationPanel();
    createMessagePanel();

    rightLayout->addWidget(new QLabel("<h2>Վիզուալ Արտապատկերում</h2>"));
    rightLayout->addWidget(visualWidget, 3);
    rightLayout->addWidget(new QLabel("<b>Համակարգի Հաղորդագրություններ:</b>"));
    rightLayout->addWidget(messageLog, 1);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel);
}

void MainWindow::createMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* fileMenu = menuBar->addMenu("Ֆայլ");
    QAction* exitAction = fileMenu->addAction("Ելք");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu* helpMenu = menuBar->addMenu("Օգնություն");
    QAction* aboutAction = helpMenu->addAction("Մասին");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "Մասին",
                           "Մեղուների Համակարգի Սիմուլյատոր\n\n"
                           "Համակարգը սիմուլացնում է մեղուների\n"
                           "նեկտարի հավաքման և ամաններում\n"
                           "պահպանման գործընթացը։\n\n"
                           "Հրամաններ:  \n"
                           "BEES, COMBS, FLOWERS, HELP");
    });
}

void MainWindow::createDescriptionPanel() {
    descriptionText = new QTextEdit();
    descriptionText->setReadOnly(true);
    descriptionText->setMaximumHeight(150);
    descriptionText->setHtml(
        "<h3>Խնդրի Նկարագրություն</h3>"
        "<p>Համակարգում կան <b>3 մեղու</b> (Մայա, Վիլի, Զիթա), "
        "որոնք հավաքում են նեկտար 3 ծաղիկներից։</p>"
        "<p>Յուրաքանչյուր մեղու ունի իր <b>տարողությունը</b> (2-3գ), "
        "իսկ ամանները ունեն <b>5 գրամ տարողություն</b>։</p>"
        "<p><b>Հրամաններ:</b> START, STOP, RESET, STATUS, BEES, COMBS, FLOWERS, HELP</p>"
        );
}

void MainWindow::createRulesPanel() {
    rulesText = new QTextEdit();
    rulesText->setReadOnly(true);
    rulesText->setMaximumHeight(180);
    rulesText->setHtml(
        "<h3>Կանոններ</h3>"
        "<ul>"
        "<li><b>Մայա</b> - տարողություն 3գ (Ոսկեգույն)</li>"
        "<li><b>Վիլի</b> - տարողություն 2.5գ (Նարնջագույն)</li>"
        "<li><b>Զիթա</b> - տարողություն 2գ (Վարդագույն)</li>"
        "<li>5 աման՝ յուրաքանչյուրը 5գ տարողությամբ</li>"
        "<li>3 ծաղիկ՝ Վարդ (8գ), Մանուշակ (6գ), Ծիածան (7գ)</li>"
        "<li>Ամանը փակվում է լիքը լինելուց հետո</li>"
        "</ul>"
        );
}

void MainWindow::createCommandPanel() {
    commandInput = new QLineEdit();
    commandInput->setPlaceholderText("Մուտքագրեք հրաման (օր.՝ STATUS, BEES, HELP)");

    executeButton = new QPushButton("Կատարել");
    executeButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 8px;");

    connect(commandInput, &QLineEdit::returnPressed, this, &MainWindow::onExecuteCommand);
    connect(executeButton, &QPushButton::clicked, this, &MainWindow::onExecuteCommand);
}

void MainWindow::createControlPanel() {
    startButton = new QPushButton("▶ Սկսել");
    startButton->setStyleSheet("background-color: #2196F3; color: white; padding: 6px;");
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStart);

    stopButton = new QPushButton("⏸ Դադարեցնել");
    stopButton->setStyleSheet("background-color: #FF9800; color: white; padding: 6px;");
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::onStop);

    resetButton = new QPushButton("⟲ Վերակայել");
    resetButton->setStyleSheet("background-color: #9C27B0; color: white; padding: 6px;");
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);

    stepButton = new QPushButton("⏭ Քայլ");
    stepButton->setStyleSheet("background-color: #607D8B; color: white; padding: 6px;");
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::onStep);

    helpButton = new QPushButton("❓ Օգնություն");
    helpButton->setStyleSheet("background-color: #795548; color: white; padding: 6px;");
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::onHelp);
}

void MainWindow::createVisualizationPanel() {
    visualWidget = new VisualizationWidget(beeSystem, this);
    visualWidget->setMinimumSize(800, 500);
}

void MainWindow::createMessagePanel() {
    messageLog = new QTextEdit();
    messageLog->setReadOnly(true);
    messageLog->setMaximumHeight(200);
}

void MainWindow::onExecuteCommand() {
    QString command = commandInput->text().trimmed().toUpper();
    if (command.isEmpty()) return;

    bool success = true;
    QString response;

    // Հիմնական հրամաններ
    if (command == "START") {
        onStart();
        response = "Սիմուլյացիան սկսվեց";
    }
    else if (command == "STOP") {
        onStop();
        response = "Սիմուլյացիան դադարեցվեց";
    }
    else if (command == "RESET") {
        onReset();
        response = "Համակարգը վերակայվեց";
    }
    else if (command == "HELP") {
        onHelp();
        response = "Օգնության պատուհանը բացվեց";
    }

    // Տեղեկատվական հրամաններ
    else if (command == "STATUS") {
        response = getSystemStatus();
    }
    else if (command == "BEES") {
        response = getBeesStatus();
    }
    else if (command == "COMBS" || command == "HONEYCOMBS") {
        response = getCombsStatus();
    }
    else if (command == "FLOWERS") {
        response = getFlowersStatus();
    }

    // Մեղուների տեղեկատվություն
    else if (command == "BEE1" || command == "ՄԱՅԱ") {
        response = getBeeInfo(0);
    }
    else if (command == "BEE2" || command == "ՎԻԼԻ") {
        response = getBeeInfo(1);
    }
    else if (command == "BEE3" || command == "ԶԻԹԱ") {
        response = getBeeInfo(2);
    }

    // Ամանների տեղեկատվություն
    else if (command.startsWith("COMB") && command.length() == 5) {
        int combNum = command.mid(4, 1).toInt() - 1;
        if (combNum >= 0 && combNum < 5) {
            response = getCombInfo(combNum);
        } else {
            success = false;
            response = "Սխալ ամանի համար։ Օգտագործեք COMB1-COMB5";
        }
    }

    // Ծաղիկների տեղեկատվություն
    else if (command.startsWith("FLOWER") && command.length() == 7) {
        int flowerNum = command.mid(6, 1).toInt() - 1;
        if (flowerNum >= 0 && flowerNum < 3) {
            response = getFlowerInfo(flowerNum);
        } else {
            success = false;
            response = "Սխալ ծաղկի համար։ Օգտագործեք FLOWER1-FLOWER3";
        }
    }

    // Անհայտ հրաման
    else {
        success = false;
        response = "Անհայտ հրաման։ Մուտքագրեք HELP՝ օգնության համար";
    }

    // Ցույց տալ արդյունքը
    if (success) {
        messageLog->append("<font color='green'>✓ " + command + "</font>");
        if (!response.isEmpty()) {
            messageLog->append(response);
        }
    } else {
        messageLog->append("<font color='red'>✗ " + command + "</font>");
        messageLog->append("<font color='red'>  " + response + "</font>");
    }

    commandInput->clear();
    visualWidget->update();
}

QString MainWindow::getSystemStatus() {
    QString status = "<b>📊 Համակարգի Վիճակ:</b>";

    // Մեղուներ
    status += "<br><b>Մեղուներ:</b>";
    for (const Bee* bee : beeSystem->getBees()) {
        status += QString("<br>  • %1: %2գ/%3գ, Էներգիա %4%")
                      .arg(bee->name())
                      .arg(bee->currentNectar(), 0, 'f', 1)
                      .arg(bee->capacity(), 0, 'f', 1)
                      .arg(bee->energy());
    }

    // Ամաններ
    status += "<br><b>Ամաններ:</b>";
    int sealedCount = 0;
    for (const Honeycomb* comb : beeSystem->getHoneycombs()) {
        if (comb->isSealed()) sealedCount++;
        QString sealStatus = comb->isSealed() ? "🔒" : "🔓";
        status += QString("<br>  • Աման %1: %2/%3գ (%4%) %5")
                      .arg(comb->getId())
                      .arg(comb->getCurrentAmount(), 0, 'f', 1)
                      .arg(comb->getCapacity(), 0, 'f', 1)
                      .arg((int)comb->getFillPercentage())
                      .arg(sealStatus);
    }

    // Ծաղիկներ
    status += "<br><b>Ծաղիկներ:</b>";
    for (const Flower* flower : beeSystem->getFlowers()) {
        status += QString("<br>  • %1: %2/%3գ")
                      .arg(flower->getType())
                      .arg(flower->getNectarAmount(), 0, 'f', 1)
                      .arg(flower->getMaxNectar(), 0, 'f', 1);
    }

    return status;
}

QString MainWindow::getBeesStatus() {
    QString status = "<b>🐝 Մեղուներ:</b>";
    for (const Bee* bee : beeSystem->getBees()) {
        status += QString("<br><b>%1 (%2):</b>")
        .arg(bee->name())
            .arg(bee->id());
        status += QString("<br>  • Տարողություն: %1գ")
                      .arg(bee->capacity(), 0, 'f', 1);
        status += QString("<br>  • Ընթացիկ նեկտար: %1գ")
                      .arg(bee->currentNectar(), 0, 'f', 1);
        status += QString("<br>  • Էներգիա: %1%")
                      .arg(bee->energy());
        status += QString("<br>  • Դիրք: %1")
                      .arg(bee->location());
    }
    return status;
}

QString MainWindow::getCombsStatus() {
    QString status = "<b>🍯 Ամաններ:</b>";
    for (const Honeycomb* comb : beeSystem->getHoneycombs()) {
        QString sealStatus = comb->isSealed() ? "Փակված 🔒" : "Բաց 🔓";
        status += QString("<br><b>Աման %1:</b>")
                      .arg(comb->getId());
        status += QString("<br>  • Տարողություն: %1գ")
                      .arg(comb->getCapacity(), 0, 'f', 1);
        status += QString("<br>  • Լցված: %1գ (%2%)")
                      .arg(comb->getCurrentAmount(), 0, 'f', 1)
                      .arg((int)comb->getFillPercentage());
        status += QString("<br>  • Վիճակ: %1")
                      .arg(sealStatus);
    }
    return status;
}

QString MainWindow::getFlowersStatus() {
    QString status = "<b>🌸 Ծաղիկներ:</b>";
    for (const Flower* flower : beeSystem->getFlowers()) {
        status += QString("<br><b>%1:</b>")
        .arg(flower->getType());
        status += QString("<br>  • Նեկտար: %1/%2գ")
                      .arg(flower->getNectarAmount(), 0, 'f', 1)
                      .arg(flower->getMaxNectar(), 0, 'f', 1);
        status += QString("<br>  • Վերականգնման արագություն: %1գ/վրկ")
                      .arg(flower->getRegenerationRate(), 0, 'f', 1);
    }
    return status;
}

QString MainWindow::getBeeInfo(int index) {
    if (index < 0 || index >= beeSystem->getBees().size()) {
        return "Սխալ մեղվի ինդեքս";
    }

    const Bee* bee = beeSystem->getBees()[index];
    QString info = QString("<b>🐝 %1 - Մանրամասն Տեղեկություն:</b>").arg(bee->name());
    info += QString("<br>• ID: %1").arg(bee->id());
    info += QString("<br>• Տարողություն: %1գ").arg(bee->capacity(), 0, 'f', 1);
    info += QString("<br>• Ընթացիկ նեկտար: %1գ").arg(bee->currentNectar(), 0, 'f', 1);
    info += QString("<br>• Էներգիա: %1%").arg(bee->energy());
    info += QString("<br>• Դիրք: %1").arg(bee->location());
    info += QString("<br>• Թռչու՞մ է: %1").arg(bee->isFlying() ? "Այո ✈️" : "Ոչ");

    if (!bee->isEmpty()) {
        info += QString("<br>• Նեկտարի տեսակ: %1").arg(bee->nectarType());
    }

    return info;
}

QString MainWindow::getCombInfo(int index) {
    if (index < 0 || index >= beeSystem->getHoneycombs().size()) {
        return "Սխալ ամանի ինդեքս";
    }

    const Honeycomb* comb = beeSystem->getHoneycombs()[index];
    QString info = QString("<b>🍯 Աման %1 - Մանրամասն Տեղեկություն:</b>").arg(comb->getId());
    info += QString("<br>• Տարողություն: %1գ").arg(comb->getCapacity(), 0, 'f', 1);
    info += QString("<br>• Լցված: %1գ").arg(comb->getCurrentAmount(), 0, 'f', 1);
    info += QString("<br>• Լցվածության տոկոս: %1%").arg((int)comb->getFillPercentage());
    info += QString("<br>• Ազատ տեղ: %1գ").arg(comb->getAvailableSpace(), 0, 'f', 1);
    info += QString("<br>• Վիճակ: %1").arg(comb->isSealed() ? "Փակված 🔒" : "Բաց 🔓");

    return info;
}

QString MainWindow::getFlowerInfo(int index) {
    if (index < 0 || index >= beeSystem->getFlowers().size()) {
        return "Սխալ ծաղկի ինդեքս";
    }

    const Flower* flower = beeSystem->getFlowers()[index];
    QString info = QString("<b>🌸 %1 - Մանրամասն Տեղեկություն:</b>").arg(flower->getType());
    info += QString("<br>• Առավելագույն նեկտար: %1գ").arg(flower->getMaxNectar(), 0, 'f', 1);
    info += QString("<br>• Ընթացիկ նեկտար: %1գ").arg(flower->getNectarAmount(), 0, 'f', 1);
    info += QString("<br>• Մնացած տոկոս: %1%").arg((int)((flower->getNectarAmount() / flower->getMaxNectar()) * 100));
    info += QString("<br>• Վերականգնման արագություն: %1գ/վրկ").arg(flower->getRegenerationRate(), 0, 'f', 1);

    return info;
}

void MainWindow::onStart() {
    beeSystem->start();
    messageLog->append("<font color='blue'>▶ Համակարգը սկսվեց</font>");
}

void MainWindow::onStop() {
    beeSystem->stop();
    messageLog->append("<font color='orange'>⏸ Համակարգը դադարեցվեց</font>");
}

void MainWindow::onReset() {
    beeSystem->reset();
    messageLog->clear();
    messageLog->append("<font color='purple'>⟲ Համակարգը վերակայվեց</font>");
    messageLog->append("<font color='gray'>Մուտքագրեք STATUS՝ տեսնելու սկզբնական վիճակը</font>");
    visualWidget->update();
}

void MainWindow::onStep() {
    beeSystem->step();
    visualWidget->update();
}

void MainWindow::onHelp() {
    QString help =
        "📚 ՀՐԱՄԱՆՆԵՐԻ ՑԱՆԿ\n\n"

        "🎮 ՀԻՄՆԱԿԱՆ ԿԱՌԱՎԱՐՈՒՄ:\n"
        "  START - Սկսել սիմուլյացիան\n"
        "  STOP - Դադարեցնել սիմուլյացիան\n"
        "  RESET - Վերակայել համակարգը\n"
        "  HELP - Ցույց տալ այս օգնությունը\n\n"

        "📊 ՏԵՂԵԿԱՏՎՈՒԹՅՈՒՆ:\n"
        "  STATUS - Ամբողջ համակարգի վիճակ\n"
        "  BEES - Բոլոր մեղուների տեղեկություններ\n"
        "  COMBS - Բոլոր ամանների տեղեկություններ\n"
        "  FLOWERS - Բոլոր ծաղիկների տեղեկություններ\n\n"

        "🐝 ՄԵՂՈՒՆԵՐ:\n"
        "  BEE1 / ՄԱՅԱ - Մայա մեղվի տեղեկություն\n"
        "  BEE2 / ՎԻԼԻ - Վիլի մեղվի տեղեկություն\n"
        "  BEE3 / ԶԻԹԱ - Զիթա մեղվի տեղեկություն\n\n"

        "🍯 ԱՄԱՆՆԵՐ:\n"
        "  COMB1-COMB5 - Կոնկրետ ամանի տեղեկություն\n\n"

        "🌸 ԾԱՂԻԿՆԵՐ:\n"
        "  FLOWER1-FLOWER3 - Կոնկրետ ծաղկի տեղեկություն\n\n"

        "💡 ԽՈՐՀՈՒՐԴՆԵՐ:\n"
        "  • Մուտքագրեք STATUS սկզբում\n"
        "  • Օգտագործեք կոճակները մեղուների կառավարման համար\n"
        "  • Հրամանները case-insensitive են (մեծատառ/փոքրատառ)";

    QMessageBox::information(this, "Օգնություն - Հրամանների Ուղեցույց", help);
}

void MainWindow::onSystemUpdated() {
    visualWidget->update();
}

void MainWindow::onMessageAdded(const QString& message) {
    messageLog->append(message);
}

// ========== VisualizationWidget ==========

VisualizationWidget::VisualizationWidget(BeeSystem* sys, QWidget* parent)
    : QWidget(parent), system(sys) {
    setMinimumSize(800, 500);
    setStyleSheet("background-color: #FFF8E7;");
}

void VisualizationWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor("#FFF8E7"));

    QLinearGradient skyGradient(0, 0, 0, height() / 2);
    skyGradient.setColorAt(0, QColor("#E8F4F8"));
    skyGradient.setColorAt(0.5, QColor("#F0E6F6"));
    skyGradient.setColorAt(1, QColor("#FFF8DC"));
    painter.fillRect(0, 0, width(), height() / 2, skyGradient);

    QLinearGradient grassGradient(0, height() / 2, 0, height());
    grassGradient.setColorAt(0, QColor("#E8F5E9"));
    grassGradient.setColorAt(0.5, QColor("#C8E6C9"));
    grassGradient.setColorAt(1, QColor("#A5D6A7"));
    painter.fillRect(0, height() / 2, width(), height() / 2, grassGradient);

    QPen sunrayPen(QColor(255, 255, 200, 30), 2);
    painter.setPen(sunrayPen);
    for (int i = 0; i < 8; ++i) {
        int x = width() - 100;
        int y = 80;
        double angle = i * M_PI / 4;
        painter.drawLine(x, y, x + 60 * cos(angle), y + 60 * sin(angle));
    }

    QRadialGradient sunGradient(width() - 100, 80, 40);
    sunGradient.setColorAt(0, QColor("#FFF9C4"));
    sunGradient.setColorAt(0.7, QColor("#FFE082"));
    sunGradient.setColorAt(1, QColor("#FFD54F"));
    painter.setBrush(sunGradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(width() - 100, 80), 35, 35);

    painter.setBrush(QColor(255, 255, 255, 180));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(150, 60, 60, 30);
    painter.drawEllipse(170, 50, 50, 35);
    painter.drawEllipse(200, 55, 55, 30);

    painter.drawEllipse(400, 90, 70, 35);
    painter.drawEllipse(425, 80, 60, 40);
    painter.drawEllipse(460, 85, 65, 35);

    for (const Honeycomb* comb : system->getHoneycombs()) {
        drawHoneycomb(painter, comb);
    }

    for (const Flower* flower : system->getFlowers()) {
        drawFlower(painter, flower);
    }

    for (const Bee* bee : system->getBees()) {
        drawBee(painter, bee);
    }
}

void VisualizationWidget::drawBee(QPainter& painter, const Bee* bee) {
    QPointF pos(bee->positionX() * width(), bee->positionY() * height());

    painter.setBrush(bee->color());
    painter.setPen(Qt::black);

    painter.drawEllipse(pos, 8, 8);

    painter.setBrush(bee->color());
    painter.drawEllipse(pos + QPointF(12, 0), 12, 10);

    painter.setBrush(Qt::black);
    painter.drawEllipse(pos + QPointF(8, 0), 3, 10);
    painter.drawEllipse(pos + QPointF(16, 0), 3, 10);

    painter.setBrush(QColor(255, 255, 255, 150));
    painter.setPen(QPen(QColor(200, 200, 200), 1));

    QPainterPath leftWing;
    leftWing.moveTo(pos + QPointF(10, -5));
    leftWing.quadTo(pos + QPointF(0, -18), pos + QPointF(-5, -8));
    leftWing.quadTo(pos + QPointF(5, -10), pos + QPointF(10, -5));
    painter.drawPath(leftWing);

    QPainterPath rightWing;
    rightWing.moveTo(pos + QPointF(10, 5));
    rightWing.quadTo(pos + QPointF(0, 18), pos + QPointF(-5, 8));
    rightWing.quadTo(pos + QPointF(5, 10), pos + QPointF(10, 5));
    painter.drawPath(rightWing);

    painter.setBrush(Qt::black);
    painter.drawEllipse(pos + QPointF(-3, -3), 2, 2);
    painter.drawEllipse(pos + QPointF(-3, 3), 2, 2);

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);

    painter.drawText(pos + QPointF(-20, -25), bee->name());

    font.setPointSize(7);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(pos + QPointF(-30, -15),
                     QString("%1գ/%2գ").arg(bee->currentNectar(), 0, 'f', 1)
                         .arg(bee->capacity(), 0, 'f', 1));
}

void VisualizationWidget::drawHoneycomb(QPainter& painter, const Honeycomb* comb) {
    QPointF pos = comb->getPosition();

    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 3.0 * i;
        hexagon << QPointF(pos.x() + 20 * cos(angle), pos.y() + 20 * sin(angle));
    }

    double fillPct = comb->getFillPercentage();
    QColor fillColor;
    if (comb->isSealed()) {
        fillColor = QColor("#D4AF37");
    } else if (fillPct > 80) {
        fillColor = QColor("#FFE082");
    } else if (fillPct > 50) {
        fillColor = QColor("#FFECB3");
    } else if (fillPct > 20) {
        fillColor = QColor("#FFF9C4");
    } else {
        fillColor = QColor("#FFFDE7");
    }

    painter.setBrush(fillColor);
    painter.setPen(QPen(QColor("#8B4513"), 2));
    painter.drawPolygon(hexagon);

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    QString text = QString("%1\n%2%%")
                       .arg(comb->getId())
                       .arg((int)fillPct);

    QRectF textRect(pos.x() - 15, pos.y() - 10, 30, 20);
    painter.drawText(textRect, Qt::AlignCenter, text);

    if (comb->isSealed()) {
        painter.drawText(pos + QPointF(-8, 18), "🔒");
    }
}

void VisualizationWidget::drawFlower(QPainter& painter, const Flower* flower) {
    QPointF pos = flower->getPosition();

    QColor petalColor;
    if (flower->getType() == "Վարդ") {
        petalColor = QColor("#FF69B4");
    } else if (flower->getType() == "Մանուշակ") {
        petalColor = QColor("#9370DB");
    } else {
        petalColor = QColor("#FFD700");
    }

    painter.setBrush(petalColor);
    painter.setPen(QPen(petalColor.darker(120), 1));

    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 3.0 * i;
        QPointF petalPos = pos + QPointF(15 * cos(angle), 15 * sin(angle));
        painter.drawEllipse(petalPos, 10, 10);
    }

    painter.setBrush(QColor("#FFD700"));
    painter.drawEllipse(pos, 8, 8);

    painter.setPen(QPen(QColor("#228B22"), 3));
    painter.drawLine(pos, pos + QPointF(0, 40));

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    painter.drawText(pos + QPointF(-30, 60),
                     QString("%1: %2գ").arg(flower->getType())
                         .arg(flower->getNectarAmount(), 0, 'f', 1));
}
