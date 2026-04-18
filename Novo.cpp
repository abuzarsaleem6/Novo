#include<iostream>
#include<fstream>
#include<string>
#include"User.h"
#include"Post.h"
using namespace std;
int main() {
    cout << "Program started" << endl;

    User* allUsers[100];
    int userCount = 0;
    loadAllUsers(allUsers, userCount);
    allUsers[0]->displayProfile();
    (*(allUsers + 0))->displayProfile();
    
    return 0;
}