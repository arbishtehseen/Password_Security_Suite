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
    // Start with lowercase — 'l' removed upfront (looks like 1)
    string pool = "abcdefghijkmnopqrstuvwxyz";

    if (avoidLookAlikes) {
        // Additionally remove 'o' (looks like 0)
        string cleaned = "";
        for (char c : pool) {
            if (c != 'o') cleaned += c;
        }
        pool = cleaned;
    }

    if (useUppercase) {
        if (avoidLookAlikes) {
            // Remove I (looks like 1) and O (looks like 0)
            pool += "ABCDEFGHJKLMNPQRSTUVWXYZ";
        } else {
            pool += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
    }

    if (useDigits) {
        // Remove 0 and 1 if avoiding look-alikes
        pool += avoidLookAlikes ? "23456789" : "0123456789";
    }

    if (useSymbols) {
        pool += "@#$!%&*";
    }

    return pool;
}

string RandomGenerator::generate() {
    string pool = buildCharPool();

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

        if (i < wordCount - 1) {
            passphrase += separator;
        }
    }

    // Always append 2 random digits at the end for extra strength
    int num = 10 + rand() % 90;
    passphrase += to_string(num);

    return passphrase;
}


// ─────────────────────────────────────────────
//  HELPER — read a valid yes/no answer
//
//  FIX: cin >> input leaves '\n' in the buffer.
//  We now call cin.ignore() after every cin >>
//  so the buffer is always clean for the next read.
// ─────────────────────────────────────────────
bool askYesNo(const string& question) {
    string input;
    while (true) {
        cout << question << " (y/n): ";
        cin >> input;
        cin.ignore(1000, '\n');   // ← FIX: flush leftover newline

        if (input == "y" || input == "Y" || input == "yes" || input == "YES") return true;
        if (input == "n" || input == "N" || input == "no"  || input == "NO")  return false;

        cout << "  Invalid input. Please enter y or n.\n";
    }
}

// ─────────────────────────────────────────────
//  HELPER — read an integer within a range
//
//  FIX: same cin.ignore() after every cin >>
// ─────────────────────────────────────────────
int askInt(const string& question, int minVal, int maxVal) {
    string input;
    while (true) {
        cout << question;
        cin >> input;
        cin.ignore(1000, '\n');   // ← FIX: flush leftover newline

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
    }
}

// ─────────────────────────────────────────────
//  FREE FUNCTION — runPasswordGenerator
//  Asks the user questions and returns the
//  generated password string.
// ─────────────────────────────────────────────
string runPasswordGenerator() {
    cout << "\n========================================\n";
    cout << "         PASSWORD GENERATOR\n";
    cout << "========================================\n\n";

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

        int words = askInt("\nHow many words? (2 / 3 / 4): ", 2, 4);
        pg.setWordCount(words);

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
            len     = askInt("\nPassword length (8 to 64): ", 8, 64);
            upper   = askYesNo("Include uppercase letters? (A-Z)");
            digits  = askYesNo("Include numbers?           (0-9)");
            symbols = askYesNo("Include symbols?           (@#$!)");
            avoid   = askYesNo("Avoid look-alike characters? (0/O, 1/l)");

        } else if (strength == 2) {
            len = askInt("\nPassword length (12 to 64, recommended 16): ", 12, 64);
            // uppercase, digits, symbols stay ON — no extra questions needed

        } else {
            // Maximum — everything on automatically
            len = 20;
            cout << "\nMaximum mode selected. All character types enabled. Length set to 20.\n";
        }

        if (!upper && !digits && !symbols) {
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

    // FIX: cin buffer must be fully clean before returning to main.
    // main.cpp does:  cin.ignore(1000, '\n')  for "Press Enter..."
    // Since askInt/askYesNo already flushed the buffer above,
    // getline here consumes the actual Enter press from the user
    // so main's cin.ignore doesn't swallow it and skip the prompt.
    cout << "\n  Press Enter to return to menu...";
    cin.ignore(1000, '\n');

    return generatedPassword;
}
