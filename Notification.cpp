// Notification.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Notification.h"

using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  NOTIFICATION CLASS IMPLEMENTATION
// ══════════════════════════════════════════════════════════════════════════════

Notification::Notification() {
    this->message = "";
    this->type = "";
    this->timestamp = "";
    this->isRead = false;
}

Notification::Notification(string msg, string t, string time) {
    this->message = msg;
    this->type = t;
    this->timestamp = time;
    this->isRead = false;
}

void Notification::markAsRead() {
    this->isRead = true;
}

void Notification::markAsUnread() {
    this->isRead = false;
}

void Notification::saveNotificationToFile(string username) {
    QDir().mkpath("data/Notifications");
    string path = "data/Notifications/" + username + "_notif.txt";
    ofstream outFile(path, ios::app);

    if (outFile.is_open()) {
       
        outFile << type << "|\n" << message << "|\n"  << timestamp << "|\n" <<
            (isRead ? "1|" : "0|") << "\n---\n"; 
        outFile.close();
    }
}

// ─── GETTERS ───
string Notification::getMessage() const {
    return this->message;
}

string Notification::getType() const {
    return this->type;
}

string Notification::getTimestamp() const {
    return this->timestamp;
}

bool Notification::getStatus() const {
    return this->isRead;
}

// ══════════════════════════════════════════════════════════════════════════════
//  NOTIFICATION MANAGER - STATIC HELPER CLASS
//  Used by Qt GUI to batch load/save notifications
// ══════════════════════════════════════════════════════════════════════════════

QList<Notification> NotificationManager::loadAllNotifications(const string& username) {
    QList<Notification> notifications;

    QString path = "data/Notifications/" + QString::fromStdString(username) + "_notif.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No notifications file found for" << QString::fromStdString(username);
        return notifications;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // Format: type|message|timestamp|isRead
        // Use indexOf to split on FIRST 3 pipes only — protects against | in message
        int p1 = line.indexOf('|');
        if (p1 == -1) { qDebug() << "WARNING: bad notif line (no sep):" << line; continue; }

        int p2 = line.indexOf('|', p1 + 1);
        if (p2 == -1) { qDebug() << "WARNING: bad notif line (1 sep):" << line; continue; }

        int p3 = line.lastIndexOf('|');  // last pipe = isRead field
        if (p3 == p2) { qDebug() << "WARNING: bad notif line (2 sep):" << line; continue; }

        QString typeStr = line.mid(0, p1).trimmed();
        QString msgStr = line.mid(p1 + 1, p2 - p1 - 1).trimmed();
        QString timeStr = line.mid(p2 + 1, p3 - p2 - 1).trimmed();
        QString isReadStr = line.mid(p3 + 1).trimmed();

        if (typeStr.isEmpty() || msgStr.isEmpty() || timeStr.isEmpty()) continue;

        Notification notif(msgStr.toStdString(), typeStr.toStdString(), timeStr.toStdString());
        if (isReadStr == "1") notif.markAsRead();

        notifications.append(notif);
    }

    file.close();
    qDebug() << "Successfully loaded" << notifications.size() << "notifications.";
    return notifications;
}

void NotificationManager::saveAllNotifications(const string& username, const QList<Notification>& notifications) {
    QDir().mkpath("data/Notifications");

    QString path = "data/Notifications/" + QString::fromStdString(username) + "_notif.txt";
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "ERROR: Could not open notification file for writing:" << path;
        return;
    }

    QTextStream out(&file);
    int savedCount = 0;

    // Write each notification to file
    for (const auto& notif : notifications) {
        out << QString::fromStdString(notif.getType()) << "|"
            << QString::fromStdString(notif.getMessage()) << "|"
            << QString::fromStdString(notif.getTimestamp()) << "|"
            << (notif.getStatus() ? "1" : "0") << "\n";
        savedCount++;
    }

    file.close();
    qDebug() << "Saved" << savedCount << "notifications for" << QString::fromStdString(username);
}
void NotificationManager::clearAllNotifications(const string& username) {
    QString path = "data/Notifications/" + QString::fromStdString(username) + "_notif.txt";
    QFile::remove(path); 
}
void NotificationManager::markAllAsRead(const string& username) {
    // Load all notifications
    QList<Notification> notifications = loadAllNotifications(username);

    // Mark each as read
    for (auto& notif : notifications) {
        notif.markAsRead();
    }

    // Save back to file
    saveAllNotifications(username, notifications);

    qDebug() << "All notifications marked as read for" << QString::fromStdString(username);
}