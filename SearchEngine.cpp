// SearchEngine.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "SearchEngine.h"
#include <QDateTime>


using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  PRIVATE HELPER METHODS
// ══════════════════════════════════════════════════════════════════════════════

bool SearchEngine::caseInsensitiveFind(const string& str, const string& substr) {
    // Convert both to lowercase for comparison
    string strLower = str;
    string substrLower = substr;

    transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
    transform(substrLower.begin(), substrLower.end(), substrLower.begin(), ::tolower);

    return strLower.find(substrLower) != string::npos;
}

int SearchEngine::calculateRelevanceScore(const string& str, const string& query) {
    // Score based on how well query matches str
    // Higher score = better match

    int score = 0;
    string strLower = str;
    string queryLower = query;

    transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
    transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    // Exact match
    if (strLower == queryLower) {
        score += 100;
    }
    // Starts with query
    else if (strLower.find(queryLower) == 0) {
        score += 80;
    }
    // Contains query
    else if (strLower.find(queryLower) != string::npos) {
        score += 50;
    }
    // Partial match
    else {
        for (char c : queryLower) {
            if (strLower.find(c) != string::npos) {
                score += 5;
            }
        }
    }

    return score;
}

// ══════════════════════════════════════════════════════════════════════════════
//  CONSTRUCTOR & DESTRUCTOR
// ══════════════════════════════════════════════════════════════════════════════

SearchEngine::SearchEngine() {
    qDebug() << "SearchEngine initialized";
}

SearchEngine::~SearchEngine() {
    allUsers.clear();
    qDebug() << "SearchEngine destroyed";
}

// ══════════════════════════════════════════════════════════════════════════════
//  USER MANAGEMENT
// ══════════════════════════════════════════════════════════════════════════════

void SearchEngine::addUser(User* user) {
    if (!user) {
        qDebug() << "ERROR: Cannot add null user to search index";
        return;
    }

    // Don't add duplicates
    for (User* existingUser : allUsers) {
        if (existingUser->getUsername() == user->getUsername()) {
            qDebug() << "WARNING: User already in index:"
                << QString::fromStdString(user->getUsername());
            return;
        }
    }

    allUsers.append(user);
    qDebug() << "Added user to index:" << QString::fromStdString(user->getUsername());
}

void SearchEngine::clearIndex() {
    allUsers.clear();
    qDebug() << "Search index cleared";
}

void SearchEngine::rebuildIndex(User** allUsersArray, int userCount) {
    // Clear old index
    clearIndex();

    // Rebuild from array
    for (int i = 0; i < userCount; i++) {
        if (allUsersArray[i]) {
            addUser(allUsersArray[i]);
        }
    }

    qDebug() << "Search index rebuilt with" << userCount << "users";
}

// ══════════════════════════════════════════════════════════════════════════════
//  SEARCH METHODS - EXACT & PREFIX MATCHING
// ══════════════════════════════════════════════════════════════════════════════

User* SearchEngine::searchByUsernameExact(const QString& username) const {
    string queryStr = username.toStdString();

    for (User* user : allUsers) {
        if (user->getUsername() == queryStr) {
            qDebug() << "Found exact match:" << username;
            return user;
        }
    }

    qDebug() << "No exact match found for:" << username;
    return nullptr;
}

QList<User*> SearchEngine::searchByUsernamePrefix(const QString& query) const {
    QList<User*> results;
    string queryStr = query.toStdString();

    qDebug() << "Searching for username prefix:" << query;

    for (User* user : allUsers) {
        const string& username = user->getUsername();

        // Check if username starts with query (case-insensitive)
        if (caseInsensitiveFind(username, queryStr) &&
            username.find(queryStr) == 0) {  // Starts with
            results.append(user);
            qDebug() << "  Found:" << QString::fromStdString(username);
        }
    }

    qDebug() << "Prefix search returned" << results.size() << "results";
    return results;
}

QList<User*> SearchEngine::searchByBio(const QString& query) const {
    QList<User*> results;
    string queryStr = query.toStdString();

    qDebug() << "Searching in bio:" << query;

    for (User* user : allUsers) {
        if (user && caseInsensitiveFind(user->getBio(), queryStr)) {
            results.append(user);
            qDebug() << "  Found:" << QString::fromStdString(user->getUsername());
        }
    }

    qDebug() << "Bio search returned" << results.size() << "results";
    return results;
}

// ══════════════════════════════════════════════════════════════════════════════
//  ADVANCED SEARCH - COMBINED & FUZZY
// ══════════════════════════════════════════════════════════════════════════════

QList<User*> SearchEngine::searchCombined(const QString& query) const {
    // Search both username and bio, return combined results

    QList<User*> results;
    string queryStr = query.toStdString();

    qDebug() << "Combined search for:" << query;

    for (User* user : allUsers) {
        if (!user) continue;

        bool usernameMatch = caseInsensitiveFind(user->getUsername(), queryStr);
        bool bioMatch = caseInsensitiveFind(user->getBio(), queryStr);

        if (usernameMatch || bioMatch) {
            if (!results.contains(user)) {
                results.append(user);
                qDebug() << "  Found:" << QString::fromStdString(user->getUsername());
            }
        }
    }

    qDebug() << "Combined search returned" << results.size() << "results";
    return results;
}

QList<User*> SearchEngine::getTopResults(const QString& query, int limit) const {
    // Calculate relevance scores and return top N results

    QList<pair<User*, int>> scored;  // User + relevance score
    string queryStr = query.toStdString();

    qDebug() << "Getting top" << limit << "results for:" << query;

    for (User* user : allUsers) {
        if (!user) continue;

        int usernameScore = calculateRelevanceScore(user->getUsername(), queryStr);
        int bioScore = calculateRelevanceScore(user->getBio(), queryStr);
        int totalScore = usernameScore + bioScore;

        if (totalScore > 0) {
            scored.append(qMakePair(user, totalScore));
        }
    }

    // Sort by score (descending)
    sort(scored.begin(), scored.end(),
        [](const pair<User*, int>& a, const pair<User*, int>& b) {
            return a.second > b.second;
        });

    // Return top N
    QList<User*> results;
    int resultLimit = (limit < scored.size()) ? limit : scored.size();
    for (int i = 0; i < resultLimit; i++) {
        results.append(scored[i].first);
        qDebug() << "  " << QString::fromStdString(scored[i].first->getUsername())
            << "- Score:" << scored[i].second;
    }

    return results;
}

QList<User*> SearchEngine::filterResults(const QList<User*>& results,
    bool excludeBanned,
    bool excludeReported) const {
    QList<User*> filtered;

    for (User* user : results) {
        if (!user) continue;

        // Skip banned users if requested
        if (excludeBanned && user->getIsBanned()) {
            qDebug() << "  Filtered out banned user:"
                << QString::fromStdString(user->getUsername());
            continue;
        }

        // Skip reported users if requested
        if (excludeReported && user->getIsReported()) {
            qDebug() << "  Filtered out reported user:"
                << QString::fromStdString(user->getUsername());
            continue;
        }

        filtered.append(user);
    }

    qDebug() << "Filtered results:" << filtered.size() << "remaining";
    return filtered;
}

// ══════════════════════════════════════════════════════════════════════════════
//  GETTERS
// ══════════════════════════════════════════════════════════════════════════════

QList<User*> SearchEngine::getAllUsers() const {
    return allUsers;
}

int SearchEngine::getIndexSize() const {
    return allUsers.size();
}

// ══════════════════════════════════════════════════════════════════════════════
//  DISPLAY
// ══════════════════════════════════════════════════════════════════════════════

void SearchEngine::displayResults(const QString& username) const {
    User* user = searchByUsernameExact(username);
    if (user) {
        displayUserDetails(user);
    }
    else {
        qDebug() << "No user found!";
    }
}

void SearchEngine::displayUserDetails(User* user) const {
    if (!user) {
        qDebug() << "ERROR: Null user pointer";
        return;
    }

    qDebug() << "╔════ USER PROFILE ════╗";
    qDebug() << "Username:" << QString::fromStdString(user->getUsername());
    qDebug() << "Bio:" << QString::fromStdString(user->getBio());
    qDebug() << "Posts:" << user->getPostCount();
    qDebug() << "Followers:" << user->getFollowersCount();
    qDebug() << "Following:" << user->getFollowingCount();

    if (user->getIsBanned()) {
        qDebug() << "Status: 🚫 BANNED";
    }
    else if (user->getIsReported()) {
        qDebug() << "Status: ⚠️  REPORTED";
    }
    else {
        qDebug() << "Status: ✓ Active";
    }

    qDebug() << "╚═══════════════════════╝";
}