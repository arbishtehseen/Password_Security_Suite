#include "AuthGuard.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

// ── convertToHash ─────────────────────────────────────────────────────────────
// Produces a numeric checksum from the master password.
// Kept simple intentionally — the real protection comes from
// encryptHash() before the value ever touches disk.
string AuthGuard::convertToHash(string masterpassword) {
    if (masterpassword.size() < 8 || masterpassword.size() > 20)
        return "N/A";

    int sum = 0;
    for (int i = 0; i < (int)masterpassword.size(); i++)
        sum += masterpassword[i] * (i + 1);   // position-weighted so "abc" != "bca"

    return to_string(sum);
}

// ── encryptHash ───────────────────────────────────────────────────────────────
// Multi-layer XOR obfuscation applied to the hash string before it is
// written to disk.  The same function decrypts (XOR is symmetric).
string AuthGuard::encryptHash(string hash) const {
    // Layer 1: XOR every byte with a fixed key
    const char KEY1 = 0x5A;
    for (int i = 0; i < (int)hash.size(); i++)
        hash[i] ^= KEY1;

    // Layer 2: XOR each byte with its position index
    for (int i = 0; i < (int)hash.size(); i++)
        hash[i] ^= (char)(i + 1);

    // Layer 3: reverse the string
    int l = 0, r = (int)hash.size() - 1;
    while (l < r) { char tmp = hash[l]; hash[l] = hash[r]; hash[r] = tmp; l++; r--; }

    return hash;
}

// ── saveHashToFile ────────────────────────────────────────────────────────────
bool AuthGuard::saveHashToFile(const string& keyFilePath) const {
    if (masterHash == "N/A") return false;

    ofstream file(keyFilePath, ios::binary);
    if (!file.is_open()) return false;

    string encrypted = encryptHash(masterHash);
    file.write(encrypted.c_str(), (int)encrypted.size());
    file.close();
    return true;
}

// ── loadHashFromFile ──────────────────────────────────────────────────────────
bool AuthGuard::loadHashFromFile(const string& keyFilePath) {
    ifstream file(keyFilePath, ios::binary);
    if (!file.is_open()) return false;

    string encrypted((istreambuf_iterator<char>(file)),
                      istreambuf_iterator<char>());
    file.close();

    if (encrypted.empty()) return false;

    masterHash = encryptHash(encrypted);   // XOR is symmetric — same function decrypts
    return true;
}

// ── Constructors ──────────────────────────────────────────────────────────────
AuthGuard::AuthGuard()
    : masterHash("N/A"), failCount(0), isLocked(false), lockTime(0) {}

AuthGuard::AuthGuard(string masterpassword)
    : masterHash(convertToHash(masterpassword)),
      failCount(0), isLocked(false), lockTime(0) {}

// ── verifymasterPassword ──────────────────────────────────────────────────────
bool AuthGuard::verifymasterPassword(string masterPassword) {
    if (getLockstatus()) return false;

    if (convertToHash(masterPassword) == masterHash) {
        resetfailCount();
        return true;
    }

    failCount++;
    if (failCount == 3) {
        isLocked = true;
        startLockTimer();
    }
    return false;
}

bool AuthGuard::getLockstatus() const { return isLocked; }
void AuthGuard::resetfailCount()      { failCount = 0;   }

void AuthGuard::startLockTimer() {
    lockTime = time(0);
}

int AuthGuard::getRemainingTime() {
    if (!isLocked) return 0;
    double elapsed = difftime(time(0), lockTime);
    int remaining  = 30 - (int)elapsed;
    if (remaining <= 0) {
        isLocked  = false;
        failCount = 0;
        return 0;
    }
    return remaining;
}

