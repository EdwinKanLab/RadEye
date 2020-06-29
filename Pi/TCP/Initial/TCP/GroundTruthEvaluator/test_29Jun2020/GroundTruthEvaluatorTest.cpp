#include "../../BlobDetector/include/BlobDetector.hpp"

#include "../../Cropper/include/Cropper.hpp"

#include "../../CornerDetector/include/CornerDetector.hpp"

#include "../../Warper/include/Warper.hpp"

#include "../include/GroundTruthEvaluator.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <chrono> 

using namespace cv;
using namespace std;


int main(){

    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    auto total = duration.count();


    ///////////////////////////////////////////////////////////////////////////
    float screenWidthMM = 267;
    float screenHeightMM = 180.5;

    ///////////////////////////////////////////////////////////////////////////
    Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
    Cropper topLeftRegion(0, 0.10, 0.0, 0.10);
    Cropper topRightRegion(0.05, 0.15, 0.85, 1);
    Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
    Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);

    ///////////////////////////////////////////////////////////////////////////
    
    CornerDetector singleCorner(1,0.1,100);
    
    ///////////////////////////////////////////////////////////////////////////

    Warper warper;

    ///////////////////////////////////////////////////////////////////////////

    BlobDetector blobDetector;
    blobDetector.setThresholdParams(200, 255, 10);
    blobDetector.setColorParams(true, 255);
    blobDetector.setAreaParams(false, 0, 2);
    blobDetector.setCircularityParams(false, 0, 1);
    blobDetector.setInertiaParams(true, 0.2, 1);
    blobDetector.setConvexityParams(false, 0.0, 1);
    blobDetector.setMinRepeatability(2);
    blobDetector.setMinDistBetweenBlobs(0);

    ///////////////////////////////////////////////////////////////////////////

    GroundTruthEvaluator groundTruthEvaluator;
    groundTruthEvaluator.setScreenDimMM(screenWidthMM, screenHeightMM);

    groundTruthEvaluator.setCroppers(initialDiscard, topLeftRegion,
                                     topRightRegion, bottomRightRegion,
                                     bottomLeftRegion);

    
    groundTruthEvaluator.setSingleCorner(singleCorner);
    groundTruthEvaluator.setWarper(warper);
    groundTruthEvaluator.setBlobDetector(blobDetector);

    ///////////////////////////////////////////////////////////////////////////

    cout << "Testing on 100 images" << endl;
    Mat image;
    ofstream myfile;
    myfile.open ("../coordinates.csv");
    
    for (int i = 0; i < 100; i++){
        // cout << "Reading image # " << i << endl;
        image = imread("../SavedImages/"+
        to_string(i)+"a_inputImage.jpg", IMREAD_COLOR);

        if(!(image.data)){
            cout <<  "Could not open or find one or more images" << endl ;
            return -1;
        }
        // cout << "Finished reading" << endl;

        start = chrono::high_resolution_clock::now();
        
        // cout << "Started timing" << endl;
        groundTruthEvaluator.setInputImage(image);
        // cout << "Set input image" << endl;
        groundTruthEvaluator.doTheJob();
        
        // cout << "Evaluated Ground Truth" << endl;
        stop = chrono::high_resolution_clock::now();
        groundTruthEvaluator.saveResultAsImage("../SavedImages/"+to_string(i)+
        "b_groundTruthImage.jpg");
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        total += duration.count();

        string result = "";
        result += to_string(groundTruthEvaluator.getResult()[0].x);
        result += ",";
        result += to_string(groundTruthEvaluator.getResult()[0].y);
        result += "\n";
        myfile << result;
        
    }
    myfile.close();
    cout << "Total time for 100 images: " << total << endl;
    cout << "Finished Test" << endl;
    cout << "Check SavedImages directory for the image files" << endl;

}
