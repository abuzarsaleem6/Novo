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
    string getTimeOfCreation() const;
    void inputContent(string& content);
    string generatePostId();
    void savePostToFile();
    void display() const;
    void loadPostFromFile(string ownerUsername, string postId);
    string getPostId() const;
    void addComment(const QString& commentContent, const QString& cUsername);
    void deleteComment(int index, const QString& rUsername);
    void editComment(int index, const QString& newContent, const QString& rUsername);
    QList<Comment> getComments() const;
    void saveCommentsToFile() const;
    void loadCommentsFromFile();
    void likePost(const string& likerUsername);
    void unlikePost(const string& likerUsername);
    string getContent() const;
    void setContent(const string& newContent);
    int getLikeCount() const;
    int getReportCount() const;
     void reportPost();
     void deleteCommentAsAdmin(int index);
     int getCommentsCount() const;
     bool isValid() const;
     void setTimeOfCreation(const string& time);
     bool getIsReported() const;
      string getCreatorUsername() const;
};