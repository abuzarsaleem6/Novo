#pragma once
#ifndef POST_H
#define POST_H

#include <string>
#include <iostream>
#include <fstream>
#include "Comment.h"

using namespace std;

class Posts {
private:
    string content;
    string postId;
    string timeOfCreation;
    string creatorUsername;
    bool isReported;
    int reportCount;
    int likeCount;
    Comment* commentList;
    int commentsCount;
    int commentCapacity;

    void expandComments();

public:
    Posts();
    Posts(string authorUsername, string content);
    ~Posts();

    // ─── BASIC GETTERS ───
    string getTimeOfCreation() const;
    string getPostId() const;
    string getContent() const;
    string getCreatorUsername() const;
    int getLikeCount() const;
    int getReportCount() const;
    int getCommentsCount() const;
    bool getIsReported() const;
    bool isValid() const;

    // ─── SETTERS ───
    void setContent(const string& newContent);
    void setTimeOfCreation(const string& time);

    // ─── ID GENERATION ───
    string generatePostId();

    // ─── FILE I/O ───
    void savePostToFile();
    void loadPostFromFile(string ownerUsername, string postId);

    // ─── COMMENTS ───
    void addComment(const string& commentContent, const string& cUsername);
    void deleteComment(int index, const string& rUsername);
    void editComment(int index, const string& newContent, const string& rUsername);
    void deleteCommentAsAdmin(int index);
    Comment* getComments() const;
    void saveCommentsToFile() const;
    void loadCommentsFromFile();

    // ─── LIKES ───
    void likePost(const string& likerUsername);
    void unlikePost(const string& likerUsername);
    bool isLikedBy(const string& username) const;

    // ─── REPORTING ───
    void reportPost(const string& reporterUsername);
    bool hasReportedBy(const string& username) const;
};

#endif