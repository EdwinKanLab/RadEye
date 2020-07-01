/*
 *PC_commands.cpp
 *
 * Created: 03-Jun-20
 * Last Modified: 04-Jun-2020
 * Author: tejas_advait
 *
 * Description: Commands received from PC to be executed on Pi. 
 */

#include "BlobDetector/include/BlobDetector.hpp"

#include "Cropper/include/Cropper.hpp"

#include "CornerDetector/include/CornerDetector.hpp"

#include "Warper/include/Warper.hpp"

#include "GroundTruthEvaluator/include/GroundTruthEvaluator.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include "PC_Commands.hpp"
#include "Tic/TicDriver.h"
#include <string>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstdint>

#include <raspicam/raspicam_cv.h>

using namespace std;
using namespace cv;


///////////////////////////////////////////////////////////////////////////
float screenWidthMM;
float screenHeightMM;

///////////////////////////////////////////////////////////////////////////

Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
Cropper topLeftRegion(0, 0.10, 0.01, 0.10);
Cropper topRightRegion(0.05, 0.15, 0.85, 1);
Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);

// Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
// Cropper topLeftRegion(0, 0.10, 0.0, 0.10);
// Cropper topRightRegion(0.05, 0.15, 0.85, 1);
// Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
// Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);

///////////////////////////////////////////////////////////////////////////

CornerDetector singleCorner(1,0.1,100);

///////////////////////////////////////////////////////////////////////////

Warper warper;

///////////////////////////////////////////////////////////////////////////

BlobDetector blobDetector;

//////////////////////////////////////////////////////////////////////////
GroundTruthEvaluator groundTruthEvaluator;

//////////////////////////////////////////////////////////////////////////
raspicam::RaspiCam_Cv Camera;

//////////////////////////////////////////////////////////////////////////

const char vertical_1_sno [9] = "00295780";
const char horizontal_1_sno [9] = "00286960";

const char vertical_2_sno [9] = "00295780";
const char horizontal_2_sno [9] = "00286960";


uint8_t step_mode = 3;
uint8_t mult = (uint8_t) pow(2, (double)step_mode);
uint32_t max_speed = 2000000 * mult;
uint32_t starting_speed = 0;
uint32_t max_decel = 40000 * mult;
uint32_t max_accel = 40000 * mult; 
uint16_t current_limit = 1000;
int32_t curr_pos = 0;
int32_t full_vertical = 83;

TicDriver* vertical_1 = createTicDriver(vertical_1_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

TicDriver* horizontal_1 = createTicDriver(horizontal_1_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

TicDriver* vertical_2 = createTicDriver(vertical_2_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

TicDriver* horizontal_2 = createTicDriver(horizontal_2_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

Mat image;

//////////////////////////////////////////////////////////////////////////
void applyCroppers(){
    Camera.grab();
    Camera.retrieve(image);
    Camera.release();

    initialDiscard.setInputImage(image);
    initialDiscard.doTheJob();

    topLeftRegion.setInputImage(initialDiscard.getResult());
    topRightRegion.setInputImage(initialDiscard.getResult());
    bottomRightRegion.setInputImage(initialDiscard.getResult());
    bottomLeftRegion.setInputImage(initialDiscard.getResult());
    
    topLeftRegion.doTheJob();
    topRightRegion.doTheJob();
    bottomRightRegion.doTheJob();
    bottomLeftRegion.doTheJob();
}

Point2f getCorner(Cropper& region){

    singleCorner.setInputImage(region.getResult());
    singleCorner.doTheJob();
    Point2f result = singleCorner.getResult()[0];
    Point2f origin;
    origin.x =  (float) (int) (region.getLeftRatio() * (region.getInputImage().cols));
    origin.y = (float) (int) (region.getTopRatio() * (region.getInputImage().rows));
    result += origin;

    return result;

}
///////////////////////////////////////////////////////////////////////////////

void setup(){
    if (Camera.isOpened()){
        Camera.release();
    }
    screenWidthMM = 267;
    screenHeightMM = 180.5;
    blobDetector.setThresholdParams(200, 255, 10);
    blobDetector.setColorParams(true, 255);
    blobDetector.setAreaParams(false, 0, 2);
    blobDetector.setCircularityParams(false, 0, 1);
    blobDetector.setInertiaParams(true, 0.2, 1);
    blobDetector.setConvexityParams(false, 0.0, 1);
    blobDetector.setMinRepeatability(2);
    blobDetector.setMinDistBetweenBlobs(0);

///////////////////////////////////////////////////////////////////////////
    groundTruthEvaluator.setScreenDimMM(screenWidthMM, screenHeightMM);

    groundTruthEvaluator.setCroppers(initialDiscard, topLeftRegion,
                                        topRightRegion, bottomRightRegion,
                                        bottomLeftRegion);


    groundTruthEvaluator.setSingleCorner(singleCorner);
    // groundTruthEvaluator.setWarper(warper);
    groundTruthEvaluator.setBlobDetector(blobDetector);

    ///////////////////////////////////////////////////////////////////////////
    Camera.set( CAP_PROP_FORMAT, CV_32FC3 );
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;}
    //////////////////////////////////////////////////////////////////////////
    sleep(3);
    Camera.grab();
    Camera.retrieve(image);
    Camera.release();

    initialDiscard.setInputImage(image);
    initialDiscard.doTheJob();

    topLeftRegion.setInputImage(initialDiscard.getResult());
    topRightRegion.setInputImage(initialDiscard.getResult());
    bottomRightRegion.setInputImage(initialDiscard.getResult());
    bottomLeftRegion.setInputImage(initialDiscard.getResult());
    
    topLeftRegion.doTheJob();
    topRightRegion.doTheJob();
    bottomRightRegion.doTheJob();
    bottomLeftRegion.doTheJob();

    warper.setTopLeftSrcPoint(getCorner(topLeftRegion));
    warper.setTopRightSrcPoint(getCorner(topRightRegion));    
    warper.setBottomRightSrcPoint(getCorner(bottomRightRegion));
    warper.setBottomLeftSrcPoint(getCorner(bottomLeftRegion));
    groundTruthEvaluator.setWarper(warper);
}



const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

string formatFloat(float input){
    float rounded = roundf(input*10);
    int r = (int) rounded;
    int dec = r % 10;
    r /= 10;
    string result = to_string(r) + "." + to_string(dec);
    result = string(5-result.length(), '0')+result;
    return result;
}

string getCoordinate(int i){
    // vector<vector<float>> coord;
    // coord = IP.getCoordinate();
    Camera.grab();
    Camera.retrieve(image);
    groundTruthEvaluator.setInputImage(image);
    groundTruthEvaluator.doTheJob(false);
    int numTries = 1;
    while (numTries < 10 && groundTruthEvaluator.getResult()[0].x == -1){
        Camera.grab();
        Camera.retrieve (image);
        groundTruthEvaluator.setInputImage(image);
        groundTruthEvaluator.doTheJob(false);
        numTries++;
    }

    groundTruthEvaluator.saveInputImage("../SavedImages/"+
        to_string(i)+"a_inputImage.jpg");
    
    groundTruthEvaluator.saveResultAsImage("../SavedImages/"+
        to_string(i)+"b_groundTruthImage.jpg");

    string result ="23";

    for (int j = 0; j < 2; j++){
        // result += to_string(groundTruthEvaluator.getResult()[i].x).substr(0,5);
        result += formatFloat(groundTruthEvaluator.getResult()[j].x);
        result += " ";
        // result += to_string(groundTruthEvaluator.getResult()[i].y).substr(0,5);
        result += formatFloat(groundTruthEvaluator.getResult()[j].y);
        result += " ";
    }
    return result.substr(0,25);
}

string gotoCenter(){
    return "04done";
}

string step(string command){
    vector<string> v{explode(command, ' ')};
    int steps = stoi(v[1]);

    if (v[2].compare("LR1") == 0){
        if (steps != 0) horizontal_1->steps(horizontal_1, steps);
        else {horizontal_1->deenergize(horizontal_1);}
    }
    else if (v[2].compare("LR2") == 0){
        if (steps != 0) horizontal_2->steps(horizontal_2, steps);
        else {horizontal_2->deenergize(horizontal_2);}
    }
    else if (v[2].compare("UD1") == 0){
        if (steps != 0) vertical_1->steps(vertical_1, steps);
        else {vertical_1->deenergize(vertical_1);}
    }
    else if (v[2].compare("UD2") == 0){
        if (steps != 0) vertical_2->steps(vertical_2, steps);
        else {vertical_2->deenergize(vertical_2);}
    }
    
    return "04done";
}

void run_exit(){
    cout << "Stopping the camera." << endl;
    Camera.release();

    horizontal_1->deenergize(horizontal_1);

    horizontal_2->deenergize(horizontal_2);

    vertical_1->deenergize(vertical_1);

    vertical_2->deenergize(vertical_2);
}



