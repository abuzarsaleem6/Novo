#pragma once
#pragma once
#ifndef COMMENT_H
#define COMMENT_H

#include<QString>
#include<QDateTime>

class Comment {
private:
	QString content;
	QString commentId;
	QString timeOfCreation;
	QString creatorUsername;
	bool isReported;
	int likeCount;

public:
	Comment(QString content, QString commentId, QString creatorUsername);
	Comment();

	Comment(const Comment& other);

	Comment& operator=(const Comment& other);

	void editText(const QString& newContent);
	QString getContent() const;
	QString getCommentId() const;
	QString getTimeOfCreation() const;
	QString getCreatorUsername() const;
	bool getIsReported() const;
	int getLikeCount() const;

	void setContent(const QString& newContent);

	void setIsReported(bool reported);

	void like();
	void unlike();

	void setTimeOfCreation(const QString& time);
	void setLikeCount(int count);
	QString getDisplayName() const;
};

#endif