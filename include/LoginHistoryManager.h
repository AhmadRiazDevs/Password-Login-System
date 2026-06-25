#ifndef LOGINHISTORYMANAGER_H
#define LOGINHISTORYMANAGER_H

// ============================================================
//  LoginHistoryManager.h  —  Login History Utility
//  OOP Concepts: Encapsulation, Abstraction
//  Responsibility: Recording and displaying login events
// ============================================================

#include <string>
using namespace std;

class LoginHistoryManager {
public:
    // Record a login attempt (success or failure) to disk
    static void recordLogin(const string& username, bool success);

    // Print history for a specific user (or all users if empty)
    static void displayHistory(const string& username = "");
};

#endif // LOGINHISTORYMANAGER_H
