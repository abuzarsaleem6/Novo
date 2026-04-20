#include "FollowWindow.h"
#include <QVBoxLayout>

FollowWindow::FollowWindow(User* loggedInUser, User**& allUsers, int& userCount,
    bool isFollow, QWidget* parent)
    : QWidget(parent), loggedInUser(loggedInUser), allUsers(allUsers),
    userCount(userCount), isFollow(isFollow) {

    setWindowTitle(isFollow ? "Follow User" : "Unfollow User");
    setFixedSize(400, 200);

    QLabel* title = new QLabel(isFollow ? "Follow a User" : "Unfollow a User");
    title->setAlignment(Qt::AlignCenter);

    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Enter username");

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* actionBtn = new QPushButton(isFollow ? "Follow" : "Unfollow");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(usernameInput);
    layout->addWidget(errorLabel);
    layout->addWidget(actionBtn);
    setLayout(layout);

    connect(actionBtn, &QPushButton::clicked, this, &FollowWindow::onActionClicked);
}

void FollowWindow::onActionClicked() {
    string targetUsername = usernameInput->text().toStdString();

    if (targetUsername.empty()) {
        errorLabel->setText("Please enter a username.");
        return;
    }

    if (isFollow) {
        if (targetUsername == loggedInUser->getUsername()) {
            errorLabel->setText("You cannot follow yourself.");
            return;
        }
        User* target = nullptr;
        for (int i = 0; i < userCount; i++) {
            if (allUsers[i]->getUsername() == targetUsername) {
                target = allUsers[i];
                break;
            }
        }
        if (target == nullptr) {
            errorLabel->setText("User not found.");
            return;
        }
        if (loggedInUser->isFollowing(targetUsername)) {
            errorLabel->setText("Already following.");
            return;
        }
        loggedInUser->followUser(target, allUsers, userCount);
        errorLabel->setStyleSheet("color: green;");
        errorLabel->setText("Followed " + QString::fromStdString(targetUsername));
    }
    else {
        if (!loggedInUser->isFollowing(targetUsername)) {
            errorLabel->setText("You are not following this user.");
            return;
        }
        loggedInUser->unfollowUser(targetUsername);
        errorLabel->setStyleSheet("color: green;");
        errorLabel->setText("Unfollowed " + QString::fromStdString(targetUsername));
    }
}