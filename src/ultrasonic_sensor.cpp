#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
using namespace std;

 
#define TRUE 1
 
#define TRIG 23
#define ECHO 24

static volatile int timeDetected = 0 ;



void setup_2() {
        cout << "Wiring Pi..." << endl;
        wiringPiSetup();
        cout << "Setting Pin Modes..." << endl;
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
        //TRIG pin must start LOW
        cout << "Setting pin mode to LOW..." << endl;
        digitalWrite(TRIG, LOW);
        delay(500);
}
 
int getCM() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        //Wait for echo start
        while(digitalRead(ECHO) == 0);
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == 1);
        long travelTime = micros() - startTime;
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
 
int do_sonar(void) {
    cout << "Started Sensor System..." << endl;
    setup_2();
    while (TRUE)
    {
        printf("Distance: %dcm\n", getCM());
        delay(1000);
    }
    return 0;
}
