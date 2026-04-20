#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "User.h"

class FollowWindow : public QWidget {
    Q_OBJECT
public:
    // isFollow = true → follow, false → unfollow
    FollowWindow(User* loggedInUser, User**& allUsers, int& userCount,
        bool isFollow = true, QWidget* parent = nullptr);

private slots:
    void onActionClicked();

private:
    User* loggedInUser;
    User**& allUsers;
    int& userCount;
    bool isFollow;
    QLineEdit* usernameInput;
    QLabel* errorLabel;
};