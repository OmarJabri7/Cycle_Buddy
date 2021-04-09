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
#include <raspicam/raspicam.h>
#include <regex>
#include "sensors_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>
#define _USE_MATHS_DEFINES
#include <math.h>
#include <cstdio>
#include <mutex>
#include <raspicam/raspicam.h>
#include <memory>
#include "pstream.h"
#include <stdexcept>
#include <string>
#include <array>
#include <regex>

using namespace std;

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

int main(){
    raspicam::RaspiCam Camera;
    if(!Camera.open()){
      cerr << "Error opening camera" << endl;
    }
    cout << "####### CAPTURING IMAGE #######" << endl;
    Camera.grab();
    //allocate memory
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
    //extract the image in rgb format
    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB ); //get camera image
    //save
    ofstream outFile ( "img.jpg",ios::binary );
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
    cout<<"Image saved at img.jpg"<<endl;
    //sleep(5);
    string result = exec("alpr -c gb img.jpg");
    cout << result << endl;
    /*redi::ipstream proc("./simple_bash.sh", redi::pstreams::pstdout | redi::pstreams::pstderr);
    string line;
    string result = "";
    int counter = 0;
    while (std::getline(proc.out(), line)){
        //if(counter == 1){
            result+=line + "\n";
        //}
        counter++;
    }
    cout << result << endl;
    return 0;
    regex pattern("[A-Za-z0-9]+[A-Za-z]");
    smatch match;
    regex_search(result, match, pattern);
    string best_result = "";
    for (auto x : match)
        best_result+=x;
    cout << best_result << endl;*/
}
