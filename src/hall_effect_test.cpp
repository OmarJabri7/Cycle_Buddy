#include "sensors_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>
using namespace std;

class sensorSampleCallback : public SensorCallback{
    virtual void dataIn(int v){
        printf("vel = %d m/s\n", v);
    }
};

int main(int argc, char *argv[]){
    int pinIn = 1;
    int pinOut = 4;
    int wheelRadius = 10;
    Sensor* hallEffectSensor = new Sensor(pinIn,pinOut);
    sensorSampleCallback hallCallback;
    hallEffectSensor->setCallBack(&hallCallback);
    hallEffectSensor->start(pinIn, pinOut);
    getchar();
    hallEffectSensor->stop();
    delete hallEffectSensor;
    return 0;
}