#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

// ============================================================
//  AuthenticationManager.h  —  Auth Logic Class
//  OOP Concepts: Encapsulation, Abstraction
//  Responsibility: Registration, login, password validation
// ============================================================

#include <string>
#include "User.h"
using namespace std;

class AuthenticationManager {
private:
    // Static helper — validates a password against all rules
    static bool isValidPassword(const string& pwd, string& errorMsg);

    // Hides the details of prompting/collecting a secure password
    static string promptNewPassword();

public:
    // ---- Core Auth Operations ----
    static User* registerUser();        // Returns new User* (caller owns)
    static User* loginUser();           // Returns logged-in User* or nullptr
    static bool  forgotPassword();      // Security-question reset flow

    // ---- Password Utilities ----
    static bool changePassword(User* user);
    static bool resetPassword(const string& username);  // Admin-only

    // ---- Display ----
    static void showPasswordRules();
};

#endif // AUTHENTICATIONMANAGER_H
