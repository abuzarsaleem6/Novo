// Notification.h
#pragma once
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <iostream>
#include <string>
#include <fstream>
#include <QList>
#include <QDir>
#include <QDebug>

using namespace std;

class Notification {
private:
    string message;
    string type;
    string timestamp;
    bool isRead;

public:
    // ─── CONSTRUCTORS ───
    Notification();
    Notification(string msg, string t, string time);

    // ─── CORE LOGIC ───
    void markAsRead();
    void markAsUnread();
    void saveNotificationToFile(string username);

    // ─── Qt GETTERS ───
    string getMessage() const;
    string getType() const;
    string getTimestamp() const;
    bool getStatus() const;  // Returns isRead status
};

// ─── STATIC HELPER CLASS FOR BATCH OPERATIONS ───
// Used by Qt GUI to load/save multiple notifications at once
class NotificationManager {
public:
  
    static QList<Notification> loadAllNotifications(const string& username);
    static void saveAllNotifications(const string& username, const QList<Notification>& notifications);
    static void clearAllNotifications(const string& username);
    static void markAllAsRead(const string& username);
};

#endif