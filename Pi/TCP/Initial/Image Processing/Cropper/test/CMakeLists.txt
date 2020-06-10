cmake_minimum_required(VERSION 2.8)
project( CropImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "../include")
file(GLOB SOURCES "../src/*.cpp")
add_executable(CropperTest CropperTest.cpp ${SOURCES} )
target_link_libraries( CropperTest ${OpenCV_LIBS} )