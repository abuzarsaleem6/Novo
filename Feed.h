#ifndef FEED_H
#define FEED_H
#include <iostream>
#include <string>
#include "Post.h" 
using namespace std;
class Feed {
private:
    Posts** feedPosts; 
    int postCount;
    string ownerUsername;
    void addPostToFeed(Posts* p);
public:
    Feed(string username);
    ~Feed();

    // Main Logic Functions
    void loadFeedFromFollowedUsers();
    void refreshFeed();

    // Getters for Qt/Main (Inhein 'const' hi rehne dein)
    Posts** getPosts() const;
    int getPostCount() const;
    string getOwner() const;
};

#endif