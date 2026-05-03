#include "SearchEngine.h"

// Implementation of SearchEngine class

SearchEngine::SearchEngine() : allUsers(nullptr), userCount(0) {}

SearchEngine::SearchEngine(User** users, int count) : allUsers(users), userCount(count) {}

void SearchEngine::setUsers(User** users, int count) {
    allUsers = users;
    userCount = count;
}

User** SearchEngine::searchUsersBySubstring(const string& query, int& outResultsCount) const {
    outResultsCount = 0;
    if (query.empty() || !allUsers) 
        return nullptr;

    
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i]) {
            string uname = allUsers[i]->getUsername();

            
            if (uname == "admin123") 
                continue;

            if (uname.find(query) != string::npos) {
                outResultsCount++;
            }
        }
    }

    if (outResultsCount == 0) return nullptr;

    
    User** results = new User * [outResultsCount];
    int currentMatch = 0;

    
    for (int i = 0; i < userCount; i++) {
        if (allUsers[i]) {
            string uname = allUsers[i]->getUsername();

            if (uname == "admin123") 
                continue;

            if (uname.find(query) != string::npos) {
                results[currentMatch++] = allUsers[i];
            }
        }
    }

    return results;
}