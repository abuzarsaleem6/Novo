#include<iostream>
#include<fstream>
#include"Post.h"
#include<QFile>
#include<QTextStream>
using namespace std;

Posts::Posts(string authorUsername) {
    inputContent(this->content);
    this->postId = generatePostId();
    this->creatorUsername = authorUsername;
    this->timeOfCreation = "";
    this->comments = nullptr;
    this->commentsCount = 0;
    this->isReported = false;
    this->reportCount = 0;

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
    this->comments = nullptr;
    this->commentsCount = 0;
    this->likeCount = 0;
}
void Posts::savePostToFile() {
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
void Posts::likePost() {
    likeCount++;
    cout << " liked post " << this->postId << endl;
    savePostToFile();
}

void Posts::unlikePost() {
    if (likeCount <= 0) {
        cout << "No likes to remove." << endl;
        return;
    }
    likeCount--;
    cout << " unliked post " << this->postId << endl;
    savePostToFile();
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
void Posts::loadPostFromFile(string ownerUsername, string postId) {
    string path = "data/Posts/" + ownerUsername + "/" + postId + ".txt";
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Post not found: " << postId << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        int separatorIndex = 0;
        while (line[separatorIndex] != '|') {
            separatorIndex++;
        }
        string key = "";
        for (int i = 0; i < separatorIndex; i++) {
            key += line[i];
        }
        string value = "";
        for (int j = separatorIndex + 1; line[j] != '\0'; j++) {
            value += line[j];
        }

        if (key == "postId")
            this->postId = value;
        else if (key == "content")
            this->content = value;
        else if (key == "authorUsername")
            this->creatorUsername = value;
        else if (key == "timestamp")
            this->timeOfCreation = value;
        else if (key == "likeCount")
            this->likeCount = stoi(value);
        else if (key == "commentCount")
            this->commentsCount = stoi(value);
    }
    file.close();
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
}

QList<Comment> Posts::getComments() const {
    return commentList;
}

void Posts::saveCommentsToFile()const {
    QString path = QString::fromStdString("data/Posts/" + this->creatorUsername + "/" + this->postId + "_comments.txt");
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Comment& comment : commentList) {
            out << "commentId|" << comment.getCommentId();
            out << " |content|" << comment.getContent();
            out << " |creatorUsername|" << comment.getCreatorUsername();
            out << " |timeOfCreation|" << comment.getTimeOfCreation();
            out << " |isReported|" << comment.getIsReported();
            out << " |likeCount|" << comment.getLikeCount();
            out << "----------------------------------------\n";
        }
        file.close();
    }
    else {
        qDebug() << "Error saving comments to file: " << path;
    }
}

void Posts::loadCommentsFromFile() {
    QString path = QString::fromStdString("data/Posts/" + this->creatorUsername + "/" + this->postId + "_comments.txt");
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        commentList.clear();
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed().isEmpty()) continue; // Skip empty lines
            QStringList parts = line.split(" |");
            QString commentId, content, creatorUsername, timeOfCreation;
            bool isReported = false;
            int likeCount = 0;
            for (const QString& part : parts) {
                QStringList keyValue = part.split("|");
                if (keyValue.size() != 2) continue;
                QString key = keyValue[0].trimmed();
                QString value = keyValue[1].trimmed();
                if (key == "commentId")
                    commentId = value;
                else if (key == "content")
                    content = value;
                else if (key == "creatorUsername")
                    creatorUsername = value;
                else if (key == "timeOfCreation")
                    timeOfCreation = value;
                else if (key == "isReported")
                    isReported = (value.toLower() == "true");
                else if (key == "likeCount")
                    likeCount = value.toInt();
            }
            Comment c(content, commentId, creatorUsername);
            c.setTimeOfCreation(timeOfCreation);
            c.setIsReported(isReported);
            c.setLikeCount(likeCount);
            commentList.append(c);
        }
        file.close();
    }
    else {
        qDebug() << "Error loading comments from file: " << path;
    }
}
Posts::~Posts() {

}
string Posts::getPostId() const {
    return this->postId;
}