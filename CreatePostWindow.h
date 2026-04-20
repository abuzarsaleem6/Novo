#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include "User.h"

class CreatePostWindow : public QWidget {
    Q_OBJECT
public:
    CreatePostWindow(User* user, QWidget* parent = nullptr);

private slots:
    void onPostClicked();

private:
    User* user;
    QTextEdit* contentInput;
    QLabel* errorLabel;
};