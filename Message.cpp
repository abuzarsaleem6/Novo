#define _CRT_SECURE_NO_WARNINGS
#include "Message.h"


Message::Message() {
    content = nullptr; 
    senderID = 0;
    receiverID = 0;
}


Message::Message(const char* text, int sender, int receiver) {
    senderID = sender;
    receiverID = receiver;

    
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
    senderID = source.senderID;
    receiverID = source.receiverID;

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

    senderID = source.senderID;
    receiverID = source.receiverID;

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
int Message::getSenderID() const { return senderID; }
int Message::getReceiverID() const { return receiverID; }


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

void Message::setSenderID(int sender) { senderID = sender; }
void Message::setReceiverID(int receiver) { receiverID = receiver; }

void Message::saveToFile(std::ofstream& out) const {
    
    out << senderID << " " << receiverID << "\n";
    if (content != nullptr) {
        out << content << "\n";
    }
    else {
        out << "NULL\n";
    }
}

void Message::loadFromStream(std::ifstream& in) {
    in >> senderID >> receiverID;
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