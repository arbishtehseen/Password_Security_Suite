#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "HealthDashboard.h"
#include "LeakChecker.h"
#include "PasswordEvaluator.h"
#include "PasswordGenerator.h"
#include "PasswordVault.h"

using namespace std;

static int readMenuChoice() {
    int choice;
    cout << "\n========================================\n";
    cout << "         PASSWORD SECURITY SUITE\n";
    cout << "========================================\n";
    cout << "1. Evaluate Password\n";
    cout << "2. Generate Password\n";
    cout << "3. Vault Health Dashboard\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

int main() {
    while (true) {
        int choice = readMenuChoice();

        if (choice == 1) {
            try {
                PasswordEvaluator evaluator;
                string pwd;
                cout << "Enter password to evaluate: ";
                getline(cin, pwd);
                evaluator.setPassword(pwd);
                evaluator.evaluate();
                evaluator.displayReport();
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        } else if (choice == 2) {
            runPasswordGenerator();
        } else if (choice == 3) {
            string masterPassword;
            string vaultPath;

            cout << "Enter vault file path: ";
            getline(cin, vaultPath);
            cout << "Enter master password: ";
            getline(cin, masterPassword);

            PasswordVault vault(masterPassword, vaultPath);
            if (!vault.open(masterPassword)) {
                cout << "Failed to open vault.\n";
                continue;
            }

            vector<PasswordEntry> entries = vault.getAllEntries();
            LeakChecker checker("leaked_passwords.txt");
            PasswordEvaluator evaluator;
            HealthDashboard dashboard;
            dashboard.analyzeVault(entries, checker, evaluator);
            dashboard.displaySummary();
        } else if (choice == 4) {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
