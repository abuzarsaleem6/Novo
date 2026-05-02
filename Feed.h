#pragma once
#ifndef FEED_H
#define FEED_H

#include <iostream>
#include <string>
#include "Post.h"
#include "User.h"

using namespace std;

class Feed {
private:
    Posts** feedPosts;
    int postCount;
    int postCapacity;
    string ownerUsername;

    void addPostToFeed(Posts* p);
    void clearFeed();

public:
    Feed(string username);
    ~Feed();

    void loadFeedFromFollowedUsers(User* currentUser, User** allUsers, int userCount);
    void sortByNewest();

    Posts** getPosts() const;
    int getPostCount() const;
    string getOwner() const;
};

#endif