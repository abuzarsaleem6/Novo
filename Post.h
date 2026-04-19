#pragma once
#include <QString>
#include <QList>
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
    QList<Comment> commentList;
    int commentsCount;

public:
    Posts();
    Posts( string postContent);
    ~Posts();
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
};