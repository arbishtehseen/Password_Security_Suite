#include "PasswordEntry.h"
#include<string>
using namespace std;

//defaultconstructor
PasswordEntry::PasswordEntry() : label("N/A") , dateadded("N/A") ,
 encryptedpassword("N/A"){}
 
//parameterised constructor
PasswordEntry::PasswordEntry(string label, string dateadded, string encryptedpassword) 
    : label(!label.empty()? label : "N/A") , 
    dateadded("N/A") , 
    encryptedpassword(!encryptedpassword.empty() ? encryptedpassword : "N/A") {
	
	if(!dateadded.empty() && dateadded.size() == 10){
	    if(dateadded[2] == '-' && dateadded[5] == '-'){
	    	
	    	int day = stoi(dateadded.substr(0,2));
	    	int month = stoi(dateadded.substr(3,2));
	    	int year = stoi(dateadded.substr(6,4));
	    	
	        if((day >= 1 && day <= 31) &&
			(month >= 1 && month <= 12) && 
			(year >= 1 && year <= 2026)){
				
				this->dateadded = dateadded;
			}
			else {
                this->dateadded = "N/A";
            }		
		}
		else{
	     	this->dateadded = "N/A";
	   }
	}
	else{
		this->dateadded = "N/A";
	}
	
}

string PasswordEntry::getLabel() const{
	return label;
}

string PasswordEntry::getDateAdded() const{
	return dateadded;
}

string PasswordEntry::getEncryptedPassword() const{
	return encryptedpassword;
}