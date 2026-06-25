// ============================================================
//  Admin.cpp  ->  Admin derived class implementation
// ============================================================

#include "../include/Admin.h"
#include "../include/FileManager.h"
#include "../include/AuthenticationManager.h"
#include "../include/LoginHistoryManager.h"

#include <iostream>
using namespace std;

// ============================================================
//  Helper: Pad a string with spaces to a fixed width (for display)
// ============================================================
static string padRight(const string& str, int width) {
    string result = str;
    while ((int)result.length() < width)
        result += ' ';
    return result;
}

// ---- Constructor (delegates to User base) ----
Admin::Admin(string uname, string pwd, string mail,
             string name, string sq, string sa, string date)
    : User(uname, pwd, mail, name, sq, sa, "admin", date)
{}

// ============================================================
//  displayProfile  ->  Overrides pure virtual from User
// ============================================================
void Admin::displayProfile() const {
    cout << "\n============================================\n";
    cout << "           ADMIN PROFILE\n";
    cout << "============================================\n";
    cout << "  Username      : " << username        << "\n";
    cout << "  Full Name     : " << fullName        << "\n";
    cout << "  Email         : " << email           << "\n";
    cout << "  Role          : ADMINISTRATOR\n";
    cout << "  Registered On : " << dateRegistered  << "\n";
    cout << "============================================\n";
}

// ============================================================
//  displayMenu  ->  Overrides pure virtual from User
// ============================================================
void Admin::displayMenu() {
    cout << "\n============================================\n";
    cout << "     ADMIN MENU  (Logged in as: " << username << ")\n";
    cout << "============================================\n";
    cout << "  1. View Profile\n";
    cout << "  2. View All Users\n";
    cout << "  3. Search User\n";
    cout << "  4. Delete User Account\n";
    cout << "  5. Reset User Password\n";
    cout << "  6. View All Login History\n";
    cout << "  7. Change My Password\n";
    cout << "  8. Logout\n";
    cout << "============================================\n";
    cout << "  Choice: ";
}

// ============================================================
//  viewAllUsers  ->  List every registered account
// ============================================================
void Admin::viewAllUsers() {
    User* users[MAX_USERS];
    int userCount = FileManager::loadAllUsers(users, MAX_USERS);

    cout << "\n============================================\n";
    cout << "         ALL REGISTERED USERS\n";
    cout << "============================================\n";

    if (userCount == 0) {
        cout << "  No users registered yet.\n";
        cout << "============================================\n";
        return;
    }

    // Column headers with manual padding
    cout << padRight("Username", 18)
         << padRight("Full Name", 22)
         << padRight("Role", 10)
         << "Registered On\n";

    // Separator line
    for (int i = 0; i < 68; i++) cout << '-';
    cout << "\n";

    for (int i = 0; i < userCount; i++) {
        cout << padRight(users[i]->getUsername(), 18)
             << padRight(users[i]->getFullName(), 22)
             << padRight(users[i]->getRole(), 10)
             << users[i]->getDateRegistered() << "\n";
    }

    cout << "============================================\n";
    cout << "Total users: " << userCount << "\n";
    FileManager::freeUserList(users, userCount);
}

// ============================================================
//  searchUser  ->  Find and display one user's details
// ============================================================
void Admin::searchUser() {
    cout << "\n  Enter username to search: ";
    string uname;
    cin  >> uname;

    User* user = FileManager::loadUser(uname);
    if (!user) {
        cout << "  [!] User '" << uname << "' not found.\n";
        return;
    }

    cout << "\n============================================\n";
    cout << "           USER DETAILS\n";
    cout << "============================================\n";
    cout << "  Username      : " << user->getUsername()        << "\n";
    cout << "  Full Name     : " << user->getFullName()        << "\n";
    cout << "  Email         : " << user->getEmail()           << "\n";
    cout << "  Role          : " << user->getRole()            << "\n";
    cout << "  Registered On : " << user->getDateRegistered()  << "\n";
    cout << "============================================\n";

    delete user;
}

// ============================================================
//  deleteUser  ->  Remove a user account permanently
// ============================================================
void Admin::deleteUser() {
    cout << "\n  Enter username to delete: ";
    string uname;
    cin  >> uname;

    if (uname == username) {
        cout << "  [!] You cannot delete your own admin account.\n";
        return;
    }

    if (!FileManager::userExists(uname)) {
        cout << "  [!] User '" << uname << "' not found.\n";
        return;
    }

    cout << "  Are you sure you want to delete '" << uname << "'? (y/n): ";
    char confirm;
    cin  >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        cout << "  Deletion cancelled.\n";
        return;
    }

    if (FileManager::deleteUser(uname))
        cout << "  [+] User '" << uname << "' deleted successfully.\n";
    else
        cout << "  [!] Failed to delete user.\n";
}

// ============================================================
//  resetUserPassword  ->  Admin resets any user's password
// ============================================================
void Admin::resetUserPassword() {
    cout << "\n  Enter username to reset password for: ";
    string uname;
    cin  >> uname;

    if (!FileManager::userExists(uname)) {
        cout << "  [!] User '" << uname << "' not found.\n";
        return;
    }

    AuthenticationManager::resetPassword(uname);
}

// ============================================================
//  viewAllLoginHistory  ->  Show every login event in the system
// ============================================================
void Admin::viewAllLoginHistory() {
    LoginHistoryManager::displayHistory("");   // empty = all users
}
