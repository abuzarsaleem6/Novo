
#define _CRT_SECURE_NO_WARNINGS
#include "Notification.h"

#ifdef _WIN32
#include <direct.h>
static void mkdirIfNeeded(const char* path) { _mkdir(path); }
#else
#include <sys/stat.h>
static void mkdirIfNeeded(const char* path) { mkdir(path, 0755); }
#endif

using namespace std;


Notification::Notification() {

    this->message = "";
    this->type = "";
    this->timestamp = "";

}

Notification::Notification(string msg, string t, string time) {

    this->message = msg;
    this->type = t;
    this->timestamp = time;

}

void Notification::saveNotificationToFile(string username) {

    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Notifications");

    string path = "data/Notifications/" + username + "_notif.txt";
    ofstream outFile(path, ios::app);

    if (outFile.is_open()) {

        outFile << type << "|\n"
            << message << "|\n"
            << timestamp << "|\n---\n";
        outFile.close();

    }
}


string Notification::getMessage() const {

    return this->message;

}

string Notification::getType() const {

    return this->type;

}

string Notification::getTimestamp() const {

    return this->timestamp;

}


Notification* NotificationManager::loadAllNotifications(const string& username, int& outCount) {

    outCount = 0;

    string path = "data/Notifications/" + username + "_notif.txt";
    ifstream file(path);

    if (!file.is_open()) {

        cout << "No notifications file found for " << username << endl;
        return nullptr;

    }

    int capacity = 16;
    Notification* notifications = new Notification[capacity];

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        
        size_t p1 = line.find('|');
        if (p1 == string::npos) {
            cout << "WARNING: bad notif line (no sep): " << line << endl;
            continue;
        }

        size_t p2 = line.find('|', p1 + 1);

        if (p2 == string::npos) {
            cout << "WARNING: bad notif line (1 sep): " << line << endl;
            continue; 
        }

        size_t p3 = line.rfind('|');

        if (p3 == p2) { 
            cout << "WARNING: bad notif line (2 sep): " << line << endl;
            continue;
        }

        string typeStr = line.substr(0, p1);
        string msgStr = line.substr(p1 + 1, p2 - p1 - 1);
        string timeStr = line.substr(p2 + 1, p3 - p2 - 1);

        
        while (!typeStr.empty() && (typeStr.back() == ' ' || typeStr.back() == '\r')) {

            typeStr.pop_back();

        }

        while (!msgStr.empty() && (msgStr.back() == ' ' || msgStr.back() == '\r')) {

            msgStr.pop_back(); 

        }

        while (!timeStr.empty() && (timeStr.back() == ' ' || timeStr.back() == '\r')) {

            timeStr.pop_back();

        }

        if (typeStr.empty() || msgStr.empty() || timeStr.empty()) {

            continue;

        }
        
        if (outCount >= capacity) {

            capacity *= 2;
            Notification* tmp = new Notification[capacity];
            for (int i = 0; i < outCount; i++) tmp[i] = notifications[i];
            delete[] notifications;
            notifications = tmp;

        }

        Notification notif(msgStr, typeStr, timeStr);
        notifications[outCount++] = notif;
    }

    file.close();
    cout << "Successfully loaded " << outCount << " notifications." << endl;
    return notifications;
}

void NotificationManager::saveAllNotifications(const string& username, Notification* notifications, int count) {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Notifications");

    string path = "data/Notifications/" + username + "_notif.txt";
    ofstream file(path);   

    if (!file.is_open()) {

        cout << "ERROR: Could not open notification file for writing: " << path << endl;
        return;

    }

    int savedCount = 0;
    for (int i = 0; i < count; i++) {

        file << notifications[i].getType() << "|" << notifications[i].getMessage() << "|" << notifications[i].getTimestamp() << "\n";
        savedCount++;

    }

    file.close();
    cout << "Saved " << savedCount << " notifications for " << username << endl;

}

void NotificationManager::clearAllNotifications(const string& username) {

    string path = "data/Notifications/" + username + "_notif.txt";
    ofstream file(path, ios::trunc);
    file.close();

}