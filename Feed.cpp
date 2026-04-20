#include "Feed.h"
Feed::Feed(string username) {
    this->ownerUsername = username;
    this->postCount = 0;
    this->feedPosts = nullptr;
}
Feed::~Feed() {
    if (feedPosts != nullptr) {
        delete[] feedPosts; 
    }
}
void Feed::addPostToFeed(Posts* p) {
    Posts** newArray = new Posts * [postCount + 1];
    for (int i = 0; i < postCount; i++) {
        newArray[i] = feedPosts[i];
    }
    newArray[postCount] = p;
    delete[] feedPosts;
    feedPosts = newArray;
    postCount++;
}
void Feed::refreshFeed() {
    cout << "\n--- " << ownerUsername << "'s Social Feed ---" << endl;
    if (postCount == 0 || feedPosts == nullptr) {
        cout << "Your feed is empty!" << endl;
        return;
    }
    for (int i = 0; i < postCount; i++) {
        feedPosts[i]->display(); 
    }
}
void Feed::loadFeedFromFollowedUsers() {
    cout << "Loading feed for: " << ownerUsername << "..." << endl;
}

// Qt Getters
Posts** Feed::getPosts() const { return feedPosts; }
int Feed::getPostCount() const { return postCount; }
string Feed::getOwner() const { return ownerUsername; }