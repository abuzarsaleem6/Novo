#include <iostream>
#include <fstream>
#include <string>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "User.h"
#include "Post.h"
#include "Notification.h"
using namespace std;

//Constructors

User::User() {
    username = "";
    password = "";
    bio = "";
    isReported = false;
    isBanned = false;
    isReportedCount = 0;
    followingCount = 0;
    followersCount = 0;
    postCount = 0;
    savedPostCount = 0;
    posts = nullptr;
    followers = nullptr;
    following = nullptr;
    savedPosts = nullptr;
}

User::User(string username, string password, string bio) {
    this->following = nullptr;
    this->followers = nullptr;
    this->posts = nullptr;
    this->savedPosts = nullptr;
    this->isReported = false;
    this->isBanned = false;
    this->isReportedCount = 0;
    this->followingCount = 0;
    this->followersCount = 0;
    this->postCount = 0;
    this->savedPostCount = 0;
    this->username = username;
    this->password = password;
    this->bio = bio;

    QDir().mkpath("data/Users");
    QDir().mkpath("data/Posts/" + QString::fromStdString(username));
    this->saveToFile();
    this->addToUserList();
}

User::User(const User& o) {
    username = o.username;
    password = o.password;
    bio = o.bio;
    isReported = o.isReported;
    isBanned = o.isBanned;
    isReportedCount = o.isReportedCount;
    followingCount = o.followingCount;
    followersCount = o.followersCount;
    postCount = o.postCount;
    savedPostCount = o.savedPostCount;

    following = nullptr;
    if (o.followingCount > 0 && o.following) {
        following = new User * [o.followingCount];
        for (int i = 0; i < o.followingCount; i++)
            following[i] = o.following[i];
    }

    followers = nullptr;
    if (o.followersCount > 0 && o.followers) {
        followers = new User * [o.followersCount];
        for (int i = 0; i < o.followersCount; i++)
            followers[i] = o.followers[i];
    }

    posts = nullptr;
    if (o.postCount > 0 && o.posts) {
        posts = new Posts * [o.postCount];
        for (int i = 0; i < o.postCount; i++)
            posts[i] = new Posts(*o.posts[i]);
    }

    // savedPosts are non-owned pointers — shallow copy only
    savedPosts = nullptr;
    if (o.savedPostCount > 0 && o.savedPosts) {
        savedPosts = new Posts * [o.savedPostCount];
        for (int i = 0; i < o.savedPostCount; i++)
            savedPosts[i] = o.savedPosts[i];
    }
}

User& User::operator=(const User& o) {
    if (this == &o) return *this;

    // Delete only owned data (posts are owned, savedPosts are NOT)
    for (int i = 0; i < postCount; i++) delete posts[i];
    delete[] posts;
    delete[] following;
    delete[] followers;
    delete[] savedPosts; // array only, not the Posts objects inside

    username = o.username;
    password = o.password;
    bio = o.bio;
    isReported = o.isReported;
    isBanned = o.isBanned;
    isReportedCount = o.isReportedCount;
    followingCount = o.followingCount;
    followersCount = o.followersCount;
    postCount = o.postCount;
    savedPostCount = o.savedPostCount;

    following = nullptr;
    if (o.followingCount > 0 && o.following) {
        following = new User * [o.followingCount];
        for (int i = 0; i < o.followingCount; i++)
            following[i] = o.following[i];
    }

    followers = nullptr;
    if (o.followersCount > 0 && o.followers) {
        followers = new User * [o.followersCount];
        for (int i = 0; i < o.followersCount; i++)
            followers[i] = o.followers[i];
    }

    posts = nullptr;
    if (o.postCount > 0 && o.posts) {
        posts = new Posts * [o.postCount];
        for (int i = 0; i < o.postCount; i++)
            posts[i] = new Posts(*o.posts[i]);
    }

    savedPosts = nullptr;
    if (o.savedPostCount > 0 && o.savedPosts) {
        savedPosts = new Posts * [o.savedPostCount];
        for (int i = 0; i < o.savedPostCount; i++)
            savedPosts[i] = o.savedPosts[i];
    }

    return *this;
}

User::~User() {
    for (int i = 0; i < postCount; i++) {
        delete posts[i];
        posts[i] = nullptr;
    }
    delete[] posts;
    posts = nullptr;

    // savedPosts: free the array only — the Posts objects are owned by their author
    delete[] savedPosts;
    savedPosts = nullptr;

    delete[] following;
    following = nullptr;

    delete[] followers;
    followers = nullptr;
}

// ══════════════════════════════════════════════════════════════════════════════
//  VALIDATION
// ══════════════════════════════════════════════════════════════════════════════

QString User::validateUsername(const string& username) {
    if (username.length() < 6)  return 
        "Username must be at least 6 characters";
    if (username.length() > 16) return 
        "Username cannot exceed 16 characters ";
    for (int i=0;i<username.length();i++)
        if (username[i] == '|' || username[i] == ' ') 
           return  "Username cannot contain '|' or spaces ";

    return "";
}

QString User::validatePassword(const string& password) {
    if (password.length() < 8)  
        return "Password must be at least 8 characters ";
    if (password.length() > 16)
        return "Password cannot exceed 16 characters ";
    for (int i = 0; i < password.length(); i++)
        if (password[i] == '|' || password[i] == ' ')
            return "Password cannot contain '|' or spaces ";
    return "";
}

QString User::validateBio(const string& bio) {
    if (bio.length() > 100) return "Bio cannot exceed 100 characters ";
    for (int i = 0; i < bio.length(); i++) {
        if (bio[i] == '|') return "Bio cannot contain '|' ";
    }
    return "";
}

QString User::validatePostContent(const string& content) {
    if (content.empty()) return "Post content cannot be empty.";
    for (int i = 0; i < content.length(); i++)
        if (content[i] == '|') 
            return "Post content cannot contain '|'.";
    return "";
}

// ══════════════════════════════════════════════════════════════════════════════
//  AUTH
// ══════════════════════════════════════════════════════════════════════════════

bool User::login(string password) {
    return (password == this->password);
}

void User::logOut() {
    for (int i = 0; i < postCount; i++) {
        delete posts[i]; 
        posts[i] = nullptr;
    }

    delete[] posts;  
    posts = nullptr;  
    postCount = 0;

    delete[] following;  
    following = nullptr; 
    followingCount = 0;
    delete[] followers; 
    followers = nullptr;  
    followersCount = 0;

    delete[] savedPosts; 
    savedPosts = nullptr;
    savedPostCount = 0;
}

// ══════════════════════════════════════════════════════════════════════════════
//  PROFILE UPDATES
// ══════════════════════════════════════════════════════════════════════════════

bool User::updatePassword(const string& newPassword, QString& errorOut) {
    errorOut = validatePassword(newPassword);
    if (!errorOut.isEmpty()) return false;
    password = newPassword;
    saveToFile();
    return true;
}

bool User::updateBio(const string& newBio, QString& errorOut) {
    errorOut = validateBio(newBio);
    if (!errorOut.isEmpty()) return false;
    bio = newBio;
    saveToFile();
    return true;
}

// ══════════════════════════════════════════════════════════════════════════════
//  FILE I/O
// ══════════════════════════════════════════════════════════════════════════════

void User::saveToFile() {
    QDir().mkpath("data/Users");
    string path = "data/Users/" + this->username + ".txt";
    ofstream file(path);
    if (!file.is_open()) {
        qDebug() << "Cannot save user file:" << QString::fromStdString(path);
        return;
    }
    file << "username|" << this->username << "\n";
    file << "password|" << this->password << "\n";
    file << "bio|" << this->bio << "\n";
    file << "isBanned|" << (this->isBanned ? 1 : 0) << "\n";
    file << "isReported|" << (this->isReported ? 1 : 0) << "\n";
    file << "isReportedCount|" << this->isReportedCount << "\n";
    file << "followingCount|" << this->followingCount << "\n";
    file << "followersCount|" << this->followersCount << "\n";
    file << "postCount|" << this->postCount << "\n";
    file << "savedPostCount|" << this->savedPostCount << "\n";
    file.close();
}

void User::loadFromFile(string username) {
    string path = "data/Users/" + username + ".txt";
    ifstream file(path);
    if (!file.is_open()) {
        qDebug() << "User not found:" << QString::fromStdString(username);
        return;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t sep = line.find('|');
        if (sep == string::npos || sep == 0) continue;

        string key = line.substr(0, sep);
        string value = line.substr(sep + 1);
        while (!value.empty() && (value.back() == '\r' || value.back() == ' '))
            value.pop_back();
        if (value.empty()) continue;

        try {
            if (key == "username")        this->username = value;
            else if (key == "password")        this->password = value;
            else if (key == "bio")             this->bio = value;
            else if (key == "isBanned")        this->isBanned = (value == "1");
            else if (key == "isReported")      this->isReported = (value == "1");
            else if (key == "isReportedCount") this->isReportedCount = stoi(value);
            else if (key == "followingCount")  this->followingCount = stoi(value);
            else if (key == "followersCount")  this->followersCount = stoi(value);
            else if (key == "postCount")       this->postCount = stoi(value);
            else if (key == "savedPostCount")  this->savedPostCount = stoi(value);
        }
        catch (...) {
            qDebug() << "Error parsing user field:" << QString::fromStdString(key);
        }
    }
    file.close();
}

void User::addToUserList() {
    // ← CHECK FIRST if already exists
    ifstream checkFile("data/users_list.txt");
    string line;
    while (getline(checkFile, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line == this->username) {
            checkFile.close();
            return;  // already in list, don't add again
        }
    }
    checkFile.close();

    // Only append if not already there
    ofstream file("data/users_list.txt", ios::app);
    if (file.is_open()) {
        file << this->username << "\n";
        file.close();
    }
}

void User::removeFromUser_List(string username) {
    string updatedContent;
    string line;
    ifstream file("data/users_list.txt");
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line != username)
            updatedContent += line + "\n";
    }
    file.close();
    ofstream out("data/users_list.txt", ios::out);
    out << updatedContent;
    out.close();
}

void User::addToReviewList() {
    QDir().mkpath("data");
    ofstream file("data/users_to_review.txt", ios::app);
    if (file.is_open()) {
        file << this->username << "\n";
        file.close();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  GETTERS / SETTERS
// ══════════════════════════════════════════════════════════════════════════════

string User::getUsername()      const { return this->username; }
string User::getBio()           const { return this->bio; }
string User::getPassword()      const { return this->password; }
bool   User::getIsReported()    const { return this->isReported; }
bool   User::getIsBanned()      const { return this->isBanned; }
int    User::getFollowingCount()const { return this->followingCount; }
int    User::getFollowersCount()const { return this->followersCount; }
int    User::getPostCount()     const { return this->postCount; }
int    User::getSavedPostCount()const { return this->savedPostCount; }

void User::setBio(string bio) { this->bio = bio; }
void User::setPassword(string password) { this->password = password; }
void User::setBan(bool banned) { this->isBanned = banned; saveToFile(); }

// ══════════════════════════════════════════════════════════════════════════════
//  FOLLOWING / FOLLOWERS
// ══════════════════════════════════════════════════════════════════════════════

void User::followUser(User* target, User** allUsers, int userCount) {
    if (!target) return;
    if (isFollowing(target->getUsername())) return;

    // Grow following array
    User** newFollowing = new User * [followingCount + 1];
    for (int i = 0; i < followingCount; i++)
        newFollowing[i] = following[i];
    newFollowing[followingCount] = target;
    delete[] following;
    following = newFollowing;
    followingCount++;

    target->addFollower(this);

    QDir().mkpath("data/Following");
    string path = "data/Following/" + this->username + "_following.txt";
    ofstream file(path, ios::app);
    if (file.is_open()) {
        file << target->getUsername() << "\n";
        file.close();
    }

    // Notify target
    QDir().mkpath("data/Notifications");
    string notifPath = "data/Notifications/" + target->getUsername() + "_notif.txt";
    ofstream notifFile(notifPath, ios::app);
    if (notifFile.is_open()) {
        notifFile << "follow|"
            << this->username << " followed you|"
            << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString()
            << "|0\n";
        notifFile.close();
    }

    saveToFile();
}

bool User::isFollowing(string username) {
    if (!following || followingCount == 0) return false;
    for (int i = 0; i < followingCount; i++) {
        if (following[i] && following[i]->getUsername() == username)
            return true;
    }
    return false;
}

void User::addFollower(User* ptr) {
    if (!ptr) return;
    User** newFollowers = new User * [followersCount + 1];
    for (int i = 0; i < followersCount; i++) {
        if (followers[i] != nullptr)
            newFollowers[i] = followers[i];
        else
            newFollowers[i] = nullptr;
    }
    newFollowers[followersCount] = ptr;
    delete[] followers;
    followers = newFollowers;
    followersCount++;

    QDir().mkpath("data/Following");
    string path = "data/Following/" + this->username + "_followers.txt";
    ofstream file(path, ios::app);
    if (file.is_open()) {
        file << ptr->getUsername() << "\n";
        file.close();
    }
    saveToFile();
}

void User::unfollowUser(string username) {
    if (followingCount == 0 || !following) return;
    if (!isFollowing(username)) return;

    User* target = nullptr;
    int removeIdx = -1;
    for (int i = 0; i < followingCount; i++) {
        if (following[i] && following[i]->getUsername() == username) {
            target = following[i];
            removeIdx = i;
            break;
        }
    }
    if (removeIdx == -1) return;

    if (followingCount == 1) {
        delete[] following;
        following = nullptr;
        followingCount = 0;
    }
    else {
        User** newFollowing = new User * [followingCount - 1];
        int idx = 0;
        for (int i = 0; i < followingCount; i++)
            if (i != removeIdx) newFollowing[idx++] = following[i];
        delete[] following;
        following = newFollowing;
        followingCount--;
    }

    // Rewrite _following.txt
    string path = "data/Following/" + this->username + "_following.txt";
    ofstream file(path, ios::out);
    if (file.is_open()) {
        for (int i = 0; i < followingCount; i++)
            if (following[i]) file << following[i]->getUsername() << "\n";
        file.close();
    }

    if (target) target->removeFollower(this->username);
    saveToFile();
}

void User::removeFollower(const string& usernameToRemove) {
    if (usernameToRemove.empty()) return;

    // Rewrite file without the removed follower
    string path = "data/Following/" + this->username + "_followers.txt";
    QStringList remaining;
    QFile f(QString::fromStdString(path));
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
            QString l = in.readLine().trimmed();
            if (!l.isEmpty() && l.toStdString() != usernameToRemove)
                remaining.append(l);
        }
        f.close();
    }
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        for (const QString& l : remaining) out << l << "\n";
        f.close();
    }

    // Rebuild in-memory array from file (no stale pointers)
    delete[] followers;
    followers = nullptr;
    followersCount = remaining.size();
    saveToFile();
}

void User::loadFollowing(User** allUsers, int userCount) {
    delete[] following;
    following = nullptr;

    if (followingCount == 0) return;

    string path = "data/Following/" + this->username + "_following.txt";
    ifstream file(path);
    if (!file.is_open()) { followingCount = 0; return; }

    following = new User * [followingCount]();
    int loaded = 0;
    string line;
    while (getline(file, line) && loaded < followingCount) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        for (int i = 0; i < userCount; i++) {
            if (allUsers[i] && allUsers[i]->getUsername() == line) {
                following[loaded++] = allUsers[i];
                break;
            }
        }
    }
    file.close();
    followingCount = loaded;
}

void User::loadFollowers(User** allUsers, int userCount) {
    delete[] followers;
    followers = nullptr;

    if (followersCount == 0) return;

    string path = "data/Following/" + this->username + "_followers.txt";
    ifstream file(path);
    if (!file.is_open()) { followersCount = 0; return; }

    followers = new User * [followersCount]();
    int loaded = 0;
    string line;
    while (getline(file, line) && loaded < followersCount) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        for (int i = 0; i < userCount; i++) {
            if (allUsers[i] && allUsers[i]->getUsername() == line) {
                followers[loaded++] = allUsers[i];
                break;
            }
        }
    }
    file.close();
    followersCount = loaded;
}

// ══════════════════════════════════════════════════════════════════════════════
//  POSTS
// ══════════════════════════════════════════════════════════════════════════════

void User::createPost(string content) {
    Posts** newPosts = new Posts * [postCount + 1];
    for (int i = 0; i < postCount; i++)
        newPosts[i] = posts[i];
    delete[] posts;

    newPosts[postCount] = new Posts(this->username, content);
    posts = newPosts;
    postCount++;

    posts[postCount - 1]->savePostToFile();

    QDir().mkpath(QString::fromStdString("data/Posts/" + this->username));
    string listPath = "data/Posts/" + this->username + "/posts_list.txt";
    ofstream listFile(listPath, ios::app);
    if (listFile.is_open()) {
        listFile << posts[postCount - 1]->getPostId() << "\n";
        listFile.close();
    }
    saveToFile();
}

void User::loadAllPosts() {
    if (posts != nullptr) {
        for (int i = 0; i < postCount; i++) { delete posts[i]; }
        delete[] posts;
        posts = nullptr;
    }
    postCount = 0;

    string listPath = "data/Posts/" + this->username + "/posts_list.txt";
    ifstream listFile(listPath);
    if (!listFile.is_open()) return;

    
    int count = 0;
    string temp;
    while (getline(listFile, temp)) {
        if (!temp.empty() && temp != "\r") count++;
    }
    listFile.close();
    if (count == 0) return;

    posts = new Posts * [count]();

    ifstream listFile2(listPath);
    string postId;
    while (getline(listFile2, postId) && postCount < count) {
        if (!postId.empty() && postId.back() == '\r') postId.pop_back();
        if (postId.empty()) continue;

        Posts* p = new Posts();
        p->loadPostFromFile(this->username, postId);

        // Only keep the post if it loaded successfully
        if (p->isValid()) {
            posts[postCount++] = p;
        }
        else {
            delete p;
            // Remove corrupt entry from posts_list.txt
            qDebug() << "Skipping invalid post:" << QString::fromStdString(postId);
        }
    }
    listFile2.close();
}

Posts* User::getPostById(string postId) {
    if (!posts) return nullptr;
    for (int i = 0; i < postCount; i++) {
        if (posts[i] && posts[i]->getPostId() == postId)
            return posts[i];
    }
    return nullptr;
}

Posts* User::getPostByIndex(int index) {
    if (index < 0 || index >= postCount || !posts) return nullptr;
    return posts[index];
}

bool User::editPost(const string& postId, const string& newContent, QString& errorOut) {
    errorOut = validatePostContent(newContent);
    if (!errorOut.isEmpty()) return false;

    Posts* p = getPostById(postId);
    if (!p) { errorOut = "Post not found."; return false; }

    p->setContent(newContent);
    p->savePostToFile();
    return true;
}

void User::deletePost(string postId) {
    Posts* p = getPostById(postId);
    if (!p) { qDebug() << "Post not found."; return; }

    QString base = QString::fromStdString("data/Posts/" + this->username + "/" + postId);
    QFile::remove(base + ".txt");
    QFile::remove(base + "_comments.txt");
    QFile::remove(base + "_reported.txt");

    // Update posts_list.txt
    QString listPath = QString::fromStdString("data/Posts/" + this->username + "/posts_list.txt");
    QFile listFile(listPath);
    QString updatedList;
    if (listFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&listFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty() && line.toStdString() != postId)
                updatedList += line + "\n";
        }
        listFile.close();
    }
    if (listFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&listFile);
        out << updatedList;
        listFile.close();
    }

    // Rebuild posts array
    Posts** newPosts = postCount > 1 ? new Posts * [postCount - 1] : nullptr;
    int idx = 0;
    for (int i = 0; i < postCount; i++) {
        if (posts[i]->getPostId() != postId)
            newPosts[idx++] = posts[i];
        else
            delete posts[i];
    }
    delete[] posts;
    posts = newPosts;
    postCount--;
    saveToFile();
}

void User::reportPost(string postId, User* postOwner) {
    if (!postOwner) return;
    postOwner->loadAllPosts();
    for (int i = 0; i < postOwner->getPostCount(); ++i) {
        Posts* p = postOwner->getPostByIndex(i);
        if (!p || !p->isValid()) continue;
        if (p->getPostId() == postId) {
            p->reportPost(this->username);
            return;
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  SAVED POSTS
// ══════════════════════════════════════════════════════════════════════════════

void User::savePost(string postId, User* postOwner) {
    if (!postOwner || postOwner->getUsername() == this->username) return;

    // Check already saved
    for (int i = 0; i < savedPostCount; i++) {
        if (savedPosts[i] && savedPosts[i]->getPostId() == postId) return;
    }

    if (!postOwner->posts && postOwner->postCount > 0)
        postOwner->loadAllPosts();

    Posts* p = postOwner->getPostById(postId);
    if (!p) { qDebug() << "Post not found."; return; }

    Posts** newSaved = new Posts * [savedPostCount + 1];
    for (int i = 0; i < savedPostCount; i++)
        newSaved[i] = savedPosts[i];
    newSaved[savedPostCount] = p;
    delete[] savedPosts;
    savedPosts = newSaved;
    savedPostCount++;

    saveSavedPostsToFile();
    saveToFile();
}

void User::unsavePost(string postId) {
    if (!savedPosts || savedPostCount == 0) return;

    int foundIdx = -1;
    for (int i = 0; i < savedPostCount; i++) {
        if (savedPosts[i] && savedPosts[i]->getPostId() == postId) {
            foundIdx = i;
            break;
        }
    }
    if (foundIdx == -1) { qDebug() << "Post not in saved list."; return; }

    Posts** newSaved = savedPostCount > 1 ? new Posts * [savedPostCount - 1] : nullptr;
    int idx = 0;
    for (int i = 0; i < savedPostCount; i++) {
        if (i != foundIdx) newSaved[idx++] = savedPosts[i];
    }
    delete[] savedPosts;
    savedPosts = newSaved;
    savedPostCount--;

    saveSavedPostsToFile();
    saveToFile();
}

void User::saveSavedPostsToFile() {
    QDir().mkpath(QString::fromStdString("data/Posts/" + this->username));
    string path = "data/Posts/" + this->username + "/saved_posts.txt";
    ofstream file(path);
    if (!file.is_open()) { qDebug() << "Cannot open saved posts file."; return; }
    for (int i = 0; i < savedPostCount; i++) {
        if (savedPosts[i])
            file << savedPosts[i]->getPostId() << "|"
            << savedPosts[i]->getCreatorUsername() << "\n";
    }
    file.close();
}

void User::loadSavedPosts(User** allUsers, int userCount) {
    delete[] savedPosts;
    savedPosts = nullptr;
    savedPostCount = 0;

    string path = "data/Posts/" + this->username + "/saved_posts.txt";
    ifstream file(path);
    if (!file.is_open()) return;

    // Count lines
    int count = 0;
    string line;
    while (getline(file, line))
        if (!line.empty() && line != "\r") count++;
    file.close();
    if (count == 0) return;

    savedPosts = new Posts * [count]();

    ifstream file2(path);
    while (getline(file2, line) && savedPostCount < count) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        size_t sep = line.find('|');
        if (sep == string::npos) continue;

        string pid = line.substr(0, sep);
        string owner = line.substr(sep + 1);

        // Trim both
        while (!pid.empty() && (pid.back() == ' ' || pid.back() == '\r')) pid.pop_back();
        while (!owner.empty() && (owner.back() == ' ' || owner.back() == '\r')) owner.pop_back();
        if (pid.empty() || owner.empty()) continue;

        for (int i = 0; i < userCount; i++) {
            if (!allUsers[i] || allUsers[i]->getUsername() != owner) continue;
            // Ensure owner's posts are loaded
            if (!allUsers[i]->posts && allUsers[i]->postCount > 0)
                allUsers[i]->loadAllPosts();
            Posts* p = allUsers[i]->getPostById(pid);
            if (p) savedPosts[savedPostCount++] = p;
            break;
        }
    }
    file2.close();
}

void User::clearSavedPostsArray() {
    delete[] savedPosts;
    savedPosts = nullptr;
    savedPostCount = 0;
}
bool User::hasSavedPost(const string& postId) const {
    for (int i = 0; i < savedPostCount; ++i) {
        Posts* sp = savedPosts[i];
        if (sp && sp->getPostId() == postId)
            return true;
    }
    return false;
}
Posts* User::getSavedPostByIndex(int index) {
    if (index < 0 || index >= savedPostCount || !savedPosts) return nullptr;
    return savedPosts[index];
}

// ══════════════════════════════════════════════════════════════════════════════
//  REPORTING
// ══════════════════════════════════════════════════════════════════════════════

void User::reportUser() {
    isReportedCount++;
    if (isReportedCount >= 3 && !isReported) {
        isReported = true;
        // admin_notifications.txt — already exists
        QDir().mkpath("data/Admin");
        QFile notifFile("data/Admin/admin_notifications.txt");
        if (notifFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&notifFile);
            QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            out << ts << "|user_reported|User '@"
                << QString::fromStdString(username)
                << "' has reached 3 reports and is marked for review.|0\n";
            notifFile.close();
        }
    }

    // ← ADD THIS: always update reported_users.txt
    QDir().mkpath("data/Admin");
    QFile usersFile("data/Admin/reported_users.txt");
    if (usersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Read existing entries first
        QMap<QString, int> reportMap;
        QFile readFile("data/Admin/reported_users.txt");
        // reopen for reading by using a separate read
    }

    // Simpler approach — just append/rewrite:
    // Read current file into map
    QMap<QString, int> reportMap;
    QFile rf("data/Admin/reported_users.txt");
    if (rf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&rf);
        QString uname;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split("|");
            if (parts.size() < 2) continue;
            if (parts[0] == "username") uname = parts[1];
            else if (parts[0] == "reportCount") reportMap[uname] = parts[1].toInt();
        }
        rf.close();
    }

    // Update count for this user
    reportMap[QString::fromStdString(username)] = isReportedCount;

    // Rewrite file
    QFile wf("data/Admin/reported_users.txt");
    if (wf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&wf);
        for (auto it = reportMap.begin(); it != reportMap.end(); ++it) {
            out << "username|" << it.key() << "\n"
                << "reportCount|" << it.value() << "\n";
        }
        wf.close();
    }

    saveToFile();
}

bool User::hasReportedUser(const string& reporterUsername) const {
    string path = "data/Users/" + this->username + "_reporters.txt";
    ifstream file(path);
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line == reporterUsername) return true;
    }
    return false;
}

void User::reportUserBy(const string& reporterUsername) {
    if (hasReportedUser(reporterUsername)) return;
    string path = "data/Users/" + this->username + "_reporters.txt";
    ofstream file(path, ios::app);
    if (file.is_open()) {
        file << reporterUsername << "\n";
        file.close();
    }
    reportUser();
}

// ══════════════════════════════════════════════════════════════════════════════
//  ACCOUNT DELETION
// ══════════════════════════════════════════════════════════════════════════════

void User::deleteAccount(User**& allUsers, int& userCount) {
    string uname = this->username;

    loadFollowers(allUsers, userCount);
    loadFollowing(allUsers, userCount);

    // Remove this user from every follower's following list
    for (int i = 0; i < followersCount; i++) {
        if (!followers[i]) continue;
        followers[i]->loadFollowing(allUsers, userCount);
        User* follower = followers[i];
        if (!follower->isFollowing(uname)) continue;

        if (follower->followingCount == 1) {
            delete[] follower->following;
            follower->following = nullptr;
            follower->followingCount = 0;
        }
        else {
            User** newF = new User * [follower->followingCount - 1];
            int idx = 0;
            for (int j = 0; j < follower->followingCount; j++)
                if (follower->following[j]->getUsername() != uname)
                    newF[idx++] = follower->following[j];
            delete[] follower->following;
            follower->following = newF;
            follower->followingCount--;
        }

        string fPath = "data/Following/" + follower->getUsername() + "_following.txt";
        ofstream fFile(fPath, ios::out);
        if (fFile.is_open()) {
            for (int j = 0; j < follower->followingCount; j++)
                fFile << follower->following[j]->getUsername() << "\n";
            fFile.close();
        }
        follower->saveToFile();
    }

    // Remove this user from every followed user's followers list
    for (int i = 0; i < followingCount; i++) {
        if (!following[i]) continue;
        following[i]->loadFollowers(allUsers, userCount);
        User* followedUser = following[i];

        if (followedUser->followersCount == 1) {
            delete[] followedUser->followers;
            followedUser->followers = nullptr;
            followedUser->followersCount = 0;
        }
        else {
            User** newF = new User * [followedUser->followersCount - 1];
            int idx = 0;
            for (int j = 0; j < followedUser->followersCount; j++)
                if (followedUser->followers[j]->getUsername() != uname)
                    newF[idx++] = followedUser->followers[j];
            delete[] followedUser->followers;
            followedUser->followers = newF;
            followedUser->followersCount--;
        }

        string fPath = "data/Following/" + followedUser->getUsername() + "_followers.txt";
        ofstream fFile(fPath, ios::out);
        if (fFile.is_open()) {
            for (int j = 0; j < followedUser->followersCount; j++)
                fFile << followedUser->followers[j]->getUsername() << "\n";
            fFile.close();
        }
        followedUser->saveToFile();
    }

    // Delete this user's post files
    loadAllPosts();
    for (int i = 0; i < postCount; i++) {
        if (!posts[i]) continue;
        string pid = posts[i]->getPostId();
        QString base = QString::fromStdString("data/Posts/" + uname + "/" + pid);
        QFile::remove(base + ".txt");
        QFile::remove(base + "_comments.txt");
        QFile::remove(base + "_reported.txt");
    }

    // Delete misc files for this user
    QFile::remove(QString::fromStdString("data/Posts/" + uname + "/" + uname + "_liked.txt"));
    QFile::remove(QString::fromStdString("data/Posts/" + uname + "/posts_list.txt"));
    QFile::remove(QString::fromStdString("data/Posts/" + uname + "/saved_posts.txt"));
    QDir(QString::fromStdString("data/Posts/" + uname)).removeRecursively();

    // Remove this user's comments from other users' posts
    for (int u = 0; u < userCount; u++) {
        if (!allUsers[u] || allUsers[u]->getUsername() == uname) continue;
        allUsers[u]->loadAllPosts();
        for (int p = 0; p < allUsers[u]->getPostCount(); p++) {
            Posts* post = allUsers[u]->getPostByIndex(p);
            if (!post || !post->isValid()) continue;
            QList<Comment> comments = post->getComments();
            for (int c = comments.size() - 1; c >= 0; c--) {
                if (comments[c].getCreatorUsername().toStdString() == uname)
                    post->deleteCommentAsAdmin(c);
            }
        }
    }

    // Remove this user's saved posts from other users' saved_posts.txt
    for (int u = 0; u < userCount; u++) {
        if (!allUsers[u] || allUsers[u]->getUsername() == uname) continue;
        QString savedPath = QString::fromStdString(
            "data/Posts/" + allUsers[u]->getUsername() + "/saved_posts.txt");
        QFile f(savedPath);
        QStringList remaining;
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&f);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty() && !line.endsWith("|" + QString::fromStdString(uname)))
                    remaining << line;
            }
            f.close();
        }
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            for (const QString& l : remaining) out << l << "\n";
            f.close();
        }
    }

    // Delete following/followers/notifications files
    QFile::remove(QString::fromStdString("data/Following/" + uname + "_following.txt"));
    QFile::remove(QString::fromStdString("data/Following/" + uname + "_followers.txt"));
    QFile::remove(QString::fromStdString("data/Notifications/" + uname + "_notif.txt"));
    QFile::remove(QString::fromStdString("data/Users/" + uname + "_reporters.txt"));

    // Clean up messages
    QString quname = QString::fromStdString(uname);
    QDir msgDir("data/Messages");
    QStringList msgFiles = msgDir.entryList(QStringList() << "*.txt", QDir::Files);
    for (const QString& fname : msgFiles) {
        if (fname.contains(quname))
            QFile::remove("data/Messages/" + fname);
    }
    for (int u = 0; u < userCount; u++) {
        if (!allUsers[u] || allUsers[u]->getUsername() == uname) continue;
        QString indexPath = "data/Messages/" + QString::fromStdString(allUsers[u]->getUsername()) + "_index.txt";
        QFile f(indexPath);
        QStringList remaining;
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&f);
            while (!in.atEnd()) {
                QString l = in.readLine().trimmed();
                if (!l.isEmpty() && l != quname) remaining << l;
            }
            f.close();
        }
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            for (const QString& l : remaining) out << l << "\n";
            f.close();
        }
    }

    // Delete user file and remove from users_list.txt
    QFile::remove(QString::fromStdString("data/Users/" + uname + ".txt"));
    removeFromUser_List(uname);

    // Remove from allUsers array — null the slot first, then rebuild
    User** newArray = (userCount - 1 > 0) ? new User * [userCount - 1] : nullptr;
    int idx = 0;
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i] && allUsers[i]->getUsername() != uname)
            newArray[idx++] = allUsers[i];
        // Do NOT delete this — caller owns allUsers[i] for the deleted user
    }
    delete[] allUsers;
    allUsers = newArray;
    userCount--;

    qDebug() << "Account fully deleted:" << QString::fromStdString(uname);
}

// ══════════════════════════════════════════════════════════════════════════════
//  Qt HELPERS
// ══════════════════════════════════════════════════════════════════════════════

QString User::getDisplayUsername() const { return QString::fromStdString(this->username); }
QString User::getDisplayBio()      const { return QString::fromStdString(this->bio); }

QList<Posts*> User::getPostsAsQList() const {
    QList<Posts*> result;
    for (int i = 0; i < postCount; i++)
        if (posts[i]) result.append(posts[i]);
    return result;
}

QList<Posts*> User::getSavedPostsAsQList() const {
    QList<Posts*> result;
    for (int i = 0; i < savedPostCount; i++)
        if (savedPosts[i]) result.append(savedPosts[i]);
    return result;
}

bool User::hasPost(const string& postId) const {
    for (int i = 0; i < postCount; i++)
        if (posts[i] && posts[i]->getPostId() == postId) return true;
    return false;
}

// ══════════════════════════════════════════════════════════════════════════════
//  CONVERSATION HISTORY (Messages)
// ══════════════════════════════════════════════════════════════════════════════

QList<QString> User::getConversationHistory(const string& username) {
    QList<QString> peers;
    QString path = QString::fromStdString("data/Messages/" + username + "_index.txt");
    QFile f(path);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
            QString peer = in.readLine().trimmed();
            if (!peer.isEmpty() && !peers.contains(peer))
                peers.append(peer);
        }
        f.close();
    }
    return peers;
}

void User::addConversationToHistory(const string& username, const string& peerUsername) {
    QDir().mkpath("data/Messages");
    QString path = QString::fromStdString("data/Messages/" + username + "_index.txt");
    QString peerStr = QString::fromStdString(peerUsername);

    QList<QString> existing;
    QFile rf(path);
    if (rf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&rf);
        while (!in.atEnd()) {
            QString l = in.readLine().trimmed();
            if (!l.isEmpty()) existing.append(l);
        }
        rf.close();
    }
    if (existing.contains(peerStr)) return;

    QFile wf(path);
    if (wf.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&wf);
        out << peerStr << "\n";
        wf.close();
    }
}

void User::removeConversationFromHistory(const string& username, const string& peerUsername) {
    QString path = QString::fromStdString("data/Messages/" + username + "_index.txt");
    QFile f(path);
    QStringList remaining;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        while (!in.atEnd()) {
            QString l = in.readLine().trimmed();
            if (!l.isEmpty() && l.toStdString() != peerUsername)
                remaining.append(l);
        }
        f.close();
    }
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        for (const QString& l : remaining) out << l << "\n";
        f.close();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  FREE FUNCTIONS
// ══════════════════════════════════════════════════════════════════════════════

void loadAllUsers(User** allUsers, int& userCount) {
    ifstream userList("data/users_list.txt");
    if (!userList.is_open()) { userCount = 0; return; }

    int index = 0;
    string username;
    while (getline(userList, username)) {
        if (!username.empty() && username.back() == '\r') username.pop_back();
        if (username.empty()) continue;
        allUsers[index] = new User();
        allUsers[index]->loadFromFile(username);
        index++;
    }
    userCount = index;
    userList.close();
}

User* signUp(User**& allUsers, int& userCount, string username, string password, string bio) {
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i] && allUsers[i]->getUsername() == username)
            return nullptr; // username taken
    }
    User* newUser = new User(username, password, bio);
    User** newArray = new User * [userCount + 1];
    for (int i = 0; i < userCount; i++)
        newArray[i] = allUsers[i];
    newArray[userCount] = newUser;
    delete[] allUsers;
    allUsers = newArray;
    userCount++;
    return newUser;
}

User* findAndLogin(User**& allUsers, int userCount, string username, string password) {
    for (int i = 0; i < userCount; i++) {
        if (!allUsers[i]) continue;
        if (allUsers[i]->getUsername() == username) {
            if (allUsers[i]->getIsBanned()) return nullptr;
            if (allUsers[i]->login(password)) return allUsers[i];
            return nullptr;
        }
    }
    return nullptr;
}