#pragma once
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include"User.h"
#include<QList>

class SearchEngine {
private:
	QList<User*> allUsers;  // List of all users in the system
public:
	SearchEngine();
	~SearchEngine();
	void addUser(User* user);
	User* searchUsers(const QString& username) const;
	//void displayResults(const QString& username) const;
};

#endif