#include "sensors_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>

using namespace std;

#define WHEELRADIUS 10

#define HALL 1
#define LED 4

#include "sensors_lib.h"

class sensorSampleCallback : public SensorCallback{
    virtual void dataIn(int v){
        printf("vel = %d\n", v);
    }
}

int main(int argc, char *argv[]){
    int pinIn = 1;
    int pinOut = 4;
    Sensor* hallEffectSensor = new Sensor(pinIn,pinOut);
    sensorSampleCallback hallCallback;
    hallEffectSensor->setCallback(&hallCallback);
    hallEffectSensor->start(WHEELRADIUS);
    getchar();
    hallEffectSensor->stop();
    delete hallEffectSensor;
    return 0;
}