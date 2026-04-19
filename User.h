#pragma once
#include<iostream>
#include<fstream>
#include<string>
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
	int isReportedCount;
	Notification* notifications;
	int notificationCount;
	User** following;
	int  followingCount;
	User** followers;
	int followersCount;
	Posts* posts;
	int postCount;
public:
	User();
	User(string username, string password);
	void InputUserName(string& username);
	void InputPassWord(string& password);
	void validateUsername(string& username);
	void validatePassWord(string& password);
	void InputBio(string& bio);
	void validateBio(string& bio);
	void logOut();
	void reportUser();
	void updatePassword();
	void updateBio();
	void displayProfile();
	void saveToFile();
	void addToUserList();
	void loadFromFile(string username);
	bool login(string  password);
	void removeFromUser_List(string username);
	string getUsername();
	void followUser(User* target, User** allUsers, int userCount);
	bool isFollowing(string username);
	void addFollower(User* ptr);
	void unfollowUser(string username);
	void removeFollower(User* ptr);
	void loadFollowing(User** allUsers, int userCount);
	void loadFollowers(User** allusers, int userCount);
	void deleteAccount(User**& allUsers, int& userCount);
	void addToReviewList();
	//void createPost(string content);
	//void loadAllPosts();
	//void displayAllPosts();
	//Posts* getPostById(string postId);

	bool getIsReported()const;
};
void loadAllUsers(User** allUsers, int& userCount);
User* signUp(User**& allUsers, int& userCount);
User* findAndLogin(User** allUsers, int userCount);