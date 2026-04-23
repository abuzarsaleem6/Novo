#pragma once

#include <QtWidgets/QMainWindow>
#include "User.h"
#include "Post.h"
#include "Comment.h"
#include "Feed.h"
#include "PasswordChecker.h"
#include "Notification.h"
#include "SearchEngine.h"
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextEdit>  // FIX: needed for inline post composer
#include <QTimer>
#include <QtWidgets/QLineEdit>
#include <QDateTime>

// Forward declarations
class User;
class Posts;
class Notification;

// --- Custom Widgets ---

class SidebarButton : public QPushButton {
    Q_OBJECT
public:
    SidebarButton(const QString& icon, const QString& label, QWidget* parent = nullptr);
    void setActive(bool active);
};

class PostCard : public QFrame {
    Q_OBJECT
public:
    PostCard(Posts* post, const QString& authorUsername, bool isOwner, QWidget* parent = nullptr);
signals:
    void likeClicked(Posts* post);
    void commentClicked(Posts* post);
    void deleteClicked(Posts* post);
private:
    Posts* m_post;
    QString m_authorUsername;
    bool m_isOwner;
};

class NotificationItem : public QFrame {
    Q_OBJECT
public:
    NotificationItem(const Notification& notif, QWidget* parent = nullptr);
};

// --- Page Classes ---

class AuthPage : public QWidget {
    Q_OBJECT
public:
    AuthPage(QWidget* parent = nullptr);
    void resetToLogin();
signals:
    void loginSuccess(User* user, User** allUsers, int userCount);
    void loginAdminSuccess();
private slots:
    void onLogin();
    void onSignUp();
private:
    QWidget* createLandingWidget();
    QWidget* createLoginWidget();
    QWidget* createSignupWidget();
    QWidget* createAdminLoginWidget();
    QStackedWidget* m_stack;
    QLineEdit* m_loginUser, * m_loginPass, * m_signupUser, * m_signupPass, * m_signupBio;
    User** m_allUsers;
    int m_userCount;
};

class FeedPage : public QWidget {
    Q_OBJECT
public:
    FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();

    // FIX 3 & FIX 4: Inline post composer - exposed so MainWindow can show/hide
    QFrame* m_composerCard;
    QTextEdit* m_postInput;   // public so MainWindow can call setFocus()

private slots:
    void onLikePost(Posts* post);
    void onCommentPost(Posts* post);
    void onDeletePost(Posts* post);
    void onSubmitPost();    // FIX 4: inline submit
private:
    void loadPosts();
    void clearFeed();
    User* m_user;
    User** m_allUsers;
    int m_userCount;
    QScrollArea* m_scrollArea;
    QWidget* m_feedContent;
    QVBoxLayout* m_feedLayout;
};

class NotificationsPage : public QWidget {
    Q_OBJECT
public:
    NotificationsPage(User* currentUser, QWidget* parent = nullptr);
    void refresh();
private:
    void loadNotifications();
    User* m_user;
    QScrollArea* m_scrollArea;
    QWidget* m_listContent;
    QVBoxLayout* m_listLayout;
};

class SearchPage : public QWidget {
    Q_OBJECT
public:
    SearchPage(User** allUsers, int userCount, User* currentUser, QWidget* parent = nullptr);
private slots:
    void onSearch();
    void onFollowUser();
private:
    void showUserCard(User* user);
    User* m_currentUser;
    User** m_allUsers;
    int m_userCount;
    QLineEdit* m_searchInput;
    QScrollArea* m_scrollArea;
    QWidget* m_resultsContent;
    QVBoxLayout* m_resultsLayout;
    User* m_foundUser;
    QPushButton* m_followBtn;
    SearchEngine m_engine;
};

class ProfilePage : public QWidget {
    Q_OBJECT
public:
    ProfilePage(User* currentUser, User** allUsers, int* userCount, QWidget* parent = nullptr);
    void refresh();
private slots:
    void onUpdateBio();
    void onUpdatePassword();
    void onDeleteAccount();
private:
    User* m_user;
    User** m_allUsers;
    int* m_userCountPtr;
    QLabel* m_usernameLabel, * m_bioLabel;
    QLineEdit* m_newBioInput, * m_newPassInput;
};

class TimeSpentPage : public QWidget {
    Q_OBJECT
public:
    TimeSpentPage(QWidget* parent = nullptr);
    void startSession();
    void stopSession();
private slots:
    void onTick();
private:
    void updateDisplay();
    QTimer* m_timer;
    int m_elapsed;
    QDateTime m_sessionStart;
    QLabel* m_timerLabel, * m_sessionLabel;
};

class MessagesPage : public QWidget { Q_OBJECT public: MessagesPage(QWidget* parent = nullptr); };
class AdminPage : public QWidget { Q_OBJECT public: AdminPage(QWidget* parent = nullptr); };

// --- Main Window ---

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(User* user, User** allUsers, int userCount);
    void onLoginAdminSuccess();
    void onNavFeed();
    void onNavNotifications();
    void onNavSearch();
    void onNavMessages();
    void onNavProfile();
    void onNavTimeSpent();
    void onLogout();
    void onSwitchUser();
    void onSidebarCreatePost(); // FIX 4: now shows inline composer on feed

private:
    void buildSidebar();
    void buildPages();
    void tearDownShell();
    void setActiveSidebarButton(SidebarButton* active);
    void animateFade(QWidget* widget);

    AuthPage* m_authPage;
    QWidget* m_appShell;
    QHBoxLayout* m_shellLayout;
    QWidget* m_sidebar;
    QStackedWidget* m_pages;

    SidebarButton* m_btnFeed, * m_btnNotifications, * m_btnSearch,
        * m_btnMessages, * m_btnProfile, * m_btnTimeSpent,
        * m_btnSwitchUser, * m_btnLogout;

    FeedPage* m_feedPage;
    NotificationsPage* m_notifPage;
    SearchPage* m_searchPage;
    MessagesPage* m_messagesPage;
    ProfilePage* m_profilePage;
    TimeSpentPage* m_timeSpentPage;
    AdminPage* m_adminPage;

    User* m_currentUser;
    User** m_allUsers;
    int    m_userCount;
};