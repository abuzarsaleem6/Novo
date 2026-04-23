#include"SearchEngine.h"
#include<QDebug>

SearchEngine::SearchEngine() {
}

SearchEngine::~SearchEngine() {
}

void SearchEngine::addUser(User* user) {
	allUsers.append(user);
}

User* SearchEngine::searchUsers(const QString& username) const {
	for (User* user : allUsers) {
		if (user->getUsername() == username) {
			return user;
		}
	}
	return nullptr;
}

void SearchEngine::displayResults(const QString& username) const {
	User* user = searchUsers(username);
	if (user) {
		// Display user information
		if(user->getIsReported()) {
			qDebug() << "Username: " << QString::fromStdString(user->getUsername()) << " (Reported)";
		} else {
			qDebug() << "Username: " << QString::fromStdString(user->getUsername());
		}	
	} else {
		qDebug()<<"No user found!";
	}
}

QList<User*> SearchEngine::searchPartial(const QString& query) const {
	QList<User*> results;
	for (User* user : allUsers) {
		if (QString::fromStdString(user->getUsername()).contains(query, Qt::CaseInsensitive)) {
			results.append(user);
		}
	}
	return results;
}

int SearchEngine::getUserCount() const {
	return allUsers.size();
}

void SearchEngine::setUserPool(User** users, int count) {
	allUsers.clear();	
	for (int i = 0; i < count; ++i) {
		allUsers.append(users[i]);
	}
}
