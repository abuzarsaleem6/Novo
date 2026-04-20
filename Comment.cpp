#include"Comment.h"

#include<QString>
#include<QDateTime>

Comment::Comment(QString content, QString commentId, QString creatorUsername)
	: content(content), commentId(commentId), creatorUsername(creatorUsername), isReported(false), likeCount(0) {
	timeOfCreation = QDateTime::currentDateTime().toString();
}

Comment::Comment() : content(""), commentId(""), creatorUsername(""), isReported(false), likeCount(0) {
	timeOfCreation = QDateTime::currentDateTime().toString();
}

Comment::Comment(const Comment& other)
	: content(other.content),
	commentId(other.commentId),
	timeOfCreation(other.timeOfCreation),
	creatorUsername(other.creatorUsername),
	isReported(other.isReported),
	likeCount(other.likeCount) {
}

Comment& Comment::operator=(const Comment& other) {
	if (this != &other) {
		content = other.content;
		commentId = other.commentId;
		timeOfCreation = other.timeOfCreation;
		creatorUsername = other.creatorUsername;
		isReported = other.isReported;
		likeCount = other.likeCount;
	}
	return *this;
}

void Comment::editText(const QString& newContent) {
	content = newContent;
}

QString Comment::getContent() const {
	return content;
}

QString Comment::getCommentId() const {
	return commentId;
}

QString Comment::getTimeOfCreation() const {
	return timeOfCreation;
}

QString Comment::getCreatorUsername() const {
	return creatorUsername;
}

bool Comment::getIsReported() const {
	return isReported;
}

int Comment::getLikeCount() const {
	return likeCount;
}

void Comment::setContent(const QString& newContent) {
	content = newContent;
}

void Comment::setIsReported(bool reported) {
	isReported = reported;
}

void Comment::like() {
	likeCount++;
}

void Comment::unlike() {
	if (likeCount > 0) {
		likeCount--;
	}
}

void Comment::setTimeOfCreation(const QString& time) {
	timeOfCreation = time;
}

void Comment::setLikeCount(int count) {
	likeCount = count;
}

QString Comment::getDisplayName() const {
	if(creatorUsername.isEmpty()) {
		return "Unknown User";
	}
	return creatorUsername;
}