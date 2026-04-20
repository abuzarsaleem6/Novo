#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "User.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(User* loggedInUser, User**& allUsers, int& userCount, QWidget* parent = nullptr);

private slots:
    void onDisplayProfileClicked();
    void onUpdateBioClicked();
    void onUpdatePasswordClicked();
    void onFollowClicked();
    void onUnfollowClicked();
    void onCreatePostClicked();
    void onDisplayPostsClicked();
    void onLikePostClicked();
    void onUnlikePostClicked();
    void onLogoutClicked();

private:
    User* loggedInUser;
    User**& allUsers;
    int& userCount;
    QLabel* welcomeLabel;
};