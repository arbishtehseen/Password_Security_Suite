#include "PasswordVault.h"
#include<string>
#include<ctime>
#include<iostream>
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
vector <PasswordEntry> PasswordVault::getAllEntries() const{
	if(isOpen){
        return entries;
    }
    else{
    	return {};
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
	
	if(isOpen){
		for(int i = 0 ; i < label.size() ; i++){
           	if(label[i] == ','){
           		label[i] = ' ';
				}
			}
			
		if(searchEntry(label) != nullptr) {
            cout << "Error: Label '" << label << "' already exists!" << endl;
            return; 
        }
			
		time_t t = time(0);
        tm* now = localtime(&t);
        
        string d = (now->tm_mday < 10 ? "0" : "") + to_string(now->tm_mday);
        string m = (now->tm_mon + 1 < 10 ? "0" : "") + to_string(now->tm_mon + 1);
        string y = to_string(now->tm_year + 1900);
        string formattedDate = d + "-" + m + "-" + y;

		entries.push_back(PasswordEntry(label, formattedDate, encryptpass(password)));
		
		saveToFile();
	}
}

bool PasswordVault::open(string masterpassword){
	if(filePath == "N/A"){
		return false;
	}
	if(guard.verifymasterPassword(masterpassword)){
		loadFromFile();
		isOpen = true;
		return true;
	}
	else{
		return false;
	}
}

string PasswordVault::encryptpass(string password){
	char key = 9;
	
	for(int i = 0 ; i < password.size() ; i++){
		password[i]^=key;
	}
	return password;
}

string PasswordVault::decryptpass(string password){
	char key = 9;
	
	for(int i = 0 ; i < password.size() ; i++){
		password[i]^=key;
	}
	return password;
}


void PasswordVault::saveToFile(){
	if(filePath == "N/A"){
    	return;
	}
	
    ofstream file(filePath);
	
	if(!file.is_open()){
		return;
	}
	else{
		for(const auto& entry : entries){
			file<< entry.getLabel() <<","
			    <<  entry.getDateAdded() <<","
			    <<   entry.getEncryptedPassword() <<endl;
		}
		file.close();
	}
}

void PasswordVault::loadFromFile(){
	entries.clear();
	ifstream file(filePath);
	string label;
	string dateadded;
	string encryptedpassword;
	
	if(!file.is_open()){
		return;
	}
	else{
		while(getline(file, label, ',') && 
           getline(file, dateadded, ',') && 
           getline(file, encryptedpassword)){
		    entries.push_back(
		    PasswordEntry(label , dateadded , encryptedpassword));
		}
		file.close();
	}	
}
