#pragma once
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include "User.h"

class SplashScreen : public QWidget {
    Q_OBJECT
public:
    SplashScreen(User**& allUsers, int& userCount, QWidget* parent = nullptr);

private slots:
    void onAnimationDone();

private:
    User**& allUsers;
    int& userCount;
    QLabel* logoLabel;
    QTimer* timer;
};