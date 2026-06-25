 // ============================================================
//  RegularUser.cpp  ->  RegularUser derived class implementation
// ============================================================

#include "../include/RegularUser.h"
#include "../include/FileManager.h"
#include "../include/AuthenticationManager.h"
#include "../include/LoginHistoryManager.h"

#include <iostream>
using namespace std;

//  Constructor 
RegularUser::RegularUser(string uname, string pwd, string mail,
                         string name, string sq, string sa, string date)
    : User(uname, pwd, mail, name, sq, sa, "user", date)
{}

// ============================================================
//  displayProfile  ->  Overrides pure virtual from User
// ============================================================
void RegularUser::displayProfile() const {
    cout << "\n============================================\n";
    cout << "           MY PROFILE\n";
    cout << "============================================\n";
    cout << "  Username      : " << username        << "\n";
    cout << "  Full Name     : " << fullName        << "\n";
    cout << "  Email         : " << email           << "\n";
    cout << "  Role          : Regular User\n";
    cout << "  Registered On : " << dateRegistered  << "\n";
    cout << "============================================\n";
}

// ============================================================
//  displayMenu  ->  Overrides pure virtual from User
// ============================================================
void RegularUser::displayMenu() {
    cout << "\n============================================\n";
    cout << "     USER MENU  (Logged in as: " << username << ")\n";
    cout << "============================================\n";
    cout << "  1. View Profile\n";
    cout << "  2. Change Password\n";
    cout << "  3. Update Profile\n";
    cout << "  4. View My Login History\n";
    cout << "  5. Logout\n";
    cout << "============================================\n";
    cout << "  Choice: ";
}

// ============================================================
//  changePassword
// ============================================================
void RegularUser::changePassword() {
    AuthenticationManager::changePassword(this);
}

// ============================================================
//  updateProfile  ->  Let the user edit their name and email
// ============================================================
void RegularUser::updateProfile() {
    cout << "\n============================================\n";
    cout << "           UPDATE PROFILE\n";
    cout << "============================================\n";
    cout << "  (Press Enter to keep current value)\n\n";

    cin.ignore(10000, '\n');

    cout << "  Current Full Name : " << fullName << "\n";
    cout << "  New Full Name     : ";
    string newName;
    getline(cin, newName);
    if (!newName.empty()) setFullName(newName);

    cout << "  Current Email     : " << email << "\n";
    cout << "  New Email         : ";
    string newEmail;
    getline(cin, newEmail);
    if (!newEmail.empty()) setEmail(newEmail);

    if (FileManager::updateUser(*this))
        cout << "\n  [+] Profile updated successfully!\n";
    else
        cout << "\n  [!] Failed to save profile changes.\n";
}

// ============================================================
//  viewPersonalLoginHistory
// ============================================================
void RegularUser::viewPersonalLoginHistory() {
    LoginHistoryManager::displayHistory(username);
}
