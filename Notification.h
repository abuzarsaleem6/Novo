#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class Notification {
private:
    string message;
    string type; 
    string timestamp;
    bool isRead;

public:
    Notification();
    Notification(string msg, string t, string time);

    // Core Logic
    void markAsRead();
    void saveNotificationToFile(string username);

    // Qt Getters (Data access for GUI)
    string getMessage() const;
    string getType() const;
    string getTimestamp() const;
    bool getStatus() const;
};

#endif