#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <cstring>
#include <ctime>

class Timer {
private:
    char* ownerUsername;
    int startHour;
    int startMinute;
    int startSecond;
    int elapsedSeconds;
    bool isRunning;


    void allocateAndCopy(char*& dest, const char* source);

public:

    Timer(const char* username);
    Timer(const Timer& other);
    Timer& operator=(const Timer& other);
    ~Timer();


    void start();
    void stop();
    void reset();
    void update();


    void displayTimeSpent() const;
    int getElapsedSeconds() const;
    bool getIsRunning() const;
};

#endif