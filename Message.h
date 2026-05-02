#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <fstream>

class Message {
private:
    std::string content;
    std::string senderUsername;
    std::string receiverUsername;
    std::string timestamp;

public:
    Message();
    Message(const std::string& text, const std::string& sender, const std::string& receiver);
    Message(const Message& source) = default;
    Message& operator=(const Message& source) = default;
    ~Message() = default;

    const std::string& getContent() const;
    const std::string& getSenderUsername() const;
    const std::string& getReceiverUsername() const;
    const std::string& getTimestamp() const;
    void setContent(const std::string& text);
    void setSenderUsername(const std::string& sender);
    void setReceiverUsername(const std::string& receiver);

    void generateTimestamp();

    void saveToFile(std::ofstream& out) const;
};

#endif