#define _CRT_SECURE_NO_WARNINGS 
#include "Message.h"

using namespace std; 

Message::Message() {
    content = nullptr;
    senderUsername = "System";
    receiverUsername = "System";
}

Message::Message(const char* text, string sender, string receiver) {
    senderUsername = sender;
    receiverUsername = receiver;

    if (text != nullptr) {
        content = new char[strlen(text) + 1];
        strcpy(content, text);
    }
    else {
        content = nullptr;
    }
}

Message::~Message() {
    delete[] content;
}

Message::Message(const Message& source) {
    senderUsername = source.senderUsername;
    receiverUsername = source.receiverUsername;

    if (source.content != nullptr) {
        content = new char[strlen(source.content) + 1];
        strcpy(content, source.content);
    }
    else {
        content = nullptr;
    }
}

Message& Message::operator=(const Message& source) {
    if (this == &source) {
        return *this;
    }

    delete[] content;

    senderUsername = source.senderUsername;
    receiverUsername = source.receiverUsername;

    if (source.content != nullptr) {
        content = new char[strlen(source.content) + 1];
        strcpy(content, source.content);
    }
    else {
        content = nullptr;
    }

    return *this;
}

const char* Message::getContent() const { return content; }
string Message::getSenderUsername() const { return senderUsername; }
string Message::getReceiverUsername() const { return receiverUsername; }

void Message::setContent(const char* text) {
    delete[] content;

    if (text != nullptr) {
        content = new char[strlen(text) + 1];
        strcpy(content, text);
    }
    else {
        content = nullptr;
    }
}

void Message::setSenderUsername(string sender) { senderUsername = sender; }
void Message::setReceiverUsername(string receiver) { receiverUsername = receiver; }

void Message::saveToFile(ofstream& out) const {
    out << senderUsername << " " << receiverUsername << "\n";
    if (content != nullptr) {
        out << content << "\n";
    }
    else {
        out << "NULL\n";
    }
}

void Message::loadFromStream(ifstream& in) {
    in >> senderUsername >> receiverUsername;
    in.ignore();

    char tempBuffer[1024];
    in.getline(tempBuffer, 1024);

    if (strcmp(tempBuffer, "NULL") != 0) {
        setContent(tempBuffer);
    }
    else {
        setContent(nullptr);
    }
}