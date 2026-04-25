
#include <QDir>
#include"Admin.h"
#include "Notification.h"
Admin* authenticateAdmin(const QString& username, const QString& password) {
	QFile file("data/Admin/admin_credentials.txt");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Admin credentials file not found";
		return nullptr;
	}

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine().trimmed();
		if (line.isEmpty()) continue;

		QStringList parts = line.split("|");
		if (parts.size() < 2) continue;

		QString fileUsername = parts[0];
		QString filePassword = parts[1];

		if (username == fileUsername && password == filePassword) {
			file.close();

			// Create and load admin
			Admin* admin = new Admin(username, password,parts.size() > 2 ? parts[2] : "Admin");
			admin->loadAdminFromFile();
			return admin;
		}
	}

	file.close();
	return nullptr;
}
Admin::Admin() : User() {
	
	reportedUserCount = 0;
	reportedPostCount = 0;
}

Admin::Admin(QString username, QString password, QString bio) : User(username.toStdString(), password.toStdString(), bio.toStdString()) {
	
	reportedUserCount = 0;
	reportedPostCount = 0;
}

void Admin::deleteUser(User**& allUsers, int& userCount, QString username) {
	for (int i = 0; i < userCount; i++) {
		if (QString::fromStdString(allUsers[i]->getUsername()) == username) {
			allUsers[i]->deleteAccount(allUsers, userCount);
			qDebug() << "User" << username << "deleted by admin ";
			return;
		}
	}
	qDebug() << "User not found ";
}


void Admin::deletePost(User** allUsers, int userCount, QString postId) {
	for (int i = 0; i < userCount; i++) {
		Posts* post = allUsers[i]->getPostById(postId.toStdString());
		if (post != nullptr) {
			allUsers[i]->deletePost(postId.toStdString());
			qDebug() << "Post" << postId << "deleted by admin.";
			return;
		}
	}
	qDebug() << "Post not found for deletion.";
}
void Admin::deleteComment(Posts* post, int commentIndex) {
	if (!post) return;
	QList<Comment> comments = post->getComments();
	if (commentIndex < 0 || commentIndex >= comments.size()) {
		qDebug() << "Invalid comment index.";
		return;
	}
	// Admin bypasses ownership check
	post->deleteCommentAsAdmin(commentIndex);
}

void Admin::reviewReports(User**& allUsers, int& userCount) {
	qDebug() << "Reviewing reported users and posts...";
	for (int i = 0; i < reportedUsers.size(); i++) {
		User* user = reportedUsers[i];
		qDebug() << "User: " << QString::fromStdString(user->getUsername()) << " has been reported " << reportedUsers.count(user) << " times.";
	}
	for (int i = 0; i < reportedPosts.size(); i++) {
		Posts* post = reportedPosts[i];
		qDebug() << "Post: " << QString::fromStdString(post->getPostId()) << " has been reported " << reportedPosts.count(post) << " times.";
	}
}


Admin::~Admin() {
	reportedUsers.clear();
	reportedPosts.clear();
	
	adminNotifications.clear();
}

void Admin::displayAdminDashboard() {
	qDebug() << "Admin Dashboard";
	qDebug() << "Username: " << QString::fromStdString(getUsername());
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
	for (int i = 0; i < reportedPosts.size(); i++) {
		Posts* post = reportedPosts[i];
		qDebug() << "Post ID: " << QString::fromStdString(post->getPostId()) << " | Report Count: " << reportedPosts.count(post) << Qt::endl;
	}
}

void Admin::receiveReport(User* user) {
	reportedUsers.append(user);
	reportedUserCount++;
	addNotification("New report received for user: " + QString::fromStdString(user->getUsername()));
	saveReportsToFile();
	saveAdminToFile();
}
void Admin::receivePostReport(Posts* post) {
	reportedPosts.append(post);
	reportedPostCount++;
	addNotification("New report received for post: " +
		QString::fromStdString(post->getPostId()));
	saveReportsToFile();
	saveAdminToFile();
}
Admin::Admin(const Admin& other) : User(other) {
	
	reportedUsers = other.reportedUsers;
	reportedPosts = other.reportedPosts;
	reportedUserCount = other.reportedUserCount;
	reportedPostCount = other.reportedPostCount;
}

Admin& Admin::operator=(const Admin& other) {
	if (this != &other) {
		User::operator=(other);
		
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
		post->deleteCommentAsAdmin(index);
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
	adminNotifications.append(message);
}

void Admin::viewAllNotifications() const {
	for (const QString& n : adminNotifications) {
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
	// FIX 7: QDir().mkpath() replaces system("mkdir ...") — cross-platform, no shell
	QDir().mkpath("data/Admin");

	// FIX 2: use QPair<Posts*, int> to deduplicate before writing,
	// so save→load cycles don't inflate counts.
	QList<QPair<Posts*, int>> deduplicated;
	for (Posts* post : reportedPosts) {
		bool found = false;
		for (auto& pair : deduplicated) {
			if (pair.first == post) { found = true; break; }
		}
		if (!found)
			deduplicated.append(qMakePair(post, reportedPosts.count(post)));
	}

	ofstream file("data/Admin/reported_posts.txt");
	if (file.is_open()) {
		for (const auto& pair : deduplicated) {
			file << "postId|" << pair.first->getPostId() << "\n";
			file << "creatorUsername|" << pair.first->getCreatorUsername() << "\n";
			file << "reportCount|" << pair.second << "\n";
		}
		file.close();
		qDebug() << "Reported posts saved!"; // FIX 6
	}

	// FIX 2 (users): save reported users using QPair<User*, int> the same way
	QList<QPair<User*, int>> deduplicatedUsers;
	for (User* user : reportedUsers) {
		bool found = false;
		for (auto& pair : deduplicatedUsers) {
			if (pair.first == user) { found = true; break; }
		}
		if (!found)
			deduplicatedUsers.append(qMakePair(user, reportedUsers.count(user)));
	}

	ofstream usersFile("data/Admin/reported_users.txt");
	if (usersFile.is_open()) {
		for (const auto& pair : deduplicatedUsers) {
			usersFile << "username|" << pair.first->getUsername() << "\n";
			usersFile << "reportCount|" << pair.second << "\n";
		}
		usersFile.close();
		qDebug() << "Reported users saved!"; // FIX 6
	}
}

void Admin::loadReportsFromFile(User**& allUsers, int& userCount) {
	// --- load reported posts ---
	ifstream file("data/Admin/reported_posts.txt");
	if (!file.is_open()) {
		reportedPostCount = 0;
	}
	else {
		string line;
		QString postId;
		QString creatorUsername;
		int count = 0;

		while (getline(file, line)) {
			if (line.find("postId|") == 0) {
				postId = QString::fromStdString(line.substr(7));
			}
			else if (line.find("creatorUsername|") == 0) {
				creatorUsername = QString::fromStdString(line.substr(16));
			}
			else if (line.find("reportCount|") == 0) {
				count = stoi(line.substr(12));
				for (int i = 0; i < userCount; i++) {
					if (allUsers[i] && QString::fromStdString(allUsers[i]->getUsername()) == creatorUsername) {
						Posts* p = allUsers[i]->getPostById(postId.toStdString());
						if (p) {
							for (int j = 0; j < count; j++)
								reportedPosts.append(p);
						}
						break;
					}
				}
			}
		}
		file.close();
		reportedPostCount = reportedPosts.size();
		qDebug() << "Reported posts loaded!"; // FIX 6
	}

	// --- load reported users (FIX 2) ---
	ifstream usersFile("data/Admin/reported_users.txt");
	if (usersFile.is_open()) {
		string line;
		QString username;
		int count = 0;

		while (getline(usersFile, line)) {
			if (line.find("username|") == 0) {
				username = QString::fromStdString(line.substr(9));
			}
			else if (line.find("reportCount|") == 0) {
				count = stoi(line.substr(12));
				for (int i = 0; i < userCount; i++) {
					if (allUsers[i] && QString::fromStdString(allUsers[i]->getUsername()) == username) {
						for (int j = 0; j < count; j++)
							reportedUsers.append(allUsers[i]);
						break;
					}
				}
			}
		}
		usersFile.close();
		reportedUserCount = reportedUsers.size();
		qDebug() << "Reported users loaded!"; // FIX 6
	}
}


void Admin::saveAdminToFile() {
	// FIX 7: QDir().mkpath() replaces system("mkdir ...")
	QDir().mkpath("data/Admin");

	ofstream file("data/Admin/adminInfo.txt");
	if (file.is_open()) {
		file << "username|" << getUsername() << "\n";
		file << "reportedUserCount|" << reportedUserCount << "\n";
		file << "reportedPostCount|" << reportedPostCount << "\n";
		file.close();
		qDebug() << "Admin info saved!"; // FIX 6
	}
}

void Admin::loadAdminFromFile() {
	ifstream file("data/Admin/adminInfo.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			 if (line.find("reportedUserCount|") == 0) {
				reportedUserCount = stoi(line.substr(18));
			}
			else if (line.find("reportedPostCount|") == 0) {
				reportedPostCount = stoi(line.substr(18)); // FIX: was 19, correct is 18
			}
		}
		file.close();
		qDebug() << "Admin info loaded!"; // FIX 6
	}
}
void Admin::saveProcessedReports() {
	QDir().mkpath("data/Admin");

	// Save processed posts
	QFile postsFile("data/Admin/processed_post_reports.txt");
	if (postsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&postsFile);
		for (const QString& postId : processedPostReports) {
			out << postId << "\n";
		}
		postsFile.close();
	}

	// Save processed users
	QFile usersFile("data/Admin/processed_user_reports.txt");
	if (usersFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&usersFile);
		for (const QString& username : processedUserReports) {
			out << username << "\n";
		}
		usersFile.close();
	}
}
void Admin::loadProcessedReports() {
	QDir().mkpath("data/Admin");

	// Load processed posts
	QFile postsFile("data/Admin/processed_post_reports.txt");
	if (postsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&postsFile);
		while (!in.atEnd()) {
			QString line = in.readLine().trimmed();
			if (!line.isEmpty()) {
				processedPostReports.insert(line);
			}
		}
		postsFile.close();
	}

	// Load processed users
	QFile usersFile("data/Admin/processed_user_reports.txt");
	if (usersFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&usersFile);
		while (!in.atEnd()) {
			QString line = in.readLine().trimmed();
			if (!line.isEmpty()) {
				processedUserReports.insert(line);
			}
		}
		usersFile.close();
	}

	qDebug() << "Processed reports loaded. Posts:" << processedPostReports.size()
		<< "Users:" << processedUserReports.size();
}
void Admin::notifyOnPostThreshold(const QString& postId, const QString& creatorUsername) {
	// Prevent duplicate notifications
	if (processedPostReports.contains(postId)) {
		return;
	}

	QString message = "🚨 Alert: Post '" + postId + "' by @" + creatorUsername +
		" has reached 3 reports and is marked for review.";
	addNotification(message);
	processedPostReports.insert(postId);
	saveProcessedReports();
	saveAdminToFile();

	qDebug() << "Admin notified about reported post:" << postId;
}

void Admin::notifyOnUserThreshold(const QString& username) {
	// Prevent duplicate notifications
	if (processedUserReports.contains(username)) {
		return;
	}

	QString message = "🚨 Alert: User '@" + username +
		"' has reached 3 reports and is marked for review.";
	addNotification(message);
	processedUserReports.insert(username);
	saveProcessedReports();
	saveAdminToFile();

	qDebug() << "Admin notified about reported user:" << username;
}