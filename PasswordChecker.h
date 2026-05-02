// PasswordChecker.h
#pragma once
#include <string>
#include <iostream>

using namespace std;

class PasswordChecker {
private:
    int minLength;

    // ─── PRIVATE VALIDATION HELPERS ───
    bool hasSpecialChar(const string& password);
    bool hasDigit(const string& password);
    bool hasUppercase(const string& password);
    bool hasLowercase(const string& password);
    bool hasNoSpaces(const string& password);
    bool hasNoPipe(const string& password);

public:
    // ─── CONSTRUCTOR ───
    PasswordChecker(int min = 8);

    // ─── VALIDATION METHODS ───
    bool checkAll(const string& password);
    int  calculateStrength(const string& password);

    // ─── DETAILED VALIDATION ───
    // Returns error message if invalid, empty string if valid
    string validatePassword(const string& password);

    // Returns human-readable feedback for each requirement
    string getValidationFeedback(const string& password);
};