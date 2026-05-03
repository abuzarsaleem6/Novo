#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Comment.h"
using namespace std;
class Posts {
private:
    string  content;
    string  postId;
    string timeOfCreation;
    string creatorUsername;
    bool isReported;
    int  reportCount;
    int likeCount;
    Comment* commentList;
    int commentsCount;
    int commentCapacity;
    void expandComments();

public:

	//Constructors and assignment operator and Destructor
    Posts();
    Posts(string authorUsername, string content);
    ~Posts();

	// Helper functions
    string getTimeOfCreation() const;
    string getPostId() const;
    string getContent() const;
    string getCreatorUsername() const;
    int getLikeCount() const;
    int getReportCount() const;
    int getCommentsCount() const;
    bool getIsReported() const;
    bool isValid() const;

	// setters
    void setContent(const string& newContent);
    void setTimeOfCreation(const string& time);

   
    string generatePostId();

	//Helper functions for file handling
    void savePostToFile();
    void loadPostFromFile(string ownerUsername, string postId);

	// Helper functions for comment management
    void addComment(const string& commentContent, const string& cUsername);
    void deleteComment(int index, const string& rUsername);
    void editComment(int index, const string& newContent, const string& rUsername);
    void deleteCommentAsAdmin(int index);
    Comment* getComments() const;
    void saveCommentsToFile() const;
    void loadCommentsFromFile();

	//Helper functions for like and report management
    void likePost(const string& likerUsername);
    void unlikePost(const string& likerUsername);
    bool isLikedBy(const string& username) const;

    void reportPost(const string& reporterUsername);
    bool hasReportedBy(const string& username) const;
};
