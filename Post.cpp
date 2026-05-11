#define _CRT_SECURE_NO_WARNINGS
#include "Post.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <sys/stat.h>

using namespace std;

// Helper Functions

static void mkdirRecursive(const string& path) {
#ifdef _WIN32
    system(("mkdir \"" + path + "\" 2>nul").c_str());
//#else
//    system(("mkdir -p \"" + path + "\"").c_str());
#endif
}

static string currentTimeString() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

static string likedFilePath(const string& username) {
    return "data/Posts/" + username + "/" + username + "_liked.txt";
}

static bool hasUserLikedPost(const string& username, const string& postId) {
    ifstream file(likedFilePath(username));
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line == postId) { file.close(); return true; }
    }
    file.close();
    return false;
}

static void addToLikedFile(const string& username, const string& postId) {
    mkdirRecursive("data/Posts/" + username);
    ofstream file(likedFilePath(username), ios::app);
    if (file.is_open()) {
        file << postId << "\n";
        file.close();
    }
}

static void removeFromLikedFile(const string& username, const string& postId) {
    string path = likedFilePath(username);

    
    ifstream file(path);
    if (!file.is_open()) return;

    int count = 0;
    string line;
    while (getline(file, line))
        if (!line.empty() && line != postId) count++;
    file.close();

    
    string* lines = new string[count];
    int idx = 0;

    ifstream file2(path);
    while (getline(file2, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty() && line != postId)
            lines[idx++] = line;
    }
    file2.close();

    ofstream out(path);
    for (int i = 0; i < count; i++)
        out << lines[i] << "\n";
    out.close();

    delete[] lines;
}

// Constructors / Destructor
Posts::Posts(string authorUsername, string content) {
    this->content = content;
    this->postId = generatePostId();
    this->creatorUsername = authorUsername;
    this->timeOfCreation = currentTimeString();
    this->commentsCount = 0;
    this->commentCapacity = 10;
    this->commentList = new Comment[commentCapacity];
    this->isReported = false;
    this->reportCount = 0;
    this->likeCount = 0;
}

Posts::Posts() {
    this->postId = "";
    this->content = "";
    this->creatorUsername = "";
    this->isReported = false;
    this->reportCount = 0;
    this->timeOfCreation = "";
    this->commentsCount = 0;
    this->commentCapacity = 10;
    this->commentList = new Comment[commentCapacity];
    this->likeCount = 0;
}

Posts::~Posts() {
    delete[] commentList;
}

void Posts::expandComments() {
    commentCapacity *= 2;
    Comment* newList = new Comment[commentCapacity];
    for (int i = 0; i < commentsCount; i++)
        newList[i] = commentList[i];
    delete[] commentList;
    commentList = newList;
}

// Id Generation

string Posts::generatePostId() {
    mkdirRecursive("data");
    ifstream file("data/post_counter.txt");
    int counter = 1000;
    if (file.is_open()) { file >> counter; file.close(); }

    string num;
    int temp = counter;
    while (temp > 0) {
        num = char('0' + (temp % 10)) + num;
        temp /= 10;
    }
    string id = "P" + num;

    ofstream outFile("data/post_counter.txt");
    if (outFile.is_open()) { outFile << counter + 1; outFile.close(); }
    return id;
}

// File I/O

void Posts::savePostToFile() {
    mkdirRecursive("data/Posts/" + this->creatorUsername);
    string path = "data/Posts/" + this->creatorUsername + "/" + this->postId + ".txt";
    ofstream file(path);
    if (!file.is_open()) { cerr << "Error saving post: " << path << "\n"; return; }

    file << "postId|" << this->postId << "\n";
    file << "content|" << this->content << "\n";
    file << "authorUsername|" << this->creatorUsername << "\n";
    file << "timeOfCreation|" << this->timeOfCreation << "\n";
    file << "isReported|" << (this->isReported ? "1" : "0") << "\n";
    file << "reportCount|" << this->reportCount << "\n";
    file << "commentCount|" << this->commentsCount << "\n";
    file << "likeCount|" << this->likeCount << "\n";
    file.close();
}

void Posts::loadPostFromFile(string ownerUsername, string postId) {
    this->postId = "";
    this->content = "";
    this->creatorUsername = "";
    this->timeOfCreation = "";
    this->isReported = false;
    this->reportCount = 0;
    this->likeCount = 0;
    this->commentsCount = 0;
    delete[] commentList;
    commentList = new Comment[commentCapacity];

    string path = "data/Posts/" + ownerUsername + "/" + postId + ".txt";
    ifstream file(path);
    if (!file.is_open()) { cerr << "Post file not found: " << path << "\n"; return; }

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
        catch (...) {
            cerr << "Error parsing post field: " << key << "\n";
        }
    }
    file.close();
    if (isValid()) loadCommentsFromFile();
}

//Getters / Setters

string Posts::getPostId() const {
    if (postId.empty() || postId.length() > 10000) return "";
    return postId;
}
string Posts::getContent() const {
    return content;
}
string Posts::getCreatorUsername() const { 
    return creatorUsername; 
}
string Posts::getTimeOfCreation() const { 
    return timeOfCreation; 
}
int    Posts::getLikeCount() const {
    return likeCount; 
}
int    Posts::getReportCount() const {
    return reportCount;
}
int    Posts::getCommentsCount() const {
    return commentsCount; 
}
bool   Posts::getIsReported() const {
    return isReported;
}

void Posts::setContent(const string& newContent) {
    content = newContent; 
}
void Posts::setTimeOfCreation(const string& time) {
    timeOfCreation = time;
}

bool Posts::isValid() const {
    if (postId.empty() || postId.length() > 20) return false;
    if (postId[0] != 'P') return false;
    if (creatorUsername.empty() || creatorUsername.length() > 50) return false;
    return true;
}

// Likes

void Posts::likePost(const string& likerUsername) {
    if (likerUsername == creatorUsername) { cout << "Cannot like own post\n";
    return; 
    }
    if (hasUserLikedPost(likerUsername, postId)) {
        likeCount = likeCount > 0 ? likeCount - 1 : 0;
        removeFromLikedFile(likerUsername, postId);
    }
    else {
        likeCount++;
        addToLikedFile(likerUsername, postId);
        mkdirRecursive("data/Notifications");
        string notifPath = "data/Notifications/" + creatorUsername + "_notif.txt";
        ofstream notifFile(notifPath, ios::app);
        if (notifFile.is_open()) {
            notifFile << "like|" << likerUsername << " liked your post|"
                << currentTimeString() << "|0\n";
            notifFile.close();
        }
    }
    savePostToFile();
}

void Posts::unlikePost(const string& likerUsername) {
    if (likerUsername == creatorUsername) return;
    if (hasUserLikedPost(likerUsername, postId)) {
        likeCount = likeCount > 0 ? likeCount - 1 : 0;
        removeFromLikedFile(likerUsername, postId);
        savePostToFile();
    }
}

bool Posts::isLikedBy(const string& username) const {
    return hasUserLikedPost(username, postId);
}

//Reporting
void Posts::reportPost(const string& reporterUsername) {
    string reportedFilePath = "data/Posts/" + creatorUsername + "/" + postId + "_reported.txt";

    // Check for duplicate
    ifstream checkFile(reportedFilePath);
    if (checkFile.is_open()) {
        string line;
        while (getline(checkFile, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line == reporterUsername) {
                checkFile.close();
                return;
            }
        }
        checkFile.close();
    }

    ofstream outFile(reportedFilePath, ios::app);
    if (outFile.is_open()) { outFile << reporterUsername << "\n";
    outFile.close();
    }

    
    reportCount++;

    if (reportCount == 3) {
        isReported = true;
        mkdirRecursive("data/Admin");
        ofstream notifFile("data/Admin/admin_notifications.txt", ios::app);
        if (notifFile.is_open()) {
            notifFile << currentTimeString() << "|post_reported|Post '"
                << postId << "' by @" << creatorUsername
                << " has reached 3 reports and is marked for review \n";
            notifFile.close();
        }
    }
    savePostToFile();
}

bool Posts::hasReportedBy(const string& username) const {
    string reportedFilePath = "data/Posts/" + creatorUsername + "/" + postId + "_reported.txt";
    ifstream file(reportedFilePath);
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line == username) { file.close(); return true; }
    }
    file.close();
    return false;
}

// Comments
void Posts::addComment(const string& commentContent, const string& cUsername) {
    if (commentContent.empty()) { cerr << "Comment content cannot be empty.\n"; return; }

   
    
    mkdirRecursive("data");
    ifstream cntFile("data/comment_counter.txt");
    int cCounter = 1000;
    if (cntFile.is_open()) { 
        cntFile >> cCounter; 
    cntFile.close();
    }

    string num;
    int temp = cCounter;
    while (temp > 0) {
        num = char('0' + (temp % 10)) + num;
        temp /= 10;
    }
    string id = "C" + num;

    ofstream cntOut("data/comment_counter.txt");
    if (cntOut.is_open()) { cntOut << cCounter + 1; cntOut.close(); }

    Comment c(commentContent, id, cUsername);
    if (commentsCount >= commentCapacity) expandComments();
    commentList[commentsCount++] = c;
    savePostToFile();
    saveCommentsToFile();

    if (creatorUsername != cUsername) {
        mkdirRecursive("data/Notifications");
        string notifPath = "data/Notifications/" + creatorUsername + "_notif.txt";
        ofstream notifFile(notifPath, ios::app);
        if (notifFile.is_open()) {
            notifFile << "comment|" << cUsername << " commented on your post|"
                << currentTimeString() << "|0\n";
            notifFile.close();
        }
    }
}

void Posts::deleteComment(int index, const string& rUsername) {
    if (index < 0 || index >= commentsCount) { cerr << "Invalid comment index.\n"; return; }
    if (commentList[index].getCreatorUsername() != rUsername) {
        cerr << "You can only delete your own comments.\n"; return;
    }
    for (int i = index; i < commentsCount - 1; i++)
        commentList[i] = commentList[i + 1];
    commentsCount--;
    savePostToFile();
    saveCommentsToFile();
}

void Posts::editComment(int index, const string& newContent, const string& rUsername) {
    if (index < 0 || index >= commentsCount) { cerr << "Invalid comment index.\n"; return; }
    if (commentList[index].getCreatorUsername() != rUsername) {
        cerr << "You can only edit your own comments.\n"; return;
    }
    commentList[index].setContent(newContent);
    savePostToFile();
    saveCommentsToFile();
}

void Posts::deleteCommentAsAdmin(int index) {
    if (index < 0 || index >= commentsCount) {
        cerr << "Invalid comment index.\n"; 
        return;
    }
    for (int i = index; i < commentsCount - 1; i++)
        commentList[i] = commentList[i + 1];
    commentsCount--;
    savePostToFile();
    saveCommentsToFile();
}

Comment* Posts::getComments() const {
    return commentList;
}

void Posts::saveCommentsToFile() const {
    string path = "data/Posts/" + creatorUsername + "/" + postId + "_comments.txt";
    ofstream file(path);
    if (!file.is_open()) { cerr << "Error opening comments file for write.\n"; return; }
    for (int i = 0; i < commentsCount; i++) {
        const Comment& c = commentList[i];
        file << "commentId|" << c.getCommentId() << "\n";
        file << "content|" << c.getContent() << "\n";
        file << "creatorUsername|" << c.getCreatorUsername() << "\n";
        file << "timeOfCreation|" << c.getTimeOfCreation() << "\n";
        file << "isReported|" << (c.getIsReported() ? "1" : "0") << "\n";
        file << "likeCount|" << c.getLikeCount() << "\n";
        file << "---\n";
    }
    file.close();
}

void Posts::loadCommentsFromFile() {
    string path = "data/Posts/" + creatorUsername + "/" + postId + "_comments.txt";
    ifstream file(path);
    delete[] commentList;
    commentList = new Comment[commentCapacity];
    commentsCount = 0;
    if (!file.is_open()) return;

    string commentId, content, creatorUser, timeOfCreation;
    bool isReported = false;
    int likeCount = 0;
    int fieldCount = 0;

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.empty() || line == "---") {
            if (fieldCount >= 5 && !commentId.empty()) {
                Comment c(content, commentId, creatorUser);
                c.setTimeOfCreation(timeOfCreation);
                c.setIsReported(isReported);
                c.setLikeCount(likeCount);
                if (commentsCount >= commentCapacity) expandComments();
                commentList[commentsCount++] = c;
                commentId.clear();
                content.clear();
                creatorUser.clear();
                timeOfCreation.clear();
                isReported = false; 
                    likeCount = 0;
                fieldCount = 0;
            }
            continue;
        }

        size_t pipeIdx = line.find('|');
        if (pipeIdx == string::npos) continue;
        string key = line.substr(0, pipeIdx);
        string value = line.substr(pipeIdx + 1);
        if (value.empty()) continue;

        if (key == "commentId") { 
            commentId = value;
            fieldCount++;
        }
        else if (key == "content") {
            content = value; 
            fieldCount++;
        }
        else if (key == "creatorUsername") {
            creatorUser = value; fieldCount++;
        }
        else if (key == "timeOfCreation") {
            timeOfCreation = value; fieldCount++; 
        }
        else if (key == "isReported") { 
            isReported = (value == "1"); fieldCount++; 
        }
        else if (key == "likeCount") { 
            try { likeCount = stoi(value);
            } catch (...) {
                likeCount = 0;
            }
            fieldCount++;
        }
    }

    // Flush last record
    if (fieldCount >= 5 && !commentId.empty()) {
        Comment c(content, commentId, creatorUser);
        c.setTimeOfCreation(timeOfCreation);
        c.setIsReported(isReported);
        c.setLikeCount(likeCount);
        if (commentsCount >= commentCapacity) expandComments();
        commentList[commentsCount++] = c;
    }

    file.close();
}