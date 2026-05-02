#define _CRT_SECURE_NO_WARNINGS
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
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QDir>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QScreen>
#include <fstream>
#include <string>

using namespace std;

// ──────────────────────────────────────────────────────────────────────────────
//  APPLICATION NAME
// ──────────────────────────────────────────────────────────────────────────────
static const QString APP_NAME = "Novo";

// ──────────────────────────────────────────────────────────────────────────────
//  GLOBAL STYLESHEET
// ──────────────────────────────────────────────────────────────────────────────
static const QString APP_STYLE = R"(
* {
    font-family: 'Segoe UI', 'Helvetica Neue', Arial, sans-serif;
    color: #E2E2EC;
    box-sizing: border-box;
}
QMainWindow, QWidget#appShell, QWidget#pageArea, QWidget#authBg {
    background: #0C0C10;
}
QWidget#sidebar {
    background: #0F0F16;
    border-right: 1px solid #1C1C28;
}
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
    font-size: 22px;
    font-weight: bold;
}
QPushButton#plusCircleBtn:hover { background: #5252F0; }
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
QPushButton#iconBtn {
    background: transparent;
    border: none;
    color: #44446A;
    font-size: 16px;
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
QLabel#pageTitle   { font-size: 20px; font-weight: 700; color: #E8E8F8; }
QLabel#sectionLbl  { font-size: 10px; font-weight: 600; color: #383858; letter-spacing: 1.4px; }
QLabel#postAuthor  { font-size: 13px; font-weight: 600; color: #CCCCEE; }
QLabel#postTime    { font-size: 11px; color: #3A3A58; }
QLabel#postContent { font-size: 13px; color: #B8B8D0; }
QLabel#timerDisp   { font-size: 52px; font-weight: 700; color: #5555EE; letter-spacing: -2px; }
QLabel#brandTitle  { font-size: 34px; font-weight: 800; color: #6060FF; letter-spacing: -1px; }
QLabel#brandSub    { font-size: 12px; color: #36365A; }
QLabel#authTitle   { font-size: 19px; font-weight: 700; color: #DEDEFF; }
QScrollArea {
    background: #0C0C10;
    border: none;
}
QScrollArea > QWidget > QWidget { background: transparent; }
QScrollArea::viewport { background: transparent; }
QScrollBar:vertical {
    background: transparent;
    width: 8px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background: #2A2A3A;
    min-height: 40px;
    border-radius: 4px;
}
QScrollBar::handle:vertical:hover  { background: #505070; }
QScrollBar::handle:vertical:pressed{ background: #7070FF; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }
QMessageBox { background: #0C0C10; }
QMessageBox QLabel { color: #E8E8F8; font-size: 14px; }
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
//  ANONYMOUS NAMESPACE — UI FACTORY HELPERS
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

    QFrame* divider(QWidget* p = nullptr) {
        auto* f = new QFrame(p);
        f->setFrameShape(QFrame::HLine);
        f->setFixedHeight(1);
        f->setStyleSheet("background: #1A1A28; border: none;");
        return f;
    }

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

    // Scrollable content helper — creates QScrollArea + inner widget + layout
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
        outLayout->addStretch(1);   // push cards to top

        sa->setWidget(outContent);
        sa->verticalScrollBar()->setSingleStep(25);
        return sa;
    }

} // anonymous namespace

// ══════════════════════════════════════════════════════════════════════════════
//  SidebarButton
// ══════════════════════════════════════════════════════════════════════════════
SidebarButton::SidebarButton(const QString& icon, const QString& label, QWidget* parent)
    : QPushButton(parent)
{
    setObjectName("sidebarBtn");
    setFixedHeight(44);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setText(icon + "   " + label);
    setCursor(Qt::PointingHandCursor);
    setProperty("active", false);
    setStyleSheet("text-align: left; padding-left: 14px;");
}

void SidebarButton::setActive(bool active) {
    setProperty("active", active);
    if (style()) { style()->unpolish(this); style()->polish(this); }
    update();
}

// ══════════════════════════════════════════════════════════════════════════════
//  PostCard
// ══════════════════════════════════════════════════════════════════════════════
PostCard::PostCard(Posts* post, const QString& authorUsername,
    bool isOwner, bool isSaved,
    const QString& viewerUsername,
    bool isAuthorReported, bool isPostReported,
    QWidget* parent)
    : QFrame(parent)
    , m_post(post)
    , m_authorUsername(authorUsername)
    , m_isOwner(isOwner)
    , m_isSaved(isSaved)
    , m_viewerUsername(viewerUsername)
    , m_isAuthorReported(isAuthorReported)
    , m_isPostReported(isPostReported)
{
    setObjectName("postCard");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(12);

    // ── Header row ──────────────────────────────────────────────────────────
    auto* headerRow = new QHBoxLayout;
    headerRow->setSpacing(12);
    headerRow->setContentsMargins(0, 0, 0, 0);

    QString ini = m_authorUsername.isEmpty() ? "?" : m_authorUsername.left(1).toUpper();
    headerRow->addWidget(avatar(ini, 40), 0, Qt::AlignVCenter);

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
            "QPushButton:hover{background:#1E1E1E;color:#AAAAFF;}");
        connect(editBtn, &QPushButton::clicked, this,
            [this]() { emit editClicked(m_post); });
        headerRow->addWidget(editBtn, 0, Qt::AlignVCenter);

        auto* delBtn = new QPushButton("🗑️");
        delBtn->setFixedSize(32, 32);
        delBtn->setCursor(Qt::PointingHandCursor);
        delBtn->setStyleSheet(
            "QPushButton{background:transparent;border:none;color:#666688;font-size:18px;border-radius:6px;}"
            "QPushButton:hover{background:rgba(255,68,68,0.1);color:#FF4444;}");
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

    // ── Action row ──────────────────────────────────────────────────────────
    auto* actRow = new QHBoxLayout;
    actRow->setContentsMargins(0, 10, 0, 0);
    actRow->setSpacing(20);

    // Like button
    bool alreadyLiked = !m_isOwner && m_post->isLikedBy(m_viewerUsername.toStdString());
    auto* likeBtn = new QPushButton(alreadyLiked ? "♥" : "♡");
    likeBtn->setFixedSize(36, 36);
    likeBtn->setCursor(m_isOwner ? Qt::ArrowCursor : Qt::PointingHandCursor);
    likeBtn->setEnabled(!m_isOwner);
    likeBtn->setStyleSheet(alreadyLiked
        ? "QPushButton{background:transparent;border:none;color:#FF4444;font-size:22px;}"
        : "QPushButton{background:transparent;border:none;color:#A0A0C0;font-size:22px;}"
        "QPushButton:hover{color:#FF5555;}");

    auto* likeCountLbl = new QLabel(QString::number(m_post->getLikeCount()));
    likeCountLbl->setStyleSheet(alreadyLiked
        ? "color:#FF4444;font-size:14px;font-weight:bold;"
        : "color:#A0A0C0;font-size:14px;font-weight:bold;");

    connect(likeBtn, &QPushButton::clicked, this, [this, likeBtn, likeCountLbl]() {
        if (m_isOwner) return;
        m_post->likePost(m_viewerUsername.toStdString());
        bool nowLiked = m_post->isLikedBy(m_viewerUsername.toStdString());
        likeCountLbl->setText(QString::number(m_post->getLikeCount()));
        if (nowLiked) {
            likeBtn->setText("♥");
            likeBtn->setStyleSheet("QPushButton{background:transparent;border:none;color:#FF4444;font-size:22px;}");
            likeCountLbl->setStyleSheet("color:#FF4444;font-size:14px;font-weight:bold;");
        }
        else {
            likeBtn->setText("♡");
            likeBtn->setStyleSheet("QPushButton{background:transparent;border:none;color:#A0A0C0;font-size:22px;}"
                "QPushButton:hover{color:#FF5555;}");
            likeCountLbl->setStyleSheet("color:#A0A0C0;font-size:14px;font-weight:bold;");
        }
        emit likeClicked(m_post);
        });

    auto* likeLayout = new QHBoxLayout;
    likeLayout->setSpacing(4);
    likeLayout->addWidget(likeBtn);
    likeLayout->addWidget(likeCountLbl);
    actRow->addLayout(likeLayout);

    // Comment button
    // Comment button — hidden for owner's own posts
    auto* commentBtn = new QPushButton("💬");
    commentBtn->setFixedSize(36, 36);
    commentBtn->setCursor(Qt::PointingHandCursor);
    commentBtn->setStyleSheet(
        "QPushButton{background:transparent;border:none;color:#A0A0C0;font-size:20px;}"
        "QPushButton:hover{color:#7070FF;}");
    if (m_isOwner) {
        commentBtn->setEnabled(false);
        commentBtn->setToolTip("You cannot comment on your own posts.");
        commentBtn->setStyleSheet(
            "QPushButton{background:transparent;border:none;color:#3A3A5A;font-size:20px;}");
    }
    else {
        connect(commentBtn, &QPushButton::clicked, this,
            [this]() { emit commentClicked(m_post); });
    }

    auto* commentCountLbl = new QLabel(QString::number(m_post->getCommentsCount()));
    commentCountLbl->setStyleSheet("color:#A0A0C0;font-size:14px;font-weight:bold;");

    auto* commentLayout = new QHBoxLayout;
    commentLayout->setSpacing(4);
    commentLayout->addWidget(commentBtn);
    commentLayout->addWidget(commentCountLbl);
    actRow->addLayout(commentLayout);

    QPushButton* saveBtn = nullptr;

    if (!m_isOwner) {
        // ── Save button ──────────────────────────────────────────────────────
        auto* saveCol = new QVBoxLayout;
        saveCol->setSpacing(2);
        saveCol->setAlignment(Qt::AlignCenter);

        saveBtn = new QPushButton(m_isSaved ? "★" : "☆");
        saveBtn->setCursor(Qt::PointingHandCursor);
        saveBtn->setFixedSize(24, 24);

        QLabel* saveLbl = new QLabel(m_isSaved ? "Saved" : "Save");
        saveLbl->setAlignment(Qt::AlignCenter);

        if (m_isSaved) {
            saveBtn->setStyleSheet("background:#0F1F10;border:none;color:#44CC55;font-size:15px;border-radius:7px;");
            saveLbl->setStyleSheet("color:#44CC55;font-size:11px;");
        }
        else {
            saveBtn->setStyleSheet("background:transparent;border:none;color:#44664A;font-size:15px;border-radius:7px;");
            saveLbl->setStyleSheet("color:#F5A623;font-size:11px;");
        }

        saveCol->addWidget(saveBtn, 0, Qt::AlignCenter);
        saveCol->addWidget(saveLbl, 0, Qt::AlignCenter);

        connect(saveBtn, &QPushButton::clicked, this, [this, saveBtn, saveLbl]() {
            if (m_isSaved) {
                emit unsaveClicked(m_post);
                m_isSaved = false;
                saveBtn->setText("☆");
                saveBtn->setStyleSheet("background:transparent;border:none;color:#44664A;font-size:15px;border-radius:7px;");
                saveLbl->setText("Save");
                saveLbl->setStyleSheet("color:#F5A623;font-size:11px;");
            }
            else {
                emit saveClicked(m_post, m_authorUsername);
                m_isSaved = true;
                saveBtn->setText("★");
                saveBtn->setStyleSheet("background:#0F1F10;border:none;color:#44CC55;font-size:15px;border-radius:7px;");
                saveLbl->setText("Saved");
                saveLbl->setStyleSheet("color:#44CC55;font-size:11px;");
            }
            });

        actRow->addLayout(saveCol);

        // ── Report post button ───────────────────────────────────────────────
        auto* reportCol = new QVBoxLayout;
        reportCol->setSpacing(2);
        reportCol->setAlignment(Qt::AlignCenter);

        QPushButton* reportBtn = new QPushButton("⚑");
        reportBtn->setCursor(Qt::PointingHandCursor);
        reportBtn->setFixedSize(24, 24);

        QLabel* reportLbl = new QLabel(m_isPostReported ? "Reported" : "Report");
        reportLbl->setAlignment(Qt::AlignCenter);

        if (m_isPostReported) {
            reportBtn->setStyleSheet("background:#1C0808;border:none;color:#CC4444;font-size:15px;border-radius:7px;");
            reportLbl->setStyleSheet("color:#CC4444;font-size:11px;");
        }
        else {
            reportBtn->setStyleSheet("background:transparent;border:none;color:#664444;font-size:15px;border-radius:7px;");
            reportLbl->setStyleSheet("color:#F5A623;font-size:11px;");
        }

        reportCol->addWidget(reportBtn, 0, Qt::AlignCenter);
        reportCol->addWidget(reportLbl, 0, Qt::AlignCenter);

        connect(reportBtn, &QPushButton::clicked, this, [this, reportBtn, reportLbl]() {
            if (m_isPostReported) {
                QMessageBox::information(this, "Already Reported", "You have already reported this post.");
                return;
            }
            auto r = QMessageBox::question(this, "Report Post",
                "Are you sure you want to report this post?",
                QMessageBox::Yes | QMessageBox::No);
            if (r == QMessageBox::Yes) {
                emit reportClicked(m_post, m_authorUsername);
                m_isPostReported = true;
                reportBtn->setStyleSheet("background:#1C0808;border:none;color:#CC4444;font-size:15px;border-radius:7px;");
                reportLbl->setStyleSheet("color:#CC4444;font-size:11px;");
                reportLbl->setText("Reported");
            }
            });
        actRow->addLayout(reportCol);

        // ── Report user button ───────────────────────────────────────────────
        auto* reportUserCol = new QVBoxLayout;
        reportUserCol->setSpacing(2);
        reportUserCol->setAlignment(Qt::AlignCenter);

        QPushButton* reportUserBtn = new QPushButton("🚫");
        reportUserBtn->setCursor(Qt::PointingHandCursor);
        reportUserBtn->setFixedSize(24, 24);

        QLabel* reportUserLbl = new QLabel(m_isAuthorReported ? "Reported User" : "Report User");
        reportUserLbl->setAlignment(Qt::AlignCenter);

        if (m_isAuthorReported) {
            reportUserBtn->setStyleSheet("background:#1C0808;border:none;color:#CC4444;font-size:15px;border-radius:7px;");
            reportUserLbl->setStyleSheet("color:#CC4444;font-size:11px;");
        }
        else {
            reportUserBtn->setStyleSheet("background:transparent;border:none;color:#664444;font-size:15px;border-radius:7px;");
            reportUserLbl->setStyleSheet("color:#F5A623;font-size:11px;");
        }

        reportUserCol->addWidget(reportUserBtn, 0, Qt::AlignCenter);
        reportUserCol->addWidget(reportUserLbl, 0, Qt::AlignCenter);

        connect(reportUserBtn, &QPushButton::clicked, this, [this, reportUserBtn, reportUserLbl]() {
            if (m_isAuthorReported) {
                QMessageBox::information(this, "Already Reported", "You have already reported this user.");
                return;
            }
            auto r = QMessageBox::question(this, "Report User",
                "Report user @" + m_authorUsername + "?",
                QMessageBox::Yes | QMessageBox::No);
            if (r == QMessageBox::Yes) {
                emit reportUserClicked(m_authorUsername);
                m_isAuthorReported = true;
                reportUserBtn->setStyleSheet("background:#1C0808;border:none;color:#CC4444;font-size:15px;border-radius:7px;");
                reportUserLbl->setStyleSheet("color:#CC4444;font-size:11px;");
                reportUserLbl->setText("Reported User");
            }
            });
        actRow->addLayout(reportUserCol);
    }

    root->addLayout(actRow);
}

// ══════════════════════════════════════════════════════════════════════════════
//  CommentsPage
// ══════════════════════════════════════════════════════════════════════════════
CommentsPage::CommentsPage(const QString& currentUser, QWidget* parent)
    : QWidget(parent), m_currentUser(currentUser), m_post(nullptr)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(16);

    auto* headerRow = new QHBoxLayout;
    auto* backBtn = new QPushButton("← Back");
    backBtn->setObjectName("secondaryBtn");
    backBtn->setFixedWidth(100);
    backBtn->setFixedHeight(36);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        m_post = nullptr;
        emit backClicked();
        });

    auto* title = new QLabel("Comments");
    title->setObjectName("pageTitle");

    headerRow->addWidget(backBtn);
    headerRow->addSpacing(16);
    headerRow->addWidget(title);
    headerRow->addStretch(1);
    root->addLayout(headerRow);

    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background: transparent; border: none;");

    m_commentsContainer = new QWidget;
    m_commentsLayout = new QVBoxLayout(m_commentsContainer);
    m_commentsLayout->setContentsMargins(0, 0, 0, 0);
    m_commentsLayout->setSpacing(12);
    m_commentsLayout->addStretch(1);

    scroll->setWidget(m_commentsContainer);
    root->addWidget(scroll, 1);

    auto* inputRow = new QHBoxLayout;
    m_input = new QLineEdit;
    m_input->setPlaceholderText("Write a comment...");
    m_input->setFixedHeight(44);

    auto* sendBtn = new QPushButton("Post");
    sendBtn->setObjectName("primaryBtn");
    sendBtn->setFixedSize(80, 44);
    sendBtn->setCursor(Qt::PointingHandCursor);

    connect(sendBtn, &QPushButton::clicked, this, &CommentsPage::onAddComment);
    connect(m_input, &QLineEdit::returnPressed, this, &CommentsPage::onAddComment);

    inputRow->addWidget(m_input, 1);
    inputRow->addWidget(sendBtn);
    root->addLayout(inputRow);
}

void CommentsPage::loadPost(Posts* post) {
    m_post = post;
    m_input->clear();
    loadComments();
}

void CommentsPage::loadComments() {
    // Remove everything except the trailing stretch
    while (m_commentsLayout->count() > 1) {
        QLayoutItem* item = m_commentsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (!m_post) return;

    // Use pure C++ array from backend
    Comment* comments = m_post->getComments();
    int      count = m_post->getCommentsCount();

    if (count == 0) {
        auto* empty = new QLabel("No comments yet. Be the first!");
        empty->setStyleSheet("color: #56567A; font-size: 13px;");
        empty->setAlignment(Qt::AlignCenter);
        m_commentsLayout->insertWidget(0, empty);
        return;
    }

    for (int i = 0; i < count; ++i) {
        const Comment& c = comments[i];
        QString creatorName = QString::fromStdString(c.getCreatorUsername());
        QString creationTime = QString::fromStdString(c.getTimeOfCreation());
        QString contentText = QString::fromStdString(c.getContent());
        QString commentId = QString::fromStdString(c.getCommentId());
        bool isMine = (creatorName == m_currentUser);

        auto* card = new QFrame;
        card->setStyleSheet("QFrame{background:#141420;border-radius:8px;padding:10px;}");
        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(10, 8, 10, 8);
        cl->setSpacing(4);

        auto* hr = new QHBoxLayout;
        auto* nameLbl = new QLabel(creatorName);
        nameLbl->setStyleSheet("font-weight:bold;color:#A0A0D0;font-size:12px;");
        auto* timeLbl = new QLabel(creationTime);
        timeLbl->setStyleSheet("color:#56567A;font-size:10px;");
        hr->addWidget(nameLbl);
        hr->addWidget(timeLbl);
        hr->addStretch(1);

        if (isMine) {
            auto* editBtn = new QPushButton("✎");
            editBtn->setFixedSize(24, 24);
            editBtn->setCursor(Qt::PointingHandCursor);
            editBtn->setStyleSheet(
                "QPushButton{background:transparent;border:none;color:#8888AA;}"
                "QPushButton:hover{color:#AAAAFF;}");

            auto* delBtn = new QPushButton("🗑");
            delBtn->setFixedSize(24, 24);
            delBtn->setCursor(Qt::PointingHandCursor);
            delBtn->setStyleSheet(
                "QPushButton{background:transparent;border:none;color:#8888AA;}"
                "QPushButton:hover{color:#FFAAAA;}");

            connect(editBtn, &QPushButton::clicked, this, [this, commentId]() {
                onEditComment(commentId);
                });
            connect(delBtn, &QPushButton::clicked, this, [this, commentId]() {
                onDeleteComment(commentId);
                });

            hr->addWidget(editBtn);
            hr->addWidget(delBtn);
        }

        cl->addLayout(hr);
        auto* contentLbl = new QLabel(contentText);
        contentLbl->setWordWrap(true);
        contentLbl->setStyleSheet("color:#E2E2EC;font-size:13px;");
        cl->addWidget(contentLbl);

        m_commentsLayout->insertWidget(m_commentsLayout->count() - 1, card);
    }
}

void CommentsPage::onAddComment() {
    if (!m_post) return;
    QString text = m_input->text().trimmed();
    if (text.isEmpty()) return;
    if (text.contains('|')) {
        QMessageBox::warning(this, "Novo", "Comments cannot contain '|'.");
        return;
    }
    m_post->addComment(text.toStdString(), m_currentUser.toStdString());
    m_input->clear();
    loadComments();
}

void CommentsPage::onEditComment(const QString& commentId) {
    if (!m_post) return;

    // Resolve ID → current index at call time (safe against concurrent mutations)
    Comment* comments = m_post->getComments();
    int      count = m_post->getCommentsCount();
    int      index = -1;
    for (int i = 0; i < count; ++i) {
        if (QString::fromStdString(comments[i].getCommentId()) == commentId) {
            index = i; break;
        }
    }
    if (index == -1) return;

    QString oldText = QString::fromStdString(comments[index].getContent());
    bool ok;
    QString newText = QInputDialog::getText(this, "Edit Comment",
        "Update your comment:", QLineEdit::Normal, oldText, &ok);

    if (ok && !newText.trimmed().isEmpty()) {
        if (newText.contains('|')) {
            QMessageBox::warning(this, "Novo", "Comments cannot contain '|'.");
            return;
        }
        m_post->editComment(index, newText.trimmed().toStdString(), m_currentUser.toStdString());
        loadComments();
    }
}

void CommentsPage::onDeleteComment(const QString& commentId) {
    if (!m_post) return;

    Comment* comments = m_post->getComments();
    int      count = m_post->getCommentsCount();
    int      index = -1;
    for (int i = 0; i < count; ++i) {
        if (QString::fromStdString(comments[i].getCommentId()) == commentId) {
            index = i; break;
        }
    }
    if (index == -1) return;

    auto r = QMessageBox::question(this, "Delete Comment",
        "Delete this comment?", QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        m_post->deleteComment(index, m_currentUser.toStdString());
        loadComments();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  PublicProfileWidget
// ══════════════════════════════════════════════════════════════════════════════
PublicProfileWidget::PublicProfileWidget(QWidget* parent)
    : QWidget(parent), m_allUsers(nullptr), m_userCount(0)
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
}

void PublicProfileWidget::loadProfile(User* targetUser, User* viewer,
    User** allUsers, int userCount)
{
    m_allUsers = allUsers;
    m_userCount = userCount;

    // Clear previous content
    QLayoutItem* item;
    while ((item = m_mainLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // Back button row
    auto* topRow = new QHBoxLayout;
    topRow->setContentsMargins(28, 24, 28, 0);
    auto* backBtn = new QPushButton("← Back to Search");
    backBtn->setObjectName("secondaryBtn");
    backBtn->setFixedWidth(150);
    backBtn->setFixedHeight(34);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, &PublicProfileWidget::backClicked);
    topRow->addWidget(backBtn);
    topRow->addStretch(1);
    auto* headerWrapper = new QWidget;
    headerWrapper->setLayout(topRow);
    m_mainLayout->addWidget(headerWrapper);

    // Scrollable content
    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background: transparent; border: none;");

    auto* container = new QWidget;
    auto* layout = new QVBoxLayout(container);
    layout->setContentsMargins(28, 16, 28, 24);
    layout->setSpacing(20);

    // Profile header card
    auto* headerCard = new QFrame;
    headerCard->setObjectName("postCard");
    auto* hLayout = new QVBoxLayout(headerCard);

    auto* row = new QHBoxLayout;
    QString ini = QString::fromStdString(targetUser->getUsername()).left(1).toUpper();
    auto* av = new QLabel(ini);
    av->setFixedSize(60, 60);
    av->setAlignment(Qt::AlignCenter);
    av->setStyleSheet(
        "background:#202040;border-radius:30px;"
        "font-size:24px;font-weight:bold;color:#7070FF;");
    row->addWidget(av);

    auto* nameCol = new QVBoxLayout;
    auto* nameLbl = new QLabel(QString::fromStdString(targetUser->getUsername()));
    nameLbl->setStyleSheet("font-size:20px;font-weight:bold;color:#E0E0FF;");
    auto* bioLbl = new QLabel(QString::fromStdString(targetUser->getBio()));
    bioLbl->setStyleSheet("font-size:13px;color:#56567A;");
    bioLbl->setWordWrap(true);
    nameCol->addWidget(nameLbl);
    nameCol->addWidget(bioLbl);
    row->addLayout(nameCol, 1);
    hLayout->addLayout(row);

    auto* statsRow = new QHBoxLayout;
    auto addStat = [&](const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout;
        auto* v = new QLabel(val);
        v->setStyleSheet("font-size:18px;font-weight:bold;color:#6060FF;");
        v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size:10px;color:#36365A;letter-spacing:1px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l);
        statsRow->addLayout(c);
        };
    addStat(QString::number(targetUser->getPostCount()), "POSTS");
    addStat(QString::number(targetUser->getFollowersCount()), "FOLLOWERS");
    addStat(QString::number(targetUser->getFollowingCount()), "FOLLOWING");
    hLayout->addLayout(statsRow);
    layout->addWidget(headerCard);

    // Posts
    auto* postsTitle = new QLabel("Recent Posts");
    postsTitle->setStyleSheet("font-size:16px;font-weight:bold;color:#E8E8F8;");
    layout->addWidget(postsTitle);

    targetUser->loadAllPosts();
    viewer->loadSavedPosts(m_allUsers, m_userCount);

    if (targetUser->getPostCount() == 0) {
        auto* empty = new QLabel("This user hasn't posted anything yet.");
        empty->setStyleSheet("color:#56567A;font-size:13px;");
        empty->setAlignment(Qt::AlignCenter);
        layout->addWidget(empty);
    }
    else {
        for (int i = targetUser->getPostCount() - 1; i >= 0; --i) {
            Posts* p = targetUser->getPostByIndex(i);
            if (!p || !p->isValid()) continue;

            bool isSaved = viewer->hasSavedPost(p->getPostId());
            bool isAuthorReported = targetUser->hasReportedUser(viewer->getUsername());
            bool isPostReported = p->hasReportedBy(viewer->getUsername());

            auto* card = new PostCard(p,
                QString::fromStdString(targetUser->getUsername()),
                false, isSaved,
                QString::fromStdString(viewer->getUsername()),
                isAuthorReported,
                isPostReported);

            connect(card, &PostCard::commentClicked, this, [p, this]() {
                emit requestOpenComments(p);
                });
            connect(card, &PostCard::saveClicked, this, [viewer, targetUser](Posts* post) {
                if (viewer && targetUser && post)
                    viewer->savePost(post->getPostId(), targetUser);
                });
            connect(card, &PostCard::unsaveClicked, this, [viewer](Posts* post) {
                if (viewer && post)
                    viewer->unsavePost(post->getPostId());
                });
            connect(card, &PostCard::likeClicked, this, [](Posts*) {});
            connect(card, &PostCard::reportUserClicked, this,
                [this, viewer, targetUser](const QString&) {
                    if (targetUser->hasReportedUser(viewer->getUsername())) {
                        QMessageBox::information(this, "Already Reported",
                            "You have already reported this user.");
                        return;
                    }
                    targetUser->reportUserBy(viewer->getUsername());
                    QMessageBox::information(this, "Reported", "User has been reported.");
                });
            connect(card, &PostCard::reportClicked, this,
                [this, viewer](Posts* post, const QString&) {
                    if (!post || !viewer) return;
                    if (post->hasReportedBy(viewer->getUsername())) {
                        QMessageBox::information(this, "Already Reported",
                            "You have already reported this post.");
                        return;
                    }
                    post->reportPost(viewer->getUsername());
                    QMessageBox::information(this, "Reported", "Post has been reported to admins.");
                });
            layout->addWidget(card);
        }
    }

    layout->addStretch(1);
    scroll->setWidget(container);
    m_mainLayout->addWidget(scroll, 1);
}

// ══════════════════════════════════════════════════════════════════════════════
//  NotificationItem
// ══════════════════════════════════════════════════════════════════════════════
NotificationItem::NotificationItem(const Notification& notif, QWidget* parent)
    : QFrame(parent)
{
    setObjectName("notifItem");
    setFixedHeight(64);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* row = new QHBoxLayout(this);
    row->setContentsMargins(16, 0, 16, 0);
    row->setSpacing(12);
    row->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QString typeIcon = "●";
    QString t = QString::fromStdString(notif.getType()).toLower();
    if (t.contains("follow"))  typeIcon = "👤";
    else if (t.contains("like"))    typeIcon = "♥";
    else if (t.contains("comment")) typeIcon = "✦";

    auto* ic = new QLabel(typeIcon);
    ic->setFixedSize(32, 32);
    ic->setAlignment(Qt::AlignCenter);
    ic->setStyleSheet("font-size:16px;");

    auto* col = new QVBoxLayout;
    col->setSpacing(3);
    auto* msg = makeLabel(QString::fromStdString(notif.getMessage()));
    msg->setStyleSheet("font-size:13px;color:#C0C0DC;");
    auto* ts = makeLabel(QString::fromStdString(notif.getTimestamp()), "postTime");
    col->addWidget(msg);
    col->addWidget(ts);

    row->addWidget(ic, 0, Qt::AlignVCenter);
    row->addLayout(col, 1);
}

// ══════════════════════════════════════════════════════════════════════════════
//  AuthPage
// ══════════════════════════════════════════════════════════════════════════════
AuthPage::AuthPage(QWidget* parent)
    : QWidget(parent), m_userCount(0), m_adminUser(nullptr), m_adminPass(nullptr)
{
    setObjectName("authBg");

    // Count lines in users_list.txt to size the array
    int count = 0;
    {
        ifstream f("data/users_list.txt");
        string line;
        while (getline(f, line))
            if (!line.empty() && line != "\r") count++;
    }
    m_allUsers = new User * [qMax(count, 1)]();

    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);
    outer->setContentsMargins(20, 20, 20, 20);

    auto* card = new QFrame;
    card->setObjectName("authCard");
    card->setFixedWidth(400);
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
    m_stack->addWidget(createLandingWidget());    // 0
    m_stack->addWidget(createLoginWidget());      // 1
    m_stack->addWidget(createSignupWidget());     // 2
    m_stack->addWidget(createAdminLoginWidget()); // 3
    cl->addWidget(m_stack);

    outer->addWidget(card, 0, Qt::AlignCenter);

    loadAllUsers(m_allUsers, m_userCount);
}

AuthPage::~AuthPage() {
    delete[] m_allUsers;
}

void AuthPage::resetToLogin() {
    m_loginUser->clear();  m_loginPass->clear();
    m_signupUser->clear(); m_signupPass->clear(); m_signupBio->clear();
    if (m_adminUser) m_adminUser->clear();
    if (m_adminPass) m_adminPass->clear();
    m_stack->setCurrentIndex(0);

    int count = 0;
    {
        ifstream f("data/users_list.txt");
        string line;
        while (getline(f, line))
            if (!line.empty() && line != "\r") count++;
    }
    m_allUsers = new User * [qMax(count, 1)]();
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

    m_adminUser = new QLineEdit;
    m_adminUser->setPlaceholderText("Admin Username");
    m_adminUser->setFixedHeight(42);

    m_adminPass = new QLineEdit;
    m_adminPass->setPlaceholderText("Admin Password");
    m_adminPass->setFixedHeight(42);
    m_adminPass->setEchoMode(QLineEdit::Password);

    vl->addWidget(m_adminUser);
    vl->addWidget(m_adminPass);

    auto* loginBtn = makeDanger("Enter System");
    loginBtn->setFixedHeight(44);

    connect(loginBtn, &QPushButton::clicked, this, [this]() {
        QFile file("data/Admin/admin_credentials.txt");
        bool authenticated = false;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString line = in.readLine();
            if (!line.isEmpty()) {
                QStringList parts = line.split("|");
                if (parts.size() >= 2 &&
                    m_adminUser->text() == parts[0] &&
                    m_adminPass->text() == parts[1])
                    authenticated = true;
            }
            file.close();
        }
        if (authenticated) {
            m_adminUser->clear();
            m_adminPass->clear();
            emit loginAdminSuccess();
        }
        else {
            QMessageBox::critical(this, "Novo", "Invalid Admin Credentials.");
            m_adminPass->clear();
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
    User* user = findAndLogin(m_allUsers, m_userCount,
        uname.toStdString(), pass.toStdString());
    if (user) {
        user->loadFollowing(m_allUsers, m_userCount);
        user->loadFollowers(m_allUsers, m_userCount);
        user->loadSavedPosts(m_allUsers, m_userCount);
        m_loginUser->clear();
        m_loginPass->clear();

        User** arr = m_allUsers;
        int    cnt = m_userCount;
        m_allUsers = nullptr;
        m_userCount = 0;
        emit loginSuccess(user, arr, cnt);
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

    string validationErr = checker.validatePassword(pass.toStdString());
    if (!validationErr.empty()) {
        QString feedback = QString::fromStdString(checker.getValidationFeedback(pass.toStdString()));
        QMessageBox::warning(this, "Weak Password",
            QString::fromStdString(validationErr) + "\n\n" + feedback);
        return;
    }

    int strength = checker.calculateStrength(pass.toStdString());
    if (strength < 80) {
        QString feedback = QString::fromStdString(checker.getValidationFeedback(pass.toStdString()));
        auto r = QMessageBox::warning(this, "Weak Password",
            "Password strength: " + QString::number(strength) + "/100\n\n"
            + feedback + "\nContinue anyway?",
            QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::No) return;
    }

    string usernameErr = User::validateUsername(uname.toStdString());
    if (!usernameErr.empty()) {
        QMessageBox::warning(this, "Invalid Username", QString::fromStdString(usernameErr));
        return;
    }

    string bioErr = User::validateBio(bio.toStdString());
    if (!bioErr.empty()) {
        QMessageBox::warning(this, "Invalid Bio", QString::fromStdString(bioErr));
        return;
    }

    User* newUser = signUp(m_allUsers, m_userCount,
        uname.toStdString(), pass.toStdString(), bio.toStdString());
    if (newUser) {
        QMessageBox::information(this, APP_NAME, "Account created! You can now sign in.");
        m_signupUser->clear();
        m_signupPass->clear();
        m_signupBio->clear();
        m_stack->setCurrentIndex(1);
    }
    else {
        QMessageBox::critical(this, APP_NAME, "Username already taken or invalid.");
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  FeedPage
// ══════════════════════════════════════════════════════════════════════════════
FeedPage::FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    // Header
    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Feed", "pageTitle"));
    hRow->addStretch(1);
    outer->addLayout(hRow);

    // Composer card (hidden until the + button is pressed)
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
        QString ini = currentUser
            ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper()
            : "?";
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

    m_scrollArea = makeScrollArea(m_feedContent, m_feedLayout);
    outer->addWidget(m_scrollArea, 1);

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

    m_user->loadFollowing(m_allUsers, m_userCount);
    for (int u = 0; u < m_userCount; ++u) {
        if (m_allUsers[u] && m_allUsers[u] != m_user &&
            m_user->isFollowing(m_allUsers[u]->getUsername()))
            m_allUsers[u]->loadAllPosts();
    }
    m_user->loadSavedPosts(m_allUsers, m_userCount);

    bool any = false;
    for (int u = 0; u < m_userCount; ++u) {
        if (!m_allUsers[u] || m_allUsers[u] == m_user) continue;
        if (!m_user->isFollowing(m_allUsers[u]->getUsername())) continue;

        for (int i = m_allUsers[u]->getPostCount() - 1; i >= 0; --i) {
            Posts* p = m_allUsers[u]->getPostByIndex(i);
            if (!p || !p->isValid()) continue;

            bool saved = m_user->hasSavedPost(p->getPostId());
            bool reportedByMe = m_allUsers[u]->hasReportedUser(m_user->getUsername());
            bool postReportedByMe = p->hasReportedBy(m_user->getUsername());

            auto* card = new PostCard(p,
                QString::fromStdString(m_allUsers[u]->getUsername()),
                false, saved,
                QString::fromStdString(m_user->getUsername()),
                reportedByMe,
                postReportedByMe);

            connect(card, &PostCard::likeClicked, this, &FeedPage::onLikePost);
            connect(card, &PostCard::commentClicked, this, &FeedPage::onCommentPost);
            connect(card, &PostCard::deleteClicked, this, &FeedPage::onDeletePost);
            connect(card, &PostCard::saveClicked, this, &FeedPage::onSavePost);
            connect(card, &PostCard::unsaveClicked, this, &FeedPage::onUnsavePost);
            connect(card, &PostCard::reportClicked, this, &FeedPage::onReportPost);
            connect(card, &PostCard::reportUserClicked, this, &FeedPage::onReportUserFromPost);
            m_feedLayout->insertWidget(m_feedLayout->count() - 1, card);
            any = true;
        }
    }

    if (!any) {
        auto* empty = new QLabel("Follow someone to see their posts here!");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A;font-size:14px;padding:40px 0;");
        m_feedLayout->insertWidget(0, empty);
    }
}

void FeedPage::onLikePost(Posts*) {}   // like is handled inline inside PostCard

void FeedPage::onCommentPost(Posts* post) {
    if (!post || !m_user) return;
    emit requestOpenComments(post);
}

void FeedPage::onDeletePost(Posts* post) {
    if (!post || !m_user) return;
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
    if (!post || !m_user || ownerUsername.isEmpty()) return;
    User* owner = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] &&
            m_allUsers[i]->getUsername() == ownerUsername.toStdString()) {
            owner = m_allUsers[i]; break;
        }
    }
    if (owner) {
        m_user->savePost(post->getPostId(), owner);
        m_user->saveSavedPostsToFile();
    }
}

void FeedPage::onUnsavePost(Posts* post) {
    if (!post || !m_user) return;
    m_user->unsavePost(post->getPostId());
    m_user->saveSavedPostsToFile();
}

void FeedPage::onReportPost(Posts* post, const QString& ownerUsername) {
    if (!post || !m_user) return;
    User* owner = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] &&
            m_allUsers[i]->getUsername() == ownerUsername.toStdString()) {
            owner = m_allUsers[i]; break;
        }
    }
    if (!owner) return;
    m_user->reportPost(post->getPostId(), owner);
    QMessageBox::information(this, APP_NAME, "Post has been reported.");
}

void FeedPage::onReportUserFromPost(const QString& username) {
    if (!m_user) return;
    User* target = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] &&
            m_allUsers[i]->getUsername() == username.toStdString()) {
            target = m_allUsers[i]; break;
        }
    }
    if (!target) return;
    if (target->hasReportedUser(m_user->getUsername())) {
        QMessageBox::information(this, "Already Reported", "You have already reported this user.");
        return;
    }
    target->reportUserBy(m_user->getUsername());
    QMessageBox::information(this, "Reported", "User @" + username + " has been reported.");
}

void FeedPage::onSubmitPost() {
    if (!m_user) return;
    QString text = m_postInput->toPlainText().trimmed();
    if (text.isEmpty()) return;
    if (text.contains('|')) {
        QMessageBox::warning(this, APP_NAME, "Posts cannot contain '|'.");
        return;
    }
    m_user->createPost(text.toStdString());
    m_postInput->clear();
    m_composerCard->hide();
    loadPosts();
}

// ══════════════════════════════════════════════════════════════════════════════
//  CreatePostPage  — standalone page for composing a new post
// ══════════════════════════════════════════════════════════════════════════════
CreatePostPage::CreatePostPage(User* currentUser, QWidget* parent)
    : QWidget(parent), m_user(currentUser)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(20);

    // Header
    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Create Post", "pageTitle"));
    hRow->addStretch(1);
    outer->addLayout(hRow);

    // Composer card
    auto* card = new QFrame;
    card->setObjectName("postCard");
    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(20, 18, 20, 18);
    cl->setSpacing(14);

    // Avatar + prompt row
    auto* topRow = new QHBoxLayout;
    topRow->setSpacing(12);
    QString ini = currentUser
        ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper()
        : "?";
    topRow->addWidget(avatar(ini, 36), 0, Qt::AlignVCenter);
    auto* prompt = new QLabel("What's on your mind?");
    prompt->setStyleSheet("font-size:14px;color:#8080CC;");
    topRow->addWidget(prompt, 1);
    cl->addLayout(topRow);

    m_postInput = new QTextEdit;
    m_postInput->setPlaceholderText("Share something with your followers...");
    m_postInput->setMinimumHeight(140);
    m_postInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cl->addWidget(m_postInput);

    // Character count
    m_charCount = new QLabel("0 / 500");
    m_charCount->setStyleSheet("color:#44446A;font-size:11px;");
    m_charCount->setAlignment(Qt::AlignRight);
    cl->addWidget(m_charCount);

    connect(m_postInput, &QTextEdit::textChanged, this, [this]() {
        int len = m_postInput->toPlainText().length();
        m_charCount->setText(QString::number(len) + " / 500");
        m_charCount->setStyleSheet(len > 500
            ? "color:#CC4444;font-size:11px;"
            : "color:#44446A;font-size:11px;");
        });

    auto* btnRow = new QHBoxLayout;
    btnRow->addStretch(1);

    auto* clearBtn = makeSecondary("Clear");
    clearBtn->setFixedWidth(90);
    clearBtn->setFixedHeight(38);
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        m_postInput->clear();
        });

    auto* postBtn = makePrimary("Post ✓");
    postBtn->setFixedWidth(110);
    postBtn->setFixedHeight(38);
    connect(postBtn, &QPushButton::clicked, this, &CreatePostPage::onSubmit);

    btnRow->addWidget(clearBtn);
    btnRow->addSpacing(8);
    btnRow->addWidget(postBtn);
    cl->addLayout(btnRow);

    outer->addWidget(card);
    outer->addStretch(1);
}

void CreatePostPage::onSubmit() {
    if (!m_user) return;
    QString text = m_postInput->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, APP_NAME, "Post cannot be empty.");
        return;
    }
    if (text.length() > 500) {
        QMessageBox::warning(this, APP_NAME, "Post cannot exceed 500 characters.");
        return;
    }
    if (text.contains('|')) {
        QMessageBox::warning(this, APP_NAME, "Posts cannot contain '|'.");
        return;
    }
    m_user->createPost(text.toStdString());
    m_postInput->clear();
    QMessageBox::information(this, APP_NAME, "Post published!");
    emit postPublished();
}

void CreatePostPage::focusInput() {
    if (m_postInput) {
        m_postInput->setFocus();
        m_postInput->clear();
    }
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
        empty->setStyleSheet("color:#28284A;font-size:13px;padding:30px 0;");
        m_postsLayout->insertWidget(0, empty);
        return;
    }

    for (int i = m_user->getPostCount() - 1; i >= 0; --i) {
        Posts* p = m_user->getPostByIndex(i);
        if (!p || !p->isValid()) continue;
        auto* card = new PostCard(p,
            QString::fromStdString(m_user->getUsername()),
            true, false,
            QString::fromStdString(m_user->getUsername()),
            m_user->getIsReported());
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
    emit requestOpenComments(post);
}

// ══════════════════════════════════════════════════════════════════════════════
//  SavedPostsPage
// ══════════════════════════════════════════════════════════════════════════════
SavedPostsPage::SavedPostsPage(User* currentUser, User** allUsers, int userCount,
    QWidget* parent)
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

    m_user->loadSavedPosts(m_allUsers, m_userCount);
    if (m_user->getSavedPostCount() == 0) {
        auto* empty = new QLabel("No saved posts yet. Save posts from your feed using ☆");
        empty->setAlignment(Qt::AlignCenter);
        empty->setWordWrap(true);
        empty->setStyleSheet("color:#28284A;font-size:13px;padding:30px 20px;");
        m_savedLayout->insertWidget(0, empty);
        return;
    }

    for (int i = 0; i < m_user->getSavedPostCount(); ++i) {
        Posts* p = m_user->getSavedPostByIndex(i);
        if (!p) continue;

        bool reportedByMe = false;
        for (int u = 0; u < m_userCount; ++u) {
            if (m_allUsers[u] &&
                m_allUsers[u]->getUsername() == p->getCreatorUsername()) {
                reportedByMe = m_allUsers[u]->hasReportedUser(m_user->getUsername());
                break;
            }
        }

        bool postReportedByMe = p->hasReportedBy(m_user->getUsername());

        auto* card = new PostCard(p,
            QString::fromStdString(p->getCreatorUsername()),
            false, true,
            QString::fromStdString(m_user->getUsername()),
            reportedByMe,
            postReportedByMe);

        connect(card, &PostCard::likeClicked, this, [](Posts*) {});
        connect(card, &PostCard::unsaveClicked, this, &SavedPostsPage::onUnsavePost);
        connect(card, &PostCard::commentClicked, this, [this, p]() {
            emit requestOpenComments(p);
            });
        connect(card, &PostCard::reportUserClicked, this, [this](const QString& username) {
            User* target = nullptr;
            for (int i = 0; i < m_userCount; ++i) {
                if (m_allUsers[i] &&
                    m_allUsers[i]->getUsername() == username.toStdString()) {
                    target = m_allUsers[i]; break;
                }
            }
            if (!target) return;
            if (target->hasReportedUser(m_user->getUsername())) {
                QMessageBox::information(this, "Already Reported",
                    "You have already reported this user.");
                return;
            }
            target->reportUserBy(m_user->getUsername());
            QMessageBox::information(this, "Reported",
                "User @" + username + " has been reported.");
            refresh();
            });
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
NotificationsPage::NotificationsPage(User* currentUser, QWidget* parent)
    : QWidget(parent), m_user(currentUser)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Notifications", "pageTitle"), 1);

    auto* clearBtn = makeDanger("🗑 Clear All");
    clearBtn->setFixedWidth(100);
    clearBtn->setFixedHeight(34);
    connect(clearBtn, &QPushButton::clicked, this, [this]() {
        if (!m_user) return;
        auto r = QMessageBox::question(this, "Clear Notifications",
            "Delete all notifications?", QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::Yes) {
            NotificationManager::clearAllNotifications(m_user->getUsername());
            refresh();
        }
        });

    auto* refBtn = makeSecondary("↻ Refresh");
    refBtn->setFixedWidth(100);
    refBtn->setFixedHeight(34);
    connect(refBtn, &QPushButton::clicked, this, &NotificationsPage::refresh);

    hRow->addWidget(clearBtn);
    hRow->addWidget(refBtn);
    outer->addLayout(hRow);

    m_scrollArea = makeScrollArea(m_listContent, m_listLayout);
    m_scrollArea->setWidgetResizable(true);
    m_listLayout->setAlignment(Qt::AlignTop);
    m_listLayout->setContentsMargins(0, 0, 0, 0);
    outer->addWidget(m_scrollArea, 1);

    loadNotifications();
}

void NotificationsPage::refresh() { loadNotifications(); }

void NotificationsPage::loadNotifications() {
    while (m_listLayout->count() > 0) {
        QLayoutItem* item = m_listLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (!m_user) return;

    // Pure C++ backend call — returns raw array + count
    int count = 0;
    Notification* notifications =
        NotificationManager::loadAllNotifications(m_user->getUsername(), count);

    if (!notifications || count == 0) {
        auto* empty = new QLabel("No notifications yet!");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A;font-size:14px;padding:40px 0;");
        m_listLayout->insertWidget(0, empty);
        delete[] notifications;
        return;
    }

    // Newest first
    for (int i = count - 1; i >= 0; --i) {
        auto* item = new NotificationItem(notifications[i]);
        m_listLayout->insertWidget(m_listLayout->count(), item);
    }

    delete[] notifications;
}

// ══════════════════════════════════════════════════════════════════════════════
//  SearchPage
// ══════════════════════════════════════════════════════════════════════════════
SearchPage::SearchPage(User**& allUsers, int& userCount, User* currentUser, QWidget* parent)
    : QWidget(parent),
    m_currentUser(currentUser),
    m_allUsers(allUsers),
    m_userCount(userCount),
    m_foundUser(nullptr),
    m_followBtn(nullptr),
    m_reportBtn(nullptr)
{
    m_engine.setUsers(m_allUsers, m_userCount);

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
    if (query.isEmpty()) {
        QMessageBox::warning(this, "Novo", "Please enter a search query.");
        return;
    }

    // Refresh search engine index in case users changed since login
    m_engine.setUsers(m_allUsers, m_userCount);

    clearResults();
    m_foundUser = nullptr;
    m_followBtn = nullptr;
    m_reportBtn = nullptr;

    // Pure C++ backend search
    User* found = m_engine.searchUser(query.toStdString());

    if (!found) {
        // Partial match: iterate manually
        for (int i = 0; i < m_userCount; ++i) {
            if (!m_allUsers[i]) continue;
            string uname = m_allUsers[i]->getUsername();
            if (uname.find(query.toStdString()) != string::npos) {
                found = m_allUsers[i];
                break;
            }
        }
    }

    if (!found) {
        auto* nf = new QLabel("No user found for \"" + query + "\"");
        nf->setAlignment(Qt::AlignCenter);
        nf->setStyleSheet("color:#34345A;font-size:13px;padding:30px 0;");
        m_resultsLayout->insertWidget(0, nf);
        return;
    }

    if (found->getIsBanned()) {
        QMessageBox::warning(this, "Novo", "User @" + query + " is banned.");
        return;
    }

    m_foundUser = found;
    showUserCard(found);
}

void SearchPage::clearResults() {
    while (m_resultsLayout->count() > 1) {
        QLayoutItem* item = m_resultsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void SearchPage::showUserCard(User* user) {
    if (!user) return;

    auto* card = new QFrame;
    card->setObjectName("resultCard");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(22, 18, 22, 18);
    cl->setSpacing(14);

    // Avatar + info
    auto* topRow = new QHBoxLayout;
    topRow->setSpacing(14);
    topRow->setContentsMargins(0, 0, 0, 0);
    QString ini = QString::fromStdString(user->getUsername()).left(1).toUpper();
    topRow->addWidget(avatar(ini, 50), 0, Qt::AlignVCenter);
    auto* infoCol = new QVBoxLayout;
    infoCol->setSpacing(4);
    auto* nameLbl = makeLabel(QString::fromStdString(user->getUsername()));
    nameLbl->setStyleSheet("font-size:16px;font-weight:700;color:#DCDCF8;");
    auto* bioLbl = makeLabel(QString::fromStdString(user->getBio()));
    bioLbl->setStyleSheet("font-size:12px;color:#56567A;");
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
        v->setStyleSheet("font-size:18px;font-weight:700;color:#6060FF;");
        v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size:10px;color:#36365A;letter-spacing:1px;");
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
        warn->setStyleSheet("color:#AA4040;font-size:12px;background:#180606;border-radius:6px;padding:6px 10px;");
        cl->addWidget(warn);
    }

    cl->addWidget(divider());

    // Action buttons
    if (m_currentUser && m_currentUser != user) {
        auto* btnRow = new QHBoxLayout;
        btnRow->setContentsMargins(0, 0, 0, 0);
        btnRow->addStretch(1);

        auto* profileBtn = new QPushButton("👤 View Profile");
        profileBtn->setObjectName("secondaryBtn");
        profileBtn->setFixedWidth(130);
        profileBtn->setFixedHeight(36);
        profileBtn->setCursor(Qt::PointingHandCursor);
        connect(profileBtn, &QPushButton::clicked, this, [this, user]() {
            emit requestViewProfile(user);
            });
        btnRow->addWidget(profileBtn);
        btnRow->addSpacing(10);

        bool following = m_currentUser->isFollowing(user->getUsername());
        m_followBtn = following ? makeSecondary("✓ Following") : makePrimary("+ Follow");
        m_followBtn->setFixedWidth(130);
        m_followBtn->setFixedHeight(36);
        connect(m_followBtn, &QPushButton::clicked, this, &SearchPage::onFollowUser);
        btnRow->addWidget(m_followBtn);
        btnRow->addSpacing(10);

        bool alreadyReported = user->hasReportedUser(m_currentUser->getUsername())
            || user->getIsReported();
        m_reportBtn = makeDanger(alreadyReported ? "⚑ Reported" : "⚑ Report");
        m_reportBtn->setFixedWidth(100);
        m_reportBtn->setFixedHeight(36);
        m_reportBtn->setEnabled(!alreadyReported);
        if (alreadyReported)
            m_reportBtn->setStyleSheet(
                "background:#1C0808;border:1px solid #4A2020;color:#CC4444;border-radius:9px;font-size:13px;");
        connect(m_reportBtn, &QPushButton::clicked, this, &SearchPage::onReportUser);
        btnRow->addWidget(m_reportBtn);
        btnRow->addStretch(1);
        cl->addLayout(btnRow);
    }

    m_resultsLayout->insertWidget(0, card);
}

void SearchPage::onFollowUser() {
    if (!m_foundUser || !m_currentUser || !m_followBtn) return;

    string foundName = m_foundUser->getUsername();

    // Re-fetch live pointer
    User* found = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] && m_allUsers[i]->getUsername() == foundName) {
            found = m_allUsers[i]; break;
        }
    }
    if (!found) return;

    if (m_currentUser->isFollowing(foundName)) {
        m_currentUser->unfollowUser(foundName);
        m_followBtn->setText("+ Follow");
        m_followBtn->setObjectName("primaryBtn");
    }
    else {
        m_currentUser->followUser(found, m_allUsers, m_userCount);
        m_followBtn->setText("✓ Following");
        m_followBtn->setObjectName("secondaryBtn");
    }
    m_followBtn->style()->unpolish(m_followBtn);
    m_followBtn->style()->polish(m_followBtn);
    m_followBtn->update();
}

void SearchPage::onReportUser() {
    if (!m_foundUser || !m_currentUser) return;
    if (m_foundUser == m_currentUser) {
        QMessageBox::warning(this, "Novo", "You cannot report yourself.");
        return;
    }
    if (m_foundUser->hasReportedUser(m_currentUser->getUsername())) {
        QMessageBox::information(this, "Already Reported",
            "You have already reported this user.");
        return;
    }
    auto r = QMessageBox::question(this, "Report User",
        "Report @" + QString::fromStdString(m_foundUser->getUsername()) + "?",
        QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        m_foundUser->reportUserBy(m_currentUser->getUsername());
        m_reportBtn->setText("⚑ Reported");
        m_reportBtn->setStyleSheet(
            "background:#1C0808;border:1px solid #4A2020;color:#CC4444;border-radius:9px;font-size:13px;");
        m_reportBtn->setEnabled(false);
        QMessageBox::information(this, "Novo", "User has been reported to admins.");
    }
}
// ============================================================================
//  QtWidgetsApplication1_completion.cpp
//
//  HOW TO USE:
//  1. Make sure the VERY FIRST LINE of QtWidgetsApplication1.cpp is:
//        #define _CRT_SECURE_NO_WARNINGS
//  2. Paste everything below this comment block at the END of
//     QtWidgetsApplication1.cpp, after SearchPage::onReportUser().
// ============================================================================

// ══════════════════════════════════════════════════════════════════════════════
//  ChatView
// ══════════════════════════════════════════════════════════════════════════════
ChatView::ChatView(const QString& currentUser, const QString& peerUsername,
    QWidget* parent)
    : QWidget(parent), m_currentUser(currentUser), m_peer(peerUsername)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Chat header ──────────────────────────────────────────────────────────
    auto* header = new QFrame;
    header->setStyleSheet(
        "QFrame{background:#0F0F16;border-bottom:1px solid #1C1C28;padding:0;}");
    header->setFixedHeight(56);
    auto* hRow = new QHBoxLayout(header);
    hRow->setContentsMargins(18, 0, 18, 0);
    hRow->setSpacing(12);

    QString ini = m_peer.isEmpty() ? "?" : m_peer.left(1).toUpper();
    auto* av = new QLabel(ini);
    av->setFixedSize(36, 36);
    av->setAlignment(Qt::AlignCenter);
    av->setStyleSheet(
        "background:#202040;border-radius:18px;"
        "font-size:15px;font-weight:700;color:#7070FF;");
    hRow->addWidget(av);

    auto* nameLbl = new QLabel("@" + m_peer);
    nameLbl->setStyleSheet("font-size:14px;font-weight:600;color:#C8C8E8;");
    hRow->addWidget(nameLbl, 1);

    auto* delBtn = new QPushButton("🗑 Delete Chat");
    delBtn->setObjectName("dangerBtn");
    delBtn->setFixedHeight(30);
    delBtn->setCursor(Qt::PointingHandCursor);
    connect(delBtn, &QPushButton::clicked, this, &ChatView::onDeleteChat);
    hRow->addWidget(delBtn);

    root->addWidget(header);

    // ── Bubble scroll area ───────────────────────────────────────────────────
    m_scroll = new QScrollArea;
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setStyleSheet("background:transparent;border:none;");
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_bubbleContainer = new QWidget;
    m_bubbleContainer->setStyleSheet("background:transparent;");
    m_bubbleLayout = new QVBoxLayout(m_bubbleContainer);
    m_bubbleLayout->setContentsMargins(16, 12, 16, 12);
    m_bubbleLayout->setSpacing(6);
    m_bubbleLayout->addStretch(1);

    m_scroll->setWidget(m_bubbleContainer);
    root->addWidget(m_scroll, 1);

    // ── Input row ────────────────────────────────────────────────────────────
    auto* inputFrame = new QFrame;
    inputFrame->setStyleSheet(
        "QFrame{background:#0F0F16;border-top:1px solid #1C1C28;}");
    inputFrame->setFixedHeight(60);
    auto* inputRow = new QHBoxLayout(inputFrame);
    inputRow->setContentsMargins(16, 10, 16, 10);
    inputRow->setSpacing(10);

    m_input = new QLineEdit;
    m_input->setPlaceholderText("Type a message...");
    m_input->setFixedHeight(38);
    connect(m_input, &QLineEdit::returnPressed, this, &ChatView::onSend);

    auto* sendBtn = new QPushButton("Send");
    sendBtn->setObjectName("primaryBtn");
    sendBtn->setFixedSize(70, 38);
    sendBtn->setCursor(Qt::PointingHandCursor);
    connect(sendBtn, &QPushButton::clicked, this, &ChatView::onSend);

    inputRow->addWidget(m_input, 1);
    inputRow->addWidget(sendBtn);
    root->addWidget(inputFrame);

    loadMessages();
}

QString ChatView::chatFilePath() const {
    // Canonical: sort usernames alphabetically so both sides share one file
    QStringList pair = { m_currentUser, m_peer };
    pair.sort();
    return "data/Messages/" + pair[0] + "_" + pair[1] + ".txt";
}

void ChatView::loadMessages() {
    clearMessages();

    QFile f(chatFilePath());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        // Format: sender|receiver|timestamp|content
        QStringList parts = line.split('|');
        if (parts.size() < 4) continue;
        QString sender = parts[0];
        // parts[1] = receiver, parts[2] = timestamp
        QString content = parts.mid(3).join('|');   // content may contain '|'
        appendBubble(sender, content);
    }
    f.close();

    // Scroll to bottom
    QApplication::processEvents();
    m_scroll->verticalScrollBar()->setValue(
        m_scroll->verticalScrollBar()->maximum());
}

void ChatView::clearMessages() {
    while (m_bubbleLayout->count() > 1) {
        QLayoutItem* item = m_bubbleLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void ChatView::appendBubble(const QString& sender, const QString& text) {
    bool isMe = (sender == m_currentUser);

    auto* row = new QHBoxLayout;
    row->setContentsMargins(0, 0, 0, 0);
    row->setSpacing(0);

    auto* bubble = new QLabel(text);
    bubble->setWordWrap(true);
    bubble->setMaximumWidth(340);
    bubble->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    if (isMe) {
        bubble->setStyleSheet(
            "background:#2A2A80;color:#E8E8FF;border-radius:14px 14px 4px 14px;"
            "padding:9px 14px;font-size:13px;");
        row->addStretch(1);
        row->addWidget(bubble);
    }
    else {
        bubble->setStyleSheet(
            "background:#1A1A28;color:#C8C8DC;border-radius:14px 14px 14px 4px;"
            "padding:9px 14px;font-size:13px;");
        row->addWidget(bubble);
        row->addStretch(1);
    }

    auto* wrapper = new QWidget;
    wrapper->setLayout(row);
    wrapper->setStyleSheet("background:transparent;");
    m_bubbleLayout->insertWidget(m_bubbleLayout->count() - 1, wrapper);
}

void ChatView::onSend() {
    QString text = m_input->text().trimmed();
    if (text.isEmpty() || m_peer.isEmpty()) return;
    if (text.contains('|')) {
        QMessageBox::warning(this, "Novo", "Messages cannot contain the '|' character.");
        return;
    }

    // Append to file
    QDir().mkpath("data/Messages");
    QFile f(chatFilePath());
    if (f.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&f);
        QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        out << m_currentUser << "|" << m_peer << "|" << ts << "|" << text << "\n";
        f.close();
    }

    appendBubble(m_currentUser, text);
    m_input->clear();
    QApplication::processEvents();
    m_scroll->verticalScrollBar()->setValue(
        m_scroll->verticalScrollBar()->maximum());
}

void ChatView::onDeleteChat() {
    auto r = QMessageBox::question(this, "Delete Chat",
        "Delete the entire conversation with @" + m_peer + "?",
        QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;

    QFile::remove(chatFilePath());
    emit chatDeleted(m_peer);
}

void ChatView::refresh() {
    loadMessages();
}

void ChatView::markConversationUnread(const QString& receiver, const QString& sender) {
    // Writes a marker file so the receiver's conversation list can show an
    // unread indicator. The marker is cleared when the receiver opens the chat.
    QDir().mkpath("data/Messages");
    QString markerPath = "data/Messages/" + receiver + "_unread_" + sender + ".flag";
    QFile f(markerPath);
    (void)f.open(QIODevice::WriteOnly);
    f.close();
}

// ══════════════════════════════════════════════════════════════════════════════
//  MessagesPage
// ══════════════════════════════════════════════════════════════════════════════
MessagesPage::MessagesPage(User* currentUser, User** allUsers, int userCount,
    QWidget* parent)
    : QWidget(parent),
    m_currentUser(currentUser),
    m_allUsers(allUsers),
    m_userCount(userCount),
    m_chatView(nullptr)
{
    setObjectName("pageArea");
    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Left panel: conversation list ─────────────────────────────────────────
    m_leftPanel = new QWidget;
    m_leftPanel->setFixedWidth(240);
    m_leftPanel->setStyleSheet(
        "QWidget{background:#0F0F16;border-right:1px solid #1C1C28;}");

    auto* leftLayout = new QVBoxLayout(m_leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    // Header
    auto* leftHeader = new QFrame;
    leftHeader->setStyleSheet(
        "QFrame{background:#0F0F16;border-bottom:1px solid #1C1C28;}");
    leftHeader->setFixedHeight(56);
    auto* lhRow = new QHBoxLayout(leftHeader);
    lhRow->setContentsMargins(14, 0, 10, 0);
    lhRow->setSpacing(8);
    auto* msgTitle = new QLabel("Messages");
    msgTitle->setStyleSheet("font-size:16px;font-weight:700;color:#E0E0F8;");
    lhRow->addWidget(msgTitle, 1);
    leftLayout->addWidget(leftHeader);

    // Search / new chat input
    m_searchInput = new QLineEdit;
    m_searchInput->setPlaceholderText("New chat: enter username");
    m_searchInput->setFixedHeight(38);
    m_searchInput->setStyleSheet(
        "QLineEdit{background:#141420;border:none;border-bottom:1px solid #1C1C28;"
        "border-radius:0;padding:0 14px;color:#E2E2EC;font-size:12px;}");
    connect(m_searchInput, &QLineEdit::returnPressed,
        this, &MessagesPage::onSearchUser);
    leftLayout->addWidget(m_searchInput);

    // Conversation list
    auto* convScroll = new QScrollArea;
    convScroll->setWidgetResizable(true);
    convScroll->setFrameShape(QFrame::NoFrame);
    convScroll->setStyleSheet("background:transparent;border:none;");

    m_convListContent = new QWidget;
    m_convListContent->setStyleSheet("background:transparent;");
    m_convListLayout = new QVBoxLayout(m_convListContent);
    m_convListLayout->setContentsMargins(0, 4, 0, 4);
    m_convListLayout->setSpacing(0);
    m_convListLayout->addStretch(1);
    convScroll->setWidget(m_convListContent);
    leftLayout->addWidget(convScroll, 1);
    root->addWidget(m_leftPanel);

    // ── Right panel: chat view ────────────────────────────────────────────────
    m_rightStack = new QStackedWidget;

    // Placeholder when no chat selected
    auto* placeholder = new QWidget;
    placeholder->setStyleSheet("background:#0C0C10;");
    auto* phLayout = new QVBoxLayout(placeholder);
    auto* phLabel = new QLabel("Select a conversation\nor start a new one");
    phLabel->setAlignment(Qt::AlignCenter);
    phLabel->setStyleSheet("color:#28284A;font-size:15px;");
    phLayout->addWidget(phLabel);
    m_rightStack->addWidget(placeholder);   // index 0

    root->addWidget(m_rightStack, 1);

    loadConversationList();
}

void MessagesPage::loadConversationList() {
    clearConversationList();
    if (!m_currentUser) return;

    QString me = QString::fromStdString(m_currentUser->getUsername());

    // Scan data/Messages/ for files involving this user
    QDir msgDir("data/Messages");
    if (!msgDir.exists()) return;

    QStringList files = msgDir.entryList(QStringList("*.txt"), QDir::Files);
    QStringList peers;
    auto peersContains = [&](const QString& s) {
        for (const QString& p : peers) if (p == s) return true;
        return false;
        };
    for (const QString& fname : files) {
        // filename: userA_userB.txt
        QString base = fname;
        base.remove(".txt");
        QStringList parts = base.split('_');
        if (parts.size() < 2) continue;
        // Might have underscores in names; try both splits
        if (parts[0] == me) {
            QString p = parts.mid(1).join('_');
            if (!peersContains(p)) peers.append(p);
        }
        else if (parts.last() == me) {
            QString p = parts.mid(0, parts.size() - 1).join('_');
            if (!peersContains(p)) peers.append(p);
        }
        else {
            for (int i = 1; i < parts.size(); ++i) {
                QString a = parts.mid(0, i).join('_');
                QString b = parts.mid(i).join('_');
                if (a == me) { if (!peersContains(b)) peers.append(b); break; }
                if (b == me) { if (!peersContains(a)) peers.append(a); break; }
            }
        }
    }

    for (const QString& peer : peers) {
        addConversationButton(peer);
    }
}

void MessagesPage::clearConversationList() {
    while (m_convListLayout->count() > 1) {
        QLayoutItem* item = m_convListLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void MessagesPage::addConversationButton(const QString& peer) {
    auto* btn = new QPushButton;
    btn->setFixedHeight(54);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Use first letter of username; fall back to '#' if username starts with digit
    QChar firstChar = peer.isEmpty() ? QChar('?') : peer.at(0).toUpper();
    QString ini = firstChar.isLetter() ? QString(firstChar) : QString("#");
    btn->setText("  " + ini + "  @" + peer);
    btn->setStyleSheet(
        "QPushButton{background:transparent;border:none;border-bottom:1px solid #14141E;"
        "text-align:left;color:#A0A0CC;font-size:13px;padding:0 14px;}"
        "QPushButton:hover{background:#131320;color:#E0E0FF;}"
        "QPushButton:checked{background:#1B1B2E;color:#8080FF;}");
    btn->setCheckable(true);

    connect(btn, &QPushButton::clicked, this, [this, peer, btn]() {
        // uncheck siblings
        for (int i = 0; i < m_convListLayout->count() - 1; ++i) {
            QLayoutItem* it = m_convListLayout->itemAt(i);
            if (it && it->widget()) {
                auto* b = qobject_cast<QPushButton*>(it->widget());
                if (b && b != btn) b->setChecked(false);
            }
        }
        btn->setChecked(true);
        onConversationSelected(peer);
        });

    m_convListLayout->insertWidget(m_convListLayout->count() - 1, btn);
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

    // Verify user exists
    bool found = false;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] &&
            QString::fromStdString(m_allUsers[i]->getUsername()) == peer) {
            found = true; break;
        }
    }
    if (!found) {
        QMessageBox::warning(this, "Novo", "User @" + peer + " does not exist.");
        return;
    }

    m_searchInput->clear();
    openChatWith(peer);

    // Add to conversation list if not already there
    bool alreadyListed = false;
    for (int i = 0; i < m_convListLayout->count() - 1; ++i) {
        QLayoutItem* it = m_convListLayout->itemAt(i);
        if (it && it->widget()) {
            auto* b = qobject_cast<QPushButton*>(it->widget());
            if (b && b->text().contains("@" + peer)) {
                alreadyListed = true; break;
            }
        }
    }
    if (!alreadyListed) addConversationButton(peer);
}

void MessagesPage::onConversationSelected(const QString& peer) {
    openChatWith(peer);
}

void MessagesPage::openChatWith(const QString& peer) {
    if (!m_currentUser) return;
    m_activePeer = peer;

    // Remove old ChatView if any
    if (m_chatView) {
        m_rightStack->removeWidget(m_chatView);
        m_chatView->deleteLater();
        m_chatView = nullptr;
    }

    QString me = QString::fromStdString(m_currentUser->getUsername());
    m_chatView = new ChatView(me, peer);
    connect(m_chatView, &ChatView::chatDeleted,
        this, &MessagesPage::onChatDeleted);

    m_rightStack->addWidget(m_chatView);
    m_rightStack->setCurrentWidget(m_chatView);
}

void MessagesPage::onChatDeleted(const QString& peer) {
    // Remove conversation button
    for (int i = 0; i < m_convListLayout->count() - 1; ++i) {
        QLayoutItem* it = m_convListLayout->itemAt(i);
        if (it && it->widget()) {
            auto* b = qobject_cast<QPushButton*>(it->widget());
            if (b && b->text().contains("@" + peer)) {
                m_convListLayout->takeAt(i);
                b->deleteLater();
                break;
            }
        }
    }

    // Remove chat view
    if (m_chatView) {
        m_rightStack->setCurrentIndex(0);
        m_rightStack->removeWidget(m_chatView);
        m_chatView->deleteLater();
        m_chatView = nullptr;
    }
    m_activePeer.clear();
}

void MessagesPage::refresh() {
    loadConversationList();
    if (m_chatView) m_chatView->refresh();
}

// ══════════════════════════════════════════════════════════════════════════════
//  TimeSpentPage
// ══════════════════════════════════════════════════════════════════════════════
TimeSpentPage::TimeSpentPage(QWidget* parent)
    : QWidget(parent), m_elapsed(0)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(40, 40, 40, 40);
    root->setSpacing(20);
    root->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    root->addWidget(new QLabel("⏱"), 0, Qt::AlignCenter);
    auto* title = new QLabel("Time Spent");
    title->setObjectName("pageTitle");
    title->setAlignment(Qt::AlignCenter);
    root->addWidget(title);

    m_timerLabel = new QLabel("00:00:00");
    m_timerLabel->setObjectName("timerDisp");
    m_timerLabel->setAlignment(Qt::AlignCenter);
    root->addWidget(m_timerLabel);

    m_sessionLabel = new QLabel("Session started: —");
    m_sessionLabel->setAlignment(Qt::AlignCenter);
    m_sessionLabel->setStyleSheet("color:#36365A;font-size:12px;");
    root->addWidget(m_sessionLabel);

    root->addStretch(1);

    // Info card
    auto* card = new QFrame;
    card->setObjectName("postCard");
    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(20, 16, 20, 16);
    cl->setSpacing(8);
    auto* infoLbl = new QLabel(
        "Your active time on Novo is tracked automatically.\n"
        "The timer starts when you log in and runs while the app is open.");
    infoLbl->setWordWrap(true);
    infoLbl->setAlignment(Qt::AlignCenter);
    infoLbl->setStyleSheet("color:#56567A;font-size:13px;");
    cl->addWidget(infoLbl);
    root->addWidget(card);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &TimeSpentPage::onTick);
}

void TimeSpentPage::startSession() {
    m_elapsed = 0;
    m_sessionStart = QDateTime::currentDateTime();
    m_sessionLabel->setText("Session started: " +
        m_sessionStart.toString("hh:mm:ss"));
    m_timer->start();
    updateDisplay();
}

void TimeSpentPage::stopSession() {
    m_timer->stop();
}

void TimeSpentPage::onTick() {
    ++m_elapsed;
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

// ══════════════════════════════════════════════════════════════════════════════
//  ProfilePage
// ══════════════════════════════════════════════════════════════════════════════
ProfilePage::ProfilePage(User* currentUser, User**& allUsers, int* userCount,
    QWidget* parent)
    : QWidget(parent),
    m_user(currentUser),
    m_allUsers(allUsers),
    m_userCountPtr(userCount)
{
    // ── Scroll wrapper so all profile content is scrollable ───────────────────
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background:transparent;border:none;");

    auto* scrollContent = new QWidget;
    auto* outer = new QVBoxLayout(scrollContent);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(0);

    scrollArea->setWidget(scrollContent);
    outerLayout->addWidget(scrollArea);

    // ── Profile header ────────────────────────────────────────────────────────
    auto* headerCard = new QFrame;
    headerCard->setObjectName("postCard");
    auto* hl = new QVBoxLayout(headerCard);
    hl->setContentsMargins(24, 20, 24, 20);
    hl->setSpacing(16);

    // Avatar + name row
    auto* topRow = new QHBoxLayout;
    topRow->setSpacing(18);
    QString ini = m_user
        ? QString::fromStdString(m_user->getUsername()).left(1).toUpper()
        : "?";
    topRow->addWidget(avatar(ini, 64), 0, Qt::AlignVCenter);

    auto* nameCol = new QVBoxLayout;
    nameCol->setSpacing(4);
    m_usernameLabel = new QLabel(
        m_user ? QString::fromStdString(m_user->getUsername()) : "—");
    m_usernameLabel->setStyleSheet(
        "font-size:20px;font-weight:800;color:#E0E0FF;");
    m_bioLabel = new QLabel(
        m_user ? QString::fromStdString(m_user->getBio()) : "");
    m_bioLabel->setStyleSheet("font-size:13px;color:#56567A;");
    m_bioLabel->setWordWrap(true);
    nameCol->addWidget(m_usernameLabel);
    nameCol->addWidget(m_bioLabel);
    topRow->addLayout(nameCol, 1);
    hl->addLayout(topRow);

    // Stats row
    auto* statsRow = new QHBoxLayout;
    statsRow->setContentsMargins(0, 0, 0, 0);
    auto addStat = [&](QLabel*& ref, const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout; c->setSpacing(2);
        ref = new QLabel(val);
        ref->setStyleSheet("font-size:20px;font-weight:700;color:#6060FF;");
        ref->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size:10px;color:#36365A;letter-spacing:1px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(ref); c->addWidget(l);
        statsRow->addLayout(c);
        };
    addStat(m_postsCountLabel,
        m_user ? QString::number(m_user->getPostCount()) : "0", "POSTS");
    addStat(m_followersCountLabel,
        m_user ? QString::number(m_user->getFollowersCount()) : "0", "FOLLOWERS");
    addStat(m_followingCountLabel,
        m_user ? QString::number(m_user->getFollowingCount()) : "0", "FOLLOWING");
    hl->addLayout(statsRow);

    outer->addWidget(headerCard);
    outer->addSpacing(18);

    // ── Edit bio ──────────────────────────────────────────────────────────────
    auto* editCard = new QFrame;
    editCard->setObjectName("postCard");
    auto* ecl = new QVBoxLayout(editCard);
    ecl->setContentsMargins(20, 14, 20, 14);
    ecl->setSpacing(10);

    auto* bioLbl = new QLabel("Update Bio");
    bioLbl->setStyleSheet("font-size:12px;font-weight:600;color:#4444AA;");
    ecl->addWidget(bioLbl);

    auto* bioRow = new QHBoxLayout;
    m_newBioInput = new QLineEdit;
    m_newBioInput->setPlaceholderText("New bio...");
    m_newBioInput->setFixedHeight(32);
    auto* bioBtn = new QPushButton("Save Bio");
    bioBtn->setObjectName("secondaryBtn");
    bioBtn->setFixedHeight(32);
    bioBtn->setFixedWidth(90);
    bioBtn->setCursor(Qt::PointingHandCursor);
    connect(bioBtn, &QPushButton::clicked, this, &ProfilePage::onUpdateBio);
    bioRow->addWidget(m_newBioInput, 1);
    bioRow->addWidget(bioBtn);
    ecl->addLayout(bioRow);

    outer->addWidget(editCard);
    outer->addSpacing(10);

    // ── Change password ────────────────────────────────────────────────────────
    auto* passCard = new QFrame;
    passCard->setObjectName("postCard");
    auto* pcl = new QVBoxLayout(passCard);
    pcl->setContentsMargins(20, 14, 20, 14);
    pcl->setSpacing(10);

    auto* passLbl = new QLabel("Change Password");
    passLbl->setStyleSheet("font-size:12px;font-weight:600;color:#4444AA;");
    pcl->addWidget(passLbl);

    auto* passRow = new QHBoxLayout;
    m_newPassInput = new QLineEdit;
    m_newPassInput->setPlaceholderText("New password...");
    m_newPassInput->setEchoMode(QLineEdit::Password);
    m_newPassInput->setFixedHeight(32);
    auto* passBtn = new QPushButton("Update");
    passBtn->setObjectName("secondaryBtn");
    passBtn->setFixedHeight(32);
    passBtn->setFixedWidth(90);
    passBtn->setCursor(Qt::PointingHandCursor);
    connect(passBtn, &QPushButton::clicked, this, &ProfilePage::onUpdatePassword);
    passRow->addWidget(m_newPassInput, 1);
    passRow->addWidget(passBtn);
    pcl->addLayout(passRow);

    outer->addWidget(passCard);
    outer->addSpacing(10);

    // ── Danger zone ────────────────────────────────────────────────────────────
    auto* dangerCard = new QFrame;
    dangerCard->setObjectName("postCard");
    dangerCard->setStyleSheet(
        "QFrame#postCard{background:#0D0808;border:1px solid #3A1010;}");
    auto* dcl = new QVBoxLayout(dangerCard);
    dcl->setContentsMargins(20, 14, 20, 14);
    dcl->setSpacing(10);

    auto* dangerLbl = new QLabel("Danger Zone");
    dangerLbl->setStyleSheet("font-size:12px;font-weight:600;color:#AA3333;");
    dcl->addWidget(dangerLbl);

    auto* delBtn = new QPushButton("🗑  Delete My Account");
    delBtn->setObjectName("dangerBtn");
    delBtn->setFixedHeight(38);
    delBtn->setCursor(Qt::PointingHandCursor);
    connect(delBtn, &QPushButton::clicked, this, &ProfilePage::onDeleteAccount);
    dcl->addWidget(delBtn);

    outer->addWidget(dangerCard);
    outer->addSpacing(18);

    // ── Tabs: My Posts / Saved ─────────────────────────────────────────────────
    auto* tabRow = new QHBoxLayout;
    tabRow->setSpacing(0);
    tabRow->setContentsMargins(0, 0, 0, 0);

    auto* tabMyPosts = new QPushButton("My Posts");
    tabMyPosts->setFixedHeight(36);
    tabMyPosts->setCursor(Qt::PointingHandCursor);
    tabMyPosts->setStyleSheet(
        "QPushButton{background:#1B1B2A;border:none;border-radius:0;"
        "border-top-left-radius:8px;border-bottom-left-radius:8px;"
        "color:#7070FF;font-size:13px;font-weight:600;}"
        "QPushButton:hover{background:#222238;}");

    auto* tabSaved = new QPushButton("Saved");
    tabSaved->setFixedHeight(36);
    tabSaved->setCursor(Qt::PointingHandCursor);
    tabSaved->setStyleSheet(
        "QPushButton{background:#111119;border:none;border-radius:0;"
        "border-top-right-radius:8px;border-bottom-right-radius:8px;"
        "color:#44446A;font-size:13px;}"
        "QPushButton:hover{background:#16161F;color:#8888CC;}");

    tabRow->addWidget(tabMyPosts, 1);
    tabRow->addWidget(tabSaved, 1);
    outer->addLayout(tabRow);

    auto* tabContent = new QStackedWidget;
    m_myPostsPage = new MyPostsPage(m_user);
    m_savedPage = new SavedPostsPage(m_user, m_allUsers, *m_userCountPtr);
    tabContent->addWidget(m_myPostsPage);   // 0
    tabContent->addWidget(m_savedPage);     // 1
    outer->addWidget(tabContent, 1);

    connect(tabMyPosts, &QPushButton::clicked, this, [tabContent, tabMyPosts, tabSaved, this]() {
        tabContent->setCurrentIndex(0);
        tabMyPosts->setStyleSheet(
            "QPushButton{background:#1B1B2A;border:none;border-radius:0;"
            "border-top-left-radius:8px;border-bottom-left-radius:8px;"
            "color:#7070FF;font-size:13px;font-weight:600;}"
            "QPushButton:hover{background:#222238;}");
        tabSaved->setStyleSheet(
            "QPushButton{background:#111119;border:none;border-radius:0;"
            "border-top-right-radius:8px;border-bottom-right-radius:8px;"
            "color:#44446A;font-size:13px;}"
            "QPushButton:hover{background:#16161F;color:#8888CC;}");
        m_myPostsPage->refresh();
        });
    connect(tabSaved, &QPushButton::clicked, this, [tabContent, tabMyPosts, tabSaved, this]() {
        tabContent->setCurrentIndex(1);
        tabSaved->setStyleSheet(
            "QPushButton{background:#1B1B2A;border:none;border-radius:0;"
            "border-top-right-radius:8px;border-bottom-right-radius:8px;"
            "color:#7070FF;font-size:13px;font-weight:600;}"
            "QPushButton:hover{background:#222238;}");
        tabMyPosts->setStyleSheet(
            "QPushButton{background:#111119;border:none;border-radius:0;"
            "border-top-left-radius:8px;border-bottom-left-radius:8px;"
            "color:#44446A;font-size:13px;}"
            "QPushButton:hover{background:#16161F;color:#8888CC;}");
        m_savedPage->refresh();
        });

    // Forward comment requests
    connect(m_myPostsPage, &MyPostsPage::requestOpenComments,
        this, &ProfilePage::requestOpenComments);
    connect(m_savedPage, &SavedPostsPage::requestOpenComments,
        this, &ProfilePage::requestOpenComments);
}

void ProfilePage::refresh() {
    if (!m_user) return;
    m_usernameLabel->setText(QString::fromStdString(m_user->getUsername()));
    m_bioLabel->setText(QString::fromStdString(m_user->getBio()));
    if (m_postsCountLabel)
        m_postsCountLabel->setText(QString::number(m_user->getPostCount()));
    if (m_followersCountLabel)
        m_followersCountLabel->setText(QString::number(m_user->getFollowersCount()));
    if (m_followingCountLabel)
        m_followingCountLabel->setText(QString::number(m_user->getFollowingCount()));
    m_myPostsPage->refresh();
    m_savedPage->refresh();
}

void ProfilePage::onUpdateBio() {
    if (!m_user) return;
    QString newBio = m_newBioInput->text().trimmed();
    if (newBio.isEmpty()) {
        QMessageBox::warning(this, "Novo", "Bio cannot be empty.");
        return;
    }
    if (newBio.contains('|')) {
        QMessageBox::warning(this, "Novo", "Bio cannot contain '|'.");
        return;
    }
    string err = User::validateBio(newBio.toStdString());
    if (!err.empty()) {
        QMessageBox::warning(this, "Invalid Bio", QString::fromStdString(err));
        return;
    }
    m_user->updateBio(newBio.toStdString());
    m_bioLabel->setText(newBio);
    m_newBioInput->clear();
    QMessageBox::information(this, "Novo", "Bio updated successfully.");
}

void ProfilePage::onUpdatePassword() {
    if (!m_user) return;
    QString newPass = m_newPassInput->text();
    if (newPass.isEmpty()) {
        QMessageBox::warning(this, "Novo", "Password cannot be empty.");
        return;
    }
    PasswordChecker checker(8);
    string err = checker.validatePassword(newPass.toStdString());
    if (!err.empty()) {
        QString feedback = QString::fromStdString(
            checker.getValidationFeedback(newPass.toStdString()));
        QMessageBox::warning(this, "Weak Password",
            QString::fromStdString(err) + "\n\n" + feedback);
        return;
    }
    m_user->updatePassword(newPass.toStdString());
    m_newPassInput->clear();
    QMessageBox::information(this, "Novo", "Password updated successfully.");
}

void ProfilePage::onDeleteAccount() {
    if (!m_user) return;
    auto r = QMessageBox::question(this, "Delete Account",
        "⚠️ This will PERMANENTLY delete your account, all your posts, and all your data.\n\n"
        "This action CANNOT be undone. Are you absolutely sure?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);   // default to No for safety
    if (r != QMessageBox::Yes) return;

    m_user->deleteAccount(m_allUsers, *m_userCountPtr);
    m_user = nullptr;
    emit accountDeleted();
}

// ══════════════════════════════════════════════════════════════════════════════
//  AdminPage
// ══════════════════════════════════════════════════════════════════════════════
AdminPage::AdminPage(Admin* admin, User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent),
    m_admin(admin),
    m_allUsers(allUsers),
    m_userCount(userCount)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    // Header
    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    auto* title = new QLabel("Admin Dashboard");
    title->setObjectName("pageTitle");
    hRow->addWidget(title);
    hRow->addStretch(1);

    auto* refBtn = new QPushButton("↻ Refresh");
    refBtn->setObjectName("secondaryBtn");
    refBtn->setFixedHeight(34);
    refBtn->setFixedWidth(100);
    refBtn->setCursor(Qt::PointingHandCursor);
    connect(refBtn, &QPushButton::clicked, this, &AdminPage::refresh);
    hRow->addWidget(refBtn);
    outer->addLayout(hRow);

    // Stats strip
    auto* statsCard = new QFrame;
    statsCard->setObjectName("postCard");
    auto* sl = new QHBoxLayout(statsCard);
    sl->setContentsMargins(20, 16, 20, 16);
    sl->setSpacing(0);

    auto addAdminStat = [&](const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout; c->setSpacing(2);
        auto* v = new QLabel(val);
        v->setStyleSheet("font-size:22px;font-weight:700;color:#6060FF;");
        v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size:10px;color:#36365A;letter-spacing:1.2px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l);
        sl->addLayout(c);
        };

    if (m_admin) {
        m_admin->loadReportsFromFile(m_allUsers, m_userCount);
        addAdminStat(QString::number(m_admin->getReportedUserCount()), "REPORTED USERS");
        addAdminStat(QString::number(m_admin->getReportedPostCount()), "REPORTED POSTS");
        addAdminStat(QString::number(m_userCount), "TOTAL USERS");
    }
    outer->addWidget(statsCard);

    // Action buttons
    auto* actRow = new QHBoxLayout;
    actRow->setContentsMargins(0, 0, 0, 0);
    actRow->setSpacing(12);

    auto* reviewUsersBtn = new QPushButton("👥 Review Reported Users");
    reviewUsersBtn->setObjectName("secondaryBtn");
    reviewUsersBtn->setFixedHeight(40);
    reviewUsersBtn->setCursor(Qt::PointingHandCursor);
    connect(reviewUsersBtn, &QPushButton::clicked,
        this, &AdminPage::onReviewReportedUsers);

    auto* reviewPostsBtn = new QPushButton("📋 Review Reported Posts");
    reviewPostsBtn->setObjectName("secondaryBtn");
    reviewPostsBtn->setFixedHeight(40);
    reviewPostsBtn->setCursor(Qt::PointingHandCursor);
    connect(reviewPostsBtn, &QPushButton::clicked,
        this, &AdminPage::onReviewReportedPosts);

    auto* notifBtn = new QPushButton("🔔 Admin Notifications");
    notifBtn->setObjectName("secondaryBtn");
    notifBtn->setFixedHeight(40);
    notifBtn->setCursor(Qt::PointingHandCursor);
    connect(notifBtn, &QPushButton::clicked,
        this, &AdminPage::loadAdminNotifications);

    actRow->addWidget(reviewUsersBtn, 1);
    actRow->addWidget(reviewPostsBtn, 1);
    actRow->addWidget(notifBtn, 1);
    outer->addLayout(actRow);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setStyleSheet("background:transparent;border:none;");

    m_content = new QWidget;
    m_content->setStyleSheet("background:transparent;");
    m_layout = new QVBoxLayout(m_content);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(10);
    m_layout->addStretch(1);

    m_scrollArea->setWidget(m_content);
    outer->addWidget(m_scrollArea, 1);

    // Start by loading reported users
    loadReportedUsers();
}

void AdminPage::refresh() {
    if (m_admin) m_admin->loadReportsFromFile(m_allUsers, m_userCount);
    loadReportedUsers();
}

void AdminPage::onReviewReportedUsers() { loadReportedUsers(); }
void AdminPage::onReviewReportedPosts() { loadReportedPosts(); }

void AdminPage::loadAdminNotifications() {
    // Clear list
    while (m_layout->count() > 1) {
        QLayoutItem* item = m_layout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    if (!m_admin) return;

    auto* sectionLbl = new QLabel("Admin Notifications");
    sectionLbl->setStyleSheet(
        "font-size:14px;font-weight:700;color:#E0E0F8;margin-bottom:8px;");
    m_layout->insertWidget(0, sectionLbl);

    int count = m_admin->getAdminNotifCount();
    if (count == 0) {
        auto* empty = new QLabel("No admin notifications.");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A;font-size:13px;padding:24px 0;");
        m_layout->insertWidget(1, empty);
        return;
    }

    Notification* notifs = m_admin->getAllNotifications();
    for (int i = count - 1; i >= 0; --i) {
        auto* item = new NotificationItem(notifs[i]);
        m_layout->insertWidget(m_layout->count() - 1, item);
    }
}

void AdminPage::loadReportedUsers() {
    while (m_layout->count() > 1) {
        QLayoutItem* item = m_layout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    if (!m_admin) return;

    auto* sectionLbl = new QLabel("Reported Users");
    sectionLbl->setStyleSheet(
        "font-size:14px;font-weight:700;color:#E0E0F8;margin-bottom:8px;");
    m_layout->insertWidget(0, sectionLbl);

    int count = m_admin->getReportedUserCount();
    if (count == 0) {
        auto* empty = new QLabel("No reported users. 🎉");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A;font-size:13px;padding:24px 0;");
        m_layout->insertWidget(1, empty);
        return;
    }

    User** reportedUsers = m_admin->getReportedUsers();
    for (int i = 0; i < count; ++i) {
        User* u = reportedUsers[i];
        if (!u) continue;

        // deduplicate
        bool dup = false;
        for (int j = 0; j < i; ++j) if (reportedUsers[j] == u) { dup = true; break; }
        if (dup) continue;

        auto* card = new QFrame;
        card->setObjectName("resultCard");
        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 12, 16, 12);
        cl->setSpacing(8);

        auto* nameRow = new QHBoxLayout;
        auto* nameLbl = new QLabel(QString::fromStdString(u->getUsername()));
        nameLbl->setStyleSheet("font-size:15px;font-weight:700;color:#DCDCF8;");
        nameRow->addWidget(nameLbl, 1);

        auto* banBtn = new QPushButton(u->getIsBanned() ? "🔓 Unban" : "🔨 Ban");
        banBtn->setObjectName(u->getIsBanned() ? "secondaryBtn" : "dangerBtn");
        banBtn->setFixedHeight(32);
        banBtn->setFixedWidth(90);
        banBtn->setCursor(Qt::PointingHandCursor);
        connect(banBtn, &QPushButton::clicked, this, [this, u, banBtn]() {
            if (u->getIsBanned()) {
                u->setBan(false);
                banBtn->setText("🔨 Ban");
                banBtn->setObjectName("dangerBtn");
                QMessageBox::information(this, "Admin", "User unbanned.");
            }
            else {
                auto r = QMessageBox::question(this, "Ban User",
                    "Ban @" + QString::fromStdString(u->getUsername()) + "?",
                    QMessageBox::Yes | QMessageBox::No);
                if (r == QMessageBox::Yes) {
                    u->setBan(true);
                    u->saveToFile();
                    banBtn->setText("🔓 Unban");
                    banBtn->setObjectName("secondaryBtn");
                    QMessageBox::information(this, "Admin", "User banned.");
                }
            }
            banBtn->style()->unpolish(banBtn);
            banBtn->style()->polish(banBtn);
            });
        nameRow->addWidget(banBtn);

        auto* deleteBtn = new QPushButton("🗑 Delete User");
        deleteBtn->setObjectName("dangerBtn");
        deleteBtn->setFixedHeight(32);
        deleteBtn->setFixedWidth(110);
        deleteBtn->setCursor(Qt::PointingHandCursor);
        connect(deleteBtn, &QPushButton::clicked, this, [this, u]() {
            auto r = QMessageBox::question(this, "Delete User",
                "Permanently delete @" +
                QString::fromStdString(u->getUsername()) + " and all their data?",
                QMessageBox::Yes | QMessageBox::No);
            if (r == QMessageBox::Yes) {
                m_admin->deleteUser(m_allUsers, m_userCount, u->getUsername());
                QMessageBox::information(this, "Admin", "User deleted.");
                refresh();
            }
            });
        nameRow->addSpacing(8);
        nameRow->addWidget(deleteBtn);
        cl->addLayout(nameRow);

        auto* bioLbl = new QLabel(QString::fromStdString(u->getBio()));
        bioLbl->setStyleSheet("font-size:12px;color:#56567A;");
        cl->addWidget(bioLbl);

        m_layout->insertWidget(m_layout->count() - 1, card);
    }
}

void AdminPage::loadReportedPosts() {
    while (m_layout->count() > 1) {
        QLayoutItem* item = m_layout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    if (!m_admin) return;

    auto* sectionLbl = new QLabel("Reported Posts");
    sectionLbl->setStyleSheet(
        "font-size:14px;font-weight:700;color:#E0E0F8;margin-bottom:8px;");
    m_layout->insertWidget(0, sectionLbl);

    int count = m_admin->getReportedPostCount();
    if (count == 0) {
        auto* empty = new QLabel("No reported posts. 🎉");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color:#28284A;font-size:13px;padding:24px 0;");
        m_layout->insertWidget(1, empty);
        return;
    }

    Posts** reportedPosts = m_admin->getReportedPosts();
    for (int i = 0; i < count; ++i) {
        Posts* p = reportedPosts[i];
        if (!p) continue;

        bool dup = false;
        for (int j = 0; j < i; ++j) if (reportedPosts[j] == p) { dup = true; break; }
        if (dup) continue;

        auto* card = new QFrame;
        card->setObjectName("postCard");
        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 12, 16, 12);
        cl->setSpacing(8);

        auto* topRow = new QHBoxLayout;
        auto* authorLbl = new QLabel(
            "@" + QString::fromStdString(p->getCreatorUsername()));
        authorLbl->setStyleSheet("font-size:13px;font-weight:600;color:#AAAAEE;");
        topRow->addWidget(authorLbl, 1);

        auto* timeLbl = new QLabel(QString::fromStdString(p->getTimeOfCreation()));
        timeLbl->setStyleSheet("font-size:11px;color:#44445A;");
        topRow->addWidget(timeLbl);
        cl->addLayout(topRow);

        auto* contentLbl = new QLabel(QString::fromStdString(p->getContent()));
        contentLbl->setWordWrap(true);
        contentLbl->setStyleSheet("font-size:13px;color:#BBBBCC;");
        cl->addWidget(contentLbl);

        auto* btnRow = new QHBoxLayout;
        btnRow->addStretch(1);
        auto* delPostBtn = new QPushButton("🗑 Delete Post");
        delPostBtn->setObjectName("dangerBtn");
        delPostBtn->setFixedHeight(30);
        delPostBtn->setFixedWidth(110);
        delPostBtn->setCursor(Qt::PointingHandCursor);
        connect(delPostBtn, &QPushButton::clicked, this, [this, p]() {
            auto r = QMessageBox::question(this, "Delete Post",
                "Permanently delete this post?",
                QMessageBox::Yes | QMessageBox::No);
            if (r == QMessageBox::Yes) {
                m_admin->deletePost(m_allUsers, m_userCount, p->getPostId());
                QMessageBox::information(this, "Admin", "Post deleted.");
                loadReportedPosts();
            }
            });
        btnRow->addWidget(delPostBtn);
        cl->addLayout(btnRow);

        m_layout->insertWidget(m_layout->count() - 1, card);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  MainWindow
// ══════════════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_currentUser(nullptr),
    m_allUsers(nullptr),
    m_userCount(0),
    m_appShell(nullptr),
    m_shellLayout(nullptr),
    m_sidebar(nullptr),
    m_btnFeed(nullptr),
    m_btnCreatePost(nullptr),
    m_btnNotifications(nullptr),
    m_btnSearch(nullptr),
    m_btnMessages(nullptr),
    m_btnProfile(nullptr),
    m_btnTimeSpent(nullptr),
    m_btnLogout(nullptr),
    m_pages(nullptr),
    m_feedPage(nullptr),
    m_notifPage(nullptr),
    m_searchPage(nullptr),
    m_profilePage(nullptr),
    m_timeSpentPage(nullptr),
    m_createPostPage(nullptr),
    m_adminPage(nullptr),
    m_commentsPage(nullptr),
    m_publicProfilePage(nullptr)
{
    setWindowTitle(APP_NAME);
    resize(1080, 720);
    setMinimumSize(800, 600);
    qApp->setStyleSheet(APP_STYLE);

    // Centre window on screen
    if (QScreen* s = QGuiApplication::primaryScreen()) {
        QRect sg = s->availableGeometry();
        move(sg.center() - rect().center());
    }

    m_rootStack = new QStackedWidget(this);
    setCentralWidget(m_rootStack);

    m_authPage = new AuthPage;
    connect(m_authPage, &AuthPage::loginSuccess,
        this, &MainWindow::onLoginSuccess);
    connect(m_authPage, &AuthPage::loginAdminSuccess,
        this, &MainWindow::onLoginAdminSuccess);

    m_rootStack->addWidget(m_authPage);   // index 0
    m_rootStack->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    tearDownShell();
    // m_allUsers array is owned by whoever last passed it to us
    if (m_allUsers) {
        for (int i = 0; i < m_userCount; ++i) delete m_allUsers[i];
        delete[] m_allUsers;
    }
}

// ── Shell builders ────────────────────────────────────────────────────────────
void MainWindow::buildSidebar() {
    m_sidebar = new QWidget;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(200);

    auto* sl = new QVBoxLayout(m_sidebar);
    sl->setContentsMargins(10, 20, 10, 16);
    sl->setSpacing(4);

    // Brand
    auto* brand = new QLabel(APP_NAME);
    brand->setObjectName("brandTitle");
    brand->setAlignment(Qt::AlignCenter);
    brand->setFixedHeight(44);
    sl->addWidget(brand);
    sl->addSpacing(12);

    // Section label helper
    auto addSection = [&](const QString& text) {
        auto* lbl = new QLabel(text);
        lbl->setObjectName("sectionLbl");
        sl->addSpacing(8);
        sl->addWidget(lbl);
        };

    addSection("MAIN");
    m_btnFeed = new SidebarButton("📰", "Feed");
    m_btnCreatePost = new SidebarButton("✏️", "Create Post");
    m_btnNotifications = new SidebarButton("🔔", "Notifications");
    m_btnSearch = new SidebarButton("🔍", "Search");
    m_btnMessages = new SidebarButton("💬", "Messages");
    sl->addWidget(m_btnFeed);
    sl->addWidget(m_btnCreatePost);
    sl->addWidget(m_btnNotifications);
    sl->addWidget(m_btnSearch);
    sl->addWidget(m_btnMessages);

    addSection("YOU");
    m_btnProfile = new SidebarButton("👤", "Profile");
    m_btnTimeSpent = new SidebarButton("⏱", "Time Spent");
    sl->addWidget(m_btnProfile);
    sl->addWidget(m_btnTimeSpent);

    sl->addStretch(1);

    m_btnLogout = new SidebarButton("⬅", "Logout");
    m_btnLogout->setObjectName("sidebarBtnDanger");
    m_btnLogout->setFixedHeight(44);
    sl->addWidget(m_btnLogout);

    connect(m_btnFeed, &QPushButton::clicked, this, &MainWindow::onNavFeed);
    connect(m_btnCreatePost, &QPushButton::clicked, this, &MainWindow::onSidebarCreatePost);
    connect(m_btnNotifications, &QPushButton::clicked, this, &MainWindow::onNavNotifications);
    connect(m_btnSearch, &QPushButton::clicked, this, &MainWindow::onNavSearch);
    connect(m_btnMessages, &QPushButton::clicked, this, &MainWindow::onNavMessages);
    connect(m_btnProfile, &QPushButton::clicked, this, &MainWindow::onNavProfile);
    connect(m_btnTimeSpent, &QPushButton::clicked, this, &MainWindow::onNavTimeSpent);
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
}

void MainWindow::buildPages() {
    m_pages = new QStackedWidget;

    // 0 — Feed
    m_feedPage = new FeedPage(m_currentUser, m_allUsers, m_userCount);
    connect(m_feedPage, &FeedPage::requestOpenComments,
        this, &MainWindow::onOpenComments);
    m_pages->addWidget(m_feedPage);

    // 1 — Notifications
    m_notifPage = new NotificationsPage(m_currentUser);
    m_pages->addWidget(m_notifPage);

    // 2 — Search
    m_searchPage = new SearchPage(m_allUsers, m_userCount, m_currentUser);
    connect(m_searchPage, &SearchPage::requestOpenComments,
        this, &MainWindow::onOpenComments);
    connect(m_searchPage, &SearchPage::requestViewProfile,
        this, [this](User* target) {
            if (!m_publicProfilePage) {
                m_publicProfilePage = new PublicProfileWidget;
                connect(m_publicProfilePage, &PublicProfileWidget::backClicked,
                    this, [this]() {
                        m_pages->setCurrentIndex(m_previousPageIndex);
                    });
                connect(m_publicProfilePage, &PublicProfileWidget::requestOpenComments,
                    this, &MainWindow::onOpenComments);
                m_pages->addWidget(m_publicProfilePage);
            }
            m_previousPageIndex = m_pages->currentIndex();
            m_publicProfilePage->loadProfile(target, m_currentUser,
                m_allUsers, m_userCount);
            m_pages->setCurrentWidget(m_publicProfilePage);
        });
    m_pages->addWidget(m_searchPage);

    // 3 — Messages
    m_messagesPage = new MessagesPage(m_currentUser, m_allUsers, m_userCount);
    m_pages->addWidget(m_messagesPage);

    // 4 — Profile
    m_profilePage = new ProfilePage(m_currentUser, m_allUsers, &m_userCount);
    connect(m_profilePage, &ProfilePage::accountDeleted,
        this, &MainWindow::onLogout);
    connect(m_profilePage, &ProfilePage::requestOpenComments,
        this, &MainWindow::onOpenComments);
    m_pages->addWidget(m_profilePage);

    // 5 — Time Spent
    m_timeSpentPage = new TimeSpentPage;
    m_timeSpentPage->startSession();
    m_pages->addWidget(m_timeSpentPage);

    // 6 — Create Post (standalone page)
    m_createPostPage = new CreatePostPage(m_currentUser);
    connect(m_createPostPage, &CreatePostPage::postPublished, this, [this]() {
        // After posting, switch to Feed so user sees the result
        onNavFeed();
        });
    m_pages->addWidget(m_createPostPage);

    // 7 — Comments (hidden until triggered)
    m_commentsPage = new CommentsPage(
        m_currentUser ? QString::fromStdString(m_currentUser->getUsername()) : "");
    connect(m_commentsPage, &CommentsPage::backClicked,
        this, [this]() {
            m_pages->setCurrentIndex(m_previousPageIndex);
            // Refresh the originating page
            if (m_previousPageIndex == 0) m_feedPage->refresh();
        });
    m_pages->addWidget(m_commentsPage);
}

void MainWindow::tearDownShell() {
    if (m_timeSpentPage) m_timeSpentPage->stopSession();

    delete m_appShell;
    m_appShell = nullptr;
    m_shellLayout = nullptr;
    m_sidebar = nullptr;
    m_pages = nullptr;
    m_feedPage = nullptr;
    m_notifPage = nullptr;
    m_searchPage = nullptr;
    m_messagesPage = nullptr;
    m_profilePage = nullptr;
    m_timeSpentPage = nullptr;
    m_createPostPage = nullptr;
    m_adminPage = nullptr;
    m_commentsPage = nullptr;
    m_publicProfilePage = nullptr;
    m_btnFeed = m_btnCreatePost = m_btnNotifications = m_btnSearch = m_btnMessages = nullptr;
    m_btnProfile = m_btnTimeSpent = m_btnLogout = nullptr;
}

// ── Login slots ───────────────────────────────────────────────────────────────
void MainWindow::onLoginSuccess(User* user, User** allUsers, int userCount) {
    m_currentUser = user;
    m_allUsers = allUsers;
    m_userCount = userCount;

    buildSidebar();
    buildPages();

    m_appShell = new QWidget;
    m_appShell->setObjectName("appShell");
    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0);
    m_shellLayout->setSpacing(0);
    m_shellLayout->addWidget(m_sidebar);
    m_shellLayout->addWidget(m_pages, 1);

    m_rootStack->addWidget(m_appShell);
    m_rootStack->setCurrentWidget(m_appShell);

    m_btnFeed->setActive(true);
    m_pages->setCurrentIndex(0);
}

void MainWindow::onLoginAdminSuccess() {
    // Build minimal admin shell (no user, no posts)
    m_currentUser = nullptr;

    auto* adminUser = new Admin("admin123", "", "Platform Administrator");

    m_sidebar = new QWidget;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(200);
    auto* sl = new QVBoxLayout(m_sidebar);
    sl->setContentsMargins(10, 20, 10, 16);
    sl->setSpacing(4);

    auto* brand = new QLabel(APP_NAME);
    brand->setObjectName("brandTitle");
    brand->setAlignment(Qt::AlignCenter);
    brand->setFixedHeight(44);
    sl->addWidget(brand);
    sl->addSpacing(12);

    auto* consoleLbl = new QLabel("ADMIN CONSOLE");
    consoleLbl->setObjectName("sectionLbl");
    sl->addWidget(consoleLbl);

    auto* dashBtn = new SidebarButton("🛡", "Dashboard");
    dashBtn->setActive(true);
    sl->addWidget(dashBtn);
    sl->addStretch(1);

    auto* logoutBtn = new SidebarButton("⬅", "Logout");
    logoutBtn->setObjectName("sidebarBtnDanger");
    logoutBtn->setFixedHeight(44);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(logoutBtn);

    m_pages = new QStackedWidget;
    m_adminPage = new AdminPage(adminUser, m_allUsers, m_userCount);
    m_pages->addWidget(m_adminPage);

    m_appShell = new QWidget;
    m_appShell->setObjectName("appShell");
    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0);
    m_shellLayout->setSpacing(0);
    m_shellLayout->addWidget(m_sidebar);
    m_shellLayout->addWidget(m_pages, 1);

    m_rootStack->addWidget(m_appShell);
    m_rootStack->setCurrentWidget(m_appShell);

    connect(dashBtn, &QPushButton::clicked,
        this, [this]() { if (m_adminPage) m_adminPage->refresh(); });
}

// ── Navigation slots ──────────────────────────────────────────────────────────
void MainWindow::setActiveSidebarButton(SidebarButton* active) {
    for (SidebarButton* b : {
        m_btnFeed, m_btnCreatePost, m_btnNotifications, m_btnSearch,
        m_btnMessages, m_btnProfile, m_btnTimeSpent })
    {
        if (b) b->setActive(b == active);
    }
}

void MainWindow::onNavFeed() {
    if (!m_pages) return;
    setActiveSidebarButton(m_btnFeed);
    m_previousPageIndex = 0;
    m_pages->setCurrentIndex(0);
    if (m_feedPage) {
        m_feedPage->m_composerCard->hide();  // never show composer on plain feed nav
        m_feedPage->refresh();
    }
}

void MainWindow::onNavNotifications() {
    if (!m_pages) return;
    setActiveSidebarButton(m_btnNotifications);
    m_previousPageIndex = 1;
    m_pages->setCurrentIndex(1);
    if (m_notifPage) m_notifPage->refresh();
}

void MainWindow::onNavSearch() {
    if (!m_pages) return;
    setActiveSidebarButton(m_btnSearch);
    m_previousPageIndex = 2;
    m_pages->setCurrentIndex(2);
}

void MainWindow::onNavMessages() {
    if (!m_pages) return;
    setActiveSidebarButton(m_btnMessages);
    m_previousPageIndex = 3;
    m_pages->setCurrentIndex(3);
    if (m_messagesPage) m_messagesPage->refresh();
}

void MainWindow::onNavProfile() {
    if (!m_pages) return;
    setActiveSidebarButton(m_btnProfile);
    m_previousPageIndex = 4;
    m_pages->setCurrentIndex(4);
    if (m_profilePage) m_profilePage->refresh();
}

void MainWindow::onNavTimeSpent() {
    if (!m_pages) return;
    setActiveSidebarButton(m_btnTimeSpent);
    m_previousPageIndex = 5;
    m_pages->setCurrentIndex(5);
}

void MainWindow::onSidebarCreatePost() {
    if (!m_pages || !m_createPostPage) return;
    setActiveSidebarButton(m_btnCreatePost);
    m_previousPageIndex = m_pages->currentIndex();
    m_createPostPage->focusInput();
    m_pages->setCurrentWidget(m_createPostPage);
}

void MainWindow::onOpenComments(Posts* post) {
    if (!m_commentsPage || !post) return;
    m_previousPageIndex = m_pages->currentIndex();
    m_commentsPage->loadPost(post);
    m_pages->setCurrentWidget(m_commentsPage);
    setActiveSidebarButton(nullptr);
}

void MainWindow::onLogout() {
    auto r = QMessageBox::question(this, "Logout",
        "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;

    tearDownShell();

    // Remove the shell widget from the root stack
    while (m_rootStack->count() > 1)
        m_rootStack->removeWidget(m_rootStack->widget(1));

    m_currentUser = nullptr;
    if (m_allUsers) {
        for (int i = 0; i < m_userCount; ++i) delete m_allUsers[i];
        delete[] m_allUsers;
        m_allUsers = nullptr;
        m_userCount = 0;
    }

    m_authPage->resetToLogin();
    m_rootStack->setCurrentIndex(0);
}