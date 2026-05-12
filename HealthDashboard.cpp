#include "HealthDashboard.h"
#include <iostream>
#include "WinCompat.h"
using namespace std;

// ── Colour helper ────────────────────────────────────────────────────────────
static void setDashColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}
static void resetDashColor() { setDashColor(7); }

// ── XOR decrypt (key = 9, matches PasswordVault) ─────────────────────────────
static string decryptForCheck(string p) {
    for (int i = 0; i < (int)p.size(); i++) p[i] ^= 9;
    return p;
}

// ── Constructor ───────────────────────────────────────────────────────────────
HealthDashboard::HealthDashboard()
    : totalEntries(0), compromisedCount(0), weakCount(0) {}

// ── analyzeVault ──────────────────────────────────────────────────────────────
void HealthDashboard::analyzeVault(const PasswordVault& vault,
                                   LeakChecker& lc,
                                   PasswordEvaluator& pe) {
    // Reset counters before every scan
    totalEntries     = vault.getentryCount();
    compromisedCount = 0;
    weakCount        = 0;

    if (totalEntries == 0) {
        setDashColor(14);
        cout << "\n  Vault is empty — nothing to scan.\n";
        resetDashColor();
        return;
    }

    cout << "\n";
    setDashColor(11);
    cout << "  --- Initializing Vault Health Scan ---\n";
    resetDashColor();

    vector<PasswordEntry> entries = vault.getAllEntries();

    for (int i = 0; i < (int)entries.size(); i++) {
        string plainPass = decryptForCheck(entries[i].getEncryptedPassword());

        bool leaked = lc.isLeaked(plainPass);

        int score = 0;
        try {
            pe.setPassword(plainPass);
            pe.evaluate();
            score = pe.getScore();
        } catch (...) {
            // Password has spaces or is otherwise unreadable — flag as weak
            weakCount++;
            setDashColor(14);
            cout << "  Checking [" << entries[i].getLabel() << "]: WEAK (invalid format)\n";
            resetDashColor();
            continue;
        }

        if (leaked)     compromisedCount++;
        if (score < 50) weakCount++;

        cout << "  Checking [" << entries[i].getLabel() << "]: ";
        if (leaked) {
            setDashColor(12);
            cout << "CRITICAL (LEAKED)";
        } else if (score < 50) {
            setDashColor(14);
            cout << "WEAK  (score: " << score << ")";
        } else {
            setDashColor(10);
            cout << "SECURE (score: " << score << ")";
        }
        resetDashColor();
        cout << "\n";
    }
}

// ── displaySummary ────────────────────────────────────────────────────────────
void HealthDashboard::displaySummary() const {
    setDashColor(11);
    cout << "\n  ========================================\n";
    cout << "           FINAL HEALTH REPORT\n";
    cout << "  ========================================\n";
    resetDashColor();

    cout << "  Total Passwords Managed : " << totalEntries     << "\n";

    setDashColor(12);
    cout << "  Compromised (Leaked)    : " << compromisedCount << "\n";
    setDashColor(14);
    cout << "  Vulnerable  (Weak)      : " << weakCount        << "\n";

    setDashColor(11);
    cout << "  ----------------------------------------\n";

    float healthScore = (totalEntries > 0)
        ? ((float)(totalEntries - compromisedCount) / totalEntries) * 100.0f
        : 0.0f;

    cout << "  OVERALL VAULT HEALTH    : ";
    if      (healthScore >= 80) setDashColor(10);
    else if (healthScore >= 50) setDashColor(14);
    else                        setDashColor(12);

    cout << (int)healthScore << "%\n";
    resetDashColor();
    cout << "  ========================================\n\n";
}
