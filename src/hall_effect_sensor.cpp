#include "sensors_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>
// using namespace sensor;
using namespace std;

#define TRUE (1==1)

static volatile int timeDetected = 0;

void handle(void){
    timeDetected++;
    // interval = micros()/1000000.0 - timeDetected;
    // printf("Velocity: %f m/s\n", abs((2*pi*wheelRadius)/interval));
    // timeDetected = micros()/1000000.0;
    // cout << triggeredCounter << endl;
    // digitalWrite(LED,1);
}

// int do_hall(){
//     setup_1();
//     double timeDetected = 0;
//     int wheelRadius = 10;
//     double pi = M_PI;
//     double interval = 0;
//     int triggeredCounter = 0;
//     while(TRUE){
//     if(digitalRead(HALL) == 1 && ((micros()/1000000.0 - timeDetected >= 2.5) || timeDetected == 0)){
//         triggeredCounter++;
//         interval = micros()/1000000.0 - timeDetected;
//         printf("Velocity: %f m/s\n", abs((2*pi*wheelRadius)/interval));
//         timeDetected = micros()/1000000.0;
//         cout << triggeredCounter << endl;
//         digitalWrite(LED,1);
//     }
//     else{
//         digitalWrite(LED,0);
//     }
//     }
//     return 0;
// }

Sensor::Sensor(int pinIn, int pinOut){
    try{
        wiringPiSetup();
        pinMode(pinIn,INPUT);
        pinMode(pinOut,OUTPUT);
        // wiringPiISR(pinIn, INT_EDGE_RISING, &handle); //Calls handle once pin edge is rising HIGH
    }
    catch(const char* error){
        cout << error << endl;
    }
}

void Sensor::setCallBack(SensorCallback* cb){
    sensorCb = cb;
}

void Sensor::run(Sensor* hallEffect, int pinIn, int pinOut){
    hallEffect->running = 1;
    int wheelRadius = 10;
    double pi = M_PI;
    double interval = 0;
    int triggeredCounter = 0;
    while(hallEffect->running){
        if(digitalRead(pinIn) == 1 && ((micros()/1000000.0 - timeDetected >= 2.5) || timeDetected == 0)){
            triggeredCounter++;
            interval = micros()/1000000.0 - timeDetected;
            printf("Velocity: %f m/s\n", abs((2*pi*wheelRadius)/interval));
            timeDetected = micros()/1000000.0;
            cout << triggeredCounter << endl;
            digitalWrite(pinOut,1);
        }
        else{
            digitalWrite(pinOut,0);
        }
        if(hallEffect->sensorCb){
            // timeDetected = micros();
            double timeDetected = 0;
            hallEffect->sensorCb->dataIn(abs((2*pi*wheelRadius)/interval));
        }
    }
    exit(0);
}

void Sensor::start(int pinIn, int pinOut){
    sensorThread = new std::thread(run,this,pinIn,pinOut);
}

void Sensor::stop(){
    running = 0;
    if(sensorThread){
        sensorThread->join();
        delete sensorThread;
        sensorThread = NULL;
    }
}