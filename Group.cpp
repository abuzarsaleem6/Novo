#define _CRT_SECURE_NO_WARNINGS
#include "Group.h"

Group::Group() {
    groupName = new char[8];
    strcpy(groupName, "Unknown");

    memberCapacity = 100;
    memberCount = 0;
    memberUsernames = new std::string[memberCapacity]; 

    messageCapacity = 500;
    messageCount = 0;
    messageHistory = new Message[messageCapacity];
}

Group::Group(const char* name, int initialMemberCapacity, int initialMessageCapacity) {
    if (name != nullptr) {
        groupName = new char[strlen(name) + 1];
        strcpy(groupName, name);
    }
    else {
        groupName = new char[8];
        strcpy(groupName, "Unknown");
    }

    memberCapacity = (initialMemberCapacity > 0) ? initialMemberCapacity : 100;
    memberCount = 0;
    memberUsernames = new std::string[memberCapacity];

    messageCapacity = (initialMessageCapacity > 0) ? initialMessageCapacity : 500;
    messageCount = 0;
    messageHistory = new Message[messageCapacity];
}

Group::~Group() {
    delete[] groupName;
    delete[] memberUsernames;
    delete[] messageHistory;
}

Group::Group(const Group& source) {
    if (source.groupName != nullptr) {
        groupName = new char[strlen(source.groupName) + 1];
        strcpy(groupName, source.groupName);
    }
    else {
        groupName = nullptr;
    }

    memberCapacity = source.memberCapacity;
    memberCount = source.memberCount;
    memberUsernames = new std::string[memberCapacity];
    for (int i = 0; i < memberCount; i++) {
        memberUsernames[i] = source.memberUsernames[i];
    }

    messageCapacity = source.messageCapacity;
    messageCount = source.messageCount;
    messageHistory = new Message[messageCapacity];
    for (int i = 0; i < messageCount; i++) {
        messageHistory[i] = source.messageHistory[i];
    }
}

Group& Group::operator=(const Group& source) {
    if (this == &source) {
        return *this;
    }

    delete[] groupName;
    delete[] memberUsernames;
    delete[] messageHistory;

    if (source.groupName != nullptr) {
        groupName = new char[strlen(source.groupName) + 1];
        strcpy(groupName, source.groupName);
    }
    else {
        groupName = nullptr;
    }

    memberCapacity = source.memberCapacity;
    memberCount = source.memberCount;
    memberUsernames = new std::string[memberCapacity];
    for (int i = 0; i < memberCount; i++) {
        memberUsernames[i] = source.memberUsernames[i];
    }

    messageCapacity = source.messageCapacity;
    messageCount = source.messageCount;
    messageHistory = new Message[messageCapacity];
    for (int i = 0; i < messageCount; i++) {
        messageHistory[i] = source.messageHistory[i];
    }

    return *this;
}

void Group::addMember(std::string username) {
    if (memberCount >= memberCapacity) {
        int newCapacity = memberCapacity * 2;
        std::string* newArray = new std::string[newCapacity];

        for (int i = 0; i < memberCount; i++) {
            newArray[i] = memberUsernames[i];
        }

        delete[] memberUsernames;
        memberUsernames = newArray;
        memberCapacity = newCapacity;
    }

    memberUsernames[memberCount] = username;
    memberCount++;
}

void Group::addMessage(const Message& msg) {
    if (messageCount >= messageCapacity) {
        int newCapacity = messageCapacity * 2;
        Message* newArray = new Message[newCapacity];

        for (int i = 0; i < messageCount; i++) {
            newArray[i] = messageHistory[i];
        }

        delete[] messageHistory;
        messageHistory = newArray;
        messageCapacity = newCapacity;
    }

    messageHistory[messageCount] = msg;
    messageCount++;
}

const char* Group::getGroupName() const { return groupName; }
int Group::getMemberCount() const { return memberCount; }
int Group::getMessageCount() const { return messageCount; }

void Group::saveToFile(const char* filename) const {
    std::ofstream out(filename);

    if (!out.is_open()) return;

    out << groupName << "\n";
    out << memberCount << "\n";

    for (int i = 0; i < memberCount; i++) {
        out << memberUsernames[i] << " ";
    }
    out << "\n";

    out << messageCount << "\n";
    for (int i = 0; i < messageCount; i++) {
        messageHistory[i].saveToFile(out);
    }

    out.close();
}

void Group::loadFromFile(const char* filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;

    char tempName[256];
    in.getline(tempName, 256);

    delete[] groupName;
    groupName = new char[strlen(tempName) + 1];
    strcpy(groupName, tempName);

    in >> memberCount;
    for (int i = 0; i < memberCount; i++) {
        in >> memberUsernames[i];
    }

    in >> messageCount;
    for (int i = 0; i < messageCount; i++) {
        messageHistory[i].loadFromStream(in);
    }

    in.close();
}