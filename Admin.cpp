#include"Admin.h"
#include "Notification.h"

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
	if (reportCount >= 3) {
		for (int i = 0; i < userCount; i++) {
			if (QString::fromStdString(reportedUsers[i]->getUsername()) == username) {
				allUsers[i]->deleteAccount(allUsers, userCount);
				break;
			}
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
	addNotification("New report received for user: " + QString::fromStdString(user->getUsername()));
	saveReportsToFile();
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

void Admin::banUser(User* user) {
	if (user) {
		user->setBan(true);
		addNotification("User banned: " +
			QString::fromStdString(user->getUsername()));
	}
}

void Admin::unbanUser(User* user) {
	if (user) {
		user->setBan(false);
		addNotification("User unbanned: " +
			QString::fromStdString(user->getUsername()));
	}
}

void Admin::deleteReportedComment(Posts* post, int index) {
	if (post) {
		post->deleteComment(index, QString::fromStdString(getUsername()));
	}
}

void Admin::viewReportedComments(Posts* post) {
	if (post) {
		QList<Comment> comments = post->getComments();
		for (int i = 0; i < comments.size(); i++) {
			if (comments[i].getIsReported()) {
				qDebug() << i << ":" << comments[i].getContent()
					<< "by" << comments[i].getCreatorUsername();
			}
		}
	}
}

void Admin::addNotification(const QString& message) {
	notifications.append(message);
}

void Admin::viewAllNotifications() const {
	for (const QString& n : notifications) {
		qDebug() << n;
	}
}	

void Admin::check_ban_status(User* user) {
	if (user) {
		if (reportedUsers.count(user) >= 3) {
			banUser(user);
		}
		else {
			unbanUser(user);
		}
	}
}

void Admin::saveReportsToFile() {
	string folder = "data/Admin";
#ifdef _WIN32
	system(("mkdir \"" + folder + "\" 2>nul").c_str());
#else
	system(("mkdir -p \"" + folder + "\"").c_str());
#endif

	string path = folder + "/reports.txt";
	ofstream file(path);
	if (file.is_open()) {
		for (int i = 0; i < reportedUsers.size(); i++) {
			file << "username|" << reportedUsers[i]->getUsername() << "\n";
			file << "reportCount|" << reportedUsers.count(reportedUsers[i]) << "\n";
		}
		file.close();
		cout << "Reports saved!" << endl;
	}
}

void Admin::loadReportsFromFile(User**& allUsers, int& userCount) {
	string path = "data/Admin/reports.txt";
	ifstream file(path);
	if (file.is_open()) {
		string line;
		QString username;
		int count = 0;
		while (getline(file, line)) {
			if (line.find("username|") == 0) {
				username = QString::fromStdString(line.substr(9));
			}
			else if (line.find("reportCount|") == 0) {
				count = stoi(line.substr(12));
				// ab us user ko dhundo aur reportedUsers mein add karo
				for (int i = 0; i < userCount; i++) {
					if (QString::fromStdString(allUsers[i]->getUsername()) == username) {
						for (int j = 0; j < count; j++) {
							reportedUsers.append(allUsers[i]);
						}
						break;
					}
				}
			}
		}
		file.close();
		cout << "Reports loaded!" << endl;
	}
}

void Admin::saveAdminToFile() {
	string folder = "data/Admin";
#ifdef _WIN32
	system(("mkdir \"" + folder + "\" 2>nul").c_str());
#else
	system(("mkdir -p \"" + folder + "\"").c_str());
#endif

	string path = folder + "/adminInfo.txt";
	ofstream file(path);
	if (file.is_open()) {
		file << "username|" << getUsername() << "\n";
		file << "adminLevel|" << adminLevel.toStdString() << "\n";
		file << "reportedUserCount|" << reportedUserCount << "\n";
		file << "reportedPostCount|" << reportedPostCount << "\n";
		file.close();
		cout << "Admin info saved!" << endl;
	}
}

void Admin::loadAdminFromFile() {
	string path = "data/Admin/adminInfo.txt";
	ifstream file(path);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (line.find("adminLevel|") == 0) {
				adminLevel = QString::fromStdString(line.substr(11));
			}
			else if (line.find("reportedUserCount|") == 0) {
				reportedUserCount = stoi(line.substr(18));
			}
			else if (line.find("reportedPostCount|") == 0) {
				reportedPostCount = stoi(line.substr(19));
			}
		}
		file.close();
		cout << "Admin info loaded!" << endl;
	}
}