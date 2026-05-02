#define _CRT_SECURE_NO_WARNINGS
#include "Feed.h"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

// ─── CONSTRUCTOR ───
Feed::Feed(string username) {
    this->ownerUsername = username;
    this->postCount = 0;
    this->postCapacity = 100;
    this->feedPosts = new Posts * [postCapacity];

    // Replacing memset: Simple loop to initialize pointers to nullptr
    for (int i = 0; i < postCapacity; i++) {
        feedPosts[i] = nullptr;
    }
}

// ─── DESTRUCTOR ───
Feed::~Feed() {
    if (feedPosts != nullptr) {
        // Note: We don't delete the Posts themselves here 
        // because the Feed only "borrows" them from the Users.
        delete[] feedPosts;
        feedPosts = nullptr;
    }
}

// ─── ADD POST ───
void Feed::addPostToFeed(Posts* p) {
    if (!p) return;

    if (postCount >= postCapacity) {
        postCapacity *= 2;
        Posts** newArray = new Posts * [postCapacity];

        // Copy old pointers and initialize the rest of the new capacity
        for (int i = 0; i < postCapacity; i++) {
            if (i < postCount) {
                newArray[i] = feedPosts[i];
            }
            else {
                newArray[i] = nullptr;
            }
        }

        delete[] feedPosts;
        feedPosts = newArray;
    }
    feedPosts[postCount++] = p;
}

// ─── CLEAR FEED ───
void Feed::clearFeed() {
    postCount = 0;
    // Replacing memset: Explicitly nullify existing entries
    for (int i = 0; i < postCapacity; i++) {
        feedPosts[i] = nullptr;
    }
}

// ─── LOADING & SORTING ───
void Feed::loadFeedFromFollowedUsers(User* currentUser, User** allUsers, int userCount) {
    clearFeed();

    if (!currentUser || !allUsers) {
        cerr << "ERROR: Invalid pointers in loadFeed\n";
        return;
    }

    currentUser->loadFollowing(allUsers, userCount);

    for (int u = 0; u < userCount; ++u) {
        if (!allUsers[u]) continue;
        if (allUsers[u]->getUsername() == currentUser->getUsername()) continue;
        if (!currentUser->isFollowing(allUsers[u]->getUsername())) continue;

        allUsers[u]->loadAllPosts();
        int postsFromThisUser = allUsers[u]->getPostCount();

        for (int p = 0; p < postsFromThisUser; ++p) {
            Posts* post = allUsers[u]->getPostByIndex(p);
            if (post && post->isValid()) {
                addPostToFeed(post);
            }
        }
    }
}

void Feed::sortByNewest() {
    // Standard Bubble Sort for pointers
    for (int i = 0; i < postCount - 1; ++i) {
        for (int j = 0; j < postCount - i - 1; ++j) {
            if (!feedPosts[j] || !feedPosts[j + 1]) continue;

            if (feedPosts[j]->getTimeOfCreation() < feedPosts[j + 1]->getTimeOfCreation()) {
                Posts* temp = feedPosts[j];
                feedPosts[j] = feedPosts[j + 1];
                feedPosts[j + 1] = temp;
            }
        }
    }
}

// ─── GETTERS ───
Posts** Feed::getPosts() const { return feedPosts; }
int Feed::getPostCount() const { return postCount; }
string Feed::getOwner() const { return ownerUsername; }