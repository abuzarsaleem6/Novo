#pragma once

#include <QtWidgets/QMainWindow>
#include "User.h"
#include "Post.h"
#include "Comment.h"
#include "Feed.h"
#include "Admin.h"
#include "PasswordChecker.h"
#include "Notification.h"
#include "Message.h"      
#include <QMap>       
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
#include <QDialog>
#include <QInputDialog>

class User;
class Posts;
class Notification;

class SidebarButton : public QPushButton {

    Q_OBJECT

public:

    SidebarButton(const QString& icon, const QString& label, QWidget* parent = nullptr);
    void setActive(bool active);

};

class PostCard : public QFrame {

    Q_OBJECT

public:

    void updateSaveStatus(bool isSaved);
    PostCard(Posts* post, const QString& authorUsername, bool isOwner, bool isSaved, const QString& viewerUsername, bool isAuthorReported = false, bool isPostReported = false, QWidget* parent = nullptr);
signals:
   
    void likeClicked(Posts* post);
    void commentClicked(Posts* post);
    void editClicked(Posts* post);
    void deleteClicked(Posts* post);
    void reportUserClicked(const QString& username);
    void saveClicked(Posts* post, const QString& ownerUsername);
    void unsaveClicked(Posts* post);
    void reportClicked(Posts* post, const QString& ownerUsername);

private:
   
    Posts* m_post;
    QString m_authorUsername;
    bool    m_isOwner;
    bool    m_isSaved;
    QString m_viewerUsername;
    bool    m_isAuthorReported; 
    bool    m_isPostReported;   
    QPushButton* m_saveBtn = nullptr;
    QLabel* m_saveLbl = nullptr;

};

class NotificationItem : public QFrame {

    Q_OBJECT

public:

    NotificationItem(const Notification& notif, QWidget* parent = nullptr);

};

class AuthPage : public QWidget {

    Q_OBJECT

public:

    explicit AuthPage(QWidget* parent = nullptr);
    ~AuthPage();
    void resetToLogin();

    User** getAllUsers() const { 
        return m_allUsers; 
    }

    int getUserCount() const { 
        return m_userCount;
    }

signals:
    void loginSuccess(User* user, User** allUsers, int userCount);
    void loginAdminSuccess(User** allUsers, int userCount);

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
    QLineEdit* m_adminUser;
    QLineEdit* m_adminPass;
    User** m_allUsers;
    int        m_userCount;

};

class PublicProfileWidget : public QWidget {

    Q_OBJECT

public:

    PublicProfileWidget(QWidget* parent = nullptr);
    void loadProfile(User* targetUser, User* viewer, User** allUsers, int userCount);
    void refresh(); 

signals:
    void backClicked();
    void requestOpenComments(Posts* post);

private:
    QVBoxLayout* m_mainLayout;
    User** m_allUsers = nullptr;
    int m_userCount = 0;

    
    User* m_targetUser = nullptr; 
    User* m_viewer = nullptr;    

};

class FeedPage : public QWidget {

    Q_OBJECT

public:

    FeedPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();
    QFrame* m_composerCard;
    QTextEdit* m_postInput;

signals:
    void requestOpenComments(Posts* post); 
private slots:
    void onLikePost(Posts* post);
    void onCommentPost(Posts* post);
    void onDeletePost(Posts* post);
    void onSavePost(Posts* post, const QString& ownerUsername);
    void onUnsavePost(Posts* post);
    void onReportPost(Posts* post, const QString& ownerUsername);
    void onSubmitPost();
    void onReportUserFromPost(const QString& username);

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

class MyPostsPage : public QWidget {

    Q_OBJECT

public:

    MyPostsPage(User* currentUser, QWidget* parent = nullptr);
    void refresh();

signals:
    void requestOpenComments(Posts* post); 

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

class SavedPostsPage : public QWidget {

    Q_OBJECT

public:

    SavedPostsPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();

signals:
    void requestOpenComments(Posts* post); 

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

class CreatePostPage : public QWidget {

    Q_OBJECT

public:

    explicit CreatePostPage(User* currentUser, QWidget* parent = nullptr);
    void focusInput();

signals:

    void postPublished();

private slots:

    void onSubmit();

private:

    User* m_user = nullptr;
    QTextEdit* m_postInput = nullptr;
    QLabel* m_charCount = nullptr;

};

class ProfilePage : public QWidget {

    Q_OBJECT

public:

    ProfilePage(User* currentUser, User**& allUsers, int* userCount, QWidget* parent = nullptr);
    void refresh();

signals:

    void accountDeleted();
    void requestOpenComments(Posts* post); 

private slots:

    void onUpdateBio();
    void onUpdatePassword();
    void onDeleteAccount();

private:

    User* m_user;
    User**& m_allUsers;
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


class CommentsPage : public QWidget {

    Q_OBJECT

public:

    CommentsPage(const QString& currentUser, QWidget* parent = nullptr);
    void loadPost(Posts* post);

signals:

    void backClicked();

private slots:

    void onAddComment();
    void onEditComment(const QString& commentId);   
    void onDeleteComment(const QString& commentId);

private:

    void loadComments();
    Posts* m_post;
    QString      m_currentUser;
    QVBoxLayout* m_commentsLayout;
    QWidget* m_commentsContainer;
    QLineEdit* m_input;

};

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

class ChatView : public QWidget {

    Q_OBJECT

public:

    ChatView(const QString& currentUser, const QString& peerUsername, QWidget* parent = nullptr);
    void refresh();

signals:

    void chatDeleted(const QString& peer);

private slots:

    void onSend();
    void onDeleteChat();
    void markConversationUnread(const QString& receiver, const QString& sender);

private:

    QString   chatFilePath() const;
    void      loadMessages();
    void      clearMessages();
    void      appendBubble(const QString& sender, const QString& text);
    QString       m_currentUser;
    QString       m_peer;
    QScrollArea* m_scroll;
    QWidget* m_bubbleContainer;
    QVBoxLayout* m_bubbleLayout;
    QLineEdit* m_input;

};

class SearchPage : public QWidget {

    Q_OBJECT

public:
    SearchPage(User**& allUsers, int& userCount, User* currentUser, QWidget* parent = nullptr);
    
signals:

    void requestOpenComments(Posts* post);
    void requestViewProfile(User* user);

private slots:

    void onSearch();
  
private:

    void showUserCard(User* user);
    void clearResults();

    SearchEngine m_engine;
    User* m_currentUser;
    User** &m_allUsers;
    int & m_userCount;
    QLineEdit* m_searchInput;
    QScrollArea* m_scrollArea;
    QWidget* m_resultsContent;
    QVBoxLayout* m_resultsLayout;
    
};
class MessagesPage : public QWidget {

    Q_OBJECT

public:

    MessagesPage(User* currentUser, User** allUsers, int userCount, QWidget* parent = nullptr);
    void refresh();
    void openChatWith(const QString& peer);

private slots:
    void onSearchUser();
    void onConversationSelected(const QString& peer);
    void onChatDeleted(const QString& peer);
   
private:
    void loadConversationList();
    void clearConversationList();
    void addConversationButton(const QString& peer);

    User* m_currentUser;
    User** m_allUsers;
    int m_userCount;
    QWidget* m_leftPanel;
    QLineEdit* m_searchInput;
    QWidget* m_convListContent;
    QVBoxLayout* m_convListLayout;
    QStackedWidget* m_rightStack;
    ChatView* m_chatView;
    QString m_activePeer;

   
};

class AdminPage : public QWidget {

    Q_OBJECT

public:
    AdminPage(Admin* admin, User**& allUsers, int& userCount, QWidget* parent = nullptr);  // ← & on both
    void refresh();

private slots:
    void onReviewReportedUsers();
    void onReviewReportedPosts();
    void loadAdminNotifications();

private:
    void loadReportedUsers();
    void loadReportedPosts();
    Admin* m_admin;
    User**& m_allUsers;  
    int& m_userCount;    
    QScrollArea* m_scrollArea;
    QLabel* m_reportedUsersStatLbl = nullptr;
    QLabel* m_reportedPostsStatLbl = nullptr;
    QLabel* m_totalUsersStatLbl = nullptr;
    QWidget* m_content;
    QVBoxLayout* m_layout;

};
class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(User* user, User** allUsers, int userCount);
    void onLoginAdminSuccess(User** allUsers, int userCount);
    void onNavFeed();
    void onNavNotifications();
    void onNavSearch();
    void onNavMessages();
    void onNavProfile();
    void onNavTimeSpent();
    void onLogout();
    
    void onSidebarCreatePost();
    void onOpenComments(Posts* post); 

private:
    void buildSidebar();
    void buildPages();
    void tearDownShell();
    void setActiveSidebarButton(SidebarButton* active);

    AuthPage* m_authPage;
    QWidget* m_appShell;
    QHBoxLayout* m_shellLayout;
    QStackedWidget* m_rootStack;

    QWidget* m_sidebar;
    SidebarButton* m_btnFeed;
    SidebarButton* m_btnCreatePost = nullptr;
    SidebarButton* m_btnNotifications;
    SidebarButton* m_btnSearch;
    SidebarButton* m_btnMessages;
    SidebarButton* m_btnProfile;
    SidebarButton* m_btnTimeSpent;
    
    SidebarButton* m_btnLogout;

    QStackedWidget* m_pages;
    FeedPage* m_feedPage;
    NotificationsPage* m_notifPage;
    SearchPage* m_searchPage;
    MessagesPage* m_messagesPage = nullptr;
    ProfilePage* m_profilePage;
    TimeSpentPage* m_timeSpentPage;
    AdminPage* m_adminPage;
    CommentsPage* m_commentsPage = nullptr; 
    CreatePostPage* m_createPostPage = nullptr;
    PublicProfileWidget* m_publicProfilePage = nullptr;
    int m_previousPageIndex = 0; 

    User* m_currentUser;
    User** m_allUsers;
    int    m_userCount;
};