#pragma once
#include <QWidget>
#include <QLabel>
#include "User.h"

class ProfileWindow : public QWidget {
    Q_OBJECT
public:
    ProfileWindow(User* user, QWidget* parent = nullptr);
};