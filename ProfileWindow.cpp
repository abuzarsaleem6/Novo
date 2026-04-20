#include "ProfileWindow.h"
#include <QVBoxLayout>

ProfileWindow::ProfileWindow(User* user, QWidget* parent)
    : QWidget(parent) {

    setWindowTitle("Profile");
    setFixedSize(400, 300);

    QLabel* title = new QLabel("Profile");
    title->setAlignment(Qt::AlignCenter);

    QLabel* username = new QLabel("Username : " + QString::fromStdString(user->getUsername()));
    QLabel* bio = new QLabel("Bio       : " + QString::fromStdString(user->getBio()));
    QLabel* following = new QLabel("Following : " + QString::number(user->getFollowingCount()));
    QLabel* followers = new QLabel("Followers : " + QString::number(user->getFollowersCount()));
    QLabel* posts = new QLabel("Posts     : " + QString::number(user->getPostCount()));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(username);
    layout->addWidget(bio);
    layout->addWidget(following);
    layout->addWidget(followers);
    layout->addWidget(posts);
    setLayout(layout);
}