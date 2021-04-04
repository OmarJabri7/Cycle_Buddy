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
#include <jsoncpp/json/json.h>
#define _USE_MATHS_DEFINES
#include <math.h>
#include <cstdio>
#include <mutex>
using namespace std;

#define PORT_SONAR 8080
#define PORT_HALL 6000
#define PORT_UDP 8888
#define MAXLINE 1024
char hostIp[MAXLINE];
/** Set old distance to calculate the derivative and get velocity */
volatile float old_distance = 0;
volatile int upcoming_car = 1;
volatile int distance_flag = 0;
volatile int bike_flag = 0;
volatile int velocity_flag = 0;
std::mutex mtx;

/** Sample Call Back class inheriting SensorCallback, 
    associated with the hall effect sensor
 */
class hallSampleCallback : public SensorCallback{
    /** Call back functin associated with hall effect sensor to return the velocity readiings.
    */
    /** @param v - velocity of bicycle wheel reading from hall sensor
    */
    virtual void dataIn(double v){
      int sock = 0, conn_status;
      auto time_now = chrono::system_clock::now();
      time_t timestamp = chrono::system_clock::to_time_t(time_now);
      printf("Velocity: %f m/s\n", v);
      cout << "TIMESTAMP HALL: " << ctime(&timestamp) << endl;
      if(upcoming_car == 1 && v >= 8 && bike_flag == 0){
          mtx.lock();
          bike_flag = 1;
          mtx.unlock();
      }
      string data = to_string(v) + "|" + to_string(timestamp);
      const void *buffer_data = data.c_str();
      /** Send sensor readings to app */
      struct sockaddr_in server_addr;
      char buffer[2048] = {0};
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
	   send(sock, buffer_data, sizeof v, 0);
	   close(sock);
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
    virtual void dataIn(double t){
        auto time_now = chrono::system_clock::now();
        time_t timestamp = chrono::system_clock::to_time_t(time_now);
        printf("Distance: %f cm\n", t/58);
        cout << "TIMESTAMP SONAR: " << ctime(&timestamp) << endl;
        double distance = t/58;
        if(upcoming_car == 1 && distance <= 100 && distance_flag == 0){
          mtx.lock();
          distance_flag = 1;
          mtx.unlock();
        }
        string data = to_string(distance) + "|" + to_string(timestamp);
        const void *buffer_data = data.c_str();
        /** Send sensor readings to app */
        int sock = 0, conn_status;
        struct sockaddr_in server_addr;
        char buffer[2048] = {0};
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
          send(sock, buffer_data, sizeof distance, 0);
          close(sock);
          }
          }
  };

  class sonarVelocitySampleCallback : public SensorCallback {
        virtual void dataIn(double t){
          float v = (old_distance - (t/58))/10; //speed of incoming item
          if(v >= 1){
              auto time_now = chrono::system_clock::now();
              time_t timestamp = chrono::system_clock::to_time_t(time_now);
              printf("CAR VELOCITY: %f m/s\n", v);
              cout << "TIMESTAMP VEL SONAR: " << ctime(&timestamp) << endl;
              if(upcoming_car == 1 && v >= 10 && velocity_flag == 0){
                mtx.lock();
                velocity_flag = 1;
                mtx.unlock();
              }
          }
          old_distance = t/58;
        }
  };

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
    Sensor* sonarSensorTwo = new Sensor(&pinInSonarTwo, &pinOutSonarTwo);
    Sensor* hallEffectSensor = new Sensor(&pinInHall,&pinOutHall);
    sonarDistanceSampleCallback sonarCallbackOne;
    sonarVelocitySampleCallback sonarCallbackTwo;
    hallSampleCallback hallCallback;
    sonarSensorOne->setCallBack(&sonarCallbackOne);
    sonarSensorTwo->setCallBack(&sonarCallbackTwo);
    hallEffectSensor->setCallBack(&hallCallback);
    sonarSensorOne->start(&pinInSonarOne, &pinOutSonarOne, SONAR);
    sonarSensorTwo->start(&pinInSonarTwo, &pinOutSonarTwo, SONAR);
    hallEffectSensor->start(&pinInHall, &pinOutHall, HALL);
    getchar();
    sonarSensorOne->stop();
    sonarSensorTwo->stop();
    hallEffectSensor->stop();
    delete sonarSensorOne;
    delete sonarSensorTwo;
    delete hallEffectSensor;
    return 0;
}
