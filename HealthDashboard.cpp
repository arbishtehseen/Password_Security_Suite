#include "HealthDashboard.h"
#include <iostream>
#include <windows.h>

using namespace std;

// Partner A ke color logic ko reuse karne ke liye helper
static void setDashColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}

// XOR Decryption function (vahi logic jo vault mein encryption ke liye hai)
string decryptForCheck(string p) {
    for(int i = 0; i < p.size(); i++) p[i] ^= 9; // XOR with 9 to reverse
    return p;
}

void HealthDashboard::analyzeVault(const vector<PasswordEntry>& entries, LeakChecker& lc, PasswordEvaluator& pe) {
    totalEntries = entries.size();
    compromisedCount = 0;
    weakCount = 0;

    cout << "\n--- Initializing Vault Health Scan ---" << endl;

    for (const auto& entry : entries) {
        // STEP 1: Decrypt encrypted password from vault
        string plainPass = decryptForCheck(entry.getEncryptedPassword()); 

        // STEP 2: Partner B - Leak Check (Aapki class)
        bool leaked = lc.isLeaked(plainPass);
        
        // STEP 3: Partner A - Strength Check (Evaluator class)
        try {
            pe.setPassword(plainPass);
            pe.evaluate();
        } catch (...) { continue; } // Error handling for empty passwords

        int score = pe.getScore();

        // Stats update karein
        if (leaked) compromisedCount++;
        if (score < 50) weakCount++;

        // Live status report
        cout << "Checking [" << entry.getLabel() << "]: ";
        if(leaked) {
            setDashColor(12); cout << "CRITICAL (LEAKED) "; 
        } else if (score < 50) {
            setDashColor(14); cout << "WEAK ";
        } else {
            setDashColor(10); cout << "SECURE ";
        }
        setDashColor(7); cout << endl;
    }
}

void HealthDashboard::displaySummary() const {
    setDashColor(11);
    cout << "\n========================================" << endl;
    cout << "          FINAL HEALTH REPORT          " << endl;
    cout << "========================================" << endl;
    setDashColor(7);

    cout << "Total Passwords Managed : " << totalEntries << endl;
    
    setDashColor(12);
    cout << "Compromised Accounts    : " << compromisedCount << endl;
    
    setDashColor(14);
    cout << "Vulnerable (Weak)       : " << weakCount << endl;
    
    setDashColor(11);
    cout << "----------------------------------------" << endl;
    
    float healthScore = (totalEntries > 0) ? 
        ((float)(totalEntries - compromisedCount) / totalEntries) * 100 : 0;

    cout << "OVERALL VAULT HEALTH   : ";
    if (healthScore >= 80) setDashColor(10); // Green
    else if (healthScore >= 50) setDashColor(14); // Yellow
    else setDashColor(12); // Red

    cout << (int)healthScore << "%" << endl;
    setDashColor(7);
    cout << "========================================\n" << endl;
}