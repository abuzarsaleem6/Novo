// PasswordChecker.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cctype>
#include "PasswordChecker.h"

using namespace std;

// ══════════════════════════════════════════════════════════════════════════════
//  CONSTRUCTOR
// ══════════════════════════════════════════════════════════════════════════════

PasswordChecker::PasswordChecker(int min) {
    this->minLength = min;
    qDebug() << "PasswordChecker initialized with minimum length:" << min;
}

// ══════════════════════════════════════════════════════════════════════════════
//  PRIVATE VALIDATION HELPERS
// ══════════════════════════════════════════════════════════════════════════════

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

// ══════════════════════════════════════════════════════════════════════════════
//  PUBLIC VALIDATION METHODS - USED BY Qt GUI
// ══════════════════════════════════════════════════════════════════════════════

bool PasswordChecker::checkAll(const string& password) {
    qDebug() << "Checking password strength...";

    // Check length
    if (password.length() < minLength) {
        qDebug() << "FAIL: Password too short. Required:" << minLength
            << "Got:" << password.length();
        return false;
    }

    // Check for spaces
    if (!hasNoSpaces(password)) {
        qDebug() << "FAIL: Password contains spaces";
        return false;
    }

    // Check for pipe character
    if (!hasNoPipe(password)) {
        qDebug() << "FAIL: Password contains '|' character";
        return false;
    }

    // Check for required characters
    bool hasSpecial = hasSpecialChar(password);
    bool hasNum = hasDigit(password);
    bool hasUpper = hasUppercase(password);

    qDebug() << "Special char:" << hasSpecial << "Digit:" << hasNum << "Uppercase:" << hasUpper;

    // ALL MUST BE TRUE
    if (!hasSpecial || !hasNum || !hasUpper) {
        qDebug() << "FAIL: Missing required character types";
        return false;
    }

    qDebug() << "PASS: Password is strong!";
    return true;
}

int PasswordChecker::calculateStrength(const string& password) {
    int score = 0;

    // Length score (40 points max)
    if (password.length() >= minLength) {
        score += 40;
    }
    else if (password.length() >= minLength - 2) {
        score += 20;
    }

    // Variety score (20 points each)
    if (hasSpecialChar(password)) score += 20;
    if (hasDigit(password)) score += 20;
    if (hasUppercase(password)) score += 20;

    // Bonus for lowercase
    if (hasLowercase(password)) score += 5;

    // Cap at 100
    score = (score > 100) ? 100 : score;

    qDebug() << "Password strength calculated:" << score << "/ 100";
    return score;
}

QString PasswordChecker::validatePassword(const string& password) {
    // Returns empty string if VALID, or error message if INVALID

    if (password.empty()) {
        return "Password cannot be empty.";
    }

    if (password.length() < minLength) {
        return QString("Password must be at least %1 characters. Current: %2")
            .arg(minLength).arg(password.length());
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

    // All validations passed
    qDebug() << "Password validation PASSED";
    return "";  // Empty string = valid
}

QString PasswordChecker::getValidationFeedback(const string& password) {
    // Returns detailed feedback about what's missing/good

    QString feedback = "Password Requirements:\n\n";

    // Length
    if (password.length() >= minLength) {
        feedback += "✓ At least " + QString::number(minLength) + " characters\n";
    }
    else {
        feedback += "✗ At least " + QString::number(minLength) + " characters (Current: "
            + QString::number(password.length()) + ")\n";
    }

    // Uppercase
    if (hasUppercase(password)) {
        feedback += "✓ Contains UPPERCASE letter\n";
    }
    else {
        feedback += "✗ Contains UPPERCASE letter\n";
    }

    // Digit
    if (hasDigit(password)) {
        feedback += "✓ Contains digit (0-9)\n";
    }
    else {
        feedback += "✗ Contains digit (0-9)\n";
    }

    // Special character
    if (hasSpecialChar(password)) {
        feedback += "✓ Contains special character (!@#$%^&*)\n";
    }
    else {
        feedback += "✗ Contains special character (!@#$%^&*)\n";
    }

    // No spaces
    if (hasNoSpaces(password)) {
        feedback += "✓ No spaces\n";
    }
    else {
        feedback += "✗ No spaces\n";
    }

    // No pipe
    if (hasNoPipe(password)) {
        feedback += "✓ No pipe character (|)\n";
    }
    else {
        feedback += "✗ No pipe character (|)\n";
    }

    // Strength
    int strength = calculateStrength(password);
    feedback += "\nStrength: " + QString::number(strength) + "/100\n";

    return feedback;
}