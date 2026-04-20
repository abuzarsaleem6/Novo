#include "SplashScreen.h"
#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>

SplashScreen::SplashScreen(User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), allUsers(allUsers), userCount(userCount) {

    setFixedSize(500, 400);
    setWindowFlags(Qt::FramelessWindowHint);

    // Background gradient
    setStyleSheet("background: qlineargradient("
        "x1:0, y1:0, x2:1, y2:1,"
        "stop:0 #1a1a2e, stop:0.5 #16213e, stop:1 #0f3460);");

    // NOVO label
    logoLabel = new QLabel("NOVO", this);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet(
        "font-size: 72px;"
        "font-weight: bold;"
        "color: white;"
        "letter-spacing: 20px;"
    );

    QLabel* tagline = new QLabel("Connect. Share. Discover.", this);
    tagline->setAlignment(Qt::AlignCenter);
    tagline->setStyleSheet("color: #a0a0ff; font-size: 16px; letter-spacing: 4px;");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addStretch();
    layout->addWidget(logoLabel);
    layout->addWidget(tagline);
    layout->addStretch();
    setLayout(layout);

    // Fade in animation
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    logoLabel->setGraphicsEffect(effect);

    QPropertyAnimation* fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(1500);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    // Timer — 3 second baad LoginWindow
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(3000);
    connect(timer, &QTimer::timeout, this, &SplashScreen::onAnimationDone);
    timer->start();
}

void SplashScreen::onAnimationDone() {
    LoginWindow* loginWin = new LoginWindow(allUsers, userCount);
    loginWin->show();
    this->close();
}