# Password Security Suite
**OOP Final Semester Project | 2nd Semester Computer Science & Information Technology**

A modular C++ console application designed to evaluate, generate, and store passwords using real-world cybersecurity standards and Object-Oriented principles.

---

## 🚀 Key Features

*   **Password Evaluator**: Analyzes length, character diversity, and patterns to provide a "Time to Crack" estimate and targeted security suggestions.
*   **Intelligent Generator**: Features **Random Mode** for high-entropy strings and **Phrase Mode** for memorable passphrases.
*   **Encrypted Vault**: Persistent storage using **Symmetric Encryption**; passwords are encrypted before being written to the file system
*   **AuthGuard**: Access control system requiring a Master Password, featuring a **30-second lockout** after three failed attempts.
*   **Leak Checker**: Cross-references passwords against a database of known compromised credentials from real-world breaches.
*   **Health Dashboard**: A "Master Controller" that scans the vault to provide an overall security score and identifies reused or weak accounts.

---

## 🏗 OOP Architecture
The project demonstrates mastery of core OOP concepts through a modular design:

*   **Encapsulation**: Private data members protect sensitive logic within specialized classes.
*   **Abstraction**: Complex cryptographic and logic processes are hidden behind public interfaces.
*   **Inheritance & Polymorphism**: A base `PasswordGenerator` class is extended by `RandomGenerator` and `PhraseGenerator`, overriding the `generate()` method.
*   **File I/O**: The `PasswordVault` manages persistent `PasswordEntry` objects via encrypted file streams.

---

## 📂 Class Structure
*   **`PasswordGenerator`**: Abstract base class.
*   **`PasswordEvaluator`**: Strength analysis and pattern matching.
*   **`PasswordVault`**: Manages encrypted file reading and writing.
*   **`AuthGuard`**: Controls authentication state and lockout timers.
*   **`HealthDashboard`**: Orchestrates multiple classes for aggregate reporting.

---

## 👥 Team Division
*   **Partner A**: Feature 1 (Evaluator) and Feature 2 (Generator subclasses).
*   **Partner B**: Feature 3 (Vault), Feature 4 (Leak Checker), and Feature 5 (Dashboard).

---

## ⚙️ Execution
1.  **Compile**: `g++ main.cpp -o PasswordSecuritySuite`
2.  **Run**: `./PasswordSecuritySuite`

> **Note**: This project utilizes local `.txt` files for the leak database and vault persistence. Ensure these files are present in the executable directory.
