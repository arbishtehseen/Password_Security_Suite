#ifndef HEALTHDASHBOARD_H
#define HEALTHDASHBOARD_H

#include <vector>
#include <string>
#include "PasswordEntry.h"
#include "PasswordEvaluator.h"
#include "LeakChecker.h"

class HealthDashboard {
private:
    int compromisedCount;
    int weakCount;
    int totalEntries;

public:
    HealthDashboard() : compromisedCount(0), weakCount(0), totalEntries(0) {}

    // Main function jo saare passwords scan karega
    void analyzeVault(const std::vector<PasswordEntry>& entries, LeakChecker& lc, PasswordEvaluator& pe);
    
    // Result display karne ke liye
    void displaySummary() const;
};

#endif