//
//  simple_bash_call.cpp
//  
//
//  Created by Omar Jabri on 03/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include "pstream.h"
#include <stdexcept>
#include <string>
#include <string.h>
#include <array>
#include <regex>

using namespace std;

int main(){
    redi::ipstream proc("./simple_bash.sh", redi::pstreams::pstdout | redi::pstreams::pstderr);
    string line;
    string result = "";
    int counter = 0;
    while (std::getline(proc.out(), line)){
        if(counter == 1){
            result+=line + "\n";
        }
        counter++;
    }
    regex pattern("[A-Za-z0-9]+[A-Za-z]");
    smatch match;
    regex_search(result, match, pattern);
    string best_result = "";
    for (auto x : match)
        best_result+=x;
    cout << best_result << endl;
}
