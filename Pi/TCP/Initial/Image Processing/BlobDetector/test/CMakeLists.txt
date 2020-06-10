cmake_minimum_required(VERSION 2.8)
project( BlobDetector )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "../include"
                     "../../Cropper/include"
                     "../../CornerDetector/include"
                     "../../Warper/include")
file(GLOB SOURCES "../src/*.cpp"
                  "../../Cropper/src/*.cpp"
                  "../../CornerDetector/src/*.cpp"
                  "../../Warper/src/*.cpp")


add_executable(BlobDetectorTest BlobDetectorTest.cpp ${SOURCES} )
target_link_libraries( BlobDetectorTest ${OpenCV_LIBS} )