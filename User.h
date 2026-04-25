#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <QString>      
#include <QFile>
#include <QTextStream>
#include <QDebug>
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
	Notification** notifications;
	int notificationCount;
	User** following;
	int  followingCount;
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
	void logOut();
	void reportUser();
	bool updatePassword(const string& newPassword, QString& errorOut);
	bool updateBio(const string& newBio, QString& errorOut);
	static QString validatePostContent(const string& content);
	void displayProfile();
	void saveToFile();
	void addToUserList();
	void loadFromFile(string username);
	bool login(string  password);
	void removeFromUser_List(string username);
	string getUsername() const;
	void followUser(User* target, User** allUsers, int userCount);
	bool isFollowing(string username);
	void addFollower(User* ptr);
	void unfollowUser(string username);
	void removeFollower(User* ptr);
	void loadFollowing(User** allUsers, int userCount);
	void loadFollowers(User** allusers, int userCount);
	void deleteAccount(User**& allUsers, int& userCount);
	void addToReviewList();
	void createPost(string content);
	void loadAllPosts();
	void displayAllPosts();
	Posts* getPostById(string postId);
	bool getIsReported()const;
	string getBio() const;
	string getPassword() const;
	void setBio(string bio);
	void setPassword(string password);
	int getFollowingCount() const;
	int getFollowersCount() const;
	int getPostCount() const;
	Posts* getPostByIndex(int index);
	bool editPost(const string& postId, const string& newContent, QString& errorOut);
	void deletePost(string postId);
	void reportPost(string postId, User* postOwner);
	void savePost(string postId, User* postOwner);
	void unsavePost(string postId);
	void displaySavedPosts();
	void loadSavedPosts(User** allUsers, int userCount);
	void saveSavedPostsToFile();
	string getCreatorUsername() const;
	bool   getIsBanned()const;
	void setBan(bool banned);
	int getSavedPostCount() const;
	Posts* getSavedPostByIndex(int index);
	~User();
};
void loadAllUsers(User** allUsers, int& userCount);
User* signUp(User**& allUsers, int& userCount, string username, string password, string bio);
User* findAndLogin(User**& allUsers, int userCount, string username, string password);