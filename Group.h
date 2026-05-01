#pragma once
#include <string>
#include <fstream>

using namespace std;

class Group {
private:
    string groupName;
    string groupOwner;
    string groupDescription;
    string createdDate;

    // ✅ Dynamic Arrays for manual memory management
    string* memberUsernames;
    int memberCount;

    // ✅ Replaced the Struct with a simple String Array!
    // Each string will hold "sender|timestamp|content"
    string* messageHistory;
    int messageCount;

public:
    // ── Constructors & Destructor ──
    Group();
    Group(string name, string owner);
    ~Group(); // Destructor prevents memory leaks

    // ── Rule of Three (Critical for dynamic arrays) ──
    Group(const Group& other);
    Group& operator=(const Group& other);

    // ── Getters ──
    string getGroupName() const;
    string getOwnerUsername() const;
    string getGroupDescription() const;
    int getMemberCount() const;
    int getMessageCount() const;

    // ── Setters ──
    void setGroupDescription(const string& desc);
    void incrementMessageCount();

    // ── Core Functionality ──
    bool hasMember(const string& username) const;
    void addMember(const string& username);

    // ✅ Takes a single formatted string instead of a struct
    void addMessage(const string& formattedMessage);

    // ── File I/O ──
    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
};