#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
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
        //TRIG pin must start LOW
        cout << "Setting pin mode to LOW..." << endl;
        digitalWrite(TRIG, LOW);
        delay(500);
        // delayMicroseconds(2);
        // delay(500);
}
 
int getCM() {
        // delay(10);
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        // sleep(0.00001);
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
 
int main(void) {
    cout << "Started Sensor System..." << endl;
    setup();
    while (TRUE)
    {
        printf("Distance: %dcm\n", getCM());
        delay(1000);
    }
    return 0;
}
