// Message.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Message.h"

using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  CONSTRUCTORS & DESTRUCTORS
// ══════════════════════════════════════════════════════════════════════════════

Message::Message() {
    content = "";
    senderUsername = "System";
    receiverUsername = "System";
    timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    qDebug() << "Message created (empty)";
}

Message::Message(const string& text, const string& sender, const string& receiver) {
    senderUsername = sender;
    receiverUsername = receiver;
    content = text;
    timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();

    qDebug() << "Message created from" << QString::fromStdString(sender)
        << "to" << QString::fromStdString(receiver);
}

Message::Message(const Message& source) {
    content = source.content;
    senderUsername = source.senderUsername;
    receiverUsername = source.receiverUsername;
    timestamp = source.timestamp;
}

Message& Message::operator=(const Message& source) {
    if (this != &source) {
        content = source.content;
        senderUsername = source.senderUsername;
        receiverUsername = source.receiverUsername;
        timestamp = source.timestamp;
    }
    return *this;
}

Message::~Message() {
    // No dynamic memory to clean up (using string instead of char*)
}

// ══════════════════════════════════════════════════════════════════════════════
//  GETTERS
// ══════════════════════════════════════════════════════════════════════════════

string Message::getContent() const {
    return content;
}

string Message::getSenderUsername() const {
    return senderUsername;
}

string Message::getReceiverUsername() const {
    return receiverUsername;
}

string Message::getTimestamp() const {
    return timestamp;
}

// ══════════════════════════════════════════════════════════════════════════════
//  SETTERS
// ══════════════════════════════════════════════════════════════════════════════

void Message::setContent(const string& text) {
    content = text;
}

void Message::setSenderUsername(const string& sender) {
    senderUsername = sender;
}

void Message::setReceiverUsername(const string& receiver) {
    receiverUsername = receiver;
}

void Message::setTimestamp(const string& time) {
    timestamp = time;
}

// ══════════════════════════════════════════════════════════════════════════════
//  AUTO TIMESTAMP
// ══════════════════════════════════════════════════════════════════════════════

void Message::generateTimestamp() {
    timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    qDebug() << "Timestamp generated:" << QString::fromStdString(timestamp);
}

// ══════════════════════════════════════════════════════════════════════════════
//  FILE I/O
// ══════════════════════════════════════════════════════════════════════════════

void Message::saveToFile(ofstream& out) const {
    out << senderUsername << "|" << receiverUsername << "|" << timestamp << "|" << content << "\n";
}

void Message::loadFromStream(ifstream& in) {
    string line;
    if (getline(in, line)) {
        // Parse: sender|receiver|timestamp|content
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            senderUsername = line.substr(0, pos1);
            receiverUsername = line.substr(pos1 + 1, pos2 - pos1 - 1);
            timestamp = line.substr(pos2 + 1, pos3 - pos2 - 1);
            content = line.substr(pos3 + 1);
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════════
//  DISPLAY (Qt)
// ══════════════════════════════════════════════════════════════════════════════

QString Message::getDisplayText() const {
    return QString::fromStdString(senderUsername) + ": " +
        QString::fromStdString(content) + " (" +
        QString::fromStdString(timestamp) + ")";
}

bool Message::isValid() const {
    return !senderUsername.empty() &&
        !receiverUsername.empty() &&
        !content.empty();
}