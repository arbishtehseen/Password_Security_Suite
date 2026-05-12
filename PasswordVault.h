#ifndef PASSWORDVAULT_H
#define PASSWORDVAULT_H

#include <vector>
#include <string>
#include <fstream>
#include "PasswordEntry.h"
#include "AuthGuard.h"

// ─────────────────────────────────────────────────────────────────────────────
//  MasterPasswordFile
//  A simple text file (master_passwords.txt) that stores every vault's
//  master password in encrypted form.
//
//  File format — one line per vault:
//      vaultname:encryptedpassword
//
//  "encrypted" means every character of the plain password is shifted
//  by +7 so it is never stored as plain text, but stays readable ASCII.
//  The same shift() function both encrypts and decrypts (it is reversible).
// ─────────────────────────────────────────────────────────────────────────────
class MasterPasswordFile {
public:
    // Encrypt or decrypt a password string (simple Caesar-style shift)
    static std::string shift(const std::string& s);

    // Returns true if a vault with this name already has an entry
    static bool exists(const std::string& vaultName);

    // Save a new entry:  vaultname:encrypt(password)
    static bool save(const std::string& vaultName,
                     const std::string& plainPassword);

    // Check if the given password matches the stored one for this vault
    // Returns: 1 = correct, 0 = wrong, -1 = vault not found
    static int verify(const std::string& vaultName,
                      const std::string& plainPassword);
};


// ─────────────────────────────────────────────────────────────────────────────
//  PasswordVault
// ─────────────────────────────────────────────────────────────────────────────
class PasswordVault {
private:
    std::vector<PasswordEntry> entries;
    AuthGuard   guard;
    std::string vaultName;
    std::string filePath;   // vaultName + ".csv"
    bool        isOpen;

    void saveToFile();
    void loadFromFile();
    std::string encryptpass(std::string password);
    std::string decryptpass(std::string password);

public:
    PasswordVault();

    // Create a brand-new vault and register its master password
    bool create(const std::string& name, const std::string& masterpassword);

    // Open an existing vault — verifies against master_passwords.txt
    bool open(const std::string& name, const std::string& masterpassword);

    bool getIsOpen()     const;
    int  getentryCount() const;
    std::vector<PasswordEntry> getAllEntries() const;

    bool isVaultLocked();
    int  getLockRemainingTime();

    void                 saveEntry(std::string label, std::string password);
    bool                 deleteEntry(std::string label);
    const PasswordEntry* searchEntry(std::string label);

    std::string decrypt(std::string encryptedPassword);
    const std::vector<PasswordEntry>& getEntriesRef() const;
};

#endif
