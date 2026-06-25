// ============================================================
//  LoginHistoryManager.cpp
// ============================================================

#include "../include/LoginHistoryManager.h"
#include "../include/FileManager.h"

#include <iostream>
using namespace std;

// ============================================================
//  Helper: Pad a string with spaces to a fixed width
// ============================================================
static string padRight(const string& str, int width) {
    string result = str;
    while ((int)result.length() < width)
        result += ' ';
    return result;
}

// ============================================================
//  Helper: Split a pipe-delimited line into tokens
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
//  recordLogin ->  Save a login event to disk
// ============================================================
void LoginHistoryManager::recordLogin(const string& username, bool success) {
    string status    = success ? "SUCCESS" : "FAILED";
    string timestamp = FileManager::getCurrentTimestamp();
    FileManager::appendHistory(username, status, timestamp);
}

// ============================================================
//  displayHistory  ->  Pretty-print history records
// ============================================================
void LoginHistoryManager::displayHistory(const string& username) {
    string records[MAX_HISTORY];
    int recordCount = FileManager::loadHistory(records, MAX_HISTORY, username);

    cout << "\n";
    cout << "============================================\n";
    if (username.empty())
        cout << "        ALL LOGIN HISTORY\n";
    else
        cout << "    LOGIN HISTORY FOR: " << username << "\n";
    cout << "============================================\n";

    if (recordCount == 0) {
        cout << "  No login history found.\n";
        cout << "============================================\n";
        return;
    }

    // Column headers with manual padding
    cout << padRight("Username", 20)
         << padRight("Status", 12)
         << "Timestamp\n";

    // Separator line
    for (int i = 0; i < 54; i++) cout << '-';
    cout << "\n";

    // Print each record (pipe-delimited: user|status|timestamp)
    for (int i = 0; i < recordCount; i++) {
        string tokens[3];
        int count = splitByPipe(records[i], tokens, 3);
        if (count < 3) continue;

        cout << padRight(tokens[0], 20)
             << padRight(tokens[1], 12)
             << tokens[2] << "\n";
    }

    cout << "============================================\n";
    cout << "Total records: " << recordCount << "\n";
}
