#pragma once
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

public:
  
    Notification();

    Notification(string msg, string t, string time);
    
    void saveNotificationToFile(string username);

    string getMessage()   const;
    string getType()      const;
    string getTimestamp() const;
};


class NotificationManager {

public:

    static Notification* loadAllNotifications(const string& username, int& outCount);

    static void saveAllNotifications(const string& username, Notification* notifications, int count);

    static void clearAllNotifications(const string& username);


};

#endif