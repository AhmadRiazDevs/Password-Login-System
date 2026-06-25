#ifndef ADMIN_H
#define ADMIN_H

// ============================================================
//  Admin.h  —  Derived Class (inherits from User)
//  OOP Concepts: Inheritance, Polymorphism (method override)
// ============================================================

#include "User.h"

class Admin : public User {           // Public Inheritance
public:
    // Constructor delegates to base-class constructor
    Admin(string uname, string pwd, string mail,
          string name, string sq, string sa, string date);

    // ---- Overrides (Polymorphism) ----
    void displayMenu()    override;
    void displayProfile() const override;

    // ---- Admin-only Features ----
    void viewAllUsers();
    void searchUser();
    void deleteUser();
    void resetUserPassword();
    void viewAllLoginHistory();
};

#endif // ADMIN_H
