#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "User.h"

class UpdateBioWindow : public QWidget {
    Q_OBJECT
public:
    UpdateBioWindow(User* user, QWidget* parent = nullptr);

private slots:
    void onSaveClicked();

private:
    User* user;
    QLineEdit* bioInput;
    QLabel* errorLabel;
};