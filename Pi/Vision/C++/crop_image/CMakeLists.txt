cmake_minimum_required(VERSION 2.8)
project( CropImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( crop_image crop_image.cpp )
target_link_libraries( crop_image ${OpenCV_LIBS} )