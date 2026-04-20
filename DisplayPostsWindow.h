#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "User.h"

class DisplayPostsWindow : public QWidget {
    Q_OBJECT
public:
    DisplayPostsWindow(User* user, QWidget* parent = nullptr);
};