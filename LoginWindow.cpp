#include "LoginWindow.h"
#include "SignupWindow.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginWindow::LoginWindow(User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), allUsers(allUsers), userCount(userCount) {

    setWindowTitle("NOVO - Login");
    setFixedSize(450, 500);

    // Main background
    setStyleSheet("QWidget {"
        "background: qlineargradient("
        "x1:0, y1:0, x2:1, y2:1,"
        "stop:0 #1a1a2e, stop:1 #0f3460);"
        "}");

    // Card widget
    QWidget* card = new QWidget();
    card->setFixedSize(380, 420);
    card->setStyleSheet(
        "QWidget {"
        "background: rgba(255,255,255,0.05);"
        "border-radius: 20px;"
        "border: 1px solid rgba(255,255,255,0.1);"
        "}"
    );

    QLabel* title = new QLabel("Welcome Back");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold;"
        "border: none; background: transparent;");

    QLabel* subtitle = new QLabel("Login to NOVO");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #a0a0ff; font-size: 14px;"
        "border: none; background: transparent;");

    QString inputStyle =
        "QLineEdit {"
        "background: rgba(255,255,255,0.08);"
        "border: 1px solid rgba(255,255,255,0.2);"
        "border-radius: 10px;"
        "padding: 12px 16px;"
        "color: white;"
        "font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "border: 1px solid #a0a0ff;"
        "}";

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username");
    usernameInput->setStyleSheet(inputStyle);
    usernameInput->setFixedHeight(48);

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(inputStyle);
    passwordInput->setFixedHeight(48);

    errorLabel = new QLabel("");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setStyleSheet("color: #ff6b6b; font-size: 13px;"
        "border: none; background: transparent;");
    errorLabel->setWordWrap(true);

    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setFixedHeight(48);
    loginBtn->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #667eea, stop:1 #764ba2);"
        "color: white; font-size: 15px; font-weight: bold;"
        "border-radius: 10px; border: none;"
        "}"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #764ba2, stop:1 #667eea); }"
        "QPushButton:pressed { opacity: 0.8; }"
    );

    QPushButton* signupBtn = new QPushButton("Don't have an account? Sign Up");
    signupBtn->setStyleSheet(
        "QPushButton { background: transparent; color: #a0a0ff;"
        "font-size: 13px; border: none; text-decoration: underline; }"
        "QPushButton:hover { color: white; }"
    );

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 30, 30, 30);
    cardLayout->setSpacing(15);
    cardLayout->addWidget(title);
    cardLayout->addWidget(subtitle);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(usernameInput);
    cardLayout->addWidget(passwordInput);
    cardLayout->addWidget(errorLabel);
    cardLayout->addWidget(loginBtn);
    cardLayout->addWidget(signupBtn);

    // Center card
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(card, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(signupBtn, &QPushButton::clicked, this, &LoginWindow::onSignupClicked);
}

void LoginWindow::onLoginClicked() {
    string username = usernameInput->text().toStdString();
    string password = passwordInput->text().toStdString();

    if (username.empty() || password.empty()) {
        errorLabel->setText("Please fill all fields.");
        return;
    }

    User* loggedInUser = findAndLogin(allUsers, userCount, username, password);
    if (loggedInUser == nullptr) {
        errorLabel->setText("Invalid username or password.");
        return;
    }

    MainWindow* mainWin = new MainWindow(loggedInUser, allUsers, userCount);
    mainWin->show();
    this->close();
}

void LoginWindow::onSignupClicked() {
    SignupWindow* signupWin = new SignupWindow(allUsers, userCount);
    signupWin->show();
    this->close();
}