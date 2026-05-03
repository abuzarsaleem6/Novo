#define _CRT_SECURE_NO_WARNINGS
#include "Admin.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

// getting current time
static string currentTimestamp() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// directory creation helper
#ifdef _WIN32
#include <direct.h>
static void mkdirIfNeeded(const char* path) { (void)_mkdir(path); }
#else
#include <sys/stat.h>
static void mkdirIfNeeded(const char* path) { (void)mkdir(path, 0755); }
#endif

// ─── CONSTRUCTORS AND DESTRUCTOR ───

Admin::Admin() : User() {
    adminLevel = "Super Admin";
    cout << "Admin created (default)" << endl;
}

Admin::Admin(string username, string password, string bio)
    : User(username, password, bio) {
    adminLevel = "Super Admin";
    cout << "Admin created: " << username << endl;
}

Admin::Admin(const Admin& other) : User(other) {
    adminLevel = other.adminLevel;
}

Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        User::operator=(other);
        adminLevel = other.adminLevel;
    }
    return *this;
}

Admin::~Admin() {
    cout << "Admin destroyed" << endl;
}

// ─── REPORT HANDLING ───

void Admin::receiveUserReport(User* user) {
    if (!user) return;
    addNotification("New report received for user: @" + user->getUsername());
    saveReportsToFile();
    checkReportThresholds(user, user->getUsername());
    cout << "User report received: " << user->getUsername() << endl;
}

void Admin::receivePostReport(Posts* post) {
    if (!post) return;
    addNotification("New report received for post: " + post->getPostId()
        + " by @" + post->getCreatorUsername());
    saveReportsToFile();
    cout << "Post report received: " << post->getPostId() << endl;
}

// ─── DELETING USERS, POSTS, COMMENTS ───

void Admin::deleteUser(User**& allUsers, int& userCount, const string& username) {
    cout << "Admin: Deleting user " << username << endl;
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i] && allUsers[i]->getUsername() == username) {
            allUsers[i]->deleteAccount(allUsers, userCount);
            addNotification("User deleted: @" + username);
            cout << "User deleted successfully: " << username << endl;
            return;
        }
    }
    cout << "ERROR: User not found for deletion: " << username << endl;
}

void Admin::deletePost(User** allUsers, int userCount, const string& postId) {
    cout << "Admin: Deleting post " << postId << endl;
    for (int i = 0; i < userCount; i++) {
        if (!allUsers[i]) {
            continue;
        }
        Posts* post = allUsers[i]->getPostById(postId);
        if (post) {
            allUsers[i]->deletePost(postId);
            addNotification("Post deleted: " + postId);
            cout << "Post deleted successfully: " << postId << endl;
            return;
        }
    }
    cout << "ERROR: Post not found for deletion: " << postId << endl;
}

void Admin::deleteComment(Posts* post, int commentIndex) {
    if (!post) return;
    if (commentIndex < 0 || commentIndex >= post->getCommentsCount()) {
        cout << "ERROR: Invalid comment index: " << commentIndex << endl;
        return;
    }
    post->deleteCommentAsAdmin(commentIndex);
    char buf[32];
    sprintf(buf, "%d", commentIndex);
    addNotification(string("Comment deleted at index ") + buf);
    cout << "Comment deleted by admin at index: " << commentIndex << endl;
}

// ─── REVIEWING REPORTS ───

void Admin::reviewReports(User**& allUsers, int& userCount) {
    cout << "---REVIEWING ALL REPORTS ---" << endl;
    cout << "Total reported users: " << getReportedUserCount() << endl;
    cout << "Total reported posts: " << getReportedPostCount(allUsers, userCount) << endl;
}

void Admin::checkReportThresholds(User* user, const string& username) {
    addNotification("ALERT: User @" + username + " reached report threshold!");
}

// ─── GETTERS — READ DIRECTLY FROM FILE ───

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

// ─── NOTIFICATION HANDLING ───

void Admin::addNotification(const string& message) {
    string ts = currentTimestamp();
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");
    ofstream nf("data/Admin/admin_notifications.txt", ios::app);
    if (nf.is_open()) {
        nf << ts << "|admin|" << message << "\n";
        nf.close();
    }
    cout << "Admin notification: " << message << endl;
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
    cout << "--- ADMIN NOTIFICATIONS ---" << endl;
    for (int i = 0; i < count; i++) {
        cout << notifs[i].getMessage()
            << " | Type: " << notifs[i].getType()
            << " | Time: " << notifs[i].getTimestamp() << endl;
    }
    delete[] notifs;
}

void Admin::clearNotifications() {
    
    ofstream f("data/Admin/admin_notifications.txt", ios::trunc);
    f.close();
    cout << "Admin notifications cleared" << endl;
}

// ─── FILE HANDLING ───

void Admin::saveReportsToFile() {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");

    ofstream postsFile("data/Admin/reported_posts.txt");
    if (postsFile.is_open()) {
        postsFile.close();
    }
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
        file << "username|" << getUsername() << "\n"
            << "adminLevel|" << adminLevel << "\n";
        file.close();
        cout << "Admin info saved" << endl;
    }
}

void Admin::loadAdminFromFile() {
    ifstream file("data/Admin/adminInfo.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            size_t sep = line.find('|');
            if (sep == string::npos) continue;
            string key = line.substr(0, sep);
            string val = line.substr(sep + 1);
            if (key == "adminLevel") adminLevel = val;
        }
        file.close();
        cout << "Admin info loaded" << endl;
    }
}

// ─── DISPLAY FUNCTIONS ───

void Admin::displayAdminDashboard(User** allUsers, int userCount) {
    cout << "|---- ADMIN DASHBOARD ----|" << endl;
    cout << "Username: " << getUsername() << endl;
    cout << "Admin Level: " << adminLevel << endl;
    cout << "Reported Users: " << getReportedUserCount() << endl;
    cout << "Reported Posts: " << getReportedPostCount(allUsers, userCount) << endl;
    cout << "Notifications: " << getAdminNotifCount() << endl;
    cout << "|-------------------------|" << endl;
}

void Admin::displayReportedUsers() {
    cout << "|---- REPORTED USERS ----|" << endl;
    ifstream f("data/Admin/reported_users.txt");
    string line;
    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.find("username|") != 0) continue;
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;
        string uname = line.substr(p1 + 1, p2 - p1 - 1);
        string rcount = line.substr(p3 + 1);
        cout << "User: " << uname << " | Reports: " << rcount << endl;
    }
}

void Admin::displayReportedPosts(User** allUsers, int userCount) {
    cout << "--- REPORTED POSTS ---" << endl;
    for (int i = 0; i < userCount; i++) {
        if (!allUsers[i]) continue;
        int pc = allUsers[i]->getPostCount();
        for (int j = 0; j < pc; j++) {
            Posts* p = allUsers[i]->getPostByIndex(j);
            if (p && p->getReportCount() >= 1)
                cout << "Post: " << p->getPostId()
                << " | Reports: " << p->getReportCount() << endl;
        }
    }
}