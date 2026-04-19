#include<iostream>
#include<fstream>
#include<string>
#include"User.h"
#include"Post.h"
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

    // ── Step 3: Load posts ──────────────────────────
    for (int i = 0; i < userCount; i++) {
        (*(allUsers + i))->loadAllPosts();
    }

    cout << userCount << " users loaded." << endl;

    // ── Step 4: Signup or Login ──────────────────────
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

    // ── Step 5: Main Menu ────────────────────────────
    char action;
    bool running = true;
    while (running) {
        cout << "\n========================================" << endl;
        cout << "  Logged in as: " << loggedInUser->getUsername() << endl;
        cout << "========================================" << endl;
        cout << "1. Display Profile" << endl;
        cout << "2. Update Bio" << endl;
        cout << "3. Update Password" << endl;
        cout << "4. Follow a User" << endl;
        cout << "5. Unfollow a User" << endl;
        cout << "6. Create a Post" << endl;
        cout << "7. Display My Posts" << endl;
        cout << "8. Like a Post" << endl;
        cout << "9. Unlike a Post" << endl;
        cout << "0. Logout" << endl;
        cout << "Enter choice: ";
        cin >> action;

        switch (action) {

        case '1':
            loggedInUser->displayProfile();
            break;

        case '2':
            loggedInUser->updateBio();
            break;

        case '3':
            loggedInUser->updatePassword();
            break;

        case '4': {
            string targetUsername;
            cout << "Enter username to follow: ";
            cin >> targetUsername;
            User* target = nullptr;
            for (int i = 0; i < userCount; i++) {
                if ((*(allUsers + i))->getUsername() == targetUsername) {
                    target = *(allUsers + i);
                    break;
                }
            }
            if (target == nullptr)
                cout << "User not found." << endl;
            else if (target == loggedInUser)
                cout << "You cannot follow yourself." << endl;
            else
                loggedInUser->followUser(target, allUsers, userCount);
            break;
        }

        case '5': {
            string targetUsername;
            cout << "Enter username to unfollow: ";
            cin >> targetUsername;
            loggedInUser->unfollowUser(targetUsername);
            break;
        }

        case '6': {
            
            loggedInUser->createPost();
            break;
        }

        case '7':
            loggedInUser->displayAllPosts();
            break;

        case '8': {
            string postId;
            cout << "Enter Post ID to like: ";
            cin >> postId;
            Posts* post = loggedInUser->getPostById(postId);
            if (post == nullptr)
                cout << "Post not found." << endl;
            else
                post->likePost();
            break;
        }

        case '9': {
            string postId;
            cout << "Enter Post ID to unlike: ";
            cin >> postId;
            Posts* post = loggedInUser->getPostById(postId);
            if (post == nullptr)
                cout << "Post not found." << endl;
            else
                post->unlikePost();
            break;
        }

        case '0':
            loggedInUser->logOut();
            cout << "Logged out successfully." << endl;
            running = false;
            break;

        default:
            cout << "Invalid choice." << endl;
        }
    }

    // ── Step 6: Cleanup ──────────────────────────────
    for (int i = 0; i < userCount; i++) {
        delete* (allUsers + i);
    }
    delete[] allUsers;

    return 0;
}