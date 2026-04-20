#include "MainWindow.h"
#include "LoginWindow.h"
#include "ProfileWindow.h"
#include "UpdateBioWindow.h"
#include "UpdatePasswordWindow.h"
#include "FollowWindow.h"
#include "CreatePostWindow.h"
#include "DisplayPostsWindow.h"
#include "LikePostWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(User* loggedInUser, User**& allUsers, int& userCount, QWidget* parent)
    : QWidget(parent), loggedInUser(loggedInUser), allUsers(allUsers), userCount(userCount) {

    setWindowTitle("NOVO");
    setFixedSize(400, 550);

    welcomeLabel = new QLabel("Logged in as: " +
        QString::fromStdString(loggedInUser->getUsername()));
    welcomeLabel->setAlignment(Qt::AlignCenter);

    QPushButton* profileBtn = new QPushButton("1. Display Profile");
    QPushButton* bioBtn = new QPushButton("2. Update Bio");
    QPushButton* passwordBtn = new QPushButton("3. Update Password");
    QPushButton* followBtn = new QPushButton("4. Follow a User");
    QPushButton* unfollowBtn = new QPushButton("5. Unfollow a User");
    QPushButton* createPostBtn = new QPushButton("6. Create a Post");
    QPushButton* displayPostsBtn = new QPushButton("7. Display My Posts");
    QPushButton* likeBtn = new QPushButton("8. Like a Post");
    QPushButton* unlikeBtn = new QPushButton("9. Unlike a Post");
    QPushButton* logoutBtn = new QPushButton("Logout");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(welcomeLabel);
    layout->addWidget(profileBtn);
    layout->addWidget(bioBtn);
    layout->addWidget(passwordBtn);
    layout->addWidget(followBtn);
    layout->addWidget(unfollowBtn);
    layout->addWidget(createPostBtn);
    layout->addWidget(displayPostsBtn);
    layout->addWidget(likeBtn);
    layout->addWidget(unlikeBtn);
    layout->addWidget(logoutBtn);
    setLayout(layout);

    connect(profileBtn, &QPushButton::clicked, this, &MainWindow::onDisplayProfileClicked);
    connect(bioBtn, &QPushButton::clicked, this, &MainWindow::onUpdateBioClicked);
    connect(passwordBtn, &QPushButton::clicked, this, &MainWindow::onUpdatePasswordClicked);
    connect(followBtn, &QPushButton::clicked, this, &MainWindow::onFollowClicked);
    connect(unfollowBtn, &QPushButton::clicked, this, &MainWindow::onUnfollowClicked);
    connect(createPostBtn, &QPushButton::clicked, this, &MainWindow::onCreatePostClicked);
    connect(displayPostsBtn, &QPushButton::clicked, this, &MainWindow::onDisplayPostsClicked);
    connect(likeBtn, &QPushButton::clicked, this, &MainWindow::onLikePostClicked);
    connect(unlikeBtn, &QPushButton::clicked, this, &MainWindow::onUnlikePostClicked);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
}

void MainWindow::onDisplayProfileClicked() {
    ProfileWindow* w = new ProfileWindow(loggedInUser);
    w->show();
}

void MainWindow::onUpdateBioClicked() {
    UpdateBioWindow* w = new UpdateBioWindow(loggedInUser);
    w->show();
}

void MainWindow::onUpdatePasswordClicked() {
    UpdatePasswordWindow* w = new UpdatePasswordWindow(loggedInUser);
    w->show();
}

void MainWindow::onFollowClicked() {
    FollowWindow* w = new FollowWindow(loggedInUser, allUsers, userCount);
    w->show();
}

void MainWindow::onUnfollowClicked() {
    FollowWindow* w = new FollowWindow(loggedInUser, allUsers, userCount, false);
    w->show();
}

void MainWindow::onCreatePostClicked() {
    CreatePostWindow* w = new CreatePostWindow(loggedInUser);
    w->show();
}

void MainWindow::onDisplayPostsClicked() {
    DisplayPostsWindow* w = new DisplayPostsWindow(loggedInUser);
    w->show();
}

void MainWindow::onLikePostClicked() {
    LikePostWindow* w = new LikePostWindow(loggedInUser, allUsers, userCount, true);
    w->show();
}

void MainWindow::onUnlikePostClicked() {
    LikePostWindow* w = new LikePostWindow(loggedInUser, allUsers, userCount, false);
    w->show();
}

void MainWindow::onLogoutClicked() {
    loggedInUser->logOut();
    LoginWindow* loginWin = new LoginWindow(allUsers, userCount);
    loginWin->show();
    this->close();
}