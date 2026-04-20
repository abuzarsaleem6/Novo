#include "Group.h"


Group::Group() {
    groupName = new char[8];
    strcpy(groupName, "Unknown");

    memberCapacity = 100;
    memberCount = 0;
    memberIDs = new int[memberCapacity];

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
    memberIDs = new int[memberCapacity];

    messageCapacity = (initialMessageCapacity > 0) ? initialMessageCapacity : 500;
    messageCount = 0;
    messageHistory = new Message[messageCapacity];
}



Group::~Group() {
    delete[] groupName;
    delete[] memberIDs;
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
    memberIDs = new int[memberCapacity];
    for (int i = 0; i < memberCount; i++) {
        memberIDs[i] = source.memberIDs[i];
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
    delete[] memberIDs;
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
    memberIDs = new int[memberCapacity];
    for (int i = 0; i < memberCount; i++) {
        memberIDs[i] = source.memberIDs[i];
    }

    messageCapacity = source.messageCapacity;
    messageCount = source.messageCount;
    messageHistory = new Message[messageCapacity];
    for (int i = 0; i < messageCount; i++) {
        messageHistory[i] = source.messageHistory[i];
    }

    return *this;
}



void Group::addMember(int userID) {
  
    if (memberCount >= memberCapacity) {
        int newCapacity = memberCapacity * 2;
        int* newArray = new int[newCapacity];

        for (int i = 0; i < memberCount; i++) {
            newArray[i] = memberIDs[i];
        }

        delete[] memberIDs;
        memberIDs = newArray;
        memberCapacity = newCapacity;
    }

    memberIDs[memberCount] = userID;
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