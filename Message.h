// Message.h
#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <QString>
#include <QDateTime>
#include <QDebug>

using namespace std;

class Message {
private:
    string content;
    string senderUsername;
    string receiverUsername;
    string timestamp;

public:
    // ─── CONSTRUCTORS & DESTRUCTORS ───
    Message();
    Message(const string& text, const string& sender, const string& receiver);
    Message(const Message& source);
    Message& operator=(const Message& source);
    ~Message();

    // ─── GETTERS ───
    string getContent() const;
    string getSenderUsername() const;
    string getReceiverUsername() const;
    string getTimestamp() const;

    // ─── SETTERS ───
    void setContent(const string& text);
    void setSenderUsername(const string& sender);
    void setReceiverUsername(const string& receiver);
    void setTimestamp(const string& time);

    // ─── AUTO TIMESTAMP ───
    void generateTimestamp();

    // ─── FILE I/O ───
    void saveToFile(ofstream& out) const;
    void loadFromStream(ifstream& in);

    // ─── DISPLAY (Qt) ───
    QString getDisplayText() const;
    bool isValid() const;
};

#endif