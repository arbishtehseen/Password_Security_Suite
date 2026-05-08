#ifndef HEALTHDASHBOARD_H
#define HEALTHDASHBOARD_H

#include <vector>

#include "LeakChecker.h"
#include "PasswordEntry.h"
#include "PasswordEvaluator.h"

class HealthDashboard {
private:
    int totalEntries;
    int compromisedCount;
    int weakCount;

public:
    HealthDashboard() : totalEntries(0), compromisedCount(0), weakCount(0) {}
    void analyzeVault(const std::vector<PasswordEntry>& entries, LeakChecker& lc, PasswordEvaluator& pe);
    void displaySummary() const;
};

#endif
