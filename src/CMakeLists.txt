#####################################
cmake_minimum_required (VERSION 2.8) 
project (mix_test)
find_package(raspicam REQUIRED)
include_directories(
    include sensors_lib.h
)
set(SRC 
    ${PROJECT_SOURCE_DIR}/sensors.cpp
)
set(CMAKE_CXX_FLAGS   "-lwiringPi -lpthread")  
set(CMAKE_C_FLAGS   "-lwiringPi -lpthread")  
add_executable (mix_test mix.cpp ${SRC})  
target_link_libraries (mix_test ${raspicam_LIBS})
#####################################
