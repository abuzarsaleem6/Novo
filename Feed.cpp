// Feed.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Feed.h"
#include <ctime>
#include <QDebug>
#include <algorithm>

using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  CONSTRUCTOR & DESTRUCTOR
// ══════════════════════════════════════════════════════════════════════════════

Feed::Feed(string username) {
    this->ownerUsername = username;
    this->postCount = 0;
    this->postCapacity = 100;
    this->feedPosts = new Posts * [postCapacity];
    memset(feedPosts, 0, sizeof(Posts*) * postCapacity);
    qDebug() << "Feed created for:" << QString::fromStdString(username);
}

Feed::~Feed() {
    if (feedPosts != nullptr) {
        // NOTE: We do NOT delete individual Posts* because they are references
        // to posts owned by User objects. The User class handles cleanup.
        delete[] feedPosts;
        feedPosts = nullptr;
    }
    qDebug() << "Feed destroyed for:" << QString::fromStdString(ownerUsername);
}

// ══════════════════════════════════════════════════════════════════════════════
//  PRIVATE HELPERS
// ══════════════════════════════════════════════════════════════════════════════

void Feed::addPostToFeed(Posts* p) {
    if (!p) return;

    // Expand capacity if needed
    if (postCount >= postCapacity) {
        postCapacity *= 2;
        Posts** newArray = new Posts * [postCapacity];
        for (int i = 0; i < postCount; i++) {
            newArray[i] = feedPosts[i];
        }
        delete[] feedPosts;
        feedPosts = newArray;
    }

    // Add post at the end
    feedPosts[postCount] = p;
    postCount++;
}

void Feed::clearFeed() {
    // Clear the feed but do NOT delete posts (User owns them)
    postCount = 0;
    memset(feedPosts, 0, sizeof(Posts*) * postCapacity);
}

// ══════════════════════════════════════════════════════════════════════════════
//  CORE FEED LOGIC - THIS IS WHAT Qt GUI CALLS
// ══════════════════════════════════════════════════════════════════════════════

void Feed::loadFeedFromFollowedUsers(User* currentUser, User** allUsers, int userCount) {
    clearFeed();

    if (!currentUser || !allUsers) {
        qDebug() << "ERROR: Invalid user or allUsers pointer in loadFeedFromFollowedUsers()";
        return;
    }

    // Step 1: Load the current user's following list
    currentUser->loadFollowing(allUsers, userCount);
    int followingCount = currentUser->getFollowingCount();

    if (followingCount == 0) {
        qDebug() << "User is not following anyone. Feed is empty.";
        return;
    }

    qDebug() << QString::fromStdString(currentUser->getUsername())
        << "is following" << followingCount << "users";

    // Step 2: Iterate through all users in the system
    for (int u = 0; u < userCount; ++u) {
        if (!allUsers[u]) continue;

        // Skip current user (don't show own posts in feed)
        if (allUsers[u]->getUsername() == currentUser->getUsername()) {
            continue;
        }

        // Check if we follow this user
        if (!currentUser->isFollowing(allUsers[u]->getUsername())) {
            continue;
        }

        // Step 3: Load all posts from this followed user
        allUsers[u]->loadAllPosts();
        int postsFromThisUser = allUsers[u]->getPostCount();

        qDebug() << "Loading" << postsFromThisUser << "posts from"
            << QString::fromStdString(allUsers[u]->getUsername());

        // Step 4: Add each post to the feed
        for (int p = 0; p < postsFromThisUser; ++p) {
            Posts* post = allUsers[u]->getPostByIndex(p);

            // Validate post before adding
            if (post && post->isValid()) {
                addPostToFeed(post);
                qDebug() << "Added post" << QString::fromStdString(post->getPostId());
            }
            else {
                qDebug() << "WARNING: Invalid post at index" << p;
            }
        }
    }

    qDebug() << "Feed loaded with" << postCount << "total posts";
}

// ══════════════════════════════════════════════════════════════════════════════
//  ADDITIONAL FEED OPERATIONS
// ══════════════════════════════════════════════════════════════════════════════

void Feed::sortByNewest() {
    // Sort posts by time of creation (newest first)
    // This uses bubble sort for simplicity — can be optimized with quicksort

    for (int i = 0; i < postCount - 1; ++i) {
        for (int j = 0; j < postCount - i - 1; ++j) {
            if (!feedPosts[j] || !feedPosts[j + 1]) continue;

            string time1 = feedPosts[j]->getTimeOfCreation();
            string time2 = feedPosts[j + 1]->getTimeOfCreation();

            // Compare timestamps (lexicographic comparison works for "YYYY-MM-DD HH:MM:SS")
            if (time1 < time2) {
                // Swap
                Posts* temp = feedPosts[j];
                feedPosts[j] = feedPosts[j + 1];
                feedPosts[j + 1] = temp;
            }
        }
    }

    qDebug() << "Feed sorted by newest posts";
}



// ══════════════════════════════════════════════════════════════════════════════
//  GETTERS - USED BY Qt GUI
// ══════════════════════════════════════════════════════════════════════════════

Posts** Feed::getPosts() const {
    return feedPosts;
}

int Feed::getPostCount() const {
    return postCount;
}

string Feed::getOwner() const {
    return ownerUsername;
}