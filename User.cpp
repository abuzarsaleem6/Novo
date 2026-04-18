#include<iostream>
#include<fstream>
#include<string>
#include"User.h"
using namespace std;
User::User() {
	this->username = "";
	this->password = "";
	this->bio = "";
	this->isLoggedIn = false;
	this->isReported = false;
	this->isReportedCount = 0;
	this->isSuspended = false;
	this->notificationCount = 0;
	this->followingCount = 0;
	this->followersCount = 0;
	this->postCount = 0;
	this->posts = nullptr;
	this->notifications = nullptr;
	this->followers = nullptr;
	this->following = nullptr;
}
User::User(string username, string password) {

	this->username = username;
	this->password = password;
	validateUsername(this->username);
	validatePassWord(this->password);
	InputBio(this->bio);
	validateBio(this->bio);
	this->isLoggedIn = true;
	this->isReported = false;
	this->isReportedCount = 0;
	this->isSuspended = false;
	this->notificationCount = 0;
	this->followingCount = 0;
	this->followersCount = 0;
	this->followersCount = 0;
	this->postCount = 0;

	this->saveToFile();

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
void User::InputPassWord(string& password) {
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
				if (username[i] == '|' || username[i] == ' ') {
					cout << "Username cannot contain '|' or spaces" << endl;
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
				if (password[i] == '|' || password[i] == ' ') {
					cout << "password cannot contain '|' or spaces" << endl;
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
	cout << "---Bio Rules---" << endl;
	cout << "[1] Must contain less than or equal to 100 characters" << endl;
	cout << "---------------" << endl;
	cout << "Enter Bio for Your Profile Again " << endl;
	cin.ignore();
	getline(cin, bio);
}
void User::validateBio(string& bio) {
	bool isBioValid = false;
	while (!isBioValid) {
		isBioValid = true;
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
		if (!isBioValid) {
			InputBio(bio);
		}
	}
}
void User::logOut() {
	this->isLoggedIn = false;

}
void User::reportUser() {
	isReported = true;
	isReportedCount++;

	if (isReportedCount >= 3) {
		string path = "data/Users/" + this->username + ".txt";
		remove(path.c_str());
		removeFromUser_List(this->username);
		cout << this->username << " Removed and deleted from Novo because of not following policy of Novo" << endl;
		isSuspended = true;
	}

}
void User::updateUsername() {
	string updateusername;
	cout << "Enter New Username" << endl;
	cin >> updateusername;
	validateUsername(updateusername);
	cout << "Username changed successfullt from [" << this->username << "] To [" << updateusername << "]" << endl;
	this->username = updateusername;

}
void User::updatePassword() {
	string updatepassword;
	cout << "Enter New Password" << endl;
	cin >> updatepassword;
	validatePassWord(updatepassword);
	cout << "Password Changed Successfully" << endl;
	this->password = updatepassword;
}
void User::updateBio() {
	string updateBio;
	cout << "Enter New Bio for Your Profile" << endl;
	cin >> updateBio;
	validateBio(updateBio);
	cout << "Bio Updated Successfully" << endl;
	this->bio = updateBio;
}
void User::displayProfile() {
	cout << "---------------------------" << endl;
	cout << "Username : " << this->username << endl;
	cout << "Bio  : " << this->bio << endl;
	cout << "---------------------------" << endl;
}
void User::saveToFile() {
	string path = "data/Users/" + this->username + ".txt";
	cout << "Trying to save at: " << path << endl;
	ofstream file(path);
	if (file.is_open()) {
		file << "username|" << this->username << "\n";
		file << "password|" << this->password << "\n";
		file << "bio|" << this->bio << "\n";
		file << "isReportedCount|" << this->isReportedCount << "\n";
		file << "notificationCount|" << this->notificationCount << "\n";
		file << "followingCount|" << this->followingCount << "\n";
		file << "followersCount|" << this->followersCount << "\n";
		file << "postCount|" << this->postCount << "\n";
		file.close();
		cout << "Profile saved successfully" << endl;

		ofstream usersList("data/users_list.txt", ios::app);
		if (usersList.is_open()) {
			usersList << this->username << "\n";

			usersList.close();
		}

	}
	else {
		cout << "File is not opening cant save profile at the moment" <<path << endl;
	}
}
void User::loadFromFile(string username) {
	string path = "data/Users/" + username + ".txt";
	ifstream file(path, ios::in);
	if (!file.is_open()) {
		cout << "User not found" << endl;
		return;
	}
	
	string line;
	while (getline(file, line)) {
		int separatorIndex = 0;
		while (line[separatorIndex] != '|') {  // get separater index 
			separatorIndex++;
		}
		string key = "";
		for (int i = 0; i < separatorIndex; i++) {  // extracting key 
			key += line[i];
		}
		string value = "";
		int k = 0;
		for (int j = separatorIndex + 1; line[j] != '\0'; j++) {  // extracting Value
			value += line[j];
			k++;
		}


		if (key == "username")
			this->username = value;
		else if (key == "password")
			this->password = value;
		else if (key == "bio")
			this->bio = value;
		else if (key == "isReportedCount")
			this->isReportedCount = stoi(value);
		else if (key == "notificationCount")
			this->notificationCount = stoi(value);
		else if (key == "followingCount")
			this->followingCount = stoi(value);
		else if (key == "followersCount")
			this->followersCount = stoi(value);
		else if (key == "postCount")
			this->postCount = stoi(value);
	}
	file.close();
}
void loadAllUsers(User** allUsers, int& userCount) {

	ifstream userList("data/users_list.txt");
	if (!userList.is_open()) {
		cout << "No Record Found" << endl;
		return;
	}
	string username;
	while (getline(userList, username)) {
		allUsers[userCount] = new User();
		allUsers[userCount]->loadFromFile(username);
		userCount++;
	}
	userList.close();
}
void User::removeFromUser_List(string username) {
	string line;
	string updatedContent = "";
	ifstream file("data/users_list.txt", ios::in);

	while (getline(file, line)) {
		if (line != username) {
			updatedContent = updatedContent + line + "\n";
		}
	}
	file.close();
	ofstream updatefile("data/users_list.txt", ios::out);
	updatefile << updatedContent;
	updatefile.close();

}