#ifndef PasswordEntry_h
#define PasswordEntry_h
#include<string>

class PasswordEntry{
	private:
		std::string label;
		std::string dateadded;
		std::string encryptedpassword;
		
	public:
		//helper function
		bool isValidFormat(const std::string& dateadded) const;
		
		// Default constructor
		PasswordEntry();
		
		// Parameterised constructor
		PasswordEntry(std::string label , std::string datadded , std::string encryptedpassword);
		
		//Getters
		std::string getLabel() const;
		std::string getDateAdded() const;
		std::string getEncryptedPassword() const;
		
		// temp
		void display() const;
};
#endif