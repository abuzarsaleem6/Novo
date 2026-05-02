#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <cstring>
#include <fstream>

class Message {
private:
    char* content;
    char* senderUsername;
    char* receiverUsername;
    char* timestamp;
    bool isRead;


    void allocateAndCopy(char*& dest, const char* source);

public:

    Message();
    Message(const char* text, const char* sender, const char* receiver);
    Message(const Message& source);
    Message& operator=(const Message& source);
    ~Message();


    const char* getContent() const;
    const char* getSenderUsername() const;
    const char* getReceiverUsername() const;
    const char* getTimestamp() const;
    bool getIsRead() const;


    void setContent(const char* text);
    void setSenderUsername(const char* sender);
    void setReceiverUsername(const char* receiver);
    void markAsRead();


    void generateTimestamp();


    void saveToFile(std::ofstream& out) const;



};

#endif