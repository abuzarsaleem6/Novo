#include"Admin.h"

Admin::Admin() : User() {
	adminLevel = "Super";
	reportedUserCount = 0;
	reportedPostCount = 0;
}

Admin::Admin(QString username, QString password) : User(username.toStdString(), password.toStdString()) {
	adminLevel = "Super";
	reportedUserCount = 0;
	reportedPostCount = 0;
}

void Admin::deleteUser(User**& allUsers, int& userCount, QString username) {
	int reportCount = 0;
	for(int i = 0; i < reportedUsers.size(); i++) {
		if (QString::fromStdString(allUsers[i]->getUsername()) == username) {
			reportCount++;
			break;
		}
	}
	if (reportCount >= 3) {}
	for (int i = 0; i < userCount; i++) {
		if (QString::fromStdString(allUsers[i]->getUsername()) == username) {
			allUsers[i]->deleteAccount(allUsers, userCount);
			break;
		}
	}
}


void Admin::deletePost(User** allUsers, int userCount, QString postId) {
	int reportCount = 0;
	for (int i = 0; i < reportedPosts.size(); i++) {
		if (QString::fromStdString(reportedPosts[i]->getPostId()) == postId) {
			reportCount++;
			break;
		}
	}
	if (reportCount >= 3) {
		for (int i = 0; i < userCount; i++) {
			Posts* post = allUsers[i]->getPostById(postId.toStdString());
			if (post != nullptr) {
				post->~Posts();
				break;
			}
		}
	}
}

void Admin::deleteComment(Posts* post, int commentIndex) {
	if (commentIndex >= 0 && commentIndex < 3)
	{
		return;
	}
	QList<Comment> comments = post->getComments();
	if (commentIndex < 0 || commentIndex >= comments.size()) {
		comments.removeAt(commentIndex);
		post->saveCommentsToFile();
	}
}

void Admin::reviewReports(User**& allUsers, int& userCount) {
	qDebug() << "Reviewing reported users and posts...";
	for(int i = 0; i < reportedUsers.size(); i++) {
		User* user = reportedUsers[i];
		qDebug() << "User: " << QString::fromStdString(user->getUsername()) << " has been reported " << reportedUsers.count(user) << " times.";
	}
	for(int i = 0; i < reportedPostCount; i++) {
		Posts* post = reportedPosts[i];
		qDebug() << "Post: " << QString::fromStdString(post->getPostId()) << " has been reported " << reportedPosts.count(post) << " times.";
	}
}

Admin::~Admin() {
	reportedUsers.clear();
	reportedPosts.clear();
}

void Admin::displayAdminDashboard() {
	qDebug() << "Admin Dashboard";
	qDebug() << "Username: " << QString::fromStdString(getUsername());
	qDebug() << "Admin Level: " << adminLevel;
	qDebug() << "Reported Users: " << reportedUserCount;
	qDebug() << "Reported Posts: " << reportedPostCount;
}

void Admin::displayReportedUsers() {
	qDebug() << "Reported Users:" << Qt::endl;
	for (int i = 0; i < reportedUsers.size(); i++) {
		User* user = reportedUsers[i];
		qDebug() << "Username: " << QString::fromStdString(user->getUsername()) << " | Report Count: " << reportedUsers.count(user) << Qt::endl;
	}
}

void Admin::displayReportedPosts() {
	qDebug() << "Reported Posts:" << Qt::endl;
	for (int i = 0; i < reportedPostCount; i++) {
		Posts* post = reportedPosts[i];
		qDebug() << "Post ID: " << QString::fromStdString(post->getPostId()) << " | Report Count: " << reportedPosts.count(post) << Qt::endl;
	}
}

void Admin::receiveReport(User* user) {
	reportedUsers.append(user);
	reportedUserCount++;
}

Admin::Admin(const Admin& other) : User(other) {
	adminLevel = other.adminLevel;
	reportedUsers = other.reportedUsers;
	reportedPosts = other.reportedPosts;
	reportedUserCount = other.reportedUserCount;
	reportedPostCount = other.reportedPostCount;
}

Admin& Admin::operator=(const Admin& other) {
	if (this != &other) {
		User::operator=(other);
		adminLevel = other.adminLevel;
		reportedUsers = other.reportedUsers;
		reportedPosts = other.reportedPosts;
		reportedUserCount = other.reportedUserCount;
		reportedPostCount = other.reportedPostCount;
	}
	return *this;
}

