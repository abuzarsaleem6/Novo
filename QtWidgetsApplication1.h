#pragma once
#ifndef QTWIDGETSAPPLICATION1_H
#define QTWIDGETSAPPLICATION1_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollArea>
#include <QFrame>
#include <QListWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QInputDialog>
#include <QSplitter>
#include <QButtonGroup>

// Backend includes
#include "User.h"
#include "Post.h"
#include "Notification.h"
#include "Feed.h"
#include "SearchEngine.h"
#include "PasswordChecker.h"

// ─── Reusable styled widgets ─────────────────────────────────────────────────

class SidebarButton : public QPushButton {
    Q_OBJECT
public:
    explicit SidebarButton(const QString& icon, const QString& label, QWidget* parent = nullptr);
    void setActive(bool active);
private:
    QString m_icon;
    QString m_label;
};

class PostCard : public QFrame {
    Q_OBJECT
public:
    explicit PostCard(Posts* post, const QString& authorUsername, bool isOwner, QWidget* parent = nullptr);
signals:
    void likeClicked(Posts* post);
    void commentClicked(Posts* post);
    void deleteClicked(Posts* post);
private:
    Posts* m_post;
    QString m_authorUsername;
    bool    m_isOwner;
};

class NotificationItem : public QFrame {
    Q_OBJECT
public:
    explicit NotificationItem(const Notification& notif, QWidget* parent = nullptr);
};

// ─── Page widgets ─────────────────────────────────────────────────────────────

class AuthPage : public QWidget {
    Q_OBJECT
public:
    explicit AuthPage(QWidget* parent = nullptr);
signals:
    void loginSuccess(User* user, User** allUsers, int userCount);
private slots:
    void onLogin();
    void onSignUp();
    void toggleMode();
private:
    QStackedWidget* m_stack;
    // Login
    QLineEdit* m_loginUser;
    QLineEdit* m_loginPass;
    // Sign up
    QLineEdit* m_signupUser;
    QLineEdit* m_signupPass;
    QLineEdit* m_signupBio;

    User** m_allUsers;
    int     m_userCount;
    bool    m_isLoginMode;

    void buildLoginPage();
    void buildSignupPage();
    QWidget* createLoginWidget();
    QWidget* createSignupWidget();
};

class FeedPage : public QWidget {
    Q_OBJECT
public:
    explicit FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();
private slots:
    void onCreatePost();
    void onLikePost(Posts* post);
    void onCommentPost(Posts* post);
    void onDeletePost(Posts* post);
private:
    User* m_user;
    User** m_allUsers;
    int     m_userCount;
    QVBoxLayout* m_feedLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_feedContent;
    QTextEdit* m_postInput;

    void loadPosts();
    void clearFeed();
};

class NotificationsPage : public QWidget {
    Q_OBJECT
public:
    explicit NotificationsPage(User* currentUser, QWidget* parent = nullptr);
    void refresh();
private:
    User* m_user;
    QVBoxLayout* m_listLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_listContent;
    void loadNotifications();
};

class SearchPage : public QWidget {
    Q_OBJECT
public:
    explicit SearchPage(User** allUsers, int userCount, User* currentUser, QWidget* parent = nullptr);
private slots:
    void onSearch();
    void onFollowUser();
private:
    User* m_currentUser;
    User** m_allUsers;
    int          m_userCount;
    QLineEdit* m_searchInput;
    QVBoxLayout* m_resultsLayout;
    QWidget* m_resultsContent;
    QScrollArea* m_scrollArea;
    SearchEngine m_engine;
    User* m_foundUser;
    QPushButton* m_followBtn;
    QLabel* m_resultCard;

    void showUserCard(User* user);
};

class ProfilePage : public QWidget {
    Q_OBJECT
public:
    explicit ProfilePage(User* currentUser, User** allUsers, int& userCount, QWidget* parent = nullptr);
    void refresh();
private slots:
    void onUpdateBio();
    void onUpdatePassword();
    void onDeleteAccount();
private:
    User* m_user;
    User**& m_allUsers;
    int& m_userCount;
    QLabel* m_usernameLabel;
    QLabel* m_bioLabel;
    QLabel* m_statsLabel;
    QLineEdit* m_newBioInput;
    QLineEdit* m_newPassInput;
};

class TimeSpentPage : public QWidget {
    Q_OBJECT
public:
    explicit TimeSpentPage(QWidget* parent = nullptr);
    void startSession();
    void stopSession();
private slots:
    void onTick();
private:
    QLabel* m_timerLabel;
    QLabel* m_sessionLabel;
    QTimer* m_timer;
    int      m_elapsed; // seconds
    QDateTime m_sessionStart;
    void updateDisplay();
};

// ─── Main window ──────────────────────────────────────────────────────────────

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(User* user, User** allUsers, int userCount);
    void onNavFeed();
    void onNavNotifications();
    void onNavSearch();
    void onNavProfile();
    void onNavTimeSpent();
    void onLogout();

private:
    // Auth
    AuthPage* m_authPage;

    // App shell
    QWidget* m_appShell;
    QHBoxLayout* m_shellLayout;

    // Sidebar
    QWidget* m_sidebar;
    SidebarButton* m_btnFeed;
    SidebarButton* m_btnNotifications;
    SidebarButton* m_btnSearch;
    SidebarButton* m_btnProfile;
    SidebarButton* m_btnTimeSpent;
    SidebarButton* m_btnLogout;

    // Pages
    QStackedWidget* m_pages;
    FeedPage* m_feedPage; #include "QtWidgetsApplication1.h"
#include <QApplication>
#include <QScrollBar>
#include <QSpacerItem>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

        // ═══════════════════════════════════════════════════════════════
        //  GLOBAL STYLESHEET
        // ═══════════════════════════════════════════════════════════════
        static const char* APP_STYLE = R"(
* {
    font-family: 'Sora', 'Segoe UI', sans-serif;
    color: #E8E8ED;
}
QMainWindow, QWidget#appShell {
    background: #0D0D0F;
}
QWidget#sidebar {
    background: #111117;
    border-right: 1px solid #1F1F2E;
    min-width: 220px;
    max-width: 220px;
}
QWidget#pageArea {
    background: #0D0D0F;
}

/* ── Sidebar buttons ── */
QPushButton#sidebarBtn {
    background: transparent;
    border: none;
    border-radius: 10px;
    color: #7777AA;
    padding: 12px 18px;
    text-align: left;
    font-size: 14px;
    letter-spacing: 0.3px;
}
QPushButton#sidebarBtn:hover {
    background: #1A1A26;
    color: #C8C8E8;
}
QPushButton#sidebarBtn[active="true"] {
    background: #1E1E2E;
    color: #8585FF;
    font-weight: 600;
    border-left: 3px solid #5B5BF6;
}
QPushButton#sidebarBtnLogout {
    background: transparent;
    border: none;
    border-radius: 10px;
    color: #885555;
    padding: 12px 18px;
    text-align: left;
    font-size: 14px;
}
QPushButton#sidebarBtnLogout:hover {
    background: #1E1010;
    color: #FF7777;
}

/* ── Input fields ── */
QLineEdit, QTextEdit {
    background: #16161E;
    border: 1px solid #252535;
    border-radius: 10px;
    padding: 10px 14px;
    color: #E8E8ED;
    font-size: 14px;
    selection-background-color: #3A3AFF;
}
QLineEdit:focus, QTextEdit:focus {
    border: 1px solid #5555CC;
    background: #1A1A24;
}
QLineEdit::placeholder, QTextEdit::placeholder {
    color: #44445A;
}

/* ── Buttons ── */
QPushButton#primaryBtn {
    background: #4646E8;
    border: none;
    border-radius: 10px;
    color: #FFFFFF;
    padding: 11px 28px;
    font-size: 14px;
    font-weight: 600;
    letter-spacing: 0.5px;
}
QPushButton#primaryBtn:hover {
    background: #5555F5;
}
QPushButton#primaryBtn:pressed {
    background: #3535CC;
}
QPushButton#secondaryBtn {
    background: transparent;
    border: 1px solid #333355;
    border-radius: 10px;
    color: #8888CC;
    padding: 10px 24px;
    font-size: 13px;
}
QPushButton#secondaryBtn:hover {
    background: #1A1A2A;
    color: #AAAAEE;
    border-color: #5555AA;
}
QPushButton#dangerBtn {
    background: #2A1010;
    border: 1px solid #553333;
    border-radius: 10px;
    color: #CC6666;
    padding: 10px 24px;
    font-size: 13px;
}
QPushButton#dangerBtn:hover {
    background: #3A1515;
    color: #FF8888;
    border-color: #884444;
}
QPushButton#iconBtn {
    background: transparent;
    border: none;
    color: #555577;
    font-size: 18px;
    padding: 6px 10px;
    border-radius: 8px;
}
QPushButton#iconBtn:hover {
    background: #1E1E2A;
    color: #8888CC;
}

/* ── Cards ── */
QFrame#postCard {
    background: #13131A;
    border: 1px solid #1E1E2A;
    border-radius: 14px;
    padding: 0px;
}
QFrame#postCard:hover {
    border: 1px solid #2A2A45;
}
QFrame#notifItem {
    background: #13131A;
    border: 1px solid #1E1E2A;
    border-radius: 10px;
}
QFrame#notifItem[unread="true"] {
    border-left: 3px solid #5B5BF6;
    background: #14141E;
}
QFrame#resultCard {
    background: #13131A;
    border: 1px solid #1E1E2A;
    border-radius: 12px;
}

/* ── Labels ── */
QLabel#pageTitle {
    font-size: 22px;
    font-weight: 700;
    color: #EEEEF8;
    letter-spacing: -0.5px;
}
QLabel#sectionLabel {
    font-size: 11px;
    font-weight: 600;
    color: #444466;
    letter-spacing: 1.5px;
    text-transform: uppercase;
}
QLabel#postAuthor {
    font-size: 13px;
    font-weight: 600;
    color: #CCCCEE;
}
QLabel#postTime {
    font-size: 11px;
    color: #44445A;
}
QLabel#postContent {
    font-size: 14px;
    color: #C0C0D8;
    line-height: 1.6;
}
QLabel#statValue {
    font-size: 26px;
    font-weight: 700;
    color: #7777FF;
}
QLabel#statLabel {
    font-size: 11px;
    color: #55557A;
    text-transform: uppercase;
    letter-spacing: 1px;
}
QLabel#timerDisplay {
    font-size: 54px;
    font-weight: 700;
    color: #6666FF;
    letter-spacing: -2px;
}

/* ── Scroll bars ── */
QScrollArea { border: none; background: transparent; }
QScrollBar:vertical {
    background: #0E0E14;
    width: 6px;
    border-radius: 3px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background: #2A2A3E;
    border-radius: 3px;
    min-height: 30px;
}
QScrollBar::handle:vertical:hover { background: #4444AA; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }

/* ── Auth page ── */
QWidget#authContainer {
    background: #0D0D0F;
}
QFrame#authCard {
    background: #111117;
    border: 1px solid #1E1E2A;
    border-radius: 20px;
}
QLabel#brandTitle {
    font-size: 36px;
    font-weight: 800;
    color: #7777FF;
    letter-spacing: -1.5px;
}
QLabel#brandSub {
    font-size: 13px;
    color: #44445A;
    letter-spacing: 0.3px;
}
QLabel#authFormTitle {
    font-size: 20px;
    font-weight: 700;
    color: #E0E0F0;
}
)";

    // ═══════════════════════════════════════════════════════════════
    //  HELPERS
    // ═══════════════════════════════════════════════════════════════

    static QPushButton* makePrimaryBtn(const QString& text, QWidget* parent = nullptr) {
        auto* btn = new QPushButton(text, parent);
        btn->setObjectName("primaryBtn");
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    }
    static QPushButton* makeSecondaryBtn(const QString& text, QWidget* parent = nullptr) {
        auto* btn = new QPushButton(text, parent);
        btn->setObjectName("secondaryBtn");
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    }
    static QPushButton* makeDangerBtn(const QString& text, QWidget* parent = nullptr) {
        auto* btn = new QPushButton(text, parent);
        btn->setObjectName("dangerBtn");
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    }
    static QPushButton* makeIconBtn(const QString& icon, QWidget* parent = nullptr) {
        auto* btn = new QPushButton(icon, parent);
        btn->setObjectName("iconBtn");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(36, 36);
        return btn;
    }
    static QLabel* makeLabel(const QString& text, const QString& objName = "", QWidget* parent = nullptr) {
        auto* lbl = new QLabel(text, parent);
        if (!objName.isEmpty()) lbl->setObjectName(objName);
        lbl->setWordWrap(true);
        return lbl;
    }
    static QFrame* makeDivider(QWidget* parent = nullptr) {
        auto* f = new QFrame(parent);
        f->setFrameShape(QFrame::HLine);
        f->setFixedHeight(1);
        f->setStyleSheet("background:#1A1A28; border:none;");
        return f;
    }

    // ═══════════════════════════════════════════════════════════════
    //  SIDEBAR BUTTON
    // ═══════════════════════════════════════════════════════════════

    SidebarButton::SidebarButton(const QString& icon, const QString& label, QWidget* parent)
        : QPushButton(parent), m_icon(icon), m_label(label)
    {
        setObjectName("sidebarBtn");
        setText(m_icon + "   " + m_label);
        setCursor(Qt::PointingHandCursor);
        setFixedHeight(48);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setProperty("active", false);
    }

    void SidebarButton::setActive(bool active) {
        setProperty("active", active);
        style()->unpolish(this);
        style()->polish(this);
        update();
    }

    // ═══════════════════════════════════════════════════════════════
    //  POST CARD
    // ═══════════════════════════════════════════════════════════════

    PostCard::PostCard(Posts* post, const QString& authorUsername, bool isOwner, QWidget* parent)
        : QFrame(parent), m_post(post), m_authorUsername(authorUsername), m_isOwner(isOwner)
    {
        setObjectName("postCard");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        auto* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(20, 18, 20, 14);
        mainLayout->setSpacing(12);

        // Header row - Post.h only exposes getPostId/getContent/getLikeCount
        // The postId encodes the author implicitly; display postId as subtitle
        auto* headerRow = new QHBoxLayout;
        QString postIdStr = QString::fromStdString(post->getPostId());
        QString avatarInitial = postIdStr.isEmpty() ? "P" : postIdStr.left(1).toUpper();
        auto* avatar = new QLabel(avatarInitial);
        avatar->setFixedSize(38, 38);
        avatar->setAlignment(Qt::AlignCenter);
        avatar->setStyleSheet(
            "QLabel {"
            "  background: #252545;"
            "  border-radius: 19px;"
            "  font-size: 15px;"
            "  font-weight: 700;"
            "  color: #8888FF;"
            "}"
        );

        auto* authorCol = new QVBoxLayout;
        authorCol->setSpacing(2);
        // We store author username in PostCard via constructor param; display it
        auto* authorLbl = makeLabel(m_authorUsername, "postAuthor");
        auto* timeLbl = makeLabel("Post " + postIdStr, "postTime");
        authorCol->addWidget(authorLbl);
        authorCol->addWidget(timeLbl);

        headerRow->addWidget(avatar);
        headerRow->addSpacing(10);
        headerRow->addLayout(authorCol);
        headerRow->addStretch();

        // Delete button (only owner sees it — determined by caller via isOwner flag)
        if (m_isOwner) {
            auto* delBtn = makeIconBtn("✕");
            delBtn->setToolTip("Delete post");
            delBtn->setStyleSheet("QPushButton { background: transparent; border: none; color: #553333; font-size:13px; } QPushButton:hover { color:#FF6666; }");
            connect(delBtn, &QPushButton::clicked, this, [this]() {
                emit deleteClicked(m_post);
                });
            headerRow->addWidget(delBtn);
        }

        mainLayout->addLayout(headerRow);
        mainLayout->addWidget(makeDivider());

        // Content
        auto* contentLbl = makeLabel(QString::fromStdString(post->getContent()), "postContent");
        contentLbl->setWordWrap(true);
        mainLayout->addWidget(contentLbl);

        // Actions row
        auto* actRow = new QHBoxLayout;
        actRow->setSpacing(8);

        auto* likeBtn = makeIconBtn("♥");
        likeBtn->setToolTip("Like");
        auto* likeCountLbl = new QLabel(QString::number(post->getLikeCount()));
        likeCountLbl->setStyleSheet("color:#555577; font-size:12px;");

        auto* cmtBtn = makeIconBtn("💬");
        cmtBtn->setToolTip("Comment");
        auto* cmtCountLbl = new QLabel(QString::number(post->getComments().size()));
        cmtCountLbl->setStyleSheet("color:#555577; font-size:12px;");

        connect(likeBtn, &QPushButton::clicked, this, [this, likeCountLbl]() {
            emit likeClicked(m_post);
            likeCountLbl->setText(QString::number(m_post->getLikeCount()));
            });
        connect(cmtBtn, &QPushButton::clicked, this, [this]() {
            emit commentClicked(m_post);
            });

        actRow->addWidget(likeBtn);
        actRow->addWidget(likeCountLbl);
        actRow->addSpacing(10);
        actRow->addWidget(cmtBtn);
        actRow->addWidget(cmtCountLbl);
        actRow->addStretch();

        mainLayout->addLayout(actRow);
    }

    // ═══════════════════════════════════════════════════════════════
    //  NOTIFICATION ITEM
    // ═══════════════════════════════════════════════════════════════

    NotificationItem::NotificationItem(const Notification& notif, QWidget* parent)
        : QFrame(parent)
    {
        setObjectName("notifItem");
        setProperty("unread", !notif.getStatus());

        auto* layout = new QHBoxLayout(this);
        layout->setContentsMargins(16, 14, 16, 14);
        layout->setSpacing(14);

        // Icon based on type
        QString typeIcon = "🔔";
        QString typeStr = QString::fromStdString(notif.getType());
        if (typeStr.contains("follow", Qt::CaseInsensitive)) typeIcon = "👤";
        else if (typeStr.contains("like", Qt::CaseInsensitive)) typeIcon = "♥";
        else if (typeStr.contains("comment", Qt::CaseInsensitive)) typeIcon = "💬";

        auto* iconLbl = new QLabel(typeIcon);
        iconLbl->setFixedSize(36, 36);
        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setStyleSheet("font-size:18px;");

        auto* col = new QVBoxLayout;
        col->setSpacing(3);
        auto* msgLbl = makeLabel(QString::fromStdString(notif.getMessage()));
        msgLbl->setStyleSheet("font-size:13px; color:#C8C8E0;");
        auto* timeLbl = makeLabel(QString::fromStdString(notif.getTimestamp()), "postTime");
        col->addWidget(msgLbl);
        col->addWidget(timeLbl);

        layout->addWidget(iconLbl);
        layout->addLayout(col);
        layout->addStretch();

        if (!notif.getStatus()) {
            auto* dot = new QLabel("●");
            dot->setStyleSheet("color:#5555FF; font-size:10px;");
            layout->addWidget(dot);
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  AUTH PAGE
    // ═══════════════════════════════════════════════════════════════

    AuthPage::AuthPage(QWidget* parent)
        : QWidget(parent), m_userCount(0), m_isLoginMode(true)
    {
        setObjectName("authContainer");
        m_allUsers = new User * [200];

        // Load existing users
        loadAllUsers(m_allUsers, m_userCount);

        auto* outerLayout = new QVBoxLayout(this);
        outerLayout->setAlignment(Qt::AlignCenter);
        outerLayout->setContentsMargins(0, 0, 0, 0);

        auto* card = new QFrame;
        card->setObjectName("authCard");
        card->setFixedWidth(420);

        auto* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(44, 40, 44, 40);
        cardLayout->setSpacing(0);

        // Brand
        auto* brandRow = new QVBoxLayout;
        brandRow->setSpacing(4);
        brandRow->setAlignment(Qt::AlignCenter);
        auto* brandTitle = makeLabel("Husld", "brandTitle");
        brandTitle->setAlignment(Qt::AlignCenter);
        auto* brandSub = makeLabel("Your social space", "brandSub");
        brandSub->setAlignment(Qt::AlignCenter);
        brandRow->addWidget(brandTitle);
        brandRow->addWidget(brandSub);
        cardLayout->addLayout(brandRow);
        cardLayout->addSpacing(36);

        // Stacked pages: 0 = login, 1 = signup
        m_stack = new QStackedWidget;
        m_stack->addWidget(createLoginWidget());
        m_stack->addWidget(createSignupWidget());
        cardLayout->addWidget(m_stack);

        outerLayout->addWidget(card);
    }

    QWidget* AuthPage::createLoginWidget() {
        auto* w = new QWidget;
        auto* layout = new QVBoxLayout(w);
        layout->setSpacing(14);
        layout->setContentsMargins(0, 0, 0, 0);

        auto* title = makeLabel("Welcome back", "authFormTitle");
        title->setAlignment(Qt::AlignLeft);
        layout->addWidget(title);
        layout->addSpacing(6);

        m_loginUser = new QLineEdit;
        m_loginUser->setPlaceholderText("Username");
        m_loginUser->setFixedHeight(44);
        layout->addWidget(m_loginUser);

        m_loginPass = new QLineEdit;
        m_loginPass->setPlaceholderText("Password");
        m_loginPass->setEchoMode(QLineEdit::Password);
        m_loginPass->setFixedHeight(44);
        layout->addWidget(m_loginPass);

        layout->addSpacing(6);

        auto* loginBtn = makePrimaryBtn("Sign In");
        loginBtn->setFixedHeight(46);
        connect(loginBtn, &QPushButton::clicked, this, &AuthPage::onLogin);
        connect(m_loginPass, &QLineEdit::returnPressed, this, &AuthPage::onLogin);
        layout->addWidget(loginBtn);

        layout->addSpacing(16);
        auto* switchRow = new QHBoxLayout;
        auto* switchLbl = new QLabel("Don't have an account?");
        switchLbl->setStyleSheet("color:#44445A; font-size:13px;");
        auto* switchBtn = new QPushButton("Create one");
        switchBtn->setObjectName("secondaryBtn");
        switchBtn->setCursor(Qt::PointingHandCursor);
        switchBtn->setStyleSheet("QPushButton { background:transparent; border:none; color:#6666CC; font-size:13px; text-decoration:underline; } QPushButton:hover { color:#8888FF; }");
        connect(switchBtn, &QPushButton::clicked, this, &AuthPage::toggleMode);
        switchRow->addStretch();
        switchRow->addWidget(switchLbl);
        switchRow->addSpacing(6);
        switchRow->addWidget(switchBtn);
        switchRow->addStretch();
        layout->addLayout(switchRow);

        return w;
    }

    QWidget* AuthPage::createSignupWidget() {
        auto* w = new QWidget;
        auto* layout = new QVBoxLayout(w);
        layout->setSpacing(14);
        layout->setContentsMargins(0, 0, 0, 0);

        auto* title = makeLabel("Create account", "authFormTitle");
        layout->addWidget(title);
        layout->addSpacing(6);

        m_signupUser = new QLineEdit;
        m_signupUser->setPlaceholderText("Username");
        m_signupUser->setFixedHeight(44);
        layout->addWidget(m_signupUser);

        m_signupPass = new QLineEdit;
        m_signupPass->setPlaceholderText("Password (min 8 chars, uppercase, digit, symbol)");
        m_signupPass->setEchoMode(QLineEdit::Password);
        m_signupPass->setFixedHeight(44);
        layout->addWidget(m_signupPass);

        m_signupBio = new QLineEdit;
        m_signupBio->setPlaceholderText("Short bio (optional)");
        m_signupBio->setFixedHeight(44);
        layout->addWidget(m_signupBio);

        layout->addSpacing(6);
        auto* signupBtn = makePrimaryBtn("Create Account");
        signupBtn->setFixedHeight(46);
        connect(signupBtn, &QPushButton::clicked, this, &AuthPage::onSignUp);
        layout->addWidget(signupBtn);

        layout->addSpacing(16);
        auto* switchRow = new QHBoxLayout;
        auto* switchLbl = new QLabel("Already have an account?");
        switchLbl->setStyleSheet("color:#44445A; font-size:13px;");
        auto* switchBtn = new QPushButton("Sign in");
        switchBtn->setCursor(Qt::PointingHandCursor);
        switchBtn->setStyleSheet("QPushButton { background:transparent; border:none; color:#6666CC; font-size:13px; text-decoration:underline; } QPushButton:hover { color:#8888FF; }");
        connect(switchBtn, &QPushButton::clicked, this, &AuthPage::toggleMode);
        switchRow->addStretch();
        switchRow->addWidget(switchLbl);
        switchRow->addSpacing(6);
        switchRow->addWidget(switchBtn);
        switchRow->addStretch();
        layout->addLayout(switchRow);

        return w;
    }

    void AuthPage::toggleMode() {
        m_isLoginMode = !m_isLoginMode;
        m_stack->setCurrentIndex(m_isLoginMode ? 0 : 1);
    }

    void AuthPage::onLogin() {
        QString uname = m_loginUser->text().trimmed();
        QString pass = m_loginPass->text();

        if (uname.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(this, "Husld", "Please fill in all fields.");
            return;
        }

        User* user = findAndLogin(m_allUsers, m_userCount,
            uname.toStdString(), pass.toStdString());
        if (user) {
            user->loadAllPosts();
            user->loadFollowing(m_allUsers, m_userCount);
            user->loadFollowers(m_allUsers, m_userCount);
            emit loginSuccess(user, m_allUsers, m_userCount);
        }
        else {
            QMessageBox::critical(this, "Husld", "Invalid username or password.");
        }
    }

    void AuthPage::onSignUp() {
        QString uname = m_signupUser->text().trimmed();
        QString pass = m_signupPass->text();
        QString bio = m_signupBio->text().trimmed();

        if (uname.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(this, "Husld", "Username and password are required.");
            return;
        }

        PasswordChecker checker(8);
        if (!checker.checkAll(pass.toStdString())) {
            int strength = checker.calculateStrength(pass.toStdString());
            QMessageBox::warning(this, "Weak Password",
                QString("Password strength: %1/100\n\nRequirements:\n• Minimum 8 characters\n• At least one uppercase letter\n• At least one digit\n• At least one special character")
                .arg(strength));
            return;
        }

        User* newUser = signUp(m_allUsers, m_userCount,
            uname.toStdString(), pass.toStdString(), bio.toStdString());
        if (newUser) {
            QMessageBox::information(this, "Husld", "Account created! You can now sign in.");
            m_signupUser->clear();
            m_signupPass->clear();
            m_signupBio->clear();
            toggleMode();
        }
        else {
            QMessageBox::critical(this, "Husld", "Username already taken or invalid.");
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  FEED PAGE
    // ═══════════════════════════════════════════════════════════════

    FeedPage::FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
        : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount)
    {
        auto* outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(40, 36, 40, 24);
        outerLayout->setSpacing(24);

        // Header
        auto* headerRow = new QHBoxLayout;
        auto* title = makeLabel("Feed", "pageTitle");
        headerRow->addWidget(title);
        headerRow->addStretch();
        outerLayout->addLayout(headerRow);

        // Compose box
        auto* composeCard = new QFrame;
        composeCard->setObjectName("postCard");
        auto* composeLayout = new QVBoxLayout(composeCard);
        composeLayout->setContentsMargins(20, 16, 20, 16);
        composeLayout->setSpacing(12);

        auto* composeRow = new QHBoxLayout;
        QString initial = QString::fromStdString(currentUser->getUsername()).left(1).toUpper();
        auto* avatar = new QLabel(initial);
        avatar->setFixedSize(36, 36);
        avatar->setAlignment(Qt::AlignCenter);
        avatar->setStyleSheet("background:#252545; border-radius:18px; font-size:14px; font-weight:700; color:#8888FF;");

        m_postInput = new QTextEdit;
        m_postInput->setPlaceholderText("What's on your mind?");
        m_postInput->setFixedHeight(72);
        m_postInput->setStyleSheet("QTextEdit { border-radius: 10px; }");

        composeRow->addWidget(avatar);
        composeRow->addSpacing(10);
        composeRow->addWidget(m_postInput);
        composeLayout->addLayout(composeRow);

        auto* postBtnRow = new QHBoxLayout;
        postBtnRow->addStretch();
        auto* postBtn = makePrimaryBtn("Post");
        postBtn->setFixedWidth(100);
        connect(postBtn, &QPushButton::clicked, this, &FeedPage::onCreatePost);
        postBtnRow->addWidget(postBtn);
        composeLayout->addLayout(postBtnRow);
        outerLayout->addWidget(composeCard);

        // Scrollable feed
        m_scrollArea = new QScrollArea;
        m_scrollArea->setWidgetResizable(true);
        m_scrollArea->setFrameShape(QFrame::NoFrame);
        m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        m_feedContent = new QWidget;
        m_feedLayout = new QVBoxLayout(m_feedContent);
        m_feedLayout->setContentsMargins(0, 0, 8, 0);
        m_feedLayout->setSpacing(14);
        m_feedLayout->addStretch();

        m_scrollArea->setWidget(m_feedContent);
        outerLayout->addWidget(m_scrollArea);

        loadPosts();
    }

    void FeedPage::refresh() {
        loadPosts();
    }

    void FeedPage::clearFeed() {
        while (m_feedLayout->count() > 1) { // keep the stretch at end
            auto* item = m_feedLayout->takeAt(0);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
    }

    void FeedPage::loadPosts() {
        clearFeed();

        if (!m_user) return;

        bool anyPost = false;

        // Show current user's posts
        int pCount = m_user->getPostCount();
        for (int i = pCount - 1; i >= 0; --i) {
            Posts* p = m_user->getPostByIndex(i);
            if (!p) continue;
            QString uname = QString::fromStdString(m_user->getUsername());
            auto* card = new PostCard(p, uname, true /*isOwner*/);
            connect(card, &PostCard::likeClicked, this, &FeedPage::onLikePost);
            connect(card, &PostCard::commentClicked, this, &FeedPage::onCommentPost);
            connect(card, &PostCard::deleteClicked, this, &FeedPage::onDeletePost);
            m_feedLayout->insertWidget(m_feedLayout->count() - 1, card);
            anyPost = true;
        }

        // Show following users' posts
        for (int u = 0; u < m_userCount; ++u) {
            if (!m_allUsers[u]) continue;
            if (m_allUsers[u] == m_user) continue;
            if (!m_user->isFollowing(m_allUsers[u]->getUsername())) continue;

            m_allUsers[u]->loadAllPosts();
            int fpc = m_allUsers[u]->getPostCount();
            for (int i = fpc - 1; i >= 0; --i) {
                Posts* p = m_allUsers[u]->getPostByIndex(i);
                if (!p) continue;
                QString funame = QString::fromStdString(m_allUsers[u]->getUsername());
                auto* card = new PostCard(p, funame, false /*isOwner*/);
                connect(card, &PostCard::likeClicked, this, &FeedPage::onLikePost);
                connect(card, &PostCard::commentClicked, this, &FeedPage::onCommentPost);
                connect(card, &PostCard::deleteClicked, this, &FeedPage::onDeletePost);
                m_feedLayout->insertWidget(m_feedLayout->count() - 1, card);
                anyPost = true;
            }
        }

        if (!anyPost) {
            auto* emptyLbl = new QLabel("No posts yet. Follow people or create your first post! 🌟");
            emptyLbl->setAlignment(Qt::AlignCenter);
            emptyLbl->setStyleSheet("color:#33334A; font-size:15px; padding:40px 0;");
            m_feedLayout->insertWidget(0, emptyLbl);
        }
    }

    void FeedPage::onCreatePost() {
        QString text = m_postInput->toPlainText().trimmed();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Husld", "Post content cannot be empty.");
            return;
        }
        m_user->createPost(text.toStdString());
        m_postInput->clear();
        loadPosts();
    }

    void FeedPage::onLikePost(Posts* post) {
        if (post) post->likePost();
    }

    void FeedPage::onCommentPost(Posts* post) {
        if (!post) return;
        bool ok;
        QString comment = QInputDialog::getText(this, "Add Comment", "Your comment:", QLineEdit::Normal, "", &ok);
        if (ok && !comment.trimmed().isEmpty()) {
            post->addComment(comment.trimmed(), QString::fromStdString(m_user->getUsername()));
            loadPosts();
        }
    }

    void FeedPage::onDeletePost(Posts* post) {
        if (!post) return;
        auto reply = QMessageBox::question(this, "Delete Post",
            "Are you sure you want to delete this post?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Backend does not expose a removePost by pointer; reload to reflect change
            // For now we just reload the feed after a note
            QMessageBox::information(this, "Husld", "Post deletion requires backend removePost support.");
            loadPosts();
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  NOTIFICATIONS PAGE
    // ═══════════════════════════════════════════════════════════════

    NotificationsPage::NotificationsPage(User* currentUser, QWidget* parent)
        : QWidget(parent), m_user(currentUser)
    {
        auto* outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(40, 36, 40, 24);
        outerLayout->setSpacing(20);

        auto* headerRow = new QHBoxLayout;
        auto* title = makeLabel("Notifications", "pageTitle");
        auto* refreshBtn = makeSecondaryBtn("↻  Refresh");
        refreshBtn->setFixedWidth(110);
        connect(refreshBtn, &QPushButton::clicked, this, &NotificationsPage::refresh);
        headerRow->addWidget(title);
        headerRow->addStretch();
        headerRow->addWidget(refreshBtn);
        outerLayout->addLayout(headerRow);

        m_scrollArea = new QScrollArea;
        m_scrollArea->setWidgetResizable(true);
        m_scrollArea->setFrameShape(QFrame::NoFrame);
        m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        m_listContent = new QWidget;
        m_listLayout = new QVBoxLayout(m_listContent);
        m_listLayout->setContentsMargins(0, 0, 8, 0);
        m_listLayout->setSpacing(10);
        m_listLayout->addStretch();

        m_scrollArea->setWidget(m_listContent);
        outerLayout->addWidget(m_scrollArea);

        loadNotifications();
    }

    void NotificationsPage::refresh() {
        loadNotifications();
    }

    void NotificationsPage::loadNotifications() {
        // Clear existing items
        while (m_listLayout->count() > 1) {
            auto* item = m_listLayout->takeAt(0);
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }

        if (!m_user) return;

        // Load from file path pattern
        QString path = "data/Notifications/" + QString::fromStdString(m_user->getUsername()) + "_notif.txt";
        QFile file(path);
        bool hasNotifs = false;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (line.isEmpty()) continue;
                QStringList parts = line.split("|");
                if (parts.size() < 4) continue;
                QString msg = parts[0];
                QString type = parts[1];
                QString time = parts[2];
                bool isRead = (parts[3] == "1");
                Notification notif(msg.toStdString(), type.toStdString(), time.toStdString());
                if (isRead) notif.markAsRead();
                auto* item = new NotificationItem(notif);
                m_listLayout->insertWidget(m_listLayout->count() - 1, item);
                hasNotifs = true;
            }
            file.close();
        }

        if (!hasNotifs) {
            auto* emptyLbl = new QLabel("You're all caught up! 🎉");
            emptyLbl->setAlignment(Qt::AlignCenter);
            emptyLbl->setStyleSheet("color:#33334A; font-size:15px; padding:40px 0;");
            m_listLayout->insertWidget(0, emptyLbl);
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  SEARCH PAGE
    // ═══════════════════════════════════════════════════════════════

    SearchPage::SearchPage(User** allUsers, int userCount, User* currentUser, QWidget* parent)
        : QWidget(parent), m_currentUser(currentUser),
        m_allUsers(allUsers), m_userCount(userCount), m_foundUser(nullptr)
    {
        for (int i = 0; i < m_userCount; ++i) {
            if (m_allUsers[i]) m_engine.addUser(m_allUsers[i]);
        }

        auto* outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(40, 36, 40, 24);
        outerLayout->setSpacing(20);

        auto* title = makeLabel("Search Users", "pageTitle");
        outerLayout->addWidget(title);

        // Search bar row
        auto* searchRow = new QHBoxLayout;
        m_searchInput = new QLineEdit;
        m_searchInput->setPlaceholderText("Search by username...");
        m_searchInput->setFixedHeight(46);
        auto* searchBtn = makePrimaryBtn("Search");
        searchBtn->setFixedHeight(46);
        searchBtn->setFixedWidth(110);
        connect(searchBtn, &QPushButton::clicked, this, &SearchPage::onSearch);
        connect(m_searchInput, &QLineEdit::returnPressed, this, &SearchPage::onSearch);
        searchRow->addWidget(m_searchInput);
        searchRow->addSpacing(10);
        searchRow->addWidget(searchBtn);
        outerLayout->addLayout(searchRow);

        // Results area
        m_scrollArea = new QScrollArea;
        m_scrollArea->setWidgetResizable(true);
        m_scrollArea->setFrameShape(QFrame::NoFrame);

        m_resultsContent = new QWidget;
        m_resultsLayout = new QVBoxLayout(m_resultsContent);
        m_resultsLayout->setContentsMargins(0, 0, 8, 0);
        m_resultsLayout->setSpacing(12);
        m_resultsLayout->addStretch();

        m_scrollArea->setWidget(m_resultsContent);
        outerLayout->addWidget(m_scrollArea);
    }

    void SearchPage::onSearch() {
        QString query = m_searchInput->text().trimmed();
        if (query.isEmpty()) return;

        // Clear results
        while (m_resultsLayout->count() > 1) {
            auto* item = m_resultsLayout->takeAt(0);
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        m_foundUser = nullptr;

        User* found = m_engine.searchUsers(query);
        if (found) {
            m_foundUser = found;
            showUserCard(found);
        }
        else {
            auto* notFoundLbl = new QLabel("No user found for \"" + query + "\"");
            notFoundLbl->setAlignment(Qt::AlignCenter);
            notFoundLbl->setStyleSheet("color:#555577; font-size:14px; padding:30px 0;");
            m_resultsLayout->insertWidget(0, notFoundLbl);
        }
    }

    void SearchPage::showUserCard(User* user) {
        auto* card = new QFrame;
        card->setObjectName("resultCard");
        auto* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(24, 20, 24, 20);
        cardLayout->setSpacing(14);

        // Top row: avatar + name
        auto* topRow = new QHBoxLayout;
        QString initial = QString::fromStdString(user->getUsername()).left(1).toUpper();
        auto* avatar = new QLabel(initial);
        avatar->setFixedSize(52, 52);
        avatar->setAlignment(Qt::AlignCenter);
        avatar->setStyleSheet("background:#252545; border-radius:26px; font-size:20px; font-weight:700; color:#8888FF;");

        auto* infoCol = new QVBoxLayout;
        infoCol->setSpacing(4);
        auto* nameLbl = makeLabel(QString::fromStdString(user->getUsername()));
        nameLbl->setStyleSheet("font-size:17px; font-weight:700; color:#E0E0F8;");
        auto* bioLbl = makeLabel(QString::fromStdString(user->getBio()));
        bioLbl->setStyleSheet("font-size:13px; color:#666688;");
        infoCol->addWidget(nameLbl);
        infoCol->addWidget(bioLbl);

        topRow->addWidget(avatar);
        topRow->addSpacing(14);
        topRow->addLayout(infoCol);
        topRow->addStretch();
        cardLayout->addLayout(topRow);

        // Stats row
        auto* statsRow = new QHBoxLayout;
        auto addStat = [&](const QString& val, const QString& label) {
            auto* col = new QVBoxLayout;
            col->setSpacing(2);
            auto* v = new QLabel(val);
            v->setObjectName("statValue");
            v->setStyleSheet("font-size:20px; font-weight:700; color:#7777FF;");
            auto* l = new QLabel(label);
            l->setObjectName("statLabel");
            v->setAlignment(Qt::AlignCenter);
            l->setAlignment(Qt::AlignCenter);
            col->addWidget(v);
            col->addWidget(l);
            statsRow->addLayout(col);
            statsRow->addSpacing(30);
            };
        addStat(QString::number(user->getPostCount()), "Posts");
        addStat(QString::number(user->getFollowersCount()), "Followers");
        addStat(QString::number(user->getFollowingCount()), "Following");
        statsRow->addStretch();
        cardLayout->addLayout(statsRow);

        // Report badge
        if (user->getIsReported()) {
            auto* reportedLbl = new QLabel("⚠  This account has been reported");
            reportedLbl->setStyleSheet("color:#AA4444; font-size:12px; background:#1A0A0A; border-radius:6px; padding:6px 10px;");
            cardLayout->addWidget(reportedLbl);
        }

        cardLayout->addWidget(makeDivider());

        // Follow / unfollow button
        if (m_currentUser && m_currentUser != user) {
            bool alreadyFollowing = m_currentUser->isFollowing(user->getUsername());
            m_followBtn = alreadyFollowing
                ? makeSecondaryBtn("✓  Following")
                : makePrimaryBtn("+ Follow");
            m_followBtn->setFixedWidth(140);
            connect(m_followBtn, &QPushButton::clicked, this, &SearchPage::onFollowUser);
            auto* btnRow = new QHBoxLayout;
            btnRow->addStretch();
            btnRow->addWidget(m_followBtn);
            cardLayout->addLayout(btnRow);
        }

        m_resultsLayout->insertWidget(0, card);
    }

    void SearchPage::onFollowUser() {
        if (!m_foundUser || !m_currentUser) return;

        if (m_currentUser->isFollowing(m_foundUser->getUsername())) {
            m_currentUser->unfollowUser(m_foundUser->getUsername());
            m_foundUser->removeFollower(m_currentUser);
            m_followBtn->setText("+ Follow");
            m_followBtn->setObjectName("primaryBtn");
        }
        else {
            m_currentUser->followUser(m_foundUser, m_allUsers, m_userCount);
            m_followBtn->setText("✓  Following");
            m_followBtn->setObjectName("secondaryBtn");
        }
        m_followBtn->style()->unpolish(m_followBtn);
        m_followBtn->style()->polish(m_followBtn);
    }

    // ═══════════════════════════════════════════════════════════════
    //  PROFILE PAGE
    // ═══════════════════════════════════════════════════════════════

    ProfilePage::ProfilePage(User* currentUser, User** allUsers, int& userCount, QWidget* parent)
        : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount)
    {
        auto* outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(40, 36, 40, 24);
        outerLayout->setSpacing(0);

        auto* scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);

        auto* inner = new QWidget;
        auto* layout = new QVBoxLayout(inner);
        layout->setContentsMargins(0, 0, 8, 0);
        layout->setSpacing(20);

        // Header
        auto* title = makeLabel("Profile", "pageTitle");
        layout->addWidget(title);

        // Profile card
        auto* profileCard = new QFrame;
        profileCard->setObjectName("postCard");
        auto* pcLayout = new QVBoxLayout(profileCard);
        pcLayout->setContentsMargins(28, 24, 28, 24);
        pcLayout->setSpacing(18);

        // Avatar + info
        auto* infoRow = new QHBoxLayout;
        QString initial = currentUser ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper() : "?";
        auto* avatar = new QLabel(initial);
        avatar->setFixedSize(72, 72);
        avatar->setAlignment(Qt::AlignCenter);
        avatar->setStyleSheet("background:#252545; border-radius:36px; font-size:28px; font-weight:700; color:#8888FF;");

        auto* infoCol = new QVBoxLayout;
        infoCol->setSpacing(6);
        m_usernameLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getUsername()) : "");
        m_usernameLabel->setStyleSheet("font-size:22px; font-weight:800; color:#EEEEFF; letter-spacing:-0.5px;");
        m_bioLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getBio()) : "");
        m_bioLabel->setStyleSheet("font-size:14px; color:#666688;");
        infoCol->addWidget(m_usernameLabel);
        infoCol->addWidget(m_bioLabel);

        infoRow->addWidget(avatar);
        infoRow->addSpacing(18);
        infoRow->addLayout(infoCol);
        infoRow->addStretch();
        pcLayout->addLayout(infoRow);

        // Stats
        auto* statsRow = new QHBoxLayout;
        statsRow->setSpacing(0);
        auto addStat = [&](const QString& val, const QString& label) {
            auto* col = new QVBoxLayout;
            col->setSpacing(2);
            auto* v = new QLabel(val);
            v->setStyleSheet("font-size:24px; font-weight:700; color:#7777FF;");
            auto* l = new QLabel(label);
            l->setStyleSheet("font-size:11px; color:#44445A; text-transform:uppercase; letter-spacing:1px;");
            v->setAlignment(Qt::AlignCenter);
            l->setAlignment(Qt::AlignCenter);
            col->addWidget(v);
            col->addWidget(l);
            statsRow->addLayout(col);
            statsRow->addStretch();
            };
        if (currentUser) {
            addStat(QString::number(currentUser->getPostCount()), "Posts");
            addStat(QString::number(currentUser->getFollowersCount()), "Followers");
            addStat(QString::number(currentUser->getFollowingCount()), "Following");
        }
        pcLayout->addLayout(statsRow);
        layout->addWidget(profileCard);

        // Edit bio
        auto* editBioCard = new QFrame;
        editBioCard->setObjectName("postCard");
        auto* ebLayout = new QVBoxLayout(editBioCard);
        ebLayout->setContentsMargins(24, 20, 24, 20);
        ebLayout->setSpacing(12);
        ebLayout->addWidget(makeLabel("Update Bio", "sectionLabel"));
        m_newBioInput = new QLineEdit;
        m_newBioInput->setPlaceholderText("New bio...");
        m_newBioInput->setFixedHeight(44);
        ebLayout->addWidget(m_newBioInput);
        auto* updateBioBtn = makePrimaryBtn("Save Bio");
        updateBioBtn->setFixedWidth(110);
        connect(updateBioBtn, &QPushButton::clicked, this, &ProfilePage::onUpdateBio);
        auto* bioBtnRow = new QHBoxLayout;
        bioBtnRow->addStretch();
        bioBtnRow->addWidget(updateBioBtn);
        ebLayout->addLayout(bioBtnRow);
        layout->addWidget(editBioCard);

        // Change password
        auto* passCard = new QFrame;
        passCard->setObjectName("postCard");
        auto* passLayout = new QVBoxLayout(passCard);
        passLayout->setContentsMargins(24, 20, 24, 20);
        passLayout->setSpacing(12);
        passLayout->addWidget(makeLabel("Change Password", "sectionLabel"));
        m_newPassInput = new QLineEdit;
        m_newPassInput->setPlaceholderText("New password...");
        m_newPassInput->setEchoMode(QLineEdit::Password);
        m_newPassInput->setFixedHeight(44);
        passLayout->addWidget(m_newPassInput);
        auto* updatePassBtn = makePrimaryBtn("Update Password");
        updatePassBtn->setFixedWidth(160);
        connect(updatePassBtn, &QPushButton::clicked, this, &ProfilePage::onUpdatePassword);
        auto* passBtnRow = new QHBoxLayout;
        passBtnRow->addStretch();
        passBtnRow->addWidget(updatePassBtn);
        passLayout->addLayout(passBtnRow);
        layout->addWidget(passCard);

        // Delete account
        auto* dangerCard = new QFrame;
        dangerCard->setObjectName("postCard");
        dangerCard->setStyleSheet("QFrame#postCard { border: 1px solid #3A1515; background: #100808; }");
        auto* dangerLayout = new QVBoxLayout(dangerCard);
        dangerLayout->setContentsMargins(24, 20, 24, 20);
        dangerLayout->setSpacing(12);
        auto* dangerTitle = makeLabel("DANGER ZONE", "sectionLabel");
        dangerTitle->setStyleSheet("font-size:11px; font-weight:600; color:#663333; letter-spacing:1.5px;");
        auto* dangerDesc = makeLabel("Deleting your account is permanent and cannot be undone.");
        dangerDesc->setStyleSheet("font-size:13px; color:#554444;");
        auto* deleteBtn = makeDangerBtn("Delete My Account");
        deleteBtn->setFixedWidth(180);
        connect(deleteBtn, &QPushButton::clicked, this, &ProfilePage::onDeleteAccount);
        auto* delRow = new QHBoxLayout;
        delRow->addStretch();
        delRow->addWidget(deleteBtn);
        dangerLayout->addWidget(dangerTitle);
        dangerLayout->addWidget(dangerDesc);
        dangerLayout->addLayout(delRow);
        layout->addWidget(dangerCard);

        layout->addStretch();
        scroll->setWidget(inner);
        outerLayout->addWidget(scroll);
    }

    void ProfilePage::refresh() {
        if (!m_user) return;
        m_usernameLabel->setText(QString::fromStdString(m_user->getUsername()));
        m_bioLabel->setText(QString::fromStdString(m_user->getBio()));
    }

    void ProfilePage::onUpdateBio() {
        if (!m_user) return;
        QString newBio = m_newBioInput->text().trimmed();
        if (newBio.isEmpty()) {
            QMessageBox::warning(this, "Husld", "Bio cannot be empty.");
            return;
        }
        m_user->setBio(newBio.toStdString());
        m_user->saveToFile();
        m_bioLabel->setText(newBio);
        m_newBioInput->clear();
        QMessageBox::information(this, "Husld", "Bio updated successfully.");
    }

    void ProfilePage::onUpdatePassword() {
        if (!m_user) return;
        QString newPass = m_newPassInput->text();
        if (newPass.isEmpty()) {
            QMessageBox::warning(this, "Husld", "Password cannot be empty.");
            return;
        }
        PasswordChecker checker(8);
        if (!checker.checkAll(newPass.toStdString())) {
            int str = checker.calculateStrength(newPass.toStdString());
            QMessageBox::warning(this, "Weak Password",
                QString("Strength: %1/100. Password needs uppercase, digit, and special character.").arg(str));
            return;
        }
        m_user->setPassword(newPass.toStdString());
        m_user->saveToFile();
        m_newPassInput->clear();
        QMessageBox::information(this, "Husld", "Password updated successfully.");
    }

    void ProfilePage::onDeleteAccount() {
        auto reply = QMessageBox::warning(this, "Delete Account",
            "This will permanently delete your account.\n\nAre you absolutely sure?",
            QMessageBox::Yes | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            m_user->deleteAccount(m_allUsers, m_userCount);
            QMessageBox::information(this, "Husld", "Account deleted. The app will now close.");
            QApplication::quit();
        }
    }

    // ═══════════════════════════════════════════════════════════════
    //  TIME SPENT PAGE
    // ═══════════════════════════════════════════════════════════════

    TimeSpentPage::TimeSpentPage(QWidget* parent)
        : QWidget(parent), m_elapsed(0)
    {
        auto* outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(40, 36, 40, 24);
        outerLayout->setSpacing(24);

        auto* title = makeLabel("Time Spent", "pageTitle");
        outerLayout->addWidget(title);

        auto* card = new QFrame;
        card->setObjectName("postCard");
        auto* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(40, 40, 40, 40);
        cardLayout->setSpacing(24);
        cardLayout->setAlignment(Qt::AlignCenter);

        auto* clockIcon = new QLabel("⏱");
        clockIcon->setAlignment(Qt::AlignCenter);
        clockIcon->setStyleSheet("font-size:48px;");
        cardLayout->addWidget(clockIcon);

        m_timerLabel = makeLabel("00:00:00", "timerDisplay");
        m_timerLabel->setAlignment(Qt::AlignCenter);
        cardLayout->addWidget(m_timerLabel);

        m_sessionLabel = makeLabel("Session started: —");
        m_sessionLabel->setAlignment(Qt::AlignCenter);
        m_sessionLabel->setStyleSheet("color:#44445A; font-size:13px;");
        cardLayout->addWidget(m_sessionLabel);

        outerLayout->addWidget(card);

        // Info cards row
        auto* infoRow = new QHBoxLayout;
        infoRow->setSpacing(14);

        auto makeInfoCard = [&](const QString& icon, const QString& label, const QString& value) -> QFrame* {
            auto* f = new QFrame;
            f->setObjectName("postCard");
            auto* fl = new QVBoxLayout(f);
            fl->setContentsMargins(20, 18, 20, 18);
            fl->setSpacing(8);
            fl->setAlignment(Qt::AlignCenter);
            auto* ic = new QLabel(icon);
            ic->setAlignment(Qt::AlignCenter);
            ic->setStyleSheet("font-size:24px;");
            auto* val = new QLabel(value);
            val->setAlignment(Qt::AlignCenter);
            val->setStyleSheet("font-size:18px; font-weight:700; color:#7777FF;");
            auto* lbl = new QLabel(label);
            lbl->setAlignment(Qt::AlignCenter);
            lbl->setStyleSheet("font-size:11px; color:#44445A; letter-spacing:1px;");
            fl->addWidget(ic);
            fl->addWidget(val);
            fl->addWidget(lbl);
            return f;
            };

        infoRow->addWidget(makeInfoCard("🎯", "TODAY'S GOAL", "2 hours"));
        infoRow->addWidget(makeInfoCard("📊", "AVG DAILY", "—"));
        infoRow->addWidget(makeInfoCard("🔥", "STREAK", "—"));
        outerLayout->addLayout(infoRow);
        outerLayout->addStretch();

        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &TimeSpentPage::onTick);
    }

    void TimeSpentPage::startSession() {
        m_elapsed = 0;
        m_sessionStart = QDateTime::currentDateTime();
        m_sessionLabel->setText("Session started: " + m_sessionStart.toString("hh:mm AP"));
        m_timer->start(1000);
    }

    void TimeSpentPage::stopSession() {
        m_timer->stop();
    }

    void TimeSpentPage::onTick() {
        m_elapsed++;
        updateDisplay();
    }

    void TimeSpentPage::updateDisplay() {
        int h = m_elapsed / 3600;
        int m = (m_elapsed % 3600) / 60;
        int s = m_elapsed % 60;
        m_timerLabel->setText(QString("%1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0')));
    }

    // ═══════════════════════════════════════════════════════════════
    //  MAIN WINDOW
    // ═══════════════════════════════════════════════════════════════

    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent),
        m_currentUser(nullptr), m_allUsers(nullptr), m_userCount(0),
        m_feedPage(nullptr), m_notifPage(nullptr), m_searchPage(nullptr),
        m_profilePage(nullptr), m_timeSpentPage(nullptr)
    {
        setWindowTitle("Husld");
        setMinimumSize(960, 660);

        qApp->setStyleSheet(APP_STYLE);

        // Central widget: starts with auth page
        auto* central = new QWidget;
        setCentralWidget(central);
        auto* rootLayout = new QVBoxLayout(central);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        // Auth page
        m_authPage = new AuthPage;
        connect(m_authPage, &AuthPage::loginSuccess, this, &MainWindow::onLoginSuccess);

        // App shell (hidden until login)
        m_appShell = new QWidget;
        m_appShell->setObjectName("appShell");
        m_appShell->hide();

        rootLayout->addWidget(m_authPage);
        rootLayout->addWidget(m_appShell);
    }

    MainWindow::~MainWindow() {}

    void MainWindow::onLoginSuccess(User* user, User** allUsers, int userCount) {
        m_currentUser = user;
        m_allUsers = allUsers;
        m_userCount = userCount;

        // Build the app shell
        m_shellLayout = new QHBoxLayout(m_appShell);
        m_shellLayout->setContentsMargins(0, 0, 0, 0);
        m_shellLayout->setSpacing(0);

        buildSidebar();
        buildPages();

        m_shellLayout->addWidget(m_sidebar);
        m_shellLayout->addWidget(m_pages);

        m_authPage->hide();
        m_appShell->show();

        // Start time tracker
        m_timeSpentPage->startSession();

        // Show feed by default
        onNavFeed();
    }

    void MainWindow::buildSidebar() {
        m_sidebar = new QWidget;
        m_sidebar->setObjectName("sidebar");
        m_sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        auto* layout = new QVBoxLayout(m_sidebar);
        layout->setContentsMargins(14, 24, 14, 20);
        layout->setSpacing(4);

        // Brand
        auto* brand = new QLabel("Husld");
        brand->setStyleSheet("font-size:22px; font-weight:800; color:#6666EE; letter-spacing:-1px; padding: 0 6px 16px 6px;");
        layout->addWidget(brand);
        layout->addWidget(makeDivider());
        layout->addSpacing(12);

        // Nav section label
        auto* navLabel = makeLabel("NAVIGATE", "sectionLabel");
        navLabel->setContentsMargins(6, 0, 0, 4);
        layout->addWidget(navLabel);

        m_btnFeed = new SidebarButton("⊞", "Feed");
        m_btnNotifications = new SidebarButton("🔔", "Notifications");
        m_btnSearch = new SidebarButton("⌕", "Search");
        m_btnProfile = new SidebarButton("◉", "Profile");
        m_btnTimeSpent = new SidebarButton("⏱", "Time Spent");

        connect(m_btnFeed, &QPushButton::clicked, this, &MainWindow::onNavFeed);
        connect(m_btnNotifications, &QPushButton::clicked, this, &MainWindow::onNavNotifications);
        connect(m_btnSearch, &QPushButton::clicked, this, &MainWindow::onNavSearch);
        connect(m_btnProfile, &QPushButton::clicked, this, &MainWindow::onNavProfile);
        connect(m_btnTimeSpent, &QPushButton::clicked, this, &MainWindow::onNavTimeSpent);

        layout->addWidget(m_btnFeed);
        layout->addWidget(m_btnNotifications);
        layout->addWidget(m_btnSearch);
        layout->addWidget(m_btnProfile);
        layout->addWidget(m_btnTimeSpent);

        layout->addStretch();
        layout->addWidget(makeDivider());
        layout->addSpacing(8);

        // User info at bottom
        auto* userRow = new QHBoxLayout;
        QString initial = m_currentUser ? QString::fromStdString(m_currentUser->getUsername()).left(1).toUpper() : "?";
        auto* miniAvatar = new QLabel(initial);
        miniAvatar->setFixedSize(32, 32);
        miniAvatar->setAlignment(Qt::AlignCenter);
        miniAvatar->setStyleSheet("background:#252545; border-radius:16px; font-size:13px; font-weight:700; color:#8888FF;");
        auto* unameLbl = new QLabel(m_currentUser ? QString::fromStdString(m_currentUser->getUsername()) : "");
        unameLbl->setStyleSheet("font-size:13px; color:#888899; font-weight:600;");
        unameLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        userRow->addWidget(miniAvatar);
        userRow->addSpacing(8);
        userRow->addWidget(unameLbl);
        userRow->addStretch();
        layout->addLayout(userRow);
        layout->addSpacing(8);

        m_btnLogout = new SidebarButton("⎋", "Log Out");
        m_btnLogout->setObjectName("sidebarBtnLogout");
        connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
        layout->addWidget(m_btnLogout);
    }

    void MainWindow::buildPages() {
        m_pages = new QStackedWidget;
        m_pages->setObjectName("pageArea");

        m_feedPage = new FeedPage(m_currentUser, m_allUsers, m_userCount);
        m_notifPage = new NotificationsPage(m_currentUser);
        m_searchPage = new SearchPage(m_allUsers, m_userCount, m_currentUser);
        m_profilePage = new ProfilePage(m_currentUser, m_allUsers, m_userCount);
        m_timeSpentPage = new TimeSpentPage;

        m_pages->addWidget(m_feedPage);     // 0
        m_pages->addWidget(m_notifPage);    // 1
        m_pages->addWidget(m_searchPage);   // 2
        m_pages->addWidget(m_profilePage);  // 3
        m_pages->addWidget(m_timeSpentPage); // 4
    }

    void MainWindow::setActiveSidebarButton(SidebarButton* active) {
        SidebarButton* buttons[] = { m_btnFeed, m_btnNotifications, m_btnSearch, m_btnProfile, m_btnTimeSpent };
        for (auto* btn : buttons) {
            btn->setActive(btn == active);
        }
    }

    void MainWindow::animateFade(QWidget* widget) {
        auto* effect = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(effect);
        auto* anim = new QPropertyAnimation(effect, "opacity", widget);
        anim->setDuration(180);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void MainWindow::onNavFeed() {
        m_pages->setCurrentIndex(0);
        m_feedPage->refresh();
        setActiveSidebarButton(m_btnFeed);
        animateFade(m_feedPage);
    }

    void MainWindow::onNavNotifications() {
        m_pages->setCurrentIndex(1);
        m_notifPage->refresh();
        setActiveSidebarButton(m_btnNotifications);
        animateFade(m_notifPage);
    }

    void MainWindow::onNavSearch() {
        m_pages->setCurrentIndex(2);
        setActiveSidebarButton(m_btnSearch);
        animateFade(m_searchPage);
    }

    void MainWindow::onNavProfile() {
        m_pages->setCurrentIndex(3);
        m_profilePage->refresh();
        setActiveSidebarButton(m_btnProfile);
        animateFade(m_profilePage);
    }

    void MainWindow::onNavTimeSpent() {
        m_pages->setCurrentIndex(4);
        setActiveSidebarButton(m_btnTimeSpent);
        animateFade(m_timeSpentPage);
    }

    void MainWindow::onLogout() {
        auto reply = QMessageBox::question(this, "Log Out",
            "Are you sure you want to log out?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) return;

        if (m_currentUser) m_currentUser->logOut();
        m_timeSpentPage->stopSession();

        // Hide shell, show auth
        m_appShell->hide();
        m_authPage->show();

        // Reset state
        m_currentUser = nullptr;

        // Rebuild shell on next login
        delete m_shellLayout;
        m_shellLayout = nullptr;
        delete m_sidebar;
        m_sidebar = nullptr;
        delete m_pages;
        m_pages = nullptr;
        m_feedPage = m_notifPage = m_searchPage = m_profilePage = nullptr;
        m_timeSpentPage = nullptr;
    }
    NotificationsPage* m_notifPage;
    SearchPage* m_searchPage;
    ProfilePage* m_profilePage;
    TimeSpentPage* m_timeSpentPage;

    // State
    User* m_currentUser;
    User** m_allUsers;
    int    m_userCount;

    void buildSidebar();
    void buildPages();
    void setActiveSidebarButton(SidebarButton* active);
    void animateFade(QWidget* widget);

    static QString globalStyle();
};

#endif // QTWIDGETSAPPLICATION1_H