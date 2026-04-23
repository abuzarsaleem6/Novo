#include "QtWidgetsApplication1.h"
#include "User.h"
#include "Post.h"
#include "Comment.h"
#include "Feed.h"
#include "PasswordChecker.h"
#include "Notification.h"
#include "SearchEngine.h"
#include <QApplication>
#include <QStyle>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>

// ═══════════════════════════════════════════════════════════════════════════════
//  APP NAME & GLOBAL STYLESHEET
// ═══════════════════════════════════════════════════════════════════════════════
static const QString APP_NAME = "Novo";

static const QString APP_STYLE = R"(
* {
    font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;
    color: #E2E2EC;
    box-sizing: border-box;
}
QMainWindow, QWidget#appShell, QWidget#pageArea {
    background: #0C0C10;
}
QWidget#sidebar {
    background: #101016;
    border-right: 1px solid #1C1C28;
    min-width: 215px;
    max-width: 215px;
}
QPushButton#sidebarBtn {
    background: transparent;
    border: none;
    border-radius: 9px;
    color: #606080;
    padding: 11px 14px;
    text-align: left;
    font-size: 13px;
}
QPushButton#sidebarBtn:hover {
    background: #17171F;
    color: #B0B0D8;
}
QPushButton#sidebarBtn[active="true"] {
    background: #1B1B2A;
    color: #7C7CFF;
    font-weight: 600;
    border-left: 3px solid #5050F0;
    padding-left: 11px;
}
QPushButton#sidebarBtnDanger {
    background: transparent;
    border: none;
    border-radius: 9px;
    color: #7A4040;
    padding: 11px 14px;
    text-align: left;
    font-size: 13px;
}
QPushButton#sidebarBtnDanger:hover {
    background: #1C0D0D;
    color: #FF7070;
}
QLineEdit, QTextEdit {
    background: #141420;
    border: 1px solid #222235;
    border-radius: 9px;
    padding: 9px 13px;
    color: #E2E2EC;
    font-size: 13px;
}
QLineEdit:focus, QTextEdit:focus {
    border: 1px solid #4545CC;
    background: #18182A;
}
QPushButton#primaryBtn {
    background: #4040E0;
    border: none;
    border-radius: 9px;
    color: #FFFFFF;
    padding: 10px 26px;
    font-size: 13px;
    font-weight: 600;
}
QPushButton#primaryBtn:hover   { background: #5252F0; }
QPushButton#primaryBtn:pressed { background: #3030BB; }
QPushButton#plusCircleBtn {
    background: #4040E0;
    border: none;
    border-radius: 22px;
    color: #FFFFFF;
    font-size: 24px;
    font-weight: bold;
}
QPushButton#plusCircleBtn:hover { background: #5252F0; }
QPushButton#secondaryBtn {
    background: transparent;
    border: 1px solid #2A2A48;
    border-radius: 9px;
    color: #7070BB;
    padding: 9px 22px;
    font-size: 13px;
}
QPushButton#secondaryBtn:hover { background: #18182A; color: #AAAAEE; border-color: #4444AA; }
QPushButton#dangerBtn {
    background: #200808;
    border: 1px solid #4A2020;
    border-radius: 9px;
    color: #BB5555;
    padding: 9px 22px;
    font-size: 13px;
}
QPushButton#dangerBtn:hover { background: #300C0C; color: #EE7777; border-color: #774444; }
QPushButton#iconBtn {
    background: transparent;
    border: none;
    color: #44446A;
    font-size: 16px;
    padding: 5px 9px;
    border-radius: 7px;
}
QPushButton#iconBtn:hover { background: #18182A; color: #7777CC; }
QFrame#postCard {
    background: #111119;
    border: 1px solid #1C1C2A;
    border-radius: 13px;
}
QFrame#composerCard {
    background: #0F0F1A;
    border: 1px solid #2A2A50;
    border-radius: 13px;
}
QFrame#notifItem {
    background: #111119;
    border: 1px solid #1C1C2A;
    border-radius: 9px;
}
QFrame#notifItem[unread="true"] {
    border-left: 3px solid #5050F0;
    background: #12121E;
}
QFrame#resultCard {
    background: #111119;
    border: 1px solid #1C1C2A;
    border-radius: 12px;
}
QLabel#pageTitle   { font-size: 20px; font-weight: 700; color: #E8E8F8; }
QLabel#sectionLbl  { font-size: 10px; font-weight: 600; color: #383858; letter-spacing: 1.4px; }
QLabel#postAuthor  { font-size: 13px; font-weight: 600; color: #CCCCEE; }
QLabel#postTime    { font-size: 11px; color: #3A3A58; }
QLabel#postContent { font-size: 13px; color: #B8B8D0; }
QLabel#timerDisp   { font-size: 52px; font-weight: 700; color: #5555EE; letter-spacing: -2px; }
QLabel#brandTitle  { font-size: 34px; font-weight: 800; color: #6060FF; letter-spacing: -1px; }
QLabel#brandSub    { font-size: 12px; color: #36365A; }
QLabel#authTitle   { font-size: 19px; font-weight: 700; color: #DEDEFF; }
QFrame#authCard    { background: #0F0F18; border: 1px solid #1C1C2E; border-radius: 18px; }
QWidget#authBg     { background: #0C0C10; }
QScrollArea        { border: none; background: transparent; }
QScrollBar:vertical { background: #0C0C10; width: 5px; border-radius: 3px; margin: 0; }
QScrollBar::handle:vertical { background: #222234; border-radius: 3px; min-height: 28px; }
QScrollBar::handle:vertical:hover { background: #3A3AFF; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
)";

// ═══════════════════════════════════════════════════════════════════════════════
//  STATIC HELPERS
// ═══════════════════════════════════════════════════════════════════════════════

static QPushButton* makePrimary(const QString& t, QWidget* p = nullptr) {
    auto* b = new QPushButton(t, p); b->setObjectName("primaryBtn"); b->setCursor(Qt::PointingHandCursor); return b;
}
static QPushButton* makeSecondary(const QString& t, QWidget* p = nullptr) {
    auto* b = new QPushButton(t, p); b->setObjectName("secondaryBtn"); b->setCursor(Qt::PointingHandCursor); return b;
}
static QPushButton* makeDanger(const QString& t, QWidget* p = nullptr) {
    auto* b = new QPushButton(t, p); b->setObjectName("dangerBtn"); b->setCursor(Qt::PointingHandCursor); return b;
}
static QPushButton* makeIcon(const QString& icon, QWidget* p = nullptr) {
    auto* b = new QPushButton(icon, p); b->setObjectName("iconBtn"); b->setCursor(Qt::PointingHandCursor); b->setFixedSize(34, 34); return b;
}
static QLabel* makeLabel(const QString& text, const QString& obj = {}, QWidget* p = nullptr) {
    auto* l = new QLabel(text, p); if (!obj.isEmpty()) l->setObjectName(obj); l->setWordWrap(true); return l;
}
static QFrame* divider(QWidget* p = nullptr) {
    auto* f = new QFrame(p); f->setFrameShape(QFrame::HLine); f->setFixedHeight(1); f->setStyleSheet("background:#1A1A28; border:none;"); return f;
}
static QLabel* avatar(const QString& initial, int size, QWidget* p = nullptr) {
    auto* l = new QLabel(initial, p);
    l->setFixedSize(size, size);
    l->setAlignment(Qt::AlignCenter);
    l->setStyleSheet(QString("background:#202040; border-radius:%1px; font-size:%2px; font-weight:700; color:#7070FF;").arg(size / 2).arg(size / 3));
    return l;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  SidebarButton
// ═══════════════════════════════════════════════════════════════════════════════

SidebarButton::SidebarButton(const QString& icon, const QString& label, QWidget* parent) : QPushButton(parent) {
    setObjectName("sidebarBtn");
    setText(icon + "   " + label);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(46);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setProperty("active", false);
}
void SidebarButton::setActive(bool active) {
    setProperty("active", active);
    if (style()) { style()->unpolish(this); style()->polish(this); }
    update();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  PostCard
// ═══════════════════════════════════════════════════════════════════════════════

PostCard::PostCard(Posts* post, const QString& authorUsername, bool isOwner, QWidget* parent)
    : QFrame(parent), m_post(post), m_authorUsername(authorUsername), m_isOwner(isOwner) {
    setObjectName("postCard");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 16, 18, 12); root->setSpacing(10);

    auto* headerRow = new QHBoxLayout; headerRow->setSpacing(0);
    QString initial = m_authorUsername.isEmpty() ? "?" : m_authorUsername.left(1).toUpper();
    headerRow->addWidget(avatar(initial, 36)); headerRow->addSpacing(10);
    auto* col = new QVBoxLayout; col->setSpacing(2);
    col->addWidget(makeLabel(m_authorUsername, "postAuthor"));
    col->addWidget(makeLabel("Post " + QString::fromStdString(post->getPostId()), "postTime"));
    headerRow->addLayout(col); headerRow->addStretch();

    if (m_isOwner) {
        auto* delBtn = makeIcon("✕");
        delBtn->setStyleSheet("QPushButton{background:transparent;border:none;color:#553333;font-size:12px;} QPushButton:hover{color:#EE6666;}");
        connect(delBtn, &QPushButton::clicked, this, [this]() { emit deleteClicked(m_post); });
        headerRow->addWidget(delBtn);
    }
    root->addLayout(headerRow); root->addWidget(divider());

    auto* contentLbl = makeLabel(QString::fromStdString(post->getContent()), "postContent");
    contentLbl->setWordWrap(true); root->addWidget(contentLbl);

    auto* actRow = new QHBoxLayout; actRow->setSpacing(6);
    auto* likeBtn = makeIcon("♥");
    auto* likeCnt = new QLabel(QString::number(post->getLikeCount())); likeCnt->setStyleSheet("color:#44446A; font-size:12px;");
    auto* cmtBtn = makeIcon("✦");
    auto* cmtCnt = new QLabel(QString::number(post->getComments().size())); cmtCnt->setStyleSheet("color:#44446A; font-size:12px;");

    connect(likeBtn, &QPushButton::clicked, this, [this, likeCnt]() {
        m_post->likePost();
        likeCnt->setText(QString::number(m_post->getLikeCount()));
        emit likeClicked(m_post);
        });
    connect(cmtBtn, &QPushButton::clicked, this, [this]() { emit commentClicked(m_post); });

    actRow->addWidget(likeBtn); actRow->addWidget(likeCnt); actRow->addSpacing(10);
    actRow->addWidget(cmtBtn);  actRow->addWidget(cmtCnt);  actRow->addStretch();
    root->addLayout(actRow);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  NotificationItem
// ═══════════════════════════════════════════════════════════════════════════════

NotificationItem::NotificationItem(const Notification& notif, QWidget* parent) : QFrame(parent) {
    setObjectName("notifItem");
    setProperty("unread", !notif.getStatus());

    auto* row = new QHBoxLayout(this); row->setContentsMargins(16, 13, 16, 13); row->setSpacing(12);
    QString typeIcon = "●";
    QString t = QString::fromStdString(notif.getType()).toLower();
    if (t.contains("follow")) typeIcon = "👤";
    else if (t.contains("like")) typeIcon = "♥";
    else if (t.contains("comment")) typeIcon = "✦";

    auto* ic = new QLabel(typeIcon); ic->setFixedSize(32, 32); ic->setAlignment(Qt::AlignCenter); ic->setStyleSheet("font-size:16px;");
    auto* col = new QVBoxLayout; col->setSpacing(3);
    auto* msg = makeLabel(QString::fromStdString(notif.getMessage())); msg->setStyleSheet("font-size:13px; color:#C0C0DC;");
    auto* ts = makeLabel(QString::fromStdString(notif.getTimestamp()), "postTime");
    col->addWidget(msg); col->addWidget(ts);

    row->addWidget(ic); row->addLayout(col); row->addStretch();
    if (!notif.getStatus()) {
        auto* dot = new QLabel("●"); dot->setStyleSheet("color:#5050F0; font-size:9px;"); row->addWidget(dot);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  AuthPage
// ═══════════════════════════════════════════════════════════════════════════════

AuthPage::AuthPage(QWidget* parent) : QWidget(parent), m_userCount(0) {
    setObjectName("authBg");
    m_allUsers = new User * [500]();
    loadAllUsers(m_allUsers, m_userCount);

    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame; card->setObjectName("authCard"); card->setFixedWidth(420);
    auto* cl = new QVBoxLayout(card); cl->setContentsMargins(40, 36, 40, 36);

    auto* brandTitle = makeLabel(APP_NAME, "brandTitle"); brandTitle->setAlignment(Qt::AlignCenter);
    auto* brandSub = makeLabel("Connect. Share. Discover.", "brandSub"); brandSub->setAlignment(Qt::AlignCenter);
    cl->addWidget(brandTitle); cl->addWidget(brandSub); cl->addSpacing(30);

    m_stack = new QStackedWidget;
    m_stack->addWidget(createLandingWidget());    // 0
    m_stack->addWidget(createLoginWidget());      // 1
    m_stack->addWidget(createSignupWidget());     // 2
    m_stack->addWidget(createAdminLoginWidget()); // 3
    cl->addWidget(m_stack);

    outer->addWidget(card);
}

// FIX 1: resetToLogin reloads users so a fresh login always works after logout
void AuthPage::resetToLogin() {
    m_loginUser->clear(); m_loginPass->clear();
    m_signupUser->clear(); m_signupPass->clear(); m_signupBio->clear();
    m_stack->setCurrentIndex(0);
    m_userCount = 0;
    loadAllUsers(m_allUsers, m_userCount);
}

QWidget* AuthPage::createLandingWidget() {
    auto* w = new QWidget; auto* vl = new QVBoxLayout(w); vl->setSpacing(15); vl->setContentsMargins(0, 0, 0, 0);

    auto* btnLogin = makePrimary("Login as User");      btnLogin->setFixedHeight(44);
    auto* btnSignup = makeSecondary("Create New Account"); btnSignup->setFixedHeight(44);
    auto* btnAdmin = makeDanger("Login as Admin");      btnAdmin->setFixedHeight(44);

    connect(btnLogin, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(1); });
    connect(btnSignup, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(2); });
    connect(btnAdmin, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(3); });

    vl->addWidget(btnLogin); vl->addWidget(btnSignup); vl->addWidget(divider()); vl->addWidget(btnAdmin);
    return w;
}

QWidget* AuthPage::createLoginWidget() {
    auto* w = new QWidget; auto* vl = new QVBoxLayout(w); vl->setSpacing(12); vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(makeLabel("User Login", "authTitle"));

    m_loginUser = new QLineEdit; m_loginUser->setPlaceholderText("Username"); m_loginUser->setFixedHeight(42);
    m_loginPass = new QLineEdit; m_loginPass->setPlaceholderText("Password"); m_loginPass->setEchoMode(QLineEdit::Password); m_loginPass->setFixedHeight(42);
    vl->addWidget(m_loginUser); vl->addWidget(m_loginPass);

    auto* loginBtn = makePrimary("Sign In"); loginBtn->setFixedHeight(44);
    connect(loginBtn, &QPushButton::clicked, this, &AuthPage::onLogin);
    connect(m_loginPass, &QLineEdit::returnPressed, this, &AuthPage::onLogin);
    vl->addWidget(loginBtn);

    auto* backBtn = makeSecondary("← Back");
    connect(backBtn, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(0); });
    vl->addWidget(backBtn);
    return w;
}

QWidget* AuthPage::createSignupWidget() {
    auto* w = new QWidget; auto* vl = new QVBoxLayout(w); vl->setSpacing(12); vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(makeLabel("Create Account", "authTitle"));

    m_signupUser = new QLineEdit; m_signupUser->setPlaceholderText("Username"); m_signupUser->setFixedHeight(42);
    m_signupPass = new QLineEdit; m_signupPass->setPlaceholderText("Password (8+ chars, upper, digit, symbol)"); m_signupPass->setEchoMode(QLineEdit::Password); m_signupPass->setFixedHeight(42);
    m_signupBio = new QLineEdit; m_signupBio->setPlaceholderText("Short bio (optional)"); m_signupBio->setFixedHeight(42);
    vl->addWidget(m_signupUser); vl->addWidget(m_signupPass); vl->addWidget(m_signupBio);

    auto* signupBtn = makePrimary("Register"); signupBtn->setFixedHeight(44);
    connect(signupBtn, &QPushButton::clicked, this, &AuthPage::onSignUp);
    vl->addWidget(signupBtn);

    auto* backBtn = makeSecondary("← Back");
    connect(backBtn, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(0); });
    vl->addWidget(backBtn);
    return w;
}

QWidget* AuthPage::createAdminLoginWidget() {
    auto* w = new QWidget; auto* vl = new QVBoxLayout(w); vl->setSpacing(12); vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(makeLabel("Admin Console Portal", "authTitle"));

    QLineEdit* adminUser = new QLineEdit; adminUser->setPlaceholderText("Admin ID");  adminUser->setFixedHeight(42);
    QLineEdit* adminPass = new QLineEdit; adminPass->setPlaceholderText("Passcode");  adminPass->setEchoMode(QLineEdit::Password); adminPass->setFixedHeight(42);
    vl->addWidget(adminUser); vl->addWidget(adminPass);

    auto* loginBtn = makeDanger("Enter System"); loginBtn->setFixedHeight(44);
    connect(loginBtn, &QPushButton::clicked, this, [this, adminUser, adminPass]() {
        if (adminUser->text() == "admin" && adminPass->text() == "admin123")
            emit loginAdminSuccess();
        else
            QMessageBox::critical(this, APP_NAME, "Invalid Admin Credentials.");
        });
    vl->addWidget(loginBtn);

    auto* backBtn = makeSecondary("← Back");
    connect(backBtn, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(0); });
    vl->addWidget(backBtn);
    return w;
}

void AuthPage::onLogin() {
    QString uname = m_loginUser->text().trimmed();
    QString pass = m_loginPass->text();
    if (uname.isEmpty() || pass.isEmpty()) { QMessageBox::warning(this, APP_NAME, "Please fill in all fields."); return; }

    User* user = findAndLogin(m_allUsers, m_userCount, uname.toStdString(), pass.toStdString());
    if (user) {
        user->loadAllPosts();
        user->loadFollowing(m_allUsers, m_userCount);
        user->loadFollowers(m_allUsers, m_userCount);
        m_loginUser->clear(); m_loginPass->clear();
        emit loginSuccess(user, m_allUsers, m_userCount);
        // FIX 1: DO NOT hide self here — MainWindow::onLoginSuccess handles hiding
    }
    else {
        QMessageBox::critical(this, APP_NAME, "Invalid username or password.");
    }
}

void AuthPage::onSignUp() {
    QString uname = m_signupUser->text().trimmed();
    QString pass = m_signupPass->text();
    QString bio = m_signupBio->text().trimmed();
    if (uname.isEmpty() || pass.isEmpty()) { QMessageBox::warning(this, APP_NAME, "Username and password are required."); return; }

    PasswordChecker checker(8);
    if (!checker.checkAll(pass.toStdString())) {
        int strength = checker.calculateStrength(pass.toStdString());
        QMessageBox::warning(this, "Weak Password", QString("Password strength: %1 / 100\nRequirements: 8+ chars, uppercase, digit, special char").arg(strength));
        return;
    }
    User* newUser = signUp(m_allUsers, m_userCount, uname.toStdString(), pass.toStdString(), bio.toStdString());
    if (newUser) {
        QMessageBox::information(this, APP_NAME, "Account created! You can now sign in.");
        m_signupUser->clear(); m_signupPass->clear(); m_signupBio->clear();
        m_stack->setCurrentIndex(1);
    }
    else {
        QMessageBox::critical(this, APP_NAME, "Username already taken or invalid.");
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  FeedPage
//  FIX 3: loadPosts() no longer adds the current user's own posts
//  FIX 4: Inline post composer instead of QInputDialog popup
// ═══════════════════════════════════════════════════════════════════════════════

FeedPage::FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount) {

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(36, 30, 36, 20);
    outer->setSpacing(20);

    auto* hRow = new QHBoxLayout;
    hRow->addWidget(makeLabel("Feed", "pageTitle"));
    hRow->addStretch();
    outer->addLayout(hRow);

    // ── FIX 4: Inline composer card (hidden by default) ──────────────────────
    m_composerCard = new QFrame;
    m_composerCard->setObjectName("composerCard");
    m_composerCard->hide(); // hidden until "+" is clicked
    auto* ccl = new QVBoxLayout(m_composerCard);
    ccl->setContentsMargins(18, 16, 18, 14); ccl->setSpacing(10);

    auto* composerHeader = new QHBoxLayout;
    QString ini = currentUser ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper() : "?";
    composerHeader->addWidget(avatar(ini, 32));
    composerHeader->addSpacing(10);
    composerHeader->addWidget(makeLabel("What's on your mind?", "postAuthor"));
    composerHeader->addStretch();

    // Close / cancel button
    auto* closeBtn = makeIcon("✕");
    closeBtn->setStyleSheet("QPushButton{background:transparent;border:none;color:#553333;font-size:12px;} QPushButton:hover{color:#EE6666;}");
    connect(closeBtn, &QPushButton::clicked, this, [this]() { m_postInput->clear(); m_composerCard->hide(); });
    composerHeader->addWidget(closeBtn);
    ccl->addLayout(composerHeader);

    m_postInput = new QTextEdit;
    m_postInput->setPlaceholderText("Share something...");
    m_postInput->setFixedHeight(90);
    ccl->addWidget(m_postInput);

    auto* btnRow = new QHBoxLayout; btnRow->addStretch();
    auto* postBtn = makePrimary("Post");
    postBtn->setFixedWidth(90);
    connect(postBtn, &QPushButton::clicked, this, &FeedPage::onSubmitPost);
    btnRow->addWidget(postBtn);
    ccl->addLayout(btnRow);

    outer->addWidget(m_composerCard);
    // ─────────────────────────────────────────────────────────────────────────

    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_feedContent = new QWidget;
    m_feedLayout = new QVBoxLayout(m_feedContent);
    m_feedLayout->setContentsMargins(0, 0, 6, 0);
    m_feedLayout->setSpacing(12);
    m_feedLayout->addStretch();

    m_scrollArea->setWidget(m_feedContent);
    outer->addWidget(m_scrollArea);
    loadPosts();
}

// FIX 4: Submit post inline, no popup
void FeedPage::onSubmitPost() {
    if (!m_user) return;
    QString text = m_postInput->toPlainText().trimmed();
    if (text.isEmpty()) return;
    m_user->createPost(text.toStdString());
    m_postInput->clear();
    m_composerCard->hide();
    loadPosts();
}

void FeedPage::refresh() { loadPosts(); }

void FeedPage::clearFeed() {
    while (m_feedLayout->count() > 1) {
        QLayoutItem* item = m_feedLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void FeedPage::loadPosts() {
    clearFeed();
    if (!m_user) return;

    bool any = false;

    // FIX 3: Only show posts from people the current user FOLLOWS
    //        Own posts are intentionally excluded from the feed.
    for (int u = 0; u < m_userCount; ++u) {
        if (!m_allUsers[u] || m_allUsers[u] == m_user) continue;
        if (!m_user->isFollowing(m_allUsers[u]->getUsername())) continue;

        m_allUsers[u]->loadAllPosts();
        for (int i = m_allUsers[u]->getPostCount() - 1; i >= 0; --i) {
            Posts* p = m_allUsers[u]->getPostByIndex(i);
            if (!p) continue;
            auto* card = new PostCard(p, QString::fromStdString(m_allUsers[u]->getUsername()), false);
            connect(card, &PostCard::likeClicked, this, &FeedPage::onLikePost);
            connect(card, &PostCard::commentClicked, this, &FeedPage::onCommentPost);
            connect(card, &PostCard::deleteClicked, this, &FeedPage::onDeletePost);
            m_feedLayout->insertWidget(m_feedLayout->count() - 1, card);
            any = true;
        }
    }

    if (!any) {
        auto* empty = new QLabel("Follow someone to see their posts here!");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A; font-size:14px; padding:40px 0;");
        m_feedLayout->insertWidget(0, empty);
    }
}

void FeedPage::onLikePost(Posts* /*post*/) { /* Like count updated live inside PostCard */ }

void FeedPage::onCommentPost(Posts* post) {
    if (!post) return;
    // FIX 2: Use a simple non-blocking inline approach with QInputDialog
    // (QInputDialog is acceptable for comments; it is only the create-post
    //  flow that needed to be inline per the requirements)
    bool ok;
    QString txt = QInputDialog::getText(this, "Add Comment", "Your comment:", QLineEdit::Normal, "", &ok);
    if (ok && !txt.trimmed().isEmpty()) {
        post->addComment(txt.trimmed(), QString::fromStdString(m_user->getUsername()));
        refresh();
    }
}

void FeedPage::onDeletePost(Posts* post) {
    if (!post) return;
    auto r = QMessageBox::question(this, "Delete Post", "Permanently delete this post?", QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) { m_user->deletePost(post->getPostId()); loadPosts(); }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  NotificationsPage
// ═══════════════════════════════════════════════════════════════════════════════

NotificationsPage::NotificationsPage(User* currentUser, QWidget* parent) : QWidget(parent), m_user(currentUser) {
    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(36, 30, 36, 20); outer->setSpacing(20);
    auto* hRow = new QHBoxLayout; hRow->addWidget(makeLabel("Notifications", "pageTitle")); hRow->addStretch();
    auto* refBtn = makeSecondary("↻  Refresh"); refBtn->setFixedWidth(105);
    connect(refBtn, &QPushButton::clicked, this, &NotificationsPage::refresh);
    hRow->addWidget(refBtn); outer->addLayout(hRow);

    m_scrollArea = new QScrollArea; m_scrollArea->setWidgetResizable(true); m_scrollArea->setFrameShape(QFrame::NoFrame); m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listContent = new QWidget; m_listLayout = new QVBoxLayout(m_listContent); m_listLayout->setContentsMargins(0, 0, 6, 0); m_listLayout->setSpacing(9); m_listLayout->addStretch();
    m_scrollArea->setWidget(m_listContent); outer->addWidget(m_scrollArea);
    loadNotifications();
}

void NotificationsPage::refresh() { loadNotifications(); }

void NotificationsPage::loadNotifications() {
    while (m_listLayout->count() > 1) { QLayoutItem* item = m_listLayout->takeAt(0); if (item->widget()) item->widget()->deleteLater(); delete item; }
    if (!m_user) return;

    QString path = "data/Notifications/" + QString::fromStdString(m_user->getUsername()) + "_notif.txt";
    QFile file(path); bool any = false;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed(); if (line.isEmpty()) continue;
            QStringList parts = line.split("|"); if (parts.size() < 4) continue;
            Notification notif(parts[0].toStdString(), parts[1].toStdString(), parts[2].toStdString());
            if (parts[3] == "1") notif.markAsRead();
            auto* item = new NotificationItem(notif); m_listLayout->insertWidget(m_listLayout->count() - 1, item); any = true;
        }
        file.close();
    }
    if (!any) {
        auto* empty = new QLabel("No notifications yet!");
        empty->setAlignment(Qt::AlignCenter); empty->setStyleSheet("color:#28284A; font-size:14px; padding:40px 0;");
        m_listLayout->insertWidget(0, empty);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  SearchPage
// ═══════════════════════════════════════════════════════════════════════════════

SearchPage::SearchPage(User** allUsers, int userCount, User* currentUser, QWidget* parent)
    : QWidget(parent), m_currentUser(currentUser), m_allUsers(allUsers), m_userCount(userCount), m_foundUser(nullptr), m_followBtn(nullptr) {

    for (int i = 0; i < m_userCount; ++i) if (m_allUsers[i]) m_engine.addUser(m_allUsers[i]);

    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(36, 30, 36, 20); outer->setSpacing(20);
    outer->addWidget(makeLabel("Search Users", "pageTitle"));

    auto* searchRow = new QHBoxLayout;
    m_searchInput = new QLineEdit; m_searchInput->setPlaceholderText("Search by username..."); m_searchInput->setFixedHeight(44);
    auto* searchBtn = makePrimary("Search"); searchBtn->setFixedHeight(44); searchBtn->setFixedWidth(100);
    connect(searchBtn, &QPushButton::clicked, this, &SearchPage::onSearch);
    connect(m_searchInput, &QLineEdit::returnPressed, this, &SearchPage::onSearch);
    searchRow->addWidget(m_searchInput); searchRow->addSpacing(10); searchRow->addWidget(searchBtn);
    outer->addLayout(searchRow);

    m_scrollArea = new QScrollArea; m_scrollArea->setWidgetResizable(true); m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_resultsContent = new QWidget; m_resultsLayout = new QVBoxLayout(m_resultsContent);
    m_resultsLayout->setContentsMargins(0, 0, 6, 0); m_resultsLayout->setSpacing(12); m_resultsLayout->addStretch();
    m_scrollArea->setWidget(m_resultsContent); outer->addWidget(m_scrollArea);
}

void SearchPage::onSearch() {
    QString query = m_searchInput->text().trimmed(); if (query.isEmpty()) return;
    while (m_resultsLayout->count() > 1) { QLayoutItem* item = m_resultsLayout->takeAt(0); if (item->widget()) item->widget()->deleteLater(); delete item; }
    m_foundUser = nullptr; m_followBtn = nullptr;
    User* found = m_engine.searchUsers(query);
    if (found) { m_foundUser = found; showUserCard(found); }
    else {
        auto* nf = new QLabel("No user found for \"" + query + "\"");
        nf->setAlignment(Qt::AlignCenter); nf->setStyleSheet("color:#34345A; font-size:13px; padding:30px 0;");
        m_resultsLayout->insertWidget(0, nf);
    }
}

void SearchPage::showUserCard(User* user) {
    auto* card = new QFrame; card->setObjectName("resultCard");
    auto* cl = new QVBoxLayout(card); cl->setContentsMargins(22, 18, 22, 18); cl->setSpacing(14);

    auto* topRow = new QHBoxLayout;
    QString ini = QString::fromStdString(user->getUsername()).left(1).toUpper();
    topRow->addWidget(avatar(ini, 50)); topRow->addSpacing(14);
    auto* infoCol = new QVBoxLayout; infoCol->setSpacing(4);
    auto* nameLbl = makeLabel(QString::fromStdString(user->getUsername())); nameLbl->setStyleSheet("font-size:16px; font-weight:700; color:#DCDCF8;");
    auto* bioLbl = makeLabel(QString::fromStdString(user->getBio()));       bioLbl->setStyleSheet("font-size:12px; color:#56567A;");
    infoCol->addWidget(nameLbl); infoCol->addWidget(bioLbl);
    topRow->addLayout(infoCol); topRow->addStretch(); cl->addLayout(topRow);

    auto* statsRow = new QHBoxLayout;
    auto addStat = [&](const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout; c->setSpacing(2);
        auto* v = new QLabel(val); v->setStyleSheet("font-size:19px; font-weight:700; color:#6060FF;"); v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl); l->setStyleSheet("font-size:10px; color:#36365A; letter-spacing:1px;"); l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l); statsRow->addLayout(c); statsRow->addSpacing(28);
        };
    addStat(QString::number(user->getPostCount()), "POSTS");
    addStat(QString::number(user->getFollowersCount()), "FOLLOWERS");
    addStat(QString::number(user->getFollowingCount()), "FOLLOWING");
    statsRow->addStretch(); cl->addLayout(statsRow);

    if (user->getIsReported()) {
        auto* warn = new QLabel("⚠  This account has been reported");
        warn->setStyleSheet("color:#AA4040; font-size:12px; background:#180606; border-radius:6px; padding:6px 10px;");
        cl->addWidget(warn);
    }
    cl->addWidget(divider());

    if (m_currentUser && m_currentUser != user) {
        bool following = m_currentUser->isFollowing(user->getUsername());
        m_followBtn = following ? makeSecondary("✓  Following") : makePrimary("+ Follow");
        m_followBtn->setFixedWidth(130);
        connect(m_followBtn, &QPushButton::clicked, this, &SearchPage::onFollowUser);
        auto* btnRow = new QHBoxLayout; btnRow->addStretch(); btnRow->addWidget(m_followBtn);
        cl->addLayout(btnRow);
    }
    m_resultsLayout->insertWidget(0, card);
}

void SearchPage::onFollowUser() {
    if (!m_foundUser || !m_currentUser || !m_followBtn) return;
    if (m_currentUser->isFollowing(m_foundUser->getUsername())) {
        m_currentUser->unfollowUser(m_foundUser->getUsername());
        m_followBtn->setText("+ Follow"); m_followBtn->setObjectName("primaryBtn");
    }
    else {
        m_currentUser->followUser(m_foundUser, m_allUsers, m_userCount);
        m_followBtn->setText("✓  Following"); m_followBtn->setObjectName("secondaryBtn");
    }
    if (m_followBtn->style()) { m_followBtn->style()->unpolish(m_followBtn); m_followBtn->style()->polish(m_followBtn); }
    m_followBtn->update();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  MessagesPage
// ═══════════════════════════════════════════════════════════════════════════════

MessagesPage::MessagesPage(QWidget* parent) : QWidget(parent) {
    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(36, 30, 36, 20);
    outer->addWidget(makeLabel("Direct Messages", "pageTitle"));
    auto* emptyCard = new QFrame; emptyCard->setObjectName("postCard"); auto* l = new QVBoxLayout(emptyCard);
    auto* lbl = makeLabel("Select a conversation to start chatting...\n(Messaging implementation goes here)");
    lbl->setAlignment(Qt::AlignCenter); lbl->setStyleSheet("color:#56567A; padding: 100px;"); l->addWidget(lbl);
    outer->addWidget(emptyCard); outer->addStretch();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  AdminPage
// ═══════════════════════════════════════════════════════════════════════════════

AdminPage::AdminPage(QWidget* parent) : QWidget(parent) {
    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(36, 30, 36, 20);
    outer->addWidget(makeLabel("Admin Dashboard", "pageTitle"));
    auto* panel = new QFrame; panel->setObjectName("postCard"); auto* l = new QVBoxLayout(panel);
    l->addWidget(makeLabel("Welcome to the Admin Control Panel."));
    l->addWidget(makePrimary("Review Reported Posts"));
    l->addWidget(makeSecondary("Manage Users"));
    outer->addWidget(panel); outer->addStretch();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  ProfilePage
// ═══════════════════════════════════════════════════════════════════════════════

ProfilePage::ProfilePage(User* currentUser, User** allUsers, int* userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCountPtr(userCount) {

    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(36, 30, 36, 20); outer->setSpacing(0);
    auto* scroll = new QScrollArea; scroll->setWidgetResizable(true); scroll->setFrameShape(QFrame::NoFrame);
    auto* inner = new QWidget; auto* layout = new QVBoxLayout(inner);
    layout->setContentsMargins(0, 0, 6, 0); layout->setSpacing(16);
    layout->addWidget(makeLabel("Profile", "pageTitle"));

    // Info card
    auto* pCard = new QFrame; pCard->setObjectName("postCard"); auto* pcl = new QVBoxLayout(pCard);
    pcl->setContentsMargins(24, 20, 24, 20); pcl->setSpacing(16);
    auto* infoRow = new QHBoxLayout;
    QString ini = currentUser ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper() : "?";
    infoRow->addWidget(avatar(ini, 64)); infoRow->addSpacing(16);
    auto* infoCol = new QVBoxLayout; infoCol->setSpacing(5);
    m_usernameLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getUsername()) : "");
    m_usernameLabel->setStyleSheet("font-size:20px; font-weight:800; color:#E0E0FF;");
    m_bioLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getBio()) : "");
    m_bioLabel->setStyleSheet("font-size:13px; color:#56567A;");
    infoCol->addWidget(m_usernameLabel); infoCol->addWidget(m_bioLabel);
    infoRow->addLayout(infoCol); infoRow->addStretch(); pcl->addLayout(infoRow);

    auto* statsRow = new QHBoxLayout;
    auto addStat = [&](const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout; c->setSpacing(2);
        auto* v = new QLabel(val); v->setStyleSheet("font-size:22px; font-weight:700; color:#6060FF;"); v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl); l->setStyleSheet("font-size:10px; color:#36365A; letter-spacing:1px;"); l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l); statsRow->addLayout(c); statsRow->addStretch();
        };
    if (currentUser) {
        addStat(QString::number(currentUser->getPostCount()), "POSTS");
        addStat(QString::number(currentUser->getFollowersCount()), "FOLLOWERS");
        addStat(QString::number(currentUser->getFollowingCount()), "FOLLOWING");
    }
    pcl->addLayout(statsRow); layout->addWidget(pCard);

    // Bio card
    auto* bioCard = new QFrame; bioCard->setObjectName("postCard"); auto* bcl = new QVBoxLayout(bioCard);
    bcl->setContentsMargins(22, 18, 22, 18); bcl->setSpacing(10);
    bcl->addWidget(makeLabel("UPDATE BIO", "sectionLbl"));
    m_newBioInput = new QLineEdit; m_newBioInput->setPlaceholderText("New bio..."); m_newBioInput->setFixedHeight(42);
    bcl->addWidget(m_newBioInput);
    auto* saveBioBtn = makePrimary("Save Bio"); saveBioBtn->setFixedWidth(100);
    connect(saveBioBtn, &QPushButton::clicked, this, &ProfilePage::onUpdateBio);
    auto* bioRow = new QHBoxLayout; bioRow->addStretch(); bioRow->addWidget(saveBioBtn); bcl->addLayout(bioRow);
    layout->addWidget(bioCard);

    // Password card
    auto* passCard = new QFrame; passCard->setObjectName("postCard"); auto* playout = new QVBoxLayout(passCard);
    playout->setContentsMargins(22, 18, 22, 18); playout->setSpacing(10);
    playout->addWidget(makeLabel("CHANGE PASSWORD", "sectionLbl"));
    m_newPassInput = new QLineEdit; m_newPassInput->setPlaceholderText("New password..."); m_newPassInput->setEchoMode(QLineEdit::Password); m_newPassInput->setFixedHeight(42);
    playout->addWidget(m_newPassInput);
    auto* savePassBtn = makePrimary("Update Password"); savePassBtn->setFixedWidth(150);
    connect(savePassBtn, &QPushButton::clicked, this, &ProfilePage::onUpdatePassword);
    auto* passRow2 = new QHBoxLayout; passRow2->addStretch(); passRow2->addWidget(savePassBtn); playout->addLayout(passRow2);
    layout->addWidget(passCard);

    // Danger zone
    auto* dangerCard = new QFrame; dangerCard->setObjectName("postCard");
    dangerCard->setStyleSheet("QFrame#postCard { border:1px solid #3A1212; background:#0E0606; }");
    auto* dl = new QVBoxLayout(dangerCard); dl->setContentsMargins(22, 18, 22, 18); dl->setSpacing(10);
    auto* dtitle = makeLabel("DANGER ZONE", "sectionLbl"); dtitle->setStyleSheet("font-size:10px; font-weight:600; color:#5A2222; letter-spacing:1.4px;");
    auto* ddesc = makeLabel("Permanently deletes your account and all associated data."); ddesc->setStyleSheet("font-size:12px; color:#4A3030;");
    auto* delBtn = makeDanger("Delete My Account"); delBtn->setFixedWidth(170);
    connect(delBtn, &QPushButton::clicked, this, &ProfilePage::onDeleteAccount);
    auto* delRow = new QHBoxLayout; delRow->addStretch(); delRow->addWidget(delBtn);
    dl->addWidget(dtitle); dl->addWidget(ddesc); dl->addLayout(delRow);
    layout->addWidget(dangerCard);

    layout->addStretch(); scroll->setWidget(inner); outer->addWidget(scroll);
}

void ProfilePage::refresh() {
    if (!m_user) return;
    m_usernameLabel->setText(QString::fromStdString(m_user->getUsername()));
    m_bioLabel->setText(QString::fromStdString(m_user->getBio()));
}
void ProfilePage::onUpdateBio() {
    if (!m_user) return;
    QString bio = m_newBioInput->text().trimmed();
    if (bio.isEmpty()) { QMessageBox::warning(this, APP_NAME, "Bio cannot be empty."); return; }
    m_user->setBio(bio.toStdString()); m_user->saveToFile();
    m_bioLabel->setText(bio); m_newBioInput->clear();
    QMessageBox::information(this, APP_NAME, "Bio updated.");
}
void ProfilePage::onUpdatePassword() {
    if (!m_user) return;
    QString pass = m_newPassInput->text();
    if (pass.isEmpty()) { QMessageBox::warning(this, APP_NAME, "Password cannot be empty."); return; }
    PasswordChecker checker(8);
    if (!checker.checkAll(pass.toStdString())) {
        int s = checker.calculateStrength(pass.toStdString());
        QMessageBox::warning(this, "Weak Password", QString("Strength: %1/100. Needs uppercase, digit & special character.").arg(s));
        return;
    }
    m_user->setPassword(pass.toStdString()); m_user->saveToFile();
    m_newPassInput->clear();
    QMessageBox::information(this, APP_NAME, "Password updated.");
}
void ProfilePage::onDeleteAccount() {
    auto r = QMessageBox::warning(this, "Delete Account", "This will permanently delete your account.\nAre you absolutely sure?", QMessageBox::Yes | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) {
        m_user->deleteAccount(m_allUsers, *m_userCountPtr);
        QMessageBox::information(this, APP_NAME, "Account deleted. The app will now exit.");
        QApplication::quit();
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  TimeSpentPage
// ═══════════════════════════════════════════════════════════════════════════════

TimeSpentPage::TimeSpentPage(QWidget* parent) : QWidget(parent), m_elapsed(0) {
    auto* outer = new QVBoxLayout(this); outer->setContentsMargins(36, 30, 36, 20); outer->setSpacing(20);
    outer->addWidget(makeLabel("Time Spent", "pageTitle"));

    auto* timerCard = new QFrame; timerCard->setObjectName("postCard");
    auto* tcl = new QVBoxLayout(timerCard); tcl->setContentsMargins(40, 36, 40, 36); tcl->setSpacing(14); tcl->setAlignment(Qt::AlignCenter);
    auto* clockIcon = new QLabel("⏱"); clockIcon->setAlignment(Qt::AlignCenter); clockIcon->setStyleSheet("font-size:44px;");
    tcl->addWidget(clockIcon);
    m_timerLabel = makeLabel("00:00:00", "timerDisp"); m_timerLabel->setAlignment(Qt::AlignCenter); tcl->addWidget(m_timerLabel);
    m_sessionLabel = makeLabel("Session started: —"); m_sessionLabel->setAlignment(Qt::AlignCenter); m_sessionLabel->setStyleSheet("color:#34345A; font-size:12px;");
    tcl->addWidget(m_sessionLabel); outer->addWidget(timerCard);

    auto* infoRow = new QHBoxLayout; infoRow->setSpacing(12);
    auto makeInfoCard = [&](const QString& ic, const QString& label, const QString& val) -> QFrame* {
        auto* f = new QFrame; f->setObjectName("postCard"); auto* fl = new QVBoxLayout(f);
        fl->setContentsMargins(18, 16, 18, 16); fl->setSpacing(6); fl->setAlignment(Qt::AlignCenter);
        auto* i = new QLabel(ic); i->setAlignment(Qt::AlignCenter); i->setStyleSheet("font-size:22px;");
        auto* v = new QLabel(val); v->setAlignment(Qt::AlignCenter); v->setStyleSheet("font-size:17px; font-weight:700; color:#6060FF;");
        auto* l = new QLabel(label); l->setAlignment(Qt::AlignCenter); l->setStyleSheet("font-size:10px; color:#34345A; letter-spacing:1px;");
        fl->addWidget(i); fl->addWidget(v); fl->addWidget(l); return f;
        };
    infoRow->addWidget(makeInfoCard("🎯", "TODAY'S GOAL", "2 hours"));
    infoRow->addWidget(makeInfoCard("📊", "AVG DAILY", "—"));
    infoRow->addWidget(makeInfoCard("🔥", "STREAK", "—"));
    outer->addLayout(infoRow); outer->addStretch();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &TimeSpentPage::onTick);
}
void TimeSpentPage::startSession() {
    m_elapsed = 0; m_sessionStart = QDateTime::currentDateTime();
    m_sessionLabel->setText("Session started: " + m_sessionStart.toString("hh:mm AP"));
    m_timer->start(1000);
}
void TimeSpentPage::stopSession() { m_timer->stop(); }
void TimeSpentPage::onTick() { ++m_elapsed; updateDisplay(); }
void TimeSpentPage::updateDisplay() {
    int h = m_elapsed / 3600, m = (m_elapsed % 3600) / 60, s = m_elapsed % 60;
    m_timerLabel->setText(QString("%1:%2:%3").arg(h, 2, 10, QChar('0')).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
}

// ═══════════════════════════════════════════════════════════════════════════════
//  MainWindow
// ═══════════════════════════════════════════════════════════════════════════════

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_authPage(nullptr), m_appShell(nullptr), m_shellLayout(nullptr), m_sidebar(nullptr),
    m_btnFeed(nullptr), m_btnNotifications(nullptr), m_btnSearch(nullptr), m_btnMessages(nullptr),
    m_btnProfile(nullptr), m_btnTimeSpent(nullptr), m_btnSwitchUser(nullptr), m_btnLogout(nullptr),
    m_pages(nullptr), m_feedPage(nullptr), m_notifPage(nullptr), m_searchPage(nullptr),
    m_messagesPage(nullptr), m_profilePage(nullptr), m_timeSpentPage(nullptr), m_adminPage(nullptr),
    m_currentUser(nullptr), m_allUsers(nullptr), m_userCount(0) {

    setWindowTitle(APP_NAME);
    setMinimumSize(960, 660);
    qApp->setStyleSheet(APP_STYLE);

    auto* central = new QWidget;
    setCentralWidget(central);
    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    m_authPage = new AuthPage;
    connect(m_authPage, &AuthPage::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(m_authPage, &AuthPage::loginAdminSuccess, this, &MainWindow::onLoginAdminSuccess);

    m_appShell = new QWidget;
    m_appShell->setObjectName("appShell");
    m_appShell->hide();

    rootLayout->addWidget(m_authPage);
    rootLayout->addWidget(m_appShell);
}

MainWindow::~MainWindow() {}

// FIX 1: Proper sequence — hide auth, build shell, show shell
void MainWindow::onLoginSuccess(User* user, User** allUsers, int userCount) {
    m_currentUser = user; m_allUsers = allUsers; m_userCount = userCount;

    // Build the shell layout fresh
    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0);
    m_shellLayout->setSpacing(0);

    buildSidebar();
    buildPages();

    m_shellLayout->addWidget(m_sidebar);
    m_shellLayout->addWidget(m_pages);

    // FIX 1: hide auth THEN show shell — no flicker, no lingering login form
    m_authPage->hide();
    m_appShell->show();

    m_timeSpentPage->startSession();
    onNavFeed();
}

void MainWindow::onLoginAdminSuccess() {
    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0); m_shellLayout->setSpacing(0);

    m_sidebar = new QWidget; m_sidebar->setObjectName("sidebar");
    auto* sl = new QVBoxLayout(m_sidebar);
    sl->addWidget(makeLabel("ADMINISTRATOR", "sectionLbl"));
    sl->addStretch();
    m_btnLogout = new SidebarButton("←", "Log Out");
    m_btnLogout->setObjectName("sidebarBtnDanger");
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(m_btnLogout);

    m_pages = new QStackedWidget;
    m_adminPage = new AdminPage;
    m_pages->addWidget(m_adminPage);

    m_shellLayout->addWidget(m_sidebar);
    m_shellLayout->addWidget(m_pages);

    m_authPage->hide();
    m_appShell->show();
}

void MainWindow::buildSidebar() {
    m_sidebar = new QWidget; m_sidebar->setObjectName("sidebar");
    m_sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    auto* sl = new QVBoxLayout(m_sidebar); sl->setContentsMargins(12, 22, 12, 18); sl->setSpacing(3);

    auto* brand = new QLabel(APP_NAME);
    brand->setStyleSheet("font-size:21px; font-weight:800; color:#6060FF; padding: 0 4px 14px 4px;");
    sl->addWidget(brand); sl->addWidget(divider()); sl->addSpacing(15);

    // FIX 4: "+" now shows inline composer on feed page
    QPushButton* plusBtn = new QPushButton("+");
    plusBtn->setObjectName("plusCircleBtn"); plusBtn->setFixedSize(44, 44); plusBtn->setCursor(Qt::PointingHandCursor);
    connect(plusBtn, &QPushButton::clicked, this, &MainWindow::onSidebarCreatePost);
    sl->addWidget(plusBtn, 0, Qt::AlignCenter); sl->addSpacing(20);

    auto* navLbl = makeLabel("NAVIGATE", "sectionLbl"); navLbl->setContentsMargins(4, 0, 0, 2); sl->addWidget(navLbl);

    m_btnFeed = new SidebarButton("⊞", "Feed");
    m_btnNotifications = new SidebarButton("◎", "Notifications");
    m_btnSearch = new SidebarButton("⊙", "Search");
    m_btnMessages = new SidebarButton("✉", "Messages");
    m_btnProfile = new SidebarButton("◉", "Profile");
    m_btnTimeSpent = new SidebarButton("⏱", "Time Spent");

    connect(m_btnFeed, &QPushButton::clicked, this, &MainWindow::onNavFeed);
    connect(m_btnNotifications, &QPushButton::clicked, this, &MainWindow::onNavNotifications);
    connect(m_btnSearch, &QPushButton::clicked, this, &MainWindow::onNavSearch);
    connect(m_btnMessages, &QPushButton::clicked, this, &MainWindow::onNavMessages);
    connect(m_btnProfile, &QPushButton::clicked, this, &MainWindow::onNavProfile);
    connect(m_btnTimeSpent, &QPushButton::clicked, this, &MainWindow::onNavTimeSpent);

    sl->addWidget(m_btnFeed); sl->addWidget(m_btnNotifications); sl->addWidget(m_btnSearch);
    sl->addWidget(m_btnMessages); sl->addWidget(m_btnProfile); sl->addWidget(m_btnTimeSpent);
    sl->addStretch(); sl->addWidget(divider()); sl->addSpacing(6);

    // Username row
    auto* userRow = new QHBoxLayout; userRow->setSpacing(8);
    QString ini = m_currentUser ? QString::fromStdString(m_currentUser->getUsername()).left(1).toUpper() : "?";
    userRow->addWidget(avatar(ini, 28));
    auto* uname = new QLabel(m_currentUser ? QString::fromStdString(m_currentUser->getUsername()) : "");
    uname->setStyleSheet("font-size:12px; color:#7070AA; font-weight:600;");
    uname->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    userRow->addWidget(uname); userRow->addStretch();
    sl->addLayout(userRow); sl->addSpacing(6);

    m_btnSwitchUser = new SidebarButton("⇄", "Switch User"); m_btnSwitchUser->setObjectName("sidebarBtnDanger");
    connect(m_btnSwitchUser, &QPushButton::clicked, this, &MainWindow::onSwitchUser);
    sl->addWidget(m_btnSwitchUser);

    m_btnLogout = new SidebarButton("←", "Log Out"); m_btnLogout->setObjectName("sidebarBtnDanger");
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(m_btnLogout);
}

void MainWindow::buildPages() {
    m_pages = new QStackedWidget; m_pages->setObjectName("pageArea");
    m_feedPage = new FeedPage(m_currentUser, m_allUsers, m_userCount);
    m_notifPage = new NotificationsPage(m_currentUser);
    m_searchPage = new SearchPage(m_allUsers, m_userCount, m_currentUser);
    m_messagesPage = new MessagesPage;
    m_profilePage = new ProfilePage(m_currentUser, m_allUsers, &m_userCount);
    m_timeSpentPage = new TimeSpentPage;

    m_pages->addWidget(m_feedPage);      // 0
    m_pages->addWidget(m_notifPage);     // 1
    m_pages->addWidget(m_searchPage);    // 2
    m_pages->addWidget(m_messagesPage);  // 3
    m_pages->addWidget(m_profilePage);   // 4
    m_pages->addWidget(m_timeSpentPage); // 5
}

// FIX 4: Instead of QInputDialog, navigate to feed and reveal the inline composer
void MainWindow::onSidebarCreatePost() {
    if (!m_currentUser) return;
    onNavFeed(); // make sure we're on the feed page
    if (m_feedPage) {
        m_feedPage->m_composerCard->show();
        m_feedPage->m_postInput->setFocus();
    }
}

void MainWindow::setActiveSidebarButton(SidebarButton* active) {
    SidebarButton* btns[] = { m_btnFeed, m_btnNotifications, m_btnSearch, m_btnMessages, m_btnProfile, m_btnTimeSpent };
    for (auto* b : btns) if (b) b->setActive(b == active);
}

void MainWindow::animateFade(QWidget* widget) {
    if (!widget) return;
    auto* effect = new QGraphicsOpacityEffect(widget); widget->setGraphicsEffect(effect);
    auto* anim = new QPropertyAnimation(effect, "opacity", widget);
    anim->setDuration(160); anim->setStartValue(0.0); anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::onNavFeed() { m_pages->setCurrentIndex(0); m_feedPage->refresh();    setActiveSidebarButton(m_btnFeed);          animateFade(m_feedPage); }
void MainWindow::onNavNotifications() { m_pages->setCurrentIndex(1); m_notifPage->refresh();   setActiveSidebarButton(m_btnNotifications);  animateFade(m_notifPage); }
void MainWindow::onNavSearch() { m_pages->setCurrentIndex(2);                           setActiveSidebarButton(m_btnSearch);         animateFade(m_searchPage); }
void MainWindow::onNavMessages() { m_pages->setCurrentIndex(3);                           setActiveSidebarButton(m_btnMessages);       animateFade(m_messagesPage); }
void MainWindow::onNavProfile() { m_pages->setCurrentIndex(4); m_profilePage->refresh(); setActiveSidebarButton(m_btnProfile);        animateFade(m_profilePage); }
void MainWindow::onNavTimeSpent() { m_pages->setCurrentIndex(5);                           setActiveSidebarButton(m_btnTimeSpent);      animateFade(m_timeSpentPage); }

void MainWindow::onLogout() {
    auto r = QMessageBox::question(this, APP_NAME, "Are you sure you want to log out?", QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;
    if (m_currentUser) m_currentUser->logOut();
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    tearDownShell();
    m_currentUser = nullptr;
    m_authPage->resetToLogin();
    m_appShell->hide();
    m_authPage->show(); // FIX 1: explicitly re-show auth page after logout
}

void MainWindow::onSwitchUser() {
    if (m_currentUser) m_currentUser->logOut();
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    tearDownShell();
    m_currentUser = nullptr;
    m_authPage->resetToLogin();
    m_appShell->hide();
    m_authPage->show(); // FIX 1: explicitly re-show auth page
}

void MainWindow::tearDownShell() {
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    if (m_appShell->layout()) {
        QLayoutItem* item;
        while ((item = m_appShell->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) { item->widget()->hide(); item->widget()->deleteLater(); }
            delete item;
        }
        delete m_appShell->layout();
    }
    m_shellLayout = nullptr; m_sidebar = nullptr;
    m_btnFeed = nullptr; m_btnNotifications = nullptr; m_btnSearch = nullptr;
    m_btnMessages = nullptr; m_btnProfile = nullptr; m_btnTimeSpent = nullptr;
    m_btnSwitchUser = nullptr; m_btnLogout = nullptr;
    m_pages = nullptr; m_feedPage = nullptr; m_notifPage = nullptr;
    m_searchPage = nullptr; m_messagesPage = nullptr; m_profilePage = nullptr;
    m_timeSpentPage = nullptr; m_adminPage = nullptr;
}