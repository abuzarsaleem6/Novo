#include "Timer.h"
using namespace std;

Timer::Timer() {
    isRunning = false;
}

Timer::~Timer() {
    
}


void Timer::startSession() {
   
    startTime = std::chrono::steady_clock::now();
    isRunning = true;
}

double Timer::getSessionDuration() const {
    if (!isRunning) {
        return 0.0;
    }

    
    auto currentTime = std::chrono::steady_clock::now();
    chrono::duration<double> elapsedSeconds = currentTime - startTime;

    return elapsedSeconds.count();
}

void Timer::displaySessionTime() const {
    double totalSeconds = getSessionDuration();

   
    int minutes = static_cast<int>(totalSeconds) / 60;
    int seconds = static_cast<int>(totalSeconds) % 60;

    cout << "Active Session Time: " << minutes << "m " << seconds << "s\n";
}