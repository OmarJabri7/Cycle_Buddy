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

        virtual void dataIn(double data) = 0;
};

class Sensor{

    public:

        Sensor(int *pinIn, int *pinOut);

        void setCallBack(SensorCallback* cb);

        void start(int *pinIn, int *pinOut, const int sensorType);

        void stop();

    private:

        std::thread* sensorThread = NULL;

        int running = 0;

        SensorCallback* sensorCb = NULL;

        static void run_hall(Sensor* sensor, int *pinIn, int *pinOut);
        
        static void run_sonar(Sensor* sensor, int *pinIn, int *pinOut);
};

#endif