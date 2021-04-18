## Project structure

The project is composed by two separate sections, an embedded system developed with C++, and an android application developed with JavaScript. This part will focus on introducing the structure of the embedded system using C++.

The embedded system can be separated into 4 part, they are in charge of connection, two types of sensors and the camera. Each part has its own cpp file: 

First the raspi system will establish connection with the mobile application. after connecting with the cellphone, the raspi system will then start three thread at the same time, each in charge of a unique hardware action for the system. The three hardware components are one hall-effect sensor, one ultrasonic radar and one Pi camera. The camera is in standby while the hall-effect sensor and ultrasonic radar refresh data in a frequency of 2Hz. Also, we use volatile variable and mutex locks to allow communication between the three-running thread

 
### Part A. app_connect.cpp.

The system would need to establish connection with the mobile application in order to start measurement. The function try_connect will be executed first in the main program, only when the connection successfully information returns form the phone application will the main program continue to next part.

For the sensors, we wish to make the system expandable to more sensors in the future, therefore we have sensors_lib.h that defines 2 parent class for sensors: Sensors(contains functions as constructor, start and stop) and SensorCallback(contains callback function that response to sensor input). 



### Part B. callback_hall.cpp.

This is the cpp file that define the callback function of the hall sensor, which inherited from parent class SensorCallback, it uses the sensor input from hall effect sensor to calculate the velocity of bike speed and then package the data and forward it to the mobile app. Besides, it changes the flag condition whenever the bike speed exceed threshold.

 

### Part C. callback_sonar.cpp.

This is the cpp file that define the callback function of the ultrasonic radar, which inherited from parent class SensorCallback, it uses the sensor input from ultrasonic radar to get the relative distance between the sensor and incoming car. Also, it uses the difference between two time-adjacent distance to calculate the relative speed of incoming car towards the radar. In the meantime, it packages the velocity and distance data and forward it to the mobile app. Also, it changes the flag conditions for relative speed and relative distance when they exceed threshold.

 

### Part D. camera.cpp.

This cpp controls the camera action and will constantly check if all flags for threshold situation is real to check if there are dangerous car driving towards the bike. When all flags are real while the camera is in stand-by status, it will then trigger the camera to take a photo of incoming car, and then forward it to online API for car-plate recognition. After receiving the identified car-plate number, it then packages the plate number, the car photo and the instant speed and distance of the car and the bike the moment the picture was taken. Then this thread will forward them to the online database(Firebase)for later use, while in the meantime send them to the application. After that, the camera requires some time for cold down and prevent be triggered multiple times by the same car. Then the camera restores stand-by status and wait for another car.

- [GitHub Repository](https://github.com/OmarJabri7/Cycle_Buddy)
- [Youtube](https://www.youtube.com/channel/UC4mHw6LXU8YYIvdZxgI5Btw)
- [Hackaday.io](https://hackaday.io/project/179217-cycle-buddy)
