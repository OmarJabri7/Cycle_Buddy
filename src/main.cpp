#include "stdafx.h"
#include "sensors_lib.h"
#include "global_var.h"

#include "plate_id.cpp"
#include "callback_hall.cpp"
#include "callback_sonar.cpp"
#include "app_connect.cpp"
#include "camera.cpp"

using namespace std;
using json = nlohmann::json;


char hostIp[MAXLINE];

/** Set old distance to calculate the derivative and get velocity */
volatile double old_distance = 0;


volatile int upcoming_car = 1;
volatile int bike_flag = 0;
volatile conditions conds;

int main(int argc, char *argv[]){
	/** system initialzation: making connection with mobile app */ 
	try_connect();
	
	/**get gpio pin info from gpio_def */
	int p_s_in = pinInSonarOne;
	int p_s_out = pinOutSonarOne;
	int p_h_in = pinInHall;
	int p_h_out = pinOutHall;
	
	/** construct the sensor objects and assign callbacks to proper sensor. */
    Sensor* sonarSensorOne = new Sensor(&p_s_in, &p_s_out);
    Sensor* hallEffectSensor = new Sensor(&p_h_in,&p_h_out);
    sonarCallback sonarCallbackOne;
    hallSampleCallback hallCallback;
    sonarSensorOne->setCallBack(&sonarCallbackOne);
    hallEffectSensor->setCallBack(&hallCallback);
   
	/** start running all three threads */
    sonarSensorOne->start(&p_s_in, &p_s_out, SONAR);
    hallEffectSensor->start(&p_h_in, &p_h_out, HALL);
	camera_process();
	
	/** close the system, delete all threads */
    getchar();
    sonarSensorOne->stop();
    hallEffectSensor->stop();
    printf("System shutdown.");
    delete sonarSensorOne;
    delete hallEffectSensor;
    return 0;
}
