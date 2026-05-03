// Admin.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Admin.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

//getting current time
static string currentTimestamp() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

//directory creation helper (cross-platform)
#ifdef _WIN32
#include <direct.h>
static void mkdirIfNeeded(const char* path) { _mkdir(path); }
#else
#include <sys/stat.h>
static void mkdirIfNeeded(const char* path) { mkdir(path, 0755); }
#endif

//helper functions to expand dynamic arrays when capacity is reached
void Admin::expandReportedUsers() {
    int newCap = reportedUserCapacity * 2;
    User** tmp = new User * [newCap];

    for (int i = 0; i < reportedUserCount; i++)
        tmp[i] = reportedUsers[i];

    delete[] reportedUsers;
    reportedUsers = tmp;
    reportedUserCapacity = newCap;
}

void Admin::expandReportedPosts() {
    int newCap = reportedPostCapacity * 2;
    Posts** tmp = new Posts * [newCap];

    for (int i = 0; i < reportedPostCount; i++) 
        tmp[i] = reportedPosts[i];

    delete[] reportedPosts;
    reportedPosts = tmp;
    reportedPostCapacity = newCap;
}

void Admin::expandNotifications() {
    int newCap = adminNotifCapacity * 2;
    Notification* tmp = new Notification[newCap];

    for (int i = 0; i < adminNotifCount; i++) 
        tmp[i] = adminNotifications[i];

    delete[] adminNotifications;
    adminNotifications = tmp;
    adminNotifCapacity = newCap;
}

//constructors and destructor

Admin::Admin() : User() {
    adminLevel = "Super Admin";

    reportedUserCapacity = 10;
    reportedUserCount = 0;
    reportedUsers = new User * [reportedUserCapacity];

    reportedPostCapacity = 10;
    reportedPostCount = 0;
    reportedPosts = new Posts * [reportedPostCapacity];

    adminNotifCapacity = 10;
    adminNotifCount = 0;
    adminNotifications = new Notification[adminNotifCapacity];

    cout << "Admin created (default)" << endl;
}

Admin::Admin(string username, string password, string bio)
    : User(username, password, bio) {
    adminLevel = "Super Admin";

    reportedUserCapacity = 10;
    reportedUserCount = 0;
    reportedUsers = new User * [reportedUserCapacity];

    reportedPostCapacity = 10;
    reportedPostCount = 0;
    reportedPosts = new Posts * [reportedPostCapacity];

    adminNotifCapacity = 10;
    adminNotifCount = 0;
    adminNotifications = new Notification[adminNotifCapacity];

    cout << "Admin created: " << username << endl;
}

Admin::Admin(const Admin& other) : User(other) {
    adminLevel = other.adminLevel;

    reportedUserCapacity = other.reportedUserCapacity;
    reportedUserCount = other.reportedUserCount;
    reportedUsers = new User * [reportedUserCapacity];

    for (int i = 0; i < reportedUserCount; i++)
        reportedUsers[i] = other.reportedUsers[i];

    reportedPostCapacity = other.reportedPostCapacity;
    reportedPostCount = other.reportedPostCount;
    reportedPosts = new Posts * [reportedPostCapacity];

    for (int i = 0; i < reportedPostCount; i++)
        reportedPosts[i] = other.reportedPosts[i];

    adminNotifCapacity = other.adminNotifCapacity;
    adminNotifCount = other.adminNotifCount;
    adminNotifications = new Notification[adminNotifCapacity];

    for (int i = 0; i < adminNotifCount; i++)
        adminNotifications[i] = other.adminNotifications[i];
}

Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        User::operator=(other);
        adminLevel = other.adminLevel;

        delete[] reportedUsers;
        reportedUserCapacity = other.reportedUserCapacity;


        reportedUserCount = other.reportedUserCount;
        reportedUsers = new User * [reportedUserCapacity];
        for (int i = 0; i < reportedUserCount; i++)
            reportedUsers[i] = other.reportedUsers[i];

        delete[] reportedPosts;
        reportedPostCapacity = other.reportedPostCapacity;


        reportedPostCount = other.reportedPostCount;
        reportedPosts = new Posts * [reportedPostCapacity];
        for (int i = 0; i < reportedPostCount; i++)
            reportedPosts[i] = other.reportedPosts[i];

        delete[] adminNotifications;
        adminNotifCapacity = other.adminNotifCapacity;


        adminNotifCount = other.adminNotifCount;
        adminNotifications = new Notification[adminNotifCapacity];
        for (int i = 0; i < adminNotifCount; i++)
            adminNotifications[i] = other.adminNotifications[i];
    }
    return *this;
}

Admin::~Admin() {
    delete[] reportedUsers;
    delete[] reportedPosts;
    delete[] adminNotifications;
    cout << "Admin destroyed" << endl;
}

//report handling

void Admin::receiveUserReport(User* user) {
    if (!user) return;

    if (reportedUserCount >= reportedUserCapacity)
        expandReportedUsers();
    reportedUsers[reportedUserCount++] = user;

    addNotification("New report received for user: @" + user->getUsername());
    cout << "User report received: " << user->getUsername()
        << " Total reports: " << reportedUserCount << endl;

    saveReportsToFile();
    checkReportThresholds(user, user->getUsername());
}

void Admin::receivePostReport(Posts* post) {
    if (!post) return;

    if (reportedPostCount >= reportedPostCapacity)
        expandReportedPosts();
    reportedPosts[reportedPostCount++] = post;

    addNotification("New report received for post: " + post->getPostId()
        + " by @" + post->getCreatorUsername());
    cout << "Post report received: " << post->getPostId()
        << " Total reports: " << reportedPostCount << endl;

    saveReportsToFile();
}

//deleting users, posts, comments

void Admin::deleteUser(User**& allUsers, int& userCount, const string& username) {
    cout << "Admin: Deleting user " << username << endl;

    for (int i = 0; i < userCount; i++) {
        if (allUsers[i]->getUsername() == username) {
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
        if (!allUsers[i])
            continue;

        Posts* post = allUsers[i]->getPostById(postId);
        if (post != nullptr) {
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

    // convert int to string manually (no std::to_string issues on MSVC)
    char buf[32];
    sprintf(buf, "%d", commentIndex);
    addNotification(string("Comment deleted at index ") + buf);
    cout << "Comment deleted by admin at index: " << commentIndex << endl;
}

// reviewing reports

void Admin::reviewReports(User**& allUsers, int& userCount) {
    cout << "=== REVIEWING ALL REPORTS ===" << endl;
    cout << "Total reported users: " << reportedUserCount << endl;
    cout << "Total reported posts: " << reportedPostCount << endl;

    for (int i = 0; i < reportedUserCount; i++) {
        User* user = reportedUsers[i];
        if (!user) continue;

        // count occurrences
        int count = 0;
        for (int j = 0; j < reportedUserCount; j++)
            if (reportedUsers[j] == user) count++;

        cout << "User: " << user->getUsername() << " Reports: " << count << endl;
    }

    for (int i = 0; i < reportedPostCount; i++) {
        Posts* post = reportedPosts[i];
        if (!post)
            continue;

        int count = 0;
        for (int j = 0; j < reportedPostCount; j++)
            if (reportedPosts[j] == post) count++;

        cout << "Post: " << post->getPostId() << " Reports: " << count << endl;
    }
}

void Admin::checkReportThresholds(User* user, const string& username) {
    addNotification("ALERT: User @" + username + " reached 3+ reports!");
}

//getters

User** Admin::getReportedUsers() const {
    return reportedUsers;
}

Posts** Admin::getReportedPosts() const {
    return reportedPosts;
}

int Admin::getReportedUserCount() const {
    return reportedUserCount;
}

int Admin::getReportedPostCount() const {
    return reportedPostCount;
}

// notification handling

void Admin::addNotification(const string& message) {
    string ts = currentTimestamp();

    if (adminNotifCount >= adminNotifCapacity)
        expandNotifications();
    adminNotifications[adminNotifCount++] = Notification(message, "admin", ts);

    // Persist immediately so it survives session restarts
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");
    ofstream nf("data/Admin/admin_notifications.txt", ios::app);
    if (nf.is_open()) {
        nf << ts << "|admin|" << message << "\n";
        nf.close();
    }

    cout << "Admin notification: " << message << endl;
}

Notification* Admin::getAllNotifications() const {
    return adminNotifications;
}

int Admin::getAdminNotifCount() const {
    return adminNotifCount;
}

void Admin::viewAllNotifications() const {
    cout << "=== ADMIN NOTIFICATIONS ===" << endl;
    for (int i = 0; i < adminNotifCount; i++) {
        const Notification& n = adminNotifications[i];
        cout << n.getMessage()
            << " | Type: " << n.getType()
            << " | Time: " << n.getTimestamp()<< endl;
    }
}

void Admin::clearNotifications() {
    adminNotifCount = 0;
    cout << "Admin notifications cleared" << endl;
}

//file handling

void Admin::saveReportsToFile() {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");

    //Save reported posts
    ofstream postsFile("data/Admin/reported_posts.txt");
    if (postsFile.is_open()) {
        // deduplicate: write each unique post once
        for (int i = 0; i < reportedPostCount; i++) {
            Posts* post = reportedPosts[i];
            if (!post) 
                continue;

            // check if already written
            bool already = false;
            for (int j = 0; j < i; j++)
                if (reportedPosts[j] == post) {
                    already = true;
                    break; 
                }
            if (already) 
                continue;

            int count = 0;
            for (int j = 0; j < reportedPostCount; j++)
                if (reportedPosts[j] == post) 
                    count++;

            postsFile << "postId|" << post->getPostId() << "\n"
                << "creator|" << post->getCreatorUsername() << "\n"
                << "reportCount|" << count << "\n";
        }
        postsFile.close();
        cout << "Reported posts saved." << endl;
    }

    //Save reported users
    ofstream usersFile("data/Admin/reported_users.txt");
    if (usersFile.is_open()) {
        for (int i = 0; i < reportedUserCount; i++) {
            User* user = reportedUsers[i];
            if (!user)
                continue;

            bool already = false;
            for (int j = 0; j < i; j++)
                if (reportedUsers[j] == user) {
                    already = true; 
                    break;
                }
            if (already) continue;

            int count = 0;
            for (int j = 0; j < reportedUserCount; j++)
                if (reportedUsers[j] == user) 
                    count++;

            usersFile << "username|" << user->getUsername() << "\n"
                << "reportCount|" << count << "\n";
        }
        usersFile.close();
        cout << "Reported users saved." << endl;
    }
}

void Admin::loadReportsFromFile(User**& allUsers, int& userCount) {
    // reset arrays
    reportedUserCount = 0;
    reportedPostCount = 0;

    //Load reported posts by scanning _reported.txt files
    if (allUsers && userCount > 0) {
        for (int i = 0; i < userCount; i++) {
            if (!allUsers[i]) 
                continue;

            string uname = allUsers[i]->getUsername();
            if (uname.empty())
                continue;

            allUsers[i]->loadAllPosts();

            int postCount = allUsers[i]->getPostCount();
            for (int j = 0; j < postCount; j++) {
                Posts* post = allUsers[i]->getPostByIndex(j);
                if (!post || !post->isValid()) 
                    continue;

                string pid = post->getPostId();
                if (pid.empty())
                    continue;

                string reportedPath = "data/Posts/" + uname + "/" + pid + "_reported.txt";
                ifstream rf(reportedPath);
                if (!rf.is_open())
                    continue;

                int reporterCount = 0;
                string line;
                while (getline(rf, line))
                    if (!line.empty()) reporterCount++;
                rf.close();

                if (reporterCount >= 3) {
                    if (reportedPostCount >= reportedPostCapacity)
                        expandReportedPosts();
                    reportedPosts[reportedPostCount++] = post;
                }
            }
        }
    }
    cout << "Reported posts loaded: " << reportedPostCount << endl;

    
    if (allUsers && userCount > 0) {
        for (int i = 0; i < userCount; i++) {
            if (!allUsers[i]) 
                continue;

            
            if (allUsers[i]->getIsReported()) {

                
                bool alreadyLoaded = false;
                for (int j = 0; j < reportedUserCount; j++) {
                    if (reportedUsers[j] == allUsers[i]) {
                        alreadyLoaded = true;
                        break;
                    }
                }

                if (!alreadyLoaded) {
                    if (reportedUserCount >= reportedUserCapacity) 
                        expandReportedUsers();
                    reportedUsers[reportedUserCount++] = allUsers[i];
                }
            }
        }
    }
    cout << "Reported users loaded: " << reportedUserCount << endl;
}

void Admin::saveAdminToFile() {
    mkdirIfNeeded("data");
    mkdirIfNeeded("data/Admin");

    ofstream file("data/Admin/adminInfo.txt");
    if (file.is_open()) {
        file << "username|" << getUsername() << "\n"
            << "adminLevel|" << adminLevel << "\n"
            << "reportedUserCount|" << reportedUserCount << "\n"
            << "reportedPostCount|" << reportedPostCount << "\n";
        file.close();
        cout << "Admin info saved" << endl;
    }
}

void Admin::loadAdminFromFile() {
    ifstream file("data/Admin/adminInfo.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) 
                continue;
            size_t sep = line.find('|');
            if (sep == string::npos) 
                continue;
            string key = line.substr(0, sep);
            string val = line.substr(sep + 1);

            if (key == "adminLevel")
                adminLevel = val;
            else if (key == "reportedUserCount")
                reportedUserCount = stoi(val);
            else if (key == "reportedPostCount")
                reportedPostCount = stoi(val);
        }
        file.close();
        cout << "Admin info loaded" << endl;
    }

    // Load persisted notifications
    adminNotifCount = 0;
    ifstream nf("data/Admin/admin_notifications.txt");
    if (nf.is_open()) {
        string line;
        while (getline(nf, line)) {
            if (!line.empty() && line.back() == '\r') 
                line.pop_back();
            if (line.empty())
                continue;

            
            size_t p1 = line.find('|');
            if (p1 == string::npos) 
                continue;

            size_t p2 = line.find('|', p1 + 1);
            if (p2 == string::npos) 
                continue;

            string ts = line.substr(0, p1);
            string type = line.substr(p1 + 1, p2 - p1 - 1);
            string msg = line.substr(p2 + 1);

            // trim \r
            while (!ts.empty() && ts.back() == '\r') 
                ts.pop_back();
            while (!type.empty() && type.back() == '\r')
                type.pop_back();
            while (!msg.empty() && msg.back() == '\r')
                msg.pop_back();

            if (ts.empty() || msg.empty()) continue;

            if (adminNotifCount >= adminNotifCapacity)
                expandNotifications();
            adminNotifications[adminNotifCount++] = Notification(msg, type, ts);
        }
        nf.close();
    }
}

// display functions

void Admin::displayAdminDashboard() {
    cout << "|---- ADMIN DASHBOARD ----|" << endl;
    cout << "Username: " << getUsername() << endl;
    cout << "Admin Level: " << adminLevel << endl;
    cout << "Reported Users: " << reportedUserCount << endl;
    cout << "Reported Posts: " << reportedPostCount << endl;
    cout << "Pending Notifications: " << adminNotifCount << endl;
    cout << "|-------------------------|" << endl;
}

void Admin::displayReportedUsers() {
    cout << "|---- REPORTED USERS ----| " << endl;
    for (int i = 0; i < reportedUserCount; i++) {
        User* user = reportedUsers[i];
        if (!user) 
            continue;

      
        bool already = false;
        for (int j = 0; j < i; j++)
            if (reportedUsers[j] == user) {
                already = true;
                break; 
            }
        if (already) continue;

        int count = 0;
        for (int j = 0; j < reportedUserCount; j++)
            if (reportedUsers[j] == user)
                count++;

        cout << "User: " << user->getUsername() << " | Reports: " << count << endl;
    }
}

void Admin::displayReportedPosts() {
    cout << "--- REPORTED POSTS ---" << endl;

    for (int i = 0; i < reportedPostCount; i++) {
        Posts* post = reportedPosts[i];
        if (!post) 
            continue;

        bool already = false;
        for (int j = 0; j < i; j++)
            if (reportedPosts[j] == post) {
                already = true;
                break; 
            }
        if (already)
            continue;

        int count = 0;
        for (int j = 0; j < reportedPostCount; j++)
            if (reportedPosts[j] == post)
                count++;

        cout << "Post: " << post->getPostId() << " | Reports: " << count << endl;
    }
}