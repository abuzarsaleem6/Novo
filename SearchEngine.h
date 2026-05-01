// SearchEngine.h
#pragma once
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QString>
#include <QList>
#include <QDebug>
#include <string>
#include <algorithm>
#include <cctype>
#include "User.h"

using namespace std;

class SearchEngine {
private:
    QList<User*> allUsers;

    // ─── PRIVATE HELPER METHODS ───
    // Case-insensitive substring search
    static bool caseInsensitiveFind(const string& str, const string& substr);

    // Calculate relevance score for fuzzy matching
    static int calculateRelevanceScore(const string& str, const string& query);

public:
    // ─── CONSTRUCTORS & DESTRUCTORS ───
    SearchEngine();
    ~SearchEngine();

    // ─── USER MANAGEMENT ───
    // Add a user to the search index
    void addUser(User* user);

    // Clear all users from the search index
    void clearIndex();

    // Rebuild index (call after users are added/removed)
    void rebuildIndex(User** allUsers, int userCount);

    // ─── SEARCH METHODS ───

    // Search by exact username (case-sensitive)
    User* searchByUsernameExact(const QString& username) const;

    // Search by username prefix (case-insensitive)
    QList<User*> searchByUsernamePrefix(const QString& query) const;

    // Search by bio/display content (fuzzy match)
    QList<User*> searchByBio(const QString& query) const;

    // Combined search (username + bio)
    QList<User*> searchCombined(const QString& query) const;

    // ─── ADVANCED SEARCH ───

    // Get all users (for browsing)
    QList<User*> getAllUsers() const;

    // Get top N search results by relevance
    QList<User*> getTopResults(const QString& query, int limit = 10) const;

    // Filter results by criteria
    QList<User*> filterResults(const QList<User*>& results,
        bool excludeBanned = true,
        bool excludeReported = false) const;

    // ─── GETTERS ───
    int getIndexSize() const;

    // ─── DISPLAY ───
    void displayResults(const QString& username) const;
    void displayUserDetails(User* user) const;
};

#endif