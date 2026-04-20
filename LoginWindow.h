#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "User.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(User**& allUsers, int& userCount, QWidget* parent = nullptr);

private slots:
    void onLoginClicked();
    void onSignupClicked();

private:
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QLabel* errorLabel;
    User**& allUsers;
    int& userCount;
};