#pragma once
#include <iostream>
#include <cstring>
#include "Message.h" 

class Group {
private:
    char* groupName;        

    int* memberIDs;           
    int memberCount;
    int memberCapacity;

    Message* messageHistory;  
    int messageCount;
    int messageCapacity;

public:
    
    Group();
    Group(const char* name, int initialMemberCapacity = 100, int initialMessageCapacity = 500);

    
    ~Group();
    Group(const Group& source);
    Group& operator=(const Group& source);

    
    void addMember(int userID);
    void addMessage(const Message& msg);

    
    const char* getGroupName() const;
    int getMemberCount() const;
    int getMessageCount() const;
};
