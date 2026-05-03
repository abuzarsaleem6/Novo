#define _CRT_SECURE_NO_WARNINGS
#include "Feed.h"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

// Constructor
Feed::Feed(string username) {


    this->ownerUsername = username;
    this->postCount = 0;
    this->postCapacity = 100;
    this->feedPosts = new Posts * [postCapacity];

   
    for (int i = 0; i < postCapacity; i++) {

        feedPosts[i] = nullptr;

    }
}


Feed::~Feed() {

    if (feedPosts != nullptr) {
 
        delete[] feedPosts;
        feedPosts = nullptr;

    }
}

//Helper functions
void Feed::addPostToFeed(Posts* p) {

    if (!p) return;

    if (postCount >= postCapacity) {

        postCapacity *= 2;
        Posts** newArray = new Posts * [postCapacity];

        
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


void Feed::clearFeed() {

    postCount = 0;
   
    for (int i = 0; i < postCapacity; i++) {

        feedPosts[i] = nullptr;

    }
}


void Feed::loadFeedFromFollowedUsers(User* currentUser, User** allUsers, int userCount) {

    clearFeed();

    if (!currentUser || !allUsers) {

        cerr << "ERROR: Invalid pointers in loadFeed\n";
        return;

    }

    currentUser->loadFollowing(allUsers, userCount);

    for (int u = 0; u < userCount; ++u) {

        if (!allUsers[u]) {
            continue;
        }

        if (allUsers[u]->getUsername() == currentUser->getUsername()) {
            continue;
        }

        if (!currentUser->isFollowing(allUsers[u]->getUsername())) {
            continue;
        }

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
    
    for (int i = 0; i < postCount - 1; ++i) {

        for (int j = 0; j < postCount - i - 1; ++j) {

            if (!feedPosts[j] || !feedPosts[j + 1]){
                continue;
            }

            if (feedPosts[j]->getTimeOfCreation() < feedPosts[j + 1]->getTimeOfCreation()) {

                Posts* temp = feedPosts[j];
                feedPosts[j] = feedPosts[j + 1];
                feedPosts[j + 1] = temp;

            }
        }
    }
}


Posts** Feed::getPosts() const {

    return feedPosts; 

}
int Feed::getPostCount() const {

    return postCount; 

}
string Feed::getOwner() const {

    return ownerUsername;

}