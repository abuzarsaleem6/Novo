#include "SignupWindow.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include <QVBoxLayout>

SignupWindow::SignupWindow(User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), allUsers(allUsers), userCount(userCount) {

    setWindowTitle("NOVO - Sign Up");
    setFixedSize(450, 560);

    setStyleSheet("QWidget { background: qlineargradient("
        "x1:0, y1:0, x2:1, y2:1,"
        "stop:0 #1a1a2e, stop:1 #0f3460); }");

    QWidget* card = new QWidget();
    card->setFixedSize(380, 480);
    card->setStyleSheet(
        "QWidget { background: rgba(255,255,255,0.05);"
        "border-radius: 20px;"
        "border: 1px solid rgba(255,255,255,0.1); }"
    );

    QLabel* title = new QLabel("Create Account");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold;"
        "border: none; background: transparent;");

    QLabel* subtitle = new QLabel("Join NOVO today");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #a0a0ff; font-size: 14px;"
        "border: none; background: transparent;");

    QString inputStyle =
        "QLineEdit {"
        "background: rgba(255,255,255,0.08);"
        "border: 1px solid rgba(255,255,255,0.2);"
        "border-radius: 10px; padding: 12px 16px;"
        "color: white; font-size: 14px;"
        "}"
        "QLineEdit:focus { border: 1px solid #a0a0ff; }";

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username (6-16 chars)");
    usernameInput->setStyleSheet(inputStyle);
    usernameInput->setFixedHeight(48);

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password (8-16 chars)");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(inputStyle);
    passwordInput->setFixedHeight(48);

    bioInput = new QLineEdit();
    bioInput->setPlaceholderText("Bio (max 100 chars)");
    bioInput->setStyleSheet(inputStyle);
    bioInput->setFixedHeight(48);

    errorLabel = new QLabel("");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setStyleSheet("color: #ff6b6b; font-size: 13px;"
        "border: none; background: transparent;");
    errorLabel->setWordWrap(true);

    QPushButton* signupBtn = new QPushButton("Create Account");
    signupBtn->setFixedHeight(48);
    signupBtn->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #f093fb, stop:1 #f5576c);"
        "color: white; font-size: 15px; font-weight: bold;"
        "border-radius: 10px; border: none; }"
        "QPushButton:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #f5576c, stop:1 #f093fb); }"
    );

    QPushButton* backBtn = new QPushButton("Already have an account? Login");
    backBtn->setStyleSheet(
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
    cardLayout->addWidget(bioInput);
    cardLayout->addWidget(errorLabel);
    cardLayout->addWidget(signupBtn);
    cardLayout->addWidget(backBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(card, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(signupBtn, &QPushButton::clicked, this, &SignupWindow::onSignupClicked);
    connect(backBtn, &QPushButton::clicked, this, &SignupWindow::onBackToLoginClicked);
}

void SignupWindow::onSignupClicked() {
    string username = usernameInput->text().toStdString();
    string password = passwordInput->text().toStdString();
    string bio = bioInput->text().toStdString();

    if (username.empty() || password.empty()) {
        errorLabel->setText("Username and password cannot be empty.");
        return;
    }
    if (username.length() < 6 || username.length() > 16) {
        errorLabel->setText("Username must be 6-16 characters.");
        return;
    }
    if (password.length() < 8 || password.length() > 16) {
        errorLabel->setText("Password must be 8-16 characters.");
        return;
    }
    if (bio.length() > 100) {
        errorLabel->setText("Bio must be less than 100 characters.");
        return;
    }
    for (char c : username) {
        if (c == '|' || c == ' ') {
            errorLabel->setText("Username cannot contain '|' or spaces.");
            return;
        }
    }
    for (char c : password) {
        if (c == '|' || c == ' ') {
            errorLabel->setText("Password cannot contain '|' or spaces.");
            return;
        }
    }
    for (char c : bio) {
        if (c == '|') {
            errorLabel->setText("Bio cannot contain '|'.");
            return;
        }
    }
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i]->getUsername() == username) {
            errorLabel->setText("Username already taken.");
            return;
        }
    }

    User* newUser = signUp(allUsers, userCount, username, password, bio);
    if (newUser == nullptr) {
        errorLabel->setText("Signup failed. Try again.");
        return;
    }

    MainWindow* mainWin = new MainWindow(newUser, allUsers, userCount);
    mainWin->show();
    this->close();
}

void SignupWindow::onBackToLoginClicked() {
    LoginWindow* loginWin = new LoginWindow(allUsers, userCount);
    loginWin->show();
    this->close();
}