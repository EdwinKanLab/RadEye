cmake_minimum_required(VERSION 2.8)
project( BlobDetector )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "../include"
                     "../../Cropper/include"
                     "../../CornerDetector/include"
                     "../../Warper/include"
                     "../../BlobDetector/include")
file(GLOB SOURCES "../src/*.cpp"
                  "../../Cropper/src/*.cpp"
                  "../../CornerDetector/src/*.cpp"
                  "../../Warper/src/*.cpp"
                  "../../BlobDetector/src/*.cpp")

add_link_options("-lstdc++" "-lm")
add_executable(GroundTruthEvaluatorTest GroundTruthEvaluatorTest.cpp ${SOURCES} )
target_link_libraries( GroundTruthEvaluatorTest ${OpenCV_LIBS} )
