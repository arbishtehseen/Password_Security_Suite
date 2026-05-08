#include "HealthDashboard.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
static void setDashColor(int colorCode) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
#else
    (void)colorCode;
#endif
}

string decryptForCheck(string p) {
    for(int i = 0; i < p.size(); i++) p[i] ^= 9;
    return p;
}

void HealthDashboard::analyzeVault(const vector<PasswordEntry>& entries, 
LeakChecker& lc, PasswordEvaluator& pe) {
    totalEntries = entries.size();
    compromisedCount = 0;
    weakCount = 0;

    cout << "\n--- Initializing Vault Health Scan ---" << endl;

    for (const auto& entry : entries) {
        string plainPass = decryptForCheck(entry.getEncryptedPassword()); 

        bool leaked = lc.isLeaked(plainPass);
        
        try {
            pe.setPassword(plainPass);
            pe.evaluate();
        } catch (...) { continue; } 

        int score = pe.getScore();

        if (leaked) compromisedCount++;
        if (score < 50) weakCount++;

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
    if (healthScore >= 80) setDashColor(10); 
    else if (healthScore >= 50) setDashColor(14); 
    else setDashColor(12);

    cout << (int)healthScore << "%" << endl;
    setDashColor(7);
    cout << "========================================\n" << endl;
}
