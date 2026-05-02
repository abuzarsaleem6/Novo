#include "Timer.h"
#include <iostream>
#include <fstream>

using namespace std;

Timer::Timer() : isRunning(false) {}

void Timer::startSession() {
    startTime = chrono::steady_clock::now();
    isRunning = true;
}

double Timer::getSessionDuration() const {
    if (!isRunning) return 0.0;

    auto currentTime = chrono::steady_clock::now();
    chrono::duration<double> elapsed = currentTime - startTime;
    return elapsed.count();
}

void Timer::displaySessionTime() const {
    double totalSeconds = getSessionDuration();
    int minutes = static_cast<int>(totalSeconds) / 60;
    int seconds = static_cast<int>(totalSeconds) % 60;

    cout << "Active Session Time: " << minutes << "m " << seconds << "s\n";
}

void Timer::saveSessionLog(const string& username) const {
    ofstream outFile("session_logs.txt", ios::app);

    if (outFile.is_open()) {
        double totalSeconds = getSessionDuration();
        int minutes = static_cast<int>(totalSeconds) / 60;
        int seconds = static_cast<int>(totalSeconds) % 60;

        outFile << "User: " << username << " | Session Time: "
            << minutes << "m " << seconds << "s\n";
    }
    else {
        cout << "Error: Could not open session_logs.txt to save the timer.\n";
    }
}