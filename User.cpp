#include<iostream>
#include<fstream>
#include<string>
#include"User.h"
#include"Post.h"
using namespace std;
User::User() {
	this->username = "";
	this->password = "";
	this->bio = "";
	this->isLoggedIn = false;
	this->isReported = false;
	this->isReportedCount = 0;
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
	this->following = nullptr;
	this->followers = nullptr;
	this->posts = nullptr;
	this->notifications = nullptr;
	this->isLoggedIn = true;
	this->isReported = false;
	this->isReportedCount = 0;
	this->notificationCount = 0;
	this->followingCount = 0;
	this->followersCount = 0;
	this->postCount = 0;

	this->username = username;
	this->password = password;
	validateUsername(this->username);
	validatePassWord(this->password);
	InputBio(this->bio);
	validateBio(this->bio);

	this->saveToFile();
	this->addToUserList();
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
	cout << "Enter Bio for Your Profile: ";

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
			cout << "Enter Bio Again: ";
			getline(cin, bio);
		}
	}
}
void User::logOut() {
	this->isLoggedIn = false;

}
void User::reportUser() {

	isReportedCount++;
	saveToFile();
	if (isReportedCount >= 3) {
		isReported = true;
		addToReviewList();
		// Iqra -- Admin class idr se age  handle kre gi 
		cout << this->username << " reported 3 times Sent to Admin for review" << endl;
	}
	else {
		cout << "Reported has been Submitted" << endl;

	}

}

void User::updatePassword() {
	string updatepassword;
	cout << "Enter New Password" << endl;
	cin >> updatepassword;
	validatePassWord(updatepassword);
	cout << "Password Changed Successfully" << endl;
	this->password = updatepassword;
	saveToFile();
}
void User::updateBio() {
	string updateBio;
	cout << "Enter New Bio for Your Profile: ";
	cin.ignore();
	getline(cin, updateBio);
	validateBio(updateBio);
	this->bio = updateBio;
	saveToFile();
	cout << "Bio Updated Successfully" << endl;
}
void User::displayProfile() {
	cout << "---------------------------" << endl;
	cout << "Username : " << this->username << endl;
	cout << "Bio  : " << this->bio << endl;
	cout << "---------------------------" << endl;
}
void User::saveToFile() {
	string path = "data/Users/" + this->username + ".txt";
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

	}
	else {
		cout << "File is not opening cant save profile at the moment" << path << endl;
	}
}
void User::addToUserList() {
	ofstream usersList("data/users_list.txt", ios::app);
	if (usersList.is_open()) {
		usersList << this->username << "\n";
		usersList.close();
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
	int index = 0;
	string username;
	while (getline(userList, username)) {
		allUsers[index] = new User();
		allUsers[index]->loadFromFile(username);
		index++;
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
User* signUp(User**& allUsers, int& userCount) {
	string username, password;
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	for (int i = 0; i < userCount; i++) {
		if (allUsers[i]->getUsername() == username) {
			cout << "Error: Username '" << username << "' is already taken. Please choose another." << endl;
			return nullptr;
		}
	}

	User* newUser = new User(username, password);
	User** newArray = new User * [userCount + 1];
	for (int i = 0; i < userCount; i++) {
		newArray[i] = allUsers[i];
	}
	newArray[userCount] = newUser;
	delete[] allUsers;
	allUsers = newArray;
	userCount++;
	return newUser;
}
bool User::login(string  password) {
	if (password == this->password) {
		this->isLoggedIn = true;
		return true;

	}
	else {
		return false;
	}

}
string User::getUsername() {
	return this->username;
}
User* findAndLogin(User** allUsers, int userCount) {
	string username;
	string password;
	cout << "Enter Username: ";
	cin >> username;
	cout << "Enter Password: ";
	cin >> password;
	for (int i = 0; i < userCount; i++) {
		if ((*(allUsers + i))->getUsername() == username) {
			if ((*(allUsers + i))->login(password)) {
				cout << "Logged in successfully" << endl;
				cout << "  Welcome back, " << username << "!" << endl;
				return *(allUsers + i);
			}
			else {
				cout << "Wrong password " << endl;
				return nullptr;
			}
		}
	}
	cout << "User not found " << endl;
	return nullptr;
}
void User::followUser(User* target, User** allUsers, int userCount) {

	if (isFollowing(target->getUsername())) {
		cout << "Already following" << endl;
		return;
	}
	User** newFollowing = new User * [followingCount + 1];
	if (following != nullptr) {
		for (int i = 0; i < followingCount; i++) {
			*(newFollowing + i) = *(this->following + i);
		}
	}
	*(newFollowing + followingCount) = target;
	delete[] following;
	following = newFollowing;
	followingCount++;
	target->addFollower(this);
	string path = "data/Following/" + this->username + "_following.txt";
	ofstream file(path, ios::app);
	if (file.is_open()) {
		file << target->getUsername() << "\n";
		file.close();
	}
	saveToFile();
}
bool User::isFollowing(string usernamee) {
	if (following == nullptr || followingCount == 0) return false;

	for (int i = 0; i < followingCount; i++) {
		if ((*(following + i))->getUsername() == usernamee) {
			return true;
		}
	}
	return false;
}
void User::addFollower(User* ptr) {
	User** Newfollowers = new User * [followersCount + 1];
	if (followers != nullptr) {
		for (int i = 0; i < followersCount; i++) {
			*(Newfollowers + i) = *(followers + i);
		}
	}
	*(Newfollowers + followersCount) = ptr;
	delete[] followers;
	followers = Newfollowers;
	followersCount++;
	string path = "data/Following/" + this->username + "_followers.txt";
	ofstream file(path, ios::app);
	if (file.is_open()) {
		file << ptr->getUsername() << "\n";
		file.close();
	}
	saveToFile();
}

void User::unfollowUser(string username) {
	if (!isFollowing(username)) {
		cout << "You are not following " << username << endl;
		return;
	}
	User* target = nullptr;
	for (int i = 0; i < followingCount; i++) {
		if ((*(following + i))->getUsername() == username) {
			target = *(following + i);
			break;
		}
	}
	if (followingCount == 1) {
		delete[] following;
		following = nullptr;
		followingCount = 0;
	}
	else {

		User** newFollowing = new User * [followingCount - 1];
		int index = 0;
		for (int i = 0; i < followingCount; i++) {
			if ((*(following + i))->getUsername() != username) {
				*(newFollowing + index) = *(following + i);
				index++;
			}
		}
		delete[] following;
		following = newFollowing;
		newFollowing = nullptr;
		followingCount--;
	}
	string path = "data/Following/" + this->username + "_following.txt";
	ofstream file(path, ios::out);
	if (file.is_open()) {
		for (int i = 0; i < followingCount; i++) {
			file << (*(following + i))->getUsername() << "\n";
		}
		file.close();
	}
	if (target != nullptr) {
		target->removeFollower(this);
	}
	saveToFile();
	cout << "Unfollowed " << username << endl;
}
void User::removeFollower(User* ptr) {
	if (followersCount == 1) {
		delete[] followers;
		followers = nullptr;
		followersCount = 0;
		string path = "data/Following/" + this->username + "_followers.txt";
		ofstream file(path, ios::out);
		file.close();
		saveToFile();
	}
	else {
		User** newFollowers = new User * [followersCount - 1];
		int index = 0;
		for (int i = 0; i < followersCount; i++) {
			if ((*(followers + i))->getUsername() != ptr->getUsername()) {
				*(newFollowers + index) = *(followers + i);
				index++;
			}
		}
		delete[] followers;
		followers = newFollowers;
		newFollowers = NULL;
		followersCount--;
		string path = "data/Following/" + this->username + "_followers.txt";
		ofstream file(path, ios::out);
		if (file.is_open()) {
			for (int i = 0; i < followersCount; i++) {
				file << (*(followers + i))->getUsername() << "\n";
			}
			file.close();
		}
	}
}
void User::loadFollowing(User** allUsers, int userCount) {
	if (followingCount == 0) {
		following = nullptr;
		return;
	}

	string path = "data/Following/" + this->username + "_following.txt";
	ifstream file(path);
	if (!file.is_open()) {
		following = nullptr;
		followingCount = 0;
		return;
	}

	following = new User * [followingCount];
	int loaded = 0;

	string line;
	while (getline(file, line)) {
		for (int i = 0; i < userCount; i++) {
			if ((*(allUsers + i))->getUsername() == line) {
				*(following + loaded) = *(allUsers + i);
				loaded++;
				break;
			}
		}
	}
	file.close();
	followingCount = loaded;
}
void User::loadFollowers(User** allusers, int userCount) {
	if (followersCount == 0) {
		followers = nullptr;
		return;
	}

	string path = "data/Following/" + this->username + "_followers.txt";
	ifstream file(path);
	if (!file.is_open()) {
		followers = nullptr;
		followersCount = 0;
		return;
	}

	followers = new User * [followersCount];
	int loaded = 0;

	string line;
	while (getline(file, line)) {
		for (int i = 0; i < userCount; i++) {
			if ((*(allusers + i))->getUsername() == line) {
				*(followers + loaded) = *(allusers + i);
				loaded++;
				break;
			}
		}
	}
	file.close();
	followersCount = loaded;
}
void User::deleteAccount(User**& allUsers, int& userCount) {

	for (int i = 0; i < followersCount; i++) {
		(*(followers + i))->unfollowUser(this->username);   // remove myself from following of my followers
	}
	for (int i = 0; i < followingCount; i++) {    //removing myself from followers of them
		(*(following + i))->removeFollower(this);
	}
	string followingPath = "data/Following/" + this->username + "_following.txt";
	string followersPath = "data/Following/" + this->username + "_followers.txt";
	remove(followingPath.c_str());
	remove(followersPath.c_str());
	string userPath = "data/Users/" + this->username + ".txt";
	remove(userPath.c_str());
	removeFromUser_List(this->username);
	User** newArray = new User * [userCount - 1];
	int index = 0;
	for (int i = 0; i < userCount; i++) {
		if ((*(allUsers + i))->getUsername() != this->username) {
			*(newArray + index) = *(allUsers + i);
			index++;
		}
	}
	delete[] allUsers;
	allUsers = newArray;
	userCount--;
	cout << username << " account deleted successfully." << endl;
}
void User::addToReviewList() {
	ofstream file("data/users_to_review.txt", ios::app);
	if (file.is_open()) {
		file << this->username << "\n";
		file.close();
	}
}
void User::createPost() {
	cin.ignore();
	Posts** newPosts = new Posts * [postCount + 1];
	if (posts != nullptr) {
		for (int i = 0; i < postCount; i++) {
			newPosts[i] = posts[i];
		}
		delete[] posts;
	}
	newPosts[postCount] = new Posts(this->username);
	posts = newPosts;
	postCount++;
	posts[postCount - 1]->savePostToFile();
	string listPath = "data/Posts/" + this->username + "/posts_list.txt";
	ofstream listFile(listPath, ios::app);
	if (listFile.is_open()) {
		listFile << posts[postCount - 1]->getPostId() << "\n";
		listFile.close();
	}
	saveToFile();
}
void User::displayAllPosts() {
	if (postCount == 0 || posts == nullptr) {
		cout << "No posts yet" << endl;
		return;
	}
	for (int i = 0; i < postCount; i++) {
		posts[i]->display();
	}
}
void User::loadAllPosts() {
	string listPath = "data/Posts/" + this->username + "/posts_list.txt";
	ifstream listFile(listPath);
	if (!listFile.is_open()) {
		posts = nullptr;
		postCount = 0;
		return;
	}
	int count = 0;
	string temp;
	while (getline(listFile, temp)) {
		if (!temp.empty()) count++;
	}
	listFile.close();

	if (count == 0) {
		posts = nullptr;
		postCount = 0;
		return;
	}

	posts = new Posts * [count];
	postCount = 0;

	ifstream listFile2(listPath);
	string postId;
	while (getline(listFile2, postId)) {
		if (postId.empty()) continue;
		posts[postCount] = new Posts();
		posts[postCount]->loadPostFromFile(this->username, postId);
		postCount++;
	}
	listFile2.close();
}
Posts* User::getPostById(string postId) {
	for (int i = 0; i < postCount; i++) {
		if (posts[i]->getPostId() == postId) {
			return posts[i];
		}
	}
	return nullptr;
}
bool User::getIsReported()const {
	return this->isReported;
}
User::~User() {
	for (int i = 0; i < postCount; i++) {
		delete posts[i];
	}
	delete[] posts;
	delete[] following;
	delete[] followers;
}