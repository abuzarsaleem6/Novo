#define _CRT_SECURE_NO_WARNINGS
#include "Group.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  GROUP CLASS IMPLEMENTATION
// ══════════════════════════════════════════════════════════════════════════════

Group::Group() {
    groupName = "";
    groupOwner = "";
    groupDescription = "";
    createdDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();

    memberCount = 0;
    memberUsernames = nullptr;

    messageCount = 0;
    messageHistory = nullptr;
}

Group::Group(string name, string owner) {
    groupName = name;
    groupOwner = owner;
    groupDescription = "";
    createdDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();

    memberCount = 1;
    memberUsernames = new string[1];
    memberUsernames[0] = owner;

    messageCount = 0;
    messageHistory = nullptr;
}

// ── DESTRUCTOR ──
Group::~Group() {
    if (memberUsernames != nullptr) delete[] memberUsernames;
    if (messageHistory != nullptr) delete[] messageHistory;
}

// ── COPY CONSTRUCTOR (Rule of Three) ──
Group::Group(const Group& other) {
    groupName = other.groupName;
    groupOwner = other.groupOwner;
    groupDescription = other.groupDescription;
    createdDate = other.createdDate;

    memberCount = other.memberCount;
    if (memberCount > 0) {
        memberUsernames = new string[memberCount];
        for (int i = 0; i < memberCount; ++i) {
            memberUsernames[i] = other.memberUsernames[i];
        }
    }
    else {
        memberUsernames = nullptr;
    }

    messageCount = other.messageCount;
    if (messageCount > 0) {
        messageHistory = new string[messageCount];
        for (int i = 0; i < messageCount; ++i) {
            messageHistory[i] = other.messageHistory[i];
        }
    }
    else {
        messageHistory = nullptr;
    }
}

// ── ASSIGNMENT OPERATOR (Rule of Three) ──
Group& Group::operator=(const Group& other) {
    if (this == &other) return *this;

    if (memberUsernames != nullptr) delete[] memberUsernames;
    if (messageHistory != nullptr) delete[] messageHistory;

    groupName = other.groupName;
    groupOwner = other.groupOwner;
    groupDescription = other.groupDescription;
    createdDate = other.createdDate;

    memberCount = other.memberCount;
    if (memberCount > 0) {
        memberUsernames = new string[memberCount];
        for (int i = 0; i < memberCount; ++i) {
            memberUsernames[i] = other.memberUsernames[i];
        }
    }
    else {
        memberUsernames = nullptr;
    }

    messageCount = other.messageCount;
    if (messageCount > 0) {
        messageHistory = new string[messageCount];
        for (int i = 0; i < messageCount; ++i) {
            messageHistory[i] = other.messageHistory[i];
        }
    }
    else {
        messageHistory = nullptr;
    }

    return *this;
}

// ── GETTERS ──
string Group::getGroupName() const { return groupName; }
string Group::getOwnerUsername() const { return groupOwner; }
string Group::getGroupDescription() const { return groupDescription; }
int Group::getMemberCount() const { return memberCount; }
int Group::getMessageCount() const { return messageCount; }

// ── SETTERS ──
void Group::setGroupDescription(const string& desc) { groupDescription = desc; }
void Group::incrementMessageCount() { messageCount++; }

// ── LOGIC ──
bool Group::hasMember(const string& username) const {
    for (int i = 0; i < memberCount; ++i) {
        if (memberUsernames[i] == username) return true;
    }
    return false;
}

void Group::addMember(const string& username) {
    if (hasMember(username)) return;

    string* newArray = new string[memberCount + 1];
    for (int i = 0; i < memberCount; ++i) {
        newArray[i] = memberUsernames[i];
    }
    newArray[memberCount] = username;

    if (memberUsernames != nullptr) delete[] memberUsernames;
    memberUsernames = newArray;
    memberCount++;
}

void Group::addMessage(const string& formattedMessage) {
    string* newArray = new string[messageCount + 1];
    for (int i = 0; i < messageCount; ++i) {
        newArray[i] = messageHistory[i];
    }
    newArray[messageCount] = formattedMessage;

    if (messageHistory != nullptr) delete[] messageHistory;
    messageHistory = newArray;
    messageCount++;
}

// ── FILE I/O ──
void Group::saveToFile(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        qDebug() << "ERROR: Could not open file for saving group:" << QString::fromStdString(filename);
        return;
    }

    // Header
    out << "groupName|" << groupName << "\n"
        << "groupOwner|" << groupOwner << "\n"
        << "groupDescription|" << groupDescription << "\n"
        << "createdDate|" << createdDate << "\n";

    // Members
    out << "memberCount|" << memberCount << "\n";
    for (int i = 0; i < memberCount; i++) {
        out << "member|" << memberUsernames[i] << "\n";
    }

    // Messages
    out << "messageCount|" << messageCount << "\n";
    for (int i = 0; i < messageCount; i++) {
        out << messageHistory[i] << "\n"; // Just write the pre-formatted string!
    }

    out.close();
    qDebug() << "Group saved to file:" << QString::fromStdString(filename);
}

void Group::loadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return;

    if (memberUsernames != nullptr) delete[] memberUsernames;
    memberUsernames = nullptr;
    memberCount = 0;

    if (messageHistory != nullptr) delete[] messageHistory;
    messageHistory = nullptr;
    messageCount = 0;

    string line;
    while (getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        size_t sep1 = line.find('|');
        if (sep1 == string::npos) continue;

        string key = line.substr(0, sep1);
        string val = line.substr(sep1 + 1);

        if (key == "groupName") groupName = val;
        else if (key == "groupOwner") groupOwner = val;
        else if (key == "groupDescription") groupDescription = val;
        else if (key == "createdDate") createdDate = val;
        else if (key == "memberCount") { /* Auto-calculated */ }
        else if (key == "messageCount") { /* Auto-calculated */ }
        else if (key == "member") {
            addMember(val);
        }
        else {
            // ✅ If it's none of the above headers, the line IS a message!
            // It will naturally be in "sender|timestamp|content" format.
            addMessage(line);
        }
    }
    in.close();
}