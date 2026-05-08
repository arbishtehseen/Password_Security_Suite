#include "PasswordGenerator.h"
#include "PasswordVault.h"
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

string RandomGenerator::buildCharPool() {
    string pool = "abcdefghijkmnopqrstuvwxyz";

    if (avoidLookAlikes) {
        string cleaned = "";
        for (char c : pool) {
            if (c != 'o') cleaned += c;
        }
        pool = cleaned;
    }

    if (useUppercase) {
        if (avoidLookAlikes) {
            pool += "ABCDEFGHJKLMNPQRSTUVWXYZ";
        } else {
            pool += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
    }

    if (useDigits) {
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
        "Blue", "Tiger", "Storm", "Cloud", "River", "Eagle", "Stone", "Flame",
        "Swift", "Night", "Brave", "Solar", "Frost", "Cedar", "Maple", "Ocean",
        "Pixel", "Lunar", "Amber", "Steel", "Crisp", "Vivid", "Blaze", "Delta",
        "Flint", "Shadow", "Glacier", "Summit", "Zenith", "Orbit", "Pulse", "Canyon",
        "Prism", "Echo", "Falcon", "Gully", "Harbor", "Jungle", "Kite", "Legend",
        "Mist", "Nomad", "Onyx", "Panda", "Quartz", "Radar", "Siren", "Tulip",
        "Under", "Vortex", "Willow", "Xenon", "Yacht", "Zebra", "Alpha", "Beta"
    };
    int wordListSize = 56;

    string passphrase = "";

    for (int i = 0; i < wordCount; i++) {
        int index = rand() % wordListSize;
        passphrase += words[index];

        if (i < wordCount - 1) {
            passphrase += separator;
        }
    }

    int num = 10 + rand() % 90;
    passphrase += to_string(num);

    return passphrase;
}


// ─────────────────────────────────────────────
//  HELPER — read a valid yes/no answer
// ─────────────────────────────────────────────
bool askYesNo(const string& question) {
    string input;
    while (true) {
        cout << question << " (y/n): ";
        cin >> input;
        cin.ignore(1000, '\n');

        if (input == "y" || input == "Y" || input == "yes" || input == "YES") return true;
        if (input == "n" || input == "N" || input == "no"  || input == "NO")  return false;

        cout << "  Invalid input. Please enter y or n.\n";
    }
}

// ─────────────────────────────────────────────
//  HELPER — read an integer within a range
// ─────────────────────────────────────────────
int askInt(const string& question, int minVal, int maxVal) {
    string input;
    while (true) {
        cout << question;
        cin >> input;
        cin.ignore(1000, '\n');

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
// ─────────────────────────────────────────────
string runPasswordGenerator() {
    cout << "\n========================================\n";
    cout << "         PASSWORD GENERATOR\n";
    cout << "========================================\n\n";

    cout << "Do you need to remember this password, or will it be saved in the vault?\n";
    cout << "  [1] I need to remember it  (Phrase mode)\n";
    cout << "  [2] I will save it         (Random mode)\n";

    int modeChoice = askInt("Enter choice: ", 1, 2);

    string generatedPassword = "";

    // PHRASE MODE
    if (modeChoice == 1) {
        PhraseGenerator pg;

        int words = askInt("\nHow many words? (2 / 3 / 4): ", 2, 4);
        pg.setWordCount(words);

        cout << "\nSeparator between words?\n";
        cout << "  [1] None   -- BlueStorm49\n";
        cout << "  [2] Dash   -- Blue-Storm-49\n";
        cout << "  [3] Symbol -- Blue$Storm$49\n";
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

    // RANDOM MODE
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

        } else {
            len = 20;
            cout << "\nMaximum mode: All character types enabled. Length set to 20.\n";
        }

        if (!upper && !digits && !symbols) {
            cout << "\nNote: Only lowercase letters will be used.\n";
        }

        RandomGenerator rg(len, upper, digits, symbols, avoid);

        try {
            generatedPassword = rg.generate();
        } catch (exception& e) {
            cout << "\nError generating password: " << e.what() << "\n";
            return "";
        }
    }

    cout << "\n----------------------------------------\n";
    cout << "  Generated Password:  " << generatedPassword << "\n";
    cout << "----------------------------------------\n";

    // SAVE TO VAULT (Random mode only)
    if (modeChoice == 2) {
        cout << "\nWould you like to save this password to your vault? (y/n): ";
        string saveChoice;
        cin >> saveChoice;
        cin.ignore(1000, '\n');

        if (saveChoice == "y" || saveChoice == "Y") {
            string vaultPath;
            cout << "Enter vault file path (e.g., myvault.txt): ";
            getline(cin, vaultPath);

            if (vaultPath.empty()) {
                cout << "Invalid path.\n";
            } else {
                string masterPass;
                cout << "Enter your master password: ";
                cin >> masterPass;
                cin.ignore(1000, '\n');

                PasswordVault vault(masterPass, vaultPath);
                if (vault.open(masterPass)) {
                    string label;
                    cout << "\nEnter a label for this password (e.g., Gmail, Bank): ";
                    getline(cin, label);

                    if (!label.empty()) {
                        vault.saveEntry(label, generatedPassword);
                        cout << "\n[OK] Password saved to vault successfully!\n";
                    } else {
                        cout << "Invalid label.\n";
                    }
                } else {
                    cout << "\n[ERROR] Failed to open vault. Master password may be incorrect.\n";
                }
            }
        }
    }

    cout << "\nTip: You can evaluate this password using Option 1 from the main menu.\n";
    cout << "\n  Press Enter to return to menu...";
    cin.ignore(1000, '\n');

    return generatedPassword;
}
