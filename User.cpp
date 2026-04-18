#include<iostream>
#include<string>
#include"User.h"
using namespace std;
User::User(string username, string password) {
	this->username = username;
	this->password = password;
	validateUsername(this->username);
	validatePassWord(this->password);
	InputBio(this->bio);
}
void User::InputUserName(string& username) {
	cout << "---UserName Rules---" << endl;
	cout << "[1] Cant have '|' in it " << endl;
	cout << "[2] must be 6 or greater than 6 characters long  " << endl;
	cout << "[3] must not exceed  16 characters  " << endl;
	cout << "--------------------" << endl;
	cout << "Enter Username Again" << endl;
	cin >> username;
}
void User::InputPassWord (string& password) {
	cout << "---Password  Rules---" << endl;
	cout << "[1] Cant have '|' in it " << endl;
	cout << "[2] must be 8 or greater than 8 characters long  " << endl;
	cout << "[3] must not exceed  16 characters  " << endl;
	cout << "--------------------" << endl;
	cout << "Enter password Again" << endl;
	cin >> password;
}
void User::validateUsername(string& username) {
	bool isValid = false;
	while (!isValid) {
		isValid = true;
		if (username.length() < 6) {
			cout << "Username can not be shorter then 6 characters" << endl;
			isValid = false;
		}
		else if (username.length() > 16) {
			cout << "Username can not be longer then 16 characters" << endl;
			isValid = false;
		}
		else {
			for (int i = 0; username[i] != '\0'; i++) {
				if (username[i] == '|') {
					cout << "Username Can not contain '|' in it " << endl;
					isValid = false;
					
					break;
				}
			}
			
		}
		if (!isValid) {
			InputUserName(username);
		}
	}

}
void User::validatePassWord(string& password) {
	bool isValid = false;
	while (!isValid) {
		isValid = true;
		if (password.length() < 8) {
			cout << "password can not be shorter then 8 characters" << endl;
			isValid = false;
		}
		else if (password.length() > 16) {
			cout << "password can not be longer then 16 characters" << endl;
			isValid = false;
		}
		else {
			for (int i = 0; password[i] != '\0'; i++) {
				if (password[i] == '|') {
					cout << "Password  Can not contain '|'  in it " << endl;
					isValid = false;
					break;
				}
			}

		}
		if (!isValid) {
			InputPassWord(password);
		}
	}
}
void User::InputBio(string& bio) {
	bool isBioValid = false;
	cin.ignore();
	while (!isBioValid) {
		isBioValid = true;
		cout << "Enter Bio for Your Profile" << endl;
		getline(cin, bio);
		
		if (bio.length() > 100) {
			cout << "Bio length must be less than 100" << endl;
			isBioValid = false;
		}
		else {
			for (int i = 0; bio[i] != '\0'; i++) {
				if (bio[i] == '|') {
					cout << "Bio Can not contain '|'  in it" << endl;
					isBioValid = false;
					break;
				}
			}

		}
	}



}