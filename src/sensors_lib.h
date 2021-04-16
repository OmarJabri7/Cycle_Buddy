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
// #include "firebase/app.h"

/** Class that allows sensors to return their readings using threading callbacks.
    @author Omar Jabri
    @date March 2020
     */
class SensorCallback{
    public:
    /** Public asbtract function accessed by each thread to return their readings.
     */
    /** @param data - data (type double) returned from the different sensors.
     */
  virtual void dataIn(double data, bool isInterrupt = false) = 0;
};
/** Abstract Sensor Class that creates instances of each sensor used in the system.
    @author Omar Jabri
    @date March 2020
     */
class Sensor{
    /** public functions/values accessible from threads.
     */
    public:
        /** Default Sensor Constructor
         */
        /** @param pinIn - Input Pin number of the raspberry pi GPIO extracted from the sensors.
         */
        /** @param pinOut - Output Pin number of the raspberry pi GPIO writing to the sensors.
         */
        Sensor(int *pinIn, int *pinOut);

        /** Function that allows threads to set Callbacks to their assigned functions.
         */
        /** @param cb - call back object to call dataIn function for each thread.
         */
        void setCallBack(SensorCallback* cb);

        /** Start threads
         */
        /** @param sensorType - Integer id of which sensor is executing the particular thread.
         */
        void start(int *pinIn, int *pinOut, const int sensorType);
        /** Stop threads.
         */
        void stop();
/** Private values 
 */
    private:

        std::thread* sensorThread = NULL;

        int running = 0;

        SensorCallback* sensorCb = NULL;

        static void run_hall(Sensor* sensor, int *pinIn, int *pinOut);
        
        static void run_sonar(Sensor* sensor, int *pinIn, int *pinOut);
};

#endif
