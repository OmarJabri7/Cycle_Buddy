#ifndef __SENSORSLIB_h
#define __SENSORSLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>
#include <thread>

class SensorCallback{

    public:

        virtual void dataIn(int data) = 0;
};

class Sensor{

    public:

        Sensor(int pinIn, int pinOut);

        void setCallBack(SensorCallback* cb);

        void start(int wheelRadius = 10);

        void stop();

    private:

        std::thread* sensorThread = NULL;

        int running = 0;

        SensorCallback* sensorCb = NULL;

        static void run(Sensor* sensor);
};

#endif