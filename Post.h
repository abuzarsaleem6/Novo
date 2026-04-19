#pragma once
#include<iostream>
#include<string>
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
public:
	Posts();
	Posts( string authorUsername);
	void inputContent(string& content);
	string generatePostId();
	void savePostToFile();
	void likePost();
	void unlikePost();
	void display() const;
	void loadPostFromFile(string ownerUsername, string postId);
	string getPostId() const;
};
