#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>
#include "Post.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

using namespace std;

// ──────────────────────────────────────────────────────────────────────────────
//  FILE-LOCAL HELPERS  (liked-posts persistence)
// ──────────────────────────────────────────────────────────────────────────────

static QString likedFilePath(const string& username) {
    return QString::fromStdString("data/Posts/" + username + "/" + username + "_liked.txt");
}

static bool hasUserLikedPost(const string& username, const string& postId) {
    QFile file(likedFilePath(username));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        if (in.readLine().trimmed().toStdString() == postId) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

static void addToLikedFile(const string& username, const string& postId) {
    QDir().mkpath(QString::fromStdString("data/Posts/" + username));
    QFile file(likedFilePath(username));
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromStdString(postId) << "\n";
        file.close();
    }
}

static void removeFromLikedFile(const string& username, const string& postId) {
    QFile file(likedFilePath(username));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QStringList remaining;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty() && line.toStdString() != postId)
            remaining.append(line);
    }
    file.close();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString& l : remaining)
            out << l << "\n";
        file.close();
    }
}

// Internal helper — not exposed in header.
static string currentTimeString() {
    return QDateTime::currentDateTime()
        .toString("yyyy-MM-dd HH:mm:ss")
        .toStdString();
}

// ──────────────────────────────────────────────────────────────────────────────
//  CONSTRUCTORS / DESTRUCTOR
// ──────────────────────────────────────────────────────────────────────────────

Posts::Posts(string authorUsername, string content) {
    this->content = content;
    this->postId = generatePostId();
    this->creatorUsername = authorUsername;
    this->timeOfCreation = currentTimeString();
    this->commentsCount = 0;
    this->isReported = false;
    this->reportCount = 0;
    this->likeCount = 0;
}

// Default constructor: used only by loadPostFromFile(); leave timeOfCreation
// empty — it will be overwritten immediately by the file load.
Posts::Posts() {
    this->postId = "";
    this->content = "";
    this->creatorUsername = "";
    this->isReported = false;
    this->reportCount = 0;
    this->timeOfCreation = "";   // FIX: was incorrectly calling currentTimeString()
    this->commentsCount = 0;
    this->likeCount = 0;
}

Posts::~Posts() {}

// ──────────────────────────────────────────────────────────────────────────────
//  ID GENERATION
// ──────────────────────────────────────────────────────────────────────────────

string Posts::generatePostId() {
    // FIX: ensure the data/ directory exists before reading/writing the counter
    QDir().mkpath("data");

    ifstream file("data/post_counter.txt", ios::in);
    int counter = 1000;
    if (file.is_open()) {
        file >> counter;
        file.close();
    }

    // Build "P<counter>" without std::to_string for broader compiler compat
    string num;
    int temp = counter;
    while (temp > 0) {
        num = char('0' + (temp % 10)) + num;
        temp /= 10;
    }
    string id = "P" + num;

    ofstream outFile("data/post_counter.txt", ios::out);
    if (outFile.is_open()) {
        outFile << counter + 1;
        outFile.close();
    }
    return id;
}

// ──────────────────────────────────────────────────────────────────────────────
//  FILE I/O
// ──────────────────────────────────────────────────────────────────────────────

void Posts::savePostToFile() {
    QDir().mkpath(QString::fromStdString("data/Posts/" + this->creatorUsername));

    string path = "data/Posts/" + this->creatorUsername + "/" + this->postId + ".txt";
    ofstream file(path);
    if (!file.is_open()) {
        qDebug() << "Error saving post:" << QString::fromStdString(path);
        return;
    }

    file << "postId|" << this->postId << "\n";
    file << "content|" << this->content << "\n";
    file << "authorUsername|" << this->creatorUsername << "\n";
    file << "timeOfCreation|" << this->timeOfCreation << "\n";
    file << "isReported|" << (this->isReported ? "1" : "0") << "\n";  // FIX: was printing bool (0/1 was compiler-dependent)
    file << "reportCount|" << this->reportCount << "\n";
    file << "commentCount|" << this->commentsCount << "\n";
    file << "likeCount|" << this->likeCount << "\n";
    file.close();
}

void Posts::loadPostFromFile(string ownerUsername, string postId) {
    // Clear everything first
    this->postId = "";
    this->content = "";
    this->creatorUsername = "";
    this->timeOfCreation = "";
    this->isReported = false;
    this->reportCount = 0;
    this->likeCount = 0;
    this->commentsCount = 0;
    commentList.clear();

    string path = "data/Posts/" + ownerUsername + "/" + postId + ".txt";
    ifstream file(path);
    if (!file.is_open()) {
        qDebug() << "Post file not found:" << QString::fromStdString(path);
        return;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        size_t sep = line.find('|');
        if (sep == string::npos || sep == 0) continue;

        string key = line.substr(0, sep);
        string value = line.substr(sep + 1);

        while (!value.empty() && (value.back() == '\r' || value.back() == ' '))
            value.pop_back();

        if (value.empty()) continue;

        try {
            if (key == "postId")         this->postId = value;
            else if (key == "content")         this->content = value;
            else if (key == "authorUsername")  this->creatorUsername = value;
            else if (key == "timeOfCreation")  this->timeOfCreation = value;
            else if (key == "likeCount")       this->likeCount = stoi(value);
            else if (key == "commentCount")    this->commentsCount = stoi(value);
            else if (key == "isReported")      this->isReported = (value == "1");
            else if (key == "reportCount")     this->reportCount = stoi(value);
        }
        catch (const std::exception& e) {
            qDebug() << "Error parsing post field:"
                << QString::fromStdString(key)
                << "Value:" << QString::fromStdString(value)
                << "Error:" << e.what();
        }
    }
    file.close();

    if (isValid())
        loadCommentsFromFile();
}

// ──────────────────────────────────────────────────────────────────────────────
//  GETTERS / SETTERS
// ──────────────────────────────────────────────────────────────────────────────

string Posts::getPostId() const {
    if (postId.empty() || postId.length() > 10000) {
        qDebug() << "WARNING: postId is empty or corrupt.";
        return "";
    }
    return postId;
}

string Posts::getContent()         const { return this->content; }
string Posts::getCreatorUsername() const { return creatorUsername; }
string Posts::getTimeOfCreation()  const { return timeOfCreation; }
int    Posts::getLikeCount()       const { return this->likeCount; }
int    Posts::getReportCount()     const { return this->reportCount; }
int    Posts::getCommentsCount()   const { return this->commentsCount; }
bool   Posts::getIsReported()      const { return this->isReported; }

void Posts::setContent(const string& newContent) { this->content = newContent; }
void Posts::setTimeOfCreation(const string& time) { timeOfCreation = time; }

bool Posts::isValid() const {
    if (postId.empty() || postId.length() > 20) return false;
    if (postId[0] != 'P')                        return false;
    if (creatorUsername.empty() || creatorUsername.length() > 50) return false;
    return true;
}

// ──────────────────────────────────────────────────────────────────────────────
//  LIKES
// ──────────────────────────────────────────────────────────────────────────────

void Posts::likePost(const string& likerUsername) {
    if (likerUsername == this->creatorUsername) {
        qDebug() << "You cannot like your own post.";
        return;
    }

    if (hasUserLikedPost(likerUsername, this->postId)) {
        // Toggle: unlike
        likeCount = max(0, likeCount - 1);
        removeFromLikedFile(likerUsername, this->postId);
    }
    else {
        // Like
        likeCount++;
        addToLikedFile(likerUsername, this->postId);

        QDir().mkpath("data/Notifications");
        string notifPath = "data/Notifications/" + this->creatorUsername + "_notif.txt";
        ofstream notifFile(notifPath, ios::app);
        if (notifFile.is_open()) {
            notifFile << "like|"
                << likerUsername << " liked your post|"
                << currentTimeString() << "|0\n";
            notifFile.close();
        }
    }
    savePostToFile();
}

void Posts::unlikePost(const string& likerUsername) {
    if (likerUsername == this->creatorUsername) {
        qDebug() << "You cannot unlike your own post.";
        return;
    }
    if (hasUserLikedPost(likerUsername, this->postId)) {
        likeCount = max(0, likeCount - 1);
        removeFromLikedFile(likerUsername, this->postId);
        savePostToFile();
    }
}

bool Posts::isLikedBy(const string& username) const {
    return hasUserLikedPost(username, this->postId);
}

// ──────────────────────────────────────────────────────────────────────────────
//  REPORTING
// ──────────────────────────────────────────────────────────────────────────────

void Posts::reportPost(const string& reporterUsername) {
    QString reportedFilePath = QString::fromStdString(
        "data/Posts/" + creatorUsername + "/" + postId + "_reported.txt");

    // Prevent duplicate reports from same user
    QFile checkFile(reportedFilePath);
    if (checkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&checkFile);
        while (!in.atEnd()) {
            if (in.readLine().trimmed().toStdString() == reporterUsername) {
                qDebug() << "User already reported this post.";
                checkFile.close();
                return;
            }
        }
        checkFile.close();
    }

    QFile outFile(reportedFilePath);
    if (outFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&outFile);
        out << QString::fromStdString(reporterUsername) << "\n";
        outFile.close();
    }

    int actualCount = 0;
    QFile countFile(reportedFilePath);
    if (countFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&countFile);
        while (!in.atEnd()) {
            if (!in.readLine().trimmed().isEmpty())
                actualCount++;
        }
        countFile.close();
    }
    this->reportCount = actualCount;
    if (this->reportCount == 3) {
        this->isReported = true;

        QDir().mkpath("data/Admin");
        QFile notifFile("data/Admin/admin_notifications.txt");
        if (notifFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&notifFile);
            QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            out << ts << "|post_reported|"
                << "Post '" << QString::fromStdString(this->postId)
                << "' by @" << QString::fromStdString(this->creatorUsername)
                << " has reached 3 reports and is marked for review.|0\n";
            notifFile.close();
        }
    }
    savePostToFile();
}
bool Posts::hasReportedBy(const string& username) const {
    QString reportedFilePath = QString::fromStdString(
        "data/Posts/" + creatorUsername + "/" + postId + "_reported.txt");
    QFile file(reportedFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        if (in.readLine().trimmed().toStdString() == username) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// ──────────────────────────────────────────────────────────────────────────────
//  COMMENTS
// ──────────────────────────────────────────────────────────────────────────────

void Posts::addComment(const QString& commentContent, const QString& cUsername) {
    if (commentContent.isEmpty()) {
        qDebug() << "Comment content cannot be empty.";
        return;
    }

    // FIX: use a timestamp-based ID instead of sequential count so IDs stay
    // unique even after deletions.
    QString commentId = "C" + QString::number(QDateTime::currentMSecsSinceEpoch());
    Comment c(commentContent, commentId, cUsername);
    commentList.append(c);
    commentsCount++;
    savePostToFile();
    saveCommentsToFile();

    if (this->creatorUsername != cUsername.toStdString()) {
        QDir().mkpath("data/Notifications");
        string notifPath = "data/Notifications/" + this->creatorUsername + "_notif.txt";
        ofstream notifFile(notifPath, ios::app);
        if (notifFile.is_open()) {
            notifFile << "comment|"
                << cUsername.toStdString() << " commented on your post|"
                << currentTimeString() << "|0\n";
            notifFile.close();
        }
    }
}

void Posts::deleteComment(int index, const QString& rUsername) {
    if (index < 0 || index >= commentList.size()) {
        qDebug() << "Invalid comment index.";
        return;
    }
    if (commentList[index].getCreatorUsername() != rUsername) {
        qDebug() << "You can only delete your own comments.";
        return;
    }
    commentList.removeAt(index);
    commentsCount--;
    savePostToFile();
    saveCommentsToFile();
}

void Posts::editComment(int index, const QString& newContent, const QString& rUsername) {
    if (index < 0 || index >= commentList.size()) {
        qDebug() << "Invalid comment index.";
        return;
    }
    if (commentList[index].getCreatorUsername() != rUsername) {
        qDebug() << "You can only edit your own comments.";
        return;
    }
    commentList[index].setContent(newContent);
    savePostToFile();
    saveCommentsToFile();
}

void Posts::deleteCommentAsAdmin(int index) {
    if (index < 0 || index >= commentList.size()) {
        qDebug() << "Invalid comment index.";
        return;
    }
    commentList.removeAt(index);
    commentsCount--;
    savePostToFile();
    saveCommentsToFile();
}

QList<Comment> Posts::getComments() const {
    return commentList;
}

void Posts::saveCommentsToFile() const {
    QString path = QString::fromStdString(
        "data/Posts/" + this->creatorUsername + "/" + this->postId + "_comments.txt");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening comments file for write:" << path;
        return;
    }

    QTextStream out(&file);
    for (const Comment& comment : commentList) {
        try {
            out << "commentId|" << comment.getCommentId() << "\n";
            out << "content|" << comment.getContent() << "\n";
            out << "creatorUsername|" << comment.getCreatorUsername() << "\n";
            out << "timeOfCreation|" << comment.getTimeOfCreation() << "\n";
            out << "isReported|" << (comment.getIsReported() ? "1" : "0") << "\n";
            out << "likeCount|" << comment.getLikeCount() << "\n";
            out << "---\n";
        }
        catch (const std::exception& e) {
            qDebug() << "Error saving comment:" << e.what();
        }
    }
    file.close();
}

void Posts::loadCommentsFromFile() {
    QString path = QString::fromStdString(
        "data/Posts/" + this->creatorUsername + "/" + this->postId + "_comments.txt");
    QFile file(path);

    if (!file.exists()) {
        commentList.clear();
        commentsCount = 0;
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        commentList.clear();
        commentsCount = 0;
        return;
    }

    commentList.clear();

    QString commentId, content, creatorUsername, timeOfCreation;
    bool isReported = false;
    int  likeCount = 0;
    int  fieldCount = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty() || line == "---") {
            if (fieldCount >= 5 && !commentId.isEmpty()) {
                try {
                    Comment c(content, commentId, creatorUsername);
                    c.setTimeOfCreation(timeOfCreation);
                    c.setIsReported(isReported);
                    c.setLikeCount(likeCount);
                    commentList.append(c);
                }
                catch (const std::exception& e) {
                    qDebug() << "Error creating comment:" << e.what();
                }
                commentId.clear(); content.clear();
                creatorUsername.clear(); timeOfCreation.clear();
                isReported = false; likeCount = 0; fieldCount = 0;
            }
            continue;
        }

        int pipeIdx = line.indexOf('|');
        if (pipeIdx < 0) continue;

        QString key = line.left(pipeIdx).trimmed();
        QString value = line.mid(pipeIdx + 1).trimmed();
        if (value.isEmpty()) continue;

        if (key == "commentId") { commentId = value; fieldCount++; }
        else if (key == "content") { content = value; fieldCount++; }
        else if (key == "creatorUsername") { creatorUsername = value; fieldCount++; }
        else if (key == "timeOfCreation") { timeOfCreation = value; fieldCount++; }
        else if (key == "isReported") { isReported = (value == "1" || value == "true"); fieldCount++; }
        else if (key == "likeCount") {
            bool ok = false;
            likeCount = value.toInt(&ok);
            if (!ok) likeCount = 0;
            fieldCount++;
        }
    }

    // Flush last record if file didn't end with "---"
    if (fieldCount >= 5 && !commentId.isEmpty()) {
        Comment c(content, commentId, creatorUsername);
        c.setTimeOfCreation(timeOfCreation);
        c.setIsReported(isReported);
        c.setLikeCount(likeCount);
        commentList.append(c);
    }

    file.close();
    commentsCount = commentList.size();
}