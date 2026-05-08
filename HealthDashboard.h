#ifndef HEALTHDASHBOARD_H
#define HEALTHDASHBOARD_H

#include <vector>
#include <string>

// Forward declarations (Avoids circular dependency)
class PasswordEntry;
class LeakChecker;
class PasswordEvaluator;

class HealthDashboard {
private:
    int totalEntries;
    int compromisedCount;
    int weakCount;

public:
    // Constructor: initialize all counts to 0
    HealthDashboard();

    // Logic for analyzing the vault
    void analyzeVault(const std::vector<PasswordEntry>& entries, 
                      LeakChecker& lc, 
                      PasswordEvaluator& pe);

    // Final result display
    void displaySummary() const;
};

#endif
