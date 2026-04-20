#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "User.h"

class UpdatePasswordWindow : public QWidget {
    Q_OBJECT
public:
    UpdatePasswordWindow(User* user, QWidget* parent = nullptr);

private slots:
    void onSaveClicked();

private:
    User* user;
    QLineEdit* passwordInput;
    QLabel* errorLabel;
};