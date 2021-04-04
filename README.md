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
</div>  
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
Cycle Buddy is System that detects danger levels in bikers relative to drivers. Once a driver overpasses a biker, the system detects this unauthorised act, and responds by recognising the face/car plate of the driver in order to contact law enforcements.
<br />
[doxygen about the project](https://omarjabri7.github.io/Cycle_Buddy/)
<br />
<div align="center"> 
<br />  
</div>

### Software
* [VNC]

* [Putty]

* [Android Studio] 
 
<br />

### Hardware
* [Raspberry Pi 3 Model B +]

* [Sensors: Hall sensors, Ultrasonic sensors, Pi camera]

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
Cmake, OpenCv, WiringPi, Openalpr

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

