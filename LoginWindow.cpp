#include "LoginWindow.h"
#include "SignupWindow.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginWindow::LoginWindow(User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), allUsers(allUsers), userCount(userCount) {

    setWindowTitle("NOVO - Login");
    setFixedSize(400, 300);

    QLabel* title = new QLabel("Welcome to NOVO");
    title->setAlignment(Qt::AlignCenter);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Username");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* loginBtn = new QPushButton("Login");
    QPushButton* signupBtn = new QPushButton("Sign Up");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(errorLabel);
    layout->addWidget(loginBtn);
    layout->addWidget(signupBtn);
    setLayout(layout);

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