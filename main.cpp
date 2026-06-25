// ============================================================
//  main.cpp  ->  Entry Point & Menu Driver
//  Password Login System -> OOP Project
//  2nd Semester, C++ OOP Course
// ============================================================

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

#include "include/User.h"
#include "include/Admin.h"
#include "include/RegularUser.h"
#include "include/AuthenticationManager.h"
#include "include/FileManager.h"
#include "include/LoginHistoryManager.h"

// Platform-specific directory creation
#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #define MKDIR(dir) mkdir(dir, 0755)
#endif

// ---- Forward declarations ----
void showMainMenu();
void runAdminSession(Admin* admin);
void runUserSession(RegularUser* user);
void printBanner();
void clearScreen();
void pressEnterToContinue();

//  main
int main() {
    // Ensure the data directory exists
    MKDIR("data");

    printBanner();

    bool running = true;
    while (running) {
        showMainMenu();

        int choice;
        cin >> choice;

        // Guard against bad input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n  [!] Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }

        switch (choice) {
            // Register
            case 1: {
                User* newUser = AuthenticationManager::registerUser();
                if (newUser) {
                    // Immediately start a session after registration
                    if (newUser->getRole() == "admin")
                        runAdminSession(static_cast<Admin*>(newUser));
                    else
                        runUserSession(static_cast<RegularUser*>(newUser));
                    delete newUser;
                }
                break;
            }

            // ---- Login ----
            case 2: {
                User* user = AuthenticationManager::loginUser();
                if (user) {
                    if (user->getRole() == "admin")
                        runAdminSession(static_cast<Admin*>(user));
                    else
                        runUserSession(static_cast<RegularUser*>(user));
                    delete user;
                }
                pressEnterToContinue();
                break;
            }

            // ---- Forgot Password ----
            case 3: {
                AuthenticationManager::forgotPassword();
                pressEnterToContinue();
                break;
            }

            // ---- Exit ----
            case 4:
                cout << "\n  Thank you for using Password Login System. Goodbye!\n\n";
                running = false;
                break;

            default:
                cout << "\n  [!] Invalid option. Please choose 1-4.\n";
                pressEnterToContinue();
        }
    }
    return 0;
}

// ============================================================
//  showMainMenu
// ============================================================
void showMainMenu() {
    clearScreen();
    printBanner();
    cout << "============================================\n";
    cout << "              MAIN MENU\n";
    cout << "============================================\n";
    cout << "  1. Register\n";
    cout << "  2. Login\n";
    cout << "  3. Forgot Password\n";
    cout << "  4. Exit\n";
    cout << "============================================\n";
    cout << "  Choice: ";
}

// ============================================================
//  runAdminSession
// ============================================================
void runAdminSession(Admin* admin) {
    bool loggedIn = true;
    while (loggedIn) {
        clearScreen();
        admin->displayMenu();

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n  [!] Invalid input.\n";
            pressEnterToContinue();
            continue;
        }

        switch (choice) {
            case 1: admin->displayProfile();       break;
            case 2: admin->viewAllUsers();         break;
            case 3: admin->searchUser();           break;
            case 4: admin->deleteUser();           break;
            case 5: admin->resetUserPassword();    break;
            case 6: admin->viewAllLoginHistory();  break;
            case 7:
                AuthenticationManager::changePassword(admin);
                break;
            case 8:
                cout << "\n  [+] Logged out successfully.\n";
                loggedIn = false;
                break;
            default:
                cout << "\n  [!] Invalid option. Choose 1-8.\n";
        }

        if (loggedIn) pressEnterToContinue();
    }
}

// ============================================================
//  runUserSession
// ============================================================
void runUserSession(RegularUser* user) {
    bool loggedIn = true;
    while (loggedIn) {
        clearScreen();
        user->displayMenu();

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n  [!] Invalid input.\n";
            pressEnterToContinue();
            continue;
        }

        switch (choice) {
            case 1: user->displayProfile();               break;
            case 2: user->changePassword();               break;
            case 3: user->updateProfile();                break;
            case 4: user->viewPersonalLoginHistory();     break;
            case 5:
                cout << "\n  [+] Logged out successfully.\n";
                loggedIn = false;
                break;
            default:
                cout << "\n  [!] Invalid option. Choose 1-5.\n";
        }

        if (loggedIn) pressEnterToContinue();
    }
}

// ============================================================
//  Utility helpers
// ============================================================
void printBanner() {
    cout << "\n";
    cout << "  +==========================================+\n";
    cout << "  |      PASSWORD LOGIN SYSTEM  v1.0         |\n";
    cout << "  |    2nd Semester OOP Project - C++        |\n";
    cout << "  +==========================================+\n\n";
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}
