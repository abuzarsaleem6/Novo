// Admin.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Admin.h"
#include <QDateTime>
#include <fstream>

using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  CONSTRUCTORS & DESTRUCTORS
// ══════════════════════════════════════════════════════════════════════════════

Admin::Admin() : User() {
    adminLevel = "Super Admin";
    reportedUserCount = 0;
    reportedPostCount = 0;
    qDebug() << "Admin created (default)";
}

Admin::Admin(QString username, QString password, QString bio)
    : User(username.toStdString(), password.toStdString(), bio.toStdString()) {
    adminLevel = "Super Admin";
    reportedUserCount = 0;
    reportedPostCount = 0;
    qDebug() << "Admin created:" << username;
}

Admin::Admin(const Admin& other) : User(other) {
    adminLevel = other.adminLevel;
    reportedUsers = other.reportedUsers;
    reportedPosts = other.reportedPosts;
    reportedUserCount = other.reportedUserCount;
    reportedPostCount = other.reportedPostCount;
    adminNotifications = other.adminNotifications;
    
}

Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        User::operator=(other);
        adminLevel = other.adminLevel;
        reportedUsers = other.reportedUsers;
        reportedPosts = other.reportedPosts;
        reportedUserCount = other.reportedUserCount;
        reportedPostCount = other.reportedPostCount;
        adminNotifications = other.adminNotifications;
        
    }
    return *this;
}

Admin::~Admin() {
    reportedUsers.clear();
    reportedPosts.clear();
    adminNotifications.clear();
    qDebug() << "Admin destroyed";
}

// ══════════════════════════════════════════════════════════════════════════════
//  REPORT MANAGEMENT
// ══════════════════════════════════════════════════════════════════════════════

void Admin::receiveUserReport(User* user) {
    if (!user) return;

    reportedUsers.append(user);
    reportedUserCount++;

    QString username = QString::fromStdString(user->getUsername());
    addNotification("🚨 New report received for user: @" + username);

    qDebug() << "User report received:" << username << "Total reports:" << reportedUserCount;

    saveReportsToFile();
    checkReportThresholds(user, username);
}

void Admin::receivePostReport(Posts* post) {
    if (!post) return;

    reportedPosts.append(post);
    reportedPostCount++;

    QString postId = QString::fromStdString(post->getPostId());
    QString creatorUsername = QString::fromStdString(post->getCreatorUsername());
    addNotification("🚨 New report received for post: " + postId + " by @" + creatorUsername);

    qDebug() << "Post report received:" << postId << "Total reports:" << reportedPostCount;

    saveReportsToFile();
}

// ══════════════════════════════════════════════════════════════════════════════
//  ACTION METHODS - DELETE/BAN USERS AND POSTS
// ══════════════════════════════════════════════════════════════════════════════

void Admin::deleteUser(User**& allUsers, int& userCount, const QString& username) {
    qDebug() << "Admin: Deleting user" << username;

    for (int i = 0; i < userCount; i++) {
        if (QString::fromStdString(allUsers[i]->getUsername()) == username) {
            allUsers[i]->deleteAccount(allUsers, userCount);
            addNotification("✓ User deleted: @" + username);
            qDebug() << "User deleted successfully:" << username;
            return;
        }
    }

    qDebug() << "ERROR: User not found for deletion:" << username;
}

void Admin::deletePost(User** allUsers, int userCount, const QString& postId) {
    qDebug() << "Admin: Deleting post" << postId;

    for (int i = 0; i < userCount; i++) {
        if (!allUsers[i]) continue;

        Posts* post = allUsers[i]->getPostById(postId.toStdString());
        if (post != nullptr) {
            allUsers[i]->deletePost(postId.toStdString());
            addNotification("✓ Post deleted: " + postId);
            qDebug() << "Post deleted successfully:" << postId;
            return;
        }
    }

    qDebug() << "ERROR: Post not found for deletion:" << postId;
}

void Admin::deleteComment(Posts* post, int commentIndex) {
    if (!post) return;

    QList<Comment> comments = post->getComments();
    if (commentIndex < 0 || commentIndex >= comments.size()) {
        qDebug() << "ERROR: Invalid comment index:" << commentIndex;
        return;
    }

    post->deleteCommentAsAdmin(commentIndex);
    addNotification("✓ Comment deleted at index " + QString::number(commentIndex));
    qDebug() << "Comment deleted by admin at index:" << commentIndex;
}


// ══════════════════════════════════════════════════════════════════════════════
//  REPORT REVIEW & THRESHOLD CHECKING
// ══════════════════════════════════════════════════════════════════════════════

void Admin::reviewReports(User**& allUsers, int& userCount) {
    qDebug() << "=== REVIEWING ALL REPORTS ===";
    qDebug() << "Total reported users:" << reportedUserCount;
    qDebug() << "Total reported posts:" << reportedPostCount;

    for (int i = 0; i < reportedUsers.size(); i++) {
        User* user = reportedUsers[i];
        if (!user) continue;

        int reportCount = reportedUsers.count(user);
        qDebug() << "User:" << QString::fromStdString(user->getUsername())
            << "Reports:" << reportCount;
    }

    for (int i = 0; i < reportedPosts.size(); i++) {
        Posts* post = reportedPosts[i];
        if (!post) continue;

        int reportCount = reportedPosts.count(post);
        qDebug() << "Post:" << QString::fromStdString(post->getPostId())
            << "Reports:" << reportCount;
    }
}

void Admin::checkReportThresholds(User* user, const QString& username) {
    addNotification("⚠️  ALERT: User @" + username + " reached 3+ reports!");
}

// ══════════════════════════════════════════════════════════════════════════════
//  GETTERS FOR Qt GUI
// ══════════════════════════════════════════════════════════════════════════════

QList<User*> Admin::getReportedUsers() const {
    return reportedUsers;
}

QList<Posts*> Admin::getReportedPosts() const {
    return reportedPosts;
}

int Admin::getReportedUserCount() const {
    return reportedUserCount;
}

int Admin::getReportedPostCount() const {
    return reportedPostCount;
}

// ══════════════════════════════════════════════════════════════════════════════
//  NOTIFICATION MANAGEMENT
// ══════════════════════════════════════════════════════════════════════════════

void Admin::addNotification(const QString& message) {
    QString timestampedMsg = QDateTime::currentDateTime().toString("hh:mm:ss")
        + " | " + message;

    Notification n(
        timestampedMsg.toStdString(),  // msg
        "admin",                        // type
        QDateTime::currentDateTime().toString().toStdString()  // time
    );
    adminNotifications.append(n);
    qDebug() << "Admin notification:" << message;
}
QList<Notification> Admin::getAllNotifications() const {
    return adminNotifications;
}

void Admin::viewAllNotifications() const {
    qDebug() << "=== ADMIN NOTIFICATIONS ===";
    for (const Notification& n : adminNotifications) {
        qDebug() << QString::fromStdString(n.getMessage())
            << "| Type:" << QString::fromStdString(n.getType())
            << "| Time:" << QString::fromStdString(n.getTimestamp())
            << "| Status:" << (n.getStatus() ? "Read" : "Unread");
    }
}

void Admin::clearNotifications() {
    adminNotifications.clear();
    qDebug() << "Admin notifications cleared";
}

// ══════════════════════════════════════════════════════════════════════════════
//  FILE I/O OPERATIONS
// ══════════════════════════════════════════════════════════════════════════════

void Admin::saveReportsToFile() {
    QDir().mkpath("data/Admin");

    // Save reported posts
    QString postsPath = "data/Admin/reported_posts.txt";
    QFile postsFile(postsPath);

    if (postsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&postsFile);

        // Deduplicate posts before saving
        QSet<Posts*> uniquePosts;
        for (Posts* post : reportedPosts) {
            uniquePosts.insert(post);
        }

        for (Posts* post : uniquePosts) {
            if (!post) continue;
            int count = reportedPosts.count(post);
            out << "postId|" << QString::fromStdString(post->getPostId()) << "\n"
                << "creator|" << QString::fromStdString(post->getCreatorUsername()) << "\n"
                << "reportCount|" << count << "\n";
        }
        postsFile.close();
        qDebug() << "Reported posts saved:" << uniquePosts.size();
    }

    // Save reported users
    QString usersPath = "data/Admin/reported_users.txt";
    QFile usersFile(usersPath);

    if (usersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&usersFile);

        // Deduplicate users before saving
        QSet<User*> uniqueUsers;
        for (User* user : reportedUsers) {
            uniqueUsers.insert(user);
        }

        for (User* user : uniqueUsers) {
            if (!user) continue;
            int count = reportedUsers.count(user);
            out << "username|" << QString::fromStdString(user->getUsername()) << "\n"
                << "reportCount|" << count << "\n";
        }
        usersFile.close();
        qDebug() << "Reported users saved:" << uniqueUsers.size();
    }
}

void Admin::loadReportsFromFile(User**& allUsers, int& userCount) {
    reportedUsers.clear();
    reportedPosts.clear();

    // ── Load reported posts by scanning _reported.txt files ──────────────────
    if (allUsers && userCount > 0) {
        for (int i = 0; i < userCount; i++) {
            if (!allUsers[i]) continue;

            QString uname = QString::fromStdString(allUsers[i]->getUsername());
            if (uname.isEmpty()) continue;

            allUsers[i]->loadAllPosts();

            int postCount = allUsers[i]->getPostCount();
            for (int j = 0; j < postCount; j++) {
                Posts* post = allUsers[i]->getPostByIndex(j);
                if (!post || !post->isValid()) continue;

                QString pid = QString::fromStdString(post->getPostId());
                if (pid.isEmpty()) continue;

                QString reportedPath = "data/Posts/" + uname + "/" + pid + "_reported.txt";
                QFile rf(reportedPath);
                if (!rf.exists()) continue;

                if (!rf.open(QIODevice::ReadOnly | QIODevice::Text)) continue;

                int reporterCount = 0;
                QTextStream in(&rf);
                while (!in.atEnd()) {
                    QString line = in.readLine().trimmed();
                    if (!line.isEmpty())
                        reporterCount++;
                }
                rf.close();

                if (reporterCount >= 3)
                    reportedPosts.append(post);
            }
        }
    }
    qDebug() << "Reported posts loaded:" << reportedPosts.size();

    // ── Load reported users from reported_users.txt ───────────────────────────
    QString usersPath = "data/Admin/reported_users.txt";
    QFile usersFile(usersPath);
    if (usersFile.exists() && usersFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&usersFile);
        QString username;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList parts = line.split("|");
            if (parts.size() < 2) continue;

            if (parts[0] == "username") {
                username = parts[1].trimmed();
            }
            else if (parts[0] == "reportCount" && !username.isEmpty()) {
                if (!allUsers || userCount <= 0) continue;
                for (int i = 0; i < userCount; i++) {
                    if (!allUsers[i]) continue;
                    if (QString::fromStdString(allUsers[i]->getUsername()) == username) {
                        reportedUsers.append(allUsers[i]);
                        break;
                    }
                }
                username.clear();
            }
        }
        usersFile.close();
    }
    qDebug() << "Reported users loaded:" << reportedUsers.size();

    reportedUserCount = reportedUsers.size();
    reportedPostCount = reportedPosts.size();
}

   

void Admin::saveAdminToFile() {
    QDir().mkpath("data/Admin");
    QString path = "data/Admin/adminInfo.txt";
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "username|" << QString::fromStdString(getUsername()) << "\n"
            << "adminLevel|" << adminLevel << "\n"
            << "reportedUserCount|" << reportedUserCount << "\n"
            << "reportedPostCount|" << reportedPostCount << "\n";
        file.close();
        qDebug() << "Admin info saved";
    }
}

void Admin::loadAdminFromFile() {
    QString path = "data/Admin/adminInfo.txt";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split("|");
            if (parts.size() < 2) continue;
            if (parts[0] == "adminLevel")
                adminLevel = parts[1];
            else if (parts[0] == "reportedUserCount")
                reportedUserCount = parts[1].toInt();
            else if (parts[0] == "reportedPostCount")
                reportedPostCount = parts[1].toInt();
        }
        file.close();
        qDebug() << "Admin info loaded";
    }

    // Load persisted notifications from file
    adminNotifications.clear();
    QFile nf("data/Admin/admin_notifications.txt");
    if (nf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&nf);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            // Format: timestamp|type|message|isRead
            int p1 = line.indexOf('|');
            int p2 = line.indexOf('|', p1 + 1);
            int p3 = line.lastIndexOf('|');
            if (p1 == -1 || p2 == -1 || p3 == p2) continue;

            QString time = line.mid(0, p1).trimmed();
            QString msg = line.mid(p2 + 1, p3 - p2 - 1).trimmed();
            adminNotifications.append(time + " | " + msg);
        }
        nf.close();
    }
}


// ══════════════════════════════════════════════════════════════════════════════
//  DISPLAY METHODS (Console)
// ══════════════════════════════════════════════════════════════════════════════

void Admin::displayAdminDashboard() {
    qDebug() << "╔════ ADMIN DASHBOARD ════╗";
    qDebug() << "Username:" << QString::fromStdString(getUsername());
    qDebug() << "Admin Level:" << adminLevel;
    qDebug() << "Reported Users:" << reportedUserCount;
    qDebug() << "Reported Posts:" << reportedPostCount;
    qDebug() << "Pending Notifications:" << adminNotifications.size();
    qDebug() << "╚═══════════════════════════╝";
}

void Admin::displayReportedUsers() {
    qDebug() << "═══ REPORTED USERS ═══";

    QSet<User*> uniqueUsers;
    for (User* user : reportedUsers) {
        uniqueUsers.insert(user);
    }
    // I AM KALA G .. OOO YEAHHH.
    // I AM MASTER OF CODING.
    for (User* user : uniqueUsers) {
        if (!user) continue;
        int count = reportedUsers.count(user);
        qDebug() << "User:" << QString::fromStdString(user->getUsername())
            << "| Reports:" << count;
    }
}

void Admin::displayReportedPosts() {
    qDebug() << "═══ REPORTED POSTS ═══";

    QSet<Posts*> uniquePosts;
    for (Posts* post : reportedPosts) {
        uniquePosts.insert(post);
    }

    for (Posts* post : uniquePosts) {
        if (!post) continue;
        int count = reportedPosts.count(post);
        qDebug() << "Post:" << QString::fromStdString(post->getPostId())
            << "| Reports:" << count;
    }
}