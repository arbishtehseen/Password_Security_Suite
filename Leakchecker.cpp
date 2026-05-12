#include "LeakChecker.h"
#include <fstream>
#include <iostream>
#include <cctype>
using namespace std;

LeakChecker::LeakChecker() {}

LeakChecker::LeakChecker(string filename) {
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            // Trim trailing \r for Windows line endings
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (!line.empty()) {
                // Store all leaked passwords in lowercase for comparison
                string lower = "";
                for (char c : line) lower += (char)tolower(c);
                commonLeaks.push_back(lower);
            }
        }
        file.close();
    } else {
        cout << "Warning: Leaks file (" << filename << ") not found!" << endl;
    }
}

bool LeakChecker::isLeaked(string password) {
    // Convert input to lowercase for case-insensitive comparison
    string lower = "";
    for (char c : password) lower += (char)tolower(c);

    for (const string& leakedPass : commonLeaks) {
        if (lower == leakedPass) {
            return true;
        }
    }
    return false;
}
