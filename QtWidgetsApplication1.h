#pragma once

#include <QtWidgets/QMainWindow>
#include "User.h"
#include "Post.h"
#include "Comment.h"
#include "Feed.h"
#include "Admin.h"
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
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QTimer>
#include <QDateTime>
#include <QDir>

// Forward declarations
class User;
class Posts;
class Notification;

// ─────────────────────────────────────────────────────────────
//  SidebarButton
//  Custom button for sidebar navigation with active state
// ─────────────────────────────────────────────────────────────
class SidebarButton : public QPushButton {
    Q_OBJECT
public:
    SidebarButton(const QString& icon, const QString& label, QWidget* parent = nullptr);
    void setActive(bool active);
};

// ─────────────────────────────────────────────────────────────
//  PostCard
//  Displays a single post with header, content, and action bar
// ─────────────────────────────────────────────────────────────
class PostCard : public QFrame {
    Q_OBJECT
public:
    PostCard(Posts* post,
        const QString& authorUsername,
        bool isOwner,
        bool isSaved,
        const QString& viewerUsername,
        QWidget* parent = nullptr);
signals:
    void likeClicked(Posts* post);
    void commentClicked(Posts* post);
    void editClicked(Posts* post);
    void deleteClicked(Posts* post);
    void saveClicked(Posts* post, const QString& ownerUsername);
    void unsaveClicked(Posts* post);
    void reportClicked(Posts* post, const QString& ownerUsername);
private:
    Posts* m_post;
    QString m_authorUsername;
    bool    m_isOwner;
    bool    m_isSaved;
    QString m_viewerUsername;
};

// ─────────────────────────────────────────────────────────────
//  NotificationItem
//  Single notification card with type icon and status indicator
// ─────────────────────────────────────────────────────────────
class NotificationItem : public QFrame {
    Q_OBJECT
public:
    NotificationItem(const Notification& notif, QWidget* parent = nullptr);
};

// ─────────────────────────────────────────────────────────────
//  AuthPage
//  Landing, login, signup, and admin login screens
// ─────────────────────────────────────────────────────────────
class AuthPage : public QWidget {
    Q_OBJECT
public:
    explicit AuthPage(QWidget* parent = nullptr);
    ~AuthPage();
    void resetToLogin();

    User** getAllUsers() const { return m_allUsers; }
    int getUserCount() const { return m_userCount; }

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
    QLineEdit* m_loginUser;
    QLineEdit* m_loginPass;
    QLineEdit* m_signupUser;
    QLineEdit* m_signupPass;
    QLineEdit* m_signupBio;
    User** m_allUsers;
    int        m_userCount;
};

// ─────────────────────────────────────────────────────────────
//  FeedPage
//  Main feed showing posts from followed users
// ─────────────────────────────────────────────────────────────
class FeedPage : public QWidget {
    Q_OBJECT
public:
    FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();

    QFrame* m_composerCard;
    QTextEdit* m_postInput;

private slots:
    void onLikePost(Posts* post);
    void onCommentPost(Posts* post);
    void onDeletePost(Posts* post);
    void onSavePost(Posts* post, const QString& ownerUsername);
    void onUnsavePost(Posts* post);
    void onReportPost(Posts* post, const QString& ownerUsername);
    void onSubmitPost();

private:
    void loadPosts();
    void clearFeed();

    User* m_user;
    User** m_allUsers;
    int          m_userCount;
    QScrollArea* m_scrollArea;
    QWidget* m_feedContent;
    QVBoxLayout* m_feedLayout;
};

// ─────────────────────────────────────────────────────────────
//  MyPostsPage
//  User's own posts with delete functionality
// ─────────────────────────────────────────────────────────────
class MyPostsPage : public QWidget {
    Q_OBJECT
public:
    MyPostsPage(User* currentUser, QWidget* parent = nullptr);
    void refresh();
private slots:
    void onDeletePost(Posts* post);
    void onEditPost(Posts* post);
    void onCommentPost(Posts* post);
private:
    void loadPosts();
    void clearPosts();
    User* m_user;
    QScrollArea* m_scrollArea;
    QWidget* m_postsContent;
    QVBoxLayout* m_postsLayout;
};

// ─────────────────────────────────────────────────────────────
//  SavedPostsPage
//  User's bookmarked/saved posts
// ─────────────────────────────────────────────────────────────
class SavedPostsPage : public QWidget {
    Q_OBJECT
public:
    SavedPostsPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();
private slots:
    void onUnsavePost(Posts* post);
private:
    void loadSaved();
    void clearSaved();
    User* m_user;
    User** m_allUsers;
    int          m_userCount;
    QScrollArea* m_scrollArea;
    QWidget* m_savedContent;
    QVBoxLayout* m_savedLayout;
};

// ─────────────────────────────────────────────────────────────
//  NotificationsPage
//  Activity notifications for likes, comments, follows
// ─────────────────────────────────────────────────────────────
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
    void onMarkAllRead();
};

// ─────────────────────────────────────────────────────────────
//  SearchPage
//  User search with follow/report actions
// ─────────────────────────────────────────────────────────────
class SearchPage : public QWidget {
    Q_OBJECT
public:
    SearchPage(User** allUsers, int userCount, User* currentUser, QWidget* parent = nullptr);
private slots:
    void onSearch();
    void onFollowUser();
    void onReportUser();
private:
    void showUserCard(User* user);

    User* m_currentUser;
    User** m_allUsers;
    int          m_userCount;
    QLineEdit* m_searchInput;
    QScrollArea* m_scrollArea;
    QWidget* m_resultsContent;
    QVBoxLayout* m_resultsLayout;
    User* m_foundUser;
    QPushButton* m_followBtn;
    QPushButton* m_reportBtn;
    SearchEngine m_engine;
};

// ─────────────────────────────────────────────────────────────
//  ProfilePage
//  User profile with bio/password updates and post tabs
// ─────────────────────────────────────────────────────────────
class ProfilePage : public QWidget {
    Q_OBJECT
public:
    ProfilePage(User* currentUser, User** allUsers, int* userCount, QWidget* parent = nullptr);
    void refresh();
signals:
    void accountDeleted();
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

    MyPostsPage* m_myPostsPage;
    SavedPostsPage* m_savedPage;

    QLabel* m_postsCountLabel = nullptr;
    QLabel* m_followersCountLabel = nullptr;
    QLabel* m_followingCountLabel = nullptr;
};

// ─────────────────────────────────────────────────────────────
//  TimeSpentPage
//  Session timer tracking and engagement stats
// ─────────────────────────────────────────────────────────────
class TimeSpentPage : public QWidget {
    Q_OBJECT
public:
    explicit TimeSpentPage(QWidget* parent = nullptr);
    void startSession();
    void stopSession();
private slots:
    void onTick();
private:
    void updateDisplay();
    QTimer* m_timer;
    int        m_elapsed;
    QDateTime  m_sessionStart;
    QLabel* m_timerLabel;
    QLabel* m_sessionLabel;
};

// ─────────────────────────────────────────────────────────────
//  ChatView
//  Full conversation between currentUser and one peer.
//  Reads/writes data/Messages/<userA>_<userB>.txt
// ─────────────────────────────────────────────────────────────
class ChatView : public QWidget {
    Q_OBJECT
public:
    ChatView(const QString& currentUser,
        const QString& peerUsername,
        QWidget* parent = nullptr);
    void refresh();

signals:
    void chatDeleted(const QString& peer);   // emitted after delete

private slots:
    void onSend();
    void onDeleteChat();
    void markConversationUnread(const QString& receiver, const QString& sender);
private:
    QString   chatFilePath() const;
    void      loadMessages();
    void      clearMessages();
    void      appendBubble(const QString& sender,
        const QString& text);

    QString       m_currentUser;
    QString       m_peer;
    QScrollArea* m_scroll;
    QWidget* m_bubbleContainer;
    QVBoxLayout* m_bubbleLayout;
    QLineEdit* m_input;
};

// ─────────────────────────────────────────────────────────────
//  MessagesPage
//  Left panel: conversation list + "New Message" search
//  Right panel: ChatView (swapped via QStackedWidget)
// ─────────────────────────────────────────────────────────────
class MessagesPage : public QWidget {
    Q_OBJECT
public:
    MessagesPage(User* currentUser, User** allUsers, int userCount,
        QWidget* parent = nullptr);
    void refresh();

    // Called from SearchPage / anywhere that wants to open a chat directly
    void openChatWith(const QString& peer);

private slots:
    void onSearchUser();
    void onConversationSelected(const QString& peer);
    void onChatDeleted(const QString& peer);

private:
    void   loadConversationList();
    void   clearConversationList();
    void   addConversationButton(const QString& peer);
    static QStringList knownPeers(const QString& username);   // reads data/Messages index

    User* m_currentUser;
    User** m_allUsers;
    int           m_userCount;

    // Left panel
    QWidget* m_leftPanel;
    QLineEdit* m_searchInput;
    QWidget* m_convListContent;
    QVBoxLayout* m_convListLayout;

    // Right panel
    QStackedWidget* m_rightStack;   // 0=placeholder, 1=ChatView
    ChatView* m_chatView;

    QString       m_activePeer;
};

// ─────────────────────────────────────────────────────────────
//  AdminPage
//  Dashboard for reviewing reported users and posts
// ─────────────────────────────────────────────────────────────
class AdminPage : public QWidget {
    Q_OBJECT
public:
    AdminPage(User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();
private slots:
    void onReviewReportedUsers();
    void onReviewReportedPosts();
    void loadAdminNotifications();
    void markAdminNotifsRead();
private:
    void loadReportedUsers();
    void loadReportedPosts();
    User** m_allUsers;
    int m_userCount;
    QScrollArea* m_scrollArea;
    QWidget* m_content;
    QVBoxLayout* m_layout;
};

// ─────────────────────────────────────────────────────────────
//  MainWindow
//  Application shell with sidebar navigation and page stack
// ─────────────────────────────────────────────────────────────
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
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
    void onSidebarCreatePost();

private:
    void buildSidebar();
    void buildPages();
    void tearDownShell();
    void setActiveSidebarButton(SidebarButton* active);

    // Root layout components
    AuthPage* m_authPage;
    QWidget* m_appShell;
    QHBoxLayout* m_shellLayout;
    QStackedWidget* m_rootStack;

    // Sidebar components
    QWidget* m_sidebar;
    SidebarButton* m_btnFeed;
    SidebarButton* m_btnNotifications;
    SidebarButton* m_btnSearch;
    SidebarButton* m_btnMessages;
    SidebarButton* m_btnProfile;
    SidebarButton* m_btnTimeSpent;
    SidebarButton* m_btnSwitchUser;
    SidebarButton* m_btnLogout;

    // Page stack
    QStackedWidget* m_pages;
    FeedPage* m_feedPage;
    NotificationsPage* m_notifPage;
    SearchPage* m_searchPage;
    MessagesPage* m_messagesPage = nullptr;
    ProfilePage* m_profilePage;
    TimeSpentPage* m_timeSpentPage;
    AdminPage* m_adminPage;

    // Current user data
    User* m_currentUser;
    User** m_allUsers;
    int    m_userCount;
};