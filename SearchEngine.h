// SearchEngine.h
#pragma once
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <string>
#include <iostream>
#include "User.h"

using namespace std;

class SearchEngine {
private:
    User** allUsers;
    int    userCount;

public:
    SearchEngine();
    SearchEngine(User** allUsers, int userCount);

    void   setUsers(User** allUsers, int userCount);
    User* searchUser(const string& username) const;
};

#endif