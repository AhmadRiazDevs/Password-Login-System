# Password Login System — Project Documentation

## 2nd Semester C++ OOP Project

---

## Table of Contents
1. [Project Overview](#overview)
2. [File Structure](#structure)
3. [How to Compile & Run](#compile)
4. [OOP Concepts Used](#oop)
5. [Class Descriptions](#classes)
6. [Sample Input / Output](#sample)
7. [Default Admin Credentials](#admin)
8. [Password Rules](#password)
9. [File Storage Format](#files)

---

## 1. Project Overview

A menu-driven **Password Login System** written in C++ that demonstrates all core
Object-Oriented Programming (OOP) concepts. It supports two user types — Admin and
Regular User — each with their own menu and feature set. All data is persisted to
plain text files so it survives program restarts.

---

## 2. File Structure

```
PasswordLoginSystem/
│
├── main.cpp                        ← Entry point, menu driver
│
├── include/                        ← Header files (.h)
│   ├── User.h                      ← Abstract base class
│   ├── Admin.h                     ← Derived: Admin
│   ├── RegularUser.h               ← Derived: RegularUser
│   ├── AuthenticationManager.h     ← Auth logic (register, login, passwords)
│   ├── FileManager.h               ← All file I/O
│   └── LoginHistoryManager.h       ← Login event recording
│
├── src/                            ← Implementation files (.cpp)
│   ├── User.cpp
│   ├── Admin.cpp
│   ├── RegularUser.cpp
│   ├── AuthenticationManager.cpp
│   ├── FileManager.cpp
│   └── LoginHistoryManager.cpp
│
├── data/                           ← Created automatically at runtime
│   ├── users.txt                   ← All registered user records
│   └── history.txt                 ← All login events
│
├── Makefile                        ← Build automation
└── README.md                       ← This file
```

---

## 3. How to Compile & Run

### Using Make (recommended)
```bash
make        # compile
make run    # compile and run
make clean  # remove the executable
```

### Manual compilation (g++)
```bash
g++ -std=c++17 -Wall -o PasswordLoginSystem \
    main.cpp src/User.cpp src/Admin.cpp src/RegularUser.cpp \
    src/FileManager.cpp src/AuthenticationManager.cpp src/LoginHistoryManager.cpp
./PasswordLoginSystem
```

### Requirements
- g++ compiler with C++17 support
- Linux / macOS / Windows (MSYS2 / WSL)

---

## 4. OOP Concepts Used

### A. Classes & Objects
Every component of the system is modelled as a class:
- `User`, `Admin`, `RegularUser` — model accounts
- `AuthenticationManager` — handles auth logic
- `FileManager` — handles all disk I/O
- `LoginHistoryManager` — records login events

Objects are created at runtime (e.g., `new Admin(...)`) and used through pointers.

---

### B. Encapsulation
Data members in `User` are declared `protected` — hidden from the outside world and
only accessible through public getter/setter methods:

```cpp
// Data is hidden:
protected:
    string username;
    string password;

// Access is controlled:
public:
    string getUsername() const;
    void setPassword(const string& pwd);
```

This prevents external code from directly modifying sensitive fields.

---

### C. Inheritance
`Admin` and `RegularUser` both inherit publicly from `User`:

```cpp
class Admin       : public User { ... };
class RegularUser : public User { ... };
```

They automatically get all the data members and methods of `User` without
rewriting them. Admin-specific functions (`viewAllUsers`, `deleteUser`, etc.)
are added only in the `Admin` class.

---

### D. Polymorphism
The base class declares two **pure virtual functions**:

```cpp
virtual void displayMenu()    = 0;
virtual void displayProfile() const = 0;
```

Both `Admin` and `RegularUser` provide their own implementations (override).
In `main.cpp`, the same code path calls the correct menu depending on the actual
object type at runtime:

```cpp
User* user = AuthenticationManager::loginUser();
// This calls Admin::displayMenu() or RegularUser::displayMenu()
// automatically — we never need to check the type manually.
```

This is **runtime polymorphism via virtual functions**.

---

### E. Abstraction
`User` is an **abstract class** (it cannot be instantiated directly) because it
contains pure virtual functions. It defines *what* every user must be able to do
without specifying *how*. Callers work through the `User*` pointer and never need
to know whether they have an Admin or a RegularUser.

`FileManager` also demonstrates abstraction: all file-path details are hidden as
private static constants. Callers just call `FileManager::saveUser(u)` — they
don't know or care about the file name or format.

---

### F. File Handling
All user data is stored in pipe-delimited text files:

- `data/users.txt` — one line per user
- `data/history.txt` — one line per login event

`FileManager` wraps `<fstream>` for all read/write operations. Data survives
program restart because it is written to disk immediately after every change.

---

## 5. Class Descriptions

| Class | Type | Responsibility |
|-------|------|----------------|
| `User` | Abstract Base | Stores common data; declares interface |
| `Admin` | Derived | Extends User with admin-only features |
| `RegularUser` | Derived | Extends User with user-only features |
| `AuthenticationManager` | Utility | Registration, login, password logic |
| `FileManager` | Utility | All file read / write / update / delete |
| `LoginHistoryManager` | Utility | Record and display login events |

---

## 6. Sample Input / Output

### Registering an Admin Account
```
============================================
           USER REGISTRATION
============================================
  Enter username (no spaces): admin1

  Password Rules:
  - Minimum 8 characters
  - At least one UPPERCASE letter
  - At least one lowercase letter
  - At least one digit (0-9)
  - At least one special character (!@#$%^&*...)

  Enter new password : Admin@123
  Confirm password   : Admin@123
  Enter email address : admin@company.com
  Enter full name     : Alice Smith

  Security Questions:
  1. What is your mother's maiden name?
  2. What was the name of your first pet?
  3. What city were you born in?
  4. What is your favourite book?
  Choose (1-4): 1
  Answer: Johnson

  Account type:
  1. Regular User
  2. Admin
  Choose (1-2): 2
  Enter admin secret key: 1

  [✓] Registration successful! Welcome, Alice Smith!
```

### Admin Menu
```
============================================
     ADMIN MENU  (Logged in as: admin1)
============================================
  1. View Profile
  2. View All Users
  3. Search User
  4. Delete User Account
  5. Reset User Password
  6. View All Login History
  7. Change My Password
  8. Logout
============================================
```

### View All Users (Admin)
```
============================================
         ALL REGISTERED USERS
============================================
Username          Full Name             Role      Registered On
--------------------------------------------------------------------
admin1            Alice Smith           admin     2025-09-01 10:23:45
john_doe          John Doe              user      2025-09-01 11:05:12
============================================
Total users: 2
```

### View Login History (Admin)
```
============================================
        ALL LOGIN HISTORY
============================================
Username            Status      Timestamp
------------------------------------------------------
admin1              SUCCESS     2025-09-01 10:23:45
john_doe            FAILED      2025-09-01 11:04:01
john_doe            SUCCESS     2025-09-01 11:05:12
============================================
Total records: 3
```

### Password Validation Failure
```
  Enter new password : abc123
  Confirm password   : abc123
  [!] Password must contain an uppercase letter. Try again.

  Enter new password : Abcdefgh
  Confirm password   : Abcdefgh
  [!] Password must contain a digit. Try again.

  Enter new password : Admin@123
  Confirm password   : Admin@123
  [✓] (accepted)
```

---

## 7. Default Admin Credentials

To register as an Admin, use the secret key: `ADMIN2024`

This is a simple hardcoded check. In a real system this would be a
configuration file or environment variable.

---

## 8. Password Rules

A valid password must satisfy ALL of the following:

| Rule | Example violation | Example pass |
|------|-------------------|--------------|
| Minimum 8 characters | `Pass@1` | `Password@1` |
| At least one UPPERCASE | `password@1` | `Password@1` |
| At least one lowercase | `PASSWORD@1` | `Password@1` |
| At least one digit | `Password@` | `Password@1` |
| At least one special char | `Password1` | `Password@1` |

---

## 9. File Storage Format

### data/users.txt
```
username|password|email|fullName|securityQuestion|securityAnswer|role|dateRegistered
```
Example:
```
admin1|Admin@123|admin@co.com|Alice Smith|What is your mother's maiden name?|Johnson|admin|2025-09-01 10:23:45
john_doe|Pass@123|john@co.com|John Doe|What was the name of your first pet?|Fluffy|user|2025-09-01 11:05:12
```

### data/history.txt
```
username|SUCCESS_or_FAILED|timestamp
```
Example:
```
admin1|SUCCESS|2025-09-01 10:23:45
john_doe|FAILED|2025-09-01 11:04:01
john_doe|SUCCESS|2025-09-01 11:05:12
```

---

*End of Documentation*
