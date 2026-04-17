#include<iostream>
#include<string>
using namespace std;
class Posts;
class Notification;
class Comment;
class User {
private:
	string username;
	string password;
	string email;
	string bio;
	bool isLoggedIn;
	bool isReported;
	Notification* notifications;
	int notificationCount;
	User** following;
	int  followingCount ;
	User** followers;
	int followersCount;
	Posts* posts;
	int postCount;
};
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
class Notification{

};
class Comment {

};
int main() {

}