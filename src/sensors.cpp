#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#include "sensors_lib.h"
#include <iostream>
using namespace std;

Sensor::Sensor(int *pinIn, int *pinOut){
    try{
        wiringPiSetup();
        pinMode(*pinIn,INPUT);
        pinMode(*pinOut,OUTPUT);
    }
    catch(const char* error){
        cout << error << endl;
    }
}

void Sensor::setCallBack(SensorCallback* cb){
    sensorCb = cb;
}

void Sensor::run_sonar(Sensor* sonar, int *pinIn, int *pinOut){
    sonar->running = 1;
    while(sonar->running){
        digitalWrite(*pinOut, LOW);
        delay(500);
        //Send trig pulse
        digitalWrite(*pinOut, HIGH);
        delayMicroseconds(10);
        digitalWrite(*pinOut, LOW);
        //Wait for echo start
        while(digitalRead(*pinIn) == 0);
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(*pinIn) == 1);
        long travelTime = micros() - startTime;
        //Get distance in cm
        if(sonar->sensorCb){
            sonar->sensorCb->dataIn(travelTime);
        }
    }
    exit(0);
}

void Sensor::run_hall(Sensor* hallEffect, int *pinIn, int *pinOut){
    hallEffect->running = 1;
    double timeDetected = 0;
    int wheelRadius = 25; // in cm
    double pi = M_PI;
    double interval = 0;
    double vel = 0.0;
    bool isInterrupt = false;
    while(hallEffect->running){
      delay(500);
      if(digitalRead(*pinIn) == 1){
	    isInterrupt = false;
            interval = micros()/1000000.0 - timeDetected;
            vel = abs((2*pi*wheelRadius)/interval);
            timeDetected = micros()/1000000.0;
            if(hallEffect->sensorCb){
	      hallEffect->sensorCb->dataIn(vel,isInterrupt);
            }
      }
	else if(digitalRead(*pinIn) == 0){
	    if(hallEffect->sensorCb){
		hallEffect->sensorCb->dataIn(vel,false);
	    }
	}
    }
    exit(0);
}

void Sensor::start(int *pinIn, int *pinOut, const int sensorType){
    if(sensorType == 0){
        sensorThread = new std::thread(run_hall,this,pinIn,pinOut);
    }
    else if(sensorType == 1){
        sensorThread = new std::thread(run_sonar,this,pinIn,pinOut);
    }
}

void Sensor::stop(){
    running = 0;
    if(sensorThread){
        sensorThread->join();
        delete sensorThread;
        sensorThread = NULL;
    }
	
}
