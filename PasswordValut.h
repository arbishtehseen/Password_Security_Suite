#ifndef PasswordVault_h
#define PasswordVault_h
#include<fstream>
#include "PasswordEntry.h"
#include "AuthGuard.h"
#include<string>
#include<vector>

class PasswordVault{
	private:
		AuthGuard guard;
		std::vector<PasswordEntry> entries;
	    bool isOpen;
	    std::string filePath;
	    
	// private functions , that cannot be accessed publically
	//encryption
	std::string encryptpass(std::string password);
	std::string decryptpass(std::string password);
	void saveToFile();
	void loadFromFile();
	
	public:
		
		// default constructor
		PasswordVault();
		
		//parameterised constructor
		PasswordVault(std::string masterpassword , std::string filepath);
		
		//here keeping functions that can be accessed publically
		int getentryCount() const;
		
		// total list of entries
		const PasswordEntry* getAllEntries() const;
		
		// deleting entries
		bool deleteEntry(std::string label);
		
		// searching for an entry
		PasswordEntry* searchEntry(std::string label);
		
		// saving entry
		void saveEntry(std::string label , std::string password);
	
	    // accessing entry
	    bool open(std::string masterpassword); 
	    
	    //destructor
	    ~PasswordVault();
	
};

#endif