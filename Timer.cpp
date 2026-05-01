#define _CRT_SECURE_NO_WARNINGS
#include "Timer.h"

using namespace std;


void Timer::allocateAndCopy(char*& dest, const char* source) {
    if (source != nullptr) {
        dest = new char[strlen(source) + 1];
        strcpy(dest, source);
    }
    else {
        dest = new char[1];
        dest[0] = '\0';
    }
}


Timer::Timer(const char* username) {
    ownerUsername = nullptr;
    allocateAndCopy(ownerUsername, username);
    startHour = 0;
    startMinute = 0;
    startSecond = 0;
    elapsedSeconds = 0;
    isRunning = false;
}

Timer::Timer(const Timer& other) {
    ownerUsername = nullptr;
    allocateAndCopy(ownerUsername, other.ownerUsername);
    startHour = other.startHour;
    startMinute = other.startMinute;
    startSecond = other.startSecond;
    elapsedSeconds = other.elapsedSeconds;
    isRunning = other.isRunning;
}

Timer& Timer::operator=(const Timer& other) {
    if (this != &other) {
        delete[] ownerUsername;
        allocateAndCopy(ownerUsername, other.ownerUsername);
        startHour = other.startHour;
        startMinute = other.startMinute;
        startSecond = other.startSecond;
        elapsedSeconds = other.elapsedSeconds;
        isRunning = other.isRunning;
    }
    return *this;
}

Timer::~Timer() {
    delete[] ownerUsername;
}


void Timer::start() {
    if (!isRunning) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        startHour = ltm->tm_hour;
        startMinute = ltm->tm_min;
        startSecond = ltm->tm_sec;
        isRunning = true;
    }
}

void Timer::stop() {
    isRunning = false;
    update(); 
}

void Timer::reset() {
    elapsedSeconds = 0;
    isRunning = false;
}

void Timer::update() {
    if (isRunning) {
        time_t now = time(0);
        tm* ltm = localtime(&now);

        int currentHour = ltm->tm_hour;
        int currentMinute = ltm->tm_min;
        int currentSecond = ltm->tm_sec;

      
        int startInSeconds = startHour * 3600 + startMinute * 60 + startSecond;
        int currentInSeconds = currentHour * 3600 + currentMinute * 60 + currentSecond;

      
        if (currentInSeconds < startInSeconds) {
            currentInSeconds += 24 * 3600;
        }

        elapsedSeconds = currentInSeconds - startInSeconds;
    }
}


void Timer::displayTimeSpent() const {
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    cout << "Session Time for " << (ownerUsername ? ownerUsername : "Unknown")
        << ": " << minutes << "m " << seconds << "s\n";
}

int Timer::getElapsedSeconds() const {
    return elapsedSeconds;
}

bool Timer::getIsRunning() const {
    return isRunning;
}