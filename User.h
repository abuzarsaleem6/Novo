#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Posts;
class Notification;
class Comment;

class User {
private:
    string username;
    string password;
    string bio;
    bool isReported;
    bool isBanned;
    int isReportedCount;
    int followingCount;
    int followersCount;
    User** following;
    User** followers;
    Posts** posts;
    Posts** savedPosts;
    int savedPostCount;
    int postCount;

public:
    User();
    User(string username, string password, string bio);
    User(const User& o);
    User& operator=(const User& o);
    ~User();

    // ─── VALIDATION ───
    static string validateUsername(const string& username);
    static string validatePassword(const string& password);
    static string validateBio(const string& bio);
    static string validatePostContent(const string& content);

    // ─── AUTH ───
    bool login(string password);
    void logOut();

    // ─── PROFILE ───
    bool updateBio(const string& newBio);
    bool updatePassword(const string& newPassword);

    // ─── FILE I/O ───
    void saveToFile();
    void loadFromFile(string username);
    void addToUserList();
    void removeFromUser_List(string username);
    void addToReviewList();

    // ─── GETTERS ───
    string getUsername()      const;
    string getBio()           const;
    string getPassword()      const;
    bool   getIsReported()    const;
    bool   getIsBanned()      const;
    int    getFollowingCount()const;
    int    getFollowersCount()const;
    int    getPostCount()     const;
    int    getSavedPostCount()const;

    // ─── SETTERS ───
    void setBio(string bio);
    void setPassword(string password);
    void setBan(bool banned);

    // ─── FOLLOWING / FOLLOWERS ───
    void followUser(User* target, User** allUsers, int userCount);
    void unfollowUser(string username);
    bool isFollowing(string username);
    void addFollower(User* ptr);
    void removeFollower(const string& usernameToRemove);
    void loadFollowing(User** allUsers, int userCount);
    void loadFollowers(User** allUsers, int userCount);

    // ─── POSTS ───
    void   createPost(string content);
    void   loadAllPosts();
    Posts* getPostById(string postId);
    Posts* getPostByIndex(int index);
    bool   editPost(const string& postId, const string& newContent, string& errorOut);
    void   deletePost(string postId);
    void   reportPost(string postId, User* postOwner);

    // ─── SAVED POSTS ───
    void   savePost(string postId, User* postOwner);
    void   unsavePost(string postId);
    void   loadSavedPosts(User** allUsers, int userCount);
    void   saveSavedPostsToFile();
    void   clearSavedPostsArray();
    Posts* getSavedPostByIndex(int index);
    bool   hasSavedPost(const string& postId) const;

    // ─── REPORTING ───
    void reportUser();
    bool hasReportedUser(const string& reporterUsername) const;
    void reportUserBy(const string& reporterUsername);

    // ─── ACCOUNT ───
    void deleteAccount(User**& allUsers, int& userCount);

    // ─── HELPERS ───
    bool hasPost(const string& postId) const;

    // ─── CONVERSATION HISTORY (Messages) ───
    static string* getConversationHistory(const string& username, int& outCount);
    static void addConversationToHistory(const string& username, const string& peerUsername);
    static void removeConversationFromHistory(const string& username, const string& peerUsername);
};

// ─── FREE FUNCTIONS ───
void  loadAllUsers(User** allUsers, int& userCount);
User* signUp(User**& allUsers, int& userCount, string username, string password, string bio);
User* findAndLogin(User**& allUsers, int userCount, string username, string password);