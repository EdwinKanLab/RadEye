cmake_minimum_required(VERSION 2.8)
project( Warper )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "../include"
                     "../../Cropper/include"
                     "../../CornerDetector/include")
file(GLOB SOURCES "../src/*.cpp"
                  "../../Cropper/src/*.cpp"
                  "../../CornerDetector/src/*.cpp")


add_executable(WarperTest WarperTest.cpp ${SOURCES} )
target_link_libraries( WarperTest ${OpenCV_LIBS} )