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


public:
    // constructors and destructor
  
    Admin(string username, string password, string bio = "Admin");
    
    ~Admin();

    // report handling
    void receiveUserReport(User* user);
    void receivePostReport(Posts* post);

    // deleting users, posts, comments
    void deleteUser(User**& allUsers, int& userCount, const string& username);
    void deletePost(User** allUsers, int userCount, const string& postId);


    // reviewing reports
    void reviewReports(User**& allUsers, int& userCount);
    void checkReportThresholds(User* user, const string& username);

    // getters — read directly from file, no arrays
    int getReportedUserCount() const;
    int getReportedPostCount(User** allUsers, int userCount) const;

    // notification handling
    void addNotification(const string& message);
    Notification* getAllNotifications(int& outCount) const;
    int getAdminNotifCount() const;
    void clearNotifications();
    void viewAllNotifications() const;

    // file handling
    void saveReportsToFile();
    void loadReportsFromFile(User**& allUsers, int& userCount);
    void saveAdminToFile();
    void loadAdminFromFile();
};

#endif