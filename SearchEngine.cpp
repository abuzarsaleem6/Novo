// SearchEngine.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "SearchEngine.h"

using namespace std;

SearchEngine::SearchEngine() {
    allUsers = nullptr;
    userCount = 0;
}

SearchEngine::SearchEngine(User** users, int count) {
    allUsers = users;
    userCount = count;
}

void SearchEngine::setUsers(User** users, int count) {
    allUsers = users;
    userCount = count;
}

User* SearchEngine::searchUser(const string& username) const {
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i] && allUsers[i]->getUsername() == username)
            return allUsers[i];
    }
    cout << "User does not exist." << endl;
    return nullptr;
}