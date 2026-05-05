#ifndef AuthGuard_h
#define AuthGuard_h
#include<string>
#include<ctime>

class AuthGuard{
	
	private:
		// private member variables
		std::string masterHash;
		int failCount;
		bool isLocked;
		time_t lockTime;
		
		// private helper functions that shouldn't be accessed publically.
		// convert to Hash
	    std::string convertToHash(std::string masterpassword);
	    void startLockTimer();
		
	public:
		
		//Default constructor
		AuthGuard();
		
		//Parameterised Constructor
		AuthGuard(std::string masterpassword);
	
	    // verify password
	    bool verifymasterPassword(std::string masterpassword) ;
	    bool getLockstatus() const;
	    void resetfailCount();
	    int getRemainingTime();

};


#endif