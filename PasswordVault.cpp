#include "PasswordVault.h"
#include <string>
#include <ctime>
#include <iostream>
using namespace std;

// ═════════════════════════════════════════════════════════════════════════════
//  MasterPasswordFile
//  File: master_passwords.txt
//  Each line:   vaultname:encryptedpassword
// ═════════════════════════════════════════════════════════════════════════════

static const string MASTER_FILE = "master_passwords.txt";

// Shift every character by +7 to encrypt, by -7 to decrypt.
// Since we call it once to encrypt and once to decrypt it needs to be
// reversible — so we actually just use a fixed XOR with 77 (printable,
// stays in ASCII range for normal password characters).
string MasterPasswordFile::shift(const string& s) {
    string out = s;
    for (int i = 0; i < (int)out.size(); i++) {
        out[i] = (char)(out[i] ^ 77);  // XOR with 77 — same operation encrypts and decrypts
    }
    return out;
}

bool MasterPasswordFile::exists(const string& vaultName) {
    ifstream file(MASTER_FILE);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        // Strip Windows \r if present
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        // Find the colon separator
        int colon = -1;
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] == ':') { colon = i; break; }
        }
        if (colon == -1) continue;

        string name = line.substr(0, colon);
        if (name == vaultName) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool MasterPasswordFile::save(const string& vaultName, const string& plainPassword) {
    ofstream file(MASTER_FILE, ios::app);
    if (!file.is_open()) return false;

    // Store as:   vaultname:encryptedpassword
    file << vaultName << ":" << shift(plainPassword) << "\n";
    file.close();
    return true;
}

// Returns  1  if password is correct
//          0  if password is wrong
//         -1  if vault name not found
int MasterPasswordFile::verify(const string& vaultName, const string& plainPassword) {
    ifstream file(MASTER_FILE);
    if (!file.is_open()) return -1;

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        int colon = -1;
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] == ':') { colon = i; break; }
        }
        if (colon == -1) continue;

        string name      = line.substr(0, colon);
        string stored    = line.substr(colon + 1);   // this is the encrypted password

        if (name == vaultName) {
            file.close();
            // Decrypt the stored password and compare with what the user typed
            string decrypted = shift(stored);
            if (decrypted == plainPassword) return 1;   // correct
            else                            return 0;   // wrong
        }
    }
    file.close();
    return -1;  // vault name not found
}


// ═════════════════════════════════════════════════════════════════════════════
//  PasswordVault
// ═════════════════════════════════════════════════════════════════════════════

PasswordVault::PasswordVault()
    : vaultName(""), filePath(""), isOpen(false) {}

// ── create ────────────────────────────────────────────────────────────────────
bool PasswordVault::create(const string& name, const string& masterpassword) {
    if (masterpassword.size() < 8 || masterpassword.size() > 20)
        return false;

    if (MasterPasswordFile::exists(name))
        return false;

    if (!MasterPasswordFile::save(name, masterpassword))
        return false;

    vaultName = name;
    filePath  = name + ".csv";
    guard     = AuthGuard(masterpassword);

    loadFromFile();   // new vault — loads nothing, that is fine
    isOpen = true;
    return true;
}

// ── open ──────────────────────────────────────────────────────────────────────
bool PasswordVault::open(const string& name, const string& masterpassword) {
    // Check lockout first
    if (guard.getLockstatus()) return false;

    int result = MasterPasswordFile::verify(name, masterpassword);

    if (result == 1) {
        // Correct password
        vaultName = name;
        filePath  = name + ".csv";
        guard.resetfailCount();
        loadFromFile();
        isOpen = true;
        return true;
    }

    // Wrong password or vault not found — increment fail counter via guard.
    // guard was constructed as AuthGuard() so masterHash = "N/A".
    // Any call to verifymasterPassword will fail, incrementing failCount,
    // and locking after 3 attempts. That is exactly what we want.
    guard.verifymasterPassword(masterpassword);
    return false;
}

// ── Getters ───────────────────────────────────────────────────────────────────
bool   PasswordVault::getIsOpen()           const { return isOpen;                   }
int    PasswordVault::getentryCount()       const { return (int)entries.size();      }
bool   PasswordVault::isVaultLocked()             { return guard.getLockstatus();    }
int    PasswordVault::getLockRemainingTime()       { return guard.getRemainingTime(); }
string PasswordVault::decrypt(string enc)         { return decryptpass(enc);         }

vector<PasswordEntry> PasswordVault::getAllEntries() const {
    if (isOpen) return entries;
    return {};
}

const vector<PasswordEntry>& PasswordVault::getEntriesRef() const {
    return entries;
}

// ── Entry management ──────────────────────────────────────────────────────────
void PasswordVault::saveEntry(string label, string password) {
    if (!isOpen) return;

    for (int i = 0; i < (int)label.size(); i++)
        if (label[i] == ',') label[i] = ' ';

    if (searchEntry(label) != nullptr) {
        cout << "  Error: Label '" << label << "' already exists!\n";
        return;
    }

    time_t t  = time(0);
    tm*   now = localtime(&t);
    string d  = (now->tm_mday < 10 ? "0" : "")    + to_string(now->tm_mday);
    string m  = (now->tm_mon+1 < 10 ? "0" : "")   + to_string(now->tm_mon+1);
    string y  = to_string(now->tm_year + 1900);
    string formattedDate = d + "-" + m + "-" + y;

    entries.push_back(PasswordEntry(label, formattedDate, encryptpass(password)));
    saveToFile();
}

bool PasswordVault::deleteEntry(string label) {
    if (!isOpen) return false;
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->getLabel() == label) {
            entries.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

const PasswordEntry* PasswordVault::searchEntry(string label) {
    if (!isOpen) return nullptr;
    for (auto it = entries.begin(); it != entries.end(); ++it)
        if (it->getLabel() == label) return &(*it);
    return nullptr;
}

// ── XOR Encrypt / Decrypt ─────────────────────────────────────────────────────
string PasswordVault::encryptpass(string password) {
    char key = 9;
    for (int i = 0; i < (int)password.size(); i++) password[i] ^= key;
    return password;
}

string PasswordVault::decryptpass(string password) {
    char key = 9;
    for (int i = 0; i < (int)password.size(); i++) password[i] ^= key;
    return password;
}

// ── File I/O ──────────────────────────────────────────────────────────────────
void PasswordVault::saveToFile() {
    if (filePath.empty()) return;
    ofstream file(filePath);
    if (!file.is_open()) return;
    for (const auto& entry : entries) {
        file << entry.getLabel()             << ","
             << entry.getDateAdded()         << ","
             << entry.getEncryptedPassword() << "\n";
    }
    file.close();
}

void PasswordVault::loadFromFile() {
    entries.clear();
    if (filePath.empty()) return;
    ifstream file(filePath);
    if (!file.is_open()) return;
    string label, dateadded, encryptedpassword;
    while (getline(file, label, ',') &&
           getline(file, dateadded, ',') &&
           getline(file, encryptedpassword)) {
        if (!encryptedpassword.empty() && encryptedpassword.back() == '\r')
            encryptedpassword.pop_back();
        entries.push_back(PasswordEntry(label, dateadded, encryptedpassword));
    }
    file.close();
}
