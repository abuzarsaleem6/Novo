#pragma once
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class Post;
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
	string* followingUsernames;  
	string* followerUsernames;
	Notification* notifications;
	int notificationCount;
	User** following;
	int  followingCount;
	User** followers;
	int followersCount;
	Post* posts;
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
	void addToUserList();
	void loadFromFile(string username);
	bool login(string  password);
	void removeFromUser_List(string username);
	string getUsername();
};
void loadAllUsers(User** allUsers, int& userCount);
User* signUp(User**& allUsers, int& userCount);
User* findAndLogin(User** allUsers, int userCount);