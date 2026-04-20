#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "User.h"

class LikePostWindow : public QWidget {
    Q_OBJECT
public:
    // isLike = true → like, false → unlike
    LikePostWindow(User* loggedInUser, User**& allUsers, int& userCount,
        bool isLike = true, QWidget* parent = nullptr);

private slots:
    void onActionClicked();

private:
    User* loggedInUser;
    User**& allUsers;
    int& userCount;
    bool isLike;
    QLineEdit* postIdInput;
    QLabel* errorLabel;
};