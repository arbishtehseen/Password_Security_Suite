#ifndef PASSWORDEVALUATOR_H
#define PASSWORDEVALUATOR_H

#include <string>
using namespace std;

// ─────────────────────────────────────────────
//  Strength levels — returned after evaluation
// ─────────────────────────────────────────────
enum StrengthLevel {
    VERY_WEAK,
    WEAK,
    MODERATE,
    STRONG,
    VERY_STRONG
};


// ─────────────────────────────────────────────
//  PasswordEvaluator Class
//  Evaluates a password across 5 criteria and
//  produces a full strength report
// ─────────────────────────────────────────────
class PasswordEvaluator {
private:
    string password;
    int    score;          // 0 to 100
    StrengthLevel level;

    // ── Private checker methods (encapsulation) ──
    bool checkLength()         const;
    bool checkUppercase()      const;
    bool checkDigits()         const;
    bool checkSymbols()        const;
    bool checkCommonPatterns() const;
    bool checkLeakedList()     const;

    // ── Private helpers ──
    void calculateScore();
    string getCrackTime() const;

public:
    // Constructor
    PasswordEvaluator();

    // Set the password to evaluate
    void setPassword(const string& pwd);

    // Run the full evaluation — call this before displaying results
    void evaluate();

    // Display the full coloured report to the console
    void displayReport() const;

    // Getters (used by HealthDashboard later)
    int          getScore()    const;
    StrengthLevel getLevel()   const;
    string       getPassword() const;
};

#endif
