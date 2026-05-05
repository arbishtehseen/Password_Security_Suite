#include "LeakChecker.h"
#include <fstream>
#include <iostream>  
using namespace std;

LeakChecker::LeakChecker(){}

LeakChecker::LeakChecker(string filename) {
    ifstream file(filename);
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if(!line.empty()) {
                commonLeaks.push_back(line); 
            }
        }
        file.close();
    } else {
        cout << "Warning: Leaks file (" << filename << ") not found!" << endl;
    }
}

bool LeakChecker::isLeaked(string password) {
    for (const string& leakedPass : commonLeaks) {
        if (password == leakedPass) {
            return true; 
        }
    }
    return false;
}