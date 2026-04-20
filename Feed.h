// Feed.h
#pragma once
#ifndef FEED_H
#define FEED_H

#include <iostream>
#include <string>
#include <QList>
#include <QString>
#include "Post.h"
#include "User.h"

using namespace std;

class Feed {
private:
    Posts** feedPosts;
    int postCount;
    int postCapacity;
    string ownerUsername;

    // Private helper to dynamically add posts
    void addPostToFeed(Posts* p);
    void clearFeed();

public:
    // Constructor & Destructor
    Feed(string username);
    ~Feed();

    // ─── CORE FEED OPERATIONS ───

    // Load feed from all followed users' posts (MAIN FUNCTION USED BY GUI)
    void loadFeedFromFollowedUsers(User* currentUser, User** allUsers, int userCount);

    // Refresh feed display (console only, not used by Qt)
    void refreshFeed();

    // Sort posts by time (newest first)
    void sortByNewest();

    // ─── GETTERS FOR QT GUI ───
    Posts** getPosts() const;
    int getPostCount() const;
    string getOwner() const;
};

#endif