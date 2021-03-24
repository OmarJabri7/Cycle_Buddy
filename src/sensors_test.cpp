#include "sensors_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>
using namespace std;
#define INIT 2147483647

volatile float old_dist = 0;  //global variable used for speed calculation

class hallSampleCallback : public SensorCallback{
    virtual void dataIn(double v){
        if(v != INIT){
            printf("Car_Velocity: %f m/s\n", v);            
        }
    }
};

class sonarSampleCallback : public SensorCallback{
    virtual void dataIn(double t){
        if(t/58 <= 10){
          printf("1stDistance: %f cm\n", t/58);
        }
    }
};
class sonarSampleCallback1 : public SensorCallback{
    virtual void dataIn(double t){
        if(t/58 <= 10){
          printf("2ndDistance: %f cm\n", t/58);
        }
    }
};

class sonarSampleCallbacks : public SensorCallback{
    virtual void dataIn(double t){                     
          float v = (old_dist-t/58)/10;  // speed of incoming item
          if(v>=1){                        // eliminate first unwanted one
            printf("3rdvelocity: %f m/s\n", v);
          }
          old_dist = t/58;
    }
};

int main(int argc, char *argv[]){
    const int HALL = 0;
    const int SONAR = 1;
    int pinInHall = 1;
    int pinOutHall = 21;
    
    int pinInSonar = 5;
    int pinOutSonar = 4;
    
    int pinInSonar_1 = 28;
    int pinOutSonar_1 = 29;
    
    int pinInSonar_s = 10;
    int pinOutSonar_s = 11;
    
    Sensor* sonarSensor = new Sensor(&pinInSonar, &pinOutSonar);
    Sensor* sonarSensor_1 = new Sensor(&pinInSonar_1, &pinOutSonar_1);
    Sensor* sonarSensor_s = new Sensor(&pinInSonar_s, &pinOutSonar_s);
    Sensor* hallEffectSensor = new Sensor(&pinInHall,&pinOutHall);
    
    sonarSampleCallback sonarCallback;
    sonarSampleCallback1 sonarCallback1;
    sonarSampleCallbacks sonarCallbacks;
    hallSampleCallback hallCallback;
    
    sonarSensor->setCallBack(&sonarCallback);
    sonarSensor_1->setCallBack(&sonarCallback1);
    sonarSensor_s->setCallBack(&sonarCallbacks);
    hallEffectSensor->setCallBack(&hallCallback);
    
    sonarSensor->start(&pinInSonar, &pinOutSonar, SONAR);
    sonarSensor_1->start(&pinInSonar_1, &pinOutSonar_1, SONAR);
    sonarSensor_s->start(&pinInSonar_s, &pinOutSonar_s, SONAR);
    hallEffectSensor->start(&pinInHall, &pinOutHall, HALL);
    getchar();
    sonarSensor->stop();
    sonarSensor_1->stop();
    sonarSensor_s->stop();
    hallEffectSensor->stop();
    delete sonarSensor;
    delete hallEffectSensor;
    delete sonarSensor_1;
    delete sonarSensor_s;
    return 0;
}