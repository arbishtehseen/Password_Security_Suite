# Password Security Suite
**OOP Final Semester Project | 2nd Semester Computer Science**[cite: 1]

A modular C++ console application designed to evaluate, generate, and store passwords using real-world cybersecurity standards and Object-Oriented principles[cite: 1].

---

## 🚀 Key Features

*   **Password Evaluator**: Analyzes length, character diversity, and patterns to provide a "Time to Crack" estimate and targeted security suggestions[cite: 1].
*   **Intelligent Generator**: Features **Random Mode** for high-entropy strings and **Phrase Mode** for memorable passphrases[cite: 1].
*   **Encrypted Vault**: Persistent storage using **Symmetric Encryption**; passwords are encrypted before being written to the file system[cite: 1].
*   **AuthGuard**: Access control system requiring a Master Password, featuring a **30-second lockout** after three failed attempts[cite: 1].
*   **Leak Checker**: Cross-references passwords against a database of known compromised credentials from real-world breaches[cite: 1].
*   **Health Dashboard**: A "Master Controller" that scans the vault to provide an overall security score and identifies reused or weak accounts[cite: 1].

---

## 🏗 OOP Architecture
The project demonstrates mastery of core OOP concepts through a modular design[cite: 1]:

*   **Encapsulation**: Private data members protect sensitive logic within specialized classes[cite: 1].
*   **Abstraction**: Complex cryptographic and logic processes are hidden behind public interfaces[cite: 1].
*   **Inheritance & Polymorphism**: A base `PasswordGenerator` class is extended by `RandomGenerator` and `PhraseGenerator`, overriding the `generate()` method[cite: 1].
*   **File I/O**: The `PasswordVault` manages persistent `PasswordEntry` objects via encrypted file streams[cite: 1].

---

## 📂 Class Structure
*   **`PasswordGenerator`**: Abstract base class[cite: 1].
*   **`PasswordEvaluator`**: Strength analysis and pattern matching[cite: 1].
*   **`PasswordVault`**: Manages encrypted file reading and writing[cite: 1].
*   **`AuthGuard`**: Controls authentication state and lockout timers[cite: 1].
*   **`HealthDashboard`**: Orchestrates multiple classes for aggregate reporting[cite: 1].

---

## 👥 Team Division
*   **Partner A**: Feature 1 (Evaluator) and Feature 2 (Generator subclasses)[cite: 1].
*   **Partner B**: Feature 3 (Vault), Feature 4 (Leak Checker), and Feature 5 (Dashboard)[cite: 1].

---

## ⚙️ Execution
1.  **Compile**: `g++ main.cpp -o PasswordSecuritySuite`
2.  **Run**: `./PasswordSecuritySuite`

> **Note**: This project utilizes local `.txt` files for the leak database and vault persistence[cite: 1]. Ensure these files are present in the executable directory.
