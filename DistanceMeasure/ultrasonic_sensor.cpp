#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

 
#define TRUE 1
 
#define TRIG 23
#define ECHO 24



void setup() {
		cout << "Wiring Pi..." << endl;
        wiringPiSetup();
        cout << "Setting Pin Modes..." << endl;
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
}
 
int getCM() {
		//TRIG pin must start LOW
        cout << "Setting pin mode to LOW..." << endl;
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        //Send trig pulse
        cout << "Sending TRIG pulse..." << endl;
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
		cout << "Sending ECHO pulse" << endl;
        //Wait for echo start
        while(digitalRead(ECHO) == 0);
		cout << "ECHO end" << endl;
        //Wait for echo end
        long startTime = micros();
        cout << "ECHO traveling..." << endl;
        while(digitalRead(ECHO) == 1);
        long travelTime = micros() - startTime;
		cout << "ECHO travel complete." << endl;
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
 
int main(void) {
	cout << "Started Sensor System..." << endl;
    setup();
    while (TRUE)
    {
        int dist=getCM();
        cout << "Distance: %dcm\n" << dist << endl;
    }
    delay(1000);
    return 0;
}
