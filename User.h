#pragma once
#include<iostream>
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
	Notification* notifications;
	int notificationCount;
	User** following;
	int  followingCount;
	User** followers;
	int followersCount;
	Posts* posts;
	int postCount;
};
