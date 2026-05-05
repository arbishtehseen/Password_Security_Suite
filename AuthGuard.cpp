#include "AuthGuard.h"
#include<string>
using namespace std;


// helperfunction --> convertTohash
string AuthGuard::convertToHash(string masterpassword){
	string hash;
	
	if(masterpassword.size() >= 8 && masterpassword.size() <= 20){
	int sum = 0;
	for(int i = 0 ; i < masterpassword.size() ; i++){
		sum+=masterpassword[i];
	    }
	     hash = to_string(sum);
    }
    else{
    	hash = "N/A";
	}
	return hash;
}

// default constructor
AuthGuard::AuthGuard() : masterHash("N/A") , failCount(0) , isLocked(false) ,
  lockTime(0){}
  
// parameterised constructor
AuthGuard::AuthGuard(string masterpassword) : masterHash(convertToHash(masterpassword)) ,
    failCount(0) , isLocked(false) ,lockTime(0) {}
    
   // verify masterpassword
   bool AuthGuard::verifymasterPassword(string masterPassword) {
   	
   	if(!getLockstatus()){
   		if(convertToHash(masterPassword) == masterHash){
   			resetfailCount();
   			return true;
		   }
		else{
			failCount++; 
			if(failCount == 3){
				isLocked = true;
				startLockTimer();
			}
			return false;
		}
	   }
   	else{
   		return false;
	   }
   }
    
bool AuthGuard::getLockstatus() const{
	return isLocked;
}

void AuthGuard::resetfailCount(){
	failCount = 0;
}

void AuthGuard::startLockTimer() {
    lockTime = time(0); 
}

int AuthGuard::getRemainingTime() {
    if (!isLocked) return 0;
    double elapsed = difftime(time(0), lockTime);
    int remaining = 30 - (int)elapsed;
    if (remaining <= 0) {
        isLocked = false;
        failCount = 0;
        return 0;
    }
    return remaining;
}







