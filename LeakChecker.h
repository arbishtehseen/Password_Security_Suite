#ifndef LeakChecker_h
#define LeakChecker_h
#include<vector>
#include<string>

class LeakChecker{
  	private:
  		std::vector<std::string> commonLeaks;
	
	public:
		LeakChecker();
		LeakChecker(std::string filename);
		bool isLeaked(std::string password);
};
#endif
