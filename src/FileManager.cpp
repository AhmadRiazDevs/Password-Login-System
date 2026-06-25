// ============================================================
//  FileManager.cpp  ->  All file I/O for the system
// ============================================================

#include "../include/FileManager.h"
#include "../include/Admin.h"
#include "../include/RegularUser.h"

#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstring>
using namespace std;

// ---- Static member initialisation ----
const string FileManager::USERS_FILE   = "data/users.txt";
const string FileManager::HISTORY_FILE = "data/history.txt";

// ============================================================
//  Helper: Split a pipe-delimited line into tokens (manual parse)
//  Returns the number of tokens found (up to maxTokens)
// ============================================================
static int splitByPipe(const string& line, string* tokens, int maxTokens) {
    int count = 0;
    int start = 0;
    int len   = line.length();

    for (int i = 0; i <= len && count < maxTokens; i++) {
        if (i == len || line[i] == '|') {
            tokens[count] = line.substr(start, i - start);
            count++;
            start = i + 1;
        }
    }
    return count;
}

// ============================================================
//  saveUser  ->  Append a new user record to the file
// ============================================================
bool FileManager::saveUser(const User& user) {
    FILE* fp = fopen(USERS_FILE.c_str(), "a");   // append mode
    if (!fp) {
        cerr << "[FileManager] Cannot open " << USERS_FILE << endl;
        return false;
    }
    string record = user.toFileString() + "\n";
    fputs(record.c_str(), fp);
    fclose(fp);
    return true;
}

// ============================================================
//  userExists  ->  Check whether a username is already taken
// ============================================================
bool FileManager::userExists(const string& username) {
    FILE* fp = fopen(USERS_FILE.c_str(), "r");
    if (!fp) return false;

    char buf[1024];
    while (fgets(buf, sizeof(buf), fp)) {
        string line = buf;
        // Remove trailing newline
        if (!line.empty() && line[line.length() - 1] == '\n')
            line = line.substr(0, line.length() - 1);
        if (line.empty()) continue;

        // The first field (before the first '|') is the username
        string storedName = line.substr(0, line.find('|'));
        if (storedName == username) {
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false;
}

// ============================================================
//  parseLine  ->  Internal helper: turn a file line into a User*
// ============================================================
static User* parseLine(const string& line) {
    if (line.empty()) return nullptr;

    // Split pipe-delimited record into tokens
    string tokens[8];
    int count = splitByPipe(line, tokens, 8);

    if (count < 8) return nullptr;   // Corrupted record

    string uname = tokens[0], pwd  = tokens[1],
           mail  = tokens[2], name = tokens[3],
           sq    = tokens[4], sa   = tokens[5],
           role  = tokens[6], date = tokens[7];

    // Polymorphic object creation based on role field
    if (role == "admin")
        return new Admin(uname, pwd, mail, name, sq, sa, date);
    else
        return new RegularUser(uname, pwd, mail, name, sq, sa, date);
}

// ============================================================
//  Helper: Read a line from FILE* into a string (removes newline)
// ============================================================
static bool readLine(FILE* fp, string& out) {
    char buf[1024];
    if (!fgets(buf, sizeof(buf), fp)) return false;
    out = buf;
    if (!out.empty() && out[out.length() - 1] == '\n')
        out = out.substr(0, out.length() - 1);
    return true;
}

// ============================================================
//  loadUser  ->  Find and return one user by username
// ============================================================
User* FileManager::loadUser(const string& username) {
    FILE* fp = fopen(USERS_FILE.c_str(), "r");
    if (!fp) return nullptr;

    string line;
    while (readLine(fp, line)) {
        if (line.empty()) continue;
        string storedName = line.substr(0, line.find('|'));
        if (storedName == username) {
            fclose(fp);
            return parseLine(line);
        }
    }
    fclose(fp);
    return nullptr;
}

// ============================================================
//  loadAllUsers  ->  Fill array with every user in the file
//  Returns: number of users loaded
// ============================================================
int FileManager::loadAllUsers(User** arr, int maxSize) {
    int count = 0;
    FILE* fp = fopen(USERS_FILE.c_str(), "r");
    if (!fp) return 0;

    string line;
    while (readLine(fp, line) && count < maxSize) {
        User* u = parseLine(line);
        if (u) {
            arr[count] = u;
            count++;
        }
    }
    fclose(fp);
    return count;
}

// ============================================================
//  updateUser  ->  Rewrite the file, replacing one user's record
// ============================================================
bool FileManager::updateUser(const User& user) {
    // Read all lines into a fixed array
    FILE* inFp = fopen(USERS_FILE.c_str(), "r");
    if (!inFp) return false;

    string lines[MAX_USERS];
    int lineCount = 0;
    while (readLine(inFp, lines[lineCount]) && lineCount < MAX_USERS) {
        lineCount++;
    }
    fclose(inFp);

    // Replace the matching line
    bool found = false;
    for (int i = 0; i < lineCount; i++) {
        if (lines[i].empty()) continue;
        string storedName = lines[i].substr(0, lines[i].find('|'));
        if (storedName == user.getUsername()) {
            lines[i] = user.toFileString();
            found = true;
        }
    }
    if (!found) return false;

    // Write all lines back
    FILE* outFp = fopen(USERS_FILE.c_str(), "w");
    if (!outFp) return false;
    for (int i = 0; i < lineCount; i++) {
        if (!lines[i].empty()) {
            string record = lines[i] + "\n";
            fputs(record.c_str(), outFp);
        }
    }
    fclose(outFp);
    return true;
}

// ============================================================
//  deleteUser  ->  Rewrite the file, skipping the target user
// ============================================================
bool FileManager::deleteUser(const string& username) {
    FILE* inFp = fopen(USERS_FILE.c_str(), "r");
    if (!inFp) return false;

    string lines[MAX_USERS];
    int lineCount = 0;
    bool found = false;

    string line;
    while (readLine(inFp, line)) {
        if (line.empty()) continue;
        string storedName = line.substr(0, line.find('|'));
        if (storedName == username) {
            found = true;
            continue;   // skip this user
        }
        if (lineCount < MAX_USERS) {
            lines[lineCount] = line;
            lineCount++;
        }
    }
    fclose(inFp);
    if (!found) return false;

    FILE* outFp = fopen(USERS_FILE.c_str(), "w");
    if (!outFp) return false;
    for (int i = 0; i < lineCount; i++) {
        string record = lines[i] + "\n";
        fputs(record.c_str(), outFp);
    }
    fclose(outFp);
    return true;
}

// ============================================================
//  appendHistory  ->  Write one login event line
//  Format: username | SUCCESS/FAILED | timestamp
// ============================================================
bool FileManager::appendHistory(const string& username,
                                const string& status,
                                const string& timestamp) {
    FILE* fp = fopen(HISTORY_FILE.c_str(), "a");
    if (!fp) return false;
    string record = username + "|" + status + "|" + timestamp + "\n";
    fputs(record.c_str(), fp);
    fclose(fp);
    return true;
}

// ============================================================
//  loadHistory  ->  Fill array with history lines
//  Returns: number of records loaded
// ============================================================
int FileManager::loadHistory(string* arr, int maxSize,
                             const string& username) {
    int count = 0;
    FILE* fp = fopen(HISTORY_FILE.c_str(), "r");
    if (!fp) return 0;

    string line;
    while (readLine(fp, line) && count < maxSize) {
        if (line.empty()) continue;
        if (username.empty()) {
            arr[count] = line;            // collect all
            count++;
        } else {
            string storedName = line.substr(0, line.find('|'));
            if (storedName == username) {
                arr[count] = line;        // collect only this user
                count++;
            }
        }
    }
    fclose(fp);
    return count;
}

// ============================================================
//  getCurrentTimestamp  ->  Human-readable date/time string
// ============================================================
string FileManager::getCurrentTimestamp() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// ============================================================
//  freeUserList  ->  Delete heap-allocated User* objects
// ============================================================
void FileManager::freeUserList(User** arr, int count) {
    for (int i = 0; i < count; i++) {
        delete arr[i];
        arr[i] = nullptr;
    }
}
