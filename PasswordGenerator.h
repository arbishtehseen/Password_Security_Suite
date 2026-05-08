#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
using namespace std;

// Forward declaration
class PasswordVault;

// ─────────────────────────────────────────────
//  BASE CLASS  (Abstract)
//  Every generator must have a generate() method
// ─────────────────────────────────────────────
class PasswordGenerator {
protected:
    int length;
    bool useUppercase;
    bool useDigits;
    bool useSymbols;
    bool avoidLookAlikes; // avoid 0/O, 1/l/I confusion

public:
    // Constructor with sensible defaults
    PasswordGenerator(int len = 12,
                      bool upper = true,
                      bool digits = true,
                      bool symbols = true,
                      bool avoidConfusing = false);

    // Pure virtual — every subclass MUST implement this
    virtual string generate() = 0;

    // Virtual destructor (good OOP practice)
    virtual ~PasswordGenerator() {}

    // Setters with validation
    void setLength(int len);
    void setUseUppercase(bool val);
    void setUseDigits(bool val);
    void setUseSymbols(bool val);
    void setAvoidLookAlikes(bool val);

    // Getters
    int getLength() const;
};


// ─────────────────────────────────────────────
//  SUBCLASS 1 — Random Generator
//  Generates a fully random character password
// ─────────────────────────────────────────────
class RandomGenerator : public PasswordGenerator {
public:
    RandomGenerator(int len = 12,
                    bool upper = true,
                    bool digits = true,
                    bool symbols = true,
                    bool avoidConfusing = false);

    string generate() override;

private:
    string buildCharPool();
};


// ─────────────────────────────────────────────
//  SUBCLASS 2 — Phrase Generator
//  Generates a memorable word-based passphrase
//  e.g.  Blue$Tiger$Runs49
// ─────────────────────────────────────────────
class PhraseGenerator : public PasswordGenerator {
private:
    int wordCount;
    char separator;

public:
    PhraseGenerator(int words = 3, char sep = '$');

    void setWordCount(int words);
    void setSeparator(char sep);

    string generate() override;
};

string runPasswordGenerator();

#endif
