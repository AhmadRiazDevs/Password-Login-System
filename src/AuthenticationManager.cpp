// ============================================================
//  AuthenticationManager.cpp  ->  Registration, Login, Passwords
// ============================================================

#include "../include/AuthenticationManager.h"
#include "../include/FileManager.h"
#include "../include/LoginHistoryManager.h"
#include "../include/Admin.h"
#include "../include/RegularUser.h"

#include <iostream>
using namespace std;

// ============================================================
//  Manual character-type checks (replaces <cctype>)
// ============================================================
static bool myIsUpper(char c)   { return (c >= 'A' && c <= 'Z'); }
static bool myIsLower(char c)   { return (c >= 'a' && c <= 'z'); }
static bool myIsDigit(char c)   { return (c >= '0' && c <= '9'); }
static bool myIsSpecial(char c) {
    // Not a letter, not a digit, and not a space
    return !myIsUpper(c) && !myIsLower(c) && !myIsDigit(c) && c != ' ';
}

// ============================================================
//  showPasswordRules
// ============================================================
void AuthenticationManager::showPasswordRules() {
    cout << "\n  Password Rules:\n";
    cout << "  - Minimum 8 characters\n";
    cout << "  - At least one UPPERCASE letter\n";
    cout << "  - At least one lowercase letter\n";
    cout << "  - At least one digit (0-9)\n";
    cout << "  - At least one special character (!@#$%^&*...)\n";
}

// ============================================================
//  isValidPassword
// ============================================================
bool AuthenticationManager::isValidPassword(const string& pwd, string& errorMsg) {
    if (pwd.length() < 8) {
        errorMsg = "Password must be at least 8 characters long.";
        return false;
    }
    bool hasUpper = false, hasLower = false,
         hasDigit = false, hasSpecial = false;

    for (int i = 0; i < (int)pwd.length(); i++) {
        char c = pwd[i];
        if (myIsUpper(c))   hasUpper   = true;
        if (myIsLower(c))   hasLower   = true;
        if (myIsDigit(c))   hasDigit   = true;
        if (myIsSpecial(c)) hasSpecial = true;
    }

    if (!hasUpper)   { errorMsg = "Password must contain an uppercase letter."; return false; }
    if (!hasLower)   { errorMsg = "Password must contain a lowercase letter.";  return false; }
    if (!hasDigit)   { errorMsg = "Password must contain a digit.";             return false; }
    if (!hasSpecial) { errorMsg = "Password must contain a special character."; return false; }

    return true;
}

// ============================================================
//  promptNewPassword
// ============================================================
string AuthenticationManager::promptNewPassword() {
    showPasswordRules();
    string pwd, confirm;

    while (true) {
        cout << "\n  Enter new password : ";
        cin  >> pwd;
        cout << "  Confirm password   : ";
        cin  >> confirm;

        if (pwd != confirm) {
            cout << "  [!] Passwords do not match. Try again.\n";
            continue;
        }

        string errorMsg;
        if (!isValidPassword(pwd, errorMsg)) {
            cout << "  [!] " << errorMsg << " Try again.\n";
            continue;
        }
        break;
    }
    return pwd;
}

// ============================================================
//  registerUser
// ============================================================
User* AuthenticationManager::registerUser() {
    cout << "\n============================================\n";
    cout << "           USER REGISTRATION\n";
    cout << "============================================\n";

    // 1. Username
    string username;
    while (true) {
        cout << "  Enter username (no spaces): ";
        cin  >> username;
        if (username.length() < 3) {
            cout << "  [!] Username must be at least 3 characters.\n";
            continue;
        }
        if (FileManager::userExists(username)) {
            cout << "  [!] Username already taken. Choose another.\n";
            continue;
        }
        break;
    }

    // 2. Password
    string pwd = promptNewPassword();

    // 3. Email
    cout << "  Enter email address : ";
    string email;
    cin  >> email;

    // 4. Full name (allow spaces via getline)
    cin.ignore(10000, '\n');
    cout << "  Enter full name     : ";
    string fullName;
    getline(cin, fullName);

    // 5. Security question
    cout << "\n  Security Questions:\n";
    cout << "  1. What is your mother's maiden name?\n";
    cout << "  2. What was the name of your first pet?\n";
    cout << "  3. What city were you born in?\n";
    cout << "  4. What is your favourite book?\n";
    cout << "  Choose (1-4): ";
    int sqChoice;
    cin >> sqChoice;
    cin.ignore(10000, '\n');

    string sq;
    switch (sqChoice) {
        case 1: sq = "What is your mother's maiden name?";    break;
        case 2: sq = "What was the name of your first pet?";  break;
        case 3: sq = "What city were you born in?";           break;
        case 4: sq = "What is your favourite book?";          break;
        default: sq = "What is your mother's maiden name?";   break;
    }

    cout << "  Answer: ";
    string sa;
    getline(cin, sa);

    // 6. Role
    cout << "\n  Account type:\n";
    cout << "  1. Regular User\n";
    cout << "  2. Admin\n";
    cout << "  Choose (1-2): ";
    int roleChoice;
    cin >> roleChoice;

    string role = (roleChoice == 2) ? "admin" : "user";

    // 7. Admin secret key check
    if (role == "admin") {
        cout << "  Enter admin secret key: ";
        string key;
        cin  >> key;
        if (key != "ADMIN2024") {
            cout << "  [!] Wrong admin key. Registering as regular user.\n";
            role = "user";
        }
    }

    string date = FileManager::getCurrentTimestamp();

    // 8. Create the appropriate object (Polymorphism)
    User* newUser = nullptr;
    if (role == "admin")
        newUser = new Admin(username, pwd, email, fullName, sq, sa, date);
    else
        newUser = new RegularUser(username, pwd, email, fullName, sq, sa, date);

    // 9. Persist to file
    if (!FileManager::saveUser(*newUser)) {
        cout << "  [!] Failed to save user. Try again.\n";
        delete newUser;
        return nullptr;
    }

    cout << "\n  [+] Registration successful! Welcome, " << fullName << "!\n";
    return newUser;
}

// ============================================================
//  loginUser
// ============================================================
User* AuthenticationManager::loginUser() {
    cout << "\n============================================\n";
    cout << "              USER LOGIN\n";
    cout << "============================================\n";

    string username, password;
    cout << "  Username: ";
    cin  >> username;
    cout << "  Password: ";
    cin  >> password;

    User* user = FileManager::loadUser(username);

    if (!user || user->getPassword() != password) {
        cout << "  [!] Invalid username or password.\n";
        LoginHistoryManager::recordLogin(username, false);
        delete user;
        return nullptr;
    }

    cout << "\n  [+] Login successful! Welcome back, "
         << user->getFullName() << "!\n";
    LoginHistoryManager::recordLogin(username, true);
    return user;
}

// ============================================================
//  forgotPassword
// ============================================================
bool AuthenticationManager::forgotPassword() {
    cout << "\n============================================\n";
    cout << "          FORGOT PASSWORD\n";
    cout << "============================================\n";

    cout << "  Enter your username: ";
    string username;
    cin  >> username;

    User* user = FileManager::loadUser(username);
    if (!user) {
        cout << "  [!] Username not found.\n";
        return false;
    }

    cout << "\n  Security Question: " << user->getSecurityQuestion() << "\n";
    cout << "  Your Answer: ";
    cin.ignore(10000, '\n');
    string answer;
    getline(cin, answer);

    if (answer != user->getSecurityAnswer()) {
        cout << "  [!] Incorrect answer.\n";
        delete user;
        return false;
    }

    cout << "  [+] Identity verified! Set a new password.\n";
    string newPwd = promptNewPassword();
    user->setPassword(newPwd);

    bool ok = FileManager::updateUser(*user);
    if (ok)
        cout << "  [+] Password reset successfully!\n";
    else
        cout << "  [!] Failed to save new password.\n";

    delete user;
    return ok;
}

// ============================================================
//  changePassword
// ============================================================
bool AuthenticationManager::changePassword(User* user) {
    if (!user) return false;

    cout << "\n  Enter current password: ";
    string current;
    cin  >> current;

    if (current != user->getPassword()) {
        cout << "  [!] Current password is incorrect.\n";
        return false;
    }

    cout << "  Set new password:\n";
    string newPwd = promptNewPassword();
    user->setPassword(newPwd);

    bool ok = FileManager::updateUser(*user);
    if (ok) cout << "  [+] Password changed successfully!\n";
    else    cout << "  [!] Failed to save new password.\n";
    return ok;
}

// ============================================================
//  resetPassword  ->  Admin resets any user's password
// ============================================================
bool AuthenticationManager::resetPassword(const string& username) {
    User* user = FileManager::loadUser(username);
    if (!user) {
        cout << "  [!] User not found.\n";
        return false;
    }

    cout << "  Set new password for " << username << ":\n";
    string newPwd = promptNewPassword();
    user->setPassword(newPwd);

    bool ok = FileManager::updateUser(*user);
    if (ok) cout << "  [+] Password for " << username << " reset successfully!\n";
    else    cout << "  [!] Failed to update password.\n";
    delete user;
    return ok;
}
