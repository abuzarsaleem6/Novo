#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Post.h"
#include "Notification.h"
#include <string>
#include <fstream>

using namespace std;

class Admin : public User {
private:
    string adminLevel;

    User** reportedUsers;
    int reportedUserCount;
    int reportedUserCapacity;

    Posts** reportedPosts;
    int reportedPostCount;
    int reportedPostCapacity;

    Notification* adminNotifications;
    int adminNotifCount;
    int adminNotifCapacity;

    void expandReportedUsers();
    void expandReportedPosts();
    void expandNotifications();

public:
    //constructor n destruxtor
    Admin();
    Admin(string username, string password, string bio = "Admin");
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);
    ~Admin();

    //report handling
    void receiveUserReport(User* user);
    void receivePostReport(Posts* post);

	//deleteing users, posts, comments
    void deleteUser(User**& allUsers, int& userCount, const string& username);
    void deletePost(User** allUsers, int userCount, const string& postId);
    void deleteComment(Posts* post, int commentIndex);

	//reviewing reports
    void reviewReports(User**& allUsers, int& userCount);
    void checkReportThresholds(User* user, const string& username);

    //getters
    User** getReportedUsers()     const;
    Posts** getReportedPosts()     const;
    int getReportedUserCount() const;
    int getReportedPostCount() const;

    //notification handling
    void addNotification(const string& message);
    Notification* getAllNotifications()  const;
    int getAdminNotifCount()   const;
    void clearNotifications();
    void viewAllNotifications() const;

    //file handling
    void saveReportsToFile();
    void loadReportsFromFile(User**& allUsers, int& userCount);
    void saveAdminToFile();
    void loadAdminFromFile();

	//display functions
    void displayAdminDashboard();
    void displayReportedUsers();
    void displayReportedPosts();
};

#endif