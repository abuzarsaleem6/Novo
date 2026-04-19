#pragma once
#include<iostream>
#include<string>

#include"Comment.h"
#include<QList>
#include<QFile>
#include<QTextStream>

using namespace std;
class Posts;
class Notification;
class Comment;
class User;
class Posts {
private:
	string content;
	string postId;
	string timeOfCreation;
	string creatorUsername;
	bool isReported;
	int reportCount;
	Comment* comments;
	int commentsCount;
	int likeCount;
	QList<Comment> commentList;
public:
	Posts();
	Posts(string content, string authorUsername);
	void inputContent(string& content);
	string generatePostId();
	void savePostToFile();
	void likePost();
	void unlikePost();
	void display() const;
	void loadPostFromFile(string ownerUsername, string postId);

	void addComment(const QString& commentContent, const QString& cUsername);    //cusername is for the commenter

	void deleteComment(int index, const QString& rUsername);      //rusername is for the requester
	void editComment(int index, const QString& newContent, const QString& rUsername);    //rusername is for the requester
	QList<Comment> getComments() const;

	void saveCommentsToFile() const;
	void loadCommentsFromFile();
};