#include "PasswordEntry.h"
#include<string>
#include <stdexcept>  
#include <cctype> 
#include<iostream>
using namespace std;

// Helper Function
bool PasswordEntry::isValidFormat(const string& date) const {
	if(date.empty()){
		return false;
	}
    if (date.size() != 10) {
    	return false; }
    if (date[2] != '-' || date[5] != '-') {
	    return false; }
    int digitPositions[] = {0, 1, 3, 4, 6, 7, 8, 9};
    for (int i : digitPositions) {
        if (!isdigit(date[i])) {
		return false;
		}
    }
    return true;
}

//defaultconstructor
PasswordEntry::PasswordEntry() : label("N/A") , dateadded("N/A") ,
 encryptedpassword("N/A"){}
 
//parameterised constructor
PasswordEntry::PasswordEntry(string label, string dateadded, string encryptedpassword) 
    : label(!label.empty()? label : "N/A") , 
    dateadded("N/A") , 
    encryptedpassword(!encryptedpassword.empty() ? encryptedpassword : "N/A") {
	
	if(isValidFormat(dateadded)){
	
	try{
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
    catch(invalid_argument& e){
    	this->dateadded = "N/A";
	}
	catch(out_of_range& e){
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

void PasswordEntry::display() const{
	cout<<"Label : " << getLabel() <<endl;
	cout<<"DateAdded : " << getDateAdded() <<endl;
	cout<<"EncryptedPassword : " << getEncryptedPassword() <<endl;
}
