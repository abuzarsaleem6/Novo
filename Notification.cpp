#include "Notification.h"
#include <QDir>

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

void Notification::saveNotificationToFile(string username) {
    QDir().mkpath("data/Notifications");

    string path = "data/Notifications/" + username + "_notif.txt";
    ofstream outFile(path, ios::app);
    if (outFile.is_open()) {
        outFile << message << "|" << type << "|" << timestamp << "|" << isRead << "\n";
        outFile.close();
    }
    else {
        qDebug() << "Error: Could not open notification file for"
            << QString::fromStdString(username);
    }
}

string Notification::getMessage()   const {
    return this->message;
}
string Notification::getType()      const {
    return this->type; 
}
string Notification::getTimestamp() const { 
    return this->timestamp;
}
bool   Notification::getStatus()    const { 
    return this->isRead; 
}