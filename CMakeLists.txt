cmake_minimum_required(VERSION 3.13)

project(Cycle_Buddy)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(.)
include_directories(src)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
find_package(nlohmann_json REQUIRED
    CONFIG
    PATHS "/usr/lib/cmake/"
)

find_library(wiringPi_LIB wiringPi)
find_package(raspicam REQUIRED)

add_executable(Cycle_Buddy 
            src/sensors_test.cpp
            src/sensors.cpp)

target_link_libraries(Cycle_Buddy ${wiringPi_LIB})
target_link_libraries(Cycle_Buddy ${raspicam_LIBS})
target_link_libraries(Cycle_Buddy nlohmann_json::nlohmann_json)
target_link_libraries(Cycle_Buddy ${PYTHON_LIBRARIES})
