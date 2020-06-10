/*
 *PC_commands.hpp
 *
 * Created: 03-Jun-20
 * Last Modified: 03-Jun-2020
 * Author: tejas_advait
 *
 * Description: Commands received from PC to be executed on Pi. 
 */ 

#pragma once
#include <string>
// #include "BlobDetector/include/BlobDetector.hpp"

// #include "Cropper/include/Cropper.hpp"

// #include "CornerDetector/include/CornerDetector.hpp"

// #include "Warper/include/Warper.hpp"

// #include "GroundTruthEvaluator/include/GroundTruthEvaluator.hpp"

// #include <opencv2/core/mat.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <raspicam/raspicam_cv.h>




using namespace std;
// using namespace cv;

void setup();

void run_exit();

string getCoordinate();

string gotoCenter();

string step(string command);