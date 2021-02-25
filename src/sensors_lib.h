#ifndef __SENSORSLIB.h
#define __SENSORSLIB.h
// namespace sensor;
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>

class SensorCallback{

    public:

        virtual void dataIn(int data) = 0;
};

class Sensor{

    private:

        int pinIn;
        int pinOut;

    public:


    Sensor(int pinIn, int pinOut){
        this->pinIn = pinIn;
        this->pinOut = pinOut;
    }

    void setPins(int pinIn, int pinOut){
        this->pinIn = pinIn;
        this->pinOut = pinOut;
    }

    int getPins(){
        return pinIn, pinOut;
    }

    void setCallBack(SensorCallback* cb);

    void start(int wheelRadius);

    void stop();
};

#endif