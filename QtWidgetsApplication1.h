#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollArea>
#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QStyle>

#include "User.h"
#include "Post.h"
#include "Notification.h"
#include "Feed.h"
#include "SearchEngine.h"
#include "PasswordChecker.h"

// ─────────────────────────────────────────────────────────────────────────────
//  SidebarButton
// ─────────────────────────────────────────────────────────────────────────────
class SidebarButton : public QPushButton
{
    Q_OBJECT
public:
    explicit SidebarButton(const QString& icon, const QString& label,
        QWidget* parent = nullptr);
    void setActive(bool active);
};

// ─────────────────────────────────────────────────────────────────────────────
//  PostCard
// ─────────────────────────────────────────────────────────────────────────────
class PostCard : public QFrame
{
    Q_OBJECT
public:
    explicit PostCard(Posts* post, const QString& authorUsername,
        bool isOwner, QWidget* parent = nullptr);
signals:
    void likeClicked(Posts* post);
    void commentClicked(Posts* post);
    void deleteClicked(Posts* post);
private:
    Posts* m_post;
    QString m_authorUsername;
    bool    m_isOwner;
};

// ─────────────────────────────────────────────────────────────────────────────
//  NotificationItem
// ─────────────────────────────────────────────────────────────────────────────
class NotificationItem : public QFrame
{
    Q_OBJECT
public:
    explicit NotificationItem(const Notification& notif, QWidget* parent = nullptr);
};

// ─────────────────────────────────────────────────────────────────────────────
//  AuthPage
// ─────────────────────────────────────────────────────────────────────────────
class AuthPage : public QWidget
{
    Q_OBJECT
public:
    explicit AuthPage(QWidget* parent = nullptr);
    // Called after switch-user so fields are cleared
    void resetToLogin();

signals:
    void loginSuccess(User* user, User** allUsers, int userCount);

private slots:
    void onLogin();
    void onSignUp();
    void toggleMode();

private:
    QStackedWidget* m_stack;
    QLineEdit* m_loginUser;
    QLineEdit* m_loginPass;
    QLineEdit* m_signupUser;
    QLineEdit* m_signupPass;
    QLineEdit* m_signupBio;
    User** m_allUsers;
    int    m_userCount;
    bool   m_isLoginMode;

    QWidget* createLoginWidget();
    QWidget* createSignupWidget();
};

// ─────────────────────────────────────────────────────────────────────────────
//  FeedPage
// ─────────────────────────────────────────────────────────────────────────────
class FeedPage : public QWidget
{
    Q_OBJECT
public:
    explicit FeedPage(User* currentUser, User** allUsers, int userCount,
        QWidget* parent = nullptr);
    void refresh();

private slots:
    void onCreatePost();
    void onLikePost(Posts* post);
    void onCommentPost(Posts* post);
    void onDeletePost(Posts* post);

private:
    User* m_user;
    User** m_allUsers;
    int          m_userCount;
    QVBoxLayout* m_feedLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_feedContent;
    QTextEdit* m_postInput;

    void loadPosts();
    void clearFeed();
};

// ─────────────────────────────────────────────────────────────────────────────
//  NotificationsPage
// ─────────────────────────────────────────────────────────────────────────────
class NotificationsPage : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationsPage(User* currentUser, QWidget* parent = nullptr);
    void refresh();

private:
    User* m_user;
    QVBoxLayout* m_listLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_listContent;

    void loadNotifications();
};

// ─────────────────────────────────────────────────────────────────────────────
//  SearchPage
// ─────────────────────────────────────────────────────────────────────────────
class SearchPage : public QWidget
{
    Q_OBJECT
public:
    explicit SearchPage(User** allUsers, int userCount, User* currentUser,
        QWidget* parent = nullptr);

private slots:
    void onSearch();
    void onFollowUser();

private:
    User* m_currentUser;
    User** m_allUsers;
    int          m_userCount;
    QLineEdit* m_searchInput;
    QVBoxLayout* m_resultsLayout;
    QWidget* m_resultsContent;
    QScrollArea* m_scrollArea;
    SearchEngine m_engine;
    User* m_foundUser;
    QPushButton* m_followBtn;

    void showUserCard(User* user);
};

// ─────────────────────────────────────────────────────────────────────────────
//  ProfilePage
// ─────────────────────────────────────────────────────────────────────────────
class ProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit ProfilePage(User* currentUser, User** allUsers, int* userCount,
        QWidget* parent = nullptr);
    void refresh();

private slots:
    void onUpdateBio();
    void onUpdatePassword();
    void onDeleteAccount();

private:
    User* m_user;
    User** m_allUsers;
    int* m_userCountPtr;
    QLabel* m_usernameLabel;
    QLabel* m_bioLabel;
    QLineEdit* m_newBioInput;
    QLineEdit* m_newPassInput;
};

// ─────────────────────────────────────────────────────────────────────────────
//  TimeSpentPage
// ─────────────────────────────────────────────────────────────────────────────
class TimeSpentPage : public QWidget
{
    Q_OBJECT
public:
    explicit TimeSpentPage(QWidget* parent = nullptr);
    void startSession();
    void stopSession();

private slots:
    void onTick();

private:
    QLabel* m_timerLabel;
    QLabel* m_sessionLabel;
    QTimer* m_timer;
    int       m_elapsed;
    QDateTime m_sessionStart;

    void updateDisplay();
};

// ─────────────────────────────────────────────────────────────────────────────
//  MainWindow
// ─────────────────────────────────────────────────────────────────────────────
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(User* user, User** allUsers, int userCount);
    void onNavFeed();
    void onNavNotifications();
    void onNavSearch();
    void onNavProfile();
    void onNavTimeSpent();
    void onLogout();
    void onSwitchUser();   // ← NEW

private:
    AuthPage* m_authPage;
    QWidget* m_appShell;
    QHBoxLayout* m_shellLayout;
    QWidget* m_sidebar;
    SidebarButton* m_btnFeed;
    SidebarButton* m_btnNotifications;
    SidebarButton* m_btnSearch;
    SidebarButton* m_btnProfile;
    SidebarButton* m_btnTimeSpent;
    SidebarButton* m_btnSwitchUser;  // ← NEW
    SidebarButton* m_btnLogout;
    QStackedWidget* m_pages;
    FeedPage* m_feedPage;
    NotificationsPage* m_notifPage;
    SearchPage* m_searchPage;
    ProfilePage* m_profilePage;
    TimeSpentPage* m_timeSpentPage;
    User* m_currentUser;
    User** m_allUsers;
    int                m_userCount;

    void buildSidebar();
    void buildPages();
    void setActiveSidebarButton(SidebarButton* active);
    void animateFade(QWidget* widget);
    void tearDownShell();
};