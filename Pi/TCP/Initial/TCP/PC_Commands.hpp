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
#include "BlobDetector/include/BlobDetector.hpp"

#include "Cropper/include/Cropper.hpp"

#include "CornerDetector/include/CornerDetector.hpp"

#include "Warper/include/Warper.hpp"

#include "GroundTruthEvaluator/include/GroundTruthEvaluator.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <raspicam/raspicam_cv.h>




using namespace std;
using namespace cv;

///////////////////////////////////////////////////////////////////////////
float screenWidthMM = 267;
float screenHeightMM = 180.5;

///////////////////////////////////////////////////////////////////////////
// Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
// Cropper topLeftRegion(0, 0.10, 0.0, 0.10);
// Cropper topRightRegion(0.05, 0.15, 0.85, 1);
// Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
// Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);

Cropper initialDiscard(0.18, 0.98, 0.03, 0.88);
Cropper topLeftRegion(0.05, 0.15, 0.0, 0.15);
Cropper topRightRegion(0.0, 0.10, 0.9, 1);
Cropper bottomLeftRegion(0.85, 1, 0.01, 0.15);
Cropper bottomRightRegion(0.85, 0.98, 0.85, 1);

///////////////////////////////////////////////////////////////////////////

CornerDetector singleCorner(1,0.1,100);
CornerDetector fourPlaneCorners(4, 0.1, 100);

///////////////////////////////////////////////////////////////////////////

Warper warper;

///////////////////////////////////////////////////////////////////////////

BlobDetector blobDetector;

//////////////////////////////////////////////////////////////////////////
GroundTruthEvaluator groundTruthEvaluator;

//////////////////////////////////////////////////////////////////////////
raspicam::RaspiCam_Cv Camera;

//////////////////////////////////////////////////////////////////////////


void setup();

void run_exit();

string getCoordinate();

string gotoCenter();

string step(string command);