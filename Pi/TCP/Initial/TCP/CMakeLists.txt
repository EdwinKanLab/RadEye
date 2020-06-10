cmake_minimum_required(VERSION 2.8)
project( BlobDetector )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "GroundTruthEvaluator/include"
                     "Cropper/include"
                     "CornerDetector/include"
                     "Warper/include"
                     "BlobDetector/include"
                     ".")
file(GLOB SOURCES "GroundTruthEvaluator/src/*.cpp"
                  "Cropper/src/*.cpp"
                  "CornerDetector/src/*.cpp"
                  "Warper/src/*.cpp"
                  "BlobDetector/src/*.cpp"
                  "PC_Commands.cpp"
                  "TicDriver.c")


add_executable(tcp_pi client_ii.cpp ${SOURCES}  )
target_link_libraries( GroundTruthEvaluatorTest ${OpenCV_LIBS} )
