#ifndef AuthGuard_h
#define AuthGuard_h
#include<string>
#include<ctime>

class AuthGuard{
	
	private:
		std::string masterHash;
		int failCount;
		bool isLocked;
		time_t lockTime;
		
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
	    int getRemainingTime();
	    void resetfailCount();
	
	
	
	
	
};


#endif