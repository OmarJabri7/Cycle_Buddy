cmake_minimum_required(VERSION 3.13)

project(Cycle_Buddy)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories(.)
include_directories(src)

find_library(wiringPi_LIB wiringPi)

add_executable(Cycle_Buddy 
            src/hall_effect_test.cpp
            src/hall_effect_sensor.cpp
            src/ultrasonic_sensor.cpp)

target_link_libraries(Cycle_Buddy ${wiringPi_LIB})