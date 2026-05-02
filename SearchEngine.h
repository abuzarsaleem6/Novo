// SearchEngine.h
#pragma once
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <string>
#include <iostream>
#include "User.h"

using namespace std;

class SearchEngine {
public:
    SearchEngine();
    SearchEngine(User** users, int count);
    void setUsers(User** users, int count);

    // Exact match ke bajaye substring search:
    User** searchUsersBySubstring(const string& query, int& outResultsCount) const;

private:
    User** allUsers;
    int    userCount;
};

#endif