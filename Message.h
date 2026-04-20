#pragma once
#include <iostream>
#include <cstring>

class Message {
private:
    char* content;      
    int senderID;       
    int receiverID;

public:
    
    Message();                                           
    Message(const char* text, int sender, int receiver);  

    
    ~Message();                                           
    Message(const Message& source);                      

    
    const char* getContent() const;
    int getSenderID() const;
    int getReceiverID() const;

   
    void setContent(const char* text);
    void setSenderID(int sender);
    void setReceiverID(int receiver);


};