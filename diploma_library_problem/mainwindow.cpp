// #include "mainwindow.h"
// #include "resultsaver.h"
// #include <QHeaderView>
// #include <QMessageBox>
// #include <QDebug>
// #include <QApplication>
// #include <QDialog>
// #include <QDialogButtonBox>
// #include <QGridLayout>
// #include <QCheckBox>
// #include <QScrollArea>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
// {
//     interpreter = new Interpreter();

//     setupUI();
//     applyStyles();
//     initializeState();
// }

// MainWindow::~MainWindow()
// {
//     delete interpreter;
// }

// void MainWindow::setupUI() {
//     setWindowTitle("Գրադարանավարի խնդիր");
//     resize(1400, 900);

//     QWidget *centralWidget = new QWidget(this);
//     setCentralWidget(centralWidget);

//     QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

//     QVBoxLayout *leftPanel = new QVBoxLayout();
//     leftPanel->setSpacing(10);

//     // Tab Widget
//     tabWidget = new QTabWidget();

//     // Tab 1: Խնդրի նկարագրություն
//     QWidget *descTab = new QWidget();
//     QVBoxLayout *descLayout = new QVBoxLayout(descTab);
//     problemDescriptionText = new QTextEdit();
//     problemDescriptionText->setReadOnly(true);
//     descLayout->addWidget(problemDescriptionText);
//     tabWidget->addTab(descTab, "Նկարագրություն");

//     // Tab 2: Կանոններ
//     QWidget *rulesTab = new QWidget();
//     QVBoxLayout *rulesLayout = new QVBoxLayout(rulesTab);
//     rulesText = new QTextEdit();
//     rulesText->setReadOnly(true);
//     rulesLayout->addWidget(rulesText);
//     tabWidget->addTab(rulesTab, "Կանոններ");

//     // Tab 3: Հրամաններ
//     QWidget *commandsTab = new QWidget();
//     QVBoxLayout *commandsLayout = new QVBoxLayout(commandsTab);
//     commandsText = new QTextEdit();
//     commandsText->setReadOnly(true);
//     commandsLayout->addWidget(commandsText);
//     tabWidget->addTab(commandsTab, "Հրամաններ");

//     // Tab 4: Գրքեր
//     QWidget *booksTab = new QWidget();
//     QVBoxLayout *booksLayout = new QVBoxLayout(booksTab);
//     booksTable = new QTableWidget(9, 4);
//     booksTable->setHorizontalHeaderLabels({"ID", "Սկիզբ", "Նպատակ", "Կարգավիճակ"});
//     booksTable->horizontalHeader()->setStretchLastSection(true);
//     booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     booksTable->setSelectionMode(QAbstractItemView::NoSelection);
//     booksLayout->addWidget(booksTable);
//     tabWidget->addTab(booksTab, "Գրքեր");

//     leftPanel->addWidget(tabWidget);

//     // Status Group
//     QGroupBox *statusGroup = new QGroupBox("Ընթացիկ վիճակ");
//     QGridLayout *statusLayout = new QGridLayout();
//     statusLayout->setSpacing(10);

//     QLabel *roomLabelTitle = new QLabel("Սենյակ:");
//     roomLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
//     roomLabel = new QLabel("S");
//     roomLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #e94560;");

//     QLabel *booksLabelTitle = new QLabel("Ձեռքին գրքեր:");
//     booksLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
//     booksInHandLabel = new QLabel("0 / 2");
//     booksInHandLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #00d4aa;");

//     QLabel *energyLabelTitle = new QLabel("Էներգիա:");
//     energyLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
//     energyLabel = new QLabel("0");
//     energyLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #ffa500;");

//     QLabel *statusLabelTitle = new QLabel("Վիճակ:");
//     statusLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
//     statusLabel = new QLabel("Պատրաստ է");
//     statusLabel->setStyleSheet("font-size: 12pt; color: #eaeaea;");

//     statusLayout->addWidget(roomLabelTitle, 0, 0);
//     statusLayout->addWidget(roomLabel, 0, 1);
//     statusLayout->addWidget(booksLabelTitle, 1, 0);
//     statusLayout->addWidget(booksInHandLabel, 1, 1);
//     statusLayout->addWidget(energyLabelTitle, 2, 0);
//     statusLayout->addWidget(energyLabel, 2, 1);
//     statusLayout->addWidget(statusLabelTitle, 3, 0);
//     statusLayout->addWidget(statusLabel, 3, 1);

//     statusGroup->setLayout(statusLayout);
//     leftPanel->addWidget(statusGroup);


//     QGroupBox *manualGroup = new QGroupBox("Հրամանի մուտքագրում");
//     QVBoxLayout *manualLayout = new QVBoxLayout();

//     commandInput = new QLineEdit();
//     commandInput->setPlaceholderText("Օրինակ: PICK(1), MOVE(S->A), PLACE(1)");
//     commandInput->setStyleSheet("padding: 10px; font-size: 12pt;");
//     connect(commandInput, &QLineEdit::returnPressed, this, &MainWindow::onExecuteCommandClicked);

//     executeCommandButton = new QPushButton("Կատարել հրաման");
//     executeCommandButton->setFixedHeight(45);
//     executeCommandButton->setStyleSheet("background-color: #00d4aa; font-size: 12pt;");
//     connect(executeCommandButton, &QPushButton::clicked, this, &MainWindow::onExecuteCommandClicked);

//     manualLayout->addWidget(commandInput);
//     manualLayout->addWidget(executeCommandButton);
//     manualGroup->setLayout(manualLayout);
//     leftPanel->addWidget(manualGroup);

//     QVBoxLayout *rightPanel = new QVBoxLayout();

//     resetButton = new QPushButton("Վերսկսել");
//     resetButton->setFixedHeight(50);
//     resetButton->setStyleSheet("background-color: #e94560; font-size: 13pt;");
//     connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
//     rightPanel->addWidget(resetButton);

//     QGroupBox *historyGroup = new QGroupBox("Հրամանների պատմություն");
//     QVBoxLayout *historyLayout = new QVBoxLayout();

//     commandHistoryList = new QListWidget();
//     commandHistoryList->setStyleSheet("font-family: 'Courier New'; font-size: 11pt;");

//     historyLayout->addWidget(commandHistoryList);
//     historyGroup->setLayout(historyLayout);
//     rightPanel->addWidget(historyGroup);

//     QGroupBox *outputGroup = new QGroupBox("Հաղորդագրություններ");
//     QVBoxLayout *outputLayout = new QVBoxLayout();
//     outputText = new QTextEdit();
//     outputText->setReadOnly(true);
//     outputText->setMaximumHeight(250);
//     outputLayout->addWidget(outputText);
//     outputGroup->setLayout(outputLayout);
//     rightPanel->addWidget(outputGroup);

//     mainLayout->addLayout(leftPanel, 2);
//     mainLayout->addLayout(rightPanel, 3);

//     setupProblemDescription();
//     setupRules();
//     setupCommands();
// }

// void MainWindow::setupProblemDescription() {
//     QString desc = R"(
// <h2 style='color: #e94560;'>Գրադարանավարի խնդիր</h2>

// <h3 style='color: #00d4aa;'>Խնդրի ձևակերպում</h3>
// <p style='font-size: 11pt; line-height: 1.6;'>
// Համալսարանի հնագույն գրադարանում փակվել է գրադարանավարը։ Գրքերը իրենց տեղերում չեն,
// նա պետք է գտնի լուծում՝ քիչ էներգիա ծախսելով բոլոր գրքերը համապատասխան սենյակներ
// հասցնելու համար։ Միայն այդ դեպքում նա կարող է բացել գրադարանի գլխավոր դուռը։
// Իր առջև կան որոշ սահմանափակումներ։
// </p>

// <p style='font-size: 11pt; font-weight: color: #ffa500;'>
// Պետք է ամեն բան անել, որ գրադարանավարը հնարավորինս շուտ դուրս գա։
// </p>

// <h3 style='color: #00d4aa;'>Գրադարանի կառուցվածք</h3>
// <p><b>Սենյակներ:</b> S (գլխավոր), A, B, C</p>
// <p><b>Միջանցքներ:</b></p>

// <ul style='line-height: 1.8;'>
// <li>S ↔ A </li>
// <li>S ↔ B </li>
// <li>S ↔ C </li>
// <li>A ↔ C </li>
// <li>B ↔ C </li>
// </ul>

// <h3 style='color: #e94560;'>Նպատակ</h3>
// <p style='font-size: 12pt;'>
// Գտնել հրամանների այնպիսի հաջորդականություն, որը կտեղափոխի բոլոր
// 9 գրքերը իրենց նպատակային սենյակներ՝ <b>նվազագույն քայլերով և էներգիա ծախսելով</b>։
// </p>

// <h3 style='color: #00d4aa;'>⭐ Գնահատման համակարգ</h3>
// <p style='font-size: 11pt; color: #aaa;'>Խնդիրն ավարտելուց հետո կստանաք գնահատական՝ ըստ ծախսած էներգիայի և կատարված քայլերի.</p>

// <table border='1' cellpadding='10' style='border-collapse: collapse; width: 100%; margin-top: 8px;'>
// <tr style='background-color: #0f3460; font-weight: bold; text-align: center;'>
//     <th>Գնահատական</th>
//     <th>Էներգիա</th>
//     <th>Քայլեր</th>
// </tr>
// <tr style='text-align: center;'>
//     <td style='font-size: 14pt; letter-spacing: 2px;'>⭐</td>
//     <td style='color: #aaa;'>ցանկացած</td>
//     <td style='color: #aaa;'>ցանկացած</td>
// </tr>
// <tr style='text-align: center;'>
//     <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐</td>
//     <td style='color: #eaeaea;'>&lt; 80</td>
//     <td style='color: #eaeaea;'>&lt; 60</td>
// </tr>
// <tr style='text-align: center;'>
//     <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐⭐</td>
//     <td style='color: #eaeaea;'>&lt; 72</td>
//     <td style='color: #eaeaea;'>&lt; 52</td>
// </tr>
// <tr style='text-align: center;'>
//     <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐⭐⭐</td>
//     <td style='color: #ffa500;'>&lt; 55</td>
//     <td style='color: #ffa500;'>&lt; 40</td>
// </tr>
// <tr style='background-color: #0f3460; text-align: center;'>
//     <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐⭐⭐⭐</td>
//     <td style='color: #ffd700; font-weight: bold;'>&lt; 48</td>
//     <td style='color: #ffd700; font-weight: bold;'>&lt; 35</td>
// </tr>
// </table>
// )";
//     problemDescriptionText->setHtml(desc);
// }

// void MainWindow::setupRules() {
//     QString rules = R"(
// <h2 style='color: #e94560;'>Կանոններ և սահմանափակումներ</h2>

// <h3 style='color: #00d4aa;'>Էներգիայի ծախս</h3>
// <table border='1' cellpadding='8' style='border-collapse: collapse; width: 100%; margin-bottom: 20px;'>
// <tr style='background-color: #0f3460; font-weight: bold;'>
//     <th>Գործողություն</th>
//     <th>Էներգիա</th>
//     <th>Նկարագրություն</th>
// </tr>
// <tr>
//     <td><b>MOVE</b></td>
//     <td style='color: #ffa500; font-weight: bold;'>1 միավոր</td>
//     <td>Անցում՝մի սենյակից մյուսը</td>
// </tr>
// <tr>
//     <td><b>PICK</b></td>
//     <td style='color: #ffa500; font-weight: bold;'>2 միավոր</td>
//     <td>Գիրք վերցնելը ընթացիկ սենյակից</td>
// </tr>
// <tr>
//     <td><b>PLACE</b></td>
//     <td style='color: #ffa500; font-weight: bold;'>2 միավոր</td>
//     <td>Գիրք դնելը ընթացիկ սենյակում</td>
// </tr>
// </table>

// <h3 style='color: #00d4aa;'>Սահմանափակումներ</h3>

// <p><b>1. Գրքերի քանակ:</b></p>
// <ul>
// <li>Միաժամանակ առավելագույնը 2 գիրք կարող եք տանել</li>
// </ul>

// <p><b>2. Դռների սահմանափակումներ:</b></p>
// <table border='1' cellpadding='5' style='border-collapse: collapse; width: 100%;'>
// <tr style='background-color: #0f3460;'>
//     <th>Դուռ</th>
//     <th>Անցման պայման</th>
// </tr>
// <tr>
//     <td>S ↔ B</td>
//     <td style='color: #e94560;'>Միայն դատարկ ձեռքերով</td>
// </tr>
// <tr>
//     <td>A ↔ C</td>
//     <td style='color: #e94560;'>Միայն դատարկ ձեռքերով</td>
// </tr>
// <tr>
//     <td>S ↔ A</td>
//     <td style='color: #00d4aa;'>Ազատ անցում</td>
// </tr>
// <tr>
//     <td>S ↔ C</td>
//     <td style='color: #00d4aa;'>Ազատ անցում</td>
// </tr>
// <tr>
//     <td>B ↔ C</td>
//     <td style='color: #00d4aa;'>Ազատ անցում</td>
// </tr>
// </table>

// <p><b>3. Այլ սահմանափակումներ:</b></p>
// <ul>
// <li>Գիրք վերցնելը հնարավոր է միայն ընթացիկ սենյակից</li>
// <li>Գրադարանավարը սկսում է S սենյակից</li>
// <li>Գիրքը համարվում է հասցված միայն երբ դրված է իր նպատակային սենյակում</li>
// </ul>
// )";
//     rulesText->setHtml(rules);
// }

// void MainWindow::setupCommands() {
//     QString commands = R"(
// <h2 style='color: #e94560;'>Հրամանների նկարագրություն</h2>

// <h3 style='color: #00d4aa;'>MOVE(from->to)</h3>
// <p><b>Գործողություն:</b> Գրադարանավարը շարժվում է մեկ սենյակից մյուսը</p>
// <p><b>Պայման:</b></p>
// <ul>
// <li>Սենյակների միջև պետք է լինի միջանցք</li>
// </ul>
// <p><b>Լրիվ օրինակ:</b></p>
// <pre style='background-color: #0f3460; padding: 15px; border-radius: 5px; font-size: 11pt;'>
// // Հասցնել գիրք 1-ը A սենյակ
// PICK(1)       // Վերցնել գիրք 1-ը (S-ից)
// MOVE(S->A)    // Տեղափոխվել S սենյակից A
// PLACE(1)      // Դնել գիրք 1-ը (հասավ նպատակին)
// </pre>

// <hr style='border-color: #0f3460; margin: 20px 0;'>

// <h3 style='color: #00d4aa;'>PICK(book_id)</h3>
// <p><b>Գործողություն:</b> Վերցնել գիրքը ընթացիկ սենյակից</p>
// <p><b>Պայմաններ:</b></p>
// <ul>
// <li>Գիրքը պետք է գտնվի ընթացիկ սենյակում</li>
// <li>Ձեռքերը չպետք է լիքը լինեն (առավելագույնը 2 գիրք)</li>
// <li>Գիրքը չպետք է արդեն ձեռքին լինի</li>
// </ul>
// <p><b>Լրիվ օրինակ:</b></p>
// <pre style='background-color: #0f3460; padding: 15px; border-radius: 5px; font-size: 11pt;'>
// PICK(1)       // Վերցնել գիրք 1-ը
// PICK(5)       // Վերցնել գիրք 5-ը (հիմա ձեռքերը լիքն են)
// MOVE(S->A)    // Տեղափոխվել A սենյակ
// PLACE(1)      // Դնել գիրք 1-ը
// PLACE(5)      // Դնել գիրք 5-ը
// </pre>

// <hr style='border-color: #0f3460; margin: 20px 0;'>

// <h3 style='color: #00d4aa;'>PLACE(book_id)</h3>
// <p><b>Գործողություն:</b> Դնել գիրքը ընթացիկ սենյակում</p>
// <p><b>Պայմաններ:</b></p>
// <ul>
// <li>Գիրքը պետք է ձեռքին լինի</li>
// <li>Եթե սա նպատակային սենյակն է, գիրքը համարվում է հասցված</li>
// </ul>
// <p><b>Լրիվ օրինակ:</b></p>
// <pre style='background-color: #0f3460; padding: 15px; border-radius: 5px; font-size: 11pt;'>
// PICK(3)       // Վերցնել գիրք 3-ը (A-ից)
// MOVE(A->S)    // Գնալ S (A->C-ի համար պետք են դատարկ ձեռքեր)
// MOVE(S->C)    // Գնալ նպատակային սենյակ
// PLACE(3)      // Դնել նպատակային սենյակում
// </pre>

// <hr style='border-color: #0f3460; margin: 20px 0;'>


// )";
//     commandsText->setHtml(commands);
// }

// //Սահմանում ենք մեր գրածների գույներն ու չափերը
// void MainWindow::applyStyles() {
//     QString style = R"(
//         QMainWindow {
//             background-color: #1a1a2e;
//         }
//         QTabWidget::pane {
//             border: 2px solid #0f3460;
//             background-color: #16213e;
//             border-radius: 5px;
//         }
//         QTabBar::tab {
//             background-color: #0f3460;
//             color: #eaeaea;
//             padding: 10px 20px;
//             margin-right: 2px;
//             border-top-left-radius: 5px;
//             border-top-right-radius: 5px;
//             font-size: 11pt;
//         }
//         QTabBar::tab:selected {
//             background-color: #e94560;
//             font-weight: bold;
//         }
//         QTabBar::tab:hover {
//             background-color: #16213e;
//         }
//         QGroupBox {
//             font-weight: bold;
//             font-size: 12pt;
//             border: 2px solid #0f3460;
//             border-radius: 8px;
//             margin-top: 10px;
//             padding-top: 10px;
//             color: #eaeaea;
//             background-color: #16213e;
//         }
//         QGroupBox::title {
//             subcontrol-origin: margin;
//             left: 15px;
//             padding: 0 5px 0 5px;
//         }
//         QPushButton {
//             background-color: #0f3460;
//             color: white;
//             border: none;
//             border-radius: 8px;
//             font-size: 11pt;
//             font-weight: bold;
//             padding: 8px;
//         }
//         QPushButton:hover {
//             background-color: #16213e;
//         }
//         QPushButton:pressed {
//             background-color: #c93550;
//         }
//         QTextEdit {
//             background-color: #0f3460;
//             color: #eaeaea;
//             border: 1px solid #16213e;
//             border-radius: 5px;
//             font-size: 10pt;
//             padding: 5px;
//         }
//         QLineEdit {
//             background-color: #0f3460;
//             color: #eaeaea;
//             border: 2px solid #16213e;
//             border-radius: 5px;
//             font-size: 12pt;
//         }
//         QLineEdit:focus {
//             border: 2px solid #e94560;
//         }
//         QListWidget {
//             background-color: #0f3460;
//             color: #eaeaea;
//             border: 1px solid #16213e;
//             border-radius: 5px;
//             font-size: 10pt;
//         }
//         QListWidget::item:hover {
//             background-color: #16213e;
//         }
//         QListWidget::item:selected {
//             background-color: #e94560;
//         }
//         QTableWidget {
//             background-color: #0f3460;
//             color: #eaeaea;
//             gridline-color: #16213e;
//             border: none;
//             font-size: 10pt;
//         }
//         QHeaderView::section {
//             background-color: #16213e;
//             color: #eaeaea;
//             padding: 8px;
//             border: 1px solid #0f3460;
//             font-weight: bold;
//         }
//     )";

//     setStyleSheet(style);
// }

// void MainWindow::initializeState() {
//     currentState.initialize();
//     commandHistory.clear();
//     commandHistoryList->clear();

//     updateDisplay();
//     updateBooksTable();

//     logOutput("Մուտքագրեք հրամաններ և լուծեք խնդիրը։");
// }

// void MainWindow::updateDisplay() {
//     roomLabel->setText(currentState.librarianRoom);
//     booksInHandLabel->setText(QString("%1 / 2").arg(currentState.booksInHand.size()));
//     energyLabel->setText(QString::number(currentState.totalEnergy));

//     QString status = QString("Հասցված: %1 / %2")
//                          .arg(currentState.deliveredBooks.size())
//                          .arg(currentState.books.size());
//     statusLabel->setText(status);

//     if (currentState.isComplete()) {
//         statusLabel->setText("✓ Բոլոր գրքերը հասցված են!");
//         logOutput("🏆 Բոլոր գրքերը հաջողությամբ հասցվեցին! Խնդիրը լուծված է։");
//         showStarRatingDialog();
//     }
// }

// void MainWindow::updateBooksTable() {
//     for (int i = 0; i < 9; i++) {
//         int bookId = i + 1;
//         const Book& book = currentState.books[bookId];

//         booksTable->setItem(i, 0, new QTableWidgetItem(QString::number(bookId)));
//         booksTable->setItem(i, 1, new QTableWidgetItem(book.startRoom));
//         booksTable->setItem(i, 2, new QTableWidgetItem(book.targetRoom));

//         bool inHand = currentState.booksInHand.contains(bookId);
//         QString status = getBookStatusIcon(book.delivered, inHand);
//         booksTable->setItem(i, 3, new QTableWidgetItem(status));

//         for (int col = 0; col < 4; col++) {
//             QTableWidgetItem* item = booksTable->item(i, col);
//             if (book.delivered) {
//                 item->setBackground(QColor("#00d4aa"));
//                 item->setForeground(QColor("#000"));
//                 item->setFont(QFont("Arial", 10, QFont::Bold));
//             } else if (inHand) {
//                 item->setBackground(QColor("#e94560"));
//                 item->setForeground(QColor("#fff"));
//             }
//         }
//     }
// }

// QString MainWindow::getBookStatusIcon(bool delivered, bool inHand) {
//     if (delivered) return "Հասցված";
//     if (inHand) return "Ձեռքին";
//     return "Սպասում";
// }

// void MainWindow::onExecuteCommandClicked() {
//     QString command = commandInput->text().trimmed();
//     if (command.isEmpty()) {
//         logOutput("Խնդրում եմ մուտքագրել հրաման", true);
//         return;
//     }

//     QString output;
//     bool success = interpreter->executeCommand(command, currentState, output);

//     if (success) {
//         logOutput(QString("%1 - %2").arg(command, output));
//         addToHistory(command);
//         updateDisplay();
//         updateBooksTable();
//     } else {
//         logOutput(QString("%1 - %2").arg(command, output), true);
//     }

//     commandInput->clear();
//     commandInput->setFocus();
// }

// void MainWindow::addToHistory(const QString& command) {
//     commandHistory.append(command);
//     QString historyItem = QString("%1. %2").arg(commandHistory.size()).arg(command);
//     commandHistoryList->addItem(historyItem);
//     commandHistoryList->scrollToBottom();
// }

// void MainWindow::onResetClicked() {
//     QMessageBox::StandardButton reply = QMessageBox::question(
//         this, "Վերսկսել",
//         "Վստա՞հ եք, որ ցանկանում եք վերսկսել։ Ողջ առաջընթացը կկորի։",
//         QMessageBox::Yes | QMessageBox::No);

//     if (reply == QMessageBox::Yes) {
//         initializeState();
//         outputText->clear();
//         commandInput->clear();

//         logOutput("Համակարգը վերականգնված է։");
//     }
// }

// void MainWindow::logOutput(const QString& message, bool isError) {
//     QString color = isError ? "#e94560" : "#eaeaea";
//     QString html = QString("<span style='color: %1;'>%2</span>").arg(color, message);
//     outputText->append(html);
// }

// void MainWindow::showStarRatingDialog() {
//     int steps = commandHistory.size();
//     int energy = currentState.totalEnergy;

//     // Star rating based on combined energy + steps thresholds
//     // Optimal solution: ~31 steps, 48 energy
//     // c1: completed at all (always true here)
//     bool c1 = true;
//     // c2: 2 stars — energy < 80
//     bool c2 = (energy < 80);
//     // c3: 3 stars — energy < 72
//     bool c3 = (energy < 72);
//     // c4: 4 stars — energy < 55
//     bool c4 = (energy < 55);
//     // c5: 5 stars — energy < 52
//     bool c5 = (energy < 52);

//     // Stars are cumulative: each higher level requires all lower levels too
//     int stars = 1;
//     if (c2) stars = 2;
//     if (c2 && c3) stars = 3;
//     if (c2 && c3 && c4) stars = 4;
//     if (c2 && c3 && c4 && c5) stars = 5;

//     QDialog *dialog = new QDialog(this);
//     dialog->setWindowTitle("Արդյունք");
//     dialog->setFixedSize(380, 280);
//     dialog->setStyleSheet(R"(
//         QDialog {
//             background-color: #1a1a2e;
//         }
//         QLabel {
//             color: #eaeaea;
//             background-color: transparent;
//         }
//         QPushButton {
//             background-color: #00d4aa;
//             color: #1a1a2e;
//             border: none;
//             border-radius: 8px;
//             font-size: 13pt;
//             font-weight: bold;
//             padding: 10px 30px;
//             min-width: 120px;
//         }
//         QPushButton:hover {
//             background-color: #00b894;
//         }
//     )");

//     QVBoxLayout *layout = new QVBoxLayout(dialog);
//     layout->setSpacing(12);
//     layout->setContentsMargins(30, 25, 30, 25);

//     // Title
//     QString titleText;
//     switch (stars) {
//     case 5: titleText = "🏆 ԿԱՏԱՐՅԱԼ!"; break;
//     case 4: titleText = "⭐ ԳԵՐԱԶԱՆՑ!"; break;
//     case 3: titleText = " ԼԱՎ"; break;
//     case 2: titleText = " ԲԱՎԱՐԱՐ"; break;
//     default: titleText = " ԹՈՒՅԼ"; break;
//     }

//     QLabel *titleLabel = new QLabel(titleText);
//     titleLabel->setAlignment(Qt::AlignCenter);
//     titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #ffd700;");
//     layout->addWidget(titleLabel);

//     // Stars row
//     QString starsHtml;
//     for (int i = 0; i < 5; i++) {
//         if (i < stars)
//             starsHtml += "<span style='color: #ffd700; font-size: 24pt;'>★</span>";
//         else
//             starsHtml += "<span style='color: #555; font-size: 24pt;'>★</span>";
//     }
//     QLabel *starsLabel = new QLabel(starsHtml);
//     starsLabel->setAlignment(Qt::AlignCenter);
//     starsLabel->setTextFormat(Qt::RichText);
//     layout->addWidget(starsLabel);

//     // Score label
//     QLabel *scoreLabel = new QLabel(QString("%1 / 5 աստղ").arg(stars));
//     scoreLabel->setAlignment(Qt::AlignCenter);
//     scoreLabel->setStyleSheet("font-size: 8pt; color: #aaa;");
//     layout->addWidget(scoreLabel);

//     // Stats line
//     QLabel *statsLabel = new QLabel(
//         QString("Քայլեր: <b style='color:#00d4aa;'>%1</b>  |  Էներգիա: <b style='color:#ffa500;'>%2</b>")
//             .arg(steps).arg(energy));
//     statsLabel->setAlignment(Qt::AlignCenter);
//     statsLabel->setTextFormat(Qt::RichText);
//     statsLabel->setStyleSheet("font-size: 9pt; margin-bottom: 5px;");
//     layout->addWidget(statsLabel);

//     layout->addStretch();

//     bool shouldRetry = false;

//     if (stars < 5) {
//         // Ask user if they want to retry
//         QLabel *retryLabel = new QLabel("Դուր եկա՞վ ձեր ստացած արդյունքը,\nթե՞ ուզում եք կրկին փորձել։");
//         retryLabel->setAlignment(Qt::AlignCenter);
//         retryLabel->setStyleSheet("font-size: 10pt; color: #eaeaea; margin-top: 3px;");
//         layout->addWidget(retryLabel);

//         QHBoxLayout *btnRow = new QHBoxLayout();
//         btnRow->setSpacing(15);

//         QPushButton *closeBtn = new QPushButton("Այո, փակել");
//         closeBtn->setStyleSheet(R"(
//             QPushButton {
//                 background-color: #0f3460;
//                 color: #eaeaea;
//                 border: 2px solid #16213e;
//                 border-radius: 8px;
//                 font-size: 11pt;
//                 font-weight: bold;
//                 padding: 9px 18px;
//             }
//             QPushButton:hover { background-color: #16213e; }
//         )");

//         QPushButton *retryBtn = new QPushButton("Կրկին փորձել");
//         retryBtn->setStyleSheet(R"(
//             QPushButton {
//                 background-color: #00d4aa;
//                 color: #1a1a2e;
//                 border: none;
//                 border-radius: 8px;
//                 font-size: 11pt;
//                 font-weight: bold;
//                 padding: 9px 18px;
//             }
//             QPushButton:hover { background-color: #00b894; }
//         )");

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
//         // 5 stars — just a close button
//         QPushButton *closeBtn = new QPushButton("Փակել");
//         QHBoxLayout *btnLayout = new QHBoxLayout();
//         btnLayout->addStretch();
//         btnLayout->addWidget(closeBtn);
//         btnLayout->addStretch();
//         layout->addLayout(btnLayout);
//         connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
//     }

//     // Պահպանել արդյունքը դատաբազայում
//     ResultSaver::save("library", stars);

//     dialog->exec();
//     delete dialog;

//     if (shouldRetry) {
//         // Reset without confirmation dialog
//         currentState.initialize();
//         commandHistory.clear();
//         commandHistoryList->clear();
//         outputText->clear();
//         commandInput->clear();
//         updateDisplay();
//         updateBooksTable();
//         logOutput("Համակարգը վերականգնված է։ Կրկին փորձեք!");
//     }
// }




#include "mainwindow.h"
#include "resultsaver.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    interpreter = new Interpreter();

    setupUI();
    applyStyles();
    initializeState();
}

MainWindow::~MainWindow()
{
    delete interpreter;
}

void MainWindow::setupUI() {
    setWindowTitle("Գրադարանավարի խնդիր");
    resize(1400, 900);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *leftPanel = new QVBoxLayout();
    leftPanel->setSpacing(10);

    // Tab Widget
    tabWidget = new QTabWidget();

    // Tab 1: Խնդրի նկարագրություն
    QWidget *descTab = new QWidget();
    QVBoxLayout *descLayout = new QVBoxLayout(descTab);
    problemDescriptionText = new QTextEdit();
    problemDescriptionText->setReadOnly(true);
    descLayout->addWidget(problemDescriptionText);
    tabWidget->addTab(descTab, "Նկարագրություն");

    // Tab 2: Կանոններ
    QWidget *rulesTab = new QWidget();
    QVBoxLayout *rulesLayout = new QVBoxLayout(rulesTab);
    rulesText = new QTextEdit();
    rulesText->setReadOnly(true);
    rulesLayout->addWidget(rulesText);
    tabWidget->addTab(rulesTab, "Կանոններ");

    // Tab 3: Հրամաններ
    QWidget *commandsTab = new QWidget();
    QVBoxLayout *commandsLayout = new QVBoxLayout(commandsTab);
    commandsText = new QTextEdit();
    commandsText->setReadOnly(true);
    commandsLayout->addWidget(commandsText);
    tabWidget->addTab(commandsTab, "Հրամաններ");

    // Tab 4: Գրքեր
    QWidget *booksTab = new QWidget();
    QVBoxLayout *booksLayout = new QVBoxLayout(booksTab);
    booksTable = new QTableWidget(9, 4);
    booksTable->setHorizontalHeaderLabels({"ID", "Սկիզբ", "Նպատակ", "Կարգավիճակ"});
    booksTable->horizontalHeader()->setStretchLastSection(true);
    booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    booksTable->setSelectionMode(QAbstractItemView::NoSelection);
    booksLayout->addWidget(booksTable);
    tabWidget->addTab(booksTab, "Գրքեր");

    leftPanel->addWidget(tabWidget);

    // Status Group
    QGroupBox *statusGroup = new QGroupBox("Ընթացիկ վիճակ");
    QGridLayout *statusLayout = new QGridLayout();
    statusLayout->setSpacing(10);

    QLabel *roomLabelTitle = new QLabel("Սենյակ:");
    roomLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
    roomLabel = new QLabel("S");
    roomLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #e94560;");

    QLabel *booksLabelTitle = new QLabel("Ձեռքին գրքեր:");
    booksLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
    booksInHandLabel = new QLabel("0 / 2");
    booksInHandLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #00d4aa;");

    QLabel *energyLabelTitle = new QLabel("Էներգիա:");
    energyLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
    energyLabel = new QLabel("0");
    energyLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #ffa500;");

    QLabel *statusLabelTitle = new QLabel("Վիճակ:");
    statusLabelTitle->setStyleSheet("font-size: 12pt; color: #aaa;");
    statusLabel = new QLabel("Պատրաստ է");
    statusLabel->setStyleSheet("font-size: 12pt; color: #eaeaea;");

    statusLayout->addWidget(roomLabelTitle, 0, 0);
    statusLayout->addWidget(roomLabel, 0, 1);
    statusLayout->addWidget(booksLabelTitle, 1, 0);
    statusLayout->addWidget(booksInHandLabel, 1, 1);
    statusLayout->addWidget(energyLabelTitle, 2, 0);
    statusLayout->addWidget(energyLabel, 2, 1);
    statusLayout->addWidget(statusLabelTitle, 3, 0);
    statusLayout->addWidget(statusLabel, 3, 1);

    statusGroup->setLayout(statusLayout);
    leftPanel->addWidget(statusGroup);


    QGroupBox *manualGroup = new QGroupBox("Հրամանի մուտքագրում");
    QVBoxLayout *manualLayout = new QVBoxLayout();

    commandInput = new QLineEdit();
    commandInput->setPlaceholderText("Օրինակ: PICK(1), MOVE(S->A), PLACE(1)");
    commandInput->setStyleSheet("padding: 10px; font-size: 12pt;");
    connect(commandInput, &QLineEdit::returnPressed, this, &MainWindow::onExecuteCommandClicked);

    executeCommandButton = new QPushButton("Կատարել հրաման");
    executeCommandButton->setFixedHeight(45);
    executeCommandButton->setStyleSheet("background-color: #00d4aa; font-size: 12pt;");
    connect(executeCommandButton, &QPushButton::clicked, this, &MainWindow::onExecuteCommandClicked);

    manualLayout->addWidget(commandInput);
    manualLayout->addWidget(executeCommandButton);
    manualGroup->setLayout(manualLayout);
    leftPanel->addWidget(manualGroup);

    QVBoxLayout *rightPanel = new QVBoxLayout();

    resetButton = new QPushButton("Վերսկսել");
    resetButton->setFixedHeight(50);
    resetButton->setStyleSheet("background-color: #e94560; font-size: 13pt;");
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    rightPanel->addWidget(resetButton);

    QGroupBox *historyGroup = new QGroupBox("Հրամանների պատմություն");
    QVBoxLayout *historyLayout = new QVBoxLayout();

    commandHistoryList = new QListWidget();
    commandHistoryList->setStyleSheet("font-family: 'Courier New'; font-size: 11pt;");

    historyLayout->addWidget(commandHistoryList);
    historyGroup->setLayout(historyLayout);
    rightPanel->addWidget(historyGroup);

    QGroupBox *outputGroup = new QGroupBox("Հաղորդագրություններ");
    QVBoxLayout *outputLayout = new QVBoxLayout();
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    outputText->setMaximumHeight(250);
    outputLayout->addWidget(outputText);
    outputGroup->setLayout(outputLayout);
    rightPanel->addWidget(outputGroup);

    mainLayout->addLayout(leftPanel, 2);
    mainLayout->addLayout(rightPanel, 3);

    setupProblemDescription();
    setupRules();
    setupCommands();
}

void MainWindow::setupProblemDescription() {
    QString desc = R"(
<h2 style='color: #e94560;'>Գրադարանավարի խնդիր</h2>

<h3 style='color: #00d4aa;'>Խնդրի ձևակերպում</h3>
<p style='font-size: 11pt; line-height: 1.6;'>
Համալսարանի հնագույն գրադարանում փակվել է գրադարանավարը։ Գրքերը իրենց տեղերում չեն,
նա պետք է գտնի լուծում՝ քիչ էներգիա ծախսելով բոլոր գրքերը համապատասխան սենյակներ
հասցնելու համար։ Միայն այդ դեպքում նա կարող է բացել գրադարանի գլխավոր դուռը։
Իր առջև կան որոշ սահմանափակումներ։
</p>

<p style='font-size: 11pt; font-weight: color: #ffa500;'>
Պետք է ամեն բան անել, որ գրադարանավարը հնարավորինս շուտ դուրս գա։
</p>

<h3 style='color: #00d4aa;'>Գրադարանի կառուցվածք</h3>
<p><b>Սենյակներ:</b> S (գլխավոր), A, B, C</p>
<p><b>Միջանցքներ:</b></p>

<ul style='line-height: 1.8;'>
<li>S ↔ A </li>
<li>S ↔ B </li>
<li>S ↔ C </li>
<li>A ↔ C </li>
<li>B ↔ C </li>
</ul>

<h3 style='color: #e94560;'>Նպատակ</h3>
<p style='font-size: 12pt;'>
Գտնել հրամանների այնպիսի հաջորդականություն, որը կտեղափոխի բոլոր
9 գրքերը իրենց նպատակային սենյակներ՝ <b>նվազագույն քայլերով և էներգիա ծախսելով</b>։
</p>

<h3 style='color: #00d4aa;'>⭐ Գնահատման համակարգ</h3>
<p style='font-size: 11pt; color: #aaa;'>Խնդիրն ավարտելուց հետո կստանաք գնահատական՝ ըստ ծախսած էներգիայի և կատարված քայլերի.</p>

<table border='1' cellpadding='10' style='border-collapse: collapse; width: 100%; margin-top: 8px;'>
<tr style='background-color: #0f3460; font-weight: bold; text-align: center;'>
    <th>Գնահատական</th>
    <th>Էներգիա</th>
    <th>Քայլեր</th>
</tr>
<tr style='text-align: center;'>
    <td style='font-size: 14pt; letter-spacing: 2px;'>⭐</td>
    <td style='color: #aaa;'>ցանկացած</td>
    <td style='color: #aaa;'>ցանկացած</td>
</tr>
<tr style='text-align: center;'>
    <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐</td>
    <td style='color: #eaeaea;'>&lt; 80</td>
    <td style='color: #eaeaea;'>&lt; 60</td>
</tr>
<tr style='text-align: center;'>
    <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐⭐</td>
    <td style='color: #eaeaea;'>&lt; 72</td>
    <td style='color: #eaeaea;'>&lt; 52</td>
</tr>
<tr style='text-align: center;'>
    <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐⭐⭐</td>
    <td style='color: #ffa500;'>&lt; 55</td>
    <td style='color: #ffa500;'>&lt; 40</td>
</tr>
<tr style='background-color: #0f3460; text-align: center;'>
    <td style='font-size: 14pt; letter-spacing: 2px;'>⭐⭐⭐⭐⭐</td>
    <td style='color: #ffd700; font-weight: bold;'>&lt; 48</td>
    <td style='color: #ffd700; font-weight: bold;'>&lt; 35</td>
</tr>
</table>
)";
    problemDescriptionText->setHtml(desc);
}

void MainWindow::setupRules() {
    QString rules = R"(
<h2 style='color: #e94560;'>Կանոններ և սահմանափակումներ</h2>

<h3 style='color: #00d4aa;'>Էներգիայի ծախս</h3>
<table border='1' cellpadding='8' style='border-collapse: collapse; width: 100%; margin-bottom: 20px;'>
<tr style='background-color: #0f3460; font-weight: bold;'>
    <th>Գործողություն</th>
    <th>Էներգիա</th>
    <th>Նկարագրություն</th>
</tr>
<tr>
    <td><b>MOVE</b></td>
    <td style='color: #ffa500; font-weight: bold;'>1 միավոր</td>
    <td>Անցում՝մի սենյակից մյուսը</td>
</tr>
<tr>
    <td><b>PICK</b></td>
    <td style='color: #ffa500; font-weight: bold;'>2 միավոր</td>
    <td>Գիրք վերցնելը ընթացիկ սենյակից</td>
</tr>
<tr>
    <td><b>PLACE</b></td>
    <td style='color: #ffa500; font-weight: bold;'>2 միավոր</td>
    <td>Գիրք դնելը ընթացիկ սենյակում</td>
</tr>
</table>

<h3 style='color: #00d4aa;'>Սահմանափակումներ</h3>

<p><b>1. Գրքերի քանակ:</b></p>
<ul>
<li>Միաժամանակ առավելագույնը 2 գիրք կարող եք տանել</li>
</ul>

<p><b>2. Դռների սահմանափակումներ:</b></p>
<table border='1' cellpadding='5' style='border-collapse: collapse; width: 100%;'>
<tr style='background-color: #0f3460;'>
    <th>Դուռ</th>
    <th>Անցման պայման</th>
</tr>
<tr>
    <td>S ↔ B</td>
    <td style='color: #e94560;'>Միայն դատարկ ձեռքերով</td>
</tr>
<tr>
    <td>A ↔ C</td>
    <td style='color: #e94560;'>Միայն դատարկ ձեռքերով</td>
</tr>
<tr>
    <td>S ↔ A</td>
    <td style='color: #00d4aa;'>Ազատ անցում</td>
</tr>
<tr>
    <td>S ↔ C</td>
    <td style='color: #00d4aa;'>Ազատ անցում</td>
</tr>
<tr>
    <td>B ↔ C</td>
    <td style='color: #00d4aa;'>Ազատ անցում</td>
</tr>
</table>

<p><b>3. Այլ սահմանափակումներ:</b></p>
<ul>
<li>Գիրք վերցնելը հնարավոր է միայն ընթացիկ սենյակից</li>
<li>Գրադարանավարը սկսում է S սենյակից</li>
<li>Գիրքը համարվում է հասցված միայն երբ դրված է իր նպատակային սենյակում</li>
</ul>
)";
    rulesText->setHtml(rules);
}

void MainWindow::setupCommands() {
    QString commands = R"(
<h2 style='color: #e94560;'>Հրամանների նկարագրություն</h2>

<h3 style='color: #00d4aa;'>MOVE(from->to)</h3>
<p><b>Գործողություն:</b> Գրադարանավարը շարժվում է մեկ սենյակից մյուսը</p>
<p><b>Պայման:</b></p>
<ul>
<li>Սենյակների միջև պետք է լինի միջանցք</li>
</ul>
<p><b>Լրիվ օրինակ:</b></p>
<pre style='background-color: #0f3460; padding: 15px; border-radius: 5px; font-size: 11pt;'>
// Հասցնել գիրք 1-ը A սենյակ
PICK(1)       // Վերցնել գիրք 1-ը (S-ից)
MOVE(S->A)    // Տեղափոխվել S սենյակից A
PLACE(1)      // Դնել գիրք 1-ը (հասավ նպատակին)
</pre>

<hr style='border-color: #0f3460; margin: 20px 0;'>

<h3 style='color: #00d4aa;'>PICK(book_id)</h3>
<p><b>Գործողություն:</b> Վերցնել գիրքը ընթացիկ սենյակից</p>
<p><b>Պայմաններ:</b></p>
<ul>
<li>Գիրքը պետք է գտնվի ընթացիկ սենյակում</li>
<li>Ձեռքերը չպետք է լիքը լինեն (առավելագույնը 2 գիրք)</li>
<li>Գիրքը չպետք է արդեն ձեռքին լինի</li>
</ul>
<p><b>Լրիվ օրինակ:</b></p>
<pre style='background-color: #0f3460; padding: 15px; border-radius: 5px; font-size: 11pt;'>
PICK(1)       // Վերցնել գիրք 1-ը
PICK(5)       // Վերցնել գիրք 5-ը (հիմա ձեռքերը լիքն են)
MOVE(S->A)    // Տեղափոխվել A սենյակ
PLACE(1)      // Դնել գիրք 1-ը
PLACE(5)      // Դնել գիրք 5-ը
</pre>

<hr style='border-color: #0f3460; margin: 20px 0;'>

<h3 style='color: #00d4aa;'>PLACE(book_id)</h3>
<p><b>Գործողություն:</b> Դնել գիրքը ընթացիկ սենյակում</p>
<p><b>Պայմաններ:</b></p>
<ul>
<li>Գիրքը պետք է ձեռքին լինի</li>
<li>Եթե սա նպատակային սենյակն է, գիրքը համարվում է հասցված</li>
</ul>
<p><b>Լրիվ օրինակ:</b></p>
<pre style='background-color: #0f3460; padding: 15px; border-radius: 5px; font-size: 11pt;'>
PICK(3)       // Վերցնել գիրք 3-ը (A-ից)
MOVE(A->S)    // Գնալ S (A->C-ի համար պետք են դատարկ ձեռքեր)
MOVE(S->C)    // Գնալ նպատակային սենյակ
PLACE(3)      // Դնել նպատակային սենյակում
</pre>

<hr style='border-color: #0f3460; margin: 20px 0;'>


)";
    commandsText->setHtml(commands);
}

//Սահմանում ենք մեր գրածների գույներն ու չափերը
void MainWindow::applyStyles() {
    QString style = R"(
        QMainWindow {
            background-color: #1a1a2e;
        }
        QTabWidget::pane {
            border: 2px solid #0f3460;
            background-color: #16213e;
            border-radius: 5px;
        }
        QTabBar::tab {
            background-color: #0f3460;
            color: #eaeaea;
            padding: 10px 20px;
            margin-right: 2px;
            border-top-left-radius: 5px;
            border-top-right-radius: 5px;
            font-size: 11pt;
        }
        QTabBar::tab:selected {
            background-color: #e94560;
            font-weight: bold;
        }
        QTabBar::tab:hover {
            background-color: #16213e;
        }
        QGroupBox {
            font-weight: bold;
            font-size: 12pt;
            border: 2px solid #0f3460;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            color: #eaeaea;
            background-color: #16213e;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 0 5px 0 5px;
        }
        QPushButton {
            background-color: #0f3460;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 11pt;
            font-weight: bold;
            padding: 8px;
        }
        QPushButton:hover {
            background-color: #16213e;
        }
        QPushButton:pressed {
            background-color: #c93550;
        }
        QTextEdit {
            background-color: #0f3460;
            color: #eaeaea;
            border: 1px solid #16213e;
            border-radius: 5px;
            font-size: 10pt;
            padding: 5px;
        }
        QLineEdit {
            background-color: #0f3460;
            color: #eaeaea;
            border: 2px solid #16213e;
            border-radius: 5px;
            font-size: 12pt;
        }
        QLineEdit:focus {
            border: 2px solid #e94560;
        }
        QListWidget {
            background-color: #0f3460;
            color: #eaeaea;
            border: 1px solid #16213e;
            border-radius: 5px;
            font-size: 10pt;
        }
        QListWidget::item:hover {
            background-color: #16213e;
        }
        QListWidget::item:selected {
            background-color: #e94560;
        }
        QTableWidget {
            background-color: #0f3460;
            color: #eaeaea;
            gridline-color: #16213e;
            border: none;
            font-size: 10pt;
        }
        QHeaderView::section {
            background-color: #16213e;
            color: #eaeaea;
            padding: 8px;
            border: 1px solid #0f3460;
            font-weight: bold;
        }
    )";

    setStyleSheet(style);
}

void MainWindow::initializeState() {
    currentState.initialize();
    commandHistory.clear();
    commandHistoryList->clear();

    updateDisplay();
    updateBooksTable();

    logOutput("Մուտքագրեք հրամաններ և լուծեք խնդիրը։");
}

void MainWindow::updateDisplay() {
    roomLabel->setText(currentState.librarianRoom);
    booksInHandLabel->setText(QString("%1 / 2").arg(currentState.booksInHand.size()));
    energyLabel->setText(QString::number(currentState.totalEnergy));

    QString status = QString("Հասցված: %1 / %2")
                         .arg(currentState.deliveredBooks.size())
                         .arg(currentState.books.size());
    statusLabel->setText(status);

    if (currentState.isComplete()) {
        statusLabel->setText("✓ Բոլոր գրքերը հասցված են!");
        logOutput("🏆 Բոլոր գրքերը հաջողությամբ հասցվեցին! Խնդիրը լուծված է։");
        showStarRatingDialog();
    }
}

void MainWindow::updateBooksTable() {
    for (int i = 0; i < 9; i++) {
        int bookId = i + 1;
        const Book& book = currentState.books[bookId];

        booksTable->setItem(i, 0, new QTableWidgetItem(QString::number(bookId)));
        booksTable->setItem(i, 1, new QTableWidgetItem(book.startRoom));
        booksTable->setItem(i, 2, new QTableWidgetItem(book.targetRoom));

        bool inHand = currentState.booksInHand.contains(bookId);
        QString status = getBookStatusIcon(book.delivered, inHand);
        booksTable->setItem(i, 3, new QTableWidgetItem(status));

        for (int col = 0; col < 4; col++) {
            QTableWidgetItem* item = booksTable->item(i, col);
            if (book.delivered) {
                item->setBackground(QColor("#00d4aa"));
                item->setForeground(QColor("#000"));
                item->setFont(QFont("Arial", 10, QFont::Bold));
            } else if (inHand) {
                item->setBackground(QColor("#e94560"));
                item->setForeground(QColor("#fff"));
            }
        }
    }
}

QString MainWindow::getBookStatusIcon(bool delivered, bool inHand) {
    if (delivered) return "Հասցված";
    if (inHand) return "Ձեռքին";
    return "Սպասում";
}

void MainWindow::onExecuteCommandClicked() {
    QString command = commandInput->text().trimmed();
    if (command.isEmpty()) {
        logOutput("Խնդրում եմ մուտքագրել հրաման", true);
        return;
    }

    QString output;
    bool success = interpreter->executeCommand(command, currentState, output);

    if (success) {
        logOutput(QString("%1 - %2").arg(command, output));
        addToHistory(command);
        updateDisplay();
        updateBooksTable();
    } else {
        logOutput(QString("%1 - %2").arg(command, output), true);
    }

    commandInput->clear();
    commandInput->setFocus();
}

void MainWindow::addToHistory(const QString& command) {
    commandHistory.append(command);
    QString historyItem = QString("%1. %2").arg(commandHistory.size()).arg(command);
    commandHistoryList->addItem(historyItem);
    commandHistoryList->scrollToBottom();
}

void MainWindow::onResetClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Վերսկսել",
        "Վստա՞հ եք, որ ցանկանում եք վերսկսել։ Ողջ առաջընթացը կկորի։",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        initializeState();
        outputText->clear();
        commandInput->clear();

        logOutput("Համակարգը վերականգնված է։");
    }
}

void MainWindow::logOutput(const QString& message, bool isError) {
    QString color = isError ? "#e94560" : "#eaeaea";
    QString html = QString("<span style='color: %1;'>%2</span>").arg(color, message);
    outputText->append(html);
}

void MainWindow::showStarRatingDialog() {
    int steps = commandHistory.size();
    int energy = currentState.totalEnergy;

    // Star rating based on combined energy + steps thresholds
    // Optimal solution: ~31 steps, 48 energy
    // c1: completed at all (always true here)
    bool c1 = true;
    // c2: 2 stars — energy < 80
    bool c2 = (energy < 80);
    // c3: 3 stars — energy < 72
    bool c3 = (energy < 72);
    // c4: 4 stars — energy < 55
    bool c4 = (energy < 55);
    // c5: 5 stars — energy < 52
    bool c5 = (energy < 52);

    // Stars are cumulative: each higher level requires all lower levels too
    int stars = 1;
    if (c2) stars = 2;
    if (c2 && c3) stars = 3;
    if (c2 && c3 && c4) stars = 4;
    if (c2 && c3 && c4 && c5) stars = 5;

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Արդյունք");
    dialog->setFixedSize(380, 280);
    dialog->setStyleSheet(R"(
        QDialog {
            background-color: #1a1a2e;
        }
        QLabel {
            color: #eaeaea;
            background-color: transparent;
        }
        QPushButton {
            background-color: #00d4aa;
            color: #1a1a2e;
            border: none;
            border-radius: 8px;
            font-size: 13pt;
            font-weight: bold;
            padding: 10px 30px;
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #00b894;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(12);
    layout->setContentsMargins(30, 25, 30, 25);

    // Title
    QString titleText;
    switch (stars) {
    case 5: titleText = "🏆 ԿԱՏԱՐՅԱԼ!"; break;
    case 4: titleText = "⭐ ԳԵՐԱԶԱՆՑ!"; break;
    case 3: titleText = " ԼԱՎ"; break;
    case 2: titleText = " ԲԱՎԱՐԱՐ"; break;
    default: titleText = " ԹՈՒՅԼ"; break;
    }

    QLabel *titleLabel = new QLabel(titleText);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #ffd700;");
    layout->addWidget(titleLabel);

    // Stars row
    QString starsHtml;
    for (int i = 0; i < 5; i++) {
        if (i < stars)
            starsHtml += "<span style='color: #ffd700; font-size: 24pt;'>★</span>";
        else
            starsHtml += "<span style='color: #555; font-size: 24pt;'>★</span>";
    }
    QLabel *starsLabel = new QLabel(starsHtml);
    starsLabel->setAlignment(Qt::AlignCenter);
    starsLabel->setTextFormat(Qt::RichText);
    layout->addWidget(starsLabel);

    // Score label
    QLabel *scoreLabel = new QLabel(QString("%1 / 5 աստղ").arg(stars));
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 8pt; color: #aaa;");
    layout->addWidget(scoreLabel);

    // Stats line
    QLabel *statsLabel = new QLabel(
        QString("Քայլեր: <b style='color:#00d4aa;'>%1</b>  |  Էներգիա: <b style='color:#ffa500;'>%2</b>")
            .arg(steps).arg(energy));
    statsLabel->setAlignment(Qt::AlignCenter);
    statsLabel->setTextFormat(Qt::RichText);
    statsLabel->setStyleSheet("font-size: 9pt; margin-bottom: 5px;");
    layout->addWidget(statsLabel);

    layout->addStretch();

    bool shouldRetry = false;

    if (stars < 5) {
        // Ask user if they want to retry
        QLabel *retryLabel = new QLabel("Դուր եկա՞վ ձեր ստացած արդյունքը,\nթե՞ ուզում եք կրկին փորձել։");
        retryLabel->setAlignment(Qt::AlignCenter);
        retryLabel->setStyleSheet("font-size: 10pt; color: #eaeaea; margin-top: 3px;");
        layout->addWidget(retryLabel);

        QHBoxLayout *btnRow = new QHBoxLayout();
        btnRow->setSpacing(15);

        QPushButton *closeBtn = new QPushButton("Այո, փակել");
        closeBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #0f3460;
                color: #eaeaea;
                border: 2px solid #16213e;
                border-radius: 8px;
                font-size: 11pt;
                font-weight: bold;
                padding: 9px 18px;
            }
            QPushButton:hover { background-color: #16213e; }
        )");

        QPushButton *retryBtn = new QPushButton("Կրկին փորձել");
        retryBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #00d4aa;
                color: #1a1a2e;
                border: none;
                border-radius: 8px;
                font-size: 11pt;
                font-weight: bold;
                padding: 9px 18px;
            }
            QPushButton:hover { background-color: #00b894; }
        )");

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
        // 5 stars — just a close button
        QPushButton *closeBtn = new QPushButton("Փակել");
        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addStretch();
        btnLayout->addWidget(closeBtn);
        btnLayout->addStretch();
        layout->addLayout(btnLayout);
        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    }

    // Պահպանել արդյունքը դատաբազայում
    ResultSaver::save("library", stars);

    dialog->exec();
    delete dialog;

    if (shouldRetry) {
        // Reset without confirmation dialog
        currentState.initialize();
        commandHistory.clear();
        commandHistoryList->clear();
        outputText->clear();
        commandInput->clear();
        updateDisplay();
        updateBooksTable();
        logOutput("Համակարգը վերականգնված է։ Կրկին փորձեք!");
    } else {
        QApplication::quit();
    }
}
