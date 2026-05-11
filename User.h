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
    bool isLoggedIn;
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
	// Constructors, Destructor, and Assignment Operator
    User();
    User(string username, string password, string bio);
    
    ~User();

	//Helper functions 
    static string validateUsername(const string& username);
    static string validatePassword(const string& password);
    static string validateBio(const string& bio);
    static string validatePostContent(const string& content);

    
    bool login(string password);
    void logOut();

    
    bool updateBio(const string& newBio);
    bool updatePassword(const string& newPassword);

    
    void saveToFile();
    void loadFromFile(string username);
    void addToUserList();
    void removeFromUser_List(string username);
    

	// Getters
    string getUsername()      const;
    string getBio()           const;
    string getPassword()      const;
    bool   getIsReported()    const;
    int getIsReportedCount() const;
    bool   getIsLoggedIn()    const;
    int    getFollowingCount()const;
    int    getFollowersCount()const;
    int    getPostCount()     const;
    int    getSavedPostCount()const;

	// setters
    void setBio(string bio);
    void setPassword(string password);
    void setIsReported(bool val);
    void setIsReportedCount(int val);
	// Helper functions for following and followers management
    void followUser(User* target, User** allUsers, int userCount);
    void unfollowUser(string username);
    bool isFollowing(string username);
    void addFollower(User* ptr);
    void removeFollower(const string& usernameToRemove);
    void loadFollowing(User** allUsers, int userCount);
    void loadFollowers(User** allUsers, int userCount);

	// Helper functions for post management
    void   createPost(string content);
    void   loadAllPosts();
    Posts* getPostById(string postId);
    Posts* getPostByIndex(int index);
    bool   editPost(const string& postId, const string& newContent, string& errorOut);
    void   deletePost(string postId);
    void   reportPost(string postId, User* postOwner);

	// Helper functions for saved posts management
    void   savePost(string postId, User* postOwner);
    void   unsavePost(string postId);
    void   loadSavedPosts(User** allUsers, int userCount);
    void   saveSavedPostsToFile();
    void   clearSavedPostsArray();
    Posts* getSavedPostByIndex(int index);
    bool   hasSavedPost(const string& postId) const;

    
    void reportUser();
    bool hasReportedUser(const string& reporterUsername) const;
    void reportUserBy(const string& reporterUsername);

    
    void deleteAccount(User**& allUsers, int& userCount);

   
    bool hasPost(const string& postId) const;

    
   
};


void  loadAllUsers(User** allUsers, int& userCount);
User* signUp(User**& allUsers, int& userCount, string username, string password, string bio);
User* findAndLogin(User**& allUsers, int userCount, string username, string password);