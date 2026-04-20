#include "LikePostWindow.h"
#include "Post.h"
#include <QVBoxLayout>

LikePostWindow::LikePostWindow(User* loggedInUser, User**& allUsers, int& userCount,
    bool isLike, QWidget* parent)
    : QWidget(parent), loggedInUser(loggedInUser), allUsers(allUsers),
    userCount(userCount), isLike(isLike) {

    setWindowTitle(isLike ? "Like Post" : "Unlike Post");
    setFixedSize(400, 200);

    QLabel* title = new QLabel(isLike ? "Like a Post" : "Unlike a Post");
    title->setAlignment(Qt::AlignCenter);

    postIdInput = new QLineEdit();
    postIdInput->setPlaceholderText("Enter Post ID (e.g. P1000)");

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* actionBtn = new QPushButton(isLike ? "Like" : "Unlike");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(postIdInput);
    layout->addWidget(errorLabel);
    layout->addWidget(actionBtn);
    setLayout(layout);

    connect(actionBtn, &QPushButton::clicked, this, &LikePostWindow::onActionClicked);
}

void LikePostWindow::onActionClicked() {
    string postId = postIdInput->text().toStdString();

    if (postId.empty()) {
        errorLabel->setText("Please enter a Post ID.");
        return;
    }

    Posts* post = nullptr;
    for (int i = 0; i < userCount; i++) {
        post = allUsers[i]->getPostById(postId);
        if (post != nullptr) break;
    }

    if (post == nullptr) {
        errorLabel->setText("Post not found.");
        return;
    }

    if (isLike) {
        post->likePost();
        errorLabel->setStyleSheet("color: green;");
        errorLabel->setText("Post liked!");
    }
    else {
        post->unlikePost();
        errorLabel->setStyleSheet("color: green;");
        errorLabel->setText("Post unliked!");
    }
}