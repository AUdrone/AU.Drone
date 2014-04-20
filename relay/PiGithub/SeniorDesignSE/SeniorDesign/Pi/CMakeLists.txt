cmake_minimum_required(VERSION 2.8.3)
#This is the name of the project. The enviornment variables use this name
project(AU_AR_DRONE)

#Compiler will look here for all header includes
include_directories(${AU_AR_DRONE_SOURCE_DIR}/include/)

#The executable will be put in build/bin
SET(EXECUTABLE_OUTPUT_PATH ${AU_AR_DRONE_BINARY_DIR}/bin)

SET(CMAKE_CXX_FLAGS "-g -lpthread")

#add a library
#the format is: add_library(libName libfile.cpp libheader.h anotherlib.h whatever.h)
add_library(XBee src/XBee.cpp include/XBee.h src/Pi.cpp include/Pi.h include/ConstantsAndHeaders.h src/Navigation.cpp include/Navigation.h)
add_library(NavData src/NavData.cpp include/NavData.h include/ConstantsAndHeaders.h)
add_library(UDP src/UDP.cpp include/UDP.h include/ConstantsAndHeaders.h)
#add_library(GCS src/gcscontroller.cpp include/ConstantsAndHeaders.h)
#add_library(XbeePacket src/xbeepacket.cpp include/xbeepacket.h include/ConstantsAndHeaders.h)
#add an excutable file
#the format is: add_executable(name.o name.cpp name.h yolo.h whatever.h)
add_executable(main src/main.cpp) 
#src/XBee.cpp include/XBee.h src/Pi.cpp include/Pi.h include/ConstantsAndHeaders.h)

target_link_libraries(XBee NavData UDP)
target_link_libraries(main XBee)
#to link libraries
#target_link_libraries(name.o libName)
