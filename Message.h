#pragma once
#include <iostream>
#include <cstring>
#include <fstream>

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

    Message& operator=(const Message& source);
    void setContent(const char* text);
    void setSenderID(int sender);
    void setReceiverID(int receiver);

    void saveToFile(std::ofstream& out) const;
    void loadFromStream(std::ifstream& in);

};