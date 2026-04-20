#include "MainWindow.h"
#include "LoginWindow.h"
#include "UpdateBioWindow.h"
#include "UpdatePasswordWindow.h"
#include "CreatePostWindow.h"
#include <QFrame>
#include <QScrollArea>

// ── Shared style ─────────────────────────────────────
static const QString darkBg = "#121212";
static const QString cardBg = "#1e1e2e";
static const QString accent = "#7c6af7";
static const QString accent2 = "#f093fb";
static const QString textCol = "#ffffff";
static const QString subText = "#a0a0b0";

MainWindow::MainWindow(User* loggedInUser, User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), loggedInUser(loggedInUser), allUsers(allUsers), userCount(userCount) {

    setWindowTitle("NOVO");
    setFixedSize(900, 700);
    setStyleSheet("background-color: " + darkBg + ";");

    // ── Sidebar ──────────────────────────────────────
    QWidget* sidebar = new QWidget();
    sidebar->setFixedWidth(70);
    sidebar->setStyleSheet("background-color: #1a1a2e; border-right: 1px solid #2a2a3e;");

    auto makeSideBtn = [](const QString& emoji, const QString& tip) {
        QPushButton* btn = new QPushButton(emoji);
        btn->setToolTip(tip);
        btn->setFixedSize(50, 50);
        btn->setStyleSheet(
            "QPushButton { background: transparent; font-size: 22px;"
            "border-radius: 12px; color: white; }"
            "QPushButton:hover { background: rgba(124,106,247,0.2); }"
        );
        return btn;
        };

    QLabel* logoSide = new QLabel("N");
    logoSide->setAlignment(Qt::AlignCenter);
    logoSide->setFixedHeight(60);
    logoSide->setStyleSheet("color: #7c6af7; font-size: 28px; font-weight: bold;");

    QPushButton* feedBtn = makeSideBtn("🏠", "Feed");
    QPushButton* searchBtn = makeSideBtn("🔍", "Search");
    QPushButton* msgBtn = makeSideBtn("💬", "Messages");
    QPushButton* notifBtn = makeSideBtn("🔔", "Notifications");
    QPushButton* timeBtn = makeSideBtn("⏱", "Time Spent");
    QPushButton* logoutBtn = makeSideBtn("🚪", "Logout");
    logoutBtn->setStyleSheet(
        "QPushButton { background: transparent; font-size: 22px;"
        "border-radius: 12px; color: #ff6b6b; }"
        "QPushButton:hover { background: rgba(255,107,107,0.2); }"
    );

    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(10, 15, 10, 15);
    sideLayout->setSpacing(8);
    sideLayout->addWidget(logoSide);
    sideLayout->addSpacing(20);
    sideLayout->addWidget(feedBtn, 0, Qt::AlignCenter);
    sideLayout->addWidget(searchBtn, 0, Qt::AlignCenter);
    sideLayout->addWidget(msgBtn, 0, Qt::AlignCenter);
    sideLayout->addWidget(notifBtn, 0, Qt::AlignCenter);
    sideLayout->addWidget(timeBtn, 0, Qt::AlignCenter);
    sideLayout->addStretch();
    sideLayout->addWidget(logoutBtn, 0, Qt::AlignCenter);

    // ── Profile Header ───────────────────────────────
    QWidget* profileHeader = new QWidget();
    profileHeader->setStyleSheet("background-color: #1a1a2e;"
        "border-bottom: 1px solid #2a2a3e;");
    profileHeader->setFixedHeight(180);

    // Avatar circle
    QLabel* avatar = new QLabel(QString::fromStdString(
        loggedInUser->getUsername()).left(1).toUpper());
    avatar->setFixedSize(80, 80);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #667eea, stop:1 #764ba2);"
        "border-radius: 40px;"
        "color: white; font-size: 32px; font-weight: bold;"
    );

    // Username
    QLabel* usernameLabel = new QLabel(
        QString::fromStdString(loggedInUser->getUsername()));
    usernameLabel->setStyleSheet(
        "color: white; font-size: 20px; font-weight: bold; background: transparent;");

    // Bio
    QLabel* bioLabel = new QLabel(
        QString::fromStdString(loggedInUser->getBio()));
    bioLabel->setStyleSheet("color: " + subText + "; font-size: 13px; background: transparent;");
    bioLabel->setWordWrap(true);

    // Stats — clickable
    auto makeStatBtn = [](const QString& num, const QString& label) {
        QPushButton* btn = new QPushButton();
        btn->setStyleSheet(
            "QPushButton { background: transparent; border: none; }"
            "QPushButton:hover { background: rgba(255,255,255,0.05); border-radius: 8px; }"
        );
        QVBoxLayout* l = new QVBoxLayout(btn);
        l->setSpacing(2);
        l->setContentsMargins(12, 6, 12, 6);
        QLabel* numL = new QLabel(num);
        numL->setAlignment(Qt::AlignCenter);
        numL->setStyleSheet("color: white; font-size: 18px; font-weight: bold; background: transparent;");
        QLabel* lblL = new QLabel(label);
        lblL->setAlignment(Qt::AlignCenter);
        lblL->setStyleSheet("color: #a0a0b0; font-size: 11px; background: transparent;");
        l->addWidget(numL);
        l->addWidget(lblL);
        return btn;
        };

    QPushButton* postsBtn = makeStatBtn(
        QString::number(loggedInUser->getPostCount()), "Posts");
    QPushButton* followersBtn = makeStatBtn(
        QString::number(loggedInUser->getFollowersCount()), "Followers");
    QPushButton* followingBtn = makeStatBtn(
        QString::number(loggedInUser->getFollowingCount()), "Following");

    // Posts stat — no action
    postsBtn->setEnabled(false);

    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->addStretch();
    statsLayout->addWidget(postsBtn);
    statsLayout->addWidget(followersBtn);
    statsLayout->addWidget(followingBtn);
    statsLayout->addStretch();

    // Edit buttons
    QPushButton* updateBioBtn = new QPushButton("Update Bio");
    QPushButton* updatePassBtn = new QPushButton("Update Password");
    QPushButton* createPostBtnH = new QPushButton("+ New Post");

    QString editBtnStyle =
        "QPushButton { background: rgba(124,106,247,0.15);"
        "color: #7c6af7; border: 1px solid #7c6af7;"
        "border-radius: 8px; padding: 6px 14px; font-size: 12px; }"
        "QPushButton:hover { background: rgba(124,106,247,0.3); }";

    updateBioBtn->setStyleSheet(editBtnStyle);
    updatePassBtn->setStyleSheet(editBtnStyle);
    createPostBtnH->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #667eea, stop:1 #764ba2);"
        "color: white; border: none;"
        "border-radius: 8px; padding: 6px 14px; font-size: 12px; font-weight: bold; }"
        "QPushButton:hover { opacity: 0.8; }"
    );

    QHBoxLayout* editLayout = new QHBoxLayout();
    editLayout->addStretch();
    editLayout->addWidget(updateBioBtn);
    editLayout->addWidget(updatePassBtn);
    editLayout->addWidget(createPostBtnH);
    editLayout->addStretch();

    // Profile info layout
    QHBoxLayout* topRow = new QHBoxLayout();
    topRow->setContentsMargins(20, 15, 20, 0);
    topRow->setSpacing(20);
    topRow->addWidget(avatar);

    QVBoxLayout* infoCol = new QVBoxLayout();
    infoCol->addWidget(usernameLabel);
    infoCol->addWidget(bioLabel);
    topRow->addLayout(infoCol);
    topRow->addStretch();

    QVBoxLayout* headerLayout = new QVBoxLayout(profileHeader);
    headerLayout->setContentsMargins(0, 0, 0, 10);
    headerLayout->setSpacing(8);
    headerLayout->addLayout(topRow);
    headerLayout->addLayout(statsLayout);
    headerLayout->addLayout(editLayout);

    // ── Posts Scroll Area ────────────────────────────
    postsContainer = new QWidget();
    postsLayout = new QVBoxLayout(postsContainer);
    postsLayout->setContentsMargins(15, 15, 15, 15);
    postsLayout->setSpacing(15);
    postsLayout->addStretch();

    loadPosts();

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(postsContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; background: transparent; }"
        "QScrollBar:vertical { background: #1a1a2e; width: 6px; border-radius: 3px; }"
        "QScrollBar::handle:vertical { background: #7c6af7; border-radius: 3px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
    );

    // ── Right panel (header + scroll) ───────────────
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    rightLayout->addWidget(profileHeader);
    rightLayout->addWidget(scrollArea);

    // ── Root layout ──────────────────────────────────
    QHBoxLayout* rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(rightPanel);

    // ── Connections ──────────────────────────────────
    connect(followersBtn, &QPushButton::clicked, this, &MainWindow::onFollowersClicked);
    connect(followingBtn, &QPushButton::clicked, this, &MainWindow::onFollowingClicked);
    connect(updateBioBtn, &QPushButton::clicked, this, &MainWindow::onUpdateBioClicked);
    connect(updatePassBtn, &QPushButton::clicked, this, &MainWindow::onUpdatePasswordClicked);
    connect(createPostBtnH, &QPushButton::clicked, this, &MainWindow::onCreatePostClicked);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

    // Sidebar buttons — abhi inactive
    connect(feedBtn, &QPushButton::clicked, []() {});
    connect(searchBtn, &QPushButton::clicked, []() {});
    connect(msgBtn, &QPushButton::clicked, []() {});
    connect(notifBtn, &QPushButton::clicked, []() {});
    connect(timeBtn, &QPushButton::clicked, []() {});
}

// ── Post Card ────────────────────────────────────────
QWidget* MainWindow::createPostCard(Posts* post) {
    // Random gradient per post based on postId
    QStringList gradients = {
        "stop:0 #667eea, stop:1 #764ba2",
        "stop:0 #f093fb, stop:1 #f5576c",
        "stop:0 #4facfe, stop:1 #00f2fe",
        "stop:0 #43e97b, stop:1 #38f9d7",
        "stop:0 #fa709a, stop:1 #fee140",
        "stop:0 #a18cd1, stop:1 #fbc2eb",
    };
    int gradIdx = (post->getPostId().empty() ? 0 :
        (int)post->getPostId().back()) % gradients.size();
    QString grad = gradients[gradIdx];

    QFrame* card = new QFrame();
    card->setStyleSheet(
        "QFrame { background-color: " + cardBg + ";"
        "border-radius: 16px;"
        "border: 1px solid #2a2a3e; }"
    );

    // Top color bar
    QLabel* colorBar = new QLabel();
    colorBar->setFixedHeight(6);
    colorBar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0," + grad + ");"
        "border-radius: 3px;"
    );

    // Author row
    QLabel* authorAvatar = new QLabel(
        QString::fromStdString(loggedInUser->getUsername()).left(1).toUpper());
    authorAvatar->setFixedSize(36, 36);
    authorAvatar->setAlignment(Qt::AlignCenter);
    authorAvatar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1," + grad + ");"
        "border-radius: 18px; color: white; font-weight: bold; font-size: 14px;"
    );

    QLabel* authorName = new QLabel(
        QString::fromStdString(loggedInUser->getUsername()));
    authorName->setStyleSheet("color: white; font-weight: bold; font-size: 14px;"
        "background: transparent;");

    QLabel* postIdLabel = new QLabel(
        QString::fromStdString(post->getPostId()));
    postIdLabel->setStyleSheet("color: " + subText + "; font-size: 11px; background: transparent;");

    QHBoxLayout* authorRow = new QHBoxLayout();
    authorRow->addWidget(authorAvatar);
    QVBoxLayout* authorInfo = new QVBoxLayout();
    authorInfo->setSpacing(0);
    authorInfo->addWidget(authorName);
    authorInfo->addWidget(postIdLabel);
    authorRow->addLayout(authorInfo);
    authorRow->addStretch();

    // Content
    QLabel* content = new QLabel(QString::fromStdString(post->getContent()));
    content->setWordWrap(true);
    content->setStyleSheet(
        "color: white; font-size: 15px; line-height: 1.5;"
        "background: rgba(255,255,255,0.03);"
        "border-radius: 10px; padding: 12px;"
        "border: none;"
    );

    // Like button (toggle)
    QPushButton* likeBtn = new QPushButton("🤍  " +
        QString::number(post->getLikeCount()));
    likeBtn->setCheckable(true);
    likeBtn->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,0.05);"
        "color: #a0a0b0; border: 1px solid #2a2a3e;"
        "border-radius: 20px; padding: 6px 16px; font-size: 13px; }"
        "QPushButton:checked { background: rgba(240,80,80,0.15);"
        "color: #ff6b6b; border: 1px solid #ff6b6b; }"
        "QPushButton:hover { background: rgba(255,255,255,0.1); }"
    );

    // Comment button (inactive)
    QPushButton* commentBtn = new QPushButton("💬  Comment");
    commentBtn->setEnabled(false);
    commentBtn->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,0.05);"
        "color: #a0a0b0; border: 1px solid #2a2a3e;"
        "border-radius: 20px; padding: 6px 16px; font-size: 13px; }"
    );

    QHBoxLayout* actionsRow = new QHBoxLayout();
    actionsRow->addWidget(likeBtn);
    actionsRow->addWidget(commentBtn);
    actionsRow->addStretch();

    // Like toggle logic
    Posts* postPtr = post;
    connect(likeBtn, &QPushButton::toggled, [likeBtn, postPtr](bool checked) {
        if (checked) {
            postPtr->likePost();
            likeBtn->setText("❤️  " + QString::number(postPtr->getLikeCount()));
        }
        else {
            postPtr->unlikePost();
            likeBtn->setText("🤍  " + QString::number(postPtr->getLikeCount()));
        }
        });

    // Card layout
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(16, 16, 16, 16);
    cardLayout->setSpacing(12);
    cardLayout->addWidget(colorBar);
    cardLayout->addLayout(authorRow);
    cardLayout->addWidget(content);
    cardLayout->addLayout(actionsRow);

    return card;
}

void MainWindow::loadPosts() {
    // Clear existing
    QLayoutItem* item;
    while ((item = postsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // Latest post upar, oldest neeche — reverse order
    for (int i = loggedInUser->getPostCount() - 1; i >= 0; i--) {
        Posts* post = loggedInUser->getPostByIndex(i);
        if (post == nullptr) continue;
        postsLayout->addWidget(createPostCard(post));
    }

    if (loggedInUser->getPostCount() == 0) {
        QLabel* noPost = new QLabel("No posts yet. Create your first post!");
        noPost->setAlignment(Qt::AlignCenter);
        noPost->setStyleSheet("color: " + subText + "; font-size: 15px;");
        postsLayout->addWidget(noPost);
    }

    postsLayout->addStretch();
}

void MainWindow::onFollowersClicked() {
    // Followers list window
    QWidget* w = new QWidget();
    w->setWindowTitle("Followers");
    w->setFixedSize(300, 400);
    w->setStyleSheet("background-color: #1a1a2e;");
    QVBoxLayout* l = new QVBoxLayout(w);
    QLabel* title = new QLabel("Followers");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    l->addWidget(title);
    // TODO: list followers
    w->show();
}

void MainWindow::onFollowingClicked() {
    QWidget* w = new QWidget();
    w->setWindowTitle("Following");
    w->setFixedSize(300, 400);
    w->setStyleSheet("background-color: #1a1a2e;");
    QVBoxLayout* l = new QVBoxLayout(w);
    QLabel* title = new QLabel("Following");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    l->addWidget(title);
    // TODO: list following
    w->show();
}

void MainWindow::onUpdateBioClicked() {
    UpdateBioWindow* w = new UpdateBioWindow(loggedInUser);
    w->show();
}

void MainWindow::onUpdatePasswordClicked() {
    UpdatePasswordWindow* w = new UpdatePasswordWindow(loggedInUser);
    w->show();
}

void MainWindow::onCreatePostClicked() {
    CreatePostWindow* w = new CreatePostWindow(loggedInUser, this);
    connect(w, &CreatePostWindow::postCreated, this, &MainWindow::loadPosts);
    w->show();
}

void MainWindow::onLogoutClicked() {
    loggedInUser->logOut();
    LoginWindow* loginWin = new LoginWindow(allUsers, userCount);
    loginWin->show();
    this->close();
}