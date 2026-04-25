#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Post.h"
#include <QDir>

class Admin :public User {
private:
	QList<User*> reportedUsers;
	QList<Posts*> reportedPosts;
	int reportedUserCount;
	int reportedPostCount;
	QList<QString> adminNotifications;
	QSet<QString> processedUserReports;    
	QSet<QString> processedPostReports;
public:
	Admin();
	Admin(QString username, QString password, QString bio);
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
	void receivePostReport(Posts* post);
	void banUser(User* user);
	void unbanUser(User* user);
	void deleteReportedComment(Posts* post, int commentIndex);
	void viewReportedComments(Posts* post);
	void addNotification(const QString& message);
	void viewAllNotifications() const;
	void check_ban_status(User* user);
	void saveReportsToFile();
	void loadReportsFromFile(User**& allUsers, int& userCount);
	void saveAdminToFile();
	void loadAdminFromFile();
	void notifyOnPostThreshold(const QString& postId, const QString& creatorUsername);
	void notifyOnUserThreshold(const QString& username);
	void loadProcessedReports();
	void saveProcessedReports();
};

Admin* authenticateAdmin(const QString& username, const QString& password);

#endif