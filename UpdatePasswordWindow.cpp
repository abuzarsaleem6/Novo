#include "UpdatePasswordWindow.h"
#include <QVBoxLayout>

UpdatePasswordWindow::UpdatePasswordWindow(User* user, QWidget* parent)
    : QWidget(parent), user(user) {

    setWindowTitle("Update Password");
    setFixedSize(400, 200);

    QLabel* title = new QLabel("Update Password");
    title->setAlignment(Qt::AlignCenter);

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Enter new password (8-16 characters)");
    passwordInput->setEchoMode(QLineEdit::Password);

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* saveBtn = new QPushButton("Save");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(passwordInput);
    layout->addWidget(errorLabel);
    layout->addWidget(saveBtn);
    setLayout(layout);

    connect(saveBtn, &QPushButton::clicked, this, &UpdatePasswordWindow::onSaveClicked);
}

void UpdatePasswordWindow::onSaveClicked() {
    string newPassword = passwordInput->text().toStdString();

    if (newPassword.length() < 8 || newPassword.length() > 16) {
        errorLabel->setText("Password must be 8-16 characters.");
        return;
    }
    for (char c : newPassword) {
        if (c == '|' || c == ' ') {
            errorLabel->setText("Password cannot contain '|' or spaces.");
            return;
        }
    }

    user->setPassword(newPassword);
    user->saveToFile();
    errorLabel->setStyleSheet("color: green;");
    errorLabel->setText("Password updated successfully.");
}