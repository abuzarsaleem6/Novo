#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "User.h"
#include "Post.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(User* loggedInUser, User**& allUsers, int& userCount, QWidget* parent = nullptr);

private slots:
    void onFollowersClicked();
    void onFollowingClicked();
    void onUpdateBioClicked();
    void onUpdatePasswordClicked();
    void onLogoutClicked();
    void onCreatePostClicked();

private:
    User* loggedInUser;
    User**& allUsers;
    int& userCount;

    QLabel* followersLabel;
    QLabel* followingLabel;
    QLabel* postsLabel;
    QVBoxLayout* postsLayout;
    QWidget* postsContainer;

    void loadPosts();
    QWidget* createPostCard(Posts* post);
};