#include "stdafx.h"

using namespace std;


/** function used in camera thread to get car license plate.
    */
    /** @param res - pattern recogniztion results from the online API
    */
std::string getLicensePlate(std::string res){
    std::istringstream f(res);
    std::string line; 
    std::string result = "";
    int counter = 0;   
    while (std::getline(f, line)) {
		if(counter == 1){
			result+=line + "\n";
		}
    counter++;
	}
      if(result == ""){
        return "No license plate found.";
    }
    else {
        regex pattern("[A-Za-z0-9]+[A-Za-z]");
        smatch match;
        regex_search(result, match, pattern);
        std::string best_result = "";
        for (auto x : match)
            best_result+=x;
        return best_result;
    }
}

/** function in camera thread to call for python script that used to upload the data to firebase.
    */
    /** @param cmd - python cmd that calls for the python script.
    */  
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}