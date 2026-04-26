// PasswordChecker.h
#pragma once
#include <string>
#include <QString>
#include <QDebug>

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
    // Returns true if password meets ALL requirements
    bool checkAll(const string& password);

    // Returns strength score (0-100)
    int calculateStrength(const string& password);

    // ─── DETAILED VALIDATION FOR Qt GUI ───
    // Returns detailed error message if validation fails, empty string if valid
    QString validatePassword(const string& password);

    // Get human-readable feedback for each requirement
    QString getValidationFeedback(const string& password);
};