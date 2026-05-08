#include "PasswordEvaluator.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
using namespace std;


// ─────────────────────────────────────────────
//  CONSTRUCTOR
// ─────────────────────────────────────────────
PasswordEvaluator::PasswordEvaluator()
    : password(""), score(0), level(VERY_WEAK) {}


// ─────────────────────────────────────────────
//  SETTER
// ─────────────────────────────────────────────
void PasswordEvaluator::setPassword(const string& pwd) {
    if (pwd.empty()) {
        throw invalid_argument("Password cannot be empty.");
    }
    if (pwd.length() > 128) {
        throw invalid_argument("Password is too long (max 128 characters).");
    }
    for (char c : pwd) {
        if (c == ' ') {
            throw invalid_argument("Password should not contain spaces.");
        }
    }
    password = pwd;
    score    = 0;
    level    = VERY_WEAK;
}


// ─────────────────────────────────────────────
//  PRIVATE CHECKERS
// ─────────────────────────────────────────────

bool PasswordEvaluator::checkLength() const {
    return password.length() >= 8;
}

bool PasswordEvaluator::checkUppercase() const {
    for (char c : password) {
        if (isupper(c)) return true;
    }
    return false;
}

bool PasswordEvaluator::checkDigits() const {
    for (char c : password) {
        if (isdigit(c)) return true;
    }
    return false;
}

bool PasswordEvaluator::checkSymbols() const {
    string symbols = "@#$!%&*-_+=?";
    for (char c : password) {
        if (symbols.find(c) != string::npos) return true;
    }
    return false;
}

bool PasswordEvaluator::checkCommonPatterns() const {
    string patterns[] = {
        "123", "1234", "12345", "123456",
        "abc", "abcd", "abcde",
        "qwerty", "qwert", "asdf",
        "password", "pass", "1111", "0000",
        "aaa", "aaaa", "admin", "login",
        "letmein", "welcome", "monkey", "dragon"
    };
    int patternCount = 22;

    string lower = "";
    for (char c : password) {
        lower += tolower(c);
    }

    for (int i = 0; i < patternCount; i++) {
        if (lower.find(patterns[i]) != string::npos) {
            return true;
        }
    }
    return false;
}

bool PasswordEvaluator::checkLeakedList() const {
    ifstream file("leaked_passwords.txt");
    if (!file.is_open()) {
        return false;
    }

    string line;
    string lower = "";
    for (char c : password) lower += tolower(c);

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        string lineLower = "";
        for (char c : line) lineLower += tolower(c);

        if (lineLower == lower) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}


// ─────────────────────────────────────────────
//  SCORE CALCULATOR
// ─────────────────────────────────────────────
void PasswordEvaluator::calculateScore() {
    score = 0;

    int len = password.length();
    if      (len >= 20) score += 30;
    else if (len >= 16) score += 25;
    else if (len >= 12) score += 20;
    else if (len >= 8 ) score += 12;
    else                score += 5;

    if (checkUppercase()) score += 15;
    if (checkDigits())    score += 15;
    if (checkSymbols())   score += 20;

    bool hasLower = false;
    for (char c : password) {
        if (islower(c)) { hasLower = true; break; }
    }
    if (hasLower) score += 10;

    if (checkCommonPatterns()) score -= 20;
    if (checkLeakedList())     score  = 0;

    if (score < 0)   score = 0;
    if (score > 100) score = 100;

    if      (score >= 80) level = VERY_STRONG;
    else if (score >= 60) level = STRONG;
    else if (score >= 40) level = MODERATE;
    else if (score >= 20) level = WEAK;
    else                  level = VERY_WEAK;
}


// ─────────────────────────────────────────────
//  CRACK TIME ESTIMATOR
// ─────────────────────────────────────────────
string PasswordEvaluator::getCrackTime() const {
    if      (score >= 80) return "Centuries (practically uncrackable)";
    else if (score >= 60) return "Several years";
    else if (score >= 40) return "A few hours to days";
    else if (score >= 20) return "A few minutes";
    else                  return "Less than 1 second";
}


// ─────────────────────────────────────────────
//  PUBLIC EVALUATE
// ─────────────────────────────────────────────
void PasswordEvaluator::evaluate() {
    if (password.empty()) {
        throw runtime_error("No password set. Call setPassword() first.");
    }
    calculateScore();
}


// ─────────────────────────────────────────────
//  DISPLAY REPORT
// ─────────────────────────────────────────────
void PasswordEvaluator::displayReport() const {

    cout << "\n";
    cout << "  ========================================\n";
    cout << "         PASSWORD STRENGTH REPORT\n";
    cout << "  ========================================\n";

    cout << "  Password  : " << password << "\n";
    cout << "  Score     : " << score << " / 100\n";

    // Visual bar
    cout << "  Strength  : [";
    int filled = score / 5;
    for (int i = 0; i < 20; i++) {
        if (i < filled) cout << "#";
        else            cout << "-";
    }
    cout << "] ";

    if      (level == VERY_STRONG) cout << "VERY STRONG\n";
    else if (level == STRONG)      cout << "STRONG\n";
    else if (level == MODERATE)    cout << "MODERATE\n";
    else if (level == WEAK)        cout << "WEAK\n";
    else                           cout << "VERY WEAK\n";

    cout << "  Crack Time: " << getCrackTime() << "\n";

    cout << "\n  ---- Criteria Breakdown ----\n";

    cout << (checkLength()          ? "  [PASS] " : "  [FAIL] ") << "Length is 8 or more characters\n";
    cout << (checkUppercase()       ? "  [PASS] " : "  [FAIL] ") << "Contains uppercase letters (A-Z)\n";
    cout << (checkDigits()          ? "  [PASS] " : "  [FAIL] ") << "Contains numbers (0-9)\n";
    cout << (checkSymbols()         ? "  [PASS] " : "  [FAIL] ") << "Contains symbols (@#$!...)\n";
    cout << (!checkCommonPatterns() ? "  [PASS] " : "  [FAIL] ") << "No common weak patterns (123, abc, qwerty)\n";
    cout << (!checkLeakedList()     ? "  [PASS] " : "  [FAIL] ") << "Not found in known leaked passwords list\n";

    cout << "\n  ---- Suggestions ----\n";

    bool allGood = true;

    if (!checkLength()) {
        cout << "  -> Make your password at least 8 characters long.\n";
        allGood = false;
    }
    if (!checkUppercase()) {
        cout << "  -> Add at least one uppercase letter (e.g. A, B, C).\n";
        allGood = false;
    }
    if (!checkDigits()) {
        cout << "  -> Add at least one number (e.g. 3, 7, 9).\n";
        allGood = false;
    }
    if (!checkSymbols()) {
        cout << "  -> Add a symbol like @, #, $, or ! to boost strength.\n";
        allGood = false;
    }
    if (checkCommonPatterns()) {
        cout << "  -> Remove common patterns like '123', 'abc', or 'qwerty'.\n";
        allGood = false;
    }
    if (checkLeakedList()) {
        cout << "  -> This password was found in a known data breach list!\n";
        cout << "  -> You must NOT use this password anywhere.\n";
        allGood = false;
    }
    if (password.length() < 12 && level != VERY_STRONG) {
        cout << "  -> Consider making it 12+ characters for better protection.\n";
        allGood = false;
    }

    if (allGood) {
        cout << "  Great job! Your password meets all the criteria.\n";
    }

    cout << "  ========================================\n\n";
}


// ─────────────────────────────────────────────
//  GETTERS
// ─────────────────────────────────────────────
int           PasswordEvaluator::getScore()    const { return score;    }
StrengthLevel PasswordEvaluator::getLevel()    const { return level;    }
string        PasswordEvaluator::getPassword() const { return password; }
