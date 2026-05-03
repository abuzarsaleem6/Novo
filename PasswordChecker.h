#pragma once
#include <string>
#include <iostream>

using namespace std;

class PasswordChecker {

private:

    int minLength;

	// Helper functions 

    bool hasSpecialChar(const string& password);

    bool hasDigit(const string& password);

    bool hasUppercase(const string& password);

    bool hasLowercase(const string& password);

    bool hasNoSpaces(const string& password);

    bool hasNoPipe(const string& password);


public:
   
    PasswordChecker(int min = 8);

    bool checkAll(const string& password);

    int  calculateStrength(const string& password);

    string validatePassword(const string& password);

    string getValidationFeedback(const string& password);



};