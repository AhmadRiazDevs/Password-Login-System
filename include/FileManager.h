#ifndef FILEMANAGER_H
#define FILEMANAGER_H

// ============================================================
//  FileManager.h  —  File Handling Utility Class
//  OOP Concepts: Encapsulation, Abstraction
//  Responsibility: All disk read/write operations
// ============================================================

#include <string>
#include "User.h"
using namespace std;

// Maximum number of users the system can handle at once
const int MAX_USERS   = 500;
const int MAX_HISTORY = 1000;

class FileManager {
private:
    // File paths (encapsulated — callers never hard-code paths)
    static const string USERS_FILE;
    static const string HISTORY_FILE;

public:
    // ---- User File Operations ----
    static bool   saveUser(const User& user);
    static bool   userExists(const string& username);
    static User*  loadUser(const string& username);   // caller owns ptr

    // Load all users into a caller-provided array; returns count
    static int    loadAllUsers(User** arr, int maxSize);

    static bool   updateUser(const User& user);
    static bool   deleteUser(const string& username);

    // ---- Login History Operations ----
    static bool   appendHistory(const string& username,
                                const string& status,
                                const string& timestamp);

    // Load history into a caller-provided array; returns count
    static int    loadHistory(string* arr, int maxSize,
                              const string& username = "");

    // ---- Helpers ----
    static string getCurrentTimestamp();
    static void   freeUserList(User** arr, int count);
};

#endif // FILEMANAGER_H
