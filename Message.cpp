#define _CRT_SECURE_NO_WARNINGS
#include "Message.h"
#include <ctime>

using namespace std;


void Message::allocateAndCopy(char*& dest, const char* source) {
    if (source != nullptr) {
        dest = new char[strlen(source) + 1];
        strcpy(dest, source);
    }
    else {
        dest = new char[1];
        dest[0] = '\0';
    }
}


Message::Message() {
    content = nullptr;
    senderUsername = nullptr;
    receiverUsername = nullptr;
    timestamp = nullptr;

    allocateAndCopy(content, "");
    allocateAndCopy(senderUsername, "System");
    allocateAndCopy(receiverUsername, "System");
    allocateAndCopy(timestamp, "");

    isRead = false;
    generateTimestamp();
}

Message::Message(const char* text, const char* sender, const char* receiver) {
    content = nullptr;
    senderUsername = nullptr;
    receiverUsername = nullptr;
    timestamp = nullptr;

    allocateAndCopy(content, text);
    allocateAndCopy(senderUsername, sender);
    allocateAndCopy(receiverUsername, receiver);

    isRead = false;
    generateTimestamp();
}

Message::Message(const Message& source) {
    content = nullptr;
    senderUsername = nullptr;
    receiverUsername = nullptr;
    timestamp = nullptr;

    allocateAndCopy(content, source.content);
    allocateAndCopy(senderUsername, source.senderUsername);
    allocateAndCopy(receiverUsername, source.receiverUsername);
    allocateAndCopy(timestamp, source.timestamp);

    isRead = source.isRead;
}

Message& Message::operator=(const Message& source) {
    if (this != &source) {
        delete[] content;
        delete[] senderUsername;
        delete[] receiverUsername;
        delete[] timestamp;

        allocateAndCopy(content, source.content);
        allocateAndCopy(senderUsername, source.senderUsername);
        allocateAndCopy(receiverUsername, source.receiverUsername);
        allocateAndCopy(timestamp, source.timestamp);

        isRead = source.isRead;
    }
    return *this;
}

Message::~Message() {
    delete[] content;
    delete[] senderUsername;
    delete[] receiverUsername;
    delete[] timestamp;
}


const char* Message::getContent() const { return content; }
const char* Message::getSenderUsername() const { return senderUsername; }
const char* Message::getReceiverUsername() const { return receiverUsername; }
const char* Message::getTimestamp() const { return timestamp; }
bool Message::getIsRead() const { return isRead; }


void Message::setContent(const char* text) {
    delete[] content;
    allocateAndCopy(content, text);
}

void Message::setSenderUsername(const char* sender) {
    delete[] senderUsername;
    allocateAndCopy(senderUsername, sender);
}

void Message::setReceiverUsername(const char* receiver) {
    delete[] receiverUsername;
    allocateAndCopy(receiverUsername, receiver);
}

void Message::markAsRead() {
    isRead = true;
}


void Message::generateTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);

    if (timestamp) {
        delete[] timestamp;
    }
    allocateAndCopy(timestamp, buffer);
}


void Message::saveToFile(ofstream& out) const {
    if (senderUsername && receiverUsername && timestamp && content) {
        out << senderUsername << "|" << receiverUsername << "|" << timestamp << "|" << content << "\n";
    }
}