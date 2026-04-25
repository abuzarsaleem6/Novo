#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<ctime>  
#include"Post.h"
#include<QFile>
#include<QTextStream>
#include<QDateTime>
#include<QDir>
using namespace std;
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
string getCurrentTimeString() {
   
    return QDateTime::currentDateTime()
        .toString("yyyy-MM-dd HH:mm:ss")
        .toStdString();
}

Posts::Posts(string authorUsername, string content) {
    this->content = content;
    this->postId = generatePostId();
    this->creatorUsername = authorUsername;
    this->timeOfCreation = getCurrentTimeString();
    this->commentsCount = 0;
    this->isReported = false;
    this->reportCount = 0;
    this->likeCount = 0;
}
void Posts::inputContent(string& content) {
    bool isValid = false;
    do {
        isValid = true;
        cout << "Enter Your Post Content  (You cant write '|' in it ) " << endl;
        cin.ignore();
        getline(cin, content);
        for (int i = 0; content[i] != '\0'; i++) {
            if (content[i] == '|') {
                cout << "You cant add '|' Enter Again " << endl;
                isValid = false;
                break;
            }
        }
    } while (!isValid);
}
string Posts::generatePostId() {
    ifstream file("data/post_counter.txt", ios::in);
    int  counter = 1000;
    if (file.is_open()) {
        file >> counter;
        file.close();
    }
    string id = "P";
    string num = "";
    int temp = counter;
    while (temp > 0) {
        char c = '0' + (temp % 10);
        num = c + num;
        temp /= 10;
    }
    id += num;
    ofstream outFile("data/post_counter.txt", ios::out);
    if (outFile.is_open()) {
        outFile << counter + 1;
        outFile.close();
    }
    return id;
}
Posts::Posts() {
    this->postId = "";
    this->content = "";
    this->creatorUsername = "";
    this->isReported = false;
    this->reportCount = 0;
    this->timeOfCreation = getCurrentTimeString();
    this->commentsCount = 0;
    this->likeCount = 0;
}
void Posts::savePostToFile() {

    QDir().mkpath(QString::fromStdString("data/Posts/" + this->creatorUsername));

    string path = "data/Posts/" + this->creatorUsername + "/" + this->postId + ".txt";
    ofstream file(path);
    if (file.is_open()) {
        file << "postId|" << this->postId << "\n";
        file << "content|" << this->content << "\n";
        file << "authorUsername|" << this->creatorUsername << "\n";
        file << "timeOfCreation|" << this->timeOfCreation << "\n";
        file << "isReported|" << this->isReported << "\n";
        file << "reportCount|" << this->reportCount << "\n";
        file << "commentCount|" << this->commentsCount << "\n";
        file << "likeCount|" << this->likeCount << "\n";

        file.close();
        cout << "Post saved: " << this->postId << endl;
    }
    else {
        cout << "Error saving post: " << path << endl;
    }
}


void Posts::likePost(const string& likerUsername) {
    if (likerUsername == this->creatorUsername) {
        qDebug() << "You cannot like your own post.";
        return;
    }

    // Toggle logic using liked file
    if (hasUserLikedPost(likerUsername, this->postId)) {
        // Unlike
        likeCount = max(0, likeCount - 1);
        removeFromLikedFile(likerUsername, this->postId);
        qDebug() << likerUsername.c_str() << "unliked post" << this->postId.c_str();
    }
    else {
        // Like
        likeCount++;
        addToLikedFile(likerUsername, this->postId);
        qDebug() << likerUsername.c_str() << "liked post" << this->postId.c_str();

        QDir().mkpath("data/Notifications");
        string notifPath = "data/Notifications/" + this->creatorUsername + "_notif.txt";
        ofstream notifFile(notifPath, ios::app);
        if (notifFile.is_open()) {
            notifFile << "like|"
                << likerUsername << " liked your post|"
                << getCurrentTimeString() << "|0\n";
            notifFile.close();
            qDebug() << "Notification created for" << this->creatorUsername.c_str();
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
        qDebug() << likerUsername.c_str() << "unliked post" << this->postId.c_str();
        savePostToFile();
    }
}
void Posts::display() const {
    cout << "========================================" << endl;
    cout << "Post ID  : " << this->postId << endl;
    cout << "Author   : " << this->creatorUsername << endl;

    cout << "----------------------------------------" << endl;
    cout << this->content << endl;
    cout << "----------------------------------------" << endl;
    cout << "Likes    : " << this->likeCount << endl;
    cout << "Comments : " << this->commentsCount << endl;
    cout << "========================================" << endl;
}
// In Post.cpp
bool Posts::isValid() const {
    // A post ID must start with 'P' and have a sane length
    if (postId.empty() || postId.length() > 20) return false;
    if (postId[0] != 'P') return false;
    if (creatorUsername.empty() || creatorUsername.length() > 50) return false;
    return true;
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
        // Set a default valid state
        this->postId = postId;
        this->creatorUsername = ownerUsername;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines and lines without separator
        if (line.empty()) continue;

        size_t sep = line.find('|');
        if (sep == string::npos || sep == 0) continue;

        string key = line.substr(0, sep);
        string value = line.substr(sep + 1);

        // Skip empty values
        if (value.empty()) continue;

        try {
            if (key == "postId")                this->postId = value;
            else if (key == "content")          this->content = value;
            else if (key == "authorUsername")   this->creatorUsername = value;
            else if (key == "timeOfCreation")   this->timeOfCreation = value;
            else if (key == "likeCount")        this->likeCount = stoi(value);
            else if (key == "commentCount")     this->commentsCount = stoi(value);
            else if (key == "isReported")       this->isReported = (value == "1");
            else if (key == "reportCount")      this->reportCount = stoi(value);
        }
        catch (const std::exception& e) {
            qDebug() << "Error parsing post field:" << QString::fromStdString(key)
                << "Value:" << QString::fromStdString(value)
                << "Error:" << e.what();
        }
    }
    file.close();

    // ✅ Only load comments if post is valid
    if (isValid()) {
        loadCommentsFromFile();
    }
}
int Posts::getCommentsCount() const {
    return this->commentsCount;
}
void Posts::addComment(const QString& commentContent, const QString& cUsername) {
    if (commentContent.isEmpty()) {
        qDebug() << "Comment content cannot be empty.";
        return;
    }

    QString commentId = "C" + QString::number(commentsCount + 1);
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
                << getCurrentTimeString() << "|0\n";
            notifFile.close();
            qDebug() << "Notification created for" << this->creatorUsername.c_str();
        }
    }
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

QList<Comment> Posts::getComments() const {
    return commentList;
}

void Posts::saveCommentsToFile() const {
    if (commentList.isEmpty()) {
        qDebug() << "No comments to save";
        return;  // ✅ Don't create empty files
    }

    QString path = QString::fromStdString("data/Posts/" + this->creatorUsername + "/"
        + this->postId + "_comments.txt");
    QFile file(path);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
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
        qDebug() << "Comments saved. Count:" << commentList.size();
    }
    else {
        qDebug() << "Error opening comments file:" << path;
    }
}

void Posts::loadCommentsFromFile() {
    QString path = QString::fromStdString("data/Posts/" + this->creatorUsername + "/"
        + this->postId + "_comments.txt");
    QFile file(path);

    // ✅ If file doesn't exist, just return
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

    commentList.clear();  // ✅ Clear before loading

    QString commentId, content, creatorUsername, timeOfCreation;
    bool isReported = false;
    int likeCount = 0;
    int fieldCount = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Skip empty lines
        if (line.isEmpty() || line == "---") {
            // Create comment if we have all fields
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

                // Reset
                commentId.clear();
                content.clear();
                creatorUsername.clear();
                timeOfCreation.clear();
                isReported = false;
                likeCount = 0;
                fieldCount = 0;
            }
            continue;
        }

        // Parse key|value
        int pipeIdx = line.indexOf("|");
        if (pipeIdx < 0) continue;

        QString key = line.left(pipeIdx).trimmed();
        QString value = line.mid(pipeIdx + 1).trimmed();

        // ✅ Validate before assigning
        if (value.isEmpty()) continue;

        if (key == "commentId") {
            commentId = value;
            fieldCount++;
        }
        else if (key == "content") {
            content = value;
            fieldCount++;
        }
        else if (key == "creatorUsername") {
            creatorUsername = value;
            fieldCount++;
        }
        else if (key == "timeOfCreation") {
            timeOfCreation = value;
            fieldCount++;
        }
        else if (key == "isReported") {
            isReported = (value == "1" || value == "true");
            fieldCount++;
        }
        else if (key == "likeCount") {
            bool ok = false;
            likeCount = value.toInt(&ok);
            if (!ok) likeCount = 0;
            fieldCount++;
        }
    }

    file.close();
    commentsCount = commentList.size();
    qDebug() << "Comments loaded. Count:" << commentsCount;
}
Posts::~Posts() {

}

string Posts::getPostId() const {
    // Guard against corrupt string before attempting copy
    if (postId.empty()) {
        qDebug() << "WARNING: Getting empty postId!";
        return "";
    }
    // Extra safety: validate the string length is sane before copying
    if (postId.length() > 10000) {
        qDebug() << "ERROR: postId appears corrupt (length=" << postId.length() << ")";
        return "";
    }
    return postId;
}
string Posts::getContent() const {
    return this->content;
}

int Posts::getLikeCount() const {
    return this->likeCount;
}

void Posts::setContent(const string& newContent) {
    this->content = newContent;
}
int Posts::getReportCount() const {
    return this->reportCount;
}
string Posts::getCreatorUsername() const {
    return creatorUsername;
}
bool Posts::getIsReported() const {
    return this->isReported;
}

void Posts::reportPost() {
    this->reportCount++;
    if (this->reportCount >= 3) {
        this->isReported = true;

        QDir().mkpath("data/Admin");
        QString path = "data/Admin/admin_notifications.txt";
        QFile notifFile(path);
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
void Posts::setTimeOfCreation(const string& time) {
    timeOfCreation = time;
}
string Posts::getTimeOfCreation()  const {
    return timeOfCreation;
}