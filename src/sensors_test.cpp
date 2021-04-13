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
#include <Python.h>
#include <cstdio>
#include <mutex>
#include <raspicam/raspicam.h>
#include <memory>
#include "pstream.h"
#include <stdexcept>
#include <string>
#include <array>
#include <regex>
#include <future>
using namespace std;
using json = nlohmann::json;

#define PORT_SONAR 8080
#define PORT_SONAR_VEL 7070
#define PORT_HALL 6000
#define PORT_UDP 8888
#define PORT_DATA 4070
#define PORT_IMG 6969
#define MAXLINE 1024
#define DT 50 // distance threshold
#define VT 10 // velocity threshold
char hostIp[MAXLINE];
/** Set old distance to calculate the derivative and get velocity */
volatile double old_distance = 0;
volatile int upcoming_car = 1;
volatile int distance_flag = 0;
volatile int bike_flag = 0;
volatile int velocity_flag = 0;
volatile int interrupt = 0;
std::mutex mtx;

struct conditions {
  double car_distance;
  double car_velocity;
  double bike_velocity;
};

volatile conditions conds;

/** Sample Call Back class inheriting SensorCallback, 
    associated with the hall effect sensor
 */
class hallSampleCallback : public SensorCallback{
    /** Call back functin associated with hall effect sensor to return the velocity readiings.
    */
    /** @param v - velocity of bicycle wheel reading from hall sensor
    */
  virtual void dataIn(double v, bool isInterrupt = false){
    conds.bike_velocity = v;
    if(isInterrupt == false){
      int sock = 0, conn_status;
      auto time_now = chrono::system_clock::now();
      time_t timestamp = chrono::system_clock::to_time_t(time_now);
      //cout << "TIMESTAMP HALL: " << ctime(&timestamp) << endl;
      char time_data[20];
      strftime(time_data, 20, "%H:%M:%S",localtime(&timestamp));
      json json_data;
      json_data["distance"] = (double)-1.0;
      json_data["velocity"] = v;
      json_data["timestamp"] = time_data;
      string data = json_data.dump();
      const char *buffer_data = data.c_str();
      /** Send sensor readings to app */
      struct sockaddr_in server_addr;
      sock = socket (AF_INET, SOCK_STREAM, 0);
      if(inet_pton(AF_INET, hostIp, &server_addr.sin_addr) <= 0){
	  printf("\nInvalid address/ Address not supported \n");
      }
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(PORT_HALL);
      conn_status = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
      if(conn_status < 0){
	   perror("ERROR connecting\n");
      }
      else {
	   send(sock, buffer_data, strlen(buffer_data), 0);
	   close(sock);
      }
    }
    else{
      if(upcoming_car == 1 && v >= VT && bike_flag == 0){
	bike_flag = 1;
      }
    }
    }
};

/** Sample Call Back class inheriting SensorCallback, 
    associated with the sonar sensor
 */
class sonarDistanceSampleCallback : public SensorCallback{
    /** Call back functin associated with sonar sensor to return the velocity readiings.
    */
    /** @param t - time of echo heading t obstacle and coming back as reading from sonar sensor
    */
  virtual void dataIn(double t, bool isInterrupt = false){
        auto time_now = chrono::system_clock::now();
        time_t timestamp = chrono::system_clock::to_time_t(time_now);
	double distance = t/58;
	double v = abs((old_distance - (distance)))/10; //speed of incoming item
	conds.car_distance = distance;
	conds.car_velocity = v;
	//cout << "TIMESTAMP SONAR: " << ctime(&timestamp) << endl;
	char time_data[20];
	strftime(time_data, 20, "%H:%M:%S",localtime(&timestamp));
	json json_data;
	json_data["distance"] = distance;
	json_data["velocity"] = v;
	json_data["timestamp"] = time_data;
        string data = json_data.dump();
        const char *buffer_data = data.c_str();
        /** Send sensor readings to app */
        int sock = 0, conn_status;
        struct sockaddr_in server_addr;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(inet_pton(AF_INET, hostIp, &server_addr.sin_addr) <= 0) {
          printf("\nInvalid address/ Address not supported \n");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT_SONAR);
        conn_status = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
        if(conn_status < 0){
          perror("ERROR connecting\n");
        }
        else {
	  send(sock, buffer_data, strlen(buffer_data), 0);
          close(sock);
          }
	old_distance = distance;
       }
  };

   string getLicensePlate(string res){
      istringstream f(res);
      string line; 
      string result = "";
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
        string best_result = "";
        for (auto x : match)
            best_result+=x;
        return best_result;
    }
   }
   
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

int main(int argc, char *argv[]){
  /** Get Phone address from UDP broadcast message*/
    cout << "Awaiting mobile connection..." << endl;
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_UDP);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (const struct sockaddr *)&serv_addr,sizeof(serv_addr));
    int n;
    socklen_t len;
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&serv_addr, &len);
    buffer[n] = '\0';
    strcpy(hostIp, buffer);
    close(sockfd);
    const int HALL = 0;
    const int SONAR = 1;
    int pinInHall = 1;
    int pinOutHall = 4;
    int pinInSonarOne = 24;
    int pinOutSonarOne = 23;
    int pinInSonarTwo = 10;
    int pinOutSonarTwo = 11;
    Sensor* sonarSensorOne = new Sensor(&pinInSonarOne, &pinOutSonarOne);
    Sensor* hallEffectSensor = new Sensor(&pinInHall,&pinOutHall);
    sonarDistanceSampleCallback sonarCallbackOne;
    hallSampleCallback hallCallback;
    sonarSensorOne->setCallBack(&sonarCallbackOne);
    hallEffectSensor->setCallBack(&hallCallback);
    auto start = chrono::system_clock::now();
    time_t timestamp_start = chrono::system_clock::to_time_t(start);
    sonarSensorOne->start(&pinInSonarOne, &pinOutSonarOne, SONAR);
    hallEffectSensor->start(&pinInHall, &pinOutHall, HALL);
    raspicam::RaspiCam Camera;
    if(!Camera.open()){
      cerr << "Error opening camera" << endl;
    }
    cout << "###### Stabilizing camera... #######" << endl;
    cout << "###### Camera configured ######" << endl;
    int captureCount = 0;
    //Py_Initialize();
    while(true){
      double car_distance = 0;
      double car_velocity = 0;
      double bike_velocity = 0;
     if(conds.car_distance <= DT && conds.car_velocity >= VT && conds.bike_velocity >= VT){
	captureCount++;
        car_distance = conds.car_distance;
	car_velocity = conds.car_velocity;
	bike_velocity = conds.bike_velocity;
        mtx.lock();
        cout << "####### CAPTURING IMAGE #######" << endl;
        Camera.grab();
        //allocate memory
        unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
        //extract the image in rgb format
        Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB ); //get camera image
        //save
        ofstream outFile ( "src/img.jpg",ios::binary );
        outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
        outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
        cout<<"Image saved at src/img.jpg"<<endl;
	string res = exec("alpr -c gb src/img.jpg");
	cout << "Capture No. " << captureCount << endl;
        string car_plate = getLicensePlate(res);
	cout << "RESULT: " << car_plate << endl;
        delete data;
	exec("python3 src/upload.py");
	//char filename[] = "src/upload.py";
	//FILE* fp;
	//fp = _Py_fopen(filename, "r");
	//PyRun_SimpleFile(fp, filename);
	//Send Data
	json json_data;
	json_data["car_plate"] = car_plate;
	json_data["car_distance"] = car_distance;
	json_data["car_velocity"] = car_velocity;
	json_data["bike_velocity"] = bike_velocity;
        string result_data = json_data.dump();
        const char *buffer_data = result_data.c_str();
        /** Send sensor readings to app */
        int sock = 0, conn_status;
        struct sockaddr_in server_addr;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(inet_pton(AF_INET, hostIp, &server_addr.sin_addr) <= 0) {
          printf("\nInvalid address/ Address not supported \n");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT_DATA);
        conn_status = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
        if(conn_status < 0){
          perror("ERROR connecting\n");
        }
        else {
	  send(sock, buffer_data, strlen(buffer_data), 0);
          close(sock);
	}
	mtx.unlock();
        }
      }
    getchar();
    sonarSensorOne->stop();
    hallEffectSensor->stop();
    printf("Done");
    delete sonarSensorOne;
    delete hallEffectSensor;
    return 0;
}
