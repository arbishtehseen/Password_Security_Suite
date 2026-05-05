#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "PasswordVault.h"
#include "LeakChecker.h"
#include "PasswordEvaluator.h"
#include "HealthDashboard.h"
#include "PasswordGenerator.h"

using namespace std;

// Styling helper
void printDivider(int color = 11) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
    cout << "==================================================" << endl;
    SetConsoleTextAttribute(h, 7);
}

int main() {
    // Phase 1: Initialization
    // Master password "Admin786" set kiya hai as per your previous logic
    PasswordVault vault("Admin786", "vault_data.csv"); 
    LeakChecker checker("Leaks.txt"); // Linked with your leaked list file
    PasswordEvaluator evaluator;
    HealthDashboard dashboard;

    string masterInput;
    int mainChoice;

    printDivider(11);
    cout << "          PASSWORD SECURITY SUITE v1.0" << endl;
    cout << "       NEDUET CS End-of-Semester Project" << endl;
    printDivider(11);

    // Phase 2: Secure Login (Using your AuthGuard via Vault)
    cout << "\n[SECURE ACCESS] Enter Master Password: ";
    cin >> masterInput;

    if (!vault.open(masterInput)) {
        printDivider(12);
        cout << "CRITICAL ERROR: Unauthorized Access or Vault Locked!" << endl;
        printDivider(12);
        return 0;
    }

    // Phase 3: Main Functional Loop
    do {
        printDivider(14);
        cout << "   WELCOME TO YOUR SECURE VAULT   " << endl;
        printDivider(14);
        cout << "1. Generate New Strong Password" << endl;
        cout << "2. Evaluate a Password's Strength" << endl;
        cout << "3. Save New Entry to Vault" << endl;
        cout << "4. View Vault Health (Analytics)" << endl;
        cout << "5. Search or Delete Entry" << endl;
        cout << "6. Exit & Lock" << endl;
        cout << "\nChoice: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                string generated = runPasswordGenerator();
                if (!generated.empty()) {
                    char save;
                    cout << "Save this to vault? (y/n): ";
                    cin >> save;
                    if (save == 'y' || save == 'Y') {
                        string label;
                        cout << "Enter Label: "; cin >> label;
                        vault.saveEntry(label, generated);
                        cout << "Saved successfully!" << endl;
                    }
                }
                break;
            }
            case 2:
                runPasswordEvaluator();
                break;
            case 3: {
                string label, pass;
                cout << "Enter Label (e.g. GitHub): "; cin >> label;
                cout << "Enter Password: "; cin >> pass;
                vault.saveEntry(label, pass);
                break;
            }
            case 4:
                // Integrating everything: Vault + LeakCheck + Evaluator
                dashboard.analyzeVault(vault.getAllEntries(), checker, evaluator);
                dashboard.displaySummary();
                break;
            case 5: {
                int sub;
                cout << "1. Search | 2. Delete: "; cin >> sub;
                string label;
                cout << "Enter Label: "; cin >> label;
                if (sub == 1) {
                    const PasswordEntry* e = vault.searchEntry(label);
                    if (e) e->display();
                    else cout << "Not found!" << endl;
                } else {
                    if (vault.deleteEntry(label)) cout << "Deleted." << endl;
                    else cout << "Error deleting." << endl;
                }
                break;
            }
        }
        if (mainChoice != 6) {
            cout << "\nPress Enter to return to menu...";
            cin.ignore(1000, '\n');
            cin.get();
        }

    } while (mainChoice != 6);

    cout << "Vault state saved. System Locked." << endl;
    return 0;
}