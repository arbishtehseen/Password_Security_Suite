#ifndef PasswordEntry_h
#define PasswordEntry_h
#include<string>

class PasswordEntry{
	private:
		std::string label;
		std::string dateadded;
		std::string encryptedpassword;
		
	public:
		PasswordEntry();
		PasswordEntry(std::string label , std::string datadded , std::string encryptedpassword);
		
		std::string getLabel() const;
		std::string getDateAdded() const;
		std::string getEncryptedPassword() const;
};

#endif