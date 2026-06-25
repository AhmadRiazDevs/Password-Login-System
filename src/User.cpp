// ============================================================
//  User.cpp  ->  Base Class Implementation
// ============================================================

#include "../include/User.h"
#include <iostream>
using namespace std;

// ---------- Constructor ----------
User::User(string uname, string pwd, string mail,
           string name, string sq, string sa,
           string r, string date)
    : username(uname), password(pwd), email(mail),
      fullName(name), securityQuestion(sq), securityAnswer(sa),
      role(r), dateRegistered(date)
{}

// ---------- Getters ----------
string User::getUsername()         const { return username; }
string User::getPassword()         const { return password; }
string User::getEmail()            const { return email; }
string User::getFullName()         const { return fullName; }
string User::getSecurityQuestion() const { return securityQuestion; }
string User::getSecurityAnswer()   const { return securityAnswer; }
string User::getRole()             const { return role; }
string User::getDateRegistered()   const { return dateRegistered; }

// ---------- Setters ----------
void User::setPassword(const string& pwd)  { password = pwd; }
void User::setEmail(const string& mail)    { email = mail; }
void User::setFullName(const string& name) { fullName = name; }

// ---------- Serialisation ----------
// Format: username|password|email|fullName|securityQuestion|securityAnswer|role|date
string User::toFileString() const {
    return username         + "|" +
           password         + "|" +
           email            + "|" +
           fullName         + "|" +
           securityQuestion + "|" +
           securityAnswer   + "|" +
           role             + "|" +
           dateRegistered;
}
