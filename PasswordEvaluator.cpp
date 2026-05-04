#include "PasswordEvaluator.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <windows.h>
using namespace std;


// ─────────────────────────────────────────────
//  COLOUR HELPER
//  Windows console colours using SetConsoleTextAttribute
//
//  Colour codes:
//  7  = White (default)
//  8  = Dark Grey
//  9  = Blue
//  10 = Green
//  11 = Cyan
//  12 = Red
//  13 = Magenta
//  14 = Yellow
// ─────────────────────────────────────────────
static void setColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}

static void resetColor() {
    setColor(7); // back to default white
}


// ─────────────────────────────────────────────
//  CONSTRUCTOR
// ─────────────────────────────────────────────
PasswordEvaluator::PasswordEvaluator()
    : password(""), score(0), level(VERY_WEAK) {}


// ─────────────────────────────────────────────
//  SETTER — validates input before storing
// ─────────────────────────────────────────────
void PasswordEvaluator::setPassword(const string& pwd) {
    if (pwd.empty()) {
        throw invalid_argument("Password cannot be empty.");
    }
    if (pwd.length() > 128) {
        throw invalid_argument("Password is too long (max 128 characters).");
    }
    // Check for spaces — passwords with spaces are usually mistyped
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

bool PasswordEvaluator::checkLength()         const {
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
    // Common weak patterns to look for
    string patterns[] = {
        "123", "1234", "12345", "123456",
        "abc", "abcd", "abcde",
        "qwerty", "qwert", "asdf",
        "password", "pass", "1111", "0000",
        "aaa", "aaaa", "admin", "login",
        "letmein", "welcome", "monkey", "dragon"
    };
    int patternCount = 22;

    // Convert password to lowercase for comparison
    string lower = "";
    for (char c : password) {
        lower += tolower(c);
    }

    for (int i = 0; i < patternCount; i++) {
        if (lower.find(patterns[i]) != string::npos) {
            return true; // found a weak pattern
        }
    }
    return false;
}

bool PasswordEvaluator::checkLeakedList() const {
    // Reads from leaked_passwords.txt — one password per line
    ifstream file("leaked_passwords.txt");
    if (!file.is_open()) {
        // File not found — skip this check silently
        return false;
    }

    string line;
    string lower = "";
    for (char c : password) lower += tolower(c);

    while (getline(file, line)) {
        // Trim trailing \r for Windows line endings
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        string lineLower = "";
        for (char c : line) lineLower += tolower(c);

        if (lineLower == lower) {
            file.close();
            return true; // found in leaked list
        }
    }

    file.close();
    return false;
}


// ─────────────────────────────────────────────
//  SCORE CALCULATOR
//  Builds score from criteria and sets level
// ─────────────────────────────────────────────
void PasswordEvaluator::calculateScore() {
    score = 0;

    // ── Base points from length ──────────────
    int len = password.length();
    if      (len >= 20) score += 30;
    else if (len >= 16) score += 25;
    else if (len >= 12) score += 20;
    else if (len >= 8 ) score += 12;
    else                score += 5;

    // ── Character diversity ──────────────────
    if (checkUppercase()) score += 15;
    if (checkDigits())    score += 15;
    if (checkSymbols())   score += 20;

    // ── Lowercase is always there for base ──
    bool hasLower = false;
    for (char c : password) {
        if (islower(c)) { hasLower = true; break; }
    }
    if (hasLower) score += 10;

    // ── Penalties ───────────────────────────
    if (checkCommonPatterns()) score -= 20;
    if (checkLeakedList())     score  = 0; // instant fail if leaked

    // ── Cap score between 0 and 100 ─────────
    if (score < 0)   score = 0;
    if (score > 100) score = 100;

    // ── Assign level ────────────────────────
    if      (score >= 80) level = VERY_STRONG;
    else if (score >= 60) level = STRONG;
    else if (score >= 40) level = MODERATE;
    else if (score >= 20) level = WEAK;
    else                  level = VERY_WEAK;
}


// ─────────────────────────────────────────────
//  CRACK TIME ESTIMATOR
//  Based on score — gives a human-readable estimate
// ─────────────────────────────────────────────
string PasswordEvaluator::getCrackTime() const {
    if      (score >= 80) return "Centuries (practically uncrackable)";
    else if (score >= 60) return "Several years";
    else if (score >= 40) return "A few hours to days";
    else if (score >= 20) return "A few minutes";
    else                  return "Less than 1 second";
}


// ─────────────────────────────────────────────
//  PUBLIC EVALUATE — runs all checks
// ─────────────────────────────────────────────
void PasswordEvaluator::evaluate() {
    if (password.empty()) {
        throw runtime_error("No password set. Call setPassword() first.");
    }
    calculateScore();
}


// ─────────────────────────────────────────────
//  DISPLAY REPORT — the coloured console output
// ─────────────────────────────────────────────
void PasswordEvaluator::displayReport() const {

    // ── Header ───────────────────────────────
    cout << "\n";
    setColor(11); // Cyan
    cout << "  ========================================\n";
    cout << "         PASSWORD STRENGTH REPORT\n";
    cout << "  ========================================\n";
    resetColor();

    // ── Password display ─────────────────────
    setColor(8); // Dark grey label
    cout << "  Password  : ";
    setColor(14); // Yellow for the password itself
    cout << password << "\n";
    resetColor();

    // ── Score bar ────────────────────────────
    setColor(8);
    cout << "  Score     : ";

    // Colour the score based on how good it is
    if      (score >= 80) setColor(10); // Green
    else if (score >= 60) setColor(14); // Yellow
    else if (score >= 40) setColor(13); // Magenta
    else                  setColor(12); // Red

    cout << score << " / 100\n";
    resetColor();

    // ── Visual bar ───────────────────────────
    setColor(8);
    cout << "  Strength  : [";

    int filled = score / 5; // 20 blocks total
    for (int i = 0; i < 20; i++) {
        if (i < filled) {
            if      (score >= 80) setColor(10);
            else if (score >= 60) setColor(14);
            else if (score >= 40) setColor(13);
            else                  setColor(12);
            cout << "#";
        } else {
            setColor(8);
            cout << "-";
        }
    }
    setColor(8);
    cout << "] ";

    // Level label
    if      (level == VERY_STRONG) { setColor(10); cout << "VERY STRONG\n"; }
    else if (level == STRONG)      { setColor(10); cout << "STRONG\n";      }
    else if (level == MODERATE)    { setColor(14); cout << "MODERATE\n";    }
    else if (level == WEAK)        { setColor(12); cout << "WEAK\n";        }
    else                           { setColor(12); cout << "VERY WEAK\n";   }
    resetColor();

    // ── Crack time ───────────────────────────
    setColor(8);
    cout << "  Crack Time: ";
    setColor(11); // Cyan
    cout << getCrackTime() << "\n";
    resetColor();

    // ── Criteria breakdown ───────────────────
    cout << "\n";
    setColor(11);
    cout << "  ---- Criteria Breakdown ----\n";
    resetColor();

    // Helper lambda-style inline check display
    auto showCheck = [](bool passed, const string& label) {
        if (passed) {
            setColor(10); cout << "  [PASS] "; // Green
        } else {
            setColor(12); cout << "  [FAIL] "; // Red
        }
        resetColor();
        cout << label << "\n";
    };

    showCheck(checkLength(),         "Length is 8 or more characters");
    showCheck(checkUppercase(),      "Contains uppercase letters (A-Z)");
    showCheck(checkDigits(),         "Contains numbers (0-9)");
    showCheck(checkSymbols(),        "Contains symbols (@#$!...)");
    showCheck(!checkCommonPatterns(),"No common weak patterns (123, abc, qwerty)");
    showCheck(!checkLeakedList(),    "Not found in known leaked passwords list");

    // ── Suggestions ──────────────────────────
    cout << "\n";
    setColor(11);
    cout << "  ---- Suggestions ----\n";
    resetColor();

    bool allGood = true;

    if (!checkLength()) {
        setColor(14);
        cout << "  -> Make your password at least 8 characters long.\n";
        allGood = false;
    }
    if (!checkUppercase()) {
        setColor(14);
        cout << "  -> Add at least one uppercase letter (e.g. A, B, C).\n";
        allGood = false;
    }
    if (!checkDigits()) {
        setColor(14);
        cout << "  -> Add at least one number (e.g. 3, 7, 9).\n";
        allGood = false;
    }
    if (!checkSymbols()) {
        setColor(14);
        cout << "  -> Add a symbol like @, #, $, or ! to boost strength.\n";
        allGood = false;
    }
    if (checkCommonPatterns()) {
        setColor(12);
        cout << "  -> Remove common patterns like '123', 'abc', or 'qwerty'.\n";
        allGood = false;
    }
    if (checkLeakedList()) {
        setColor(12);
        cout << "  -> This password was found in a known data breach list!\n";
        cout << "  -> You must NOT use this password anywhere.\n";
        allGood = false;
    }
    if (password.length() < 12 && level != VERY_STRONG) {
        setColor(14);
        cout << "  -> Consider making it 12+ characters for better protection.\n";
        allGood = false;
    }

    if (allGood) {
        setColor(10); // Green
        cout << "  Great job! Your password meets all the criteria.\n";
    }

    resetColor();

    // ── Footer ───────────────────────────────
    setColor(11);
    cout << "  ========================================\n\n";
    resetColor();
}


// ─────────────────────────────────────────────
//  GETTERS
// ─────────────────────────────────────────────
int           PasswordEvaluator::getScore()    const { return score;    }
StrengthLevel PasswordEvaluator::getLevel()    const { return level;    }
string        PasswordEvaluator::getPassword() const { return password; }


// ─────────────────────────────────────────────
//  FREE FUNCTION — call this from your main menu
// ─────────────────────────────────────────────
void runPasswordEvaluator() {
    setColor(11);
    cout << "\n  ========================================\n";
    cout << "         PASSWORD EVALUATOR\n";
    cout << "  ========================================\n";
    resetColor();

    PasswordEvaluator evaluator;
    char again = 'y';

    while (again == 'y' || again == 'Y') {

        string pwd = "";

        // Keep asking until valid input is given
        while (true) {
            setColor(8);
            cout << "\n  Enter password to evaluate: ";
            resetColor();
            cin >> pwd;
            cin.ignore(1000, '\n');

            try {
                evaluator.setPassword(pwd);
                break; // valid — exit the inner loop
            }
            catch (invalid_argument& e) {
                setColor(12);
                cout << "  Error: " << e.what() << "\n";
                resetColor();
            }
        }

        try {
            evaluator.evaluate();
            evaluator.displayReport();
        }
        catch (runtime_error& e) {
            setColor(12);
            cout << "  Error: " << e.what() << "\n";
            resetColor();
        }

        setColor(8);
        cout << "  Evaluate another password? (y/n): ";
        resetColor();
        cin >> again;
        cin.ignore(1000, '\n');

        // Handle invalid y/n response
        while (again != 'y' && again != 'Y' && again != 'n' && again != 'N') {
            setColor(12);
            cout << "  Invalid input. Enter y or n: ";
            resetColor();
            cin >> again;
            cin.ignore(1000, '\n');
        }
    }
}
