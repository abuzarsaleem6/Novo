#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "User.h"

class SignupWindow : public QWidget {
    Q_OBJECT

public:
    SignupWindow(User**& allUsers, int& userCount, QWidget* parent = nullptr);

private slots:
    void onSignupClicked();
    void onBackToLoginClicked();

private:
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QLineEdit* bioInput;
    QLabel* errorLabel;
    User**& allUsers;
    int& userCount;
};