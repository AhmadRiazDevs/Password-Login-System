#ifndef REGULARUSER_H
#define REGULARUSER_H

// ============================================================
//  RegularUser.h  ->  Derived Class (inherits from User)
//  OOP Concepts: Inheritance, Polymorphism (method override)
// ============================================================

#include "User.h"

class RegularUser : public User {     // Public Inheritance
public:
    // Constructor delegates to base-class constructor
    RegularUser(string uname, string pwd, string mail,
                string name, string sq, string sa, string date);

    // ---- Overrides (Polymorphism) ----
    void displayMenu()    override;
    void displayProfile() const override;

    // ---- User-specific Features ----
    void changePassword();
    void updateProfile();
    void viewPersonalLoginHistory();
};

#endif // REGULARUSER_H
