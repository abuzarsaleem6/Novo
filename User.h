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
	bool isSuspended;
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
	void validatePassWord(string& username);
	void InputBio(string& bio);
	void validateBio(string& bio);
	void logOut();
	void reportUser();
	void updateUsername();
	void updatePassword();
	void updateBio();
	void displayProfile();
	void saveToFile();
	void loadFromFile(string username);

	void removeFromUser_List(string username);
};
void loadAllUsers(User** allUsers, int& userCount);

