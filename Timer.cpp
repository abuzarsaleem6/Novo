#define _CRT_SECURE_NO_WARNINGS
#include "Timer.h"
#include <iostream>
#include <fstream>

using namespace std;

Timer::Timer() {
    isRunning = false;
}

Timer::~Timer() {

}

void Timer::startSession() {
    startTime = chrono::steady_clock::now();
    isRunning = true;
}

double Timer::getSessionDuration() const {
    if (!isRunning) {
        return 0.0;
    }

    auto currentTime = chrono::steady_clock::now();
    chrono::duration<double> elapsedSeconds = currentTime - startTime;

    return elapsedSeconds.count();
}

void Timer::displaySessionTime() const {
    double totalSeconds = getSessionDuration();

    int minutes = static_cast<int>(totalSeconds) / 60;
    int seconds = static_cast<int>(totalSeconds) % 60;

    cout << "Active Session Time: " << minutes << "m " << seconds << "s\n";
}

void Timer::saveSessionLog(const char* username) const {
    ofstream outFile("session_logs.txt", ios::app);

    if (outFile.is_open()) {
        double totalSeconds = getSessionDuration();

        int minutes = static_cast<int>(totalSeconds) / 60;
        int seconds = static_cast<int>(totalSeconds) % 60;

        outFile << "User: " << username << " | Session Time: " << minutes << "m " << seconds << "s\n";

        outFile.close();
    }
    else {
        cout << "Error: Could not open session_logs.txt to save the timer.\n";
    }
}