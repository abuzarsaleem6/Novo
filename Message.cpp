#define _CRT_SECURE_NO_WARNINGS
#include "Message.h"
#include <ctime>

using namespace std;

Message::Message()
    : content(""), senderUsername("System"), receiverUsername("System"),
    timestamp("")
{
    generateTimestamp();
}

Message::Message(const string& text, const string& sender, const string& receiver)
    : content(text), senderUsername(sender), receiverUsername(receiver),
    timestamp("")
{
    generateTimestamp();
}

const string& Message::getContent() const { return content; }
const string& Message::getSenderUsername() const { return senderUsername; }
const string& Message::getReceiverUsername() const { return receiverUsername; }
const string& Message::getTimestamp() const { return timestamp; }
void Message::setContent(const string& text) { content = text; }
void Message::setSenderUsername(const string& sender) { senderUsername = sender; }
void Message::setReceiverUsername(const string& receiver) { receiverUsername = receiver; }

void Message::generateTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    timestamp = buffer;
}

void Message::saveToFile(ofstream& out) const {
    out << senderUsername << "|" << receiverUsername << "|"
        << timestamp << "|" << content << "\n";
}