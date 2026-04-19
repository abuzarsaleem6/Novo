#include<iostream>
#include<fstream>
#include<string>
#include<QDebug>
#include"User.h"
#include"Post.h"
#include"Comment.h"
#include"SearchEngine.h"
using namespace std;

int main() {
    // ── Step 1: Load all users ──────────────────────
    int userCount = 0;
    ifstream countFile("data/users_list.txt");
    string tempLine;
    while (getline(countFile, tempLine)) userCount++;
    countFile.close();

    User** allUsers = new User * [userCount == 0 ? 1 : userCount];
    loadAllUsers(allUsers, userCount);

    // ── Step 2: Load following/followers ────────────
    for (int i = 0; i < userCount; i++) {
        (*(allUsers + i))->loadFollowing(allUsers, userCount);
        (*(allUsers + i))->loadFollowers(allUsers, userCount);
    }
    cout << userCount << " users loaded." << endl;

    // ── Step 3: Signup or Login ──────────────────────
    User* loggedInUser = nullptr;
    char choice;
    cout << "========================================" << endl;
    cout << "         Welcome to NOVO" << endl;
    cout << "========================================" << endl;
    cout << "A. Signup" << endl;
    cout << "B. Login" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
    case 'A': case 'a':
        loggedInUser = signUp(allUsers, userCount);
        break;
    case 'B': case 'b':
        loggedInUser = findAndLogin(allUsers, userCount);
        break;
    default:
        cout << "Invalid choice." << endl;
        return 0;
    }

    if (loggedInUser == nullptr) {
        cout << "Login/Signup failed." << endl;
        return 0;
    }

    // ── Step 4: Test features ────────────────────────
    char action;
    bool running = true;
    while (running) {
        cout << "\n========================================" << endl;
        cout << "  Logged in as: " << loggedInUser->getUsername() << endl;
        cout << "========================================" << endl;
        cout << "1. Display Profile" << endl;
        cout << "2. Update Bio" << endl;
        cout << "4. Update Password" << endl;
        cout << "5. Follow a User" << endl;
        cout << "6. Unfollow a User" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter choice: ";
        cin >> action;

        switch (action) {

        case '1':
            loggedInUser->displayProfile();
            break;

        case '2':
            loggedInUser->updateBio();
            break;

        case '4':
            loggedInUser->updatePassword();
            break;

        case '5': {
            string targetUsername;
            cout << "Enter username to follow: ";
            cin >> targetUsername;

            // DEBUG — sab usernames print karo
            cout << "Available users:" << endl;
            for (int i = 0; i < userCount; i++) {
                cout << "[" << i << "] " << (*(allUsers + i))->getUsername() << endl;
            }

            User* target = nullptr;
            for (int i = 0; i < userCount; i++) {
                if ((*(allUsers + i))->getUsername() == targetUsername) {
                    target = *(allUsers + i);
                    break;
                }
            }
            if (target == nullptr) {
                cout << "User not found." << endl;
            }
            else if (target == loggedInUser) {
                cout << "You cannot follow yourself." << endl;
            }
            else {
                loggedInUser->followUser(target, allUsers, userCount);
            }
            break;
        }

        case '6': {
            string targetUsername;
            cout << "Enter username to unfollow: ";
            cin >> targetUsername;
            loggedInUser->unfollowUser(targetUsername);
            break;
        }

        case '7':
            loggedInUser->logOut();
            cout << "Logged out successfully." << endl;
            running = false;
            break;

        default:
            cout << "Invalid choice." << endl;
        }
    }

    // ── Step 5: Cleanup ──────────────────────────────
    for (int i = 0; i < userCount; i++) {
        delete* (allUsers + i);
    }
    delete[] allUsers;

    return 0;
}