#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
using namespace std;
#define _USE_MATHS_DEFINES
#include <math.h>

#define TRUE (1==1)
#define HALL 1
#define LED 4


void setup(){
    wiringPiSetup();
    pinMode(HALL, INPUT);
    pinMode(LED,OUTPUT);
}

int main(){
    setup();
    double timeDetected = 0;
    int wheelRadius = 10;
    double pi = M_PI;
    double interval = 0;
    int triggeredCounter = 0;
    while(TRUE){
    if(digitalRead(HALL) == 1 && ((micros()/1000000.0 - timeDetected >= 2.5) || timeDetected == 0)){
        triggeredCounter++;
        interval = micros()/1000000.0 - timeDetected;
        printf("Velocity: %f m/s\n", abs((2*pi*wheelRadius)/interval));
        timeDetected = micros()/1000000.0;
        cout << triggeredCounter << endl;
        digitalWrite(LED,1);
    }
    else{
        digitalWrite(LED,0);
    }
    // if(interval >= 5){
    // }
    }
}