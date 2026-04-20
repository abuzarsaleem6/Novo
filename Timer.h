#pragma once
#include <iostream>
#include <chrono>

class Timer {
private:
   
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool isRunning;

public:
   
    Timer();
    ~Timer(); 

  
    void startSession();
    double getSessionDuration() const;
    void displaySessionTime() const;  
};