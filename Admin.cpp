#define _CRT_SECURE_NO_WARNINGS
#include "Admin.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

// ─── HELPER: Current Timestamp ───
static string currentTimestamp() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// ─── HELPER: Directory Creation ───
#ifdef _WIN32
#include <direct.h>
static void mkdirIfNeeded(const char* path) { _mkdir(path); }
#else
#include <sys/stat.h>
static void mkdirIfNeeded(const char* path) { mkdir(path, 0755); }
#endif

// ════════════════════════════════════════
//   CONSTRUCTORS & DESTRUCTOR
// ════════════════════════════════════════


Admin::Admin(string username, string password, string bio)
    : User(username, password, bio) {
}
   


Admin::~Admin() {}

void Admin::receiveUserReport(User* user) {
    if (!user) return;
    addNotification("New report received for user: @" + user->getUsername());
    saveReportsToFile();
    checkReportThresholds(user, user->getUsername());
}


void Admin::receivePostReport(Posts* post) {
    if (!post) return;
    addNotification("New report received for post: " + post->getPostId()
        + " by @" + post->getCreatorUsername());
    saveReportsToFile();
}

void Admin::deleteUser(User**& allUsers, int& userCount, const string& username) {
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i] && allUsers[i]->getUsername() == username) {
            allUsers[i]->deleteAccount(allUsers, userCount);
            addNotification("User deleted: @" + username);
            return;
        }
    }
}
void Admin::deletePost(User** allUsers, int userCount, const string& postId) {
    for (int i = 0; i < userCount; i++) {
        if (!allUsers[i]) continue;
        Posts* post = allUsers[i]->getPostById(postId);
        if (post) {
            allUsers[i]->deletePost(postId);
            addNotification("Post deleted: " + postId);
            return;
        }
    }
}

void Admin::reviewReports(User**& allUsers, int& userCount) {
    cout << "Total reported users: " << getReportedUserCount() << endl;
    cout << "Total reported posts: " << getReportedPostCount(allUsers, userCount) << endl;
}
void Admin::checkReportThresholds(User* user, const string& username) {
    if (!user) return;
    if (user->getIsReportedCount() >= 3) {
        addNotification("ALERT: User @" + username + " reached report threshold!");
    }
}
int Admin::getReportedUserCount() const {
    int count = 0;
    ifstream f("data/Admin/reported_users.txt");
    string line;
    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.find("username|") == 0) count++;
    }
    return count;
}
int Admin::getReportedPostCount(User** allUsers, int userCount) const {
    int count = 0;
    for (int i = 0; i < userCount; i++) {
        if (!allUsers[i]) continue;
        int pc = allUsers[i]->getPostCount();
        for (int j = 0; j < pc; j++) {
            Posts* p = allUsers[i]->getPostByIndex(j);
            if (p && p->getReportCount() >= 1) count++;
        }
    }
    return count;
}
void Admin::addNotification(const string& message) {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");
    ofstream nf("data/Admin/admin_notifications.txt", ios::app);
    if (nf.is_open()) {
        nf << currentTimestamp() << "|admin|" << message << "\n";
        nf.close();
    }
}

Notification* Admin::getAllNotifications(int& outCount) const {
    outCount = getAdminNotifCount();
    if (outCount == 0) return nullptr;

    Notification* arr = new Notification[outCount];
    int idx = 0;

    ifstream nf("data/Admin/admin_notifications.txt");
    string line;
    while (getline(nf, line) && idx < outCount) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        if (p1 == string::npos) continue;
        size_t p2 = line.find('|', p1 + 1);
        if (p2 == string::npos) continue;

        string ts = line.substr(0, p1);
        string type = line.substr(p1 + 1, p2 - p1 - 1);
        string msg = line.substr(p2 + 1);

      
        while (!ts.empty() && ts.back() == '\r') ts.pop_back();
        while (!type.empty() && type.back() == '\r') type.pop_back();
        while (!msg.empty() && msg.back() == '\r') msg.pop_back();

        if (ts.empty() || msg.empty()) continue;
        arr[idx++] = Notification(msg, type, ts);
    }

    outCount = idx;
    return arr;
}
int Admin::getAdminNotifCount() const {
    int count = 0;
    ifstream f("data/Admin/admin_notifications.txt");
    string line;
    while (getline(f, line))
        if (!line.empty() && line != "\r") count++;
    return count;
}


void Admin::viewAllNotifications() const {
    int count = 0;
    Notification* notifs = getAllNotifications(count);
    if (count == 0) {
        cout << "No notifications." << endl;
        delete[] notifs;
        return;
    }
    // Qt mein: count-1 se 0 tak loop — newest first
    for (int i = count - 1; i >= 0; i--) {
        cout << "[" << notifs[i].getTimestamp() << "] "
            << notifs[i].getType() << " — "
            << notifs[i].getMessage() << endl;
    }
    delete[] notifs;
}

// Notifications file khali kar do — ios::trunc se
void Admin::clearNotifications() {
    ofstream f("data/Admin/admin_notifications.txt", ios::trunc);
    f.close();
}


void Admin::saveReportsToFile() {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");

    ofstream out("data/Admin/reported_users.txt", ios::out);  
    if (!out.is_open()) return;
    out.close();
}

void Admin::loadReportsFromFile(User**& allUsers, int& userCount) {
    if (!allUsers || userCount <= 0) return;
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i] && allUsers[i]->getPostCount() == 0)
            allUsers[i]->loadAllPosts();
    }
}


void Admin::saveAdminToFile() {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");
    ofstream file("data/Admin/adminInfo.txt");
    if (file.is_open()) {
        file << "username|" << getUsername() << "\n";
           
        file.close();
    }
}

void Admin::loadAdminFromFile() {
    ifstream file("data/Admin/adminInfo.txt");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t sep = line.find('|');
        if (sep == string::npos) continue;
        string key = line.substr(0, sep);
        string val = line.substr(sep + 1);
        
    }
    file.close();
}

