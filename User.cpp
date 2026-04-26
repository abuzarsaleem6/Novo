#include<iostream>
#include<fstream>
#include <QDateTime>
#include <QDir>
#include<string>
#include"User.h"
#include"Post.h"
#include "Notification.h"
using namespace std;
User::User() {
	username = "";
	password = "";
	bio = "";
	isLoggedIn = false;
	isReported = false;
	isBanned = false;
	isReportedCount = 0;
	notificationCount = 0;
	followingCount = 0;
	followersCount = 0;
	postCount = 0;
	savedPostCount = 0;
	posts = nullptr;
	
	followers = nullptr;
	following = nullptr;
	savedPosts = nullptr;
}
User::User(string username, string password, string bio) {
	this->following = nullptr;
	this->followers = nullptr;
	this->posts = nullptr;
	
	this->isLoggedIn = true;
	this->isReported = false;
	this->isBanned = false;
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
User::User(const User& o) {
	username = o.username;
	password = o.password;
	bio = o.bio;
	isLoggedIn = o.isLoggedIn;
	isReported = o.isReported;
	isBanned = o.isBanned;
	isReportedCount = o.isReportedCount;
	notificationCount = o.notificationCount;
	followingCount = o.followingCount;
	followersCount = o.followersCount;
	postCount = o.postCount;
	savedPostCount = o.savedPostCount;
	 

	following = nullptr;
	if (o.followingCount > 0 && o.following) {
		following = new User * [o.followingCount];
		for (int i = 0; i < o.followingCount; i++)
			following[i] = o.following[i];
	}

	followers = nullptr;
	if (o.followersCount > 0 && o.followers) {
		followers = new User * [o.followersCount];
		for (int i = 0; i < o.followersCount; i++)
			followers[i] = o.followers[i];
	}
	posts = nullptr;
	if (o.postCount > 0 && o.posts) {
		posts = new Posts * [o.postCount];
		for (int i = 0; i < o.postCount; i++)
			posts[i] = new Posts(*o.posts[i]);
	}

	savedPosts = nullptr;
	if (o.savedPostCount > 0 && o.savedPosts) {
		savedPosts = new Posts * [o.savedPostCount];
		for (int i = 0; i < o.savedPostCount; i++)
			savedPosts[i] = o.savedPosts[i];
	}
}
User& User::operator=(const User& o) {
	if (this == &o) return *this;

	for (int i = 0; i < postCount; i++) delete posts[i];
	delete[] posts;
	delete[] following;
	delete[] followers;
	delete[] savedPosts;

	username = o.username;
	password = o.password;
	bio = o.bio;
	isLoggedIn = o.isLoggedIn;
	isReported = o.isReported;
	isBanned = o.isBanned;
	isReportedCount = o.isReportedCount;
	notificationCount = o.notificationCount;
	followingCount = o.followingCount;
	followersCount = o.followersCount;
	postCount = o.postCount;
	savedPostCount = o.savedPostCount;
	

	following = nullptr;
	if (o.followingCount > 0 && o.following) {
		following = new User * [o.followingCount];
		for (int i = 0; i < o.followingCount; i++)
			following[i] = o.following[i];
	}

	followers = nullptr;
	if (o.followersCount > 0 && o.followers) {
		followers = new User * [o.followersCount];
		for (int i = 0; i < o.followersCount; i++)
			followers[i] = o.followers[i];
	}

	posts = nullptr;
	if (o.postCount > 0 && o.posts) {
		posts = new Posts * [o.postCount];
		for (int i = 0; i < o.postCount; i++)
			posts[i] = new Posts(*o.posts[i]);
	}

	savedPosts = nullptr;
	if (o.savedPostCount > 0 && o.savedPosts) {
		savedPosts = new Posts * [o.savedPostCount];
		for (int i = 0; i < o.savedPostCount; i++)
			savedPosts[i] = o.savedPosts[i];
	}

	return *this;
}
QString User::validateUsername(const string& username) {
	if (username.length() < 6)
		return "Username must be at least 6 characters ";
	if (username.length() > 16)
		return "Username cannot exceed 16 characters ";
	for (char c : username) {
		if (c == '|' || c == ' ')
			return "Username cannot contain '|' or spaces ";
	}
	return "";   
}
QString User::validatePassword(const string& password) {
	if (password.length() < 8)
		return "Password must be at least 8 characters.";
	if (password.length() > 16)
		return "Password cannot exceed 16 characters.";
	for (char c : password) {
		if (c == '|' || c == ' ')
			return "Password cannot contain '|' or spaces.";
	}
	return "";
}
QString User::validateBio(const string& bio) {
	if (bio.length() > 100)
		return "Bio cannot exceed 100 characters ";
	for (char c : bio) {
		if (c == '|')
			return "Bio cannot contain '|' ";
	}
	return "";
}
void User::logOut() {
	this->isLoggedIn = false;

	// Free posts
	for (int i = 0; i < postCount; i++) {
		delete posts[i];
		posts[i] = nullptr;
	}
	delete[] posts;
	posts = nullptr;
	postCount = 0;

	// Free following (non-owned pointers, just free the array)
	delete[] following;
	following = nullptr;
	followingCount = 0;

	// Free followers (non-owned pointers, just free the array)
	delete[] followers;
	followers = nullptr;
	followersCount = 0;

	// Free savedPosts (non-owned pointers, just free the array)
	delete[] savedPosts;
	savedPosts = nullptr;
	savedPostCount = 0;
}
void User::reportPost(string postId, User* postOwner) {
	if (!postOwner) return;
	postOwner->loadAllPosts();

	for (int i = 0; i < postOwner->getPostCount(); ++i) {
		Posts* p = postOwner->getPostByIndex(i);
		if (!p || !p->isValid()) continue;
		if (p->getPostId() == postId) {
			p->reportPost(this->username);   
			return;
		}
	}
}
QString User::validatePostContent(const string& content) {
	if (content.empty())
		return "Post content cannot be empty.";
	for (char c : content) {
		if (c == '|')
			return "Post content cannot contain '|'.";
	}
	return "";
}
bool User::updatePassword(const string& newPassword, QString& errorOut) {
	errorOut = validatePassword(newPassword);
	if (!errorOut.isEmpty()) return false;
	password = newPassword;
	saveToFile();
	return true;
}
bool User::updateBio(const string& newBio, QString& errorOut) {
	errorOut = validateBio(newBio);
	if (!errorOut.isEmpty()) return false;
	bio = newBio;
	saveToFile();
	return true;
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
		file << "isBanned|" << (this->isBanned ? 1 : 0) << "\n";
		file << "isReported|" << (this->isReported ? 1 : 0) << "\n";
		file << "isReportedCount|" << this->isReportedCount << "\n";
		file << "notificationCount|" << this->notificationCount << "\n";
		file << "followingCount|" << this->followingCount << "\n";
		file << "followersCount|" << this->followersCount << "\n";
		file << "postCount|" << this->postCount << "\n";
		file << "savedPostCount|" << this->savedPostCount << "\n";
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
		cout << "User not found: " << username << endl;
		return;
	}

	string line;
	while (getline(file, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		size_t sep = line.find('|');
		if (sep == string::npos || sep == 0) continue;

		string key = line.substr(0, sep);
		string value = line.substr(sep + 1);

		// Trim trailing spaces and \r from value to prevent desync bugs
		while (!value.empty() && (value.back() == '\r' || value.back() == ' ')) {
			value.pop_back();
		}

		if (key == "username")               this->username = value;
		else if (key == "password")          this->password = value;
		else if (key == "bio")               this->bio = value;
		else if (key == "isBanned")          this->isBanned = (value == "1");
		else if (key == "isReported")        this->isReported = (value == "1");
		else if (key == "isReportedCount")   this->isReportedCount = stoi(value);
		else if (key == "notificationCount") this->notificationCount = stoi(value);
		else if (key == "followingCount")    this->followingCount = stoi(value);
		else if (key == "followersCount")    this->followersCount = stoi(value);
		else if (key == "postCount")         this->postCount = stoi(value);
		else if (key == "savedPostCount")    this->savedPostCount = stoi(value);
	}
	file.close();
}
void loadAllUsers(User** allUsers, int& userCount) {
	ifstream userList("data/users_list.txt");
	if (!userList.is_open()) {
		cout << "No users found." << endl;
		return;
	}

	int index = 0;
	string username;
	while (getline(userList, username)) {
		// Strip Windows-style \r if present
		if (!username.empty() && username.back() == '\r') {
			username.pop_back();
		}

		if (username.empty()) continue;

		// Individual allocation handles its own initializations properly
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
		if (!line.empty() && line.back() == '\r') line.pop_back();
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
string User::getUsername() const {
	return this->username;
}
User* findAndLogin(User**& allUsers, int userCount, string username, string password) {
	for (int i = 0; i < userCount; i++) {
		if (allUsers[i]->getUsername() == username) {
			if (allUsers[i]->getIsBanned()) return nullptr;  
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
	QDir().mkpath("data/Notifications");
	string notifPath = "data/Notifications/" + target->getUsername() + "_notif.txt";
	ofstream notifFile(notifPath, ios::app);
	if (notifFile.is_open()) {
		QDateTime now = QDateTime::currentDateTime();
		notifFile << "follow|"
			<< this->username << " followed you|"
			<< now.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "|0\n";
		notifFile.close();
		qDebug() << "Follow notification created for" << target->getUsername().c_str();
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
	if (followingCount == 0 || following == nullptr) return;
	if (!isFollowing(username)) return;

	User* target = nullptr;
	bool found = false;
	int removeIdx = 0;

	for (int i = 0; i < followingCount; i++) {
		if (following[i] && following[i]->getUsername() == username) {
			target = following[i];
			removeIdx = i;
			found = true;
			break;
		}
	}

	if (!found) return;

	if (followingCount == 1) {
		delete[] following;
		following = nullptr;
		followingCount = 0;
	}
	else {
		User** newFollowing = new User * [followingCount - 1];
		int newIdx = 0;
		for (int i = 0; i < followingCount; i++) {
			if (i != removeIdx)
				newFollowing[newIdx++] = following[i];
		}
		delete[] following;
		following = newFollowing;
		followingCount--;
	}

	string path = "data/Following/" + this->username + "_following.txt";
	ofstream file(path, ios::out);
	if (file.is_open()) {
		for (int i = 0; i < followingCount; i++) {
			if (following[i])
				file << following[i]->getUsername() << "\n";
		}
		file.close();
	}

	if (target)
		target->removeFollower(this->username);  

	saveToFile();
}


void User::removeFollower(const string& usernameToRemove) {
	if (usernameToRemove.empty()) return;

	string path = "data/Following/" + this->username + "_followers.txt";

	// Read all current followers from file, skip the one being removed
	string lines[500];
	int count = 0;

	ifstream file(path);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			if (!line.empty() && line.back() == '\r') line.pop_back();
			if (line.empty()) continue;
			if (line != usernameToRemove)
				lines[count++] = line;
		}
		file.close();
	}

	// Rewrite file without the removed follower
	ofstream out(path, ios::out);
	if (out.is_open()) {
		for (int i = 0; i < count; i++)
			out << lines[i] << "\n";
		out.close();
	}

	// Rebuild followers[] array from scratch — no stale pointers
	delete[] followers;
	followers = nullptr;
	followersCount = count;
	saveToFile();
}
void User::loadFollowing(User** allUsers, int userCount) {
	delete[] following;
	following = nullptr;
	if (followingCount == 0) {
		return;
	}

	string path = "data/Following/" + this->username + "_following.txt";
	ifstream file(path);
	if (!file.is_open()) {
		following = nullptr;
		followingCount = 0;
		return;
	}

	// 🛡️ FIX: Zero-initialize the array
	following = new User * [followingCount]();
	int loaded = 0;

	string line;
	while (getline(file, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (line.empty()) continue;

		// 🛡️ SAFEGUARD: Prevent overflow
		if (loaded >= followingCount) break;

		for (int i = 0; i < userCount; i++) {
			if ((*(allUsers + i))->getUsername() == line) {
				*(following + loaded) = *(allUsers + i);
				loaded++;
				break;
			}
		}
	}
	file.close();
	followingCount = loaded; // Sync count with what actually loaded
}
void User::loadFollowers(User** allusers, int userCount) {
	delete[] followers;
	followers = nullptr;
	if (followersCount == 0) {
		return;
	}

	string path = "data/Following/" + this->username + "_followers.txt";
	ifstream file(path);
	if (!file.is_open()) {
		followers = nullptr;
		followersCount = 0;
		return;
	}

	// 🛡️ FIX: Zero-initialize the array
	followers = new User * [followersCount]();
	int loaded = 0;

	string line;
	while (getline(file, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (line.empty()) continue;

		// 🛡️ SAFEGUARD: Prevent overflow
		if (loaded >= followersCount) break;

		for (int i = 0; i < userCount; i++) {
			if ((*(allusers + i))->getUsername() == line) {
				*(followers + loaded) = *(allusers + i);
				loaded++;
				break;
			}
		}
	}
	file.close();
	followersCount = loaded; // Sync count with what actually loaded
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
	if (posts != nullptr) {
		for (int i = 0; i < postCount; i++) {
			delete posts[i];
		}
		delete[] posts;
	}
	posts = nullptr;
	postCount = 0;
	string listPath = "data/Posts/" + this->username + "/posts_list.txt";
	ifstream listFile(listPath);
	if (!listFile.is_open()) {
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

	// 🛡️ FIX: Zero-initialize the array
	posts = new Posts * [count]();
	postCount = 0;

	ifstream listFile2(listPath);
	string postId;
	while (getline(listFile2, postId)) {
		if (!postId.empty() && postId.back() == '\r') postId.pop_back();

		// 🛡️ SAFEGUARD: Prevent overflow if the file is larger than expected
		if (postCount >= count) break;

		posts[postCount] = new Posts();
		posts[postCount]->loadPostFromFile(this->username, postId);
		postCount++;
	}
	listFile2.close();
}
Posts* User::getPostById(string postId) {
	if (posts == nullptr) return nullptr;
	for (int i = 0; i < postCount; i++) {
		if (!posts[i]) continue;
		if (posts[i]->getPostId() == postId) return posts[i];
	}
	return nullptr;
}
bool User::getIsReported()const {
	return this->isReported;
}
User::~User()  {
	if (posts != nullptr) {
		for (int i = 0; i < postCount; i++) {
			if (posts[i] != nullptr) {
				delete posts[i];
				posts[i] = nullptr;
			}
		}
		delete[] posts;
		posts = nullptr;
	}
	if (savedPosts != nullptr) {
		delete[] savedPosts;
		savedPosts = nullptr;
	}
	if (following != nullptr) {
		delete[] following;
		following = nullptr;
	}
	if (followers != nullptr) {
		delete[] followers;
		followers = nullptr;
	}
}
void User::deleteAccount(User**& allUsers, int& userCount) {

	string uname = this->username;

	// Load our own arrays first
	loadFollowers(allUsers, userCount);
	loadFollowing(allUsers, userCount);

	
	for (int i = 0; i < followersCount; i++) {
		if (!followers[i]) continue;
		followers[i]->loadFollowing(allUsers, userCount);

		User* follower = followers[i];
		if (!follower->isFollowing(uname)) continue;

		if (follower->followingCount == 1) {
			delete[] follower->following;
			follower->following = nullptr;
			follower->followingCount = 0;
		}
		else {
			User** newF = new User * [follower->followingCount - 1];
			int idx = 0;
			for (int j = 0; j < follower->followingCount; j++) {
				if (follower->following[j]->getUsername() != uname)
					newF[idx++] = follower->following[j];
			}
			delete[] follower->following;
			follower->following = newF;
			follower->followingCount--;
		}

		// Rewrite their _following.txt
		string fPath = "data/Following/" + follower->getUsername() + "_following.txt";
		ofstream fFile(fPath, ios::out);
		if (fFile.is_open()) {
			for (int j = 0; j < follower->followingCount; j++)
				fFile << follower->following[j]->getUsername() << "\n";
			fFile.close();
		}
		follower->saveToFile();  // persists updated followingCount
	}

	// For each person I follow — remove me from their followers
	for (int i = 0; i < followingCount; i++) {
		if (!following[i]) continue;
		following[i]->loadFollowers(allUsers, userCount);

		User* followedUser = following[i];

		if (followedUser->followersCount == 1) {
			delete[] followedUser->followers;
			followedUser->followers = nullptr;
			followedUser->followersCount = 0;
		}
		else {
			User** newF = new User * [followedUser->followersCount - 1];
			int idx = 0;
			for (int j = 0; j < followedUser->followersCount; j++) {
				if (followedUser->followers[j]->getUsername() != uname)
					newF[idx++] = followedUser->followers[j];
			}
			delete[] followedUser->followers;
			followedUser->followers = newF;
			followedUser->followersCount--;
		}

		// Rewrite their _followers.txt
		string fPath = "data/Following/" + followedUser->getUsername() + "_followers.txt";
		ofstream fFile(fPath, ios::out);
		if (fFile.is_open()) {
			for (int j = 0; j < followedUser->followersCount; j++)
				fFile << followedUser->followers[j]->getUsername() << "\n";
			fFile.close();
		}
		followedUser->saveToFile();  // persists updated followersCount
	}

	// ── 2. Delete this user's posts + comment files from disk ─────────────
	loadAllPosts();
	for (int i = 0; i < postCount; i++) {
		if (!posts[i]) continue;
		string postId = posts[i]->getPostId();
		QFile::remove(QString::fromStdString(
			"data/Posts/" + uname + "/" + postId + ".txt"));
		QFile::remove(QString::fromStdString(
			"data/Posts/" + uname + "/" + postId + "_comments.txt"));
	}

	// ── 3. Delete liked file ───────────────────────────────────────────────
	QFile::remove(QString::fromStdString(
		"data/Posts/" + uname + "/" + uname + "_liked.txt"));

	// ── 4. Delete posts_list.txt and saved_posts.txt ──────────────────────
	QFile::remove(QString::fromStdString(
		"data/Posts/" + uname + "/posts_list.txt"));
	QFile::remove(QString::fromStdString(
		"data/Posts/" + uname + "/saved_posts.txt"));

	// ── 5. Remove entire Posts/username/ folder ───────────────────────────
	QDir(QString::fromStdString("data/Posts/" + uname)).removeRecursively();

	// ── 6. Remove comments this user left on other people's posts ─────────
	for (int u = 0; u < userCount; u++) {
		if (!allUsers[u] || allUsers[u]->getUsername() == uname) continue;
		allUsers[u]->loadAllPosts();
		for (int p = 0; p < allUsers[u]->getPostCount(); p++) {
			Posts* post = allUsers[u]->getPostByIndex(p);
			if (!post || !post->isValid()) continue;
			QList<Comment> comments = post->getComments();
			for (int c = comments.size() - 1; c >= 0; c--) {
				if (comments[c].getCreatorUsername().toStdString() == uname)
					post->deleteCommentAsAdmin(c);
			}
		}
	}

	// ── 7. Remove this user's posts from other users' saved_posts.txt ─────
	for (int u = 0; u < userCount; u++) {
		if (!allUsers[u] || allUsers[u]->getUsername() == uname) continue;
		QString savedPath = QString::fromStdString(
			"data/Posts/" + allUsers[u]->getUsername() + "/saved_posts.txt");
		QFile f(savedPath);
		QStringList remaining;
		if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&f);
			while (!in.atEnd()) {
				QString line = in.readLine().trimmed();
				if (!line.isEmpty() &&
					!line.endsWith("|" + QString::fromStdString(uname)))
					remaining << line;
			}
			f.close();
		}
		if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&f);
			for (const QString& l : remaining) out << l << "\n";
			f.close();
		}
	}

	// ── 8. Delete following/followers files ───────────────────────────────
	QFile::remove(QString::fromStdString(
		"data/Following/" + uname + "_following.txt"));
	QFile::remove(QString::fromStdString(
		"data/Following/" + uname + "_followers.txt"));

	// ── 9. Delete notifications file ──────────────────────────────────────
	QFile::remove(QString::fromStdString(
		"data/Notifications/" + uname + "_notif.txt"));

	// ── 10. Delete messages ────────────────────────────────────────────────
	QDir msgDir("data/Messages");
	QString quname = QString::fromStdString(uname);
	QStringList msgFiles = msgDir.entryList(QStringList() << "*.txt", QDir::Files);
	for (const QString& fname : msgFiles) {
		if (fname.contains(quname))
			QFile::remove("data/Messages/" + fname);
	}
	for (int u = 0; u < userCount; u++) {
		if (!allUsers[u] || allUsers[u]->getUsername() == uname) continue;
		QString peer = QString::fromStdString(allUsers[u]->getUsername());
		QString indexPath = "data/Messages/" + peer + "_index.txt";
		QFile f(indexPath);
		QStringList remaining;
		if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&f);
			while (!in.atEnd()) {
				QString l = in.readLine().trimmed();
				if (!l.isEmpty() && l != quname) remaining << l;
			}
			f.close();
		}
		if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&f);
			for (const QString& l : remaining) out << l << "\n";
			f.close();
		}
	}

	// ── 11. Delete user file and users_list.txt entry ─────────────────────
	QFile::remove(QString::fromStdString("data/Users/" + uname + ".txt"));
	removeFromUser_List(uname);

	// ── 12. Remove from allUsers array ────────────────────────────────────
	User** newArray = (userCount - 1 > 0) ? new User * [userCount - 1] : nullptr;
	int idx = 0;
	for (int i = 0; i < userCount; i++) {
		if (allUsers[i] && allUsers[i]->getUsername() != uname)
			newArray[idx++] = allUsers[i];
	}
	delete[] allUsers;
	allUsers = newArray;
	userCount--;

	qDebug() << "Account fully deleted:" << QString::fromStdString(uname);
}
string User::getBio() const {
	return this->bio;
}

string User::getPassword() const {
	return this->password;
}

void User::setBio(string bio) {
	this->bio = bio;
}

void User::setPassword(string password) {
	this->password = password;
}

int User::getFollowingCount() const {
	return this->followingCount;
}
void User::clearSavedPostsArray() {
	delete[] savedPosts;
	savedPosts = nullptr;
	savedPostCount = 0;
}
int User::getFollowersCount()const {
	return this->followersCount;
}
bool User::hasReportedUser(const string& reporterUsername) const {
	string path = "data/Users/" + this->username + "_reporters.txt";
	ifstream file(path);
	if (!file.is_open()) return false;
	string line;
	while (getline(file, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (line == reporterUsername) return true;
	}
	return false;
}

void User::reportUserBy(const string& reporterUsername) {
	if (hasReportedUser(reporterUsername)) return;

	// Record this reporter
	string path = "data/Users/" + this->username + "_reporters.txt";
	ofstream file(path, ios::app);
	if (file.is_open()) {
		file << reporterUsername << "\n";
		file.close();
	}

	reportUser();  // existing method — increments count, handles admin notif
}
int User::getPostCount() const {
	return this->postCount;
}

Posts* User::getPostByIndex(int index) {
	if (index < 0 || index >= postCount || posts == nullptr)
		return nullptr;
	return posts[index];
}
bool User::editPost(const string& postId, const string& newContent, QString& errorOut) {
	errorOut = validatePostContent(newContent);
	if (!errorOut.isEmpty()) return false;

	Posts* p = getPostById(postId);
	if (!p) { errorOut = "Post not found."; return false; }

	p->setContent(newContent);
	p->savePostToFile();
	return true;
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
	if (postCount == 0) return;
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

void User::reportUser() {
	isReportedCount++;

	if (isReportedCount >= 3 && !isReported) {
		isReported = true;

		// Write admin notification
		QDir().mkpath("data/Admin");
		QString adminNotifPath = "data/Admin/admin_notifications.txt";
		QFile notifFile(adminNotifPath);
		if (notifFile.open(QIODevice::Append | QIODevice::Text)) {
			QTextStream out(&notifFile);
			QString ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
			out << ts << "|user_reported|User '@"
				<< QString::fromStdString(username)
				<< "' has reached 3 reports and is marked for review.|0\n";
			notifFile.close();
			qDebug() << "Admin notified about reported user:"
				<< QString::fromStdString(username);
		}
	}

	saveToFile(); 
}


void User::unsavePost(string postId) {
	bool found = false;
	if (savedPosts == nullptr || savedPostCount == 0) return;
	for (int i = 0; i < savedPostCount; i++) {

		if (savedPosts[i]->getPostId() == postId) {
			found = true;
			break;
		}
	}
	if (!found) {
		qDebug() << "Post not found in saved list.";
		return;
	}

	Posts** newSaved = nullptr;
	if (savedPostCount - 1 > 0) {
		newSaved = new Posts * [savedPostCount - 1];
		int index = 0;
		for (int i = 0; i < savedPostCount; i++) {
			if (savedPosts[i]->getPostId() != postId)
				newSaved[index++] = savedPosts[i];
		}
	}
	delete[] savedPosts;
	savedPosts = newSaved;
	savedPostCount--;

	saveSavedPostsToFile();
	saveToFile();
	qDebug() << "Post removed from favourites.";
}

void User::displaySavedPosts() {
	if (savedPostCount == 0 || savedPosts == nullptr) {
		qDebug() << "No saved posts";
		return;
	}
	for (int i = 0; i < savedPostCount; i++) savedPosts[i]->display();
}
void User::saveSavedPostsToFile() {
	string path = "data/Posts/" + this->username + "/saved_posts.txt";
	ofstream file(path);
	if (!file.is_open()) {
		cout << "Error: Cannot open saved posts file: " << path << endl;
		return;
	}
	for (int i = 0; i < savedPostCount; i++) {
		file << savedPosts[i]->getPostId() << "|" << savedPosts[i]->getCreatorUsername() << "\n";
	}
	file.close();
}

void User::loadSavedPosts(User** allUsers, int userCount) {
	string path = "data/Posts/" + this->username + "/saved_posts.txt";
	ifstream file(path);
	if (!file.is_open()) {
		if (savedPosts != nullptr) { delete[] savedPosts; savedPosts = nullptr; }
		savedPostCount = 0;
		return;
	}
	int count = 0;
	string line;
	while (getline(file, line)) {
		if (!line.empty()) {
			count++;
		}
	}
	file.close();

	if (count == 0) {
		if (savedPosts != nullptr) { delete[] savedPosts; savedPosts = nullptr; }
		savedPostCount = 0;
		return;
	}

	if (savedPosts != nullptr) { delete[] savedPosts; }

	// 🛡️ FIX: Zero-initialize the array to prevent garbage pointers
	savedPosts = new Posts * [count]();
	savedPostCount = 0;

	ifstream file2(path);
	while (getline(file2, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (line.empty()) continue;

		int sep = 0;
		while (sep < (int)line.size() && line[sep] != '|') {
			sep++;
		}

		string postId = line.substr(0, sep);
		string ownerUsername = (sep + 1 < (int)line.size()) ? line.substr(sep + 1) : "";

		while (!postId.empty() && (postId.back() == '\r' || postId.back() == ' ')) postId.pop_back();
		while (!ownerUsername.empty() && (ownerUsername.back() == '\r' || ownerUsername.back() == ' ')) ownerUsername.pop_back();

		if (postId.empty() || ownerUsername.empty()) {
			continue;
		}
		for (int i = 0; i < userCount; i++) {
			if (!allUsers[i]) continue;
			if (allUsers[i]->getUsername() == ownerUsername) {
				if (allUsers[i]->posts == nullptr && allUsers[i]->postCount > 0)
					allUsers[i]->loadAllPosts();
				Posts* p = allUsers[i]->getPostById(postId);

				// 🛡️ SAFEGUARD: Ensure we don't overflow the array
				if (p != nullptr && savedPostCount < count) {
					savedPosts[savedPostCount++] = p;
				}
				break;
			}
		}
	}
	file2.close();
}
string User::getCreatorUsername() const {
	return this->username;
}
void User::setBan(bool banned) {
	this->isBanned = banned;
	saveToFile();
}
bool   User::getIsBanned()   const {
	return isBanned;
}
void User::savePost(string postId, User* postOwner) {
	if (postOwner->getUsername() == this->username) {
		qDebug() << "You cannot save your own post.";
		return;
	}

	// Check if already saved
	if (savedPosts != nullptr) {
		for (int i = 0; i < savedPostCount; i++) {
			if (savedPosts[i] && savedPosts[i]->getPostId() == postId) {
				qDebug() << "Post already saved.";
				return;
			}
		}
	}
	if (postOwner->posts == nullptr && postOwner->postCount > 0)
		postOwner->loadAllPosts();
	Posts* p = postOwner->getPostById(postId);
	if (p == nullptr) {
		qDebug() << "Post not found.";
		return;
	}

	Posts** newSaved = new Posts * [savedPostCount + 1];
	for (int i = 0; i < savedPostCount; i++)
		newSaved[i] = savedPosts[i];

	newSaved[savedPostCount] = p;
	delete[] savedPosts;
	savedPosts = newSaved;
	savedPostCount++;

	saveSavedPostsToFile();
	saveToFile();
	qDebug() << "Post saved to favourites.";
}
int User::getSavedPostCount() const {
	return this->savedPostCount;
}

Posts* User::getSavedPostByIndex(int index) {
	if (index < 0 || index >= savedPostCount || savedPosts == nullptr)
		return nullptr;
	return savedPosts[index];
}
// Add these methods to User.cpp (at the end, before destructor)

// ══════════════════════════════════════════════════════════════════════════════
//  Qt HELPER METHODS (NEW)
// ══════════════════════════════════════════════════════════════════════════════

QString User::getDisplayUsername() const {
	return QString::fromStdString(this->username);
}

QString User::getDisplayBio() const {
	return QString::fromStdString(this->bio);
}

QList<Posts*> User::getPostsAsQList() const {
	QList<Posts*> result;
	if (posts != nullptr) {
		for (int i = 0; i < postCount; i++) {
			if (posts[i]) {
				result.append(posts[i]);
			}
		}
	}
	return result;
}

QList<Posts*> User::getSavedPostsAsQList() const {
	QList<Posts*> result;
	if (savedPosts != nullptr) {
		for (int i = 0; i < savedPostCount; i++) {
			if (savedPosts[i]) {
				result.append(savedPosts[i]);
			}
		}
	}
	return result;
}

bool User::hasPost(const string& postId) const {
	if (posts == nullptr) return false;
	for (int i = 0; i < postCount; i++) {
		if (posts[i] && posts[i]->getPostId() == postId) {
			return true;
		}
	}
	return false;
}
// ══════════════════════════════════════════════════════════════════════════════
//  BACKEND CONVERSATION INDEX (MESSAGES)
// ══════════════════════════════════════════════════════════════════════════════

QList<QString> User::getConversationHistory(const string& username) {
	QList<QString> peers;
	QString path = QString::fromStdString("data/Messages/" + username + "_index.txt");
	QFile f(path);

	if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&f);
		while (!in.atEnd()) {
			QString peer = in.readLine().trimmed();
			if (!peer.isEmpty() && !peers.contains(peer)) {
				peers.append(peer);
			}
		}
		f.close();
	}
	return peers;
}

void User::addConversationToHistory(const string& username, const string& peerUsername) {
	QDir().mkpath("data/Messages");
	QString path = QString::fromStdString("data/Messages/" + username + "_index.txt");
	QString peerStr = QString::fromStdString(peerUsername);

	// Read existing to prevent duplicates
	QList<QString> existing;
	QFile rf(path);
	if (rf.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&rf);
		while (!in.atEnd()) {
			QString l = in.readLine().trimmed();
			if (!l.isEmpty()) existing.append(l);
		}
		rf.close();
	}

	if (existing.contains(peerStr)) return;

	// Append new peer
	QFile wf(path);
	if (wf.open(QIODevice::Append | QIODevice::Text)) {
		QTextStream out(&wf);
		out << peerStr << "\n";
		wf.close();
	}
}

void User::removeConversationFromHistory(const string& username, const string& peerUsername) {
	QString path = QString::fromStdString("data/Messages/" + username + "_index.txt");
	QFile f(path);
	QStringList remaining;

	if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&f);
		while (!in.atEnd()) {
			QString l = in.readLine().trimmed();
			if (!l.isEmpty() && l.toStdString() != peerUsername) remaining.append(l);
		}
		f.close();
	}

	if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&f);
		for (const QString& l : remaining) out << l << "\n";
		f.close();
	}
}