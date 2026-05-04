#include "PasswordGenerator.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
using namespace std;


// ─────────────────────────────────────────────
//  BASE CLASS — PasswordGenerator
// ─────────────────────────────────────────────

PasswordGenerator::PasswordGenerator(int len, bool upper, bool digits,
                                     bool symbols, bool avoidConfusing)
    : length(len), useUppercase(upper), useDigits(digits),
      useSymbols(symbols), avoidLookAlikes(avoidConfusing) {}

void PasswordGenerator::setLength(int len) {
    if (len < 4 || len > 64) {
        throw invalid_argument("Length must be between 4 and 64.");
    }
    length = len;
}

void PasswordGenerator::setUseUppercase(bool val)    { useUppercase    = val; }
void PasswordGenerator::setUseDigits(bool val)       { useDigits       = val; }
void PasswordGenerator::setUseSymbols(bool val)      { useSymbols      = val; }
void PasswordGenerator::setAvoidLookAlikes(bool val) { avoidLookAlikes = val; }
int  PasswordGenerator::getLength() const            { return length;         }


// ─────────────────────────────────────────────
//  SUBCLASS 1 — RandomGenerator
// ─────────────────────────────────────────────

RandomGenerator::RandomGenerator(int len, bool upper, bool digits,
                                 bool symbols, bool avoidConfusing)
    : PasswordGenerator(len, upper, digits, symbols, avoidConfusing) {}

// Build the pool of allowed characters based on user settings
string RandomGenerator::buildCharPool() {
    string pool = "abcdefghijkmnopqrstuvwxyz"; // lowercase (m removed — looks like rn)

    if (avoidLookAlikes) {
        // Remove confusing lowercase: l
        pool = "abcdefghijkmnopqrstuvwxyz";
        // already removed l above; also remove o for 0/o confusion
        string cleaned = "";
        for (char c : pool) {
            if (c != 'l' && c != 'o') cleaned += c;
        }
        pool = cleaned;
    }

    if (useUppercase) {
        string upper = "ABCDEFGHJKLMNPQRSTUVWXYZ"; // removed I (looks like 1)
        if (!avoidLookAlikes) {
            upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
        pool += upper;
    }

    if (useDigits) {
        string digits = avoidLookAlikes ? "23456789" : "0123456789";
        pool += digits;
    }

    if (useSymbols) {
        pool += "@#$!%&*";
    }

    return pool;
}

string RandomGenerator::generate() {
    string pool = buildCharPool();

    // Safety check — pool should never be empty but let's be safe
    if (pool.empty()) {
        throw runtime_error("Character pool is empty. Enable at least one character type.");
    }

    srand((unsigned int)time(0));

    string password = "";
    for (int i = 0; i < length; i++) {
        int index = rand() % pool.size();
        password += pool[index];
    }

    return password;
}


// ─────────────────────────────────────────────
//  SUBCLASS 2 — PhraseGenerator
// ─────────────────────────────────────────────

PhraseGenerator::PhraseGenerator(int words, char sep)
    : PasswordGenerator(12, false, false, false, false),
      wordCount(words), separator(sep) {}

void PhraseGenerator::setWordCount(int words) {
    if (words < 2 || words > 4) {
        throw invalid_argument("Word count must be 2, 3, or 4.");
    }
    wordCount = words;
}

void PhraseGenerator::setSeparator(char sep) {
    separator = sep;
}

string PhraseGenerator::generate() {
    srand((unsigned int)time(0));

    // A small built-in word list — simple and clear
    string words[] = {
        "Blue", "Tiger", "Storm", "Cloud", "River",
        "Eagle", "Stone", "Flame", "Swift", "Night",
        "Brave", "Solar", "Frost", "Cedar", "Maple",
        "Ocean", "Pixel", "Lunar", "Amber", "Steel",
        "Crisp", "Vivid", "Blaze", "Delta", "Flint"
    };
    int wordListSize = 25;

    string passphrase = "";

    for (int i = 0; i < wordCount; i++) {
        int index = rand() % wordListSize;
        passphrase += words[index];

        // Add separator between words, not after the last word
        if (i < wordCount - 1) {
            passphrase += separator;
        }
    }

    // Always append 2 random digits at the end for extra strength
    int num = 10 + rand() % 90; // gives a 2-digit number 10–99
    passphrase += to_string(num);

    return passphrase;
}


// ─────────────────────────────────────────────
//  FREE FUNCTION — asks user questions and
//  returns the generated password
//  (called from main or menu)
// ─────────────────────────────────────────────

// Helper to read a valid yes/no answer
bool askYesNo(const string& question) {
    string input;
    while (true) {
        cout << question << " (y/n): ";
        cin >> input;

        if (input == "y" || input == "Y" || input == "yes" || input == "YES") return true;
        if (input == "n" || input == "N" || input == "no"  || input == "NO")  return false;

        cout << "  Invalid input. Please enter y or n.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// Helper to read an integer within a range
int askInt(const string& question, int minVal, int maxVal) {
    string input;
    while (true) {
        cout << question;
        cin >> input;

        // Check all characters are digits
        bool valid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) { valid = false; break; }
        }

        if (valid) {
            int value = stoi(input);
            if (value >= minVal && value <= maxVal) return value;
        }

        cout << "  Invalid input. Please enter a number between "
             << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// The main generator flow — call this from your menu
string runPasswordGenerator() {
    cout << "\n========================================\n";
    cout << "         PASSWORD GENERATOR\n";
    cout << "========================================\n\n";

    // ── Question 1: Mode ───────────────────────
    cout << "Do you need to remember this password, or will it be saved in the vault?\n";
    cout << "  [1] I need to remember it  (Phrase mode)\n";
    cout << "  [2] I will save it         (Random mode)\n";
    cout << "Enter choice: ";

    int modeChoice = askInt("", 1, 2);

    string generatedPassword = "";

    // ═══════════════════════════════════
    //  PHRASE MODE
    // ═══════════════════════════════════
    if (modeChoice == 1) {
        PhraseGenerator pg;

        // Word count
        int words = askInt("\nHow many words? (2 / 3 / 4): ", 2, 4);
        pg.setWordCount(words);

        // Separator
        cout << "\nSeparator between words?\n";
        cout << "  [1] None   — BlueStorm49\n";
        cout << "  [2] Dash   — Blue-Storm-49\n";
        cout << "  [3] Symbol — Blue$Storm$49\n";
        int sepChoice = askInt("Enter choice: ", 1, 3);

        char sep = '\0';
        if (sepChoice == 2) sep = '-';
        if (sepChoice == 3) sep = '$';
        pg.setSeparator(sep);

        try {
            generatedPassword = pg.generate();
        } catch (exception& e) {
            cout << "\nError generating password: " << e.what() << "\n";
            return "";
        }
    }

    // ═══════════════════════════════════
    //  RANDOM MODE
    // ═══════════════════════════════════
    else {
        // Strength preset
        cout << "\nHow strong does it need to be?\n";
        cout << "  [1] Basic   (good for low-risk accounts)\n";
        cout << "  [2] Strong  (recommended for personal accounts)\n";
        cout << "  [3] Maximum (banking, work, email)\n";
        int strength = askInt("Enter choice: ", 1, 3);

        bool upper   = true;
        bool digits  = true;
        bool symbols = true;
        bool avoid   = false;
        int  len     = 12;

        if (strength == 1) {
            // Basic — ask everything manually
            len     = askInt("\nPassword length (8 to 64): ", 8, 64);
            upper   = askYesNo("Include uppercase letters? (A-Z)");
            digits  = askYesNo("Include numbers?           (0-9)");
            symbols = askYesNo("Include symbols?           (@#$!)");
            avoid   = askYesNo("Avoid look-alike characters? (0/O, 1/l)");

        } else if (strength == 2) {
            // Strong — set good defaults, let them adjust length
            len = askInt("\nPassword length (12 to 64, recommended 16): ", 12, 64);
            // uppercase, digits, symbols all ON — no need to ask

        } else {
            // Maximum — everything enabled automatically
            len = 20;
            cout << "\nMaximum mode selected. All character types enabled. Length set to 20.\n";
        }

        // Validate that at least one character type is on
        if (!upper && !digits && !symbols) {
            // lowercase is always on, but warn the user
            cout << "\nNote: Only lowercase letters will be used. This makes a weaker password.\n";
        }

        RandomGenerator rg(len, upper, digits, symbols, avoid);

        try {
            generatedPassword = rg.generate();
        } catch (exception& e) {
            cout << "\nError generating password: " << e.what() << "\n";
            return "";
        }
    }

    // ── Show result ────────────────────────────
    cout << "\n----------------------------------------\n";
    cout << "  Generated Password:  " << generatedPassword << "\n";
    cout << "----------------------------------------\n";
    cout << "\nTip: You can now evaluate this password using Option 1 from the main menu.\n";

    return generatedPassword;
}
