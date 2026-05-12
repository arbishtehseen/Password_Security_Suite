#ifndef AuthGuard_h
#define AuthGuard_h
#include <string>
#include <ctime>

class AuthGuard {

private:
    std::string masterHash;
    int failCount;
    bool isLocked;
    time_t lockTime;

    std::string convertToHash(std::string masterpassword);
    void startLockTimer();

    // Encrypts/decrypts the hash string before writing to disk
    std::string encryptHash(std::string hash) const;

public:
    AuthGuard();
    AuthGuard(std::string masterpassword);

    // Save the encrypted hash to a .key file
    bool saveHashToFile(const std::string& keyFilePath) const;

    // Load and decrypt the hash from a .key file (replaces in-memory hash)
    bool loadHashFromFile(const std::string& keyFilePath);

    bool verifymasterPassword(std::string masterpassword);
    bool getLockstatus() const;
    void resetfailCount();
    int  getRemainingTime();
};

#endif
