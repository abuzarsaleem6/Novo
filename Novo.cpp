#include<iostream>
#include<fstream>
#include<string>
#include<QDebug>
#include"User.h"
#include"Post.h"
#include"Comment.h"
#include"SearchEngine.h"
#include <QApplication>
#include "LoginWindow.h"
#include "User.h"
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Load all users
    int userCount = 0;
    ifstream countFile("data/users_list.txt");
    string tempLine;
    while (getline(countFile, tempLine)) userCount++;
    countFile.close();

    User** allUsers = new User * [userCount == 0 ? 1 : userCount];
    loadAllUsers(allUsers, userCount);

    for (int i = 0; i < userCount; i++) {
        allUsers[i]->loadFollowing(allUsers, userCount);
        allUsers[i]->loadFollowers(allUsers, userCount);
    }
    for (int i = 0; i < userCount; i++) {
        allUsers[i]->loadAllPosts();
    }

    LoginWindow* loginWin = new LoginWindow(allUsers, userCount);
    loginWin->show();

    return app.exec();
}