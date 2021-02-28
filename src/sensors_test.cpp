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

class hallSampleCallback : public SensorCallback{
    virtual void dataIn(double v){
        if(v != INIT){
            printf("Velocity: %f m/s\n", v);
        }
    }
};

class sonarSampleCallback : public SensorCallback{
    virtual void dataIn(double t){
        printf("Distance: %f cm\n", t/58);
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