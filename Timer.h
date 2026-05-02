#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool isRunning;

public:
    Timer();
    ~Timer() = default;

    void startSession();
    double getSessionDuration() const;
    void displaySessionTime() const;
    void saveSessionLog(const std::string& username) const;
};