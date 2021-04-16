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

class hallSampleCallback : public SensorCallback{
    // Call back functin associated with hall effect sensor to return the velocity readiings.
    // @param v - velocity of bicycle wheel reading from hall sensor

  virtual void dataIn(double v, bool isInterrupt = false){
    if(isInterrupt == false){
      mtx.lock();
      test_flag += 1;
      cout<<"Data"<<test_flag<<"......Received"<<endl;
	    mtx.unlock();
      }
    }
};


int main(int argc, char *argv[]){
    const int HALL = 0;
    int pinInHall = 1;
    int pinOutHall = 4;
    Sensor* hallEffectSensor = new Sensor(&pinInHall,&pinOutHall);
    hallSampleCallback hallCallback;
    hallEffectSensor->setCallBack(&hallCallback);
    hallEffectSensor->start(&pinInHall, &pinOutHall, HALL);
    while(1){
        if (test_flag == 10){
            cout<<"Unit test for 2Hz Hall-effect-sensor: 10 case required,"<<test_flag<<"Case received.........complete"<<endl;
            hallEffectSensor->stop();
            break;
        }
    }
    hallEffectSensor->stop();   
    delete hallEffectSensor;    
    return 0;
}




