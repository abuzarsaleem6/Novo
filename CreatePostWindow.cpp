#include "CreatePostWindow.h"
#include <QVBoxLayout>

CreatePostWindow::CreatePostWindow(User* user, QWidget* parent)
    : QWidget(parent), user(user) {

    setWindowTitle("Create Post");
    setFixedSize(400, 300);

    QLabel* title = new QLabel("Create a Post");
    title->setAlignment(Qt::AlignCenter);

    contentInput = new QTextEdit();
    contentInput->setPlaceholderText("Write your post here (no '|' allowed)");

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* postBtn = new QPushButton("Post");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(contentInput);
    layout->addWidget(errorLabel);
    layout->addWidget(postBtn);
    setLayout(layout);

    connect(postBtn, &QPushButton::clicked, this, &CreatePostWindow::onPostClicked);
}

void CreatePostWindow::onPostClicked() {
    string content = contentInput->toPlainText().toStdString();

    if (content.empty()) {
        errorLabel->setText("Post content cannot be empty.");
        return;
    }
    for (char c : content) {
        if (c == '|') {
            errorLabel->setText("Post cannot contain '|'.");
            return;
        }
    }

    user->createPost(content);
    errorLabel->setStyleSheet("color: green;");
    errorLabel->setText("Post created successfully!");
    contentInput->clear();
}