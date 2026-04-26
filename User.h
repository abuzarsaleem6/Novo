// User.h - ADD THESE METHODS ONLY (keep existing code)

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QList>  // ← ADD THIS
using namespace std;

class Posts;
class Notification;
class Comment;

class User {
private:
    string username;
    string password;
    string bio;
    bool isLoggedIn;
    bool isReported;
    bool isBanned;
    int isReportedCount;
    // NOTE: Remove unused Notification** notifications;
    int notificationCount;
    User** following;
    int followingCount;
    User** followers;
    int followersCount;
    Posts** posts;
    Posts** savedPosts;
    int savedPostCount;
    int postCount;

public:
    User();
    User(string username, string password, string bio);
    User(const User& o);
    User& operator=(const User& o);

    static QString validateUsername(const string& username);
    static QString validatePassword(const string& password);
    static QString validateBio(const string& bio);
    static QString validatePostContent(const string& content);

    void logOut();
    void reportUser();
    bool updatePassword(const string& newPassword, QString& errorOut);
    bool updateBio(const string& newBio, QString& errorOut);

    void displayProfile();
    void saveToFile();
    void addToUserList();
    void loadFromFile(string username);
    bool login(string password);
    void removeFromUser_List(string username);

    // ─── GETTERS ───
    string getUsername() const;
    string getBio() const;
    string getPassword() const;
    bool getIsReported() const;
    bool getIsBanned() const;
    int getFollowingCount() const;
    int getFollowersCount() const;
    int getPostCount() const;
    int getSavedPostCount() const;
    string getCreatorUsername() const;

    // ─── SETTERS ───
    void setBio(string bio);
    void setPassword(string password);
    void setBan(bool banned);

    // ─── FOLLOWING/FOLLOWERS ───
    void followUser(User* target, User** allUsers, int userCount);
    bool isFollowing(string username);
    void addFollower(User* ptr);
    void unfollowUser(string username);
    void removeFollower(const string& usernameToRemove);
    void loadFollowing(User** allUsers, int userCount);
    void loadFollowers(User** allusers, int userCount);
    void clearSavedPostsArray(); 
    // ─── POSTS ───
    void createPost(string content);
    void loadAllPosts();
    void displayAllPosts();
    Posts* getPostById(string postId);
    Posts* getPostByIndex(int index);
    bool editPost(const string& postId, const string& newContent, QString& errorOut);
    void deletePost(string postId);
    void reportPost(string postId, User* postOwner);

    // ─── SAVED POSTS ───
    void savePost(string postId, User* postOwner);
    void unsavePost(string postId);
    void displaySavedPosts();
    void loadSavedPosts(User** allUsers, int userCount);
    void saveSavedPostsToFile();
    Posts* getSavedPostByIndex(int index);
    bool hasReportedUser(const string& reporterUsername) const;
    void reportUserBy(const string& reporterUsername);
    // ─── ACCOUNT ───
    void deleteAccount(User**& allUsers, int& userCount);
    void addToReviewList();

    // ─── Qt HELPER METHODS ─── (NEW)
    QString getDisplayUsername() const;
    QString getDisplayBio() const;
    QList<Posts*> getPostsAsQList() const;
    QList<Posts*> getSavedPostsAsQList() const;
    bool hasPost(const string& postId) const;
    static QList<QString> getConversationHistory(const string& username);
    static void addConversationToHistory(const string& username, const string& peerUsername);
    static void removeConversationFromHistory(const string& username, const string& peerUsername);
    ~User();
};

void loadAllUsers(User** allUsers, int& userCount);
User* signUp(User**& allUsers, int& userCount, string username, string password, string bio);
User* findAndLogin(User**& allUsers, int userCount, string username, string password);