#ifndef HEALTHDASHBOARD_H
#define HEALTHDASHBOARD_H

#include <vector>
#include <string>
#include "PasswordEntry.h"
#include "LeakChecker.h"
#include "PasswordEvaluator.h"
#include "PasswordVault.h"

class HealthDashboard {
private:
    int totalEntries;
    int compromisedCount;
    int weakCount;

public:
    // Constructor — zero-initialises all counters
    HealthDashboard();

    // Analyzes the vault for leaked or weak passwords
    void analyzeVault(const PasswordVault& vault,
                      LeakChecker& lc,
                      PasswordEvaluator& pe);

    // Prints a formatted summary of the vault health
    void displaySummary() const;
};

#endif

