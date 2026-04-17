#pragma once
#include<iostream>
#include<string>
using namespace std;
class Posts;
class Notification;
class Comment;
class User;
class Posts {
	string content;
	string postId;
	string timeOfCreation;
	string creatorUsername;
	bool isSaved;
	bool isReported;
	int likeCount;
	Comment* comments;
	int commentsCount;
	User** likedBy;
	int likedByCount;
};
