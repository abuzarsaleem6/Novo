#include "UpdateBioWindow.h"
#include <QVBoxLayout>

UpdateBioWindow::UpdateBioWindow(User* user, QWidget* parent)
    : QWidget(parent), user(user) {

    setWindowTitle("Update Bio");
    setFixedSize(400, 200);

    QLabel* title = new QLabel("Update Bio");
    title->setAlignment(Qt::AlignCenter);

    bioInput = new QLineEdit();
    bioInput->setPlaceholderText("Enter new bio (max 100 characters)");
    bioInput->setText(QString::fromStdString(user->getBio()));

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: red;");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* saveBtn = new QPushButton("Save");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(bioInput);
    layout->addWidget(errorLabel);
    layout->addWidget(saveBtn);
    setLayout(layout);

    connect(saveBtn, &QPushButton::clicked, this, &UpdateBioWindow::onSaveClicked);
}

void UpdateBioWindow::onSaveClicked() {
    string newBio = bioInput->text().toStdString();

    if (newBio.length() > 100) {
        errorLabel->setText("Bio must be less than 100 characters.");
        return;
    }
    for (char c : newBio) {
        if (c == '|') {
            errorLabel->setText("Bio cannot contain '|'.");
            return;
        }
    }

    user->setBio(newBio);
    user->saveToFile();
    errorLabel->setStyleSheet("color: green;");
    errorLabel->setText("Bio updated successfully.");
}