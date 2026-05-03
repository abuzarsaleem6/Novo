#pragma once
#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <ctime>

using namespace std;

class Comment {
private:
    string content;
    string commentId;
    string timeOfCreation;
    string creatorUsername;
    bool isReported;
    int likeCount;

public:

	// Constructors and assignment operator
    Comment(string content, string commentId, string creatorUsername);
    Comment();
    Comment(const Comment& other);
    Comment& operator=(const Comment& other);

	//Helper function 
    void editText(const string& newContent);
    string getContent() const;
    string getCommentId() const;
    string getTimeOfCreation() const;
    string getCreatorUsername() const;
    bool getIsReported() const;
    int getLikeCount() const;

    void setContent(const string& newContent);
    void setIsReported(bool reported);
    void like();
    void unlike();
    void setTimeOfCreation(const string& time);
    void setLikeCount(int count);
    string getDisplayName() const;
};

#endif