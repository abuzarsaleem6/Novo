// ============================================================
//  QtWidgetsApplication1.cpp  —  Novo Social Platform GUI
//  Qt 5/6  •  Pure programmatic layouts  •  No .ui files
//  All sizing via layout managers — zero absolute positioning
// ============================================================

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
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QDir>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QScreen>

// ──────────────────────────────────────────────────────────────────────────────
//  APPLICATION NAME
// ──────────────────────────────────────────────────────────────────────────────
static const QString APP_NAME = "Novo";

// ──────────────────────────────────────────────────────────────────────────────
//  GLOBAL STYLESHEET
//  All sizing is done in C++ layout code, NOT in QSS, to avoid conflicts.
//  QSS here handles ONLY colour, font, border-radius, and hover effects.
// ──────────────────────────────────────────────────────────────────────────────
static const QString APP_STYLE = R"(
/* ── Reset ── */
* {
    font-family: 'Segoe UI', 'Helvetica Neue', Arial, sans-serif;
    color: #E2E2EC;
    box-sizing: border-box;
}

/* ── Shell ── */
QMainWindow, QWidget#appShell, QWidget#pageArea, QWidget#authBg {
    background: #0C0C10;
}

/* ── Sidebar ── */
QWidget#sidebar {
    background: #0F0F16;
    border-right: 1px solid #1C1C28;
}

/* ── Sidebar buttons ── */
QPushButton#sidebarBtn {
    background: transparent;
    border: none;
    border-radius: 9px;
    color: #50507A;
    text-align: left;
    font-size: 13px;
}
QPushButton#sidebarBtn:hover {
    background: #17171F;
    color: #A0A0D0;
}
QPushButton#sidebarBtn[active="true"] {
    background: #1B1B2A;
    color: #7C7CFF;
    font-weight: 600;
    border-left: 3px solid #5050F0;
}
QPushButton#sidebarBtnDanger {
    background: transparent;
    border: none;
    border-radius: 9px;
    color: #664040;
    text-align: left;
    font-size: 13px;
}
QPushButton#sidebarBtnDanger:hover {
    background: #1C0D0D;
    color: #FF7070;
}

/* ── Inputs ── */
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

/* ── Primary button ── */
QPushButton#primaryBtn {
    background: #4040E0;
    border: none;
    border-radius: 9px;
    color: #FFFFFF;
    font-size: 13px;
    font-weight: 600;
}
QPushButton#primaryBtn:hover   { background: #5252F0; }
QPushButton#primaryBtn:pressed { background: #3030BB; }

/* ── Plus (compose) button ── */
QPushButton#plusCircleBtn {
    background: #4040E0;
    border: none;
    border-radius: 22px;
    color: #FFFFFF;
    font-size: 22px;
    font-weight: bold;
}
QPushButton#plusCircleBtn:hover { background: #5252F0; }

/* ── Secondary button ── */
QPushButton#secondaryBtn {
    background: transparent;
    border: 1px solid #2A2A48;
    border-radius: 9px;
    color: #6060AA;
    font-size: 13px;
}
QPushButton#secondaryBtn:hover {
    background: #18182A;
    color: #AAAAEE;
    border-color: #4444AA;
}

/* ── Danger button ── */
QPushButton#dangerBtn {
    background: #200808;
    border: 1px solid #4A2020;
    border-radius: 9px;
    color: #BB5555;
    font-size: 13px;
}
QPushButton#dangerBtn:hover {
    background: #300C0C;
    color: #EE7777;
    border-color: #774444;
}

/* ── Icon button ── */
QPushButton#iconBtn {
    background: transparent;
    border: none;
    color: #44446A;
    font-size: 16px;
    border-radius: 7px;
}
QPushButton#iconBtn:hover { background: #18182A; color: #7777CC; }

/* ── Save / Saved icon buttons ── */
QPushButton#saveIconBtn {
    background: transparent;
    border: none;
    color: #44664A;
    font-size: 15px;
    border-radius: 7px;
}
QPushButton#saveIconBtn:hover { background: #0F1F10; color: #44CC55; }
QPushButton#savedIconBtn {
    background: #0F1F10;
    border: none;
    color: #44CC55;
    font-size: 15px;
    border-radius: 7px;
}

/* ── Report icon button ── */
QPushButton#reportIconBtn {
    background: transparent;
    border: none;
    color: #664444;
    font-size: 13px;
    border-radius: 7px;
}
QPushButton#reportIconBtn:hover { background: #1C0808; color: #CC4444; }

/* ── Cards ── */
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
QFrame#authCard {
    background: #0F0F18;
    border: 1px solid #1C1C2E;
    border-radius: 18px;
}

/* ── Labels ── */
QLabel#pageTitle   { font-size: 20px; font-weight: 700; color: #E8E8F8; }
QLabel#sectionLbl  { font-size: 10px; font-weight: 600; color: #383858; letter-spacing: 1.4px; }
QLabel#postAuthor  { font-size: 13px; font-weight: 600; color: #CCCCEE; }
QLabel#postTime    { font-size: 11px; color: #3A3A58; }
QLabel#postContent { font-size: 13px; color: #B8B8D0; }
QLabel#timerDisp   { font-size: 52px; font-weight: 700; color: #5555EE; letter-spacing: -2px; }
QLabel#brandTitle  { font-size: 34px; font-weight: 800; color: #6060FF; letter-spacing: -1px; }
QLabel#brandSub    { font-size: 12px; color: #36365A; }
QLabel#authTitle   { font-size: 19px; font-weight: 700; color: #DEDEFF; }

/* ── Scroll bars ── */
QScrollArea        { border: none; background: transparent; }
QScrollBar:vertical {
    background: transparent;
    width: 5px;
    border-radius: 3px;
    margin: 2px 0;
}
QScrollBar::handle:vertical {
    background: #222234;
    border-radius: 3px;
    min-height: 28px;
}
QScrollBar::handle:vertical:hover { background: #3A3AFF; }
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical { height: 0px; }
QScrollBar::add-page:vertical,
QScrollBar::sub-page:vertical  { background: transparent; }
/* ── Message Boxes & Dialogs ── */
QMessageBox {
    background: #0C0C10;
}
QMessageBox QLabel {
    color: #E8E8F8;
    font-size: 14px;
}
QMessageBox QPushButton {
    background: transparent;
    border: 1px solid #2A2A48;
    border-radius: 9px;
    color: #6060AA;
    font-size: 13px;
    padding: 6px 20px;
    min-width: 60px;
}
QMessageBox QPushButton:hover {
    background: #18182A;
    color: #AAAAEE;
    border-color: #4444AA;
}
)";

// ──────────────────────────────────────────────────────────────────────────────
//  STATIC HELPER FUNCTIONS
//  These are the only factory helpers used throughout the file.
// ──────────────────────────────────────────────────────────────────────────────
namespace {

    QPushButton* makePrimary(const QString& t, QWidget* p = nullptr) {
        auto* b = new QPushButton(t, p);
        b->setObjectName("primaryBtn");
        b->setCursor(Qt::PointingHandCursor);
        return b;
    }

    QPushButton* makeSecondary(const QString& t, QWidget* p = nullptr) {
        auto* b = new QPushButton(t, p);
        b->setObjectName("secondaryBtn");
        b->setCursor(Qt::PointingHandCursor);
        return b;
    }

    QPushButton* makeDanger(const QString& t, QWidget* p = nullptr) {
        auto* b = new QPushButton(t, p);
        b->setObjectName("dangerBtn");
        b->setCursor(Qt::PointingHandCursor);
        return b;
    }

    // Icon button with explicit fixed size so it never shrinks / overlaps
    QPushButton* makeIcon(const QString& icon, QWidget* p = nullptr,
        int w = 32, int h = 32) {
        auto* b = new QPushButton(icon, p);
        b->setObjectName("iconBtn");
        b->setCursor(Qt::PointingHandCursor);
        b->setFixedSize(w, h);
        return b;
    }

    QLabel* makeLabel(const QString& text, const QString& obj = {}, QWidget* p = nullptr) {
        auto* l = new QLabel(text, p);
        if (!obj.isEmpty()) l->setObjectName(obj);
        l->setWordWrap(true);
        return l;
    }

    // Thin horizontal rule
    QFrame* divider(QWidget* p = nullptr) {
        auto* f = new QFrame(p);
        f->setFrameShape(QFrame::HLine);
        f->setFixedHeight(1);
        f->setStyleSheet("background: #1A1A28; border: none;");
        return f;
    }

    // Circular avatar with initial
    QLabel* avatar(const QString& initial, int size, QWidget* p = nullptr) {
        auto* l = new QLabel(initial.isEmpty() ? "?" : initial, p);
        l->setFixedSize(size, size);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(QString(
            "background: #202040;"
            "border-radius: %1px;"
            "font-size: %2px;"
            "font-weight: 700;"
            "color: #7070FF;")
            .arg(size / 2).arg(qMax(10, size / 3)));
        return l;
    }

} // anonymous namespace

// ══════════════════════════════════════════════════════════════════════════════
//  SidebarButton
// ══════════════════════════════════════════════════════════════════════════════
SidebarButton::SidebarButton(const QString& icon, const QString& label, QWidget* parent)
    : QPushButton(parent)
{
    setObjectName("sidebarBtn");
    // Fixed height; width driven by parent layout (Expanding policy)
    setFixedHeight(44);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setText(icon + "   " + label);
    setCursor(Qt::PointingHandCursor);
    setProperty("active", false);
    // Padding applied via QSS; left-indent handled by property
    setStyleSheet("text-align: left; padding-left: 14px;");
}

void SidebarButton::setActive(bool active) {
    setProperty("active", active);
    if (style()) { style()->unpolish(this); style()->polish(this); }
    update();
}

// ══════════════════════════════════════════════════════════════════════════════
//  PostCard
//  Layout:
//    [avatar | author/time | (delete btn if owner)]
//    ── divider ──
//    [content label]
//    [like btn | like cnt | comment btn | comment cnt | (save btn) | (report btn)]
// ══════════════════════════════════════════════════════════════════════════════
PostCard::PostCard(Posts* post, const QString& authorUsername,
    bool isOwner, bool isSaved,
    const QString& viewerUsername, QWidget* parent)
    : QFrame(parent)
    , m_post(post)
    , m_authorUsername(authorUsername)
    , m_isOwner(isOwner)
    , m_isSaved(isSaved)
    , m_viewerUsername(viewerUsername)
{
    setObjectName("postCard");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Root vertical layout — only vertical scrolling needed
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(12);

    // ── Header row ──────────────────────────────────────────────────────────
    auto* headerRow = new QHBoxLayout;
    headerRow->setSpacing(12);
    headerRow->setContentsMargins(0, 0, 0, 0);

    QString ini = m_authorUsername.isEmpty() ? "?" : m_authorUsername.left(1).toUpper();
    headerRow->addWidget(avatar(ini, 40), 0, Qt::AlignVCenter);

    // Author + handle stacked vertically
    auto* metaCol = new QVBoxLayout;
    metaCol->setSpacing(2);
    metaCol->setContentsMargins(0, 0, 0, 0);
    auto* authorLbl = makeLabel(m_authorUsername, "postAuthor");
    authorLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto* handleLbl = makeLabel("@" + m_authorUsername, "postTime");
    handleLbl->setStyleSheet("color: #888888; font-size: 11px;");
    metaCol->addWidget(authorLbl);
    metaCol->addWidget(handleLbl);

    headerRow->addLayout(metaCol, 1);

    // Timestamp on the right
    QString timeStr = QString::fromStdString(post->getTimeOfCreation());
    if (timeStr.isEmpty())
        timeStr = "Post " + QString::fromStdString(post->getPostId());
    auto* timeRightLbl = makeLabel(timeStr, "postTime");
    timeRightLbl->setStyleSheet("color: #888888; font-size: 11px;");
    timeRightLbl->setAlignment(Qt::AlignRight);
    timeRightLbl->setFixedWidth(80);
    headerRow->addWidget(timeRightLbl, 0, Qt::AlignVCenter);

    if (m_isOwner) {
        auto* editBtn = new QPushButton("✎");
        editBtn->setFixedSize(28, 28);
        editBtn->setObjectName("iconBtn");
        editBtn->setCursor(Qt::PointingHandCursor);
        editBtn->setStyleSheet(
            "QPushButton{background:transparent;border:none;color:#888888;font-size:16px;border-radius:6px;}"
            "QPushButton:hover{background:#1E1E1E;color:#AAAAFF;}"); // Blueish hover
        connect(editBtn, &QPushButton::clicked, this,
            [this]() { emit editClicked(m_post); });
        headerRow->addWidget(editBtn, 0, Qt::AlignVCenter);
        auto* delBtn = new QPushButton("⋮");
        delBtn->setFixedSize(28, 28);
        delBtn->setObjectName("iconBtn");
        delBtn->setCursor(Qt::PointingHandCursor);
        delBtn->setStyleSheet(
            "QPushButton{background:transparent;border:none;color:#888888;font-size:16px;border-radius:6px;}"
            "QPushButton:hover{background:#1E1E1E;color:#FFFFFF;}");
        connect(delBtn, &QPushButton::clicked, this,
            [this]() { emit deleteClicked(m_post); });
        headerRow->addWidget(delBtn, 0, Qt::AlignVCenter);
    }

    root->addLayout(headerRow);

    // ── Content ─────────────────────────────────────────────────────────────
    auto* contentLbl = makeLabel(QString::fromStdString(post->getContent()), "postContent");
    contentLbl->setWordWrap(true);
    contentLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    contentLbl->setStyleSheet("color: #FFFFFF; font-size: 15px; line-height: 1.6; padding: 8px 0;");
    root->addWidget(contentLbl);

    // ── Action row (SINGLE ROW, HORIZONTAL) ──────────────────────────────────
    auto* actRow = new QHBoxLayout;
    actRow->setSpacing(24);
    actRow->setContentsMargins(0, 8, 0, 0);

    // Like (icon + count centered vertically)
    auto* likeCol = new QVBoxLayout;
    likeCol->setSpacing(2);
    likeCol->setAlignment(Qt::AlignCenter);
    auto* likeBtn = makeIcon("♥", nullptr, 24, 24);
    auto* likeCnt = new QLabel("[♥ " + QString::number(post->getLikeCount()) + "]");
    likeCnt->setStyleSheet("color: #F5A623; font-size: 12px; font-weight: 600;");
    likeCnt->setAlignment(Qt::AlignCenter);
    auto* likeLbl = new QLabel("Like");
    likeLbl->setStyleSheet("color: #F5A623; font-size: 11px;");
    likeLbl->setAlignment(Qt::AlignCenter);
    likeCol->addWidget(likeBtn, 0, Qt::AlignCenter);
    likeCol->addWidget(likeCnt, 0, Qt::AlignCenter);
    likeCol->addWidget(likeLbl, 0, Qt::AlignCenter);
    connect(likeBtn, &QPushButton::clicked, this, [this, likeCnt]() {
        m_post->likePost(m_viewerUsername.toStdString());
        likeCnt->setText("[♥ " + QString::number(m_post->getLikeCount()) + "]");
        emit likeClicked(m_post);
        });
    actRow->addLayout(likeCol);

    // Comment (icon + count centered vertically)
    auto* cmtCol = new QVBoxLayout;
    cmtCol->setSpacing(2);
    cmtCol->setAlignment(Qt::AlignCenter);
    auto* cmtBtn = makeIcon("◯", nullptr, 24, 24);
    auto* cmtCnt = new QLabel("[◯ " + QString::number(post->getCommentsCount()) + "]");
    cmtCnt->setStyleSheet("color: #F5A623; font-size: 12px; font-weight: 600;");
    cmtCnt->setAlignment(Qt::AlignCenter);
    auto* cmtLbl = new QLabel("Comment");
    cmtLbl->setStyleSheet("color: #F5A623; font-size: 11px;");
    cmtLbl->setAlignment(Qt::AlignCenter);
    cmtCol->addWidget(cmtBtn, 0, Qt::AlignCenter);
    cmtCol->addWidget(cmtCnt, 0, Qt::AlignCenter);
    cmtCol->addWidget(cmtLbl, 0, Qt::AlignCenter);
    connect(cmtBtn, &QPushButton::clicked, this,
        [this]() { emit commentClicked(m_post); });
    actRow->addLayout(cmtCol);

    if (!m_isOwner) {
        // Save (icon + label)
        auto* saveCol = new QVBoxLayout;
        saveCol->setSpacing(2);
        saveCol->setAlignment(Qt::AlignCenter);
        auto* saveBtn = new QPushButton(m_isSaved ? "★" : "☆");
        saveBtn->setObjectName(m_isSaved ? "savedIconBtn" : "saveIconBtn");
        saveBtn->setCursor(Qt::PointingHandCursor);
        saveBtn->setFixedSize(24, 24);
        auto* saveLbl = new QLabel("Save");
        saveLbl->setStyleSheet("color: #F5A623; font-size: 11px;");
        saveLbl->setAlignment(Qt::AlignCenter);
        saveCol->addWidget(saveBtn, 0, Qt::AlignCenter);
        saveCol->addWidget(saveLbl, 0, Qt::AlignCenter);
        connect(saveBtn, &QPushButton::clicked, this, [this, saveBtn, saveLbl]() {
            if (m_isSaved) {
                emit unsaveClicked(m_post);
            }
            else {
                emit saveClicked(m_post, m_authorUsername);
            }
            m_isSaved = !m_isSaved;
            saveBtn->setText(m_isSaved ? "★" : "☆");
            saveBtn->setObjectName(m_isSaved ? "savedIconBtn" : "saveIconBtn");
            if (saveBtn->style()) {
                saveBtn->style()->unpolish(saveBtn);
                saveBtn->style()->polish(saveBtn);
            }
            });
        actRow->addLayout(saveCol);

        // Report (icon + label)
        auto* reportCol = new QVBoxLayout;
        reportCol->setSpacing(2);
        reportCol->setAlignment(Qt::AlignCenter);
        auto* reportBtn = new QPushButton("⚑");
        reportBtn->setObjectName("reportIconBtn");
        reportBtn->setCursor(Qt::PointingHandCursor);
        reportBtn->setFixedSize(24, 24);
        auto* reportLbl = new QLabel("Report");
        reportLbl->setStyleSheet("color: #F5A623; font-size: 11px;");
        reportLbl->setAlignment(Qt::AlignCenter);
        reportCol->addWidget(reportBtn, 0, Qt::AlignCenter);
        reportCol->addWidget(reportLbl, 0, Qt::AlignCenter);
        connect(reportBtn, &QPushButton::clicked, this,
            [this]() { emit reportClicked(m_post, m_authorUsername); });
        actRow->addLayout(reportCol);
    }

    // Stretch to push Share to the right
    actRow->addStretch(1);

    // Share button (right-aligned, golden color)
    auto* shareCol = new QVBoxLayout;
    shareCol->setSpacing(2);
    shareCol->setAlignment(Qt::AlignCenter);
    auto* shareBtn = makeIcon("↗", nullptr, 24, 24);
    auto* shareLbl = new QLabel("Share");
    shareLbl->setStyleSheet("color: #F5A623; font-size: 11px;");
    shareLbl->setAlignment(Qt::AlignCenter);
    shareCol->addWidget(shareBtn, 0, Qt::AlignCenter);
    shareCol->addWidget(shareLbl, 0, Qt::AlignCenter);
    actRow->addLayout(shareCol);

    root->addLayout(actRow);
}

// ══════════════════════════════════════════════════════════════════════════════
//  NotificationItem
// ══════════════════════════════════════════════════════════════════════════════
NotificationItem::NotificationItem(const Notification& notif, QWidget* parent)
    : QFrame(parent)
{
    setObjectName("notifItem");
    setProperty("unread", !notif.getStatus());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto* row = new QHBoxLayout(this);
    row->setContentsMargins(16, 12, 16, 12);
    row->setSpacing(12);

    QString typeIcon = "●";
    QString t = QString::fromStdString(notif.getType()).toLower();
    if (t.contains("follow"))  typeIcon = "👤";
    else if (t.contains("like"))    typeIcon = "♥";
    else if (t.contains("comment")) typeIcon = "✦";

    auto* ic = new QLabel(typeIcon);
    ic->setFixedSize(32, 32);
    ic->setAlignment(Qt::AlignCenter);
    ic->setStyleSheet("font-size: 16px;");

    auto* col = new QVBoxLayout;
    col->setSpacing(3);
    auto* msg = makeLabel(QString::fromStdString(notif.getMessage()));
    msg->setStyleSheet("font-size: 13px; color: #C0C0DC;");
    auto* ts = makeLabel(QString::fromStdString(notif.getTimestamp()), "postTime");
    col->addWidget(msg);
    col->addWidget(ts);

    row->addWidget(ic, 0, Qt::AlignVCenter);
    row->addLayout(col, 1);

    if (!notif.getStatus()) {
        auto* dot = new QLabel("●");
        dot->setStyleSheet("color: #5050F0; font-size: 9px;");
        dot->setFixedWidth(12);
        row->addWidget(dot, 0, Qt::AlignVCenter);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  AuthPage
//  Centred card, fixed width 400px, height driven by its contents.
// ══════════════════════════════════════════════════════════════════════════════
AuthPage::AuthPage(QWidget* parent)
    : QWidget(parent), m_userCount(0)
{
    setObjectName("authBg");
    m_allUsers = new User * [500]();

    // Outer layout centres the card both horizontally and vertically
    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);
    outer->setContentsMargins(20, 20, 20, 20);

    auto* card = new QFrame;
    card->setObjectName("authCard");
    card->setFixedWidth(400);
    // Do NOT fix the height — let contents size it
    card->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(40, 36, 40, 36);
    cl->setSpacing(14);

    auto* brandTitle = makeLabel(APP_NAME, "brandTitle");
    brandTitle->setAlignment(Qt::AlignCenter);
    auto* brandSub = makeLabel("Connect. Share. Discover.", "brandSub");
    brandSub->setAlignment(Qt::AlignCenter);
    cl->addWidget(brandTitle);
    cl->addWidget(brandSub);
    cl->addSpacing(26);

    m_stack = new QStackedWidget;
    m_stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_stack->addWidget(createLandingWidget());   // 0
    m_stack->addWidget(createLoginWidget());     // 1
    m_stack->addWidget(createSignupWidget());    // 2
    m_stack->addWidget(createAdminLoginWidget());// 3
    cl->addWidget(m_stack);

    outer->addWidget(card, 0, Qt::AlignCenter);

    // CRITICAL FIX #1: actually load users from disk on startup
    loadAllUsers(m_allUsers, m_userCount);
}

AuthPage::~AuthPage() {
    delete[] m_allUsers;
}

void AuthPage::resetToLogin() {
    m_loginUser->clear();  m_loginPass->clear();
    m_signupUser->clear(); m_signupPass->clear(); m_signupBio->clear();
    m_stack->setCurrentIndex(0);
    m_userCount = 0;
    loadAllUsers(m_allUsers, m_userCount);
}

QWidget* AuthPage::createLandingWidget() {
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout(w);
    vl->setSpacing(12);
    vl->setContentsMargins(0, 0, 0, 0);

    auto* btnLogin = makePrimary("Login as User");    btnLogin->setFixedHeight(44);
    auto* btnSignup = makeSecondary("Create Account"); btnSignup->setFixedHeight(44);
    auto* btnAdmin = makeDanger("Admin Console");     btnAdmin->setFixedHeight(44);

    connect(btnLogin, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(1); });
    connect(btnSignup, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(2); });
    connect(btnAdmin, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(3); });

    vl->addWidget(btnLogin);
    vl->addWidget(btnSignup);
    vl->addWidget(divider());
    vl->addWidget(btnAdmin);
    return w;
}

QWidget* AuthPage::createLoginWidget() {
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout(w);
    vl->setSpacing(12);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(makeLabel("User Login", "authTitle"));

    m_loginUser = new QLineEdit; m_loginUser->setPlaceholderText("Username"); m_loginUser->setFixedHeight(42);
    m_loginPass = new QLineEdit; m_loginPass->setPlaceholderText("Password"); m_loginPass->setFixedHeight(42);
    m_loginPass->setEchoMode(QLineEdit::Password);
    vl->addWidget(m_loginUser);
    vl->addWidget(m_loginPass);

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
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout(w);
    vl->setSpacing(12);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(makeLabel("Create Account", "authTitle"));

    m_signupUser = new QLineEdit; m_signupUser->setPlaceholderText("Username (6–16 chars)"); m_signupUser->setFixedHeight(42);
    m_signupPass = new QLineEdit; m_signupPass->setPlaceholderText("Password (8+ chars, upper, digit, symbol)"); m_signupPass->setFixedHeight(42);
    m_signupPass->setEchoMode(QLineEdit::Password);
    m_signupBio = new QLineEdit; m_signupBio->setPlaceholderText("Short bio (optional)"); m_signupBio->setFixedHeight(42);
    vl->addWidget(m_signupUser);
    vl->addWidget(m_signupPass);
    vl->addWidget(m_signupBio);

    auto* signupBtn = makePrimary("Register"); signupBtn->setFixedHeight(44);
    connect(signupBtn, &QPushButton::clicked, this, &AuthPage::onSignUp);
    vl->addWidget(signupBtn);

    auto* backBtn = makeSecondary("← Back");
    connect(backBtn, &QPushButton::clicked, this, [this]() { m_stack->setCurrentIndex(0); });
    vl->addWidget(backBtn);
    return w;
}

QWidget* AuthPage::createAdminLoginWidget() {
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout(w);
    vl->setSpacing(12);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(makeLabel("Admin Console", "authTitle"));

    auto* adminUser = new QLineEdit;
    adminUser->setPlaceholderText("Admin Username");
    adminUser->setFixedHeight(42);

    auto* adminPass = new QLineEdit;
    adminPass->setPlaceholderText("Admin Password");
    adminPass->setFixedHeight(42);
    adminPass->setEchoMode(QLineEdit::Password);

    vl->addWidget(adminUser);
    vl->addWidget(adminPass);

    auto* loginBtn = makeDanger("Enter System");
    loginBtn->setFixedHeight(44);

    connect(loginBtn, &QPushButton::clicked, this, [this, adminUser, adminPass]() {
        // Read credentials from file
        QFile file("data/Admin/admin_credentials.txt");
        bool authenticated = false;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString line = in.readLine();

            if (!line.isEmpty()) {
                QStringList parts = line.split("|");
                if (parts.size() >= 2) {
                    QString fileUsername = parts[0];
                    QString filePassword = parts[1];

                    if (adminUser->text() == fileUsername &&
                        adminPass->text() == filePassword) {
                        authenticated = true;
                    }
                }
            }
            file.close();
        }

        if (authenticated) {
            emit loginAdminSuccess();
        }
        else {
            QMessageBox::critical(this, "Novo", "Invalid Admin Credentials.");
            adminPass->clear();
        }
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
    if (uname.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, APP_NAME, "Please fill in all fields.");
        return;
    }
    User* user = findAndLogin(m_allUsers, m_userCount, uname.toStdString(), pass.toStdString());
    if (user) {

        user->loadFollowing(m_allUsers, m_userCount);
        user->loadFollowers(m_allUsers, m_userCount);
        user->loadSavedPosts(m_allUsers, m_userCount);
        m_loginUser->clear();
        m_loginPass->clear();
        emit loginSuccess(user, m_allUsers, m_userCount);
    }
    else {
        QMessageBox::critical(this, APP_NAME, "Invalid username or password.");
    }
}

void AuthPage::onSignUp() {
    QString uname = m_signupUser->text().trimmed();
    QString pass = m_signupPass->text();
    QString bio = m_signupBio->text().trimmed();
    if (uname.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, APP_NAME, "Username and password are required.");
        return;
    }
    PasswordChecker checker(8);
    if (!checker.checkAll(pass.toStdString())) {
        int strength = checker.calculateStrength(pass.toStdString());
        QMessageBox::warning(this, "Weak Password",
            QString("Password strength: %1 / 100\nNeeds: 8+ chars, uppercase, digit, special char")
            .arg(strength));
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

// ══════════════════════════════════════════════════════════════════════════════
//  SCROLLABLE PAGE HELPER
//  Wraps any content widget in a QScrollArea with the standard settings.
//  Returns the QScrollArea (add to parent layout) and the inner content widget.
// ══════════════════════════════════════════════════════════════════════════════
namespace {
    QScrollArea* makeScrollArea(QWidget*& outContent, QVBoxLayout*& outLayout,
        QWidget* parent = nullptr)
    {
        auto* sa = new QScrollArea(parent);
        sa->setWidgetResizable(true);
        sa->setFrameShape(QFrame::NoFrame);
        sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sa->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        outContent = new QWidget;
        outContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        outLayout = new QVBoxLayout(outContent);
        outLayout->setContentsMargins(0, 0, 6, 20);
        outLayout->setSpacing(12);
        outLayout->addStretch(1); // push cards to top

        sa->setWidget(outContent);
        return sa;
    }
} // anonymous namespace

// ══════════════════════════════════════════════════════════════════════════════
//  FeedPage
//  Layout:
//    outer VBox:
//      ├─ header row (title | stretch)
//      ├─ composer card (hidden by default)
//      └─ scroll area  (Expanding — fills remaining space)
//
//  Feed cards are inserted BEFORE the trailing stretch so they stack top→down.
// ══════════════════════════════════════════════════════════════════════════════
FeedPage::FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount)
{
    auto* outer = new QVBoxLayout(this);
    // Page padding: L=28, T=24, R=28, B=16
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    // ── Header ──────────────────────────────────────────────────────────────
    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Feed", "pageTitle"));
    hRow->addStretch(1);
    outer->addLayout(hRow);

    // ── Composer card ───────────────────────────────────────────────────────
    m_composerCard = new QFrame;
    m_composerCard->setObjectName("composerCard");
    m_composerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_composerCard->hide();
    {
        auto* ccl = new QVBoxLayout(m_composerCard);
        ccl->setContentsMargins(18, 14, 18, 14);
        ccl->setSpacing(10);

        auto* hdr = new QHBoxLayout;
        hdr->setContentsMargins(0, 0, 0, 0);
        hdr->setSpacing(10);
        QString ini = currentUser ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper() : "?";
        hdr->addWidget(avatar(ini, 32), 0, Qt::AlignVCenter);
        hdr->addWidget(makeLabel("What's on your mind?", "postAuthor"), 1);

        auto* closeBtn = makeIcon("✕", nullptr, 28, 28);
        closeBtn->setStyleSheet(
            "QPushButton{background:transparent;border:none;color:#553333;font-size:11px;border-radius:5px;}"
            "QPushButton:hover{color:#EE6666;}");
        connect(closeBtn, &QPushButton::clicked, this,
            [this]() { m_postInput->clear(); m_composerCard->hide(); });
        hdr->addWidget(closeBtn, 0, Qt::AlignVCenter);
        ccl->addLayout(hdr);

        m_postInput = new QTextEdit;
        m_postInput->setPlaceholderText("Share something...");
        m_postInput->setFixedHeight(88);
        m_postInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        ccl->addWidget(m_postInput);

        auto* btnRow = new QHBoxLayout;
        btnRow->addStretch(1);
        auto* postBtn = makePrimary("Post");
        postBtn->setFixedWidth(88);
        postBtn->setFixedHeight(36);
        connect(postBtn, &QPushButton::clicked, this, &FeedPage::onSubmitPost);
        btnRow->addWidget(postBtn);
        ccl->addLayout(btnRow);
    }
    outer->addWidget(m_composerCard);

    // ── Scrollable feed ─────────────────────────────────────────────────────
    m_scrollArea = makeScrollArea(m_feedContent, m_feedLayout);
    outer->addWidget(m_scrollArea, 1); // stretch=1 so it fills all remaining height

    loadPosts();
}

void FeedPage::refresh() { loadPosts(); }

void FeedPage::clearFeed() {
    // Remove everything except the trailing stretch (last item)
    while (m_feedLayout->count() > 1) {
        QLayoutItem* item = m_feedLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void FeedPage::loadPosts() {
    clearFeed();
    if (!m_user) return;

    m_user->loadFollowing(m_allUsers, m_userCount);

    bool any = false;
    for (int u = 0; u < m_userCount; ++u) {
        if (!m_allUsers[u] || m_allUsers[u] == m_user) continue;
        if (!m_user->isFollowing(m_allUsers[u]->getUsername())) continue;
        // Always call loadAllPosts: loadFromFile() sets postCount from disk
        // but leaves posts[] as nullptr — getPostByIndex returns null for
        // every index unless the array has actually been populated.
        m_allUsers[u]->loadAllPosts();



        for (int i = m_allUsers[u]->getPostCount() - 1; i >= 0; --i) {
            Posts* p = m_allUsers[u]->getPostByIndex(i);
            if (!p || !p->isValid()) continue;

            bool saved = false;
            for (int s = 0; s < m_user->getSavedPostCount(); ++s) {
                Posts* sp = m_user->getSavedPostByIndex(s);
                if (sp && sp->getPostId() == p->getPostId()) { saved = true; break; }
            }

            auto* card = new PostCard(p,
                QString::fromStdString(m_allUsers[u]->getUsername()),
                false, saved,
                QString::fromStdString(m_user->getUsername()));

            connect(card, &PostCard::likeClicked, this, &FeedPage::onLikePost);
            connect(card, &PostCard::commentClicked, this, &FeedPage::onCommentPost);
            connect(card, &PostCard::deleteClicked, this, &FeedPage::onDeletePost);
            connect(card, &PostCard::saveClicked, this, &FeedPage::onSavePost);
            connect(card, &PostCard::unsaveClicked, this, &FeedPage::onUnsavePost);
            connect(card, &PostCard::reportClicked, this, &FeedPage::onReportPost);

            // Insert before the trailing stretch
            m_feedLayout->insertWidget(m_feedLayout->count() - 1, card);
            any = true;
        }
    }

    if (!any) {
        auto* empty = new QLabel("Follow someone to see their posts here!");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #28284A; font-size: 14px; padding: 40px 0;");
        m_feedLayout->insertWidget(0, empty);
    }
}

void FeedPage::onSubmitPost() {
    if (!m_user) return;
    QString text = m_postInput->toPlainText().trimmed();
    if (text.isEmpty()) return;
    m_user->createPost(text.toStdString());
    m_postInput->clear();
    m_composerCard->hide();
    loadPosts();
}

void FeedPage::onLikePost(Posts* /*post*/) {}

void FeedPage::onCommentPost(Posts* post) {
    if (!post) return;
    bool ok;
    QString txt = QInputDialog::getText(this, "Add Comment",
        "Your comment:", QLineEdit::Normal, "", &ok);
    if (ok && !txt.trimmed().isEmpty()) {
        post->addComment(txt.trimmed(), QString::fromStdString(m_user->getUsername()));
        refresh();
    }
}

void FeedPage::onDeletePost(Posts* post) {
    if (!post) return;
    // FIX #5: guard against deleting another user's post
    if (post->getCreatorUsername() != m_user->getUsername()) {
        QMessageBox::warning(this, APP_NAME, "You can only delete your own posts.");
        return;
    }
    auto r = QMessageBox::question(this, "Delete Post",
        "Permanently delete this post?", QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        m_user->deletePost(post->getPostId());
        loadPosts();
    }
}

void FeedPage::onSavePost(Posts* post, const QString& ownerUsername) {
    if (!post || !m_user) return;
    User* owner = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] && m_allUsers[i]->getUsername() == ownerUsername.toStdString()) {
            owner = m_allUsers[i]; break;
        }
    }
    if (owner) m_user->savePost(post->getPostId(), owner);
}

void FeedPage::onUnsavePost(Posts* post) {
    if (!post || !m_user) return;
    m_user->unsavePost(post->getPostId());
}

void FeedPage::onReportPost(Posts* post, const QString& ownerUsername) {
    if (!post || !m_user) return;
    User* owner = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] && m_allUsers[i]->getUsername() == ownerUsername.toStdString()) {
            owner = m_allUsers[i]; break;
        }
    }
    if (!owner) return;
    m_user->reportPost(post->getPostId(), owner);
    QMessageBox::information(this, APP_NAME, "Post has been reported.");
}

// ══════════════════════════════════════════════════════════════════════════════
//  MyPostsPage
// ══════════════════════════════════════════════════════════════════════════════
MyPostsPage::MyPostsPage(User* currentUser, QWidget* parent)
    : QWidget(parent), m_user(currentUser)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 10, 0, 0);
    outer->setSpacing(10);

    m_scrollArea = makeScrollArea(m_postsContent, m_postsLayout);
    outer->addWidget(m_scrollArea, 1);
    loadPosts();
}

void MyPostsPage::refresh() { loadPosts(); }

void MyPostsPage::clearPosts() {
    while (m_postsLayout->count() > 1) {
        QLayoutItem* item = m_postsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void MyPostsPage::loadPosts() {
    clearPosts();
    if (!m_user) return;

    m_user->loadAllPosts();
    if (m_user->getPostCount() == 0) {
        auto* empty = new QLabel("You haven't posted anything yet.");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #28284A; font-size: 13px; padding: 30px 0;");
        m_postsLayout->insertWidget(0, empty);
        return;
    }
    for (int i = m_user->getPostCount() - 1; i >= 0; --i) {
        Posts* p = m_user->getPostByIndex(i);
        if (!p) continue;
        auto* card = new PostCard(p,
            QString::fromStdString(m_user->getUsername()),
            true, false,
            QString::fromStdString(m_user->getUsername()));
        connect(card, &PostCard::deleteClicked, this, &MyPostsPage::onDeletePost);
        connect(card, &PostCard::editClicked, this, &MyPostsPage::onEditPost);
        connect(card, &PostCard::commentClicked, this, &MyPostsPage::onCommentPost);
        m_postsLayout->insertWidget(m_postsLayout->count() - 1, card);
    }
}

void MyPostsPage::onDeletePost(Posts* post) {
    if (!post || !m_user) return;
    auto r = QMessageBox::question(nullptr, "Delete Post",
        "Delete this post permanently?", QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        m_user->deletePost(post->getPostId());
        loadPosts();
    }
}

void MyPostsPage::onEditPost(Posts* post) {
    if (!post || !m_user) return;
    bool ok;
    QString newContent = QInputDialog::getMultiLineText(nullptr, "Edit Post",
        "New content:", QString::fromStdString(post->getContent()), &ok);
    if (ok && !newContent.trimmed().isEmpty()) {
        if (newContent.contains('|')) {
            QMessageBox::warning(nullptr, APP_NAME, "Content cannot contain '|'.");
            return;
        }
        post->setContent(newContent.trimmed().toStdString());
        post->savePostToFile();
        loadPosts();
    }
}

void MyPostsPage::onCommentPost(Posts* post) {
    if (!post || !m_user) return;
    bool ok;
    QString txt = QInputDialog::getText(this, "Add Comment",
        "Your comment:", QLineEdit::Normal, "", &ok);
    if (ok && !txt.trimmed().isEmpty()) {
        post->addComment(txt.trimmed(), QString::fromStdString(m_user->getUsername()));
        refresh();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  SavedPostsPage
// ══════════════════════════════════════════════════════════════════════════════
SavedPostsPage::SavedPostsPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 10, 0, 0);
    outer->setSpacing(10);

    m_scrollArea = makeScrollArea(m_savedContent, m_savedLayout);
    outer->addWidget(m_scrollArea, 1);
    loadSaved();
}

void SavedPostsPage::refresh() {
    if (m_user) m_user->loadSavedPosts(m_allUsers, m_userCount);
    loadSaved();
}

void SavedPostsPage::clearSaved() {
    while (m_savedLayout->count() > 1) {
        QLayoutItem* item = m_savedLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void SavedPostsPage::loadSaved() {
    clearSaved();
    if (!m_user) return;
    if (m_user->getSavedPostCount() == 0) {
        auto* empty = new QLabel("No saved posts yet. Save posts from your feed using ☆");
        empty->setAlignment(Qt::AlignCenter);
        empty->setWordWrap(true);
        empty->setStyleSheet("color: #28284A; font-size: 13px; padding: 30px 20px;");
        m_savedLayout->insertWidget(0, empty);
        return;
    }
    for (int i = 0; i < m_user->getSavedPostCount(); ++i) {
        Posts* p = m_user->getSavedPostByIndex(i);
        if (!p) continue;
        auto* card = new PostCard(p,
            QString::fromStdString(p->getCreatorUsername()),
            false, true,
            QString::fromStdString(m_user->getUsername()));
        connect(card, &PostCard::unsaveClicked, this, &SavedPostsPage::onUnsavePost);
        m_savedLayout->insertWidget(m_savedLayout->count() - 1, card);
    }
}

void SavedPostsPage::onUnsavePost(Posts* post) {
    if (!post || !m_user) return;
    m_user->unsavePost(post->getPostId());
    loadSaved();
}

// ══════════════════════════════════════════════════════════════════════════════
//  NotificationsPage
// ══════════════════════════════════════════════════════════════════════════════
// Fix #4: Add "Mark All as Read" button to NotificationsPage
NotificationsPage::NotificationsPage(User* currentUser, QWidget* parent)
    : QWidget(parent), m_user(currentUser)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Notifications", "pageTitle"), 1);

    // FIX #4: Add "Mark All as Read" button
    auto* markReadBtn = makeSecondary("✓ Mark All Read");
    markReadBtn->setFixedWidth(130);
    markReadBtn->setFixedHeight(34);
    connect(markReadBtn, &QPushButton::clicked, this, &NotificationsPage::onMarkAllRead);

    auto* refBtn = makeSecondary("↻ Refresh");
    refBtn->setFixedWidth(100);
    refBtn->setFixedHeight(34);
    connect(refBtn, &QPushButton::clicked, this, &NotificationsPage::refresh);

    hRow->addWidget(markReadBtn);
    hRow->addWidget(refBtn);
    outer->addLayout(hRow);

    m_scrollArea = makeScrollArea(m_listContent, m_listLayout);
    outer->addWidget(m_scrollArea, 1);
    loadNotifications();
}
// Add this method implementation for NotificationsPage
void NotificationsPage::onMarkAllRead() {
    if (!m_user) return;

    QString path = "data/Notifications/" +
        QString::fromStdString(m_user->getUsername()) + "_notif.txt";
    QFile file(path);

    QList<Notification> notifs;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split("|");
            if (parts.size() < 4) continue;

            QString p0 = parts[0], p1 = parts[1];
            bool wasCorrupted = p0.contains(' ');
            QString msgStr = wasCorrupted ? p0 : p1;
            QString typeStr = wasCorrupted ? p1 : p0;
            Notification n(msgStr.toStdString(), typeStr.toStdString(), parts[2].toStdString());
            n.markAsRead();
            notifs.append(n);
        }
        file.close();
    }

    // Write all notifications back as read
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream stream(&file);
        for (auto& n : notifs) {
            stream << QString::fromStdString(n.getType()) << "|"
                << QString::fromStdString(n.getMessage()) << "|"
                << QString::fromStdString(n.getTimestamp()) << "|1\n";
        }
        file.close();
    }

    QMessageBox::information(this, "Novo", "All notifications marked as read.");
    refresh();
}

void NotificationsPage::refresh() { loadNotifications(); }

void NotificationsPage::loadNotifications() {
    while (m_listLayout->count() > 1) {
        QLayoutItem* item = m_listLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    if (!m_user) return;

    QString path = "data/Notifications/" +
        QString::fromStdString(m_user->getUsername()) + "_notif.txt";
    QFile file(path);
    bool any = false;
    QList<Notification> notifs;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split("|");
            if (parts.size() < 4) continue;
            // File format written by Post.cpp / User.cpp:
            //   parts[0]=type | parts[1]=message | parts[2]=timestamp | parts[3]=isRead
            // Notification constructor is Notification(msg, type, time).
            // Detect corrupted files (old write-back stored message|type order):
            // if parts[0] looks like a full sentence (contains a space) it was
            // written in the wrong order — swap to recover gracefully.
            QString p0 = parts[0], p1 = parts[1];
            bool wasCorrupted = p0.contains(' ');  // type is always a single word
            QString msgStr = wasCorrupted ? p0 : p1;
            QString typeStr = wasCorrupted ? p1 : p0;
            Notification n(msgStr.toStdString(), typeStr.toStdString(), parts[2].toStdString());
            if (parts[3] == "1") n.markAsRead();
            notifs.append(n);
            auto* item = new NotificationItem(n);
            m_listLayout->insertWidget(m_listLayout->count() - 1, item);
            any = true;
        }
        file.close();
    }

    // Mark all as read and write back in the SAME format writers use: type|message|timestamp|isRead
    if (any && !notifs.isEmpty()) {
        QFile out(path);
        if (out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream stream(&out);
            for (auto& n : notifs) {
                n.markAsRead();
                stream << QString::fromStdString(n.getType()) << "|"
                    << QString::fromStdString(n.getMessage()) << "|"
                    << QString::fromStdString(n.getTimestamp()) << "|1\n";
            }
            out.close();
        }
    }

    if (!any) {
        auto* empty = new QLabel("No notifications yet!");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #28284A; font-size: 14px; padding: 40px 0;");
        m_listLayout->insertWidget(0, empty);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  SearchPage
// ══════════════════════════════════════════════════════════════════════════════
SearchPage::SearchPage(User** allUsers, int userCount, User* currentUser, QWidget* parent)
    : QWidget(parent), m_currentUser(currentUser),
    m_allUsers(allUsers), m_userCount(userCount),
    m_foundUser(nullptr), m_followBtn(nullptr), m_reportBtn(nullptr)
{
    for (int i = 0; i < m_userCount; ++i)
        if (m_allUsers[i]) m_engine.addUser(m_allUsers[i]);

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);
    outer->addWidget(makeLabel("Search Users", "pageTitle"));

    auto* searchRow = new QHBoxLayout;
    searchRow->setContentsMargins(0, 0, 0, 0);
    searchRow->setSpacing(10);
    m_searchInput = new QLineEdit;
    m_searchInput->setPlaceholderText("Search by username...");
    m_searchInput->setFixedHeight(44);
    auto* searchBtn = makePrimary("Search");
    searchBtn->setFixedHeight(44);
    searchBtn->setFixedWidth(100);
    connect(searchBtn, &QPushButton::clicked, this, &SearchPage::onSearch);
    connect(m_searchInput, &QLineEdit::returnPressed, this, &SearchPage::onSearch);
    searchRow->addWidget(m_searchInput, 1);
    searchRow->addWidget(searchBtn);
    outer->addLayout(searchRow);

    m_scrollArea = makeScrollArea(m_resultsContent, m_resultsLayout);
    outer->addWidget(m_scrollArea, 1);
}

void SearchPage::onSearch() {
    QString query = m_searchInput->text().trimmed();
    if (query.isEmpty()) return;

    // FIX #16: repopulate engine so users created mid-session are searchable
    m_engine = SearchEngine();
    for (int i = 0; i < m_userCount; ++i)
        if (m_allUsers[i]) m_engine.addUser(m_allUsers[i]);

    while (m_resultsLayout->count() > 1) {
        QLayoutItem* item = m_resultsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    m_foundUser = nullptr; m_followBtn = nullptr; m_reportBtn = nullptr;

    User* found = m_engine.searchUsers(query);
    if (found) {
        m_foundUser = found;
        showUserCard(found);
    }
    else {
        auto* nf = new QLabel("No user found for \"" + query + "\"");
        nf->setAlignment(Qt::AlignCenter);
        nf->setStyleSheet("color: #34345A; font-size: 13px; padding: 30px 0;");
        m_resultsLayout->insertWidget(0, nf);
    }
}

void SearchPage::showUserCard(User* user) {
    auto* card = new QFrame;
    card->setObjectName("resultCard");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(22, 18, 22, 18);
    cl->setSpacing(14);

    auto* topRow = new QHBoxLayout;
    topRow->setContentsMargins(0, 0, 0, 0);
    topRow->setSpacing(14);
    QString ini = QString::fromStdString(user->getUsername()).left(1).toUpper();
    topRow->addWidget(avatar(ini, 50), 0, Qt::AlignVCenter);
    auto* infoCol = new QVBoxLayout;
    infoCol->setSpacing(4);
    auto* nameLbl = makeLabel(QString::fromStdString(user->getUsername()));
    nameLbl->setStyleSheet("font-size: 16px; font-weight: 700; color: #DCDCF8;");
    auto* bioLbl = makeLabel(QString::fromStdString(user->getBio()));
    bioLbl->setStyleSheet("font-size: 12px; color: #56567A;");
    infoCol->addWidget(nameLbl);
    infoCol->addWidget(bioLbl);
    topRow->addLayout(infoCol, 1);
    cl->addLayout(topRow);

    // Stats
    auto* statsRow = new QHBoxLayout;
    statsRow->setContentsMargins(0, 0, 0, 0);
    statsRow->setSpacing(0);
    auto addStat = [&](const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout; c->setSpacing(2);
        auto* v = new QLabel(val);
        v->setStyleSheet("font-size: 18px; font-weight: 700; color: #6060FF;");
        v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size: 10px; color: #36365A; letter-spacing: 1px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l);
        statsRow->addLayout(c);
        statsRow->addSpacing(24);
        };
    addStat(QString::number(user->getPostCount()), "POSTS");
    addStat(QString::number(user->getFollowersCount()), "FOLLOWERS");
    addStat(QString::number(user->getFollowingCount()), "FOLLOWING");
    statsRow->addStretch(1);
    cl->addLayout(statsRow);

    if (user->getIsReported()) {
        auto* warn = makeLabel("⚠  This account has been reported");
        warn->setStyleSheet("color:#AA4040; font-size:12px; background:#180606; border-radius:6px; padding:6px 10px;");
        cl->addWidget(warn);
    }
    cl->addWidget(divider());

    if (m_currentUser && m_currentUser != user) {
        auto* btnRow = new QHBoxLayout;
        btnRow->setContentsMargins(0, 0, 0, 0);
        btnRow->addStretch(1);
        bool following = m_currentUser->isFollowing(user->getUsername());
        m_followBtn = following ? makeSecondary("✓  Following") : makePrimary("+ Follow");
        m_followBtn->setFixedWidth(130);
        m_followBtn->setFixedHeight(36);
        connect(m_followBtn, &QPushButton::clicked, this, &SearchPage::onFollowUser);
        btnRow->addWidget(m_followBtn);
        btnRow->addSpacing(10);
        m_reportBtn = makeDanger("⚑ Report");
        m_reportBtn->setFixedWidth(100);
        m_reportBtn->setFixedHeight(36);
        connect(m_reportBtn, &QPushButton::clicked, this, &SearchPage::onReportUser);
        btnRow->addWidget(m_reportBtn);
        cl->addLayout(btnRow);
    }
    m_resultsLayout->insertWidget(0, card);
}

void SearchPage::onFollowUser() {
    if (!m_foundUser || !m_currentUser || !m_followBtn) return;
    if (m_currentUser->isFollowing(m_foundUser->getUsername())) {
        m_currentUser->unfollowUser(m_foundUser->getUsername());
        m_currentUser->saveToFile();
        m_foundUser->saveToFile();
        m_followBtn->setText("+ Follow");
        m_followBtn->setObjectName("primaryBtn");
    }
    else {
        m_currentUser->followUser(m_foundUser, m_allUsers, m_userCount);
        m_currentUser->saveToFile();
        m_foundUser->saveToFile();
        m_followBtn->setText("✓  Following");
        m_followBtn->setObjectName("secondaryBtn");
    }
    if (m_followBtn->style()) {
        m_followBtn->style()->unpolish(m_followBtn);
        m_followBtn->style()->polish(m_followBtn);
    }
    m_followBtn->update();
}

void SearchPage::onReportUser() {
    if (!m_foundUser || !m_currentUser) return;
    if (m_foundUser == m_currentUser) {
        QMessageBox::warning(this, APP_NAME, "You cannot report yourself."); return;
    }
    auto r = QMessageBox::question(this, "Report User",
        "Report @" + QString::fromStdString(m_foundUser->getUsername()) + "?",
        QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        m_foundUser->reportUser();
        QMessageBox::information(this, APP_NAME, "User has been reported.");
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  MESSAGE FILE HELPERS
//  File format: data/Messages/<userA>_<userB>.txt
//  where userA < userB alphabetically (canonical pair name).
//  Each line:  sender|timestamp|message text
//
//  Index file: data/Messages/<username>_index.txt
//  One peer username per line — written whenever a new conversation starts.
// ══════════════════════════════════════════════════════════════════════════════
namespace {

    // Canonical file path for a conversation between two users
    QString msgFilePath(const QString& a, const QString& b) {
        QDir().mkpath("data/Messages");
        QString lo = (a < b) ? a : b;
        QString hi = (a < b) ? b : a;
        return "data/Messages/" + lo + "_" + hi + ".txt";
    }

    // Add peer to user's index file (no duplicates)
    void ensureInIndex(const QString& username, const QString& peer) {
        QDir().mkpath("data/Messages");
        QString path = "data/Messages/" + username + "_index.txt";
        // Read existing
        QStringList existing;
        QFile rf(path);
        if (rf.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&rf);
            while (!in.atEnd()) {
                QString l = in.readLine().trimmed();
                if (!l.isEmpty()) existing << l;
            }
            rf.close();
        }
        if (existing.contains(peer)) return;
        // Append
        QFile wf(path);
        if (wf.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&wf);
            out << peer << "\n";
            wf.close();
        }
    }

    // Remove peer from user's index file
    void removeFromIndex(const QString& username, const QString& peer) {
        QString path = "data/Messages/" + username + "_index.txt";
        QFile f(path);
        QStringList remaining;
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&f);
            while (!in.atEnd()) {
                QString l = in.readLine().trimmed();
                if (!l.isEmpty() && l != peer) remaining << l;
            }
            f.close();
        }
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            for (const QString& l : remaining) out << l << "\n";
            f.close();
        }
    }

} // anonymous namespace

// ══════════════════════════════════════════════════════════════════════════════
//  ChatView
// ══════════════════════════════════════════════════════════════════════════════

ChatView::ChatView(const QString& currentUser, const QString& peerUsername, QWidget* parent)
    : QWidget(parent), m_currentUser(currentUser), m_peer(peerUsername)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Chat header ──────────────────────────────────────────────────────────
    auto* header = new QFrame;
    header->setStyleSheet("QFrame { background: #111119; border-bottom: 1px solid #1C1C2A; }");
    header->setFixedHeight(56);
    auto* hRow = new QHBoxLayout(header);
    hRow->setContentsMargins(20, 0, 16, 0);
    hRow->setSpacing(12);

    // Avatar + peer name
    QString ini = peerUsername.isEmpty() ? "?" : peerUsername.left(1).toUpper();
    hRow->addWidget(avatar(ini, 34), 0, Qt::AlignVCenter);
    auto* peerLbl = new QLabel(peerUsername);
    peerLbl->setStyleSheet("font-size: 15px; font-weight: 700; color: #E0E0FF;");
    hRow->addWidget(peerLbl, 1);

    // Delete chat button
    auto* delBtn = makeDanger("🗑  Delete Chat");
    delBtn->setFixedHeight(30);
    delBtn->setFixedWidth(110);
    connect(delBtn, &QPushButton::clicked, this, &ChatView::onDeleteChat);
    hRow->addWidget(delBtn, 0, Qt::AlignVCenter);

    root->addWidget(header);

    // ── Bubble area ──────────────────────────────────────────────────────────
    m_scroll = new QScrollArea;
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_bubbleContainer = new QWidget;
    m_bubbleContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_bubbleLayout = new QVBoxLayout(m_bubbleContainer);
    m_bubbleLayout->setContentsMargins(16, 16, 16, 16);
    m_bubbleLayout->setSpacing(8);
    m_bubbleLayout->addStretch(1);   // push bubbles to bottom

    m_scroll->setWidget(m_bubbleContainer);
    root->addWidget(m_scroll, 1);

    // ── Input row ────────────────────────────────────────────────────────────
    auto* inputBar = new QFrame;
    inputBar->setStyleSheet("QFrame { background: #0F0F16; border-top: 1px solid #1C1C2A; }");
    auto* iRow = new QHBoxLayout(inputBar);
    iRow->setContentsMargins(14, 10, 14, 10);
    iRow->setSpacing(10);

    m_input = new QLineEdit;
    m_input->setPlaceholderText("Type a message...");
    m_input->setFixedHeight(40);
    connect(m_input, &QLineEdit::returnPressed, this, &ChatView::onSend);
    iRow->addWidget(m_input, 1);

    auto* sendBtn = makePrimary("Send");
    sendBtn->setFixedWidth(72);
    sendBtn->setFixedHeight(40);
    connect(sendBtn, &QPushButton::clicked, this, &ChatView::onSend);
    iRow->addWidget(sendBtn);

    root->addWidget(inputBar);

    loadMessages();
}

QString ChatView::chatFilePath() const {
    return msgFilePath(m_currentUser, m_peer);
}

void ChatView::clearMessages() {
    // Remove all except the trailing stretch (last item)
    while (m_bubbleLayout->count() > 1) {
        QLayoutItem* it = m_bubbleLayout->takeAt(0);
        if (it->widget()) it->widget()->deleteLater();
        delete it;
    }
}

void ChatView::appendBubble(const QString& sender, const QString& text) {
    bool isMine = (sender == m_currentUser);

    // Wrapper aligns bubble left or right
    auto* wrapper = new QHBoxLayout;
    wrapper->setContentsMargins(0, 0, 0, 0);

    auto* bubble = new QLabel(text);
    bubble->setWordWrap(true);
    bubble->setMaximumWidth(420);
    bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    if (isMine) {
        bubble->setStyleSheet(
            "background: #3535CC; color: #FFFFFF; border-radius: 12px 12px 2px 12px;"
            "padding: 9px 14px; font-size: 13px;");
        wrapper->addStretch(1);
        wrapper->addWidget(bubble);
    }
    else {
        bubble->setStyleSheet(
            "background: #1A1A2E; color: #CCCCEE; border-radius: 12px 12px 12px 2px;"
            "padding: 9px 14px; font-size: 13px;");
        bubble->setToolTip(sender);   // show sender name on hover for non-self
        // Small name label above
        auto* col = new QVBoxLayout;
        col->setSpacing(2);
        auto* nameLbl = new QLabel(sender);
        nameLbl->setStyleSheet("font-size: 10px; color: #44446A; font-weight: 600;");
        col->addWidget(nameLbl);
        col->addWidget(bubble);
        wrapper->addLayout(col);
        wrapper->addStretch(1);
    }

    // Insert before trailing stretch
    int insertPos = m_bubbleLayout->count() - 1;
    auto* wrapWidget = new QWidget;
    wrapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* wl = new QHBoxLayout(wrapWidget);
    wl->setContentsMargins(0, 0, 0, 0);
    if (isMine) {
        wl->addStretch(1);
        wl->addWidget(bubble);
    }
    else {
        auto* col = new QVBoxLayout;
        col->setSpacing(2);
        col->setContentsMargins(0, 0, 0, 0);
        auto* nameLbl2 = new QLabel(sender);
        nameLbl2->setStyleSheet("font-size: 10px; color: #44446A; font-weight: 600;");
        // bubble already created — reuse
        col->addWidget(nameLbl2);
        col->addWidget(bubble);
        wl->addLayout(col);
        wl->addStretch(1);
    }
    m_bubbleLayout->insertWidget(insertPos, wrapWidget);

    // Auto-scroll to bottom
    QTimer::singleShot(0, m_scroll, [this]() {
        m_scroll->verticalScrollBar()->setValue(
            m_scroll->verticalScrollBar()->maximum());
        });
}

void ChatView::loadMessages() {
    clearMessages();
    QFile f(chatFilePath());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;
        // Format: sender|timestamp|text
        int firstPipe = line.indexOf('|');
        if (firstPipe < 0) continue;
        int secondPipe = line.indexOf('|', firstPipe + 1);
        if (secondPipe < 0) continue;
        QString sender = line.left(firstPipe);
        // timestamp skipped for display
        QString text = line.mid(secondPipe + 1);
        if (!text.isEmpty()) appendBubble(sender, text);
    }
    f.close();
}

void ChatView::refresh() {
    loadMessages();
}
// Add this helper function for ChatView (before onSend method)
void ChatView::markConversationUnread(const QString& receiver, const QString& sender) {
    QDir().mkpath("data/Messages");
    QString unreadPath = "data/Messages/" + receiver + "_unread.txt";

    // Check if sender is already in unread list
    QFile checkFile(unreadPath);
    bool alreadyExists = false;
    if (checkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&checkFile);
        while (!in.atEnd()) {
            if (in.readLine().trimmed() == sender) {
                alreadyExists = true;
                break;
            }
        }
        checkFile.close();
    }

    // Add to unread list if not already there
    if (!alreadyExists) {
        QFile unreadFile(unreadPath);
        if (unreadFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&unreadFile);
            out << sender << "\n";
            unreadFile.close();
        }
    }
}
// Fix #2: Add message notifications when sending
void ChatView::onSend() {
    QString text = m_input->text().trimmed();
    if (text.isEmpty()) return;
    if (text.contains('|')) {
        QMessageBox::warning(this, "Novo", "Messages cannot contain '|'.");
        return;
    }

    // Append to file
    QDir().mkpath("data/Messages");
    QFile f(chatFilePath());
    if (f.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&f);
        QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        out << m_currentUser << "|" << ts << "|" << text << "\n";
        f.close();
    }

    // Ensure both sides have each other in their index
    ensureInIndex(m_currentUser, m_peer);
    ensureInIndex(m_peer, m_currentUser);

    // FIX #2: Create notification for the recipient
    QDir().mkpath("data/Notifications");
    QString notifPath = "data/Notifications/" + m_peer + "_notif.txt";
    QFile notifFile(notifPath);
    if (notifFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream notifOut(&notifFile);
        QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        notifOut << "message|" << m_currentUser << " sent you a message|" << ts << "|0\n";
        notifFile.close();
    }

    // FIX #3: Mark conversation as unread for recipient
    markConversationUnread(m_peer, m_currentUser);

    m_input->clear();
    appendBubble(m_currentUser, text);
}

void ChatView::onDeleteChat() {
    auto r = QMessageBox::question(this, "Delete Chat",
        "Delete the entire conversation with @" + m_peer + "?",
        QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;

    // Delete the message file
    QFile::remove(chatFilePath());

    // Remove peer from both index files
    removeFromIndex(m_currentUser, m_peer);
    removeFromIndex(m_peer, m_currentUser);

    emit chatDeleted(m_peer);
}

// ══════════════════════════════════════════════════════════════════════════════
//  MessagesPage
// ══════════════════════════════════════════════════════════════════════════════

// static — reads the index file for username
QStringList MessagesPage::knownPeers(const QString& username) {
    QStringList peers;
    QFile f("data/Messages/" + username + "_index.txt");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
            QString l = in.readLine().trimmed();
            if (!l.isEmpty()) peers << l;
        }
        f.close();
    }
    return peers;
}

MessagesPage::MessagesPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent),
    m_currentUser(currentUser), m_allUsers(allUsers), m_userCount(userCount),
    m_chatView(nullptr)
{
    QDir().mkpath("data/Messages");

    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── LEFT PANEL (conversation list) ───────────────────────────────────────
    m_leftPanel = new QWidget;
    m_leftPanel->setFixedWidth(240);
    m_leftPanel->setObjectName("sidebar");   // reuse sidebar style
    m_leftPanel->setStyleSheet(
        "QWidget { background: #0F0F16; border-right: 1px solid #1C1C2A; }");

    auto* ll = new QVBoxLayout(m_leftPanel);
    ll->setContentsMargins(12, 20, 12, 16);
    ll->setSpacing(10);

    // Title
    auto* titleLbl = new QLabel("Messages");
    titleLbl->setStyleSheet("font-size: 17px; font-weight: 700; color: #E0E0FF; padding-bottom: 4px;");
    ll->addWidget(titleLbl);

    // Search input + button
    auto* searchRow = new QHBoxLayout;
    searchRow->setSpacing(6);
    m_searchInput = new QLineEdit;
    m_searchInput->setPlaceholderText("Username...");
    m_searchInput->setFixedHeight(36);
    auto* goBtn = makePrimary("Chat");
    goBtn->setFixedHeight(36);
    goBtn->setFixedWidth(52);
    connect(goBtn, &QPushButton::clicked, this, &MessagesPage::onSearchUser);
    connect(m_searchInput, &QLineEdit::returnPressed, this, &MessagesPage::onSearchUser);
    searchRow->addWidget(m_searchInput, 1);
    searchRow->addWidget(goBtn);
    ll->addLayout(searchRow);

    ll->addWidget([&]() {   // thin divider
        auto* f = new QFrame; f->setFrameShape(QFrame::HLine);
        f->setFixedHeight(1);
        f->setStyleSheet("background: #1A1A28; border: none;");
        return f;
        }());

    // Scrollable conversation list
    auto* convScroll = new QScrollArea;
    convScroll->setWidgetResizable(true);
    convScroll->setFrameShape(QFrame::NoFrame);
    convScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_convListContent = new QWidget;
    m_convListLayout = new QVBoxLayout(m_convListContent);
    m_convListLayout->setContentsMargins(0, 4, 0, 4);
    m_convListLayout->setSpacing(2);
    m_convListLayout->addStretch(1);

    convScroll->setWidget(m_convListContent);
    ll->addWidget(convScroll, 1);

    root->addWidget(m_leftPanel);

    // ── RIGHT PANEL (placeholder / chat view) ─────────────────────────────────
    m_rightStack = new QStackedWidget;

    // Placeholder (index 0)
    auto* placeholder = new QWidget;
    auto* pl = new QVBoxLayout(placeholder);
    pl->setAlignment(Qt::AlignCenter);
    auto* pIcon = new QLabel("✉");
    pIcon->setAlignment(Qt::AlignCenter);
    pIcon->setStyleSheet("font-size: 52px; color: #1E1E38;");
    auto* pLbl = new QLabel("Search a username above\nor select a conversation");
    pLbl->setAlignment(Qt::AlignCenter);
    pLbl->setWordWrap(true);
    pLbl->setStyleSheet("color: #2E2E50; font-size: 14px;");
    pl->addWidget(pIcon);
    pl->addSpacing(12);
    pl->addWidget(pLbl);
    m_rightStack->addWidget(placeholder);   // index 0

    root->addWidget(m_rightStack, 1);

    loadConversationList();
}

void MessagesPage::refresh() {
    loadConversationList();
    if (m_chatView) m_chatView->refresh();
}
// Fix #3: Add unread indicator to conversation buttons
void MessagesPage::addConversationButton(const QString& peer) {
    auto* btn = new QPushButton;
    btn->setObjectName("sidebarBtn");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedHeight(46);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // FIX #3: Check if conversation has unread messages
    QString unreadPath = "data/Messages/" + QString::fromStdString(m_currentUser->getUsername()) + "_unread.txt";
    bool hasUnread = false;
    QFile unreadFile(unreadPath);
    if (unreadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&unreadFile);
        while (!in.atEnd()) {
            if (in.readLine().trimmed() == peer) {
                hasUnread = true;
                break;
            }
        }
        unreadFile.close();
    }

    // Add red dot indicator if unread
    QString displayText = "  " + peer.left(1).toUpper() + "  " + peer;
    if (hasUnread) {
        displayText += " ●";  // red dot
    }

    btn->setText(displayText);
    btn->setStyleSheet(
        "QPushButton { background: transparent; border: none; border-radius: 8px;"
        " text-align: left; padding-left: 10px; color: #7070AA; font-size: 13px; font-weight: 500; }"
        "QPushButton:hover { background: #17171F; color: #AAAAEE; }"
        "QPushButton:checked { background: #1B1B2A; color: #7C7CFF; border-left: 3px solid #5050F0; }");

    if (hasUnread) {
        btn->setStyleSheet(btn->styleSheet() +
            " QPushButton { color: #FF6060; font-weight: 600; }");  // red text if unread
    }

    btn->setCheckable(true);

    if (peer == m_activePeer) btn->setChecked(true);

    connect(btn, &QPushButton::clicked, this, [this, peer, btn]() {
        // Uncheck all other buttons
        QList<QPushButton*> siblings =
            m_convListContent->findChildren<QPushButton*>();
        for (auto* s : siblings) s->setChecked(false);
        btn->setChecked(true);

        // FIX #3: Clear unread status when conversation is opened
        QString me = QString::fromStdString(m_currentUser->getUsername());
        QString unreadPath = "data/Messages/" + me + "_unread.txt";
        QFile unreadFile(unreadPath);
        QString remaining;
        if (unreadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&unreadFile);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty() && line != peer) {
                    remaining += line + "\n";
                }
            }
            unreadFile.close();
        }
        if (unreadFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&unreadFile);
            out << remaining;
            unreadFile.close();
        }

        onConversationSelected(peer);
        });

    m_convListLayout->insertWidget(m_convListLayout->count() - 1, btn);
}
void MessagesPage::clearConversationList() {
    while (m_convListLayout->count() > 1) {
        QLayoutItem* it = m_convListLayout->takeAt(0);
        if (it->widget()) it->widget()->deleteLater();
        delete it;
    }
}

void MessagesPage::loadConversationList() {
    clearConversationList();
    if (!m_currentUser) return;

    QString me = QString::fromStdString(m_currentUser->getUsername());
    QStringList peers = knownPeers(me);

    for (const QString& peer : peers)
        addConversationButton(peer);
}


void MessagesPage::onSearchUser() {
    QString peer = m_searchInput->text().trimmed();
    if (peer.isEmpty()) return;
    if (!m_currentUser) return;

    QString me = QString::fromStdString(m_currentUser->getUsername());
    if (peer == me) {
        QMessageBox::warning(this, "Novo", "You cannot message yourself.");
        return;
    }

    // FIX: Check if user exists before opening chat
    bool userExists = false;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] && m_allUsers[i]->getUsername() == peer.toStdString()) {
            userExists = true;
            break;
        }
    }

    if (!userExists) {
        QMessageBox::warning(this, "Novo", "User '@" + peer + "' does not exist.");
        return;
    }

    m_searchInput->clear();
    openChatWith(peer);
}

void MessagesPage::openChatWith(const QString& peer) {
    if (!m_currentUser) return;
    QString me = QString::fromStdString(m_currentUser->getUsername());

    m_activePeer = peer;

    // Rebuild conversation list so the new peer appears immediately
    loadConversationList();

    // Remove old ChatView from stack if present
    if (m_chatView) {
        m_rightStack->removeWidget(m_chatView);
        m_chatView->deleteLater();
        m_chatView = nullptr;
    }

    // Create new ChatView
    m_chatView = new ChatView(me, peer);
    connect(m_chatView, &ChatView::chatDeleted, this, &MessagesPage::onChatDeleted);
    m_rightStack->addWidget(m_chatView);   // index 1
    m_rightStack->setCurrentWidget(m_chatView);

    // If this peer isn't in the list yet, add them (first message hasn't been sent)
    ensureInIndex(me, peer);
    ensureInIndex(peer, me);

    // Re-render list so button appears highlighted
    loadConversationList();
}

void MessagesPage::onConversationSelected(const QString& peer) {
    openChatWith(peer);
}

void MessagesPage::onChatDeleted(const QString& peer) {
    m_activePeer.clear();

    // Remove ChatView from stack
    if (m_chatView) {
        m_rightStack->removeWidget(m_chatView);
        m_chatView->deleteLater();
        m_chatView = nullptr;
    }
    m_rightStack->setCurrentIndex(0);   // back to placeholder

    // Rebuild list (peer is now gone from index)
    loadConversationList();
}

// ══════════════════════════════════════════════════════════════════════════════
//  AdminPage
// ══════════════════════════════════════════════════════════════════════════════
AdminPage::AdminPage(User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent), m_allUsers(allUsers), m_userCount(userCount)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);
    outer->addWidget(makeLabel("Admin Dashboard", "pageTitle"));

    auto* controlRow = new QHBoxLayout;
    controlRow->setContentsMargins(0, 0, 0, 0);
    controlRow->setSpacing(10);
    auto* reviewUsersBtn = makePrimary("Reported Users");
    auto* reviewPostsBtn = makePrimary("Reported Posts");
    auto* notifsBtn = makeSecondary("🔔 Notifications"); // ADD THIS
    auto* refreshBtn = makeSecondary("↻ Refresh");
    reviewUsersBtn->setFixedHeight(36);
    reviewPostsBtn->setFixedHeight(36);
    notifsBtn->setFixedHeight(36);                           // ADD THIS
    refreshBtn->setFixedHeight(36);
    connect(reviewUsersBtn, &QPushButton::clicked, this, &AdminPage::onReviewReportedUsers);
    connect(reviewPostsBtn, &QPushButton::clicked, this, &AdminPage::onReviewReportedPosts);
    connect(notifsBtn, &QPushButton::clicked, this, &AdminPage::loadAdminNotifications); // ADD THIS
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPage::refresh);
    controlRow->addWidget(reviewUsersBtn);
    controlRow->addWidget(reviewPostsBtn);
    controlRow->addWidget(notifsBtn);                        // ADD THIS
    controlRow->addWidget(refreshBtn);
    controlRow->addStretch(1);
    outer->addLayout(controlRow);

    m_scrollArea = makeScrollArea(m_content, m_layout);
    outer->addWidget(m_scrollArea, 1);
}
void AdminPage::loadAdminNotifications() {
    // Clear layout
    while (m_layout->count() > 1) {
        QLayoutItem* item = m_layout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QFile file("data/Admin/admin_notifications.txt");
    bool any = false;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            // Format written by User::reportUser():
            // timestamp|type|message|isRead
            QStringList parts = line.split("|");
            if (parts.size() < 4) continue;

            QString timestamp = parts[0];
            QString type = parts[1];
            QString message = parts[2];
            bool    isRead = (parts[3] == "1");

            auto* card = new QFrame;
            card->setObjectName("notifItem");
            card->setProperty("unread", !isRead);
            card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            // re-polish so QSS [unread="true"] applies
            card->style()->unpolish(card);
            card->style()->polish(card);

            auto* row = new QHBoxLayout(card);
            row->setContentsMargins(16, 12, 16, 12);
            row->setSpacing(12);

            auto* icon = new QLabel(type.contains("user") ? "👤" : "⚑");
            icon->setFixedSize(32, 32);
            icon->setAlignment(Qt::AlignCenter);
            icon->setStyleSheet("font-size:16px;");

            auto* col = new QVBoxLayout;
            col->setSpacing(3);
            auto* msgLbl = new QLabel(message);
            msgLbl->setWordWrap(true);
            msgLbl->setStyleSheet(isRead
                ? "font-size:13px; color:#666688;"
                : "font-size:13px; color:#EE7777; font-weight:600;");
            auto* tsLbl = new QLabel(timestamp);
            tsLbl->setStyleSheet("font-size:11px; color:#3A3A58;");
            col->addWidget(msgLbl);
            col->addWidget(tsLbl);

            row->addWidget(icon, 0, Qt::AlignVCenter);
            row->addLayout(col, 1);

            if (!isRead) {
                auto* dot = new QLabel("●");
                dot->setStyleSheet("color:#EE5555; font-size:9px;");
                dot->setFixedWidth(12);
                row->addWidget(dot, 0, Qt::AlignVCenter);
            }

            m_layout->insertWidget(m_layout->count() - 1, card);
            any = true;
        }
        file.close();
    }

    if (!any) {
        auto* empty = new QLabel("No admin notifications yet.");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A; font-size:14px; padding:40px 0;");
        m_layout->insertWidget(0, empty);
    }

    // Mark all as read
    markAdminNotifsRead();
}

void AdminPage::markAdminNotifsRead() {
    QString path = "data/Admin/admin_notifications.txt";
    QFile file(path);
    QStringList lines;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split("|");
            if (parts.size() >= 4) {
                parts[3] = "1";
                lines << parts.join("|");
            }
        }
        file.close();
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const QString& l : lines) out << l << "\n";
        file.close();
    }
}
void AdminPage::refresh() { 
    loadReportedUsers();
    
}
void AdminPage::onReviewReportedUsers() { loadReportedUsers(); }
void AdminPage::onReviewReportedPosts() { loadReportedPosts(); }

void AdminPage::loadReportedUsers() {
    while (m_layout->count() > 1) {
        QLayoutItem* item = m_layout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    bool hasReported = false;
    for (int i = 0; i < m_userCount; ++i) {
        if (!m_allUsers[i]) continue;

        m_allUsers[i]->loadFromFile(m_allUsers[i]->getUsername());

        if (!m_allUsers[i]->getIsReported()) continue;

        std::string targetUsername = m_allUsers[i]->getUsername();

        auto* card = new QFrame;
        card->setObjectName("postCard");
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        auto* row = new QHBoxLayout(card);
        row->setContentsMargins(16, 12, 16, 12);
        row->setSpacing(10);

        // User info
        auto* infoCol = new QVBoxLayout;
        auto* label = makeLabel("⚠ User: " + QString::fromStdString(targetUsername));
        label->setStyleSheet("font-size: 14px; font-weight: 600; color: #EE7777;");
        auto* reportLbl = makeLabel("Reported by multiple users");
        reportLbl->setStyleSheet("font-size: 11px; color: #664444;");
        infoCol->addWidget(label);
        infoCol->addWidget(reportLbl);

        // DELETE button — replaces old Ban/Unban
        auto* deleteBtn = makeDanger("Delete User");
        deleteBtn->setFixedWidth(100);
        deleteBtn->setFixedHeight(32);
        connect(deleteBtn, &QPushButton::clicked, this, [this, targetUsername]() {
            auto r = QMessageBox::question(this, "Admin",
                "Permanently delete user '@" + QString::fromStdString(targetUsername) + "'?",
                QMessageBox::Yes | QMessageBox::No);
            if (r != QMessageBox::Yes) return;

            for (int idx = 0; idx < m_userCount; ++idx) {
                if (m_allUsers[idx] && m_allUsers[idx]->getUsername() == targetUsername) {
                    m_allUsers[idx]->deleteAccount(m_allUsers, m_userCount);
                    QMessageBox::information(this, "Admin", "User deleted.");
                    refresh();
                    return;
                }
            }
            });

        row->addLayout(infoCol, 1);
        row->addWidget(deleteBtn);
        m_layout->insertWidget(m_layout->count() - 1, card);
        hasReported = true;
    }
    if (!hasReported) {
        auto* empty = new QLabel("No reported users at the moment.");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #28284A; font-size: 14px; padding: 40px 0;");
        m_layout->insertWidget(0, empty);
    }
}

void AdminPage::loadReportedPosts() {
    while (m_layout->count() > 1) {
        QLayoutItem* item = m_layout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    bool hasReported = false;
    for (int i = 0; i < m_userCount; ++i) {
        if (!m_allUsers[i]) continue;

        
        m_allUsers[i]->loadAllPosts();

        for (int j = 0; j < m_allUsers[i]->getPostCount(); ++j) {
            Posts* post = m_allUsers[i]->getPostByIndex(j);
            if (!post || !post->isValid() || !post->getIsReported()) continue;

            std::string postId = post->getPostId();
            std::string creatorName = post->getCreatorUsername();

            auto* card = new QFrame;
            card->setObjectName("postCard");
            card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            auto* cl = new QVBoxLayout(card);
            cl->setContentsMargins(16, 12, 16, 12);
            cl->setSpacing(8);
            auto* hdr = new QHBoxLayout;
            auto* label = makeLabel("⚠ Post by: " + QString::fromStdString(creatorName));
            label->setStyleSheet("font-size: 13px; font-weight: 600; color: #EE7777;");
            hdr->addWidget(label, 1);
            cl->addLayout(hdr);
            auto* content = makeLabel(QString::fromStdString(post->getContent()));
            content->setStyleSheet("font-size: 12px; color: #B8B8D0;");
            cl->addWidget(content);
            auto* btnRow = new QHBoxLayout;
            btnRow->addStretch(1);
            auto* deleteBtn = makeDanger("Delete Post");
            deleteBtn->setFixedWidth(110);
            deleteBtn->setFixedHeight(32);
            connect(deleteBtn, &QPushButton::clicked, this, [this, postId, creatorName]() {
                for (int idx = 0; idx < m_userCount; ++idx) {
                    if (!m_allUsers[idx]) continue;
                    if (m_allUsers[idx]->getUsername() == creatorName) {
                        m_allUsers[idx]->deletePost(postId);
                        QMessageBox::information(this, "Admin", "Post deleted.");
                        loadReportedPosts();
                        break;
                    }
                }
                });
            btnRow->addWidget(deleteBtn);
            cl->addLayout(btnRow);
            m_layout->insertWidget(m_layout->count() - 1, card);
            hasReported = true;
        }
    }
    if (!hasReported) {
        auto* empty = new QLabel("No reported posts at the moment.");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #28284A; font-size: 14px; padding: 40px 0;");
        m_layout->insertWidget(0, empty);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  ProfilePage
//  The profile page wraps ALL its content in a single QScrollArea so there is
//  no inner-page overflow. The tabs (My Posts / Saved) have their own
//  QStackedWidget inside but NO nested scroll areas — only the outer scroll
//  area scrolls.
// ══════════════════════════════════════════════════════════════════════════════
ProfilePage::ProfilePage(User* currentUser, User** allUsers, int* userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCountPtr(userCount)
{
    // Outer layout — just holds the single scroll area edge-to-edge
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outer->addWidget(scroll);

    // Inner container — everything lives here
    auto* inner = new QWidget;
    inner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto* layout = new QVBoxLayout(inner);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(16);

    layout->addWidget(makeLabel("Profile", "pageTitle"));

    // ── Profile card ─────────────────────────────────────────────────────────
    auto* pCard = new QFrame; pCard->setObjectName("postCard");
    pCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* pcl = new QVBoxLayout(pCard);
    pcl->setContentsMargins(24, 20, 24, 20);
    pcl->setSpacing(16);

    auto* infoRow = new QHBoxLayout;
    infoRow->setSpacing(16);
    infoRow->setContentsMargins(0, 0, 0, 0);
    QString ini = currentUser ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper() : "?";
    infoRow->addWidget(avatar(ini, 64), 0, Qt::AlignVCenter);

    auto* infoCol = new QVBoxLayout; infoCol->setSpacing(5);
    m_usernameLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getUsername()) : "");
    m_usernameLabel->setStyleSheet("font-size:20px; font-weight:800; color:#E0E0FF;");
    m_bioLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getBio()) : "");
    m_bioLabel->setStyleSheet("font-size:13px; color:#56567A;");
    infoCol->addWidget(m_usernameLabel);
    infoCol->addWidget(m_bioLabel);
    infoRow->addLayout(infoCol, 1);
    pcl->addLayout(infoRow);

    auto* statsRow = new QHBoxLayout; statsRow->setContentsMargins(0, 0, 0, 0);
    auto addStat = [&](QLabel*& outLabel, const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout; c->setSpacing(2);
        outLabel = new QLabel(val);
        outLabel->setStyleSheet("font-size:22px; font-weight:700; color:#6060FF;");
        outLabel->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size:10px; color:#36365A; letter-spacing:1px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(outLabel); c->addWidget(l);
        statsRow->addLayout(c); statsRow->addSpacing(24);
        };
    if (currentUser) {
        addStat(m_postsCountLabel, QString::number(currentUser->getPostCount()), "POSTS");
        addStat(m_followersCountLabel, QString::number(currentUser->getFollowersCount()), "FOLLOWERS");
        addStat(m_followingCountLabel, QString::number(currentUser->getFollowingCount()), "FOLLOWING");
    }
    statsRow->addStretch(1);
    pcl->addLayout(statsRow);
    layout->addWidget(pCard);

    // ── Edit bio card ────────────────────────────────────────────────────────
    auto* bioCard = new QFrame; bioCard->setObjectName("postCard");
    bioCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* bcl = new QVBoxLayout(bioCard);
    bcl->setContentsMargins(22, 18, 22, 18); bcl->setSpacing(10);
    bcl->addWidget(makeLabel("UPDATE BIO", "sectionLbl"));
    m_newBioInput = new QLineEdit;
    m_newBioInput->setPlaceholderText("New bio...");
    m_newBioInput->setFixedHeight(42);
    bcl->addWidget(m_newBioInput);
    auto* saveBioBtn = makePrimary("Save Bio");
    saveBioBtn->setFixedWidth(100); saveBioBtn->setFixedHeight(36);
    connect(saveBioBtn, &QPushButton::clicked, this, &ProfilePage::onUpdateBio);
    auto* bioRow = new QHBoxLayout; bioRow->addStretch(1); bioRow->addWidget(saveBioBtn);
    bcl->addLayout(bioRow);
    layout->addWidget(bioCard);

    // ── Change password card ─────────────────────────────────────────────────
    auto* passCard = new QFrame; passCard->setObjectName("postCard");
    passCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* playout = new QVBoxLayout(passCard);
    playout->setContentsMargins(22, 18, 22, 18); playout->setSpacing(10);
    playout->addWidget(makeLabel("CHANGE PASSWORD", "sectionLbl"));
    m_newPassInput = new QLineEdit;
    m_newPassInput->setPlaceholderText("New password...");
    m_newPassInput->setEchoMode(QLineEdit::Password);
    m_newPassInput->setFixedHeight(42);
    playout->addWidget(m_newPassInput);
    auto* savePassBtn = makePrimary("Update Password");
    savePassBtn->setFixedWidth(150); savePassBtn->setFixedHeight(36);
    connect(savePassBtn, &QPushButton::clicked, this, &ProfilePage::onUpdatePassword);
    auto* passRow2 = new QHBoxLayout; passRow2->addStretch(1); passRow2->addWidget(savePassBtn);
    playout->addLayout(passRow2);
    layout->addWidget(passCard);

    // ── Posts / Saved tabs card ───────────────────────────────────────────────
    // This card contains the two sub-pages but does NOT have its own scroll area.
    // It has a minimum height so the outer scroll can drive scrolling.
    auto* tabsCard = new QFrame; tabsCard->setObjectName("postCard");
    tabsCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto* tabsCl = new QVBoxLayout(tabsCard);
    tabsCl->setContentsMargins(0, 0, 0, 0); tabsCl->setSpacing(0);

    auto* tabBar = new QHBoxLayout;
    tabBar->setContentsMargins(16, 12, 16, 0); tabBar->setSpacing(6);
    auto* tabMyPosts = new QPushButton("My Posts");
    auto* tabSaved = new QPushButton("Saved Posts");
    tabMyPosts->setObjectName("primaryBtn");
    tabMyPosts->setFixedHeight(34);
    tabSaved->setObjectName("secondaryBtn");
    tabSaved->setFixedHeight(34);
    tabBar->addWidget(tabMyPosts); tabBar->addWidget(tabSaved); tabBar->addStretch(1);
    tabsCl->addLayout(tabBar);
    tabsCl->addSpacing(8);
    tabsCl->addWidget(divider());

    auto* tabStack = new QStackedWidget;
    tabStack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    tabStack->setMinimumHeight(320);
    m_myPostsPage = new MyPostsPage(currentUser);
    m_savedPage = new SavedPostsPage(currentUser, allUsers, *userCount);
    tabStack->addWidget(m_myPostsPage);
    tabStack->addWidget(m_savedPage);
    tabsCl->addWidget(tabStack);

    auto switchTabs = [this, tabStack, tabMyPosts, tabSaved](int idx) {
        tabStack->setCurrentIndex(idx);
        if (idx == 0) m_myPostsPage->refresh();
        else          m_savedPage->refresh();
        tabMyPosts->setObjectName(idx == 0 ? "primaryBtn" : "secondaryBtn");
        tabSaved->setObjectName(idx == 1 ? "primaryBtn" : "secondaryBtn");
        for (auto* b : { tabMyPosts, tabSaved }) {
            if (b->style()) { b->style()->unpolish(b); b->style()->polish(b); }
        }
        };
    connect(tabMyPosts, &QPushButton::clicked, this, [switchTabs]() { switchTabs(0); });
    connect(tabSaved, &QPushButton::clicked, this, [switchTabs]() { switchTabs(1); });
    layout->addWidget(tabsCard);

    // ── Danger zone card ─────────────────────────────────────────────────────
    auto* dangerCard = new QFrame; dangerCard->setObjectName("postCard");
    dangerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    dangerCard->setStyleSheet("QFrame#postCard { border:1px solid #3A1212; background:#0E0606; }");
    auto* dl = new QVBoxLayout(dangerCard);
    dl->setContentsMargins(22, 18, 22, 18); dl->setSpacing(10);
    auto* dtitle = makeLabel("DANGER ZONE", "sectionLbl");
    dtitle->setStyleSheet("font-size:10px; font-weight:600; color:#5A2222; letter-spacing:1.4px;");
    auto* ddesc = makeLabel("Permanently deletes your account and all associated data.");
    ddesc->setStyleSheet("font-size:12px; color:#4A3030;");
    auto* delBtn = makeDanger("Delete My Account");
    delBtn->setFixedWidth(160); delBtn->setFixedHeight(36);
    connect(delBtn, &QPushButton::clicked, this, &ProfilePage::onDeleteAccount);
    auto* delRow = new QHBoxLayout; delRow->addStretch(1); delRow->addWidget(delBtn);
    dl->addWidget(dtitle); dl->addWidget(ddesc); dl->addLayout(delRow);
    layout->addWidget(dangerCard);

    layout->addStretch(1);
    scroll->setWidget(inner);
}

void ProfilePage::refresh() {
    if (!m_user) return;
    m_usernameLabel->setText(QString::fromStdString(m_user->getUsername()));
    m_bioLabel->setText(QString::fromStdString(m_user->getBio()));
    if (m_postsCountLabel)     m_postsCountLabel->setText(QString::number(m_user->getPostCount()));
    if (m_followersCountLabel) m_followersCountLabel->setText(QString::number(m_user->getFollowersCount()));
    if (m_followingCountLabel) m_followingCountLabel->setText(QString::number(m_user->getFollowingCount()));
    m_myPostsPage->refresh();
    // FIX #9: also refresh saved tab so it doesn't show stale data
    m_savedPage->refresh();
}

void ProfilePage::onUpdateBio() {
    if (!m_user) return;
    QString bio = m_newBioInput->text().trimmed();
    if (bio.isEmpty()) { QMessageBox::warning(this, APP_NAME, "Bio cannot be empty."); return; }
    if (bio.contains('|')) { QMessageBox::warning(this, APP_NAME, "Bio cannot contain '|'."); return; }
    if (bio.length() > 100) { QMessageBox::warning(this, APP_NAME, "Bio must be under 100 characters."); return; }
    m_user->setBio(bio.toStdString());
    m_user->saveToFile();
    m_bioLabel->setText(bio);
    m_newBioInput->clear();
    QMessageBox::information(this, APP_NAME, "Bio updated.");
}

void ProfilePage::onUpdatePassword() {
    if (!m_user) return;
    QString pass = m_newPassInput->text();
    if (pass.isEmpty()) { QMessageBox::warning(this, APP_NAME, "Password cannot be empty."); return; }
    PasswordChecker checker(8);
    if (!checker.checkAll(pass.toStdString())) {
        int s = checker.calculateStrength(pass.toStdString());
        QMessageBox::warning(this, "Weak Password",
            QString("Strength: %1/100. Needs uppercase, digit & special character.").arg(s));
        return;
    }
    m_user->setPassword(pass.toStdString());
    m_user->saveToFile();
    m_newPassInput->clear();
    QMessageBox::information(this, APP_NAME, "Password updated.");
}

void ProfilePage::onDeleteAccount() {
    auto r = QMessageBox::warning(this, "Delete Account",
        "This will permanently delete your account.\nAre you absolutely sure?",
        QMessageBox::Yes | QMessageBox::Cancel);
    if (r == QMessageBox::Yes) {
        m_user->deleteAccount(m_allUsers, *m_userCountPtr);
        emit accountDeleted();
        // FIX #6: removed QApplication::quit() so onLogout can finish teardown properly
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  TimeSpentPage
// ══════════════════════════════════════════════════════════════════════════════
TimeSpentPage::TimeSpentPage(QWidget* parent) : QWidget(parent), m_elapsed(0) {
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(20);
    outer->addWidget(makeLabel("Time Spent", "pageTitle"));

    auto* timerCard = new QFrame; timerCard->setObjectName("postCard");
    timerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* tcl = new QVBoxLayout(timerCard);
    tcl->setContentsMargins(40, 36, 40, 36);
    tcl->setSpacing(14);
    tcl->setAlignment(Qt::AlignCenter);
    auto* clockIcon = new QLabel("⏱");
    clockIcon->setAlignment(Qt::AlignCenter);
    clockIcon->setStyleSheet("font-size: 44px;");
    tcl->addWidget(clockIcon);
    m_timerLabel = makeLabel("00:00:00", "timerDisp");
    m_timerLabel->setAlignment(Qt::AlignCenter);
    tcl->addWidget(m_timerLabel);
    m_sessionLabel = makeLabel("Session started: —");
    m_sessionLabel->setAlignment(Qt::AlignCenter);
    m_sessionLabel->setStyleSheet("color: #34345A; font-size: 12px;");
    tcl->addWidget(m_sessionLabel);
    outer->addWidget(timerCard);

    // Info cards row
    auto* infoRow = new QHBoxLayout;
    infoRow->setSpacing(12);
    infoRow->setContentsMargins(0, 0, 0, 0);
    auto makeInfoCard = [&](const QString& ic, const QString& label, const QString& val) {
        auto* f = new QFrame; f->setObjectName("postCard");
        f->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        auto* fl = new QVBoxLayout(f);
        fl->setContentsMargins(18, 16, 18, 16); fl->setSpacing(6);
        fl->setAlignment(Qt::AlignCenter);
        auto* i = new QLabel(ic); i->setAlignment(Qt::AlignCenter); i->setStyleSheet("font-size:22px;");
        auto* v = new QLabel(val); v->setAlignment(Qt::AlignCenter);
        v->setStyleSheet("font-size:17px; font-weight:700; color:#6060FF;");
        auto* l = new QLabel(label); l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet("font-size:10px; color:#34345A; letter-spacing:1px;");
        fl->addWidget(i); fl->addWidget(v); fl->addWidget(l);
        return f;
        };
    infoRow->addWidget(makeInfoCard("🎯", "TODAY'S GOAL", "2 hours"));
    infoRow->addWidget(makeInfoCard("📊", "AVG DAILY", "—"));
    infoRow->addWidget(makeInfoCard("🔥", "STREAK", "—"));
    outer->addLayout(infoRow);
    outer->addStretch(1);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &TimeSpentPage::onTick);
}

void TimeSpentPage::startSession() {
    m_elapsed = 0;
    m_sessionStart = QDateTime::currentDateTime();
    m_sessionLabel->setText("Session started: " + m_sessionStart.toString("hh:mm AP"));
    m_timer->start(1000);
}
void TimeSpentPage::stopSession() { m_timer->stop(); }
void TimeSpentPage::onTick() { ++m_elapsed; updateDisplay(); }
void TimeSpentPage::updateDisplay() {
    int h = m_elapsed / 3600, m = (m_elapsed % 3600) / 60, s = m_elapsed % 60;
    m_timerLabel->setText(QString("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0')));
}

// ══════════════════════════════════════════════════════════════════════════════
//  MainWindow
//  Structure (when logged in):
//
//    QMainWindow
//    └─ central QWidget
//       └─ QVBoxLayout (zero margins)
//          └─ m_rootStack (QStackedWidget)
//             ├─ AuthPage          (index 0)
//             └─ appShell QWidget  (index 1)
//
//  The sidebar has a fixed width set here in C++ (NOT via QSS min/max-width)
//  to guarantee it never conflicts with content area sizing.
// ══════════════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_authPage(nullptr), m_appShell(nullptr), m_shellLayout(nullptr),
    m_sidebar(nullptr),
    m_btnFeed(nullptr), m_btnNotifications(nullptr), m_btnSearch(nullptr),
    m_btnMessages(nullptr), m_btnProfile(nullptr), m_btnTimeSpent(nullptr),
    m_btnSwitchUser(nullptr), m_btnLogout(nullptr),
    m_pages(nullptr), m_feedPage(nullptr), m_notifPage(nullptr),
    m_searchPage(nullptr), m_messagesPage(nullptr), m_profilePage(nullptr),
    m_timeSpentPage(nullptr), m_adminPage(nullptr),
    m_currentUser(nullptr), m_allUsers(nullptr), m_userCount(0),
    m_rootStack(nullptr)
{
    QDir().mkpath("data/Notifications");
    QDir().mkpath("data/Posts");
    QDir().mkpath("data/Following");
    QDir().mkpath("data/Users");
    QDir().mkpath("data/Messages");


    setWindowTitle(APP_NAME);
    // Default size: sensible for 1080p; will resize by layout on smaller screens
    resize(1100, 720);
    setMinimumSize(860, 580);
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

    // FIX #2: use QStackedWidget so only one page is active at a time
    m_rootStack = new QStackedWidget(central);
    m_rootStack->addWidget(m_authPage);
    m_rootStack->addWidget(m_appShell);
    rootLayout->addWidget(m_rootStack);
}

MainWindow::~MainWindow() {}

void MainWindow::onLoginSuccess(User* user, User** allUsers, int userCount) {
    m_currentUser = user;
    m_allUsers = m_authPage->getAllUsers();
    m_userCount = m_authPage->getUserCount();

    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0);
    m_shellLayout->setSpacing(0);

    buildSidebar();
    buildPages();

    m_shellLayout->addWidget(m_sidebar, 0);
    m_shellLayout->addWidget(m_pages, 1);

    m_pages->setCurrentIndex(0);   // set BEFORE showing
    m_appShell->show();
    m_rootStack->setCurrentIndex(1);
    m_timeSpentPage->startSession();
    onNavFeed();
}

void MainWindow::onLoginAdminSuccess() {
    // FIX #3: ensure user database is loaded before handing it to AdminPage
    m_authPage->resetToLogin();
    m_allUsers = m_authPage->getAllUsers();
    m_userCount = m_authPage->getUserCount();

    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0);
    m_shellLayout->setSpacing(0);

    // ── Admin sidebar (minimal) ───────────────────────────────────────────
    m_sidebar = new QWidget;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(210);
    m_sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    auto* sl = new QVBoxLayout(m_sidebar);
    sl->setContentsMargins(12, 22, 12, 18);
    sl->setSpacing(4);

    auto* brand = new QLabel(APP_NAME);
    brand->setStyleSheet("font-size:21px; font-weight:800; color:#6060FF; padding: 0 4px 12px 4px;");
    sl->addWidget(brand);
    sl->addWidget(divider());
    sl->addSpacing(10);
    sl->addWidget(makeLabel("ADMINISTRATOR", "sectionLbl"));
    sl->addStretch(1);

    m_btnLogout = new SidebarButton("←", "Log Out");
    m_btnLogout->setObjectName("sidebarBtnDanger");
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(m_btnLogout);

    // ── Admin content ─────────────────────────────────────────────────────
    m_pages = new QStackedWidget;
    m_pages->setObjectName("pageArea");
    m_adminPage = new AdminPage(m_allUsers, m_userCount);
    m_pages->addWidget(m_adminPage);

    m_shellLayout->addWidget(m_sidebar, 0);
    m_shellLayout->addWidget(m_pages, 1);

    m_rootStack->setCurrentIndex(1);
}

void MainWindow::buildSidebar() {
    m_sidebar = new QWidget;
    m_sidebar->setObjectName("sidebar");
    // Fixed width prevents it from shrinking or growing into the feed area
    m_sidebar->setFixedWidth(210);
    m_sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    auto* sl = new QVBoxLayout(m_sidebar);
    sl->setContentsMargins(12, 22, 12, 18);
    sl->setSpacing(4);

    // Brand
    auto* brand = new QLabel(APP_NAME);
    brand->setStyleSheet("font-size:21px; font-weight:800; color:#6060FF; padding: 0 4px 12px 4px;");
    sl->addWidget(brand);
    sl->addWidget(divider());
    sl->addSpacing(12);

    // Compose button — centred
    auto* plusBtn = new QPushButton("+");
    plusBtn->setObjectName("plusCircleBtn");
    plusBtn->setFixedSize(44, 44);
    plusBtn->setCursor(Qt::PointingHandCursor);
    connect(plusBtn, &QPushButton::clicked, this, &MainWindow::onSidebarCreatePost);
    sl->addWidget(plusBtn, 0, Qt::AlignHCenter);
    sl->addSpacing(16);

    // Nav label
    auto* navLbl = makeLabel("NAVIGATE", "sectionLbl");
    navLbl->setContentsMargins(4, 0, 0, 2);
    sl->addWidget(navLbl);

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

    sl->addWidget(m_btnFeed);
    sl->addWidget(m_btnNotifications);
    sl->addWidget(m_btnSearch);
    sl->addWidget(m_btnMessages);
    sl->addWidget(m_btnProfile);
    sl->addWidget(m_btnTimeSpent);

    sl->addStretch(1); // push everything below to bottom
    sl->addWidget(divider());
    sl->addSpacing(6);

    // User row (avatar + username)
    auto* userRow = new QHBoxLayout;
    userRow->setContentsMargins(4, 0, 4, 0);
    userRow->setSpacing(8);
    QString ini = m_currentUser ? QString::fromStdString(m_currentUser->getUsername()).left(1).toUpper() : "?";
    userRow->addWidget(avatar(ini, 28), 0, Qt::AlignVCenter);
    auto* uname = new QLabel(m_currentUser ? QString::fromStdString(m_currentUser->getUsername()) : "");
    uname->setStyleSheet("font-size:12px; color:#7070AA; font-weight:600;");
    uname->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    uname->setMaximumWidth(110); // prevent overflow on long names
    uname->setWordWrap(false);
    userRow->addWidget(uname, 1);
    sl->addLayout(userRow);
    sl->addSpacing(6);

    m_btnSwitchUser = new SidebarButton("⇄", "Switch User");
    m_btnSwitchUser->setObjectName("sidebarBtnDanger");
    connect(m_btnSwitchUser, &QPushButton::clicked, this, &MainWindow::onSwitchUser);
    sl->addWidget(m_btnSwitchUser);

    m_btnLogout = new SidebarButton("←", "Log Out");
    m_btnLogout->setObjectName("sidebarBtnDanger");
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(m_btnLogout);
}

void MainWindow::buildPages() {
    // Use a plain container — manual show/hide controls visibility
    m_pages = new QStackedWidget;
    m_pages->setObjectName("pageArea");

    m_feedPage = new FeedPage(m_currentUser, m_allUsers, m_userCount);
    m_notifPage = new NotificationsPage(m_currentUser);
    m_searchPage = new SearchPage(m_allUsers, m_userCount, m_currentUser);
    m_messagesPage = new MessagesPage(m_currentUser, m_allUsers, m_userCount);
    m_profilePage = new ProfilePage(m_currentUser, m_allUsers, &m_userCount);
    m_timeSpentPage = new TimeSpentPage;

    connect(m_profilePage, &ProfilePage::accountDeleted,
        this, &MainWindow::onLogout);

    m_pages->addWidget(m_feedPage);
    m_pages->addWidget(m_notifPage);
    m_pages->addWidget(m_searchPage);
    m_pages->addWidget(m_messagesPage);
    m_pages->addWidget(m_profilePage);
    m_pages->addWidget(m_timeSpentPage);

    // Hide everything — onNavFeed() will show feed after login
    m_notifPage->hide();
    m_searchPage->hide();
    m_messagesPage->hide();
    m_profilePage->hide();
    m_timeSpentPage->hide();
}

void MainWindow::onSidebarCreatePost() {
    if (!m_currentUser || !m_feedPage) return;
    onNavFeed();
    m_feedPage->m_composerCard->show();
    m_feedPage->m_postInput->setFocus();
}

void MainWindow::setActiveSidebarButton(SidebarButton* active) {
    SidebarButton* btns[] = {
        m_btnFeed, m_btnNotifications, m_btnSearch,
        m_btnMessages, m_btnProfile, m_btnTimeSpent
    };
    for (auto* b : btns) if (b) b->setActive(b == active);
}




void MainWindow::onNavFeed() {
    if (!m_pages || !m_feedPage) return;
    m_pages->setCurrentIndex(0);
    m_feedPage->refresh();
    setActiveSidebarButton(m_btnFeed);
    // DELETE: animateFade(m_feedPage);
}
void MainWindow::onNavNotifications() {
    if (!m_pages || !m_notifPage) return;
    m_pages->setCurrentIndex(1);
    m_notifPage->refresh();
    setActiveSidebarButton(m_btnNotifications);
    // DELETE: animateFade(m_notifPage);
}
void MainWindow::onNavSearch() {
    if (!m_pages) return;
    m_pages->setCurrentIndex(2);
    setActiveSidebarButton(m_btnSearch);
    // DELETE: animateFade(m_searchPage);
}
void MainWindow::onNavMessages() {
    if (!m_pages || !m_messagesPage) return;
    m_pages->setCurrentIndex(3);
    m_messagesPage->refresh();
    setActiveSidebarButton(m_btnMessages);
}
void MainWindow::onNavProfile() {
    if (!m_pages || !m_profilePage) return;
    m_pages->setCurrentIndex(4);
    m_profilePage->refresh();
    setActiveSidebarButton(m_btnProfile);
    // DELETE: animateFade(m_profilePage);
}
void MainWindow::onNavTimeSpent() {
    if (!m_pages) return;
    m_pages->setCurrentIndex(5);
    setActiveSidebarButton(m_btnTimeSpent);
    // DELETE: animateFade(m_timeSpentPage);
}

void MainWindow::onLogout() {
    auto r = QMessageBox::question(this, APP_NAME,
        "Are you sure you want to log out?",
        QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;
    if (m_currentUser) m_currentUser->logOut();
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    m_appShell->hide();          // hide shell BEFORE teardown
    tearDownShell();
    m_currentUser = nullptr;
    m_authPage->resetToLogin();
    m_rootStack->setCurrentIndex(0);


}
void MainWindow::onSwitchUser() {
    if (m_currentUser) m_currentUser->logOut();
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    m_appShell->hide();          // hide shell BEFORE teardown
    tearDownShell();
    m_currentUser = nullptr;
    m_authPage->resetToLogin();
    m_rootStack->setCurrentIndex(0);
}

void MainWindow::tearDownShell() {
    if (m_appShell) {
        // Delete ALL child widgets of appShell, not just layout items
        QList<QWidget*> children = m_appShell->findChildren<QWidget*>(
            QString(), Qt::FindDirectChildrenOnly);
        for (QWidget* w : children) {
            w->hide();
            w->deleteLater();
        }
        // Now safe to delete the layout
        delete m_appShell->layout();
    }

    m_shellLayout = nullptr; m_sidebar = nullptr;
    m_btnFeed = nullptr; m_btnNotifications = nullptr;
    m_btnSearch = nullptr; m_btnMessages = nullptr;
    m_btnProfile = nullptr; m_btnTimeSpent = nullptr;
    m_btnSwitchUser = nullptr; m_btnLogout = nullptr;
    m_pages = nullptr; m_feedPage = nullptr;
    m_notifPage = nullptr; m_searchPage = nullptr;
    m_messagesPage = nullptr; m_profilePage = nullptr;
    m_timeSpentPage = nullptr; m_adminPage = nullptr;
}