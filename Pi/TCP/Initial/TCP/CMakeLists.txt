cmake_minimum_required(VERSION 2.8)
project( TCPI )
find_package(raspicam REQUIRED)
find_package(OpenCV)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
include_directories( ${OpenCV_INCLUDE_DIRS} 
                     "GroundTruthEvaluator/include"
                     "Cropper/include"
                     "CornerDetector/include"
                     "Warper/include"
                     "BlobDetector/include"
                     "Tic"
                     )
file(GLOB SOURCES "GroundTruthEvaluator/src/*.cpp"
                  "Cropper/src/*.cpp"
                  "CornerDetector/src/*.cpp"
                  "Warper/src/*.cpp"
                  "BlobDetector/src/*.cpp"
                  "PC_Commands.cpp"
                  "Tic/TicDriver.c")

set_source_files_properties(Tic/TicDriver.c PROPERTIES LANGUAGE CXX )


add_executable(tcp_pi client_ii.cpp ${SOURCES}  )
target_link_libraries( tcp_pi ${OpenCV_LIBS} ${raspicam_CV_LIBS})
ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()
