#include "DisplayPostsWindow.h"
#include "Post.h"
#include <QLabel>
#include <QFrame>

DisplayPostsWindow::DisplayPostsWindow(User* user, QWidget* parent)
    : QWidget(parent) {

    setWindowTitle("My Posts");
    setFixedSize(500, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    QLabel* title = new QLabel("Posts by " + QString::fromStdString(user->getUsername()));
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    if (user->getPostCount() == 0) {
        QLabel* noPost = new QLabel("No posts yet.");
        noPost->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(noPost);
    }
    else {
        QWidget* container = new QWidget();
        QVBoxLayout* containerLayout = new QVBoxLayout(container);

        for (int i = 0; i < user->getPostCount(); i++) {
            Posts* post = user->getPostByIndex(i);
            if (post == nullptr) continue;

            QFrame* card = new QFrame();
            card->setFrameShape(QFrame::Box);
            QVBoxLayout* cardLayout = new QVBoxLayout(card);

            cardLayout->addWidget(new QLabel("ID      : " +
                QString::fromStdString(post->getPostId())));
            cardLayout->addWidget(new QLabel("Content : " +
                QString::fromStdString(post->getContent())));
            cardLayout->addWidget(new QLabel("Likes   : " +
                QString::number(post->getLikeCount())));
            containerLayout->addWidget(card);
        }

        QScrollArea* scroll = new QScrollArea();
        scroll->setWidget(container);
        scroll->setWidgetResizable(true);
        mainLayout->addWidget(scroll);
    }

    setLayout(mainLayout);
}