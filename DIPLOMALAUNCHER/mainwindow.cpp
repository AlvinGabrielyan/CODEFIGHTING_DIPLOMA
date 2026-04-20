// #include "mainwindow.h"
// #include <QApplication>
// #include <QScreen>
// #include <QMessageBox>
// #include <QSqlError>
// #include <QTimer>
// #include <QPainter>
// #include <QPainterPath>
// #include <memory>

// static const QString DB_PATH = []() -> QString {
//     QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
//     dir += "/DiplomaApp";
//     QDir().mkpath(dir);
//     return dir + "/results.db";
// }();
// static const QString DB_CONN = "diploma_shared";

// QList<MainWindow::ProjectInfo> MainWindow::allProjects()
// {
//     const QString dir = QCoreApplication::applicationDirPath() + "/";
//     return {
//             { "bucket",  "Դույլերի Խնդիր",         "🪣",
//              dir + "diploma_bucket_problem.exe", 1 },
//             { "library", "Գրադարանավարի Խնդիր",      "📚",
//              dir + "diploma_library_problem.exe", 1 },
//             { "bee",     "Մեղվի Խնդիր",          "🐝",
//              dir + "diploma_bee_problem.exe", 2 },
//             { "space",   "Տիեզերական Հետախույզ", "🚀",
//              dir + "diploma_SpaceExploreEngine_problem.exe", 2 },
//             { "ship",    "Նավերի Խնդիր",         "⛵",
//              dir + "diploma_ship_problem.exe", 3 },
//             { "maze",    "Լաբիրինթի Խնդիր",      "🌀",
//              dir + "diploma_maze_problem.exe", 3 },
//             };
// }

// bool MainWindow::isLevelComplete(int level)
// {
//     for (auto &p : allProjects())
//         if (p.level == level && getStars(p.id) < 1)
//             return false;
//     return true;
// }

// void MainWindow::updateLevelButtons()
// {
//     auto applyState = [](QPushButton *btn, bool unlocked) {
//         if (!btn) return;
//         QLabel *arr = btn->findChild<QLabel*>("arrowLabel");
//         if (arr) arr->setText(unlocked ? "→" : "—");
//     };
//     applyState(m_levelBtn[2], isLevelComplete(1));
//     applyState(m_levelBtn[3], isLevelComplete(2));
// }

// void MainWindow::showLevelCompleteDialog(int completedLevel)
// {
//     // Timer-ը stop անել — dialog-ի ընթացքում կրկին չկանչվի
//     if (m_pollTimer) m_pollTimer->stop();

//     if (completedLevel >= 3) {
//         QMessageBox box(this);
//         box.setWindowTitle("Կոդամարտ — Ավարտ");
//         box.setText("<h2 style='color:#4d9fff;font-family:'Segoe UI','Arial',sans-serif;'>ԱՄԲՈՂՋԱԿԱՆ ՀԱՂԹԱՆԱԿ</h2>"
//                     "<p style='color:#c8d4f0;font-family:'Segoe UI','Arial',sans-serif;'>Դուք հաջողությամբ անցել եք բոլոր 3 մակարդակները</p>"
//                     "<p style='color:#6a8a6a;font-size:12px;'>Բոլոր 6 ալգորիթմական խնդիրները լուծված են</p>");
//         box.setStyleSheet("QMessageBox{background:#091020;} QLabel{color:#c8d4f0;}"
//                           "QPushButton{background:#0f2040;border:1px solid #4d9fff;border-radius:6px;"
//                           "color:#4d9fff;padding:8px 20px;font-family:'Segoe UI','Arial',sans-serif;}"
//                           "QPushButton:hover{background:#4d9fff;color:#091020;}");
//         QPushButton *progressBtn = box.addButton("Տեսնել Արդյունքները", QMessageBox::AcceptRole);
//         box.addButton("Փակել", QMessageBox::RejectRole);
//         box.exec();
//         if (box.clickedButton() == progressBtn) showProgress();
//         return;
//     }

//     int nextLevel = completedLevel + 1;
//     QStringList levelNames = { "", "Առաջին", "Երկրորդ", "Երրորդ" };
//     QMessageBox box(this);
//     box.setWindowTitle(QString("Մակարդակ %1 — Ավարտ").arg(completedLevel));
//     box.setText(QString(
//                     "<h2 style='color:#4d9fff;font-family:'Segoe UI','Arial',sans-serif;'>ՄԱԿԱՐԴԱԿ %1 — ԱՎԱՐՏ</h2>"
//                     "<p style='color:#c8d4f0;'>Անցնե՞նք <b style='color:#f0a500;'>%2-րդ մակարդակ</b>-ին։</p>"
//                     ).arg(completedLevel).arg(nextLevel));
//     box.setStyleSheet("QMessageBox{background:#091020;} QLabel{color:#c8d4f0;}"
//                       "QPushButton{background:#0f2040;border:1px solid #4d9fff;border-radius:6px;"
//                       "color:#4d9fff;padding:8px 20px;font-family:'Segoe UI','Arial',sans-serif;}"
//                       "QPushButton:hover{background:#4d9fff;color:#091020;}");
//     QPushButton *yesBtn = box.addButton(
//         QString("Այո, %1-րդ մակարդակ →").arg(nextLevel), QMessageBox::AcceptRole);
//     box.addButton("Ոչ", QMessageBox::RejectRole);
//     box.exec();
//     updateLevelButtons();
//     if (box.clickedButton() == yesBtn) showLevel(nextLevel);
// }

// MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
// {
//     setWindowTitle("Կոդամարտ — Ալգորիթմական Մարտ");
//     setMinimumSize(1020, 700);
//     resize(1060, 720);

//     QScreen *s = QApplication::primaryScreen();
//     QRect sg = s->availableGeometry();
//     move((sg.width() - width()) / 2, (sg.height() - height()) / 2);

//     applyGlobalStyle();
//     initDatabase();

//     QWidget *central = new QWidget(this);
//     setCentralWidget(central);
//     QVBoxLayout *ml = new QVBoxLayout(central);
//     ml->setContentsMargins(0, 0, 0, 0);
//     ml->setSpacing(0);

//     stackedWidget = new QStackedWidget(this);
//     stackedWidget->addWidget(createHomePage());
//     stackedWidget->addWidget(createLevelPage(1));
//     stackedWidget->addWidget(createLevelPage(2));
//     stackedWidget->addWidget(createLevelPage(3));

//     m_progressPage = new QWidget();
//     m_progressPage->setStyleSheet("background:#070d18;");
//     rebuildProgressPage();
//     stackedWidget->addWidget(m_progressPage);

//     stackedWidget->setCurrentIndex(0);
//     ml->addWidget(stackedWidget);
//     updateLevelButtons();
// }

// MainWindow::~MainWindow()
// {
//     if (m_pollTimer) m_pollTimer->stop();
//     QSqlDatabase::removeDatabase(DB_CONN);
// }

// // database
// bool MainWindow::initDatabase()
// {
//     QDir().mkpath(QFileInfo(DB_PATH).absolutePath());
//     if (!QSqlDatabase::contains(DB_CONN)) {
//         QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", DB_CONN);
//         db.setDatabaseName(DB_PATH);
//     }
//     QSqlDatabase db = QSqlDatabase::database(DB_CONN);
//     if (!db.open()) return false;
//     QSqlQuery q(db);
//     q.exec("CREATE TABLE IF NOT EXISTS results ("
//            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
//            "  project_id TEXT NOT NULL UNIQUE,"
//            "  stars INTEGER NOT NULL CHECK(stars BETWEEN 1 AND 5),"
//            "  saved_at TEXT NOT NULL)");
//     return true;
// }

// int MainWindow::getStars(const QString &projectId)
// {
//     QSqlDatabase db = QSqlDatabase::database(DB_CONN);
//     if (db.isOpen()) db.close();
//     if (!db.open())  return -1;
//     QSqlQuery q(db);
//     q.exec("PRAGMA wal_checkpoint(PASSIVE)");
//     q.prepare("SELECT stars FROM results WHERE project_id = :pid ORDER BY id DESC LIMIT 1");
//     q.bindValue(":pid", projectId);
//     q.exec();
//     return q.next() ? q.value(0).toInt() : -1;
// }

// QString MainWindow::getGradeLetter(double avg)
// {
//     if (avg >= 4.5) return "A+";
//     if (avg >= 4.0) return "A";
//     if (avg >= 3.5) return "B+";
//     if (avg >= 3.0) return "B";
//     if (avg >= 2.5) return "C+";
//     if (avg >= 2.0) return "C";
//     return "D";
// }

// QString MainWindow::getGradeColor(double avg)
// {
//     if (avg >= 4.0) return "#4d9fff";
//     if (avg >= 3.0) return "#f0a500";
//     if (avg >= 2.0) return "#fb923c";
//     return "#f87171";
// }

// void MainWindow::launchApp(const QString &exePath, const QString &appName,
//                            const QString &projectId, int level)
// {
//     QFileInfo fi(exePath);
//     if (!fi.exists()) {
//         QMessageBox::warning(this, "Ֆայլ չի գտնվել",
//                              QString("Հնարավոր չէ գտնել՝\n%1").arg(exePath));
//         return;
//     }
//     int starsBefore = getStars(projectId);
//     QProcess::startDetached(exePath, QStringList(), fi.absolutePath());

//     if (!m_pollTimer) {
//         m_pollTimer = new QTimer(this);
//         m_pollTimer->setInterval(2000);
//     }
//     struct PollState { int starsBefore; int level; bool triggered = false; };
//     auto state = std::make_shared<PollState>();
//     state->starsBefore = starsBefore;
//     state->level = level;

//     QObject *ctx = new QObject(this);
//     connect(m_pollTimer, &QTimer::timeout, ctx,
//             [this, projectId, state, ctx]() mutable {
//                 if (state->triggered) return;
//                 int fresh = getStars(projectId);
//                 if (fresh > state->starsBefore) {
//                     state->triggered = true;
//                     ctx->deleteLater();
//                     onAppFinished(projectId, state->level);
//                 }
//             });
//     if (!m_pollTimer->isActive()) m_pollTimer->start();
// }

// void MainWindow::onAppFinished(const QString &projectId, int level)
// {
//     int stars = getStars(projectId);
//     if (stars < 1) return;
//     updateLevelButtons();

//     auto projects = allProjects();
//     QList<ProjectInfo> levelProjects;
//     for (auto &p : projects)
//         if (p.level == level) levelProjects.append(p);
//     if (levelProjects.size() < 2) return;

//     bool firstSolved  = getStars(levelProjects[0].id) >= 1;
//     bool secondSolved = getStars(levelProjects[1].id) >= 1;

//     if (firstSolved && secondSolved) {
//         showLevelCompleteDialog(level);
//     } else if (projectId == levelProjects[0].id && !secondSolved) {
//         if (level == 3) {
//             QTimer::singleShot(400, this, [this, levelProjects]() {
//                 QDialog *dlg = new QDialog(this);
//                 dlg->setWindowTitle("Հաջողություն — Նավերի Խնդիր");
//                 dlg->setFixedSize(480, 220);
//                 QPalette pal = dlg->palette();
//                 pal.setColor(QPalette::Window, QColor("#091020"));
//                 dlg->setPalette(pal); dlg->setAutoFillBackground(true);
//                 dlg->setStyleSheet(
//                     "QLabel { color: #c8d4f0; background: transparent; }"
//                     "QPushButton { font-size:13px; font-weight:bold; padding:10px 28px;"
//                     "  border-radius:8px; border:none; }"
//                     );

//                 QVBoxLayout *vl = new QVBoxLayout(dlg);
//                 vl->setContentsMargins(36, 28, 36, 28); vl->setSpacing(12);

//                 QLabel *title = new QLabel(" Նավերի Խնդիրն ավարտված է!");
//                 title->setAlignment(Qt::AlignCenter);
//                 title->setStyleSheet("font-size:16px;font-weight:700;color:#f76a8e;background:transparent;");
//                 vl->addWidget(title);

//                 QLabel *msg = new QLabel("Ուզո՞ւմ եք անցնել վերջին խնդրին(Լաբիրինթ), թե կրկին փորձե՞լ:");
//                 msg->setAlignment(Qt::AlignCenter);
//                 msg->setWordWrap(true);
//                 vl->addWidget(msg);

//                 vl->addStretch();

//                 QHBoxLayout *bl = new QHBoxLayout();
//                 bl->setSpacing(12);

//                 QPushButton *retryBtn = new QPushButton("↺  Կրկին փորձել");
//                 retryBtn->setStyleSheet(
//                     "QPushButton { background:#1a2d50; color:#c8d4f0; border:1px solid #2a4070; }"
//                     "QPushButton:hover { background:#2a3f6a; }");
//                 connect(retryBtn, &QPushButton::clicked, dlg, &QDialog::reject);

//                 QPushButton *continueBtn = new QPushButton("→  Անցնել Լաբիրինթին");
//                 continueBtn->setStyleSheet(
//                     "QPushButton { background:#f76a8e; color:#070d18; }"
//                     "QPushButton:hover { background:#ff8aaa; }");
//                 connect(continueBtn, &QPushButton::clicked, dlg, &QDialog::accept);

//                 bl->addStretch();
//                 bl->addWidget(retryBtn);
//                 bl->addWidget(continueBtn);
//                 bl->addStretch();
//                 vl->addLayout(bl);

//                 int res = dlg->exec();
//                 delete dlg;

//                 if (res == QDialog::Accepted) {
//                     launchApp(levelProjects[1].exe, levelProjects[1].name,
//                               levelProjects[1].id, levelProjects[1].level);
//                 }

//             });
//         } else {
//             QTimer::singleShot(600, this, [this, levelProjects]() {
//                 QMessageBox::information(this, "Հաջողություն",
//                                          QString("«%1»-ը լուծված է!\n\nՀիմա կբացվի «%2»-ը։")
//                                              .arg(levelProjects[0].name).arg(levelProjects[1].name));
//                 launchApp(levelProjects[1].exe, levelProjects[1].name,
//                           levelProjects[1].id, levelProjects[1].level);
//             });
//         }
//     }
// }

// void MainWindow::applyGlobalStyle()
// {
//     setStyleSheet(R"(
//         QMainWindow, QWidget {
//             background-color: #070d18;
//             color: #c8d4f0;
//             font-family: 'Segoe UI', 'Arial Rounded MT Bold', 'Arial', sans-serif;
//         }
//         QScrollArea { border:none; background:transparent; }
//         QScrollBar:vertical {
//             background:#0d1628;
//             width:5px;
//             border-radius:2px;
//         }
//         QScrollBar::handle:vertical {
//             background:#2a3f6a;
//             border-radius:2px;
//         }
//         QToolTip {
//             background:#0f2040;
//             color:#4d9fff;
//             border:1px solid #4d9fff;
//             border-radius:4px;
//             padding:4px 8px;
//             font-size:11px;
//         }
//     )");
// }

// // home page
// QWidget* MainWindow::createHomePage()
// {
//     QWidget *page = new QWidget();
//     page->setStyleSheet("background:#070d18;");

//     QVBoxLayout *lay = new QVBoxLayout(page);
//     lay->setContentsMargins(0, 0, 0, 0);
//     lay->setSpacing(0);

//     // TOP HEADER BAR
//     QWidget *header = new QWidget();
//     header->setFixedHeight(64);
//     header->setStyleSheet(
//         "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
//         "stop:0 #091020, stop:0.5 #0d1a30, stop:1 #091020);"
//         "border-bottom:1px solid #1a2d50;");
//     QHBoxLayout *hl = new QHBoxLayout(header);
//     hl->setContentsMargins(36, 0, 36, 0);

//     // Logo center
//     hl->addStretch();
//     QWidget *logoW = new QWidget();
//     logoW->setStyleSheet("background:transparent;");
//     {
//         QHBoxLayout *ll = new QHBoxLayout(logoW);
//         ll->setContentsMargins(0,0,0,0); ll->setSpacing(10);

//         QLabel *dot1 = new QLabel("◈");
//         dot1->setStyleSheet("color:#4d9fff;font-size:18px;background:transparent;");

//         QLabel *logoText = new QLabel("ԿՈԴԱՄԱՐՏ");
//         logoText->setStyleSheet(
//             "color:#ffffff;font-size:20px;font-weight:700;"
//             "letter-spacing:10px;background:transparent;"
//             "font-family:'Segoe UI','Arial',sans-serif;");

//         QLabel *dot2 = new QLabel("◈");
//         dot2->setStyleSheet("color:#4d9fff;font-size:18px;background:transparent;");

//         ll->addWidget(dot1);
//         ll->addWidget(logoText);
//         ll->addWidget(dot2);
//     }
//     hl->addWidget(logoW);
//     hl->addStretch();

//     lay->addWidget(header);

//     // MAIN CONTENT
//     QWidget *content = new QWidget();
//     content->setStyleSheet("background:transparent;");
//     QHBoxLayout *contentLayout = new QHBoxLayout(content);
//     contentLayout->setContentsMargins(40, 32, 40, 32);
//     contentLayout->setSpacing(32);

//     //  LEFT COLUMN: Mission statement
//     QWidget *leftPanel = new QWidget();
//     leftPanel->setFixedWidth(400);
//     leftPanel->setStyleSheet(
//         "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
//         "stop:0 #0d1a30,stop:1 #070d18);"
//         "border:1px solid #1a2d50;"
//         "border-radius:16px;");
//     QVBoxLayout *leftLay = new QVBoxLayout(leftPanel);
//     leftLay->setContentsMargins(24, 28, 24, 28);
//     leftLay->setSpacing(16);

//     QLabel *missionTag = new QLabel("ՀԱՄԱԿԱՐԳԻ ՄԱՍԻՆ");
//     missionTag->setStyleSheet(
//         "color:#4d9fff;font-size:10px;font-weight:700;"
//         "letter-spacing:3px;background:transparent;"
//         "font-family:'Segoe UI','Arial',sans-serif;");
//     leftLay->addWidget(missionTag);

//     QFrame *mSep = new QFrame();
//     mSep->setFrameShape(QFrame::HLine);
//     mSep->setStyleSheet("background:#1a2d50;max-height:1px;border:none;");
//     leftLay->addWidget(mSep);

//     QScrollArea *descScroll = new QScrollArea();
//     descScroll->setWidgetResizable(true);
//     descScroll->setFrameShape(QFrame::NoFrame);
//     descScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//     descScroll->setStyleSheet(
//         "QScrollArea { background:transparent; border:none; }"
//         "QScrollBar:vertical { background:#0d1628; width:4px; border-radius:2px; }"
//         "QScrollBar::handle:vertical { background:#2a3f6a; border-radius:2px; }"
//         "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; }");

//     QWidget *descW = new QWidget();
//     descW->setStyleSheet("background:transparent;");
//     QVBoxLayout *descLay = new QVBoxLayout(descW);
//     descLay->setContentsMargins(4, 8, 4, 8);
//     descLay->setSpacing(0);

//     QLabel *descLbl = new QLabel();
//     descLbl->setWordWrap(true);
//     descLbl->setTextFormat(Qt::RichText);
//     descLbl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//     descLbl->setStyleSheet(
//         "color:#a0b4d8;font-size:12px;line-height:200%;"
//         "background:transparent;padding:4px 2px;"
//         "font-family:'Segoe UI','Arial',sans-serif;");
//     descLbl->setText("<p>Ողջույն, Դուք մուտք եք գործել <Կոդամարտ> համակարգ, որը կօգնի Ձեզ՝ սկսնակ ծրագրավորողներիդ հասկանալ, արդյո՞ք կարող եք առաջադրված խնդիրներին տալ լուծումներ՝մշալելով լուծման ալգորիթմներ՝ունենալով միայն տրված խնդրի համար սահմանված սահմանափակ քանակի կոնկրետ հրամաններ։ Պայմանները շատ պարզ են, պետք է նախապես հասկանալ խնդրի լուծմանը հասնելու քայլերի հերթականությունը, մուտքագրել հրամանները հրամանների մատյանում, հետևել տվյալ խնդրի համար սահմանված կանոններին ու սահմանափակումներին, իրականացնել Ձեր գրած կոդի հատվածը և տեսնել կատարված քայլերը վիզուալ արտապատկերման միջոցով, ինչը կպարզեցնի ձեր՝ ինչպես սխալը ուղղելու փորձերը, այնպես էլ կատարված քայլերի իրականացման պատկերացումները։Դե ինչ, սկսեք, հարգելի օգտատեր, Ձեզ հաջողություն եմ մաղթում։ </p>");
//     descLay->addWidget(descLbl);
//     descLay->addStretch();
//     descScroll->setWidget(descW);
//     leftLay->addWidget(descScroll, 1);

//     auto projects = allProjects();
//     int completedProjects = 0;
//     int totalStarsEarned  = 0;
//     for (auto &p : projects) {
//         int s = getStars(p.id);
//         if (s > 0) { completedProjects++; totalStarsEarned += s; }
//     }

//     QWidget *statsCard = new QWidget();
//     statsCard->setStyleSheet(
//         "background:#0d1a30;border:1px solid #2a3f6a;border-radius:10px;");
//     QHBoxLayout *sl = new QHBoxLayout(statsCard);
//     sl->setContentsMargins(16, 12, 16, 12); sl->setSpacing(0);

//     auto addStat = [&](const QString &val, const QString &lbl) {
//         QWidget *sw = new QWidget(); sw->setStyleSheet("background:transparent;");
//         QVBoxLayout *svl = new QVBoxLayout(sw);
//         svl->setContentsMargins(0,0,0,0); svl->setSpacing(2);
//         QLabel *v = new QLabel(val);
//         v->setAlignment(Qt::AlignCenter);
//         v->setStyleSheet("color:#4d9fff;font-size:22px;font-weight:900;background:transparent;");
//         QLabel *l = new QLabel(lbl);
//         l->setAlignment(Qt::AlignCenter);
//         l->setStyleSheet("color:#3a5080;font-size:10px;letter-spacing:1px;background:transparent;");
//         svl->addWidget(v); svl->addWidget(l);
//         return sw;
//     };

//     sl->addWidget(addStat(QString::number(completedProjects), "ԱՎԱՐՏՎԱԾ"));
//     QFrame *vSep = new QFrame(); vSep->setFrameShape(QFrame::VLine);
//     vSep->setStyleSheet("background:#1a2d50;max-width:1px;border:none;");
//     sl->addWidget(vSep);
//     sl->addWidget(addStat(QString::number(totalStarsEarned) + "/30", "ԱՍՏՂ"));
//     QFrame *vSep2 = new QFrame(); vSep2->setFrameShape(QFrame::VLine);
//     vSep2->setStyleSheet("background:#1a2d50;max-width:1px;border:none;");
//     sl->addWidget(vSep2);
//     sl->addWidget(addStat(completedProjects > 0 ? getGradeLetter((double)totalStarsEarned/6.0) : "—", "ԳՆԱՀԱՏ."));
//     leftLay->addWidget(statsCard);

//     contentLayout->addWidget(leftPanel);

//     QWidget *rightPanel = new QWidget();
//     rightPanel->setStyleSheet("background:transparent;");
//     QVBoxLayout *rightLay = new QVBoxLayout(rightPanel);
//     rightLay->setContentsMargins(0, 0, 0, 0);
//     rightLay->setSpacing(20);

//     // Level buttons — stacked vertically for this layout
//     m_levelBtn[1] = createLevelButton("ՄԱԿԱՐԴԱԿ 1", "Դույլ  ·  Գրադարան", 1);
//     m_levelBtn[2] = createLevelButton("ՄԱԿԱՐԴԱԿ 2", "Մեղու  ·  Տիեզերք",  2);
//     m_levelBtn[3] = createLevelButton("ՄԱԿԱՐԴԱԿ 3", "Լաբիրինթ  ·  Նավ",   3);

//     rightLay->addWidget(m_levelBtn[1]);
//     rightLay->addWidget(m_levelBtn[2]);
//     rightLay->addWidget(m_levelBtn[3]);

//     // Progress button
//     QPushButton *progBtn = new QPushButton(" ԴԻՏԵԼ ԱՐԴՅՈՒՆՔՆԵՐԸ ԵՎ ԳՆԱՀԱՏԱԿԱՆԸ");
//     progBtn->setCursor(Qt::PointingHandCursor);
//     progBtn->setFixedHeight(46);
//     progBtn->setStyleSheet(R"(
//         QPushButton {
//             background:#0d1a30;
//             border:1px solid #2a3f6a;
//             border-radius:10px;
//             color:#4a6090;
//             font-size:13px;
//             font-weight:700;
//             letter-spacing:1px;
//         }
//         QPushButton:hover {
//             border-color:#4d9fff;
//             color:#4d9fff;
//             background:#0f2040;
//         }
//     )");
//     connect(progBtn, &QPushButton::clicked, this, &MainWindow::showProgress);
//     rightLay->addWidget(progBtn);

//     contentLayout->addWidget(rightPanel);
//     lay->addWidget(content, 1);


//     return page;
// }

// // level button (horizontal card style)
// QPushButton* MainWindow::createLevelButton(const QString &title, const QString &subtitle, int level)
// {
//     QPushButton *btn = new QPushButton();
//     btn->setFixedHeight(100);
//     btn->setCursor(Qt::PointingHandCursor);

//     QString cols[]    = { "", "#4d9fff", "#f0a500", "#f76a8e" };
//     QString dimCols[] = { "", "#1a2a50", "#1a2a50", "#1a1a40" };
//     QString col    = cols[level];
//     QString dimCol = dimCols[level];

//     btn->setStyleSheet(QString(R"(
//         QPushButton {
//             background:qlineargradient(x1:0,y1:0,x2:1,y2:0,
//                 stop:0 #0d1a30, stop:1 #070d18);
//             border:1px solid #1a2d50;
//             border-radius:12px;
//         }
//         QPushButton:hover {
//             background:qlineargradient(x1:0,y1:0,x2:1,y2:0,
//                 stop:0 %2, stop:1 #0d1a30);
//             border:1px solid %1;
//         }
//         QPushButton:pressed { background:#070d18; }
//     )").arg(col).arg(dimCol));

//     QWidget *inner = new QWidget(btn);
//     inner->setAttribute(Qt::WA_TransparentForMouseEvents);
//     inner->setGeometry(0, 0, btn->width(), 100);
//     inner->setStyleSheet("background:transparent;");
//     inner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

//     QObject::connect(btn, &QPushButton::destroyed, []{});
//     btn->installEventFilter(new QObject(btn));

//     QHBoxLayout *hl = new QHBoxLayout(inner);
//     hl->setContentsMargins(28, 0, 28, 0);
//     hl->setSpacing(20);

//     QLabel *numBadge = new QLabel(QString::number(level));
//     numBadge->setFixedSize(52, 52);
//     numBadge->setAlignment(Qt::AlignCenter);
//     numBadge->setStyleSheet(QString(
//                                 "background:rgba(%1,0.15);"
//                                 "color:%2;"
//                                 "border:1px solid %3;"
//                                 "border-radius:26px;"
//                                 "font-size:20px;"
//                                 "font-weight:900;")
//                                 .arg(level==1?"77,159,255":level==2?"240,165,0":"247,106,142")
//                                 .arg(col).arg(col));
//     hl->addWidget(numBadge);

//     // Middle: title + subtitle + stars
//     QWidget *mid = new QWidget(); mid->setStyleSheet("background:transparent;");
//     QVBoxLayout *ml = new QVBoxLayout(mid);
//     ml->setContentsMargins(0,0,0,0); ml->setSpacing(4);

//     QLabel *t = new QLabel(title);
//     t->setStyleSheet(QString("color:#ffffff;font-size:15px;font-weight:700;"
//                              "letter-spacing:2px;background:transparent;"));
//     ml->addWidget(t);

//     auto lProjects = allProjects();
//     int levelStars = 0, levelMax = 0;
//     for (auto &p : lProjects) {
//         if (p.level == level) {
//             int st = getStars(p.id);
//             if (st > 0) levelStars += st;
//             levelMax += 5;
//         }
//     }
//     if (levelMax > 0) {
//         QString starsStr;
//         for (int i = 0; i < levelMax; i++) starsStr += (i < levelStars) ? "★" : "☆";
//         QLabel *starsLbl = new QLabel(starsStr);
//         starsLbl->setStyleSheet(levelStars > 0
//                                     ? QString("color:%1;font-size:13px;background:transparent;").arg(col)
//                                     : "color:#1a2d50;font-size:13px;background:transparent;");
//         ml->addWidget(starsLbl);
//     }
//     hl->addWidget(mid, 1);

//     QWidget *right = new QWidget(); right->setStyleSheet("background:transparent;");
//     QVBoxLayout *rl = new QVBoxLayout(right);
//     rl->setContentsMargins(0,0,0,0); rl->setSpacing(6); rl->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

//     QStringList diffLabels = {"", "Հեշտ", "Միջին Բարդության", "Բարդ"};
//     QLabel *diff = new QLabel(diffLabels[level]);
//     diff->setStyleSheet(QString(
//                             "color:%1;font-size:9px;font-weight:700;letter-spacing:2px;"
//                             "background:rgba(%2,0.12);border:1px solid %3;"
//                             "border-radius:4px;padding:2px 6px;")
//                             .arg(col)
//                             .arg(level==1?"77,159,255":level==2?"240,165,0":"247,106,142")
//                             .arg(col));
//     rl->addWidget(diff, 0, Qt::AlignRight);

//     QLabel *arr = new QLabel("→");
//     arr->setObjectName("arrowLabel");
//     arr->setStyleSheet(QString("color:%1;font-size:20px;background:transparent;").arg(col));
//     rl->addWidget(arr, 0, Qt::AlignRight);
//     hl->addWidget(right);

//     connect(btn, &QPushButton::clicked, this, [this, level]() { showLevel(level); });

//     btn->setLayout(new QVBoxLayout());
//     btn->layout()->setContentsMargins(0,0,0,0);
//     btn->layout()->addWidget(inner);

//     return btn;
// }

// // level page
// QWidget* MainWindow::createLevelPage(int level)
// {
//     QWidget *page = new QWidget();
//     page->setStyleSheet("background:#070d18;");
//     QVBoxLayout *lay = new QVBoxLayout(page);
//     lay->setContentsMargins(0, 0, 0, 0); lay->setSpacing(0);

//     // Header bar
//     QWidget *header = new QWidget();
//     header->setFixedHeight(56);
//     header->setStyleSheet("background:#091020;border-bottom:1px solid #1a2d50;");
//     QHBoxLayout *hl = new QHBoxLayout(header);
//     hl->setContentsMargins(36, 0, 36, 0);

//     QPushButton *back = new QPushButton("← ՀԵՏ");
//     back->setCursor(Qt::PointingHandCursor); back->setFixedSize(80, 30);
//     back->setStyleSheet("QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
//                         "border-radius:6px;color:#3a5080;font-size:11px;font-weight:700;"
//                         "letter-spacing:1px;}"
//                         "QPushButton:hover{border-color:#4d9fff;color:#4d9fff;}");
//     connect(back, &QPushButton::clicked, this, &MainWindow::showHome);
//     hl->addWidget(back);
//     hl->addSpacing(20);

//     // Breadcrumb
//     QLabel *logoSmall = new QLabel("< ԿՈԴԱՄԱՐՏ />");
//     logoSmall->setStyleSheet("color:#2a3f6a;font-size:13px;font-weight:700;background:transparent;");
//     hl->addWidget(logoSmall);
//     hl->addStretch();

//     QString cols[] = { "", "#4d9fff", "#f0a500", "#f76a8e" };
//     QString col = cols[level];
//     QLabel *lvlTag = new QLabel(QString("ՄԱԿԱՐԴԱԿ %1").arg(QString::number(level).rightJustified(2,'0')));
//     lvlTag->setStyleSheet(QString("color:%1;font-size:11px;font-weight:700;"
//                                   "letter-spacing:3px;background:transparent;").arg(col));
//     hl->addWidget(lvlTag);
//     lay->addWidget(header);

//     // Content
//     QWidget *content = new QWidget();
//     content->setStyleSheet("background:transparent;");
//     QVBoxLayout *cl = new QVBoxLayout(content);
//     cl->setContentsMargins(48, 36, 48, 36); cl->setSpacing(0);

//     QStringList titles = { "", "ՀԻՄՆԱՐԱՐ ՊԱՐԶ ԱԼԳՈՐԻԹՄՆԵՐ", "ՕՊՏԻՄԱԼԱՑՄԱՆ ԽՆԴԻՐՆԵՐ", "ՈՒՂՈՒ ՈՐՈՆՈՒՄ ԵՎ ՄՈԴԵԼԱՎՈՐՈՒՄ" };
//     QLabel *title = new QLabel(titles[level]);
//     title->setStyleSheet("font-size:28px;font-weight:900;color:#ffffff;"
//                          "letter-spacing:3px;background:transparent;");
//     cl->addWidget(title);
//     cl->addSpacing(6);
//     QLabel *sub = new QLabel("Ընտրեք առաջադրանքը՝ սիմուլյատորը գործարկելու համար");
//     sub->setStyleSheet("color:#2a3f6a;font-size:13px;background:transparent;");
//     cl->addWidget(sub);
//     cl->addSpacing(32);

//     QWidget *cards = new QWidget(); cards->setStyleSheet("background:transparent;");
//     QHBoxLayout *cardsLay = new QHBoxLayout(cards);
//     cardsLay->setSpacing(24); cardsLay->setContentsMargins(0, 0, 0, 0);

//     struct Proj { QString name, desc, icon, exe, id; };
//     QList<Proj> projects;
//     const QString dir = QCoreApplication::applicationDirPath() + "/";
//     if (level == 1) {
//         projects = {
//             { "Դույլերի Խնդիր",    "Ջրի լցման գլուխկոտրուկ", "🪣",
//              dir + "diploma_bucket_problem.exe", "bucket" },
//             { "Գրադարանավարի Խնդիր", "Գրքերի դասավորում և\nժամանակացույցի օպտիմալացում", "📚",
//              dir + "diploma_library_problem.exe", "library" }
//         };
//     } else if (level == 2) {
//         projects = {
//             { "Մեղուների Խնդիր",         "Մեղուների նեկտարահավաքի ուղու\nօպտիմալացման ", "🐝",
//              dir + "diploma_bee_problem.exe", "bee" },
//             { "Տիեզերագնացների խնդիր",   "Տիեզերական հետախուզման\nուղու մոդելավորում", "🚀",
//              dir + "diploma_SpaceExploreEngine_problem.exe", "space" }
//         };
//     } else {
//         projects = {
//             { "Նավերի Խնդիր",           "Փրկարարական նավերի \nռազմավարության մշակում",    "⛵",
//              dir + "diploma_ship_problem.exe", "ship" },
//             { "Լաբիրինթի Խնդիր",        "Կախարդական լաբիրինթի ելքային \nուղու որոնում", "🌀",
//              dir + "diploma_maze_problem.exe", "maze" }
//         };
//     }

//     for (auto &p : projects)
//         cardsLay->addWidget(createAppButton(p.name, p.desc, p.icon, p.exe, p.id, level));
//     cl->addWidget(cards);
//     cl->addStretch();

//     {
//         int earned = 0;
//         for (auto &p : projects) {
//             int s = getStars(p.id.isEmpty() ? "" : p.id);
//             if (s > 0) earned += s;
//         }
//         int maxStars = (int)projects.size() * 5;

//         QWidget *progressRow = new QWidget(); progressRow->setStyleSheet("background:transparent;");
//         QHBoxLayout *prl = new QHBoxLayout(progressRow);
//         prl->setContentsMargins(0,0,0,0); prl->setSpacing(12);

//         QLabel *plbl = new QLabel(QString("PROGRESS  %1 / %2 ★").arg(earned).arg(maxStars));
//         plbl->setStyleSheet(QString("color:%1;font-size:10px;letter-spacing:2px;background:transparent;").arg(col));
//         prl->addWidget(plbl);

//         QFrame *bar = new QFrame();
//         bar->setFixedHeight(4);
//         bar->setStyleSheet(QString("background:#1a2d50;border-radius:2px;"));
//         prl->addWidget(bar, 1);

//         cl->addWidget(progressRow);
//     }

//     // Nav dots
//     QWidget *dots = new QWidget(); dots->setStyleSheet("background:transparent;");
//     QHBoxLayout *dl = new QHBoxLayout(dots); dl->setAlignment(Qt::AlignCenter); dl->setSpacing(8);
//     for (int i=1; i<=3; i++) {
//         QLabel *d = new QLabel(); d->setFixedSize(i==level ? 24 : 8, 4);
//         d->setStyleSheet(QString("background:%1;border-radius:2px;")
//                              .arg(i==level ? col : "#1a2d50"));
//         dl->addWidget(d);
//     }
//     cl->addSpacing(12);
//     cl->addWidget(dots);

//     lay->addWidget(content, 1);
//     return page;
// }

// // app button
// QPushButton* MainWindow::createAppButton(const QString &title, const QString &description,
//                                          const QString &icon, const QString &exePath,
//                                          const QString &projectId, int level)
// {
//     QPushButton *btn = new QPushButton();
//     btn->setFixedSize(360, 280);
//     btn->setCursor(Qt::PointingHandCursor);

//     QString cols[] = { "", "#4d9fff", "#f0a500", "#f76a8e" };
//     QString col = cols[level];

//     btn->setStyleSheet(QString(R"(
//         QPushButton {
//             background:qlineargradient(x1:0,y1:0,x2:0,y2:1,
//                 stop:0 #0d1a30, stop:1 #070d18);
//             border:1px solid #1a2d50;
//             border-radius:16px;
//         }
//         QPushButton:hover {
//             background:qlineargradient(x1:0,y1:0,x2:0,y2:1,
//                 stop:0 #102038, stop:1 #0d1a30);
//             border:1px solid %1;
//         }
//         QPushButton:pressed { background:#070d18; }
//     )").arg(col));

//     QWidget *inner = new QWidget(btn);
//     inner->setAttribute(Qt::WA_TransparentForMouseEvents);
//     inner->setGeometry(0, 0, 360, 280);
//     inner->setStyleSheet("background:transparent;");
//     QVBoxLayout *vl = new QVBoxLayout(inner);
//     vl->setContentsMargins(32, 28, 32, 28); vl->setSpacing(0);

//     // Icon + status row
//     QWidget *topRow = new QWidget(); topRow->setStyleSheet("background:transparent;");
//     QHBoxLayout *trl = new QHBoxLayout(topRow); trl->setContentsMargins(0,0,0,0);

//     QLabel *ico = new QLabel(icon);
//     ico->setStyleSheet("font-size:36px;background:transparent;");
//     trl->addWidget(ico);
//     trl->addStretch();

//     int stars = getStars(projectId);
//     if (stars > 0) {
//         QLabel *badge = new QLabel("ԱՎԱՐՏՎԱԾ");
//         badge->setStyleSheet(QString("color:%1;font-size:9px;font-weight:700;"
//                                      "background:rgba(%2,0.15);border:1px solid %3;"
//                                      "border-radius:4px;padding:3px 7px;letter-spacing:1px;")
//                                  .arg(col)
//                                  .arg(level==1?"77,159,255":level==2?"240,165,0":"247,106,142")
//                                  .arg(col));
//         trl->addWidget(badge);
//     }
//     vl->addWidget(topRow);
//     vl->addSpacing(14);

//     QLabel *t = new QLabel(title);
//     t->setStyleSheet("color:#ffffff;font-size:15px;font-weight:700;background:transparent;");
//     vl->addWidget(t);
//     vl->addSpacing(8);

//     QLabel *desc = new QLabel(description);
//     desc->setStyleSheet("color:#3a5080;font-size:12px;background:transparent;"
//                         "font-family:'Segoe UI',sans-serif;");
//     desc->setWordWrap(true);
//     vl->addWidget(desc);
//     vl->addSpacing(16);

//     // Stars display
//     QString starsStr;
//     if (stars < 0) {
//         starsStr = "— — — — —";
//     } else {
//         for (int i = 0; i < 5; i++) starsStr += (i < stars) ? "★ " : "☆ ";
//     }
//     QLabel *starsLbl = new QLabel(starsStr.trimmed());
//     starsLbl->setStyleSheet(stars > 0
//                                 ? QString("color:%1;font-size:17px;background:transparent;").arg(col)
//                                 : "color:#1a2d50;font-size:14px;background:transparent;letter-spacing:4px;");
//     vl->addWidget(starsLbl);
//     vl->addStretch();

//     QFrame *sep = new QFrame(); sep->setFrameShape(QFrame::HLine);
//     sep->setStyleSheet("background:#1a2d50;max-height:1px;border:none;");
//     vl->addWidget(sep);
//     vl->addSpacing(10);

//     QWidget *lr = new QWidget(); lr->setStyleSheet("background:transparent;");
//     QHBoxLayout *lrl = new QHBoxLayout(lr); lrl->setContentsMargins(0,0,0,0);
//     QLabel *lt = new QLabel("ԳՈՐԾԱՐԿԵԼ");
//     lt->setStyleSheet("color:#2a3f6a;font-size:11px;font-weight:700;"
//                       "letter-spacing:2px;background:transparent;");
//     lrl->addWidget(lt); lrl->addStretch();
//     QLabel *arr = new QLabel("→");
//     arr->setStyleSheet(QString("color:%1;font-size:16px;background:transparent;").arg(col));
//     lrl->addWidget(arr);
//     vl->addWidget(lr);

//     // maze-ը locked է, երբ ship-ը չի ավարտվել
//     bool isMaze = (projectId == "maze");
//     bool shipDone = (getStars("ship") >= 1);
//     bool isLocked = isMaze && !shipDone;

//     if (isLocked) {
//         btn->setCursor(Qt::ForbiddenCursor);
//         // Lock overlay
//         QLabel *lockOverlay = new QLabel(btn);
//         lockOverlay->setGeometry(0, 0, 360, 280);
//         lockOverlay->setAlignment(Qt::AlignCenter);
//         lockOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
//         lockOverlay->setStyleSheet(
//             "background:rgba(7,13,24,0.72);border-radius:16px;"
//             "font-size:40px;");
//         lockOverlay->setText("🔒");

//         // "Unlock" hint label at bottom
//         QLabel *hintLbl = new QLabel("Նախ լուծեք Նավերի Խնդիրը", btn);
//         hintLbl->setGeometry(0, 220, 360, 40);
//         hintLbl->setAlignment(Qt::AlignCenter);
//         hintLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
//         hintLbl->setStyleSheet(
//             "background:transparent;color:#f76a8e;"
//             "font-size:11px;font-weight:700;letter-spacing:1px;");

//         btn->setStyleSheet(QString(R"(
//             QPushButton {
//                 background:qlineargradient(x1:0,y1:0,x2:0,y2:1,
//                     stop:0 #0d1a30, stop:1 #070d18);
//                 border:1px solid #1a2d50;
//                 border-radius:16px;
//             }
//         )"));

//         connect(btn, &QPushButton::clicked, this,
//                 [this]() {
//                     QMessageBox::information(this, "🔒 Կողպված է",
//                                              "Լաբիրինթի խնդիրը կբացվի Նավերի Խնդիրը ավարտելուց հետո։");
//                 });
//     } else {
//         connect(btn, &QPushButton::clicked, this,
//                 [this, exePath, title, projectId, level]() {
//                     launchApp(exePath, title, projectId, level);
//                 });
//     }
//     return btn;
// }

// // progress page
// void MainWindow::rebuildProgressPage()
// {
//     if (!m_progressPage) return;

//     QLayout *old = m_progressPage->layout();
//     if (old) {
//         QLayoutItem *item;
//         while ((item = old->takeAt(0)) != nullptr) {
//             if (item->widget()) item->widget()->deleteLater();
//             delete item;
//         }
//         delete old;
//     }

//     QVBoxLayout *lay = new QVBoxLayout(m_progressPage);
//     lay->setContentsMargins(0, 0, 0, 0); lay->setSpacing(0);

//     // Header
//     QWidget *header = new QWidget();
//     header->setFixedHeight(56);
//     header->setStyleSheet("background:#091020;border-bottom:1px solid #1a2d50;");
//     QHBoxLayout *hl = new QHBoxLayout(header);
//     hl->setContentsMargins(36, 0, 36, 0);

//     QPushButton *back = new QPushButton("← ՀԵՏ");
//     back->setCursor(Qt::PointingHandCursor); back->setFixedSize(80, 30);
//     back->setStyleSheet("QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
//                         "border-radius:6px;color:#3a5080;font-size:11px;font-weight:700;}"
//                         "QPushButton:hover{border-color:#4d9fff;color:#4d9fff;}");
//     connect(back, &QPushButton::clicked, this, &MainWindow::showHome);
//     hl->addWidget(back); hl->addSpacing(20);
//     QLabel *logoSmall = new QLabel("< ԿՈԴԱՄԱՐՏ />");
//     logoSmall->setStyleSheet("color:#2a3f6a;font-size:13px;font-weight:700;background:transparent;");
//     hl->addWidget(logoSmall);
//     hl->addStretch();
//     // QPushButton *refresh = new QPushButton("↻  ԹԱՐՄ.");
//     // refresh->setCursor(Qt::PointingHandCursor); refresh->setFixedSize(90, 30);
//     // refresh->setStyleSheet("QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
//     //                        "border-radius:6px;color:#3a5080;font-size:11px;font-weight:700;}"
//     //                        "QPushButton:hover{border-color:#4d9fff;color:#4d9fff;}");
//     // connect(refresh, &QPushButton::clicked, this, &MainWindow::refreshProgress);
//     // hl->addWidget(refresh);
//     lay->addWidget(header);

//     // Scrollable content
//     QScrollArea *scroll = new QScrollArea();
//     scroll->setWidgetResizable(true);
//     scroll->setStyleSheet("QScrollArea{background:transparent;border:none;}");

//     QWidget *contentW = new QWidget();
//     contentW->setStyleSheet("background:#070d18;");
//     QVBoxLayout *cl = new QVBoxLayout(contentW);
//     cl->setContentsMargins(48, 36, 48, 36); cl->setSpacing(0);

//     QLabel *title = new QLabel("ԱՐԴՅՈՒՆՔՆԵՐ ԵՎ ԳՆԱՀԱՏԱԿԱՆ");
//     title->setStyleSheet("font-size:24px;font-weight:900;color:#ffffff;"
//                          "letter-spacing:4px;background:transparent;");
//     cl->addWidget(title);
//     cl->addSpacing(4);
//     QLabel *sub = new QLabel("Ամփոփ տեղեկություն 6 ալգորիթմական առաջադրանքների կատարման վերաբերյալ");
//     sub->setStyleSheet("color:#2a3f6a;font-size:12px;background:transparent;");
//     cl->addWidget(sub);
//     cl->addSpacing(28);

//     // Stats
//     auto projects = allProjects();
//     int totalStars = 0, completedCount = 0;
//     for (auto &p : projects) {
//         int s = getStars(p.id);
//         if (s > 0) { totalStars += s; completedCount++; }
//     }
//     double overallAvg = (double)totalStars / 6.0;
//     QString gradeCol  = getGradeColor(overallAvg);

//     // Grade card
//     QWidget *gradeCard = new QWidget();
//     gradeCard->setFixedHeight(120);
//     gradeCard->setStyleSheet(QString(
//                                  "QWidget { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
//                                  "stop:0 #0d1a30,stop:0.3 #091020,stop:1 #070d18);"
//                                  "border:1px solid %1;border-radius:14px; }"
//                                  "QLabel { border:none;background:transparent; }").arg(gradeCol));
//     QHBoxLayout *gl = new QHBoxLayout(gradeCard);
//     gl->setContentsMargins(36, 20, 36, 20);

//     // Grade letter
//     QLabel *gradeLbl = new QLabel(completedCount > 0 ? getGradeLetter(overallAvg) : "—");
//     gradeLbl->setStyleSheet(QString("color:%1;font-size:58px;font-weight:900;"
//                                     "font-family:'Segoe UI','Arial',sans-serif;background:transparent;").arg(gradeCol));
//     gl->addWidget(gradeLbl);
//     gl->addSpacing(28);

//     QVBoxLayout *gInfo = new QVBoxLayout();
//     QLabel *gTitle = new QLabel("ՎԵՐՋՆԱԿԱՆ ԳՆԱՀԱՏԱԿԱՆ");
//     gTitle->setStyleSheet("color:#ffffff;font-size:13px;font-weight:700;"
//                           "letter-spacing:3px;background:transparent;");
//     gInfo->addWidget(gTitle);

//     QString avgStr = completedCount > 0
//                          ? QString("Միջին՝ %1 / 5.0  ·  Ավարտված՝ %2 / 6 խնդիր")
//                                .arg(overallAvg, 0, 'f', 1).arg(completedCount)
//                          : "Դեռ ոչ մի խնդիր չի ավարտվել";
//     QLabel *gSub = new QLabel(avgStr);
//     gSub->setStyleSheet("color:#3a5080;font-size:12px;background:transparent;");
//     gInfo->addWidget(gSub);

//     QString starsTotal;
//     for (int i = 0; i < 5; i++) starsTotal += (i < qRound(overallAvg)) ? "★ " : "☆ ";
//     QLabel *gStars = new QLabel(starsTotal.trimmed());
//     gStars->setStyleSheet(QString("color:%1;font-size:18px;background:transparent;").arg(gradeCol));
//     gInfo->addWidget(gStars);
//     gl->addLayout(gInfo);
//     gl->addStretch();

//     QLabel *fraction = new QLabel(QString("%1\n/ 30").arg(totalStars));
//     fraction->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//     fraction->setStyleSheet(QString("color:%1;font-size:26px;font-weight:900;"
//                                     "background:transparent;letter-spacing:-1px;").arg(gradeCol));
//     gl->addWidget(fraction);
//     cl->addWidget(gradeCard);
//     cl->addSpacing(32);

//     // Project rows by level
//     QStringList levelNames  = { "", "ՄԱԿԱՐԴԱԿ 1", "ՄԱԿԱՐԴԱԿ 2", "ՄԱԿԱՐԴԱԿ 3" };
//     QString levelColors[]   = { "", "#4d9fff",    "#f0a500",    "#f76a8e" };
//     int lastLevel = 0;

//     for (auto &p : projects) {
//         if (p.level != lastLevel) {
//             lastLevel = p.level;
//             QLabel *lvlHdr = new QLabel(QString("── %1 ──").arg(levelNames[p.level]));
//             lvlHdr->setStyleSheet(QString("color:%1;font-size:10px;font-weight:700;"
//                                           "letter-spacing:3px;background:transparent;"
//                                           "margin-top:8px;").arg(levelColors[p.level]));
//             cl->addWidget(lvlHdr);
//             cl->addSpacing(10);
//         }

//         int s = getStars(p.id);
//         QWidget *row = new QWidget();
//         row->setFixedHeight(68);
//         row->setStyleSheet(QString(
//                                "QWidget { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
//                                "stop:0 #0d1a30,stop:1 #070d18);"
//                                "border:1px solid %1;border-radius:10px; }"
//                                "QLabel { border:none;background:transparent; }")
//                                .arg(s > 0 ? levelColors[p.level] : "#1a2d50"));
//         QHBoxLayout *rl = new QHBoxLayout(row);
//         rl->setContentsMargins(20, 0, 20, 0); rl->setSpacing(16);

//         QLabel *icoLbl = new QLabel(p.icon);
//         icoLbl->setStyleSheet("font-size:11px;font-weight:700;color:#4d9fff;background:transparent;");
//         icoLbl->setFixedWidth(32);
//         rl->addWidget(icoLbl);

//         QLabel *nameLbl = new QLabel(p.name);
//         nameLbl->setStyleSheet("color:#c8d4f0;font-size:14px;font-weight:700;background:transparent;");
//         nameLbl->setFixedWidth(220);
//         rl->addWidget(nameLbl);
//         rl->addStretch();

//         if (s > 0) {
//             QString st;
//             for (int i = 0; i < 5; i++) st += (i < s) ? "★" : "☆";
//             QLabel *stLbl = new QLabel(st);
//             stLbl->setStyleSheet(QString("color:%1;font-size:18px;background:transparent;")
//                                      .arg(levelColors[p.level]));
//             rl->addWidget(stLbl);
//             rl->addSpacing(12);
//             QLabel *numLbl = new QLabel(QString("%1 / 5").arg(s));
//             numLbl->setStyleSheet("color:#3a5080;font-size:12px;background:transparent;");
//             rl->addWidget(numLbl);
//         } else {
//             QLabel *nLbl = new QLabel("ՉԻ ԱՎارՏVELS");
//             nLbl->setText("ՉԻ ԱՎԱՐՏՎԵԼ");
//             nLbl->setStyleSheet("color:#1a2d50;font-size:11px;letter-spacing:2px;background:transparent;");
//             rl->addWidget(nLbl);
//         }
//         cl->addWidget(row);
//         cl->addSpacing(8);
//     }
//     cl->addStretch();

//     scroll->setWidget(contentW);
//     lay->addWidget(scroll, 1);
// }

// // navigation
// void MainWindow::showLevel(int level)
// {
//     if (level == 2 && !isLevelComplete(1)) {
//         QMessageBox box(this);
//         box.setWindowTitle("Մակարդակը կողպված է");
//         box.setStyleSheet("QMessageBox{background:#070d18;} QLabel{color:#c8d4f0;}"
//                           "QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
//                           "border-radius:8px;color:#4d9fff;padding:8px 20px;}"
//                           "QPushButton:hover{background:#4d9fff;color:#070d18;border-color:#4d9fff;}");
//         box.setText("<h3 style='color:#f87171;'>2-րդ մակարդակը կողպված է</h3>"
//                     "<p style='color:#c8d4f0;'>Այս մակարդակի խնդիրները դեռ հասանելի չեն։</p>"
//                     "<p style='color:#3a5080;'>Նախ լուծի՛ր <b style='color:#4d9fff;'>1-ին մակարդակի</b> "
//                     "երկու խնդիրները՝ Դույլ և Գրադարան։</p>");
//         box.addButton("Լավ", QMessageBox::AcceptRole);
//         box.exec();
//         return;
//     }
//     if (level == 3 && !isLevelComplete(2)) {
//         QMessageBox box(this);
//         box.setWindowTitle("Մակարդակը կողպված է");
//         box.setStyleSheet("QMessageBox{background:#070d18;} QLabel{color:#c8d4f0;}"
//                           "QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
//                           "border-radius:8px;color:#f0a500;padding:8px 20px;}"
//                           "QPushButton:hover{background:#f0a500;color:#070d18;border-color:#f0a500;}");
//         box.setText("<h3 style='color:#f87171;'>3-րդ մակարդակը կողպված է</h3>"
//                     "<p style='color:#c8d4f0;'>Այս մակարդակի խնդիրները դեռ հասանելի չեն։</p>"
//                     "<p style='color:#3a5080;'>Նախ լուծի՛ր <b style='color:#f0a500;'>2-րդ մակարդակի</b> "
//                     "երկու խնդիրները՝ Մեղու և Տիեզերք։</p>");
//         box.addButton("Լավ", QMessageBox::AcceptRole);
//         box.exec();
//         return;
//     }
//     stackedWidget->setCurrentIndex(level);
// }

// void MainWindow::showHome()        { stackedWidget->setCurrentIndex(0); }

// void MainWindow::showProgress()    { rebuildProgressPage(); stackedWidget->setCurrentIndex(4); }
// void MainWindow::refreshProgress() { rebuildProgressPage(); }




#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QMessageBox>
#include <QSqlError>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <memory>

static const QString DB_PATH = []() -> QString {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    dir += "/DiplomaApp";
    QDir().mkpath(dir);
    return dir + "/results.db";
}();
static const QString DB_CONN = "diploma_shared";

QList<MainWindow::ProjectInfo> MainWindow::allProjects()
{
    const QString dir = QCoreApplication::applicationDirPath() + "/";
    return {
            { "bucket",  "Դույլերի Խնդիր",         "🪣",
             dir + "diploma_bucket_problem.exe", 1 },
            { "library", "Գրադարանավարի Խնդիր",      "📚",
             dir + "diploma_library_problem.exe", 1 },
            { "bee",     "Մեղվի Խնդիր",          "🐝",
             dir + "diploma_bee_problem.exe", 2 },
            { "space",   "Տիեզերական Հետախույզ", "🚀",
             dir + "diploma_SpaceExploreEngine_problem.exe", 2 },
            { "ship",    "Նավերի Խնդիր",         "⛵",
             dir + "diploma_ship_problem.exe", 3 },
            { "maze",    "Լաբիրինթի Խնդիր",      "🌀",
             dir + "diploma_maze_problem.exe", 3 },
            };
}

bool MainWindow::isLevelComplete(int level)
{
    for (auto &p : allProjects())
        if (p.level == level && getStars(p.id) < 1)
            return false;
    return true;
}

void MainWindow::updateLevelButtons()
{
    auto applyState = [](QPushButton *btn, bool unlocked) {
        if (!btn) return;
        QLabel *arr = btn->findChild<QLabel*>("arrowLabel");
        if (arr) arr->setText(unlocked ? "→" : "—");
    };
    applyState(m_levelBtn[2], isLevelComplete(1));
    applyState(m_levelBtn[3], isLevelComplete(2));
}

void MainWindow::showLevelCompleteDialog(int completedLevel)
{
    // Timer-ը stop անել — dialog-ի ընթացքում կրկին չկանչվի
    if (m_pollTimer) m_pollTimer->stop();

    if (completedLevel >= 3) {
        QMessageBox box(this);
        box.setWindowTitle("Կոդամարտ — Ավարտ");
        box.setText("<h2 style='color:#4d9fff;font-family:'Segoe UI','Arial',sans-serif;'>ԱՄԲՈՂՋԱԿԱՆ ՀԱՂԹԱՆԱԿ</h2>"
                    "<p style='color:#c8d4f0;font-family:'Segoe UI','Arial',sans-serif;'>Դուք հաջողությամբ անցել եք բոլոր 3 մակարդակները</p>"
                    "<p style='color:#6a8a6a;font-size:12px;'>Բոլոր 6 ալգորիթմական խնդիրները լուծված են</p>");
        box.setStyleSheet("QMessageBox{background:#091020;} QLabel{color:#c8d4f0;}"
                          "QPushButton{background:#0f2040;border:1px solid #4d9fff;border-radius:6px;"
                          "color:#4d9fff;padding:8px 20px;font-family:'Segoe UI','Arial',sans-serif;}"
                          "QPushButton:hover{background:#4d9fff;color:#091020;}");
        QPushButton *progressBtn = box.addButton("Տեսնել Արդյունքները", QMessageBox::AcceptRole);
        box.addButton("Փակել", QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == progressBtn) showProgress();
        return;
    }

    int nextLevel = completedLevel + 1;
    QStringList levelNames = { "", "Առաջին", "Երկրորդ", "Երրորդ" };
    QMessageBox box(this);
    box.setWindowTitle(QString("Մակարդակ %1 — Ավարտ").arg(completedLevel));
    box.setText(QString(
                    "<h2 style='color:#4d9fff;font-family:'Segoe UI','Arial',sans-serif;'>ՄԱԿԱՐԴԱԿ %1 — ԱՎԱՐՏ</h2>"
                    "<p style='color:#c8d4f0;'>Անցնե՞նք <b style='color:#f0a500;'>%2-րդ մակարդակ</b>-ին։</p>"
                    ).arg(completedLevel).arg(nextLevel));
    box.setStyleSheet("QMessageBox{background:#091020;} QLabel{color:#c8d4f0;}"
                      "QPushButton{background:#0f2040;border:1px solid #4d9fff;border-radius:6px;"
                      "color:#4d9fff;padding:8px 20px;font-family:'Segoe UI','Arial',sans-serif;}"
                      "QPushButton:hover{background:#4d9fff;color:#091020;}");
    QPushButton *yesBtn = box.addButton(
        QString("Այո, %1-րդ մակարդակ →").arg(nextLevel), QMessageBox::AcceptRole);
    box.addButton("Ոչ", QMessageBox::RejectRole);
    box.exec();
    updateLevelButtons();
    if (box.clickedButton() == yesBtn) showLevel(nextLevel);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Կոդամարտ — Ալգորիթմական Մարտ");
    setMinimumSize(1020, 700);
    resize(1060, 720);

    QScreen *s = QApplication::primaryScreen();
    QRect sg = s->availableGeometry();
    move((sg.width() - width()) / 2, (sg.height() - height()) / 2);

    applyGlobalStyle();
    initDatabase();

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *ml = new QVBoxLayout(central);
    ml->setContentsMargins(0, 0, 0, 0);
    ml->setSpacing(0);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createHomePage());
    stackedWidget->addWidget(createLevelPage(1));
    stackedWidget->addWidget(createLevelPage(2));
    stackedWidget->addWidget(createLevelPage(3));

    m_progressPage = new QWidget();
    m_progressPage->setStyleSheet("background:#070d18;");
    rebuildProgressPage();
    stackedWidget->addWidget(m_progressPage);

    stackedWidget->setCurrentIndex(0);
    ml->addWidget(stackedWidget);
    updateLevelButtons();
}

MainWindow::~MainWindow()
{
    if (m_pollTimer) m_pollTimer->stop();
    QSqlDatabase::removeDatabase(DB_CONN);
}

// database
bool MainWindow::initDatabase()
{
    QDir().mkpath(QFileInfo(DB_PATH).absolutePath());
    if (!QSqlDatabase::contains(DB_CONN)) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", DB_CONN);
        db.setDatabaseName(DB_PATH);
    }
    QSqlDatabase db = QSqlDatabase::database(DB_CONN);
    if (!db.open()) return false;
    QSqlQuery q(db);
    q.exec("CREATE TABLE IF NOT EXISTS results ("
           "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "  project_id TEXT NOT NULL UNIQUE,"
           "  stars INTEGER NOT NULL CHECK(stars BETWEEN 1 AND 5),"
           "  saved_at TEXT NOT NULL)");
    return true;
}

int MainWindow::getStars(const QString &projectId)
{
    QSqlDatabase db = QSqlDatabase::database(DB_CONN);
    if (db.isOpen()) db.close();
    if (!db.open())  return -1;
    QSqlQuery q(db);
    q.exec("PRAGMA wal_checkpoint(PASSIVE)");
    q.prepare("SELECT stars FROM results WHERE project_id = :pid ORDER BY id DESC LIMIT 1");
    q.bindValue(":pid", projectId);
    q.exec();
    return q.next() ? q.value(0).toInt() : -1;
}

QString MainWindow::getGradeLetter(double avg)
{
    if (avg >= 4.5) return "A+";
    if (avg >= 4.0) return "A";
    if (avg >= 3.5) return "B+";
    if (avg >= 3.0) return "B";
    if (avg >= 2.5) return "C+";
    if (avg >= 2.0) return "C";
    return "D";
}

QString MainWindow::getGradeColor(double avg)
{
    if (avg >= 4.0) return "#4d9fff";
    if (avg >= 3.0) return "#f0a500";
    if (avg >= 2.0) return "#fb923c";
    return "#f87171";
}

void MainWindow::launchApp(const QString &exePath, const QString &appName,
                           const QString &projectId, int level)
{
    QFileInfo fi(exePath);
    if (!fi.exists()) {
        QMessageBox::warning(this, "Ֆայլ չի գտնվել",
                             QString("Հնարավոր չէ գտնել՝\n%1").arg(exePath));
        return;
    }
    // Launch ժամանակը — polling-ը ստուգում է արդյոք DB-ում saved_at >= launchTime
    // Այս մոտեցումը ճիշտ կաշխատի նաև "կրկին փորձել" սցենարի դեպքում
    QDateTime launchTime = QDateTime::currentDateTime();
    QProcess::startDetached(exePath, QStringList(), fi.absolutePath());

    if (!m_pollTimer) {
        m_pollTimer = new QTimer(this);
        m_pollTimer->setInterval(2000);
    }
    struct PollState { QDateTime launchTime; int level; bool triggered = false; };
    auto state = std::make_shared<PollState>();
    state->launchTime = launchTime;
    state->level = level;

    QObject *ctx = new QObject(this);
    connect(m_pollTimer, &QTimer::timeout, ctx,
            [this, projectId, state, ctx]() mutable {
                if (state->triggered) return;
                QSqlDatabase db = QSqlDatabase::database(DB_CONN);
                if (db.isOpen()) db.close();
                if (!db.open()) return;
                QSqlQuery q(db);
                q.exec("PRAGMA wal_checkpoint(PASSIVE)");
                q.prepare("SELECT saved_at, stars FROM results WHERE project_id = :pid");
                q.bindValue(":pid", projectId);
                q.exec();
                if (q.next()) {
                    QString savedAt  = q.value(0).toString();
                    int freshStars   = q.value(1).toInt();
                    QDateTime savedDt = QDateTime::fromString(savedAt, "dd.MM.yyyy HH:mm");
                    // Ճշտել՝ saved_at-ը launch-ից հետո է (± 1 րոպե հանդուրժողականություն)
                    if (savedDt >= state->launchTime.addSecs(-60) && freshStars >= 1) {
                        state->triggered = true;
                        ctx->deleteLater();
                        onAppFinished(projectId, state->level);
                    }
                }
            });
    if (!m_pollTimer->isActive()) m_pollTimer->start();
}

void MainWindow::onAppFinished(const QString &projectId, int level)
{
    int stars = getStars(projectId);
    if (stars < 1) return;
    updateLevelButtons();

    auto projects = allProjects();
    QList<ProjectInfo> levelProjects;
    for (auto &p : projects)
        if (p.level == level) levelProjects.append(p);
    if (levelProjects.size() < 2) return;

    bool firstSolved  = getStars(levelProjects[0].id) >= 1;
    bool secondSolved = getStars(levelProjects[1].id) >= 1;

    if (firstSolved && secondSolved) {
        // Փակել level page-ը՝ անցնելով home
        showHome();
        showLevelCompleteDialog(level);
    } else if (projectId == levelProjects[0].id && !secondSolved) {
        if (level == 3) {
            QTimer::singleShot(400, this, [this, levelProjects]() {
                // Փակել level page-ը նախքան dialog-ը
                showHome();

                QDialog *dlg = new QDialog(this);
                dlg->setWindowTitle("Հաջողություն — Նավերի Խնդիր");
                dlg->setFixedSize(480, 220);
                QPalette pal = dlg->palette();
                pal.setColor(QPalette::Window, QColor("#091020"));
                dlg->setPalette(pal); dlg->setAutoFillBackground(true);
                dlg->setStyleSheet(
                    "QLabel { color: #c8d4f0; background: transparent; }"
                    "QPushButton { font-size:13px; font-weight:bold; padding:10px 28px;"
                    "  border-radius:8px; border:none; }"
                    );

                QVBoxLayout *vl = new QVBoxLayout(dlg);
                vl->setContentsMargins(36, 28, 36, 28); vl->setSpacing(12);

                QLabel *title = new QLabel(" Նավերի Խնդիրն ավարտված է!");
                title->setAlignment(Qt::AlignCenter);
                title->setStyleSheet("font-size:16px;font-weight:700;color:#f76a8e;background:transparent;");
                vl->addWidget(title);

                QLabel *msg = new QLabel("Ուզո՞ւմ եք անցնել վերջին խնդրին(Լաբիրինթ), թե կրկին փորձե՞լ:");
                msg->setAlignment(Qt::AlignCenter);
                msg->setWordWrap(true);
                vl->addWidget(msg);

                vl->addStretch();

                QHBoxLayout *bl = new QHBoxLayout();
                bl->setSpacing(12);

                QPushButton *retryBtn = new QPushButton("↺  Կրկին փորձել");
                retryBtn->setStyleSheet(
                    "QPushButton { background:#1a2d50; color:#c8d4f0; border:1px solid #2a4070; }"
                    "QPushButton:hover { background:#2a3f6a; }");
                connect(retryBtn, &QPushButton::clicked, dlg, &QDialog::reject);

                QPushButton *continueBtn = new QPushButton("→  Անցնել Լաբիրինթին");
                continueBtn->setStyleSheet(
                    "QPushButton { background:#f76a8e; color:#070d18; }"
                    "QPushButton:hover { background:#ff8aaa; }");
                connect(continueBtn, &QPushButton::clicked, dlg, &QDialog::accept);

                bl->addStretch();
                bl->addWidget(retryBtn);
                bl->addWidget(continueBtn);
                bl->addStretch();
                vl->addLayout(bl);

                int res = dlg->exec();
                delete dlg;

                if (res == QDialog::Accepted) {
                    // Անցնել 3-րդ level page և գործարկել maze-ը
                    showLevel(3);
                    launchApp(levelProjects[1].exe, levelProjects[1].name,
                              levelProjects[1].id, levelProjects[1].level);
                } else {
                    // Կրկին փորձել ship-ը — վերադառնալ level 3 page և նորից գործարկել
                    showLevel(3);
                    launchApp(levelProjects[0].exe, levelProjects[0].name,
                              levelProjects[0].id, levelProjects[0].level);
                }

            });
        } else {
            QTimer::singleShot(600, this, [this, levelProjects]() {
                // Փակել level page-ը
                showHome();
                QMessageBox::information(this, "Հաջողություն",
                                         QString("«%1»-ը լուծված է!\n\nՀիմա կբացվի «%2»-ը։")
                                             .arg(levelProjects[0].name).arg(levelProjects[1].name));
                launchApp(levelProjects[1].exe, levelProjects[1].name,
                          levelProjects[1].id, levelProjects[1].level);
            });
        }
    }
}

void MainWindow::applyGlobalStyle()
{
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #070d18;
            color: #c8d4f0;
            font-family: 'Segoe UI', 'Arial Rounded MT Bold', 'Arial', sans-serif;
        }
        QScrollArea { border:none; background:transparent; }
        QScrollBar:vertical {
            background:#0d1628;
            width:5px;
            border-radius:2px;
        }
        QScrollBar::handle:vertical {
            background:#2a3f6a;
            border-radius:2px;
        }
        QToolTip {
            background:#0f2040;
            color:#4d9fff;
            border:1px solid #4d9fff;
            border-radius:4px;
            padding:4px 8px;
            font-size:11px;
        }
    )");
}

// home page
QWidget* MainWindow::createHomePage()
{
    QWidget *page = new QWidget();
    page->setStyleSheet("background:#070d18;");

    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    // TOP HEADER BAR
    QWidget *header = new QWidget();
    header->setFixedHeight(64);
    header->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #091020, stop:0.5 #0d1a30, stop:1 #091020);"
        "border-bottom:1px solid #1a2d50;");
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(36, 0, 36, 0);

    // Logo center
    hl->addStretch();
    QWidget *logoW = new QWidget();
    logoW->setStyleSheet("background:transparent;");
    {
        QHBoxLayout *ll = new QHBoxLayout(logoW);
        ll->setContentsMargins(0,0,0,0); ll->setSpacing(10);

        QLabel *dot1 = new QLabel("◈");
        dot1->setStyleSheet("color:#4d9fff;font-size:18px;background:transparent;");

        QLabel *logoText = new QLabel("ԿՈԴԱՄԱՐՏ");
        logoText->setStyleSheet(
            "color:#ffffff;font-size:20px;font-weight:700;"
            "letter-spacing:10px;background:transparent;"
            "font-family:'Segoe UI','Arial',sans-serif;");

        QLabel *dot2 = new QLabel("◈");
        dot2->setStyleSheet("color:#4d9fff;font-size:18px;background:transparent;");

        ll->addWidget(dot1);
        ll->addWidget(logoText);
        ll->addWidget(dot2);
    }
    hl->addWidget(logoW);
    hl->addStretch();

    lay->addWidget(header);

    // MAIN CONTENT
    QWidget *content = new QWidget();
    content->setStyleSheet("background:transparent;");
    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(40, 32, 40, 32);
    contentLayout->setSpacing(32);

    //  LEFT COLUMN: Mission statement
    QWidget *leftPanel = new QWidget();
    leftPanel->setFixedWidth(400);
    leftPanel->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "stop:0 #0d1a30,stop:1 #070d18);"
        "border:1px solid #1a2d50;"
        "border-radius:16px;");
    QVBoxLayout *leftLay = new QVBoxLayout(leftPanel);
    leftLay->setContentsMargins(24, 28, 24, 28);
    leftLay->setSpacing(16);

    QLabel *missionTag = new QLabel("ՀԱՄԱԿԱՐԳԻ ՄԱՍԻՆ");
    missionTag->setStyleSheet(
        "color:#4d9fff;font-size:10px;font-weight:700;"
        "letter-spacing:3px;background:transparent;"
        "font-family:'Segoe UI','Arial',sans-serif;");
    leftLay->addWidget(missionTag);

    QFrame *mSep = new QFrame();
    mSep->setFrameShape(QFrame::HLine);
    mSep->setStyleSheet("background:#1a2d50;max-height:1px;border:none;");
    leftLay->addWidget(mSep);

    QScrollArea *descScroll = new QScrollArea();
    descScroll->setWidgetResizable(true);
    descScroll->setFrameShape(QFrame::NoFrame);
    descScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    descScroll->setStyleSheet(
        "QScrollArea { background:transparent; border:none; }"
        "QScrollBar:vertical { background:#0d1628; width:4px; border-radius:2px; }"
        "QScrollBar::handle:vertical { background:#2a3f6a; border-radius:2px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0px; }");

    QWidget *descW = new QWidget();
    descW->setStyleSheet("background:transparent;");
    QVBoxLayout *descLay = new QVBoxLayout(descW);
    descLay->setContentsMargins(4, 8, 4, 8);
    descLay->setSpacing(0);

    QLabel *descLbl = new QLabel();
    descLbl->setWordWrap(true);
    descLbl->setTextFormat(Qt::RichText);
    descLbl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    descLbl->setStyleSheet(
        "color:#a0b4d8;font-size:12px;line-height:200%;"
        "background:transparent;padding:4px 2px;"
        "font-family:'Segoe UI','Arial',sans-serif;");
    descLbl->setText("<p>Ողջույն, Դուք մուտք եք գործել <Կոդամարտ> համակարգ, որը կօգնի Ձեզ՝ սկսնակ ծրագրավորողներիդ հասկանալ, արդյո՞ք կարող եք առաջադրված խնդիրներին տալ լուծումներ՝մշալելով լուծման ալգորիթմներ՝ունենալով միայն տրված խնդրի համար սահմանված սահմանափակ քանակի կոնկրետ հրամաններ։ Պայմանները շատ պարզ են, պետք է նախապես հասկանալ խնդրի լուծմանը հասնելու քայլերի հերթականությունը, մուտքագրել հրամանները հրամանների մատյանում, հետևել տվյալ խնդրի համար սահմանված կանոններին ու սահմանափակումներին, իրականացնել Ձեր գրած կոդի հատվածը և տեսնել կատարված քայլերը վիզուալ արտապատկերման միջոցով, ինչը կպարզեցնի ձեր՝ ինչպես սխալը ուղղելու փորձերը, այնպես էլ կատարված քայլերի իրականացման պատկերացումները։Դե ինչ, սկսեք, հարգելի օգտատեր, Ձեզ հաջողություն եմ մաղթում։ </p>");
    descLay->addWidget(descLbl);
    descLay->addStretch();
    descScroll->setWidget(descW);
    leftLay->addWidget(descScroll, 1);

    auto projects = allProjects();
    int completedProjects = 0;
    int totalStarsEarned  = 0;
    for (auto &p : projects) {
        int s = getStars(p.id);
        if (s > 0) { completedProjects++; totalStarsEarned += s; }
    }

    QWidget *statsCard = new QWidget();
    statsCard->setStyleSheet(
        "background:#0d1a30;border:1px solid #2a3f6a;border-radius:10px;");
    QHBoxLayout *sl = new QHBoxLayout(statsCard);
    sl->setContentsMargins(16, 12, 16, 12); sl->setSpacing(0);

    auto addStat = [&](const QString &val, const QString &lbl) {
        QWidget *sw = new QWidget(); sw->setStyleSheet("background:transparent;");
        QVBoxLayout *svl = new QVBoxLayout(sw);
        svl->setContentsMargins(0,0,0,0); svl->setSpacing(2);
        QLabel *v = new QLabel(val);
        v->setAlignment(Qt::AlignCenter);
        v->setStyleSheet("color:#4d9fff;font-size:22px;font-weight:900;background:transparent;");
        QLabel *l = new QLabel(lbl);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet("color:#3a5080;font-size:10px;letter-spacing:1px;background:transparent;");
        svl->addWidget(v); svl->addWidget(l);
        return sw;
    };

    sl->addWidget(addStat(QString::number(completedProjects), "ԱՎԱՐՏՎԱԾ"));
    QFrame *vSep = new QFrame(); vSep->setFrameShape(QFrame::VLine);
    vSep->setStyleSheet("background:#1a2d50;max-width:1px;border:none;");
    sl->addWidget(vSep);
    sl->addWidget(addStat(QString::number(totalStarsEarned) + "/30", "ԱՍՏՂ"));
    QFrame *vSep2 = new QFrame(); vSep2->setFrameShape(QFrame::VLine);
    vSep2->setStyleSheet("background:#1a2d50;max-width:1px;border:none;");
    sl->addWidget(vSep2);
    sl->addWidget(addStat(completedProjects > 0 ? getGradeLetter((double)totalStarsEarned/6.0) : "—", "ԳՆԱՀԱՏ."));
    leftLay->addWidget(statsCard);

    contentLayout->addWidget(leftPanel);

    QWidget *rightPanel = new QWidget();
    rightPanel->setStyleSheet("background:transparent;");
    QVBoxLayout *rightLay = new QVBoxLayout(rightPanel);
    rightLay->setContentsMargins(0, 0, 0, 0);
    rightLay->setSpacing(20);

    // Level buttons — stacked vertically for this layout
    m_levelBtn[1] = createLevelButton("ՄԱԿԱՐԴԱԿ 1", "Դույլ  ·  Գրադարան", 1);
    m_levelBtn[2] = createLevelButton("ՄԱԿԱՐԴԱԿ 2", "Մեղու  ·  Տիեզերք",  2);
    m_levelBtn[3] = createLevelButton("ՄԱԿԱՐԴԱԿ 3", "Լաբիրինթ  ·  Նավ",   3);

    rightLay->addWidget(m_levelBtn[1]);
    rightLay->addWidget(m_levelBtn[2]);
    rightLay->addWidget(m_levelBtn[3]);

    // Progress button
    QPushButton *progBtn = new QPushButton(" ԴԻՏԵԼ ԱՐԴՅՈՒՆՔՆԵՐԸ ԵՎ ԳՆԱՀԱՏԱԿԱՆԸ");
    progBtn->setCursor(Qt::PointingHandCursor);
    progBtn->setFixedHeight(46);
    progBtn->setStyleSheet(R"(
        QPushButton {
            background:#0d1a30;
            border:1px solid #2a3f6a;
            border-radius:10px;
            color:#4a6090;
            font-size:13px;
            font-weight:700;
            letter-spacing:1px;
        }
        QPushButton:hover {
            border-color:#4d9fff;
            color:#4d9fff;
            background:#0f2040;
        }
    )");
    connect(progBtn, &QPushButton::clicked, this, &MainWindow::showProgress);
    rightLay->addWidget(progBtn);

    contentLayout->addWidget(rightPanel);
    lay->addWidget(content, 1);


    return page;
}

// level button (horizontal card style)
QPushButton* MainWindow::createLevelButton(const QString &title, const QString &subtitle, int level)
{
    QPushButton *btn = new QPushButton();
    btn->setFixedHeight(100);
    btn->setCursor(Qt::PointingHandCursor);

    QString cols[]    = { "", "#4d9fff", "#f0a500", "#f76a8e" };
    QString dimCols[] = { "", "#1a2a50", "#1a2a50", "#1a1a40" };
    QString col    = cols[level];
    QString dimCol = dimCols[level];

    btn->setStyleSheet(QString(R"(
        QPushButton {
            background:qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #0d1a30, stop:1 #070d18);
            border:1px solid #1a2d50;
            border-radius:12px;
        }
        QPushButton:hover {
            background:qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %2, stop:1 #0d1a30);
            border:1px solid %1;
        }
        QPushButton:pressed { background:#070d18; }
    )").arg(col).arg(dimCol));

    QWidget *inner = new QWidget(btn);
    inner->setAttribute(Qt::WA_TransparentForMouseEvents);
    inner->setGeometry(0, 0, btn->width(), 100);
    inner->setStyleSheet("background:transparent;");
    inner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QObject::connect(btn, &QPushButton::destroyed, []{});
    btn->installEventFilter(new QObject(btn));

    QHBoxLayout *hl = new QHBoxLayout(inner);
    hl->setContentsMargins(28, 0, 28, 0);
    hl->setSpacing(20);

    QLabel *numBadge = new QLabel(QString::number(level));
    numBadge->setFixedSize(52, 52);
    numBadge->setAlignment(Qt::AlignCenter);
    numBadge->setStyleSheet(QString(
                                "background:rgba(%1,0.15);"
                                "color:%2;"
                                "border:1px solid %3;"
                                "border-radius:26px;"
                                "font-size:20px;"
                                "font-weight:900;")
                                .arg(level==1?"77,159,255":level==2?"240,165,0":"247,106,142")
                                .arg(col).arg(col));
    hl->addWidget(numBadge);

    // Middle: title + subtitle + stars
    QWidget *mid = new QWidget(); mid->setStyleSheet("background:transparent;");
    QVBoxLayout *ml = new QVBoxLayout(mid);
    ml->setContentsMargins(0,0,0,0); ml->setSpacing(4);

    QLabel *t = new QLabel(title);
    t->setStyleSheet(QString("color:#ffffff;font-size:15px;font-weight:700;"
                             "letter-spacing:2px;background:transparent;"));
    ml->addWidget(t);

    auto lProjects = allProjects();
    int levelStars = 0, levelMax = 0;
    for (auto &p : lProjects) {
        if (p.level == level) {
            int st = getStars(p.id);
            if (st > 0) levelStars += st;
            levelMax += 5;
        }
    }
    if (levelMax > 0) {
        QString starsStr;
        for (int i = 0; i < levelMax; i++) starsStr += (i < levelStars) ? "★" : "☆";
        QLabel *starsLbl = new QLabel(starsStr);
        starsLbl->setStyleSheet(levelStars > 0
                                    ? QString("color:%1;font-size:13px;background:transparent;").arg(col)
                                    : "color:#1a2d50;font-size:13px;background:transparent;");
        ml->addWidget(starsLbl);
    }
    hl->addWidget(mid, 1);

    QWidget *right = new QWidget(); right->setStyleSheet("background:transparent;");
    QVBoxLayout *rl = new QVBoxLayout(right);
    rl->setContentsMargins(0,0,0,0); rl->setSpacing(6); rl->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QStringList diffLabels = {"", "Հեշտ", "Միջին Բարդության", "Բարդ"};
    QLabel *diff = new QLabel(diffLabels[level]);
    diff->setStyleSheet(QString(
                            "color:%1;font-size:9px;font-weight:700;letter-spacing:2px;"
                            "background:rgba(%2,0.12);border:1px solid %3;"
                            "border-radius:4px;padding:2px 6px;")
                            .arg(col)
                            .arg(level==1?"77,159,255":level==2?"240,165,0":"247,106,142")
                            .arg(col));
    rl->addWidget(diff, 0, Qt::AlignRight);

    QLabel *arr = new QLabel("→");
    arr->setObjectName("arrowLabel");
    arr->setStyleSheet(QString("color:%1;font-size:20px;background:transparent;").arg(col));
    rl->addWidget(arr, 0, Qt::AlignRight);
    hl->addWidget(right);

    connect(btn, &QPushButton::clicked, this, [this, level]() { showLevel(level); });

    btn->setLayout(new QVBoxLayout());
    btn->layout()->setContentsMargins(0,0,0,0);
    btn->layout()->addWidget(inner);

    return btn;
}

// level page
QWidget* MainWindow::createLevelPage(int level)
{
    QWidget *page = new QWidget();
    page->setStyleSheet("background:#070d18;");
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0); lay->setSpacing(0);

    // Header bar
    QWidget *header = new QWidget();
    header->setFixedHeight(56);
    header->setStyleSheet("background:#091020;border-bottom:1px solid #1a2d50;");
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(36, 0, 36, 0);

    QPushButton *back = new QPushButton("← ՀԵՏ");
    back->setCursor(Qt::PointingHandCursor); back->setFixedSize(80, 30);
    back->setStyleSheet("QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
                        "border-radius:6px;color:#3a5080;font-size:11px;font-weight:700;"
                        "letter-spacing:1px;}"
                        "QPushButton:hover{border-color:#4d9fff;color:#4d9fff;}");
    connect(back, &QPushButton::clicked, this, &MainWindow::showHome);
    hl->addWidget(back);
    hl->addSpacing(20);

    // Breadcrumb
    QLabel *logoSmall = new QLabel("< ԿՈԴԱՄԱՐՏ />");
    logoSmall->setStyleSheet("color:#2a3f6a;font-size:13px;font-weight:700;background:transparent;");
    hl->addWidget(logoSmall);
    hl->addStretch();

    QString cols[] = { "", "#4d9fff", "#f0a500", "#f76a8e" };
    QString col = cols[level];
    QLabel *lvlTag = new QLabel(QString("ՄԱԿԱՐԴԱԿ %1").arg(QString::number(level).rightJustified(2,'0')));
    lvlTag->setStyleSheet(QString("color:%1;font-size:11px;font-weight:700;"
                                  "letter-spacing:3px;background:transparent;").arg(col));
    hl->addWidget(lvlTag);
    lay->addWidget(header);

    // Content
    QWidget *content = new QWidget();
    content->setStyleSheet("background:transparent;");
    QVBoxLayout *cl = new QVBoxLayout(content);
    cl->setContentsMargins(48, 36, 48, 36); cl->setSpacing(0);

    QStringList titles = { "", "ՀԻՄՆԱՐԱՐ ՊԱՐԶ ԱԼԳՈՐԻԹՄՆԵՐ", "ՕՊՏԻՄԱԼԱՑՄԱՆ ԽՆԴԻՐՆԵՐ", "ՈՒՂՈՒ ՈՐՈՆՈՒՄ ԵՎ ՄՈԴԵԼԱՎՈՐՈՒՄ" };
    QLabel *title = new QLabel(titles[level]);
    title->setStyleSheet("font-size:28px;font-weight:900;color:#ffffff;"
                         "letter-spacing:3px;background:transparent;");
    cl->addWidget(title);
    cl->addSpacing(6);
    QLabel *sub = new QLabel("Ընտրեք առաջադրանքը՝ սիմուլյատորը գործարկելու համար");
    sub->setStyleSheet("color:#2a3f6a;font-size:13px;background:transparent;");
    cl->addWidget(sub);
    cl->addSpacing(32);

    QWidget *cards = new QWidget(); cards->setStyleSheet("background:transparent;");
    QHBoxLayout *cardsLay = new QHBoxLayout(cards);
    cardsLay->setSpacing(24); cardsLay->setContentsMargins(0, 0, 0, 0);

    struct Proj { QString name, desc, icon, exe, id; };
    QList<Proj> projects;
    const QString dir = QCoreApplication::applicationDirPath() + "/";
    if (level == 1) {
        projects = {
            { "Դույլերի Խնդիր",    "Ջրի լցման գլուխկոտրուկ", "🪣",
             dir + "diploma_bucket_problem.exe", "bucket" },
            { "Գրադարանավարի Խնդիր", "Գրքերի դասավորում և\nժամանակացույցի օպտիմալացում", "📚",
             dir + "diploma_library_problem.exe", "library" }
        };
    } else if (level == 2) {
        projects = {
            { "Մեղուների Խնդիր",         "Մեղուների նեկտարահավաքի ուղու\nօպտիմալացման ", "🐝",
             dir + "diploma_bee_problem.exe", "bee" },
            { "Տիեզերագնացների խնդիր",   "Տիեզերական հետախուզման\nուղու մոդելավորում", "🚀",
             dir + "diploma_SpaceExploreEngine_problem.exe", "space" }
        };
    } else {
        projects = {
            { "Նավերի Խնդիր",           "Փրկարարական նավերի \nռազմավարության մշակում",    "⛵",
             dir + "diploma_ship_problem.exe", "ship" },
            { "Լաբիրինթի Խնդիր",        "Կախարդական լաբիրինթի ելքային \nուղու որոնում", "🌀",
             dir + "diploma_maze_problem.exe", "maze" }
        };
    }

    for (auto &p : projects)
        cardsLay->addWidget(createAppButton(p.name, p.desc, p.icon, p.exe, p.id, level));
    cl->addWidget(cards);
    cl->addStretch();

    {
        int earned = 0;
        for (auto &p : projects) {
            int s = getStars(p.id.isEmpty() ? "" : p.id);
            if (s > 0) earned += s;
        }
        int maxStars = (int)projects.size() * 5;

        QWidget *progressRow = new QWidget(); progressRow->setStyleSheet("background:transparent;");
        QHBoxLayout *prl = new QHBoxLayout(progressRow);
        prl->setContentsMargins(0,0,0,0); prl->setSpacing(12);

        QLabel *plbl = new QLabel(QString("PROGRESS  %1 / %2 ★").arg(earned).arg(maxStars));
        plbl->setStyleSheet(QString("color:%1;font-size:10px;letter-spacing:2px;background:transparent;").arg(col));
        prl->addWidget(plbl);

        QFrame *bar = new QFrame();
        bar->setFixedHeight(4);
        bar->setStyleSheet(QString("background:#1a2d50;border-radius:2px;"));
        prl->addWidget(bar, 1);

        cl->addWidget(progressRow);
    }

    // Nav dots
    QWidget *dots = new QWidget(); dots->setStyleSheet("background:transparent;");
    QHBoxLayout *dl = new QHBoxLayout(dots); dl->setAlignment(Qt::AlignCenter); dl->setSpacing(8);
    for (int i=1; i<=3; i++) {
        QLabel *d = new QLabel(); d->setFixedSize(i==level ? 24 : 8, 4);
        d->setStyleSheet(QString("background:%1;border-radius:2px;")
                             .arg(i==level ? col : "#1a2d50"));
        dl->addWidget(d);
    }
    cl->addSpacing(12);
    cl->addWidget(dots);

    lay->addWidget(content, 1);
    return page;
}

// app button
QPushButton* MainWindow::createAppButton(const QString &title, const QString &description,
                                         const QString &icon, const QString &exePath,
                                         const QString &projectId, int level)
{
    QPushButton *btn = new QPushButton();
    btn->setFixedSize(360, 280);
    btn->setCursor(Qt::PointingHandCursor);

    QString cols[] = { "", "#4d9fff", "#f0a500", "#f76a8e" };
    QString col = cols[level];

    btn->setStyleSheet(QString(R"(
        QPushButton {
            background:qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #0d1a30, stop:1 #070d18);
            border:1px solid #1a2d50;
            border-radius:16px;
        }
        QPushButton:hover {
            background:qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #102038, stop:1 #0d1a30);
            border:1px solid %1;
        }
        QPushButton:pressed { background:#070d18; }
    )").arg(col));

    QWidget *inner = new QWidget(btn);
    inner->setAttribute(Qt::WA_TransparentForMouseEvents);
    inner->setGeometry(0, 0, 360, 280);
    inner->setStyleSheet("background:transparent;");
    QVBoxLayout *vl = new QVBoxLayout(inner);
    vl->setContentsMargins(32, 28, 32, 28); vl->setSpacing(0);

    // Icon + status row
    QWidget *topRow = new QWidget(); topRow->setStyleSheet("background:transparent;");
    QHBoxLayout *trl = new QHBoxLayout(topRow); trl->setContentsMargins(0,0,0,0);

    QLabel *ico = new QLabel(icon);
    ico->setStyleSheet("font-size:36px;background:transparent;");
    trl->addWidget(ico);
    trl->addStretch();

    int stars = getStars(projectId);
    if (stars > 0) {
        QLabel *badge = new QLabel("ԱՎԱՐՏՎԱԾ");
        badge->setStyleSheet(QString("color:%1;font-size:9px;font-weight:700;"
                                     "background:rgba(%2,0.15);border:1px solid %3;"
                                     "border-radius:4px;padding:3px 7px;letter-spacing:1px;")
                                 .arg(col)
                                 .arg(level==1?"77,159,255":level==2?"240,165,0":"247,106,142")
                                 .arg(col));
        trl->addWidget(badge);
    }
    vl->addWidget(topRow);
    vl->addSpacing(14);

    QLabel *t = new QLabel(title);
    t->setStyleSheet("color:#ffffff;font-size:15px;font-weight:700;background:transparent;");
    vl->addWidget(t);
    vl->addSpacing(8);

    QLabel *desc = new QLabel(description);
    desc->setStyleSheet("color:#3a5080;font-size:12px;background:transparent;"
                        "font-family:'Segoe UI',sans-serif;");
    desc->setWordWrap(true);
    vl->addWidget(desc);
    vl->addSpacing(16);

    // Stars display
    QString starsStr;
    if (stars < 0) {
        starsStr = "— — — — —";
    } else {
        for (int i = 0; i < 5; i++) starsStr += (i < stars) ? "★ " : "☆ ";
    }
    QLabel *starsLbl = new QLabel(starsStr.trimmed());
    starsLbl->setStyleSheet(stars > 0
                                ? QString("color:%1;font-size:17px;background:transparent;").arg(col)
                                : "color:#1a2d50;font-size:14px;background:transparent;letter-spacing:4px;");
    vl->addWidget(starsLbl);
    vl->addStretch();

    QFrame *sep = new QFrame(); sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("background:#1a2d50;max-height:1px;border:none;");
    vl->addWidget(sep);
    vl->addSpacing(10);

    QWidget *lr = new QWidget(); lr->setStyleSheet("background:transparent;");
    QHBoxLayout *lrl = new QHBoxLayout(lr); lrl->setContentsMargins(0,0,0,0);
    QLabel *lt = new QLabel("ԳՈՐԾԱՐԿԵԼ");
    lt->setStyleSheet("color:#2a3f6a;font-size:11px;font-weight:700;"
                      "letter-spacing:2px;background:transparent;");
    lrl->addWidget(lt); lrl->addStretch();
    QLabel *arr = new QLabel("→");
    arr->setStyleSheet(QString("color:%1;font-size:16px;background:transparent;").arg(col));
    lrl->addWidget(arr);
    vl->addWidget(lr);

    // maze-ը locked է, երբ ship-ը չի ավարտվել
    bool isMaze = (projectId == "maze");
    bool shipDone = (getStars("ship") >= 1);
    bool isLocked = isMaze && !shipDone;

    if (isLocked) {
        btn->setCursor(Qt::ForbiddenCursor);
        // Lock overlay
        QLabel *lockOverlay = new QLabel(btn);
        lockOverlay->setGeometry(0, 0, 360, 280);
        lockOverlay->setAlignment(Qt::AlignCenter);
        lockOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
        lockOverlay->setStyleSheet(
            "background:rgba(7,13,24,0.72);border-radius:16px;"
            "font-size:40px;");
        lockOverlay->setText("🔒");

        // "Unlock" hint label at bottom
        QLabel *hintLbl = new QLabel("Նախ լուծեք Նավերի Խնդիրը", btn);
        hintLbl->setGeometry(0, 220, 360, 40);
        hintLbl->setAlignment(Qt::AlignCenter);
        hintLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        hintLbl->setStyleSheet(
            "background:transparent;color:#f76a8e;"
            "font-size:11px;font-weight:700;letter-spacing:1px;");

        btn->setStyleSheet(QString(R"(
            QPushButton {
                background:qlineargradient(x1:0,y1:0,x2:0,y2:1,
                    stop:0 #0d1a30, stop:1 #070d18);
                border:1px solid #1a2d50;
                border-radius:16px;
            }
        )"));

        connect(btn, &QPushButton::clicked, this,
                [this]() {
                    QMessageBox::information(this, "🔒 Կողպված է",
                                             "Լաբիրինթի խնդիրը կբացվի Նավերի Խնդիրը ավարտելուց հետո։");
                });
    } else {
        connect(btn, &QPushButton::clicked, this,
                [this, exePath, title, projectId, level]() {
                    launchApp(exePath, title, projectId, level);
                });
    }
    return btn;
}

// progress page
void MainWindow::rebuildProgressPage()
{
    if (!m_progressPage) return;

    QLayout *old = m_progressPage->layout();
    if (old) {
        QLayoutItem *item;
        while ((item = old->takeAt(0)) != nullptr) {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete old;
    }

    QVBoxLayout *lay = new QVBoxLayout(m_progressPage);
    lay->setContentsMargins(0, 0, 0, 0); lay->setSpacing(0);

    // Header
    QWidget *header = new QWidget();
    header->setFixedHeight(56);
    header->setStyleSheet("background:#091020;border-bottom:1px solid #1a2d50;");
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(36, 0, 36, 0);

    QPushButton *back = new QPushButton("← ՀԵՏ");
    back->setCursor(Qt::PointingHandCursor); back->setFixedSize(80, 30);
    back->setStyleSheet("QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
                        "border-radius:6px;color:#3a5080;font-size:11px;font-weight:700;}"
                        "QPushButton:hover{border-color:#4d9fff;color:#4d9fff;}");
    connect(back, &QPushButton::clicked, this, &MainWindow::showHome);
    hl->addWidget(back); hl->addSpacing(20);
    QLabel *logoSmall = new QLabel("< ԿՈԴԱՄԱՐՏ />");
    logoSmall->setStyleSheet("color:#2a3f6a;font-size:13px;font-weight:700;background:transparent;");
    hl->addWidget(logoSmall);
    hl->addStretch();
    // QPushButton *refresh = new QPushButton("↻  ԹԱՐՄ.");
    // refresh->setCursor(Qt::PointingHandCursor); refresh->setFixedSize(90, 30);
    // refresh->setStyleSheet("QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
    //                        "border-radius:6px;color:#3a5080;font-size:11px;font-weight:700;}"
    //                        "QPushButton:hover{border-color:#4d9fff;color:#4d9fff;}");
    // connect(refresh, &QPushButton::clicked, this, &MainWindow::refreshProgress);
    // hl->addWidget(refresh);
    lay->addWidget(header);

    // Scrollable content
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{background:transparent;border:none;}");

    QWidget *contentW = new QWidget();
    contentW->setStyleSheet("background:#070d18;");
    QVBoxLayout *cl = new QVBoxLayout(contentW);
    cl->setContentsMargins(48, 36, 48, 36); cl->setSpacing(0);

    QLabel *title = new QLabel("ԱՐԴՅՈՒՆՔՆԵՐ ԵՎ ԳՆԱՀԱՏԱԿԱՆ");
    title->setStyleSheet("font-size:24px;font-weight:900;color:#ffffff;"
                         "letter-spacing:4px;background:transparent;");
    cl->addWidget(title);
    cl->addSpacing(4);
    QLabel *sub = new QLabel("Ամփոփ տեղեկություն 6 ալգորիթմական առաջադրանքների կատարման վերաբերյալ");
    sub->setStyleSheet("color:#2a3f6a;font-size:12px;background:transparent;");
    cl->addWidget(sub);
    cl->addSpacing(28);

    // Stats
    auto projects = allProjects();
    int totalStars = 0, completedCount = 0;
    for (auto &p : projects) {
        int s = getStars(p.id);
        if (s > 0) { totalStars += s; completedCount++; }
    }
    double overallAvg = (double)totalStars / 6.0;
    QString gradeCol  = getGradeColor(overallAvg);

    // Grade card
    QWidget *gradeCard = new QWidget();
    gradeCard->setFixedHeight(120);
    gradeCard->setStyleSheet(QString(
                                 "QWidget { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
                                 "stop:0 #0d1a30,stop:0.3 #091020,stop:1 #070d18);"
                                 "border:1px solid %1;border-radius:14px; }"
                                 "QLabel { border:none;background:transparent; }").arg(gradeCol));
    QHBoxLayout *gl = new QHBoxLayout(gradeCard);
    gl->setContentsMargins(36, 20, 36, 20);

    // Grade letter
    QLabel *gradeLbl = new QLabel(completedCount > 0 ? getGradeLetter(overallAvg) : "—");
    gradeLbl->setStyleSheet(QString("color:%1;font-size:58px;font-weight:900;"
                                    "font-family:'Segoe UI','Arial',sans-serif;background:transparent;").arg(gradeCol));
    gl->addWidget(gradeLbl);
    gl->addSpacing(28);

    QVBoxLayout *gInfo = new QVBoxLayout();
    QLabel *gTitle = new QLabel("ՎԵՐՋՆԱԿԱՆ ԳՆԱՀԱՏԱԿԱՆ");
    gTitle->setStyleSheet("color:#ffffff;font-size:13px;font-weight:700;"
                          "letter-spacing:3px;background:transparent;");
    gInfo->addWidget(gTitle);

    QString avgStr = completedCount > 0
                         ? QString("Միջին՝ %1 / 5.0  ·  Ավարտված՝ %2 / 6 խնդիր")
                               .arg(overallAvg, 0, 'f', 1).arg(completedCount)
                         : "Դեռ ոչ մի խնդիր չի ավարտվել";
    QLabel *gSub = new QLabel(avgStr);
    gSub->setStyleSheet("color:#3a5080;font-size:12px;background:transparent;");
    gInfo->addWidget(gSub);

    QString starsTotal;
    for (int i = 0; i < 5; i++) starsTotal += (i < qRound(overallAvg)) ? "★ " : "☆ ";
    QLabel *gStars = new QLabel(starsTotal.trimmed());
    gStars->setStyleSheet(QString("color:%1;font-size:18px;background:transparent;").arg(gradeCol));
    gInfo->addWidget(gStars);
    gl->addLayout(gInfo);
    gl->addStretch();

    QLabel *fraction = new QLabel(QString("%1\n/ 30").arg(totalStars));
    fraction->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    fraction->setStyleSheet(QString("color:%1;font-size:26px;font-weight:900;"
                                    "background:transparent;letter-spacing:-1px;").arg(gradeCol));
    gl->addWidget(fraction);
    cl->addWidget(gradeCard);
    cl->addSpacing(32);

    // Project rows by level
    QStringList levelNames  = { "", "ՄԱԿԱՐԴԱԿ 1", "ՄԱԿԱՐԴԱԿ 2", "ՄԱԿԱՐԴԱԿ 3" };
    QString levelColors[]   = { "", "#4d9fff",    "#f0a500",    "#f76a8e" };
    int lastLevel = 0;

    for (auto &p : projects) {
        if (p.level != lastLevel) {
            lastLevel = p.level;
            QLabel *lvlHdr = new QLabel(QString("── %1 ──").arg(levelNames[p.level]));
            lvlHdr->setStyleSheet(QString("color:%1;font-size:10px;font-weight:700;"
                                          "letter-spacing:3px;background:transparent;"
                                          "margin-top:8px;").arg(levelColors[p.level]));
            cl->addWidget(lvlHdr);
            cl->addSpacing(10);
        }

        int s = getStars(p.id);
        QWidget *row = new QWidget();
        row->setFixedHeight(68);
        row->setStyleSheet(QString(
                               "QWidget { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
                               "stop:0 #0d1a30,stop:1 #070d18);"
                               "border:1px solid %1;border-radius:10px; }"
                               "QLabel { border:none;background:transparent; }")
                               .arg(s > 0 ? levelColors[p.level] : "#1a2d50"));
        QHBoxLayout *rl = new QHBoxLayout(row);
        rl->setContentsMargins(20, 0, 20, 0); rl->setSpacing(16);

        QLabel *icoLbl = new QLabel(p.icon);
        icoLbl->setStyleSheet("font-size:11px;font-weight:700;color:#4d9fff;background:transparent;");
        icoLbl->setFixedWidth(32);
        rl->addWidget(icoLbl);

        QLabel *nameLbl = new QLabel(p.name);
        nameLbl->setStyleSheet("color:#c8d4f0;font-size:14px;font-weight:700;background:transparent;");
        nameLbl->setFixedWidth(220);
        rl->addWidget(nameLbl);
        rl->addStretch();

        if (s > 0) {
            QString st;
            for (int i = 0; i < 5; i++) st += (i < s) ? "★" : "☆";
            QLabel *stLbl = new QLabel(st);
            stLbl->setStyleSheet(QString("color:%1;font-size:18px;background:transparent;")
                                     .arg(levelColors[p.level]));
            rl->addWidget(stLbl);
            rl->addSpacing(12);
            QLabel *numLbl = new QLabel(QString("%1 / 5").arg(s));
            numLbl->setStyleSheet("color:#3a5080;font-size:12px;background:transparent;");
            rl->addWidget(numLbl);
        } else {
            QLabel *nLbl = new QLabel("ՉԻ ԱՎارՏVELS");
            nLbl->setText("ՉԻ ԱՎԱՐՏՎԵԼ");
            nLbl->setStyleSheet("color:#1a2d50;font-size:11px;letter-spacing:2px;background:transparent;");
            rl->addWidget(nLbl);
        }
        cl->addWidget(row);
        cl->addSpacing(8);
    }
    cl->addStretch();

    scroll->setWidget(contentW);
    lay->addWidget(scroll, 1);
}

// navigation
void MainWindow::showLevel(int level)
{
    if (level == 2 && !isLevelComplete(1)) {
        QMessageBox box(this);
        box.setWindowTitle("Մակարդակը կողպված է");
        box.setStyleSheet("QMessageBox{background:#070d18;} QLabel{color:#c8d4f0;}"
                          "QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
                          "border-radius:8px;color:#4d9fff;padding:8px 20px;}"
                          "QPushButton:hover{background:#4d9fff;color:#070d18;border-color:#4d9fff;}");
        box.setText("<h3 style='color:#f87171;'>2-րդ մակարդակը կողպված է</h3>"
                    "<p style='color:#c8d4f0;'>Այս մակարդակի խնդիրները դեռ հասանելի չեն։</p>"
                    "<p style='color:#3a5080;'>Նախ լուծի՛ր <b style='color:#4d9fff;'>1-ին մակարդակի</b> "
                    "երկու խնդիրները՝ Դույլ և Գրադարան։</p>");
        box.addButton("Լավ", QMessageBox::AcceptRole);
        box.exec();
        return;
    }
    if (level == 3 && !isLevelComplete(2)) {
        QMessageBox box(this);
        box.setWindowTitle("Մակարդակը կողպված է");
        box.setStyleSheet("QMessageBox{background:#070d18;} QLabel{color:#c8d4f0;}"
                          "QPushButton{background:#0d1a30;border:1px solid #1a2d50;"
                          "border-radius:8px;color:#f0a500;padding:8px 20px;}"
                          "QPushButton:hover{background:#f0a500;color:#070d18;border-color:#f0a500;}");
        box.setText("<h3 style='color:#f87171;'>3-րդ մակարդակը կողպված է</h3>"
                    "<p style='color:#c8d4f0;'>Այս մակարդակի խնդիրները դեռ հասանելի չեն։</p>"
                    "<p style='color:#3a5080;'>Նախ լուծի՛ր <b style='color:#f0a500;'>2-րդ մակարդակի</b> "
                    "երկու խնդիրները՝ Մեղու և Տիեզերք։</p>");
        box.addButton("Լավ", QMessageBox::AcceptRole);
        box.exec();
        return;
    }
    rebuildLevelPage(level);
    stackedWidget->setCurrentIndex(level);
}

void MainWindow::rebuildLevelPage(int level)
{
    // index: 1=level1, 2=level2, 3=level3
    QWidget *oldPage = stackedWidget->widget(level);
    QWidget *fresh = createLevelPage(level);
    stackedWidget->insertWidget(level, fresh);
    stackedWidget->removeWidget(oldPage);
    oldPage->deleteLater();
}

void MainWindow::showHome()
{
    QWidget *oldPage = stackedWidget->widget(0);
    QWidget *fresh   = createHomePage();
    stackedWidget->insertWidget(0, fresh);
    stackedWidget->removeWidget(oldPage);
    oldPage->deleteLater();
    updateLevelButtons();
    stackedWidget->setCurrentIndex(0);
}
void MainWindow::showProgress()    { rebuildProgressPage(); stackedWidget->setCurrentIndex(4); }
void MainWindow::refreshProgress() { rebuildProgressPage(); }
