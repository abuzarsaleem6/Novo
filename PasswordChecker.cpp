// PasswordChecker.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cctype>
#include "PasswordChecker.h"

using namespace std;


// Constructor
PasswordChecker::PasswordChecker(int min) {

    this->minLength = min;
    cout << "PasswordChecker initialized with minimum length: " << min << endl;

}


bool PasswordChecker::hasSpecialChar(const string& password) {

    for (int i = 0; i < password.length(); i++) {

        if (ispunct(password[i])) {

            return true;

        }
    }

    return false;

}

bool PasswordChecker::hasDigit(const string& password) {

    for (int i = 0; i < password.length(); i++) {

        if (isdigit(password[i])) {

            return true;

        }
    }

    return false;

}

bool PasswordChecker::hasUppercase(const string& password) {

    for (int i = 0; i < password.length(); i++) {

        if (isupper(password[i])) {

            return true;

        }
    }

    return false;

}

bool PasswordChecker::hasLowercase(const string& password) {

    for (int i = 0; i < password.length(); i++) {

        if (islower(password[i])) {

            return true;

        }
    }

    return false;

}

bool PasswordChecker::hasNoSpaces(const string& password) {

    for (int i = 0; i < password.length(); i++) {

        if (isspace(password[i])) {

            return false;

        }
    }

    return true;
}

bool PasswordChecker::hasNoPipe(const string& password) {

    for (int i = 0; i < password.length(); i++) {

        if (password[i] == '|') {

            return false;

        }
    }

    return true;

}


bool PasswordChecker::checkAll(const string& password) {

    cout << "Checking password strength..." << endl;

    if (password.length() < minLength) {

        cout << "FAIL: Password too short. Required: " << minLength << " Got: " << password.length() << endl;
        return false;

    }

    if (!hasNoSpaces(password)) {

        cout << "FAIL: Password contains spaces" << endl;
        return false;

    }

    if (!hasNoPipe(password)) {

        cout << "FAIL: Password contains '|' character" << endl;
        return false;

    }

    bool hasSpecial = hasSpecialChar(password);
    bool hasNum = hasDigit(password);
    bool hasUpper = hasUppercase(password);

    cout << "Special char: " << hasSpecial << " Digit: " << hasNum << " Uppercase: " << hasUpper << endl;

    if (!hasSpecial || !hasNum || !hasUpper) {

        cout << "FAIL: Missing required character types" << endl;
        return false;

    }

    cout << "PASS: Password is strong!" << endl;
    return true;

}

int PasswordChecker::calculateStrength(const string& password) {

    int score = 0;

    if (password.length() >= minLength) {

        score += 40;

    }
    else if (password.length() >= minLength - 2) {

        score += 20;

    }

    if (hasSpecialChar(password)) {
        score += 20;
}

    if (hasDigit(password)) {
        score += 20;
    }

    if (hasUppercase(password)) {
        score += 20;
    }

    if (hasLowercase(password)) {
        score += 5;
    }

    if (score > 100) {
        score = 100;
    }

    cout << "Password strength calculated: " << score << "/ 100" << endl;
    return score;

}

string PasswordChecker::validatePassword(const string& password) {

    if (password.empty()) {
        return "Password cannot be empty.";
    }

    if (password.length() < minLength) {
        return "Password must be at least " + to_string(minLength)
            + " characters. Current: " + to_string(password.length());
    }

    if (password.length() > 50) {
        return "Password cannot exceed 50 characters.";
    }

    if (!hasNoSpaces(password)) {
        return "Password cannot contain spaces.";
    }

    if (!hasNoPipe(password)) {
        return "Password cannot contain '|' character.";
    }

    if (!hasUppercase(password)) {
        return "Password must contain at least one UPPERCASE letter.";
    }

    if (!hasDigit(password)) {
        return "Password must contain at least one digit (0-9).";
    }

    if (!hasSpecialChar(password)) {
        return "Password must contain at least one special character (!@#$%^&*).";
    }

    cout << "Password validation PASSED" << endl;
    return "";  
}

string PasswordChecker::getValidationFeedback(const string& password) {

    string feedback = "Password Requirements:\n\n";

    if (password.length() >= minLength) {
        feedback += "✓ At least " + to_string(minLength) + " characters\n";
    }

    else {
        feedback += "✗ At least " + to_string(minLength) + " characters (Current: "
            + to_string(password.length()) + ")\n";
    }

    if (hasUppercase(password)) {
        feedback += "✓ Contains UPPERCASE letter\n";
    }

    else {
        feedback += "✗ Contains UPPERCASE letter\n";
    }

    if (hasDigit(password)) {
        feedback += "✓ Contains digit (0-9)\n";
    }

    else {
        feedback += "✗ Contains digit (0-9)\n";
    }

    if (hasSpecialChar(password)) {
        feedback += "✓ Contains special character (!@#$%^&*)\n";
    }

    else {
        feedback += "✗ Contains special character (!@#$%^&*)\n";
    }

    if (hasNoSpaces(password)) {
        feedback += "✓ No spaces\n";
    }

    else {
        feedback += "✗ No spaces\n";
    }

    if (hasNoPipe(password)) {
        feedback += "✓ No pipe character (|)\n";
    }

    else {
        feedback += "✗ No pipe character (|)\n";
    }

    int strength = calculateStrength(password);
    feedback += "\nStrength: " + to_string(strength) + "/100\n";

    return feedback;
}