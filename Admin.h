// Admin.h
#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Post.h"
#include <QList>
#include <QString>
#include <QDir>
#include <QDebug>

class Admin : public User {
private:
    QString adminLevel;
    QList<User*> reportedUsers;
    QList<Posts*> reportedPosts;
    int reportedUserCount;
    int reportedPostCount;
    QList<QString> adminNotifications;
    QSet<QString> processedUserReports;
    QSet<QString> processedPostReports;

public:
    // ─── CONSTRUCTORS & DESTRUCTORS ───
    Admin();
    Admin(QString username, QString password, QString bio = "Admin");
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);
    ~Admin();

    // ─── REPORT MANAGEMENT ───
    void receiveUserReport(User* user);
    void receivePostReport(Posts* post);

    // ─── ACTION METHODS (DELETE/BAN) ───
    void deleteUser(User**& allUsers, int& userCount, const QString& username);
    void deletePost(User** allUsers, int userCount, const QString& postId);
    void deleteComment(Posts* post, int commentIndex);
    void banUser(User* user);
    void unbanUser(User* user);

    // ─── REPORT REVIEW ───
    void reviewReports(User**& allUsers, int& userCount);
    void checkReportThresholds(User* user, const QString& username);

    // ─── GETTERS FOR Qt GUI ───
    QList<User*> getReportedUsers() const;
    QList<Posts*> getReportedPosts() const;
    int getReportedUserCount() const;
    int getReportedPostCount() const;

    // ─── NOTIFICATION MANAGEMENT ───
    void addNotification(const QString& message);
    QList<QString> getAllNotifications() const;
    void clearNotifications();

    // ─── FILE I/O ───
    void saveReportsToFile();
    void loadReportsFromFile(User**& allUsers, int& userCount);
    void saveAdminToFile();
    void loadAdminFromFile();
    void saveProcessedReports();
    void loadProcessedReports();

    // ─── DISPLAY ───
    void displayAdminDashboard();
    void displayReportedUsers();
    void displayReportedPosts();
};

#endif