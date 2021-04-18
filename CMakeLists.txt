cmake_minimum_required(VERSION 3.13)

project(Cycle_Buddy)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(.)
include_directories(src)
include_directories(test)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
find_package(nlohmann_json REQUIRED
    CONFIG
    PATHS "/usr/lib/cmake/"
)

find_library(wiringPi_LIB wiringPi)
find_package(raspicam REQUIRED)

add_executable(Cycle_Buddy 
            src/main.cpp
            src/sensors.cpp)

add_executable(Unit_test_hall 
            test/unit_hall.cpp
            src/sensors.cpp)

add_executable(Unit_test_ultra 
            test/unit_ultra.cpp
            src/sensors.cpp)

add_executable(Unit_test_cam 
            test/unit_cam.cpp)

target_link_libraries(Cycle_Buddy ${wiringPi_LIB})
target_link_libraries(Cycle_Buddy ${raspicam_LIBS})
target_link_libraries(Cycle_Buddy nlohmann_json::nlohmann_json)
target_link_libraries(Cycle_Buddy ${PYTHON_LIBRARIES})

target_link_libraries(Unit_test_hall ${wiringPi_LIB})
target_link_libraries(Unit_test_ultra ${wiringPi_LIB})
target_link_libraries(Unit_test_cam ${raspicam_LIBS})

enable_testing()

add_test (NAME Test_for_ultra COMMAND Unit_test_ultra)
add_test (NAME Test_for_hall COMMAND Unit_test_hall)
add_test (NAME Test_for_camera COMMAND Unit_test_cam)
