#pragma once
#include <QString>
#include <QList>
#include <ctime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <string>
#include <iostream>
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
    QList<QString> likedByUsers;
    QList<Comment> commentList;
    int commentsCount;

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
    void addComment(const QString& commentContent, const QString& cUsername);
    void deleteComment(int index, const QString& rUsername);
    void editComment(int index, const QString& newContent, const QString& rUsername);
    void deleteCommentAsAdmin(int index);
    QList<Comment> getComments() const;
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