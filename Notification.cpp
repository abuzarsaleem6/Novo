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
        // Format: type|message|timestamp|isRead
        outFile << type << "|" << message << "|" << timestamp << "|" << (isRead ? 1 : 0) << "\n";
        outFile.close();
        qDebug() << "Notification saved for user:" << QString::fromStdString(username);
    }
    else {
        qDebug() << "ERROR: Could not open notification file for" << QString::fromStdString(username);
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
        return notifications;  // Return empty list
    }

    QTextStream in(&file);
    int loadedCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Skip empty lines
        if (line.isEmpty()) continue;

        // Parse line: type|message|timestamp|isRead
        QStringList parts = line.split("|");

        // Validate format (must have 4 parts)
        if (parts.size() < 4) {
            qDebug() << "WARNING: Invalid notification format:" << line;
            continue;
        }

        try {
            QString typeStr = parts[0].trimmed();
            QString msgStr = parts[1].trimmed();
            QString timeStr = parts[2].trimmed();
            QString isReadStr = parts[3].trimmed();

            // Skip if any field is empty
            if (typeStr.isEmpty() || msgStr.isEmpty() || timeStr.isEmpty()) {
                qDebug() << "WARNING: Empty notification fields:" << line;
                continue;
            }

            // ✅ CREATE NOTIFICATION OBJECT FROM FILE DATA
            Notification notif(
                msgStr.toStdString(),
                typeStr.toStdString(),
                timeStr.toStdString()
            );

            // Mark as read if saved as read
            if (isReadStr == "1") {
                notif.markAsRead();
            }

            // Add to list
            notifications.append(notif);
            loadedCount++;

        }
        catch (const exception& e) {
            qDebug() << "ERROR parsing notification:" << e.what();
        }
    }

    file.close();
    qDebug() << "Loaded" << loadedCount << "notifications for" << QString::fromStdString(username);
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