#pragma once
#include <string>

using namespace std;

class PasswordChecker {
private:
    int minLength;
    bool hasSpecialChar(string password);
    bool hasDigit(string password);
    bool hasUppercase(string password);

public:
    PasswordChecker(int min = 8);
    bool checkAll(string password);
    int calculateStrength(string password);
};