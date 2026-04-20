#include "QtWidgetsApplication1.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QScrollArea>
#include <QTimer>

// --- HUSLD DARK THEME STYLING ---
const QString HUSLD_STYLE =
"QMainWindow { background-color: #0b0b0b; }"
"QWidget { background-color: #0b0b0b; color: #ffffff; font-family: 'Inter', sans-serif; }"
"QFrame#Sidebar { background-color: #111111; border-right: 1px solid #222222; }"
"QPushButton#NavBtn { border: none; text-align: left; font-size: 14px; padding: 12px 20px; color: #aaaaaa; border-radius: 8px; }"
"QPushButton#NavBtn:hover { background-color: #1a1a1a; color: #ffffff; }"
"QLineEdit#SearchBar { background-color: #1a1a1a; border: 1px solid #333333; border-radius: 8px; padding: 10px; color: white; }"
"QFrame#FeedCard { background-color: #111111; border: 1px solid #222222; border-radius: 12px; margin-bottom: 20px; }"
"QLabel#ProfileInitial { background-color: #222222; border-radius: 25px; border: 1px solid #444444; }";

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent) : QMainWindow(parent) {
    this->resize(1300, 900);
    this->setStyleSheet(HUSLD_STYLE);

    QStackedWidget* mainStack = new QStackedWidget(this);
    setCentralWidget(mainStack);

    // ==========================================
    // 1. SPLASH SCREEN (NOVO LOGO)
    // ==========================================
    QWidget* splashPage = new QWidget();
    QVBoxLayout* sLayout = new QVBoxLayout(splashPage);
    QLabel* logo = new QLabel("NOVO.");
    logo->setStyleSheet("font-size: 80px; font-weight: 800; color: #ffffff; letter-spacing: -2px;");
    sLayout->addStretch();
    sLayout->addWidget(logo, 0, Qt::AlignCenter);
    sLayout->addStretch();
    mainStack->addWidget(splashPage);

    // ==========================================
    // 2. MAIN HUB (HUSLD Layout)
    // ==========================================
    QWidget* mainHub = new QWidget();
    QHBoxLayout* hubLayout = new QHBoxLayout(mainHub);
    hubLayout->setContentsMargins(0, 0, 0, 0);
    hubLayout->setSpacing(0);

    // --- LEFT SIDEBAR ---
    QFrame* sidebar = new QFrame();
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(260);
    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(15, 30, 15, 20);

    QLabel* brand = new QLabel("NOVO.");
    brand->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 30px; padding-left: 10px;");
    sideLayout->addWidget(brand);

    // Navigation Buttons (Add your Backend hooks here)
    QStringList menu = { "Topics", "My Posts", "My Messages", "Notifications", "Time Spent", "Invite" };
    for (const QString& item : menu) {
        QPushButton* btn = new QPushButton(item);
        btn->setObjectName("NavBtn");
        sideLayout->addWidget(btn);
        // Backend Hook Example:
        // if(item == "Notifications") connect(btn, &QPushButton::clicked, this, &YourClass::loadNotifications);
    }

    sideLayout->addStretch();

    // User Mini Profile at bottom
    QFrame* userBox = new QFrame();
    userBox->setStyleSheet("background: #1a1a1a; border-radius: 10px; padding: 10px;");
    QHBoxLayout* uLayout = new QHBoxLayout(userBox);
    QLabel* avatar = new QLabel("AS");
    avatar->setFixedSize(40, 40);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setObjectName("ProfileInitial");
    uLayout->addWidget(avatar);
    uLayout->addWidget(new QLabel("<b>abdullah_saleem</b>"));
    sideLayout->addWidget(userBox);

    hubLayout->addWidget(sidebar);

    // --- CENTER CONTENT (FEED) ---
    QWidget* centerArea = new QWidget();
    QVBoxLayout* centerLayout = new QVBoxLayout(centerArea);
    centerLayout->setContentsMargins(30, 20, 30, 0);

    // Top Search Bar
    QLineEdit* searchBar = new QLineEdit();
    searchBar->setObjectName("SearchBar");
    searchBar->setPlaceholderText("What do you need?");
    centerLayout->addWidget(searchBar);

    // Scrollable Feed
    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    QWidget* feedContainer = new QWidget();
    QVBoxLayout* postsLayout = new QVBoxLayout(feedContainer);
    postsLayout->setAlignment(Qt::AlignTop);

    // Feed Category Tabs (Latest, New, Unread)
    QHBoxLayout* tabs = new QHBoxLayout();
    QString tabStyle = "QPushButton { background: transparent; color: #888; border: none; font-weight: bold; } QPushButton:hover { color: white; }";
    QPushButton* t1 = new QPushButton("Latest"); t1->setStyleSheet("color: white; border-bottom: 2px solid white;");
    QPushButton* t2 = new QPushButton("New (39)"); t2->setStyleSheet(tabStyle);
    tabs->addWidget(t1); tabs->addWidget(t2); tabs->addStretch();
    postsLayout->addLayout(tabs);
    postsLayout->addSpacing(20);

    // GENERATE POSTS (HUSLD Style)
    for (int i = 0; i < 5; i++) {
        QFrame* card = new QFrame();
        card->setObjectName("FeedCard");
        QVBoxLayout* cLayout = new QVBoxLayout(card);
        cLayout->setContentsMargins(20, 20, 20, 20);

        QLabel* author = new QLabel("<b>@JeffreyEpstein</b> • Created 15d ago");
        author->setStyleSheet("color: #888; font-size: 12px;");

        QLabel* title = new QLabel("17k member youtube creators super active discord server");
        title->setStyleSheet("font-size: 18px; font-weight: bold; margin-top: 10px;");
        title->setWordWrap(true);

        QLabel* body = new QLabel("dm 4 link price: offers no botted members all real youtube creators community server...");
        body->setStyleSheet("color: #bbb; margin-top: 5px;");
        body->setWordWrap(true);

        cLayout->addWidget(author);
        cLayout->addWidget(title);
        cLayout->addWidget(body);
        postsLayout->addWidget(card);
    }

    scroll->setWidget(feedContainer);
    centerLayout->addWidget(scroll);
    hubLayout->addWidget(centerArea, 2); // Takes more space

    // --- RIGHT SIDEBAR (Stats) ---
    QFrame* rightBar = new QFrame();
    rightBar->setFixedWidth(280);
    QVBoxLayout* rLayout = new QVBoxLayout(rightBar);

    QLabel* statsTitle = new QLabel("POPULAR HUSTLERS");
    statsTitle->setStyleSheet("font-size: 12px; color: #f09433; font-weight: bold;");
    rLayout->addWidget(statsTitle);

    // Placeholder for Ranking/Stats
    for (int i = 1; i <= 3; i++) {
        QLabel* rank = new QLabel(QString::number(i) + ". User_" + QString::number(i) + " - 21.9k Rep");
        rank->setStyleSheet("padding: 10px; background: #111; border-radius: 5px; margin-bottom: 5px;");
        rLayout->addWidget(rank);
    }
    rLayout->addStretch();
    hubLayout->addWidget(rightBar);

    mainStack->addWidget(mainHub);

    // Transition Logic
    QTimer::singleShot(2000, [=]() { mainStack->setCurrentIndex(1); });
}

QtWidgetsApplication1::~QtWidgetsApplication1() {}