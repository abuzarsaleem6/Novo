// Admin.h
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
    int           reportedUserCount;
    int           reportedUserCapacity;

    Posts** reportedPosts;
    int           reportedPostCount;
    int           reportedPostCapacity;

    Notification* adminNotifications;
    int           adminNotifCount;
    int           adminNotifCapacity;

    void expandReportedUsers();
    void expandReportedPosts();
    void expandNotifications();

public:
    // ─── CONSTRUCTORS & DESTRUCTORS ───
    Admin();
    Admin(string username, string password, string bio = "Admin");
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);
    ~Admin();

    // ─── REPORT MANAGEMENT ───
    void receiveUserReport(User* user);
    void receivePostReport(Posts* post);

    // ─── ACTION METHODS (DELETE/BAN) ───
    void deleteUser(User**& allUsers, int& userCount, const string& username);
    void deletePost(User** allUsers, int userCount, const string& postId);
    void deleteComment(Posts* post, int commentIndex);

    // ─── REPORT REVIEW ───
    void reviewReports(User**& allUsers, int& userCount);
    void checkReportThresholds(User* user, const string& username);

    // ─── GETTERS ───
    User** getReportedUsers()     const;
    Posts** getReportedPosts()     const;
    int           getReportedUserCount() const;
    int           getReportedPostCount() const;

    // ─── NOTIFICATION MANAGEMENT ───
    void          addNotification(const string& message);
    Notification* getAllNotifications()  const;
    int           getAdminNotifCount()   const;
    void          clearNotifications();
    void          viewAllNotifications() const;

    // ─── FILE I/O ───
    void saveReportsToFile();
    void loadReportsFromFile(User**& allUsers, int& userCount);
    void saveAdminToFile();
    void loadAdminFromFile();

    // ─── DISPLAY ───
    void displayAdminDashboard();
    void displayReportedUsers();
    void displayReportedPosts();
};

#endif