#include "sensors_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>
#include <ctime>
#include <raspicam/raspicam.h>
#include <cstdio>
#include <mutex>

using namespace std;

#define INIT 2147483647

volatile float old_dist = 0;  //global variable used for speed calculation
volatile int New_car = 1;
volatile int flag_dist = 0;
volatile int flag_s_bike = 0;
volatile int flag_velo = 0;

std::mutex cmdMtx;

/*class hallSampleCallback : public SensorCallback{
    virtual void dataIn(double v){
        if(v != INIT){
            printf("Bike_Velocity: %f m/s\n", v);  
			if(New_car && v > 8 && !flag_s_bike){		//set condition for bike speed
				cmdMtx.lock();
				flag_s_bike = True;
				cmdMtx.unlock();
			}
        }
    }
};*/

class sonarSampleCallback : public SensorCallback{
    virtual void dataIn(double t){
        if(t/58 >= 10){
          printf("Distance: %f cm\n", t/58);
    		  if(New_car==1 && t/58 < 100 && flag_dist==0){	//set condition for distance
				    cmdMtx.lock();
				    flag_dist = 1;
				    cmdMtx.unlock();
			    }
        }
    }
};

/*class sonarSampleCallback1 : public SensorCallback{
    virtual void dataIn(double t){
        if(t/58 <= 10){
          printf("2ndDistance: %f cm\n", t/58);
        }
    }
};*/

class sonarSampleCallbacks : public SensorCallback{
    virtual void dataIn(double t){   
          float v = (old_dist-t/58)/10;  // speed of incoming item
          if(v>=1){                        // eliminate first unwanted one
            printf("velocity: %f m/s\n", v);
			      if(New_car==1 && v < 10 && flag_velo==0){		//set condition for incoming speed
				      cmdMtx.lock();
				      flag_velo = 1;
				      cmdMtx.unlock();
			      }
          }
          old_dist = t/58;
    }
};

int main(int argc, char *argv[]){
    const int HALL = 0;
    const int SONAR = 1;
    //int pinInHall = 1;
    //int pinOutHall = 21;
    
    int pinInSonar = 5;
    int pinOutSonar = 4;
    
    //int pinInSonar_1 = 28;
    //int pinOutSonar_1 = 29;
    
    int pinInSonar_s = 10;
    int pinOutSonar_s = 11;
    
    Sensor* sonarSensor = new Sensor(&pinInSonar, &pinOutSonar);
    //Sensor* sonarSensor_1 = new Sensor(&pinInSonar_1, &pinOutSonar_1);
    Sensor* sonarSensor_s = new Sensor(&pinInSonar_s, &pinOutSonar_s);
    //Sensor* hallEffectSensor = new Sensor(&pinInHall,&pinOutHall);
    
    sonarSampleCallback sonarCallback;
    //sonarSampleCallback1 sonarCallback1;
    sonarSampleCallbacks sonarCallbacks;
    //hallSampleCallback hallCallback;
    
    sonarSensor->setCallBack(&sonarCallback);
    //sonarSensor_1->setCallBack(&sonarCallback1);
    sonarSensor_s->setCallBack(&sonarCallbacks);
    //hallEffectSensor->setCallBack(&hallCallback);
	
	
	  raspicam::RaspiCam Camera; //Cmaera object
    //Open camera 
    cout<<"Opening Camera..."<<endl;
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
    //wait a while until camera stabilizes
    cout<<"Sleeping for 3 secs"<<endl;
    sleep(3);
	  cout<<"cam set"<<endl;
	
    sonarSensor->start(&pinInSonar, &pinOutSonar, SONAR);
    //sonarSensor_1->start(&pinInSonar_1, &pinOutSonar_1, SONAR);
    sonarSensor_s->start(&pinInSonar_s, &pinOutSonar_s, SONAR);
    //hallEffectSensor->start(&pinInHall, &pinOutHall, HALL);
    
	  int pic_count = 0;
	  char pic_name[50];
	  while(1){
       if(flag_dist && flag_velo){
          cout<<"captured"<<endl;
			    pic_count++;
			    Camera.grab();
			    //allocate memory
			    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
			    //extract the image in rgb format
			    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image
			    //save
			    std::ofstream outFile ( "raspicam_image.ppm",std::ios::binary );
			    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
			    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
			
			    sprintf(pic_name, "%d.ppm", pic_count);
			    std::ofstream outFile_1 ( pic_name,std::ios::binary );
			    outFile_1<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
			    outFile_1.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
			
			    cout<<"Image saved at raspicam_image.ppm"<<endl;
          cmdMtx.lock();
          New_car = 0;
          cmdMtx.unlock();
      
          cout<<"wait for another car"<<endl;
			    sleep(5);
          cout<<"ready now"<<endl;
      
			    cmdMtx.lock();
			    flag_dist = 0;
			    flag_s_bike = 0;
			    flag_velo = 0;
			    New_car = 1;
			    cmdMtx.unlock();
        }
		    if (pic_count>3){
			    break;
		    }
	  }			
		
	  //getchar();	
    sonarSensor->stop();
    //sonarSensor_1->stop();
    sonarSensor_s->stop();
    //hallEffectSensor->stop();
    delete sonarSensor;
    //delete hallEffectSensor;
    //delete sonarSensor_1;
    delete sonarSensor_s;
    return 0;
}