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
    User* searchUser(const std::string& username) const;
private:
    User** allUsers;
    int    userCount;
};

#endif