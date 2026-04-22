#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Post.h"

class Admin :public User {
private:
	QString adminLevel;
	QList<User*> reportedUsers;
	QList<Posts*> reportedPosts;
	int reportedUserCount;
	int reportedPostCount;
public:
	Admin();
	Admin(QString username, QString password);
	void deleteUser(User**& allUsers, int& userCount, QString username);
	void deletePost(User** allUsers, int userCount, QString postId);
	void deleteComment(Posts* post, int commentIndex);
	void reviewReports(User**& allUsers, int& userCount);
	~Admin();
	void displayAdminDashboard();
	void displayReportedUsers();
	void displayReportedPosts();
	void receiveReport(User* user);
	Admin(const Admin& other);
	Admin& operator=(const Admin& other);

};



#endif