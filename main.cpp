#include <iostream>
#include <windows.h>
#include "PasswordGenerator.h"
#include "PasswordEvaluator.h"
using namespace std;

// Declared in their respective .cpp files
string runPasswordGenerator();
void   runPasswordEvaluator();

// ── Colour helper (same as in PasswordEvaluator.cpp) ──
static void setColor(int code) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), code);
}
static void resetColor() { setColor(7); }

// ── Draw the main menu ──
void showMenu() {
    system("cls"); // clear screen on Windows

    setColor(11); // Cyan
    cout << "\n";
    cout << "  ==========================================\n";
    cout << "       PASSWORD SECURITY SUITE v1.0\n";
    cout << "  ==========================================\n";
    resetColor();

    setColor(8); // Dark grey
    cout << "  Your personal password security toolkit\n\n";
    resetColor();

    setColor(14); // Yellow
    cout << "  [1]";
    resetColor();
    cout << "  Evaluate a Password\n";

    setColor(14);
    cout << "  [2]";
    resetColor();
    cout << "  Generate a Password\n";

    setColor(8);
    cout << "\n  [3]  Password Vault        (coming soon)\n";
    cout << "  [4]  Leaked Password Check (coming soon)\n";
    cout << "  [5]  Health Dashboard      (coming soon)\n";
    resetColor();

    cout << "\n";
    setColor(12); // Red
    cout << "  [0]";
    resetColor();
    cout << "  Exit\n";

    setColor(11);
    cout << "\n  ==========================================\n";
    resetColor();
}

int main() {
    // Enable Windows console colours
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode);

    string input = "";
    int choice   = -1;

    while (true) {
        showMenu();

        setColor(8);
        cout << "  Enter your choice: ";
        resetColor();

        cin >> input;
        cin.ignore(1000, '\n');

        // ── Input validation ───────────────────
        bool valid = input.length() == 1 && isdigit(input[0]);

        if (!valid) {
            setColor(12);
            cout << "\n  Invalid input. Please enter a number from the menu.\n";
            resetColor();
            cout << "  Press Enter to continue...";
            cin.ignore(1000, '\n');
            continue;
        }

        choice = stoi(input);

        if (choice == 0) {
            setColor(10);
            cout << "\n  Goodbye! Stay secure.\n\n";
            resetColor();
            break;
        }

        switch (choice) {
            case 1:
                runPasswordEvaluator();
                cout << "\n  Press Enter to return to menu...";
                cin.ignore(1000, '\n');
                break;

            case 2:
                runPasswordGenerator();
                cout << "\n  Press Enter to return to menu...";
                cin.ignore(1000, '\n');
                break;

            case 3:
            case 4:
            case 5:
                setColor(14);
                cout << "\n  This feature is coming soon!\n";
                resetColor();
                cout << "  Press Enter to continue...";
                cin.ignore(1000, '\n');
                break;

            default:
                setColor(12);
                cout << "\n  Invalid choice. Please select from the menu.\n";
                resetColor();
                cout << "  Press Enter to continue...";
                cin.ignore(1000, '\n');
                break;
        }
    }

    return 0;
}
