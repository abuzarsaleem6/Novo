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

static const QString APP_NAME = "Novo";

static const QString APP_STYLE = R"(
* {
    font-family: 'Inter', 'Segoe UI Variable', 'Segoe UI', system-ui, sans-serif;
    color: #C8C8E0;
    box-sizing: border-box;
    outline: none;
}

QMainWindow, QWidget {
    background: #050508;
}

QScrollArea {
    border: none;
    background: transparent;
}

QScrollBar:vertical {
    background: transparent;
    width: 4px;
    margin: 8px 2px;
    border-radius: 2px;
}

QScrollBar::handle:vertical {
    background: #1C1C32;
    border-radius: 2px;
    min-height: 40px;
}

QScrollBar::handle:vertical:hover {
    background: #5B5BD6;
}

QScrollBar::handle:vertical:pressed {
    background: #4444BB;
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    height: 0px;
    background: transparent;
}

QScrollBar::add-page:vertical,
QScrollBar::sub-page:vertical {
    background: transparent;
}

QScrollBar:horizontal {
    height: 0px;
    background: transparent;
}

QWidget#sidebar {
    background: #0A0A14;
    border-right: 1px solid #13132A;
}

QLabel#brandLabel {
    font-size: 22px;
    font-weight: 800;
    color: #7C7CEA;
    letter-spacing: -1px;
    padding: 4px 6px;
}

QFrame#userChip {
    background: #0F0F1E;
    border: 1px solid #1A1A30;
    border-radius: 10px;
}

QPushButton#navBtn {
    background: transparent;
    border: none;
    border-radius: 10px;
    color: #44445A;
    text-align: left;
    font-size: 13px;
    font-weight: 500;
    padding: 0px 12px;
}

QPushButton#navBtn:hover {
    background: #0F0F1E;
    color: #9090C0;
}

QPushButton#navBtn[active="true"] {
    background: #13132A;
    color: #7C7CEA;
    font-weight: 600;
}

QPushButton#navBtnDanger {
    background: transparent;
    border: none;
    border-radius: 10px;
    color: #442828;
    text-align: left;
    font-size: 13px;
    font-weight: 500;
    padding: 0px 12px;
}

QPushButton#navBtnDanger:hover {
    background: #150808;
    color: #CC5555;
}

QPushButton#fabBtn {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 #5B5BD6, stop:1 #7C4FD4);
    border: none;
    border-radius: 14px;
    color: #FFFFFF;
    font-size: 20px;
    font-weight: 700;
}

QPushButton#fabBtn:hover {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
        stop:0 #7070E8, stop:1 #9060E8);
}

QPushButton#fabBtn:pressed {
    background: #4444BB;
}

QWidget#pageArea {
    background: #08080E;
}

QLineEdit, QTextEdit {
    background: #0D0D18;
    border: 1px solid #1A1A30;
    border-radius: 10px;
    padding: 10px 14px;
    color: #E8E8F8;
    font-size: 13px;
    font-weight: 400;
    selection-background-color: #3B3B9E;
}

QLineEdit:focus, QTextEdit:focus {
    border: 1px solid #5B5BD6;
    background: #0F0F20;
}

QLineEdit::placeholder {
    color: #2A2A40;
}

QPushButton#primaryBtn {
    background: #5B5BD6;
    border: none;
    border-radius: 10px;
    color: #FFFFFF;
    font-size: 13px;
    font-weight: 600;
    letter-spacing: 0.2px;
}
QPushButton#primaryBtn:hover   { background: #7070E8; }
QPushButton#primaryBtn:pressed { background: #4444BB; }
QPushButton#primaryBtn:disabled { background: #1C1C32; color: #44445A; }

QPushButton#secondaryBtn {
    background: transparent;
    border: 1px solid #1C1C32;
    border-radius: 10px;
    color: #5858A0;
    font-size: 13px;
    font-weight: 500;
}
QPushButton#secondaryBtn:hover {
    background: #0F0F1E;
    color: #9090D0;
    border-color: #3A3A60;
}
QPushButton#secondaryBtn:pressed {
    background: #13132A;
}

QPushButton#dangerBtn {
    background: #150505;
    border: 1px solid #3A1515;
    border-radius: 10px;
    color: #884444;
    font-size: 13px;
    font-weight: 500;
}
QPushButton#dangerBtn:hover {
    background: #200808;
    color: #EE6666;
    border-color: #661E1E;
}
QPushButton#dangerBtn:pressed {
    background: #2A0A0A;
}

QPushButton#iconBtn {
    background: transparent;
    border: none;
    color: #33334A;
    font-size: 15px;
    border-radius: 8px;
}
QPushButton#iconBtn:hover {
    background: #0F0F1E;
    color: #7070CC;
}

QPushButton#saveBtn {
    background: transparent;
    border: none;
    color: #3A5540;
    font-size: 16px;
    border-radius: 8px;
}
QPushButton#saveBtn:hover { background: #081208; color: #44CC77; }
QPushButton#savedBtn {
    background: #081208;
    border: none;
    color: #44CC77;
    font-size: 16px;
    border-radius: 8px;
}

QPushButton#reportBtn {
    background: transparent;
    border: none;
    color: #4A2828;
    font-size: 13px;
    border-radius: 8px;
}
QPushButton#reportBtn:hover { background: #150505; color: #CC4444; }

QFrame#postCard {
    background: #0D0D18;
    border: 1px solid #161628;
    border-radius: 16px;
}

QFrame#postCard:hover {
    border-color: #222240;
}

QFrame#composerCard {
    background: #0A0A16;
    border: 1px solid #5B5BD6;
    border-radius: 16px;
}

QFrame#authCard {
    background: #0A0A16;
    border: 1px solid #161630;
    border-radius: 20px;
}

QFrame#notifCard {
    background: #0D0D18;
    border: 1px solid #161628;
    border-radius: 12px;
}

QFrame#notifCard[unread="true"] {
    border-left: 3px solid #5B5BD6;
    background: #0F0F20;
}

QFrame#resultCard {
    background: #0D0D18;
    border: 1px solid #161628;
    border-radius: 14px;
}

QLabel#bubbleOwn {
    background: #1E1E50;
    color: #E8E8FF;
    border-radius: 14px 14px 2px 14px;
    padding: 10px 14px;
    font-size: 13px;
    font-weight: 400;
}

QLabel#bubbleOther {
    background: #111120;
    color: #B8B8D8;
    border-radius: 14px 14px 14px 2px;
    padding: 10px 14px;
    font-size: 13px;
    font-weight: 400;
}

QLabel#pageTitle {
    font-size: 22px;
    font-weight: 700;
    color: #F1F1F8;
    letter-spacing: -0.5px;
}

QLabel#sectionLabel {
    font-size: 10px;
    font-weight: 600;
    color: #2A2A44;
    letter-spacing: 1.8px;
}

QLabel#postAuthor {
    font-size: 14px;
    font-weight: 600;
    color: #E0E0F8;
}

QLabel#postHandle {
    font-size: 11px;
    font-weight: 400;
    color: #44445A;
}

QLabel#postContent {
    font-size: 14px;
    font-weight: 400;
    color: #C0C0D8;
    line-height: 1.65;
}

QLabel#postMeta {
    font-size: 11px;
    color: #33334A;
}

QLabel#timerDisplay {
    font-size: 56px;
    font-weight: 800;
    color: #5B5BD6;
    letter-spacing: -3px;
}

QLabel#brandTitle {
    font-size: 36px;
    font-weight: 800;
    color: #7C7CEA;
    letter-spacing: -2px;
}

QLabel#statValue {
    font-size: 24px;
    font-weight: 700;
    color: #5B5BD6;
    letter-spacing: -1px;
}

QLabel#statLabel {
    font-size: 10px;
    font-weight: 600;
    color: #2A2A44;
    letter-spacing: 1.5px;
}

QLabel#badgeUnread {
    background: #5B5BD6;
    color: #FFFFFF;
    border-radius: 8px;
    font-size: 10px;
    font-weight: 700;
    padding: 1px 6px;
}

QWidget#msgLeftPanel {
    background: #080812;
    border-right: 1px solid #111122;
}

QFrame#msgHeader {
    background: #080812;
    border-bottom: 1px solid #111122;
}

QFrame#chatInputBar {
    background: #080812;
    border-top: 1px solid #111122;
}

QDialog {
    background: #08080E;
    border: 1px solid #1A1A30;
    border-radius: 16px;
}

QMessageBox {
    background: #08080E;
}

QMessageBox QLabel {
    color: #C8C8E0;
    font-size: 13px;
}

QMessageBox QPushButton {
    background: #0D0D18;
    border: 1px solid #1C1C32;
    border-radius: 8px;
    color: #8888AA;
    min-width: 80px;
    padding: 6px 14px;
    font-size: 12px;
}

QMessageBox QPushButton:hover {
    background: #13132A;
    color: #AAAACC;
}

QStackedWidget {
    background: transparent;
}
)";

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
        f->setStyleSheet("background: #0F0F1E; border: none;");
        return f;
    }

    QLabel* avatar(const QString& initial, int size, QWidget* p = nullptr) {
        auto* l = new QLabel(initial.isEmpty() ? "?" : initial.left(1).toUpper(), p);
        l->setFixedSize(size, size);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(QString(
            "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
            "stop:0 #2A2A6A, stop:1 #1A1A44);"
            "border-radius: %1px;"
            "font-size: %2px;"
            "font-weight: 700;"
            "color: #9090EE;"
            "border: 1px solid #2A2A50;")
            .arg(size / 2).arg(qMax(11, size / 3)));
        return l;
    }

    QLabel* sectionLabel(const QString& text, QWidget* p = nullptr) {
        auto* l = new QLabel(text.toUpper(), p);
        l->setStyleSheet("font-size:10px; font-weight:600; color:#2A2A44; letter-spacing:1.8px;");
        return l;
    }

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
        outLayout->addStretch(1);

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
    setAttribute(Qt::WA_Hover, true);
    setObjectName("navBtn");
    setFixedHeight(46);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setText(icon + "   " + label);
    setCursor(Qt::PointingHandCursor);
    setProperty("active", false);
}

void SidebarButton::setActive(bool active) {
    m_active = active;
    setProperty("active", active);
    update();
}

void SidebarButton::setNotifCount(int count) {
    m_notifCount = count;
    m_hasNotif = (count > 0);
    update();
}

void SidebarButton::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();

    // Background
    if (m_active) {
        p.fillRect(r, QColor("#13132A"));
    }
    else if (underMouse()) {
        p.fillRect(r, QColor("#0F0F1E"));
    }
    else {
        p.fillRect(r, QColor("#0A0A14"));
    }

    // Left accent bar
    if (m_active) {
        QRect bar(0, 6, 3, height() - 12);
        p.fillRect(bar, QColor("#5B5BD6"));
    }

    // Parse icon and label
    QString txt = text();
    QString iconStr;
    QString labelStr;
    int split = txt.indexOf("   ");
    if (split >= 0) {
        iconStr = txt.left(split);
        labelStr = txt.mid(split + 3);
    }
    else {
        labelStr = txt;
    }

    int x = 14;

    // Icon
    if (!iconStr.isEmpty()) {
        p.setPen(m_active ? QColor("#7C7CEA") : (underMouse() ? QColor("#8888BB") : QColor("#44445A")));
        QFont iconFont("Segoe UI Emoji", 18);
        p.setFont(iconFont);
        QRect iconRect(x, 0, 24, height());
        p.drawText(iconRect, Qt::AlignVCenter | Qt::AlignLeft, iconStr);
        x += 28;
    }

    // Label
    p.setPen(m_active ? QColor("#7C7CEA") : (underMouse() ? QColor("#8888BB") : QColor("#44445A")));
    QFont textFont("Inter", 13, m_active ? QFont::DemiBold : QFont::Medium);
    p.setFont(textFont);
    QRect textRect(x, 0, width() - x - 10, height());
    p.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, labelStr);

    // Notification badge
    if (m_hasNotif && m_notifCount > 0) {
        int badgeSize = 18;
        QRect badgeRect(r.right() - badgeSize - 10, r.center().y() - badgeSize / 2, badgeSize, badgeSize);
        p.setBrush(QColor("#5B5BD6"));
        p.setPen(Qt::NoPen);
        p.drawEllipse(badgeRect);
        p.setPen(QColor("#FFFFFF"));
        QFont badgeFont("Inter", 8, QFont::Bold);
        p.setFont(badgeFont);
        p.drawText(badgeRect, Qt::AlignCenter, QString::number(m_notifCount));
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  PostCard
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

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(12);

    // ── Header row ──────────────────────────────────────────────────────────
    auto* headerRow = new QHBoxLayout;
    headerRow->setSpacing(12);
    headerRow->setContentsMargins(0, 0, 0, 0);

    QString ini = m_authorUsername.isEmpty() ? "?" : m_authorUsername.left(1).toUpper();
    headerRow->addWidget(avatar(ini, 44), 0, Qt::AlignVCenter);

    auto* metaCol = new QVBoxLayout;
    metaCol->setSpacing(2);
    metaCol->setContentsMargins(0, 0, 0, 0);
    auto* authorLbl = makeLabel(m_authorUsername, "postAuthor");
    authorLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto* handleLbl = makeLabel("@" + m_authorUsername, "postHandle");
    metaCol->addWidget(authorLbl);
    metaCol->addWidget(handleLbl);
    headerRow->addLayout(metaCol, 1);

    QString timeStr = QString::fromStdString(post->getTimeOfCreation());
    if (timeStr.isEmpty())
        timeStr = "Post " + QString::fromStdString(post->getPostId());
    auto* timeRightLbl = makeLabel(timeStr, "postMeta");
    timeRightLbl->setAlignment(Qt::AlignRight);
    timeRightLbl->setFixedWidth(80);
    headerRow->addWidget(timeRightLbl, 0, Qt::AlignVCenter);

    if (m_isOwner) {
        auto* editBtn = new QPushButton("✎");
        editBtn->setFixedSize(28, 28);
        editBtn->setObjectName("iconBtn");
        editBtn->setCursor(Qt::PointingHandCursor);
        editBtn->setStyleSheet(
            "QPushButton{background:transparent;border:none;color:#33334A;font-size:16px;border-radius:6px;}"
            "QPushButton:hover{background:#0F0F1E;color:#AAAAFF;}");
        connect(editBtn, &QPushButton::clicked, this,
            [this]() { emit editClicked(m_post); });
        headerRow->addWidget(editBtn, 0, Qt::AlignVCenter);

        auto* delBtn = new QPushButton("🗑");
        delBtn->setFixedSize(28, 28);
        delBtn->setCursor(Qt::PointingHandCursor);
        delBtn->setStyleSheet(
            "QPushButton { background: transparent; border: none; color: #33334A; font-size: 18px; border-radius: 6px; }"
            "QPushButton:hover { background: rgba(255, 68, 68, 0.08); color: #FF4444; }");
        connect(delBtn, &QPushButton::clicked, this,
            [this]() { emit deleteClicked(m_post); });
        headerRow->addWidget(delBtn, 0, Qt::AlignVCenter);
    }

    root->addLayout(headerRow);

    // ── Content ─────────────────────────────────────────────────────────────
    auto* contentLbl = makeLabel(QString::fromStdString(post->getContent()), "postContent");
    contentLbl->setWordWrap(true);
    contentLbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    contentLbl->setStyleSheet("color: #C8C8E0; font-size: 14px; font-weight: 400; line-height: 1.65; padding: 12px 0px 8px 0px; letter-spacing: 0.1px;");
    root->addWidget(contentLbl);

    // ── Divider ─────────────────────────────────────────────────────────────
    auto* div = new QFrame;
    div->setFrameShape(QFrame::HLine);
    div->setFixedHeight(1);
    div->setStyleSheet("background: #111125; border: none; margin: 8px 0;");
    root->addWidget(div);

    // ── Action row ──────────────────────────────────────────────────────────
    auto* actRow = new QHBoxLayout;
    actRow->setContentsMargins(0, 4, 0, 0);
    actRow->setSpacing(28);

    // ── Like button ─────────────────────────────────────────────────────────
    bool alreadyLiked = m_post->isLikedBy(m_viewerUsername.toStdString());

    auto* likeBtn = new QPushButton(alreadyLiked ? "♥" : "♡");
    likeBtn->setFixedSize(36, 36);
    likeBtn->setCursor(Qt::PointingHandCursor);
    likeBtn->setStyleSheet(alreadyLiked
        ? "QPushButton { background: transparent; border: none; color: #EE4444; font-size: 19px; }"
        : "QPushButton { background: transparent; border: none; color: #33334A; font-size: 19px; }"
        "QPushButton:hover { color: #FF5555; }");

    auto* likeCountLbl = new QLabel(QString::number(m_post->getLikeCount()));
    likeCountLbl->setStyleSheet(alreadyLiked
        ? "color: #EE4444; font-size: 13px; font-weight: bold;"
        : "color: #33334A; font-size: 13px; font-weight: bold;");

    connect(likeBtn, &QPushButton::clicked, this, [this, likeBtn, likeCountLbl]() {
        m_post->likePost(m_viewerUsername.toStdString());
        int realCount = m_post->getLikeCount();
        bool nowLiked = m_post->isLikedBy(m_viewerUsername.toStdString());

        likeCountLbl->setText(QString::number(realCount));

        if (nowLiked) {
            likeBtn->setText("♥");
            likeBtn->setStyleSheet(
                "QPushButton { background: transparent; border: none; color: #EE4444; font-size: 19px; }");
            likeCountLbl->setStyleSheet("color: #EE4444; font-size: 13px; font-weight: bold;");

            // Pop animation
            QFont f = likeBtn->font();
            f.setPointSize(24);
            likeBtn->setFont(f);
            QTimer::singleShot(180, likeBtn, [likeBtn]() {
                QFont f2 = likeBtn->font();
                f2.setPointSize(19);
                likeBtn->setFont(f2);
                });
        }
        else {
            likeBtn->setText("♡");
            likeBtn->setStyleSheet(
                "QPushButton { background: transparent; border: none; color: #33334A; font-size: 19px; }"
                "QPushButton:hover { color: #FF5555; }");
            likeCountLbl->setStyleSheet("color: #33334A; font-size: 13px; font-weight: bold;");
        }

        emit likeClicked(m_post);
        });

    auto* likeLayout = new QHBoxLayout;
    likeLayout->setSpacing(5);
    likeLayout->addWidget(likeBtn);
    likeLayout->addWidget(likeCountLbl);
    actRow->addLayout(likeLayout);

    // ── Comment button ───────────────────────────────────────────────────────
    auto* commentBtn = new QPushButton("💬");
    commentBtn->setFixedSize(36, 36);
    commentBtn->setCursor(Qt::PointingHandCursor);
    commentBtn->setStyleSheet(
        "QPushButton { background: transparent; border: none; color: #33334A; font-size: 18px; }"
        "QPushButton:hover { color: #5B5BD6; }");
    connect(commentBtn, &QPushButton::clicked, this,
        [this]() { emit commentClicked(m_post); });

    auto* commentCountLbl = new QLabel(QString::number(m_post->getComments().size()));
    commentCountLbl->setStyleSheet("color: #33334A; font-size: 13px; font-weight: bold;");

    auto* commentLayout = new QHBoxLayout;
    commentLayout->setSpacing(5);
    commentLayout->addWidget(commentBtn);
    commentLayout->addWidget(commentCountLbl);
    actRow->addLayout(commentLayout);

    if (!m_isOwner) {
        // ── Save button ──────────────────────────────────────────────────────
        auto* saveCol = new QVBoxLayout;
        saveCol->setSpacing(2);
        saveCol->setAlignment(Qt::AlignCenter);
        auto* saveBtn = new QPushButton(m_isSaved ? "★" : "☆");
        saveBtn->setObjectName(m_isSaved ? "savedBtn" : "saveBtn");
        saveBtn->setCursor(Qt::PointingHandCursor);
        saveBtn->setFixedSize(24, 24);
        auto* saveLbl = new QLabel(m_isSaved ? "Saved" : "Save");
        saveLbl->setStyleSheet("color: #F0A429; font-size: 10px;");
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
            saveBtn->setObjectName(m_isSaved ? "savedBtn" : "saveBtn");
            saveLbl->setText(m_isSaved ? "Saved" : "Save");
            if (saveBtn->style()) {
                saveBtn->style()->unpolish(saveBtn);
                saveBtn->style()->polish(saveBtn);
            }
            });
        actRow->addLayout(saveCol);

        // ── Report post button ───────────────────────────────────────────────
        auto* reportCol = new QVBoxLayout;
        reportCol->setSpacing(2);
        reportCol->setAlignment(Qt::AlignCenter);
        auto* reportBtn = new QPushButton("⚑");
        reportBtn->setObjectName("reportBtn");
        reportBtn->setCursor(Qt::PointingHandCursor);
        reportBtn->setFixedSize(24, 24);
        auto* reportLbl = new QLabel("Report");
        reportLbl->setStyleSheet("color: #F0A429; font-size: 10px;");
        reportLbl->setAlignment(Qt::AlignCenter);
        reportCol->addWidget(reportBtn, 0, Qt::AlignCenter);
        reportCol->addWidget(reportLbl, 0, Qt::AlignCenter);
        connect(reportBtn, &QPushButton::clicked, this,
            [this]() { emit reportClicked(m_post, m_authorUsername); });
        actRow->addLayout(reportCol);

        // ── Report user button ───────────────────────────────────────────────
        auto* reportUserCol = new QVBoxLayout;
        reportUserCol->setSpacing(2);
        reportUserCol->setAlignment(Qt::AlignCenter);
        auto* reportUserBtn = new QPushButton("🚫");
        reportUserBtn->setObjectName("reportBtn");
        reportUserBtn->setCursor(Qt::PointingHandCursor);
        reportUserBtn->setFixedSize(24, 24);
        auto* reportUserLbl = new QLabel("Report User");
        reportUserLbl->setStyleSheet("color: #F0A429; font-size: 10px;");
        reportUserLbl->setAlignment(Qt::AlignCenter);
        reportUserCol->addWidget(reportUserBtn, 0, Qt::AlignCenter);
        reportUserCol->addWidget(reportUserLbl, 0, Qt::AlignCenter);
        connect(reportUserBtn, &QPushButton::clicked, this,
            [this]() { emit reportUserClicked(m_authorUsername); });
        actRow->addLayout(reportUserCol);
    }

    // ── Share button (right-aligned) ─────────────────────────────────────────
    actRow->addStretch(1);
    auto* shareCol = new QVBoxLayout;
    shareCol->setSpacing(2);
    shareCol->setAlignment(Qt::AlignCenter);
    auto* shareBtn = makeIcon("↗", nullptr, 24, 24);
    auto* shareLbl = new QLabel("Share");
    shareLbl->setStyleSheet("color: #F0A429; font-size: 10px;");
    shareLbl->setAlignment(Qt::AlignCenter);
    shareCol->addWidget(shareBtn, 0, Qt::AlignCenter);
    shareCol->addWidget(shareLbl, 0, Qt::AlignCenter);
    connect(shareBtn, &QPushButton::clicked, this, [this]() {
        QApplication::clipboard()->setText(QString::fromStdString(m_post->getPostId()));
        QToolTip::showText(QCursor::pos(), "Copied!", this);
        });
    actRow->addLayout(shareCol);

    root->addLayout(actRow);
}

// ══════════════════════════════════════════════════════════════════════════════
//  CommentDialog
// ══════════════════════════════════════════════════════════════════════════════
CommentDialog::CommentDialog(Posts* post, const QString& currentUser, QWidget* parent)
    : QDialog(parent), m_post(post), m_currentUser(currentUser)
{
    setWindowTitle("Comments");
    setFixedSize(480, 580);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto* rootFrame = new QFrame;
    rootFrame->setObjectName("authCard");
    auto* root = new QVBoxLayout(rootFrame);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(16);

    // Header
    auto* headerRow = new QHBoxLayout;
    auto* title = new QLabel("Comments");
    title->setStyleSheet("font-size: 17px; font-weight: 700; color: #F1F1F8;");
    headerRow->addWidget(title);
    headerRow->addStretch(1);
    auto* closeBtn = makeIcon("✕", nullptr, 28, 28);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    headerRow->addWidget(closeBtn);
    root->addLayout(headerRow);

    root->addWidget(divider());

    // Scroll Area for comments
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

    // Input Area
    auto* inputRow = new QHBoxLayout;
    m_input = new QLineEdit;
    m_input->setPlaceholderText("Write a comment...");
    m_input->setFixedHeight(42);

    auto* sendBtn = new QPushButton("Post");
    sendBtn->setObjectName("primaryBtn");
    sendBtn->setFixedSize(72, 38);
    sendBtn->setCursor(Qt::PointingHandCursor);

    connect(sendBtn, &QPushButton::clicked, this, &CommentDialog::onAddComment);
    connect(m_input, &QLineEdit::returnPressed, this, &CommentDialog::onAddComment);

    inputRow->addWidget(m_input, 1);
    inputRow->addWidget(sendBtn);
    root->addLayout(inputRow);

    mainLayout->addWidget(rootFrame);

    loadComments();
}

void CommentDialog::loadComments() {
    while (m_commentsLayout->count() > 1) {
        QLayoutItem* item = m_commentsLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    QList<Comment> comments = m_post->getComments();

    if (comments.isEmpty()) {
        auto* empty = new QLabel("✦ No comments yet");
        empty->setStyleSheet("color: #1C1C30; font-size: 14px;");
        empty->setAlignment(Qt::AlignCenter);
        m_commentsLayout->insertWidget(0, empty);
        return;
    }

    for (int i = 0; i < comments.size(); ++i) {
        const Comment& c = comments[i];
        bool isMine = (c.getCreatorUsername() == m_currentUser);

        auto* card = new QFrame;
        card->setStyleSheet("QFrame { background: #141420; border-radius: 10px; padding: 10px; }");
        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(0, 0, 0, 0);
        cl->setSpacing(4);

        auto* headerRow = new QHBoxLayout;
        auto* nameLbl = new QLabel(c.getCreatorUsername());
        nameLbl->setStyleSheet("font-weight: 600; color: #9090CC; font-size: 12px;");

        auto* timeLbl = new QLabel(c.getTimeOfCreation());
        timeLbl->setStyleSheet("color: #22223A; font-size: 10px;");

        headerRow->addWidget(nameLbl);
        headerRow->addWidget(timeLbl);
        headerRow->addStretch(1);

        if (isMine) {
            auto* editBtn = new QPushButton("✎");
            editBtn->setFixedSize(22, 22);
            editBtn->setCursor(Qt::PointingHandCursor);
            editBtn->setStyleSheet("QPushButton { background: transparent; border: none; color: #33334A; font-size: 12px; } QPushButton:hover { color: #AAAAFF; }");

            auto* delBtn = new QPushButton("🗑");
            delBtn->setFixedSize(22, 22);
            delBtn->setCursor(Qt::PointingHandCursor);
            delBtn->setStyleSheet("QPushButton { background: transparent; border: none; color: #33334A; font-size: 12px; } QPushButton:hover { color: #FFAAAA; }");

            connect(editBtn, &QPushButton::clicked, this, [this, i]() { onEditComment(i); });
            connect(delBtn, &QPushButton::clicked, this, [this, i]() { onDeleteComment(i); });

            headerRow->addWidget(editBtn);
            headerRow->addWidget(delBtn);
        }

        cl->addLayout(headerRow);

        auto* contentLbl = new QLabel(c.getContent());
        contentLbl->setWordWrap(true);
        contentLbl->setStyleSheet("color: #C0C0D8; font-size: 13px;");
        cl->addWidget(contentLbl);

        m_commentsLayout->insertWidget(m_commentsLayout->count() - 1, card);
    }
}

void CommentDialog::onAddComment() {
    QString text = m_input->text().trimmed();
    if (text.isEmpty()) return;
    if (text.contains('|')) {
        QMessageBox::warning(this, "Novo", "Comments cannot contain '|'.");
        return;
    }

    m_post->addComment(text, m_currentUser);
    m_input->clear();
    loadComments();
}

void CommentDialog::onEditComment(int index) {
    QString oldText = m_post->getComments()[index].getContent();
    bool ok;
    QString newText = QInputDialog::getText(this, "Edit Comment", "Update your comment:", QLineEdit::Normal, oldText, &ok);

    if (ok && !newText.trimmed().isEmpty()) {
        if (newText.contains('|')) {
            QMessageBox::warning(this, "Novo", "Comments cannot contain '|'.");
            return;
        }
        m_post->editComment(index, newText.trimmed(), m_currentUser);
        loadComments();
    }
}

void CommentDialog::onDeleteComment(int index) {
    auto r = QMessageBox::question(this, "Delete Comment", "Delete this comment?", QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        m_post->deleteComment(index, m_currentUser);
        loadComments();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  PublicProfileWidget
// ══════════════════════════════════════════════════════════════════════════════
PublicProfileWidget::PublicProfileWidget(QWidget* parent) : QWidget(parent) {
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
}

void PublicProfileWidget::loadProfile(User* targetUser, User* viewer) {
    QLayoutItem* item;
    while ((item = m_mainLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    // --- Back Button Header ---
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

    // --- Scrollable Profile Content ---
    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background: transparent; border: none;");

    auto* container = new QWidget;
    auto* layout = new QVBoxLayout(container);
    layout->setContentsMargins(28, 16, 28, 24);
    layout->setSpacing(20);

    // 1. PROFILE HEADER CARD
    auto* headerCard = new QFrame;
    headerCard->setObjectName("postCard");
    auto* hLayout = new QVBoxLayout(headerCard);

    auto* row = new QHBoxLayout;
    QString ini = QString::fromStdString(targetUser->getUsername()).left(1).toUpper();
    auto* av = new QLabel(ini);
    av->setFixedSize(60, 60);
    av->setAlignment(Qt::AlignCenter);
    av->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #2A2A6A, stop:1 #1A1A44); border-radius: 30px; font-size: 24px; font-weight: bold; color: #9090EE; border: 1px solid #2A2A50;");
    row->addWidget(av);

    auto* nameCol = new QVBoxLayout;
    auto* nameLbl = new QLabel(QString::fromStdString(targetUser->getUsername()));
    nameLbl->setStyleSheet("font-size: 20px; font-weight: bold; color: #E0E0FF;");
    auto* bioLbl = new QLabel(QString::fromStdString(targetUser->getBio()));
    bioLbl->setStyleSheet("font-size: 13px; color: #56567A;");
    bioLbl->setWordWrap(true);
    nameCol->addWidget(nameLbl);
    nameCol->addWidget(bioLbl);
    row->addLayout(nameCol, 1);
    hLayout->addLayout(row);

    auto* statsRow = new QHBoxLayout;
    auto addStat = [&](const QString& val, const QString& lbl) {
        auto* c = new QVBoxLayout;
        auto* v = new QLabel(val); v->setStyleSheet("font-size: 18px; font-weight: bold; color: #5B5BD6;"); v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl); l->setStyleSheet("font-size: 10px; color: #36365A; letter-spacing: 1px;"); l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l);
        statsRow->addLayout(c);
        };
    addStat(QString::number(targetUser->getPostCount()), "POSTS");
    addStat(QString::number(targetUser->getFollowersCount()), "FOLLOWERS");
    addStat(QString::number(targetUser->getFollowingCount()), "FOLLOWING");
    hLayout->addLayout(statsRow);
    layout->addWidget(headerCard);

    // 2. USERS POSTS FEED
    auto* postsTitle = new QLabel("Recent Posts");
    postsTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #E8E8F8;");
    layout->addWidget(postsTitle);

    targetUser->loadAllPosts();
    if (targetUser->getPostCount() == 0) {
        auto* empty = new QLabel("This user hasn't posted anything yet.");
        empty->setStyleSheet("color: #56567A; font-size: 13px;");
        empty->setAlignment(Qt::AlignCenter);
        layout->addWidget(empty);
    }
    else {
        for (int i = targetUser->getPostCount() - 1; i >= 0; --i) {
            Posts* p = targetUser->getPostByIndex(i);
            if (!p || !p->isValid()) continue;

            bool isSaved = false;
            for (int s = 0; s < viewer->getSavedPostCount(); ++s) {
                if (viewer->getSavedPostByIndex(s) && viewer->getSavedPostByIndex(s)->getPostId() == p->getPostId()) {
                    isSaved = true; break;
                }
            }

            auto* card = new PostCard(p, QString::fromStdString(targetUser->getUsername()), false, isSaved, QString::fromStdString(viewer->getUsername()));

            connect(card, &PostCard::commentClicked, this, [p, viewer, this]() {
                CommentDialog dlg(p, QString::fromStdString(viewer->getUsername()), this);
                dlg.exec();
                });
            connect(card, &PostCard::saveClicked, this, [viewer, targetUser](Posts* post) {
                viewer->savePost(post->getPostId(), targetUser);
                });
            connect(card, &PostCard::unsaveClicked, this, [viewer](Posts* post) {
                viewer->unsavePost(post->getPostId());
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
    setObjectName("notifCard");
    setProperty("unread", !notif.getStatus());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto* row = new QHBoxLayout(this);
    row->setContentsMargins(16, 14, 16, 14);
    row->setSpacing(12);

    QString typeIcon = "●";
    QString bgColor = "#0A0A14";
    QString iconColor = "#44445A";
    QString t = QString::fromStdString(notif.getType()).toLower();
    if (t.contains("follow")) { typeIcon = "👤"; bgColor = "#0A1A0A"; iconColor = "#44CC77"; }
    else if (t.contains("like")) { typeIcon = "♥"; bgColor = "#1A0A0A"; iconColor = "#EE4444"; }
    else if (t.contains("comment")) { typeIcon = "💬"; bgColor = "#0A0A1A"; iconColor = "#5B5BD6"; }
    else if (t.contains("message")) { typeIcon = "✉"; bgColor = "#0A0A14"; iconColor = "#F0A429"; }
    else if (t.contains("report")) { typeIcon = "⚑"; bgColor = "#1A0808"; iconColor = "#EE5555"; }

    auto* ic = new QLabel(typeIcon);
    ic->setFixedSize(36, 36);
    ic->setAlignment(Qt::AlignCenter);
    ic->setStyleSheet(QString("background: %1; color: %2; border-radius: 18px; font-size: 16px;").arg(bgColor, iconColor));

    auto* col = new QVBoxLayout;
    col->setSpacing(3);
    auto* msg = makeLabel(QString::fromStdString(notif.getMessage()));
    msg->setStyleSheet("font-size: 13px; font-weight: 500; color: #C0C0D8;");
    auto* ts = makeLabel(QString::fromStdString(notif.getTimestamp()), "postMeta");
    col->addWidget(msg);
    col->addWidget(ts);

    row->addWidget(ic, 0, Qt::AlignVCenter);
    row->addLayout(col, 1);

    if (!notif.getStatus()) {
        auto* dot = new QLabel("●");
        dot->setStyleSheet("color: #5B5BD6; font-size: 9px;");
        dot->setFixedWidth(12);
        row->addWidget(dot, 0, Qt::AlignVCenter);
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  AuthPage
// ══════════════════════════════════════════════════════════════════════════════
AuthPage::AuthPage(QWidget* parent)
    : QWidget(parent), m_userCount(0)
{
    setObjectName("authBg");
    m_allUsers = new User * [500]();

    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);
    outer->setContentsMargins(20, 20, 20, 20);

    auto* card = new QFrame;
    card->setObjectName("authCard");
    card->setFixedWidth(420);
    card->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(40, 36, 40, 36);
    cl->setSpacing(14);

    // Logo area
    auto* logoRow = new QHBoxLayout;
    logoRow->setSpacing(12);
    auto* logoCircle = new QLabel("N");
    logoCircle->setFixedSize(52, 52);
    logoCircle->setAlignment(Qt::AlignCenter);
    logoCircle->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #5B5BD6, stop:1 #7C4FD4); border-radius: 26px; font-size: 22px; font-weight: 800; color: #FFFFFF;");
    auto* brandTitle = makeLabel(APP_NAME, "brandTitle");
    auto* brandSub = makeLabel("Connect. Share. Discover.", "sectionLabel");
    brandSub->setStyleSheet("font-size: 12px; color: #2A2A44;");
    logoRow->addWidget(logoCircle);
    auto* brandCol = new QVBoxLayout;
    brandCol->addWidget(brandTitle);
    brandCol->addWidget(brandSub);
    logoRow->addLayout(brandCol, 1);
    cl->addLayout(logoRow);
    cl->addSpacing(32);

    m_stack = new QStackedWidget;
    m_stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_stack->addWidget(createLandingWidget());   // 0
    m_stack->addWidget(createLoginWidget());     // 1
    m_stack->addWidget(createSignupWidget());    // 2
    m_stack->addWidget(createAdminLoginWidget());// 3
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
    m_stack->setCurrentIndex(0);
    m_userCount = 0;
    loadAllUsers(m_allUsers, m_userCount);
}

QWidget* AuthPage::createLandingWidget() {
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout(w);
    vl->setSpacing(12);
    vl->setContentsMargins(0, 0, 0, 0);

    auto* btnLogin = makePrimary("Login as User");    btnLogin->setFixedHeight(48);
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
    vl->addWidget(makeLabel("User Login", "sectionLabel"));

    m_loginUser = new QLineEdit; m_loginUser->setPlaceholderText("Username"); m_loginUser->setFixedHeight(48);
    m_loginPass = new QLineEdit; m_loginPass->setPlaceholderText("Password"); m_loginPass->setFixedHeight(48);
    m_loginPass->setEchoMode(QLineEdit::Password);

    auto* userRow = new QHBoxLayout;
    userRow->setSpacing(10);
    auto* userIcon = new QLabel("👤");
    userIcon->setStyleSheet("font-size: 18px; color: #2A2A44;");
    userRow->addWidget(userIcon);
    userRow->addWidget(m_loginUser, 1);

    auto* passRow = new QHBoxLayout;
    passRow->setSpacing(10);
    auto* passIcon = new QLabel("🔒");
    passIcon->setStyleSheet("font-size: 18px; color: #2A2A44;");
    passRow->addWidget(passIcon);
    passRow->addWidget(m_loginPass, 1);

    vl->addLayout(userRow);
    vl->addLayout(passRow);

    auto* loginBtn = makePrimary("Sign In"); loginBtn->setFixedHeight(48);
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
    vl->addWidget(makeLabel("Create Account", "sectionLabel"));

    m_signupUser = new QLineEdit; m_signupUser->setPlaceholderText("Username (6–16 chars)"); m_signupUser->setFixedHeight(48);
    m_signupPass = new QLineEdit; m_signupPass->setPlaceholderText("Password (8+ chars, upper, digit, symbol)"); m_signupPass->setFixedHeight(48);
    m_signupPass->setEchoMode(QLineEdit::Password);
    m_signupBio = new QLineEdit; m_signupBio->setPlaceholderText("Short bio (optional)"); m_signupBio->setFixedHeight(48);

    auto* addRow = [&](const QString& icon, QLineEdit* edit) {
        auto* row = new QHBoxLayout;
        row->setSpacing(10);
        auto* ic = new QLabel(icon);
        ic->setStyleSheet("font-size: 18px; color: #2A2A44;");
        row->addWidget(ic);
        row->addWidget(edit, 1);
        return row;
        };

    vl->addLayout(addRow("👤", m_signupUser));
    vl->addLayout(addRow("🔒", m_signupPass));
    vl->addLayout(addRow("✏", m_signupBio));

    auto* signupBtn = makePrimary("Register"); signupBtn->setFixedHeight(48);
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
    vl->addWidget(makeLabel("Admin Console", "sectionLabel"));

    auto* adminUser = new QLineEdit;
    adminUser->setPlaceholderText("Admin Username");
    adminUser->setFixedHeight(48);

    auto* adminPass = new QLineEdit;
    adminPass->setPlaceholderText("Admin Password");
    adminPass->setFixedHeight(48);
    adminPass->setEchoMode(QLineEdit::Password);

    auto* addRow = [&](const QString& icon, QLineEdit* edit) {
        auto* row = new QHBoxLayout;
        row->setSpacing(10);
        auto* ic = new QLabel(icon);
        ic->setStyleSheet("font-size: 18px; color: #2A2A44;");
        row->addWidget(ic);
        row->addWidget(edit, 1);
        return row;
        };

    vl->addLayout(addRow("👤", adminUser));
    vl->addLayout(addRow("🔒", adminPass));

    auto* loginBtn = makeDanger("Enter System");
    loginBtn->setFixedHeight(48);

    connect(loginBtn, &QPushButton::clicked, this, [this, adminUser, adminPass]() {
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
//  FeedPage
// ══════════════════════════════════════════════════════════════════════════════
FeedPage::FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCount(userCount)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    // ── Header ──────────────────────────────────────────────────────────────
    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Feed", "pageTitle"));

    // Live indicator
    auto* liveDot = new QLabel;
    liveDot->setFixedSize(8, 8);
    liveDot->setStyleSheet("background: #44CC88; border-radius: 4px;");
    auto* opacity = new QGraphicsOpacityEffect(liveDot);
    opacity->setOpacity(1.0);
    liveDot->setGraphicsEffect(opacity);
    auto* anim = new QPropertyAnimation(opacity, "opacity");
    anim->setDuration(1500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.3);
    anim->setEasingCurve(QEasingCurve::InOutSine);
    anim->setLoopCount(-1);
    anim->start();

    auto* liveLbl = new QLabel("Live");
    liveLbl->setStyleSheet("font-size: 11px; font-weight: 600; color: #44CC88; margin-left: 4px;");
    hRow->addWidget(liveDot, 0, Qt::AlignVCenter);
    hRow->addWidget(liveLbl, 0, Qt::AlignVCenter);
    hRow->addStretch(1);
    outer->addLayout(hRow);

    auto* descLbl = new QLabel("Posts from people you follow");
    descLbl->setStyleSheet("font-size: 12px; color: #2A2A40; margin-top: -8px;");
    outer->addWidget(descLbl);

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
        m_postInput->setFixedHeight(96);
        m_postInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        ccl->addWidget(m_postInput);

        auto* btnRow = new QHBoxLayout;
        btnRow->addStretch(1);
        auto* postBtn = makePrimary("Post");
        postBtn->setFixedWidth(80);
        postBtn->setFixedHeight(36);
        connect(postBtn, &QPushButton::clicked, this, &FeedPage::onSubmitPost);
        btnRow->addWidget(postBtn);
        ccl->addLayout(btnRow);
    }
    outer->addWidget(m_composerCard);

    // ── Scrollable feed ─────────────────────────────────────────────────────
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

    bool any = false;
    for (int u = 0; u < m_userCount; ++u) {
        if (!m_allUsers[u] || m_allUsers[u] == m_user) continue;
        if (!m_user->isFollowing(m_allUsers[u]->getUsername())) continue;
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
            connect(card, &PostCard::reportUserClicked, this, &FeedPage::onReportUserFromPost);
            m_feedLayout->insertWidget(m_feedLayout->count() - 1, card);
            any = true;
        }
    }

    if (!any) {
        auto* emptyIcon = new QLabel("✦");
        emptyIcon->setAlignment(Qt::AlignCenter);
        emptyIcon->setStyleSheet("font-size: 48px; color: #1C1C30;");
        auto* empty = new QLabel("Your feed is quiet");
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("color: #1C1C30; font-size: 16px; font-weight: 600; padding: 8px 0;");
        auto* emptySub = new QLabel("Follow people to see their posts here");
        emptySub->setAlignment(Qt::AlignCenter);
        emptySub->setStyleSheet("color: #161625; font-size: 13px;");
        m_feedLayout->insertWidget(0, emptyIcon);
        m_feedLayout->insertWidget(1, empty);
        m_feedLayout->insertWidget(2, emptySub);
    }
}

void FeedPage::onReportUserFromPost(const QString& username) {
    if (!m_user) return;

    User* target = nullptr;
    for (int i = 0; i < m_userCount; ++i) {
        if (m_allUsers[i] && m_allUsers[i]->getUsername() == username.toStdString()) {
            target = m_allUsers[i];
            break;
        }
    }
    if (!target) return;

    auto r = QMessageBox::question(this, "Report User",
        "Are you sure you want to report user @" + username + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (r == QMessageBox::Yes) {
        target->reportUser();
        QMessageBox::information(this, "Reported", "User @" + username + " has been reported to admins.");
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

void FeedPage::onLikePost(Posts* post) {
    if (post)
        qDebug() << "Like toggled on post:" << QString::fromStdString(post->getPostId());
}

void FeedPage::onCommentPost(Posts* post) {
    if (!post || !m_user) return;

    CommentDialog dlg(post, QString::fromStdString(m_user->getUsername()), this);
    dlg.exec();
}

void FeedPage::onDeletePost(Posts* post) {
    if (!post) return;
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

    CommentDialog dlg(post, QString::fromStdString(m_user->getUsername()), this);
    dlg.exec();

    refresh();
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
NotificationsPage::NotificationsPage(User* currentUser, QWidget* parent)
    : QWidget(parent), m_user(currentUser)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    auto* hRow = new QHBoxLayout;
    hRow->setContentsMargins(0, 0, 0, 0);
    hRow->addWidget(makeLabel("Notifications", "pageTitle"), 1);

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
            QString p0 = parts[0], p1 = parts[1];
            bool wasCorrupted = p0.contains(' ');
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

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    m_searchStack = new QStackedWidget(this);
    rootLayout->addWidget(m_searchStack);

    m_searchMainWidget = new QWidget;
    auto* outer = new QVBoxLayout(m_searchMainWidget);
    outer->setContentsMargins(28, 24, 28, 16);
    outer->setSpacing(16);

    outer->addWidget(makeLabel("Search Users", "pageTitle"));

    auto* searchRow = new QHBoxLayout;
    searchRow->setContentsMargins(0, 0, 0, 0);
    searchRow->setSpacing(10);
    m_searchInput = new QLineEdit;
    m_searchInput->setPlaceholderText("Search by username...");
    m_searchInput->setFixedHeight(50);
    auto* searchBtn = makePrimary("Search");
    searchBtn->setFixedHeight(44);
    searchBtn->setFixedWidth(88);
    connect(searchBtn, &QPushButton::clicked, this, &SearchPage::onSearch);
    connect(m_searchInput, &QLineEdit::returnPressed, this, &SearchPage::onSearch);
    searchRow->addWidget(m_searchInput, 1);
    searchRow->addWidget(searchBtn);
    outer->addLayout(searchRow);

    m_scrollArea = makeScrollArea(m_resultsContent, m_resultsLayout);
    outer->addWidget(m_scrollArea, 1);

    m_profileWidget = new PublicProfileWidget(this);
    connect(m_profileWidget, &PublicProfileWidget::backClicked, this, [this]() {
        m_searchStack->setCurrentIndex(0);
        });

    m_searchStack->addWidget(m_searchMainWidget);
    m_searchStack->addWidget(m_profileWidget);
}

void SearchPage::onSearch() {
    QString query = m_searchInput->text().trimmed();
    if (query.isEmpty()) return;

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
    topRow->addWidget(avatar(ini, 56), 0, Qt::AlignVCenter);
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

    // Stats mini-cards
    auto* statsRow = new QHBoxLayout;
    statsRow->setContentsMargins(0, 0, 0, 0);
    statsRow->setSpacing(12);
    auto addStat = [&](const QString& val, const QString& lbl) {
        auto* card = new QFrame;
        card->setStyleSheet("background: #0A0A14; border: 1px solid #111125; border-radius: 10px;");
        auto* c = new QVBoxLayout(card);
        c->setContentsMargins(12, 10, 12, 10);
        c->setSpacing(2);
        auto* v = new QLabel(val);
        v->setStyleSheet("font-size: 18px; font-weight: 700; color: #5B5BD6; letter-spacing: -1px;");
        v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size: 10px; font-weight: 600; color: #2A2A44; letter-spacing: 1.5px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l);
        statsRow->addWidget(card);
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

        auto* profileBtn = new QPushButton("👤 View Profile");
        profileBtn->setObjectName("secondaryBtn");
        profileBtn->setFixedWidth(130);
        profileBtn->setFixedHeight(38);
        profileBtn->setCursor(Qt::PointingHandCursor);
        connect(profileBtn, &QPushButton::clicked, this, [this, user]() {
            m_profileWidget->loadProfile(user, m_currentUser);
            m_searchStack->setCurrentIndex(1);
            });
        btnRow->addWidget(profileBtn);
        btnRow->addSpacing(10);

        bool following = m_currentUser->isFollowing(user->getUsername());
        m_followBtn = following ? makeSecondary("✓  Following") : makePrimary("+ Follow");
        m_followBtn->setFixedWidth(130);
        m_followBtn->setFixedHeight(38);
        connect(m_followBtn, &QPushButton::clicked, this, &SearchPage::onFollowUser);
        btnRow->addWidget(m_followBtn);
        btnRow->addSpacing(10);

        m_reportBtn = makeDanger("⚑ Report");
        m_reportBtn->setFixedWidth(100);
        m_reportBtn->setFixedHeight(38);
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
// ══════════════════════════════════════════════════════════════════════════════
namespace {

    QString msgFilePath(const QString& a, const QString& b) {
        QDir().mkpath("data/Messages");
        QString lo = (a < b) ? a : b;
        QString hi = (a < b) ? b : a;
        return "data/Messages/" + lo + "_" + hi + ".txt";
    }

    void ensureInIndex(const QString& username, const QString& peer) {
        QDir().mkpath("data/Messages");
        QString path = "data/Messages/" + username + "_index.txt";
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
        QFile wf(path);
        if (wf.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&wf);
            out << peer << "\n";
            wf.close();
        }
    }

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
    header->setObjectName("msgHeader");
    header->setFixedHeight(56);
    auto* hRow = new QHBoxLayout(header);
    hRow->setContentsMargins(20, 0, 16, 0);
    hRow->setSpacing(12);

    QString ini = peerUsername.isEmpty() ? "?" : peerUsername.left(1).toUpper();
    hRow->addWidget(avatar(ini, 34), 0, Qt::AlignVCenter);
    auto* peerLbl = new QLabel(peerUsername);
    peerLbl->setStyleSheet("font-size: 15px; font-weight: 700; color: #E0E0FF;");
    hRow->addWidget(peerLbl, 1);

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
    m_bubbleLayout->addStretch(1);

    m_scroll->setWidget(m_bubbleContainer);
    root->addWidget(m_scroll, 1);

    // ── Input row ────────────────────────────────────────────────────────────
    auto* inputBar = new QFrame;
    inputBar->setObjectName("chatInputBar");
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
    while (m_bubbleLayout->count() > 1) {
        QLayoutItem* it = m_bubbleLayout->takeAt(0);
        if (it->widget()) it->widget()->deleteLater();
        delete it;
    }
}

void ChatView::appendBubble(const QString& sender, const QString& text) {
    bool isMine = (sender == m_currentUser);

    auto* wrapWidget = new QWidget;
    wrapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* wl = new QHBoxLayout(wrapWidget);
    wl->setContentsMargins(0, 0, 0, 0);

    auto* bubble = new QLabel(text);
    bubble->setWordWrap(true);
    bubble->setMaximumWidth(420);
    bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    if (isMine) {
        bubble->setObjectName("bubbleOwn");
        wl->addStretch(1);
        wl->addWidget(bubble);
    }
    else {
        bubble->setObjectName("bubbleOther");
        auto* col = new QVBoxLayout;
        col->setSpacing(2);
        col->setContentsMargins(0, 0, 0, 0);
        auto* nameLbl2 = new QLabel(sender);
        nameLbl2->setStyleSheet("font-size: 10px; color: #44445A; font-weight: 600;");
        col->addWidget(nameLbl2);
        col->addWidget(bubble);
        wl->addLayout(col);
        wl->addStretch(1);
    }
    m_bubbleLayout->insertWidget(m_bubbleLayout->count() - 1, wrapWidget);

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
        int firstPipe = line.indexOf('|');
        if (firstPipe < 0) continue;
        int secondPipe = line.indexOf('|', firstPipe + 1);
        if (secondPipe < 0) continue;
        QString sender = line.left(firstPipe);
        QString text = line.mid(secondPipe + 1);
        if (!text.isEmpty()) appendBubble(sender, text);
    }
    f.close();
}

void ChatView::refresh() {
    loadMessages();
}

void ChatView::markConversationUnread(const QString& receiver, const QString& sender) {
    QDir().mkpath("data/Messages");
    QString unreadPath = "data/Messages/" + receiver + "_unread.txt";

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

    if (!alreadyExists) {
        QFile unreadFile(unreadPath);
        if (unreadFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&unreadFile);
            out << sender << "\n";
            unreadFile.close();
        }
    }
}

void ChatView::onSend() {
    QString text = m_input->text().trimmed();
    if (text.isEmpty()) return;
    if (text.contains('|')) {
        QMessageBox::warning(this, "Novo", "Messages cannot contain '|'.");
        return;
    }

    QDir().mkpath("data/Messages");
    QFile f(chatFilePath());
    if (f.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&f);
        QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        out << m_currentUser << "|" << ts << "|" << text << "\n";
        f.close();
    }

    ensureInIndex(m_currentUser, m_peer);
    ensureInIndex(m_peer, m_currentUser);

    QDir().mkpath("data/Notifications");
    QString notifPath = "data/Notifications/" + m_peer + "_notif.txt";
    QFile notifFile(notifPath);
    if (notifFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream notifOut(&notifFile);
        QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        notifOut << "message|" << m_currentUser << " sent you a message|" << ts << "|0\n";
        notifFile.close();
    }

    markConversationUnread(m_peer, m_currentUser);

    m_input->clear();
    appendBubble(m_currentUser, text);
}

void ChatView::onDeleteChat() {
    auto r = QMessageBox::question(this, "Delete Chat",
        "Delete the entire conversation with @" + m_peer + "?",
        QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;

    QFile::remove(chatFilePath());
    removeFromIndex(m_currentUser, m_peer);
    removeFromIndex(m_peer, m_currentUser);

    emit chatDeleted(m_peer);
}

// ══════════════════════════════════════════════════════════════════════════════
//  MessagesPage
// ══════════════════════════════════════════════════════════════════════════════
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

    // ── LEFT PANEL ───────────────────────────────────────────────────────────
    m_leftPanel = new QWidget;
    m_leftPanel->setFixedWidth(240);
    m_leftPanel->setObjectName("msgLeftPanel");

    auto* ll = new QVBoxLayout(m_leftPanel);
    ll->setContentsMargins(12, 20, 12, 16);
    ll->setSpacing(10);

    auto* titleLbl = new QLabel("Messages");
    titleLbl->setStyleSheet("font-size: 17px; font-weight: 700; color: #E0E0FF; padding-bottom: 4px;");
    ll->addWidget(titleLbl);

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

    ll->addWidget(divider());

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

    // ── RIGHT PANEL ──────────────────────────────────────────────────────────
    m_rightStack = new QStackedWidget;

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
    m_rightStack->addWidget(placeholder);

    root->addWidget(m_rightStack, 1);

    loadConversationList();
}

void MessagesPage::refresh() {
    loadConversationList();
    if (m_chatView) m_chatView->refresh();
}

void MessagesPage::addConversationButton(const QString& peer) {
    auto* btn = new QPushButton;
    btn->setObjectName("navBtn");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedHeight(52);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

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

    QString displayText = "  " + peer.left(1).toUpper() + "  " + peer;
    if (hasUnread) {
        displayText += " ●";
    }

    btn->setText(displayText);
    btn->setStyleSheet(
        "QPushButton { background: transparent; border: none; border-radius: 8px;"
        " text-align: left; padding-left: 10px; color: #7070AA; font-size: 13px; font-weight: 500; }"
        "QPushButton:hover { background: #17171F; color: #AAAAEE; }"
        "QPushButton:checked { background: #13132A; color: #7C7CEA; border-left: 3px solid #5B5BD6; }");

    if (hasUnread) {
        btn->setStyleSheet(btn->styleSheet() +
            " QPushButton { color: #FF6060; font-weight: 600; }");
    }

    btn->setCheckable(true);

    if (peer == m_activePeer) btn->setChecked(true);

    connect(btn, &QPushButton::clicked, this, [this, peer, btn]() {
        QList<QPushButton*> siblings = m_convListContent->findChildren<QPushButton*>();
        for (auto* s : siblings) s->setChecked(false);
        btn->setChecked(true);

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
    loadConversationList();

    if (m_chatView) {
        m_rightStack->removeWidget(m_chatView);
        m_chatView->deleteLater();
        m_chatView = nullptr;
    }

    m_chatView = new ChatView(me, peer);
    connect(m_chatView, &ChatView::chatDeleted, this, &MessagesPage::onChatDeleted);
    m_rightStack->addWidget(m_chatView);
    m_rightStack->setCurrentWidget(m_chatView);

    ensureInIndex(me, peer);
    ensureInIndex(peer, me);
    loadConversationList();
}

void MessagesPage::onConversationSelected(const QString& peer) {
    openChatWith(peer);
}

void MessagesPage::onChatDeleted(const QString& peer) {
    m_activePeer.clear();

    if (m_chatView) {
        m_rightStack->removeWidget(m_chatView);
        m_chatView->deleteLater();
        m_chatView = nullptr;
    }
    m_rightStack->setCurrentIndex(0);
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

    auto* titleRow = new QHBoxLayout;
    auto* title = makeLabel("⚡ Admin Dashboard", "pageTitle");
    titleRow->addWidget(title);
    titleRow->addStretch(1);
    outer->addLayout(titleRow);

    auto* sub = new QLabel("Review and manage reported content");
    sub->setStyleSheet("font-size: 12px; color: #2A2A44; margin-top: -8px;");
    outer->addWidget(sub);

    // Stats row
    auto* statsRow = new QHBoxLayout;
    statsRow->setSpacing(12);
    auto addMiniStat = [&](const QString& val, const QString& lbl) {
        auto* card = new QFrame;
        card->setStyleSheet("background: #0A0A14; border: 1px solid #111125; border-radius: 10px;");
        auto* c = new QVBoxLayout(card);
        c->setContentsMargins(12, 10, 12, 10);
        c->setSpacing(2);
        auto* v = new QLabel(val);
        v->setStyleSheet("font-size: 18px; font-weight: 700; color: #5B5BD6;");
        v->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size: 10px; font-weight: 600; color: #2A2A44; letter-spacing: 1.5px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(v); c->addWidget(l);
        statsRow->addWidget(card);
        };
    // We'll just show placeholders; real counts loaded dynamically
    addMiniStat("—", "REPORTS");
    addMiniStat("—", "USERS");
    addMiniStat("—", "POSTS");
    statsRow->addStretch(1);
    outer->addLayout(statsRow);

    // Tab bar
    auto* tabBar = new QHBoxLayout;
    tabBar->setSpacing(6);
    auto* tabUsers = new QPushButton("Reported Users");
    auto* tabPosts = new QPushButton("Reported Posts");
    auto* tabNotifs = new QPushButton("🔔 Notifications");
    for (auto* b : { tabUsers, tabPosts, tabNotifs }) {
        b->setFixedHeight(34);
        b->setCursor(Qt::PointingHandCursor);
    }
    tabUsers->setObjectName("primaryBtn");
    tabPosts->setObjectName("secondaryBtn");
    tabNotifs->setObjectName("secondaryBtn");

    connect(tabUsers, &QPushButton::clicked, this, [this, tabUsers, tabPosts, tabNotifs]() {
        tabUsers->setObjectName("primaryBtn");
        tabPosts->setObjectName("secondaryBtn");
        tabNotifs->setObjectName("secondaryBtn");
        for (auto* b : { tabUsers, tabPosts, tabNotifs }) { b->style()->unpolish(b); b->style()->polish(b); }
        onReviewReportedUsers();
        });
    connect(tabPosts, &QPushButton::clicked, this, [this, tabUsers, tabPosts, tabNotifs]() {
        tabUsers->setObjectName("secondaryBtn");
        tabPosts->setObjectName("primaryBtn");
        tabNotifs->setObjectName("secondaryBtn");
        for (auto* b : { tabUsers, tabPosts, tabNotifs }) { b->style()->unpolish(b); b->style()->polish(b); }
        onReviewReportedPosts();
        });
    connect(tabNotifs, &QPushButton::clicked, this, [this, tabUsers, tabPosts, tabNotifs]() {
        tabUsers->setObjectName("secondaryBtn");
        tabPosts->setObjectName("secondaryBtn");
        tabNotifs->setObjectName("primaryBtn");
        for (auto* b : { tabUsers, tabPosts, tabNotifs }) { b->style()->unpolish(b); b->style()->polish(b); }
        loadAdminNotifications();
        });

    tabBar->addWidget(tabUsers);
    tabBar->addWidget(tabPosts);
    tabBar->addWidget(tabNotifs);
    tabBar->addStretch(1);
    outer->addLayout(tabBar);

    m_scrollArea = makeScrollArea(m_content, m_layout);
    outer->addWidget(m_scrollArea, 1);
}

void AdminPage::loadAdminNotifications() {
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

            QStringList parts = line.split("|");
            if (parts.size() < 4) continue;

            QString timestamp = parts[0];
            QString type = parts[1];
            QString message = parts[2];
            bool    isRead = (parts[3] == "1");

            auto* card = new QFrame;
            card->setObjectName("notifCard");
            card->setProperty("unread", !isRead);
            card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
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
        card->setStyleSheet("QFrame { background: #0D0808; border: 1px solid #2A1010; border-radius: 12px; border-left: 3px solid #EE5555; }");
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        auto* row = new QHBoxLayout(card);
        row->setContentsMargins(16, 12, 16, 12);
        row->setSpacing(10);

        auto* infoCol = new QVBoxLayout;
        auto* label = makeLabel("⚠ User: " + QString::fromStdString(targetUsername));
        label->setStyleSheet("font-size: 14px; font-weight: 600; color: #EE7777;");
        auto* reportLbl = makeLabel("Reported by multiple users");
        reportLbl->setStyleSheet("font-size: 11px; color: #664444;");
        infoCol->addWidget(label);
        infoCol->addWidget(reportLbl);

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
            card->setStyleSheet("QFrame { background: #0D0808; border: 1px solid #2A1010; border-radius: 12px; border-left: 3px solid #EE5555; }");
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
// ══════════════════════════════════════════════════════════════════════════════
ProfilePage::ProfilePage(User* currentUser, User** allUsers, int* userCount, QWidget* parent)
    : QWidget(parent), m_user(currentUser), m_allUsers(allUsers), m_userCountPtr(userCount)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outer->addWidget(scroll);

    auto* inner = new QWidget;
    inner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto* layout = new QVBoxLayout(inner);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(16);

    layout->addWidget(makeLabel("Profile", "pageTitle"));

    // ── Profile hero card ────────────────────────────────────────────────────
    auto* pCard = new QFrame;
    pCard->setObjectName("postCard");
    pCard->setStyleSheet("QFrame#postCard { background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #0D0D18, stop:1 #111125); }");
    pCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* pcl = new QVBoxLayout(pCard);
    pcl->setContentsMargins(28, 28, 28, 28);
    pcl->setSpacing(16);

    auto* infoRow = new QHBoxLayout;
    infoRow->setSpacing(16);
    infoRow->setContentsMargins(0, 0, 0, 0);
    QString ini = currentUser ? QString::fromStdString(currentUser->getUsername()).left(1).toUpper() : "?";
    auto* av = avatar(ini, 72);
    infoRow->addWidget(av, 0, Qt::AlignVCenter);

    auto* infoCol = new QVBoxLayout; infoCol->setSpacing(5);
    m_usernameLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getUsername()) : "");
    m_usernameLabel->setStyleSheet("font-size:22px; font-weight:700; color:#F1F1F8;");
    auto* handleLbl = new QLabel(currentUser ? "@" + QString::fromStdString(currentUser->getUsername()) : "");
    handleLbl->setStyleSheet("font-size:13px; color:#33334A;");
    m_bioLabel = makeLabel(currentUser ? QString::fromStdString(currentUser->getBio()) : "");
    m_bioLabel->setStyleSheet("font-size:13px; color:#6868A0;");
    infoCol->addWidget(m_usernameLabel);
    infoCol->addWidget(handleLbl);
    infoCol->addWidget(m_bioLabel);
    infoRow->addLayout(infoCol, 1);
    pcl->addLayout(infoRow);

    auto* statsRow = new QHBoxLayout; statsRow->setContentsMargins(0, 0, 0, 0);
    statsRow->setSpacing(12);
    auto addStatBox = [&](QLabel*& outLabel, const QString& val, const QString& lbl) {
        auto* card = new QFrame;
        card->setStyleSheet("background: #0A0A14; border: 1px solid #111125; border-radius: 10px;");
        auto* c = new QVBoxLayout(card);
        c->setContentsMargins(12, 10, 12, 10);
        c->setSpacing(2);
        outLabel = new QLabel(val);
        outLabel->setStyleSheet("font-size:20px; font-weight:700; color:#5B5BD6; letter-spacing:-1px;");
        outLabel->setAlignment(Qt::AlignCenter);
        auto* l = new QLabel(lbl);
        l->setStyleSheet("font-size:10px; font-weight:600; color:#2A2A44; letter-spacing:1.5px;");
        l->setAlignment(Qt::AlignCenter);
        c->addWidget(outLabel); c->addWidget(l);
        statsRow->addWidget(card);
        };
    if (currentUser) {
        addStatBox(m_postsCountLabel, QString::number(currentUser->getPostCount()), "POSTS");
        addStatBox(m_followersCountLabel, QString::number(currentUser->getFollowersCount()), "FOLLOWERS");
        addStatBox(m_followingCountLabel, QString::number(currentUser->getFollowingCount()), "FOLLOWING");
    }
    statsRow->addStretch(1);
    pcl->addLayout(statsRow);
    layout->addWidget(pCard);

    // ── Edit bio card ────────────────────────────────────────────────────────
    auto* bioCard = new QFrame; bioCard->setObjectName("postCard");
    bioCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* bcl = new QVBoxLayout(bioCard);
    bcl->setContentsMargins(22, 18, 22, 18); bcl->setSpacing(10);
    bcl->addWidget(sectionLabel("Update Bio"));
    m_newBioInput = new QLineEdit;
    m_newBioInput->setPlaceholderText("New bio...");
    m_newBioInput->setFixedHeight(44);
    bcl->addWidget(m_newBioInput);
    auto* saveBioBtn = makePrimary("Save Bio");
    saveBioBtn->setFixedWidth(110); saveBioBtn->setFixedHeight(36);
    connect(saveBioBtn, &QPushButton::clicked, this, &ProfilePage::onUpdateBio);
    auto* bioRow = new QHBoxLayout; bioRow->addStretch(1); bioRow->addWidget(saveBioBtn);
    bcl->addLayout(bioRow);
    layout->addWidget(bioCard);

    // ── Change password card ─────────────────────────────────────────────────
    auto* passCard = new QFrame; passCard->setObjectName("postCard");
    passCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* playout = new QVBoxLayout(passCard);
    playout->setContentsMargins(22, 18, 22, 18); playout->setSpacing(10);
    playout->addWidget(sectionLabel("Change Password"));
    m_newPassInput = new QLineEdit;
    m_newPassInput->setPlaceholderText("New password...");
    m_newPassInput->setEchoMode(QLineEdit::Password);
    m_newPassInput->setFixedHeight(44);
    playout->addWidget(m_newPassInput);
    auto* savePassBtn = makePrimary("Update Password");
    savePassBtn->setFixedWidth(150); savePassBtn->setFixedHeight(36);
    connect(savePassBtn, &QPushButton::clicked, this, &ProfilePage::onUpdatePassword);
    auto* passRow2 = new QHBoxLayout; passRow2->addStretch(1); passRow2->addWidget(savePassBtn);
    playout->addLayout(passRow2);
    layout->addWidget(passCard);

    // ── Posts / Saved tabs card ───────────────────────────────────────────────
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
    auto* dangerCard = new QFrame;
    dangerCard->setStyleSheet("QFrame { background: #0A0408; border: 1px solid #2A0D0D; border-radius: 14px; }");
    dangerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto* dl = new QVBoxLayout(dangerCard);
    dl->setContentsMargins(22, 18, 22, 18); dl->setSpacing(10);
    auto* dtitle = sectionLabel("Danger Zone");
    dtitle->setStyleSheet("font-size:10px; font-weight:600; color:#3A1A1A; letter-spacing:1.8px;");
    auto* ddesc = makeLabel("Permanently deletes your account and all associated data.");
    ddesc->setStyleSheet("font-size:12px; color:#2A1010;");
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

    auto* timerCard = new QFrame;
    timerCard->setObjectName("postCard");
    timerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto* tcl = new QVBoxLayout(timerCard);
    tcl->setContentsMargins(40, 36, 40, 36);
    tcl->setSpacing(14);
    tcl->setAlignment(Qt::AlignCenter);

    auto* clockIcon = new QLabel("⏱");
    clockIcon->setAlignment(Qt::AlignCenter);
    clockIcon->setStyleSheet("font-size: 44px; color: #5B5BD6;");
    tcl->addWidget(clockIcon);

    m_timerLabel = makeLabel("00:00:00", "timerDisplay");
    m_timerLabel->setAlignment(Qt::AlignCenter);
    tcl->addWidget(m_timerLabel);

    m_sessionLabel = makeLabel("Session started: —");
    m_sessionLabel->setAlignment(Qt::AlignCenter);
    m_sessionLabel->setStyleSheet("color: #2A2A44; font-size: 12px;");
    tcl->addWidget(m_sessionLabel);

    outer->addWidget(timerCard);

    // Info cards row
    auto* infoRow = new QHBoxLayout;
    infoRow->setSpacing(12);
    auto addInfoCard = [&](const QString& icon, const QString& val, const QString& lbl) {
        auto* card = new QFrame;
        card->setObjectName("postCard");
        auto* c = new QVBoxLayout(card);
        c->setContentsMargins(16, 16, 16, 16);
        c->setSpacing(4);
        auto* ic = new QLabel(icon);
        ic->setAlignment(Qt::AlignCenter);
        ic->setStyleSheet("font-size: 28px;");
        auto* v = new QLabel(val);
        v->setAlignment(Qt::AlignCenter);
        v->setStyleSheet("font-size: 20px; font-weight: 700; color: #5B5BD6;");
        auto* la = new QLabel(lbl);
        la->setAlignment(Qt::AlignCenter);
        la->setStyleSheet("font-size: 10px; font-weight: 600; color: #2A2A44; letter-spacing: 1.5px;");
        c->addWidget(ic);
        c->addWidget(v);
        c->addWidget(la);
        infoRow->addWidget(card);
        };
    addInfoCard("🎯", "—", "TODAY'S GOAL");
    addInfoCard("📊", "—", "AVG DAILY");
    addInfoCard("🔥", "—", "STREAK");
    infoRow->addStretch(1);
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

    m_pages->setCurrentIndex(0);
    m_appShell->show();
    m_rootStack->setCurrentIndex(1);
    m_timeSpentPage->startSession();
    onNavFeed();
}

void MainWindow::onLoginAdminSuccess() {
    m_authPage->resetToLogin();
    m_allUsers = m_authPage->getAllUsers();
    m_userCount = m_authPage->getUserCount();

    m_shellLayout = new QHBoxLayout(m_appShell);
    m_shellLayout->setContentsMargins(0, 0, 0, 0);
    m_shellLayout->setSpacing(0);

    m_sidebar = new QWidget;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(220);
    m_sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    auto* sl = new QVBoxLayout(m_sidebar);
    sl->setContentsMargins(12, 22, 12, 18);
    sl->setSpacing(4);

    auto* brand = new QLabel(APP_NAME);
    brand->setStyleSheet("font-size:21px; font-weight:800; color:#7C7CEA; padding: 0 4px 12px 4px;");
    sl->addWidget(brand);
    sl->addWidget(divider());
    sl->addSpacing(10);
    sl->addWidget(sectionLabel("Administrator"));
    sl->addStretch(1);

    m_btnLogout = new SidebarButton("←", "Log Out");
    m_btnLogout->setObjectName("navBtnDanger");
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(m_btnLogout);

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
    m_sidebar->setFixedWidth(220);
    m_sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    auto* sl = new QVBoxLayout(m_sidebar);
    sl->setContentsMargins(12, 22, 12, 18);
    sl->setSpacing(4);

    // Brand
    auto* brandRow = new QHBoxLayout;
    brandRow->setSpacing(8);
    auto* brandCircle = new QLabel("N");
    brandCircle->setFixedSize(32, 32);
    brandCircle->setAlignment(Qt::AlignCenter);
    brandCircle->setStyleSheet("background: qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #5B5BD6, stop:1 #7C4FD4); border-radius: 16px; font-size: 14px; font-weight: 800; color: #FFFFFF;");
    auto* brand = new QLabel(APP_NAME);
    brand->setObjectName("brandLabel");
    brandRow->addWidget(brandCircle);
    brandRow->addWidget(brand, 1);
    sl->addLayout(brandRow);
    sl->addWidget(divider());
    sl->addSpacing(20);

    // FAB
    auto* fabBtn = new QPushButton("＋  New Post");
    fabBtn->setObjectName("fabBtn");
    fabBtn->setFixedHeight(40);
    fabBtn->setCursor(Qt::PointingHandCursor);
    connect(fabBtn, &QPushButton::clicked, this, &MainWindow::onSidebarCreatePost);
    sl->addWidget(fabBtn);
    sl->addSpacing(20);

    // Nav label
    sl->addWidget(sectionLabel("Menu"));
    sl->addSpacing(8);

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

    sl->addStretch(1);
    sl->addWidget(divider());
    sl->addSpacing(6);

    // User chip
    auto* userChip = new QFrame;
    userChip->setObjectName("userChip");
    auto* ucLayout = new QHBoxLayout(userChip);
    ucLayout->setContentsMargins(8, 8, 8, 8);
    ucLayout->setSpacing(8);
    QString ini = m_currentUser ? QString::fromStdString(m_currentUser->getUsername()).left(1).toUpper() : "?";
    ucLayout->addWidget(avatar(ini, 28), 0, Qt::AlignVCenter);
    auto* uname = new QLabel(m_currentUser ? QString::fromStdString(m_currentUser->getUsername()) : "");
    uname->setStyleSheet("font-size:12px; color:#6868A0; font-weight:600;");
    uname->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    uname->setMaximumWidth(110);
    uname->setWordWrap(false);
    ucLayout->addWidget(uname, 1);
    sl->addWidget(userChip);
    sl->addSpacing(8);

    m_btnSwitchUser = new SidebarButton("⇄", "Switch User");
    m_btnSwitchUser->setObjectName("navBtnDanger");
    connect(m_btnSwitchUser, &QPushButton::clicked, this, &MainWindow::onSwitchUser);
    sl->addWidget(m_btnSwitchUser);

    m_btnLogout = new SidebarButton("←", "Log Out");
    m_btnLogout->setObjectName("navBtnDanger");
    connect(m_btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    sl->addWidget(m_btnLogout);
}

void MainWindow::buildPages() {
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
}

void MainWindow::onNavNotifications() {
    if (!m_pages || !m_notifPage) return;
    m_pages->setCurrentIndex(1);
    m_notifPage->refresh();
    setActiveSidebarButton(m_btnNotifications);
}

void MainWindow::onNavSearch() {
    if (!m_pages) return;
    m_pages->setCurrentIndex(2);
    setActiveSidebarButton(m_btnSearch);
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
}

void MainWindow::onNavTimeSpent() {
    if (!m_pages) return;
    m_pages->setCurrentIndex(5);
    setActiveSidebarButton(m_btnTimeSpent);
}

void MainWindow::onLogout() {
    auto r = QMessageBox::question(this, APP_NAME,
        "Are you sure you want to log out?",
        QMessageBox::Yes | QMessageBox::No);
    if (r != QMessageBox::Yes) return;
    if (m_currentUser) m_currentUser->logOut();
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    m_appShell->hide();
    tearDownShell();
    m_currentUser = nullptr;
    m_authPage->resetToLogin();
    m_rootStack->setCurrentIndex(0);
}

void MainWindow::onSwitchUser() {
    if (m_currentUser) m_currentUser->logOut();
    if (m_timeSpentPage) m_timeSpentPage->stopSession();
    m_appShell->hide();
    tearDownShell();
    m_currentUser = nullptr;
    m_authPage->resetToLogin();
    m_rootStack->setCurrentIndex(0);
}

void MainWindow::tearDownShell() {
    if (m_appShell) {
        QList<QWidget*> children = m_appShell->findChildren<QWidget*>(
            QString(), Qt::FindDirectChildrenOnly);
        for (QWidget* w : children) {
            w->hide();
            w->deleteLater();
        }
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