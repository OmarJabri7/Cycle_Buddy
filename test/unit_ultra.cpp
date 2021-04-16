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
#include <cstdio>
#include <mutex>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <regex>
#include <mutex>
using namespace std;

std::mutex mtx;

volatile int test_flag = 0;

class sonarDistanceSampleCallback : public SensorCallback{
    /** Call back functin associated with sonar sensor to return the velocity readiings.
    */
    /** @param t - time of echo heading t obstacle and coming back as reading from sonar sensor
    */
  virtual void dataIn( double t, bool isInterrupt = false){
      	mtx.lock();
        test_flag += 1;
		    cout<<"Data"<<test_flag<<"......received"<<endl;
	      mtx.unlock();  
    }       
};

int main(int argc, char *argv[]){

    const int SONAR = 1;
    int pinInSonar = 24;
    int pinOutSonar = 23;
    Sensor* sonarSensor = new Sensor(&pinInSonar, &pinOutSonar);
    sonarDistanceSampleCallback sonarCallback;
    sonarSensor->setCallBack(&sonarCallback);
    sonarSensor->start(&pinInSonar, &pinOutSonar, SONAR);
    while(1){
        if (test_flag == 10){
            cout<<"Unit test for Ultrasonic Sensor: 10 data required "<<test_flag<<"data received.........complete"<<endl;
            sonarSensor->stop();
            delete sonarSensor;
            break;
        }
    }
    sonarSensor->stop();
    delete sonarSensor;
    
    return 0;
}




