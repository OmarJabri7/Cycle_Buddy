<h2 align="center">Cycle Buddy</h2>  
<p align="center">
  <a href="https://github.com/OmarJabri7/Cycle_Buddy">
    <img src="Images/Logo.png" alt="Logo" >
  </a>
    <br />
    <br />
    <a href="https://omarjabri7.github.io/Cycle_Buddy/"><strong>Explore the doxygen»</strong></a>  
  <p align="center">
    To keep people can ride bike more safely on the road.
    <br />
    <br />
    <a href="https://www.youtube.com/channel/UC4mHw6LXU8YYIvdZxgI5Btw"><img height=40 src="Images/youtube_social_circle_red.png"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <a href=""><img height=40 src="Images/f_logo_RGB-Blue_1024.png"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <a href=""><img height=40 src="Images/2021%20Twitter%20logo%20-%20blue.png"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <br />


<div align="center">
[![Contributors](https://img.shields.io/github/contributors/OmarJabri7/Cycle_Buddy.svg?style=for-the-badge)](https://github.com/OmarJabri7/Cycle_Buddy/graphs/contributors)
[![Forks](https://img.shields.io/github/forks/OmarJabri7/Cycle_Buddy.svg?style=for-the-badge)](https://github.com/OmarJabri7/Cycle_Buddy/network/members)
[![Stars](https://img.shields.io/github/stars/OmarJabri7/Cycle_Buddy.svg?style=for-the-badge)](https://github.com/OmarJabri7/Cycle_Buddy/stargazers)
[![Issues](https://img.shields.io/github/issues/OmarJabri7/Cycle_Buddy.svg?style=for-the-badge)](https://github.com/OmarJabri7/Cycle_Buddy/issues)
[![License](https://img.shields.io/github/license/OmarJabri7/Cycle_Buddy.svg?style=for-the-badge)](https://github.com/OmarJabri7/Cycle_Buddy/blob/main/LICENSE)
</div>

<!-- TOC -->
<details open="open">
  <summary><h2 style="display: inline-block">Contents</h2></summary>
  <ol>
    <li><a href="#about">About</a>
    <li><a href="#software">Software</a></li>
    <li><a href="#hardware">Hardware</a></li>       
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#Packages">Packages</a>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>
<!-- Project descirption -->

## About
We are a group of the Real Time Embedded Programing Project in the University of Glasgow.  Two members major in Robotics & Ai, one member majors in Electronic and Electrical Engineering. 

Our project, Cycle Buddy, is a system that detects danger levels in bikers relative to drivers. Once a driver overpasses a biker, the system detects this unauthorised act, and responds by recognising the face/car plate of the driver in order to contact law enforcements.

<br />
[doxygen about the project](https://omarjabri7.github.io/Cycle_Buddy/)






### Software

#### Necessary:
* [**Android Studio**](https://developer.android.com/studio)	(**Mobile Phone Application**) 

  ​	Android Studio provides the fastest tools for building apps on every type of Android device. We use it for connecting the raspberry pi and get the real-time information from the raspberry pi, such as car distance, car velocity and bike velocity.

* [**Visual Studio 2019**](https://visualstudio.microsoft.com/zh-hans/vs/)    (**Raspberry pi threads**) 

  ​	Visual Studio 2019 has the latest features to bring real-time collaboration so it is convenient for us to wirte C++ code for ther project.

* [**Firebase**](https://firebase.google.com/)    (**Real time database**) 

  ​	Firebase is Google’s mobile application development platform that helps you build, improve, and grow your app. It manages our data and stores our license plate numbers which are from Pi camera.

* [**Cmake**](https://cmake.org/)    (**Build up the full project**) 

  ​	CMake is used to control the software compilation process using simple platform and compiler independent configuration files, and generate native makefiles and workspaces that can be used in the compiler environment of our choice.

* [**Doxygen**](https://www.doxygen.nl/index.html)    (**document generation tool**) 

  ​	Doxygen is a document generation tool that extracts documents from code, organizes them, and outputs them into beautiful documents (HTML, PDF, RTF, etc.).

  ​	With the Doxygen tool, programmers can write code directly in the document, no longer need to write a separate document for a function of the code, thus keeping the document and code as consistent as possible.

*  [**OpenALPR - Automatic License Plate Recognition**](https://www.openalpr.com/) (**API**)

  ​	This API can recoginize the license plate number directly from the picture, we find a gb (Great Britain, UK) lib trained model and use it. 

  

#### Optional: 
* Putty

  ​	Find the ip address of raspberry pi simply. 

* VNC (Other Remote Desktop Services are also fine) 

  ​	Use ssh services to log in the raspberry pi so that we do not need to use HDMI or screen to connect to the raspberry pi. It is also convenient to use UI to code in raspberry pi.

*  Typora 

  ​	Edit markdown and Html files for doxygen and web page.

* [**Search for the car information**](https://vehicleenquiry.service.gov.uk/?_ga=2.97692959.1629293611.1618412250-1021591735.1618412250)

  ​	Here is the government website to search for the car information, so when we get the license plate number, we can search for it here.





### Hardware
#### Main device:
* [Raspberry Pi 3 Model B +](https://thepihut.com/products/raspberry-pi-3-model-b-plus?src=raspberrypi) (￡33.90, **1** is needed, not included)

  ​	The Raspberry Pi 3 Model B is the earliest model of the third-generation Raspberry Pi. In our project, It can receive the real time signals of the GPIO from all the sensors and deal with the real - time data.

#### Sensors:
* [Hall sensor](https://www.amazon.co.uk/AZDelivery-Linear-Magnetic-Sensor-Arduino/dp/B07DPVTKBB/ref=sr_1_4_sspa?dchild=1&keywords=Hall+Effect+Sensor&qid=1618406619&sr=8-4-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyV1lEMzJNSUE5S1A4JmVuY3J5cHRlZElkPUEwMjc5ODE1MTI4TUc5SFI1R0gxUSZlbmNyeXB0ZWRBZElkPUEwMDIzNjQwMTkzSk03UDI4WFdROSZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=) (￡4.49, **1** is needed)

  ​	We use hall sensor to detect the velocity of the bike. The theory is to put the magnets On the axle of the bicycle wheel. Make sure the hall sensor is close to magnets enough so that it can detect the magnetic field when the bike runs.

* [Ultrasonic sensor](https://www.amazon.co.uk/HC-SR04-Ultrasonic-Distance-Measuring-Arduino/dp/B01DM8MRTS/ref=sr_1_7?dchild=1&keywords=Ultrasonic+Sensor&qid=1618407336&sr=8-7)（￡3.99,  **1** is needed)

  ​	The Ultrasonic sensor is to detect the distance between the bicycle and car. Besides, according to the velocity of the bicycle, it can calulate the velocity of the car. 

* [Pi camera](https://www.amazon.co.uk/dp/B07XBKC6H7/ref=cm_sw_r_u_apa_glt_fabc_74TKMY2AH1HFZ4SVKC7J) （￡10.99, **1** is needed) 

  ​	The Pi camera is to capture the picture of  car, especially capture the license plate number of the car. We find a feasible location for the Pi camera so that the 

#### Other auxiliary tools :

* Male/Male, Female/Female and Male/Female wires ，magnets (appoximately ￡ 2.34)
* GPIO Breakout Kit Expansion Board For Raspberry Pi 3 B+ (￡14.49)
* Power supply for Raspberry Pi or Power bank. (￡ 13.56)



**Overall**, it costs us ￡49.86 except the **Raspberry Pi 3 Model B +** for the project.



## Roadmap
See the [open issues](https://github.com/OmarJabri7/Cycle_Buddy/issues) for a list of proposed features (and known issues).



### Threads of Sensors 
3 threads: One is for ultrasonic sensors, one for hall sensor, the  rest one is for Pi camera.





## Packages
* WiringPi 

* OpenALPR
* JSON
* Cmake
* Other packages for Android application





## Packages Installations
1. WiringPi:<br>
   `sudo apt-get install wiringpi`<br/>

2. [**OpenALPR**](https://github.com/openalpr/openalpr):  (Github page)<br>
   `sudo apt-get update && sudo apt-get install -y openalpr openalpr-daemon openalpr-utils libopenalpr-dev`<br>

3. [**JSON for API**](https://github.com/nlohmann/json):  (Github page)<br>

   `git clone https://github.com/nlohmann/json`<br>

4. [**Cmake**](https://cmake.org/) : (Website)<br>

   Click and download it from the website above.<br>

5. Other Android application realted packages: <br>





## Usage
This project is built using Cmake and Makefiles in order to allow users to easily run our code and tets it.<br/>
Please follow these necessary steps: <br/>
1. Clone the project with: <br/>
  `git clone https://github.com/OmarJabri7/Cycle_Buddy.git`<br/>

2. Change directorites and relocate in Cycle_Buddy project: <br/>
  `cd Cycle_Buddy`<br/>

3. Link the necessary executables and libraries using CMake: <br/>
  `cmake .`<br/>

4. Build the project using make: <br/>
  `make`<br/>

5. Run the test file labeled Cycle_Buddy: <br/>
  `./Cycle_Buddy`<br/>

  

<!-- Contributing -->

## Contributing
This is an open source project, therefore, just fork it and build up the project according to the README.md in every document.



<!-- License -->

## License
Distributed under the GPL-3.0 License. See `LICENSE` for more information.



<!-- Contact Info -->

## Contact

Omar Jabri - [@Omar Jabri](https://github.com/OmarJabri7)
<br />

Chang Liu - [@Chang Liu](https://github.com/Cliu1993)
<br />

Yifan Xie - [@Yifan Xie](https://github.com/Yifan-Xie)
<br />

Project address [@Project address](https://github.com/OmarJabri7/Cycle_Buddy)

