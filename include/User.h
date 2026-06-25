#ifndef USER_H
#define USER_H

// ============================================================
//  User.h  —  Abstract Base Class
//  OOP Concepts: Abstraction, Encapsulation, Polymorphism
// ============================================================

#include <string>
using namespace std;

class User {
protected:
    // Encapsulated (private-to-child) data members
    string username;
    string password;
    string email;
    string fullName;
    string securityQuestion;
    string securityAnswer;
    string role;           // "admin" or "user"
    string dateRegistered;

public:
    // Constructor
    User(string uname, string pwd, string mail,
         string name, string sq, string sa,
         string r, string date);

    // Virtual destructor — good practice for base classes
    virtual ~User() {}

    // ---- Pure virtual functions (Abstract Interface) ----
    // Every derived class MUST implement these
    virtual void displayMenu()   = 0;   // Polymorphism via override
    virtual void displayProfile() const = 0;

    // ---- Getters (Encapsulation) ----
    string getUsername()        const;
    string getPassword()        const;
    string getEmail()           const;
    string getFullName()        const;
    string getSecurityQuestion()const;
    string getSecurityAnswer()  const;
    string getRole()            const;
    string getDateRegistered()  const;

    // ---- Setters (Encapsulation) ----
    void setPassword(const string& pwd);
    void setEmail(const string& mail);
    void setFullName(const string& name);

    // ---- Utility ----
    // Serialise object to a single pipe-delimited line for file storage
    string toFileString() const;
};

#endif // USER_H
