#define _CRT_SECURE_NO_WARNINGS
#include "Comment.h"
#include <ctime>

static string getCurrentTimeString() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

//constructors

Comment::Comment(string content, string commentId, string creatorUsername)
    : content(content), commentId(commentId), creatorUsername(creatorUsername),
    isReported(false), likeCount(0) {
    timeOfCreation = getCurrentTimeString();
}

Comment::Comment() : content(""), commentId(""), creatorUsername(""),
isReported(false), likeCount(0) {
    timeOfCreation = getCurrentTimeString();
}

Comment::Comment(const Comment& other)
    : content(other.content),
    commentId(other.commentId),
    timeOfCreation(other.timeOfCreation),
    creatorUsername(other.creatorUsername),
    isReported(other.isReported),
    likeCount(other.likeCount) {
}

Comment& Comment::operator=(const Comment& other) {
    if (this != &other) {
        content = other.content;
        commentId = other.commentId;
        timeOfCreation = other.timeOfCreation;
        creatorUsername = other.creatorUsername;
        isReported = other.isReported;
        likeCount = other.likeCount;
    }
    return *this;
}

void Comment::editText(const string& newContent) {
    content = newContent;
}

//getters

string Comment::getContent() const {
    return content;
}

string Comment::getCommentId() const {
    return commentId;
}

string Comment::getTimeOfCreation() const {
    return timeOfCreation; 
}

string Comment::getCreatorUsername() const {
    return creatorUsername; 
}

bool Comment::getIsReported() const {
    return isReported; 
}

int Comment::getLikeCount() const {
    return likeCount;
}

//setters

void Comment::setContent(const string& newContent) {
    content = newContent; 
}
void Comment::setIsReported(bool reported) {
    isReported = reported;
}

void Comment::setTimeOfCreation(const string& time) {
    timeOfCreation = time; 
}

void Comment::setLikeCount(int count) { 
    likeCount = count; 
}

//like counts

void Comment::like() {
    likeCount++;
}

void Comment::unlike() {
    if (likeCount > 0)
        likeCount--;
}

string Comment::getDisplayName() const {
    if (creatorUsername.empty())
        return "Unknown User";
    return creatorUsername;
}