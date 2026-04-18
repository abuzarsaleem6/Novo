#include<iostream>
#include<fstream>
#include<string>
#include"User.h"
#include"Post.h"
using namespace std;
int main() {
   
    int userCount = 0;
    ifstream countFile("data/users_list.txt");
    string tempLine;
    while (getline(countFile, tempLine)) {
        userCount++;
    }
    countFile.close();

    User** allUsers = new User * [userCount == 0 ? 1 : userCount];
    loadAllUsers(allUsers, userCount);
    char choice;
    cout << "A. Signup" << endl;
    cout << "B. Login" << endl;
    cin >> choice;
    User* loggedInUser = nullptr;
    switch (choice) {
    case 'A': case 'a':
        loggedInUser = signUp(allUsers, userCount);
        break;

    case 'B': case 'b':
        loggedInUser = findAndLogin(allUsers, userCount);
        if (loggedInUser != nullptr) {
            loggedInUser->displayProfile();
            
        }
        break;
    }

    

    return 0;
}