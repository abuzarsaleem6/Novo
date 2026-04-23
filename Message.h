#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

class Message {
private:
    char* content;
    std::string senderUsername;
    std::string receiverUsername;

public:
    Message();
    Message(const char* text, std::string sender, std::string receiver);

    ~Message();
    Message(const Message& source);
    Message& operator=(const Message& source);

    const char* getContent() const;
    std::string getSenderUsername() const;
    std::string getReceiverUsername() const;

    void setContent(const char* text);
    void setSenderUsername(std::string sender);
    void setReceiverUsername(std::string receiver);

    void saveToFile(std::ofstream& out) const;
    void loadFromStream(std::ifstream& in);
};