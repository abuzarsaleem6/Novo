#include<iostream>
#include<fstream>
#include"Post.h"
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
    this->likeCount = 0;
}
void Posts::inputContent(string& content) {
    bool isValid = false;
    do {
        isValid = true;
        cout << "Enter Your Post Content (You can't write '|' in it): " << endl;
        getline(cin, content);
        for (int i = 0; content[i] != '\0'; i++) {
            if (content[i] == '|') {
                cout << "You can't add '|' Enter again: " << endl;
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

    string folder = "data/Posts/" + this->creatorUsername;
#ifdef _WIN32
    system(("mkdir \"" + folder + "\" 2>nul").c_str());
#else
    system(("mkdir -p \"" + folder + "\"").c_str());
#endif

    string path = folder + "/" + this->postId + ".txt";
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
        else if (key == "timeOfCreation")
            this->timeOfCreation = value;
        else if (key == "likeCount")
            this->likeCount = stoi(value);
        else if (key == "commentCount")
            this->commentsCount = stoi(value);
    }
    file.close();
}
string Posts::getPostId() const {
    return this->postId;
}
Posts::~Posts() {
    
}