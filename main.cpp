#include <iostream>
#include "PasswordGenerator.h"
using namespace std;

// Declared in PasswordGenerator.cpp
string runPasswordGenerator();

int main() {
    cout << "====================================\n";
    cout << "  Password Generator — Test Run\n";
    cout << "====================================\n";

    char again = 'y';

    while (again == 'y' || again == 'Y') {
        runPasswordGenerator();

        cout << "\nGenerate another password? (y/n): ";
        cin >> again;
        cin.ignore(1000, '\n');
    }

    cout << "\nGoodbye!\n";
    return 0;
}
