#include <iostream>
#include <string>
#include "PasswordChecker.h"
using namespace std;
PasswordChecker::PasswordChecker(int min) {
    this->minLength = min;
}
bool PasswordChecker::hasSpecialChar(string password) {
    for (int i = 0; i < password.length(); i++) {
        if (ispunct(password[i])) return true;
    }
    return false;
}
bool PasswordChecker::hasDigit(string password) {
    for (int i = 0; i < password.length(); i++) {
        if (isdigit(password[i])) return true;
    }
    return false;
}
bool PasswordChecker::hasUppercase(string password) {
    for (int i = 0; i < password.length(); i++) {
        if (isupper(password[i])) return true;
    }
    return false;
}
bool PasswordChecker::checkAll(string password) {
    if (password.length() < minLength) return false;
    if (hasSpecialChar(password) && hasDigit(password) && hasUppercase(password)) {
        return true;
    }
    return false;
}
int PasswordChecker::calculateStrength(string password) {
    int score = 0;
    if (password.length() >= minLength) score += 40;
    if (hasSpecialChar(password)) score += 20;
    if (hasDigit(password)) score += 20;
    if (hasUppercase(password)) score += 20;
    return score;
}