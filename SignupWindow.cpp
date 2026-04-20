#include "SignupWindow.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>

SignupWindow::SignupWindow(User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), allUsers(allUsers), userCount(userCount) {

    setWindowTitle("NOVO - Sign Up");
    setFixedSize(400, 350);

    QLabel* title = new QLabel("Create Account");
    title->setAlignment(Qt::AlignCenter);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username (6-16 characters)");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password (8-16 characters)");
    passwordInput->setEchoMode(QLineEdit::Password);

    bioInput = new QLineEdit();
    bioInput->setPlaceholderText("Bio (max 100 characters)");

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setWordWrap(true);

    QPushButton* signupBtn = new QPushButton("Sign Up");
    QPushButton* backBtn = new QPushButton("Back to Login");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(bioInput);
    layout->addWidget(errorLabel);
    layout->addWidget(signupBtn);
    layout->addWidget(backBtn);
    setLayout(layout);

    connect(signupBtn, &QPushButton::clicked, this, &SignupWindow::onSignupClicked);
    connect(backBtn, &QPushButton::clicked, this, &SignupWindow::onBackToLoginClicked);
}

void SignupWindow::onSignupClicked() {
    string username = usernameInput->text().toStdString();
    string password = passwordInput->text().toStdString();
    string bio = bioInput->text().toStdString();

    // Validation
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

    // Duplicate check
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i]->getUsername() == username) {
            errorLabel->setText("Username already taken.");
            return;
        }
    }

    // Create user
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