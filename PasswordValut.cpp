#include "PasswordVault.h"
#include<string>
using namespace std;

// default constructor
PasswordVault::PasswordVault() : isOpen(false) , filePath("N/A"){}

//parameterised constructor
PasswordVault::PasswordVault(string masterpassword , string filepath) : 
guard(masterpassword) , filePath(filepath), isOpen(false){}

int PasswordVault::getentryCount() const{
	return entries.size();
}

// Total list of entries
const PasswordEntry* PasswordVault::getAllEntries() const{
	if(isOpen){
        return entries;
    }
    else{
    	return nullptr;
	}

}

// deleting entries
bool PasswordVault::deleteEntry(string label){
	if(!isOpen){
		return false;
	}
	else{
		for( auto it = entries.begin() ; it != entries.end(); ++it ){
			if(it->getLabel() == label){
				entries.erase(it);
				saveToFile();
				return true;
			}
		}	
   }
    return false;
}

const PasswordEntry* PasswordVault::searchEntry(string label){
	
	if(!isOpen){
		return nullptr;
	}
	else{
		for(auto it = entries.begin() ; it != entries.end() ; ++it){
			if(it->getLabel() == label){
				return &(*it);
			}
		}
	}
	return nullptr;	
}

void PasswordVault::saveEntry(string label , string password){
	
	entries()
	
	
}






