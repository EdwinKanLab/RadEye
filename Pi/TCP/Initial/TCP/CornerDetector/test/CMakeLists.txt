cmake_minimum_required(VERSION 2.8)
project( CornerDetector )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "../include")
file(GLOB SOURCES "../src/*.cpp")
add_executable(CornerDetectorTest CornerDetectorTest.cpp ${SOURCES} )
target_link_libraries( CornerDetectorTest ${OpenCV_LIBS} )