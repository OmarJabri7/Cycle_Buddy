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
#define _USE_MATHS_DEFINES
#include <math.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
using namespace std;
#define INIT 2147483647
#define UUID "163660a6-ad17-44fc-99c5-5c75e78ad815"

/** Sample Call Back class inheriting SensorCallback, 
    associated with the hall effect sensor
 */
class hallSampleCallback : public SensorCallback{
    /** Call back functin associated with hall effect sensor to return the velocity readiings.
    */
    /** @param v - velocity of bicycle wheel reading from hall sensor
    */
    virtual void dataIn(double v){
        if(v != INIT){
            auto time_now = chrono::system_clock::now();
            time_t timestamp = chrono::system_clock::to_time_t(time_now);
            printf("Velocity: %f m/s\n", v);
            cout << "TIMESTAMP HALL: " << ctime(&timestamp) << endl;
        }
    }
};
/** Sample Call Back class inheriting SensorCallback, 
    associated with the sonar sensor
 */
class sonarSampleCallback : public SensorCallback{
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
        string data = to_string(distance);
        const void *buffer = data.c_str();
        //Send Data via bluetooth
        struct sockaddr_rc addr = {0};
        int s, status;
        // char dest[18] = "14:D1:69:2F:E0:67"; // Android MAC addr
        // char dest[18] = "78:31:c1:d7:19:1a"; // Laptop MAC addr
        char dest[18] = "A0:AF:BD:8D:5A:53"; // VM MAC addr
        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_channel = (uint8_t) 1;
        str2ba(dest, &addr.rc_bdaddr);
        status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
        cout << "Status: " << status << endl;
        if(status == 0){
            status = write(s,buffer, sizeof(buffer));
        }
        close(s);
    }
};

int main(int argc, char *argv[]){
    const int HALL = 0;
    const int SONAR = 1;
    int pinInHall = 1;
    int pinOutHall = 4;
    int pinInSonar = 24;
    int pinOutSonar = 23;
    Sensor* sonarSensor = new Sensor(&pinInSonar, &pinOutSonar);
    Sensor* hallEffectSensor = new Sensor(&pinInHall,&pinOutHall);
    sonarSampleCallback sonarCallback;
    hallSampleCallback hallCallback;
    sonarSensor->setCallBack(&sonarCallback);
    hallEffectSensor->setCallBack(&hallCallback);
    sonarSensor->start(&pinInSonar, &pinOutSonar, SONAR);
    hallEffectSensor->start(&pinInHall, &pinOutHall, HALL);
    getchar();
    sonarSensor->stop();
    hallEffectSensor->stop();
    delete sonarSensor;
    delete hallEffectSensor;
    return 0;
}