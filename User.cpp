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
	this->savedPosts = nullptr;
	this->savedPostCount = 0;
}
User::User(string username, string password, string bio) {
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
	this->savedPosts = nullptr;
	this->savedPostCount = 0;
	this->username = username;
	this->password = password;
	this->bio = bio;

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
	userCount = index;
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
User* signUp(User**& allUsers, int& userCount, string username, string password, string bio) {
	for (int i = 0; i < userCount; i++) {
		if (allUsers[i]->getUsername() == username) {
			return nullptr;
		}
	}
	User* newUser = new User(username, password, bio);
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
User* findAndLogin(User**& allUsers, int userCount, string username, string password) {
	for (int i = 0; i < userCount; i++) {
		if (allUsers[i]->getUsername() == username) {
			if (allUsers[i]->login(password)) {
				return allUsers[i];
			}
			else {
				return nullptr; 
			}
		}
	}
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
	if (following != nullptr) {
		delete[] following;
	}
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
	if (followers != nullptr) {
		delete[] followers;
	}
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
void User::createPost(string content) {
	Posts** newPosts = new Posts * [postCount + 1];
	if (posts != nullptr) {
		for (int i = 0; i < postCount; i++) {
			newPosts[i] = posts[i];
		}
		delete[] posts;
	}
	newPosts[postCount] = new Posts(this->username, content);
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
string User::getBio() {
	return this->bio;
}

string User::getPassword() {
	return this->password;
}

void User::setBio(string bio) {
	this->bio = bio;
}

void User::setPassword(string password) {
	this->password = password;
}

int User::getFollowingCount() {
	return this->followingCount;
}

int User::getFollowersCount() {
	return this->followersCount;
}

int User::getPostCount() {
	return this->postCount;
}

Posts* User::getPostByIndex(int index) {
	if (index < 0 || index >= postCount || posts == nullptr)
		return nullptr;
	return posts[index];
}
void User::editPost(string postId) {
	Posts* p = getPostById(postId);
	if (p == nullptr) {
		qDebug() << "Post not found.";
		return;
	}

	qDebug() << "Current content:" << QString::fromStdString(p->getContent());

	QString newContent;
	bool isValid = false;
	do {
		isValid = true;
		cout << "Enter new content (cannot contain '|'): ";
		cin.ignore();
		string temp;
		getline(cin, temp);
		newContent = QString::fromStdString(temp);

		if (newContent.isEmpty()) {
			qDebug() << "Content cannot be empty.";
			isValid = false;
		}
		else {
			for (QChar c : newContent) {
				if (c == '|') {
					qDebug() << "Content cannot contain '|'. Enter again.";
					isValid = false;
					break;
				}
			}
		}
	} while (!isValid);

	p->setContent(newContent.toStdString());
	p->savePostToFile();
	qDebug() << "Post edited successfully.";
}

void User::deletePost(string postId) {
	Posts* p = getPostById(postId);
	if (p == nullptr) {
		qDebug() << "Post not found.";
		return;
	}

	// file delete karo
	QString path = QString::fromStdString("data/Posts/" + this->username + "/" + postId + ".txt");
	QString commentsPath = QString::fromStdString("data/Posts/" + this->username + "/" + postId + "_comments.txt");
	QFile::remove(path);
	QFile::remove(commentsPath);

	// posts_list.txt update karo
	QString listPath = QString::fromStdString("data/Posts/" + this->username + "/posts_list.txt");
	QFile listFile(listPath);
	QString updatedList = "";

	if (listFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&listFile);
		while (!in.atEnd()) {
			QString line = in.readLine();
			if (line != QString::fromStdString(postId)) {
				updatedList += line + "\n";
			}
		}
		listFile.close();
	}

	if (listFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&listFile);
		out << updatedList;
		listFile.close();
	}
	Posts** newPosts = new Posts * [postCount - 1];
	int index = 0;
	for (int i = 0; i < postCount; i++) {
		if (posts[i]->getPostId() != postId) {
			newPosts[index++] = posts[i];
		}
		else {
			delete posts[i];
		}
	}
	delete[] posts;
	posts = newPosts;
	postCount--;
	saveToFile();
	qDebug() << "Post deleted successfully.";
}

void User::reportPost(string postId, User* postOwner) {
	if (postOwner->getUsername() == this->username) {
		qDebug() << "You cannot report your own post";
		return;
	}

	Posts* p = postOwner->getPostById(postId);
	if (p == nullptr) {
		qDebug() << "Post not found.";
		return;
	}

	p->reportPost();
	qDebug() << "Post reported successfully";
	if (p->getReportCount() >= 3) {
		QString adminPath = "data/reported_posts.txt";
		QFile file(adminPath);
		if (file.open(QIODevice::Append | QIODevice::Text)) {
			QTextStream out(&file);
			out << QString::fromStdString(postOwner->getUsername())
				<< "|" << QString::fromStdString(postId) << "\n";
			file.close();
		}
		qDebug() << "Post has been sent to admin for review.";
	}
}

void User::savePost(string postId, User* postOwner) {
	for (int i = 0; i < savedPostCount; i++) {
		if (savedPosts[i]->getPostId() == postId) {
			qDebug() << "Post already saved";
			return;
		}
	}

	Posts* p = postOwner->getPostById(postId);
	if (p == nullptr) {
		qDebug() << "Post not found.";
		return;
	}

	Posts** newSaved = new Posts * [savedPostCount + 1];
	for (int i = 0; i < savedPostCount; i++) {
		newSaved[i] = savedPosts[i];
	}
	newSaved[savedPostCount] = p;
	delete[] savedPosts;
	savedPosts = newSaved;
	savedPostCount++;

	saveSavedPostsToFile();
	qDebug() << "Post saved to favourites";
}

void User::unsavePost(string postId) {
	bool found = false;
	for (int i = 0; i < savedPostCount; i++) {
		if (savedPosts[i]->getPostId() == postId) {
			found = true;
			break;
		}
	}
	if (!found) {
		qDebug() << "Post not in saved list";
		return;
	}

	Posts** newSaved = new Posts * [savedPostCount - 1];
	int index = 0;
	for (int i = 0; i < savedPostCount; i++) {
		if (savedPosts[i]->getPostId() != postId) {
			newSaved[index++] = savedPosts[i];
		}
	}
	delete[] savedPosts;
	savedPosts = newSaved;
	savedPostCount--;

	saveSavedPostsToFile();
	qDebug() << "Post removed from favourites.";
}

void User::displaySavedPosts() {
	if (savedPostCount == 0 || savedPosts == nullptr) {
		qDebug() << "No saved posts";
		return;
	}
	for (int i = 0; i < savedPostCount; i++) {
		savedPosts[i]->display();
	}
}

void User::saveSavedPostsToFile() {
	QString path = QString::fromStdString("data/SavedPosts/" + this->username + "_saved.txt");
	QFile file(path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		for (int i = 0; i < savedPostCount; i++) {
			out << QString::fromStdString(savedPosts[i]->getPostId())
				<< "|" << QString::fromStdString(savedPosts[i]->getCreatorUsername())
				<< "\n";
		}
		file.close();
	}
}

void User::loadSavedPosts(User** allUsers, int userCount) {
	QString path = QString::fromStdString("data/SavedPosts/" + this->username + "_saved.txt");
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		savedPosts = nullptr;
		savedPostCount = 0;
		return;
	}
	QTextStream in(&file);
	int count = 0;
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (!line.isEmpty()) count++;
	}
	file.close();

	if (count == 0) {
		savedPosts = nullptr;
		savedPostCount = 0;
		return;
	}

	savedPosts = new Posts * [count];
	savedPostCount = 0;

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in2(&file);
		while (!in2.atEnd()) {
			QString line = in2.readLine();
			if (line.isEmpty()) continue;

			QStringList parts = line.split("|");
			if (parts.size() != 2) continue;

			string postId = parts[0].toStdString();
			string ownerUsername = parts[1].toStdString();
			for (int i = 0; i < userCount; i++) {
				if (allUsers[i]->getUsername() == ownerUsername) {
					Posts* p = allUsers[i]->getPostById(postId);
					if (p != nullptr) {
						savedPosts[savedPostCount++] = p;
					}
					break;
				}
			}
		}
		file.close();
	}
}
string User::getCreatorUsername() const {
	return this->username;
}