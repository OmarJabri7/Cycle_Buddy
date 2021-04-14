<h2 align="center">Cycle Buddy</h2>  
<p align="center">
  <a href="https://github.com/OmarJabri7/Cycle_Buddy">
    <img src="https://github.com/Yifan-Xie/Image/blob/Cycle_Buddy/pictures/Logo.png" alt="Logo" >
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
We are a group of the Real Time Embedded Programing Project in the University of Glasgow. Our project, Cycle Buddy, is a system that detects danger levels in bikers relative to drivers. Once a driver overpasses a biker, the system detects this unauthorised act, and responds by recognising the face/car plate of the driver in order to contact law enforcements.

<br />
[doxygen about the project](https://omarjabri7.github.io/Cycle_Buddy/)
<br />
<div align="center"> 
<br />  
</div>

### Software

#### Necessary:
*[Android Studio] (**Mobile Phone Application**) :
Android Studio provides the fastest tools for building apps on every type of Android device. We use it for connecting the raspberry pi and get the real-time information from the raspberry pi, such as car distance, car velocity and bike velocity.
 
*[Visual Studio 2019] (**Raspberry pi threads**) :
Visual Studio 2019 has the latest features to bring real-time collaboration so it is convenient for us to wirte C++ code for ther project.

*[firebase] (**Real time database**) :
Firebase is Google’s mobile application development platform that helps you build, improve, and grow your app. It manages our data and stores our license plate numbers which are from Pi camera.

*[Cmake] (**Build up the full project**) :
CMake is used to control the software compilation process using simple platform and compiler independent configuration files, and generate native makefiles and workspaces that can be used in the compiler environment of our choice.

*[Doxygen] (**document generation tool**) : 
Doxygen is a document generation tool that extracts documents from code, organizes them, and outputs them into beautiful documents (HTML, PDF, RTF, etc.).
With the Doxygen tool, programmers can write code directly in the document, no longer need to write a separate document for a function of the code, thus keeping the document and code as consistent as possible.

#### Optional: 
*[Putty]：
Find the ip address of raspberry pi simply. 

*[VNC]: (Other Remote Desktop Services are also fine) 
Use ssh services to log in the raspberry pi so that we do not need to use HDMI or screen to connect to the raspberry pi. It is also convenient to use UI to code in raspberry pi.
 
<br />

### Hardware
#### Main device:
*[Raspberry Pi 3 Model B +] 
The Raspberry Pi 3 Model B is the earliest model of the third-generation Raspberry Pi.

#### Sensors:
*[Hall sensor]

*[Ultrasonic sensor]

*[Pi camera]

<br />

## Roadmap
### Process of license plate recognition
#### 1. Image capture
#### 2. Image prepreocessing
#### 3. License plate location
#### 4. License plate location character segmentation
#### 5. Character recognition 
#### 6. Output the results

### Threads of Sensors 
4 threads: Two for ultrasonic sensors, one for hall sensor, the rest for Pi camera.

<!-- Getting Started -->
## Packages
Cmake, WiringPi, Openalpr

<!-- Installations -->
## Packages Installations
1- WiringPi:<br>
`sudo apt-get install wiringpi`<br/>
2- ALPR: <br>
`sudo apt-get update && sudo apt-get install -y openalpr openalpr-daemon openalpr-utils libopenalpr-dev`<br>

<!-- Usage -->
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

