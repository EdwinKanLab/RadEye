#include "../include/BlobDetector.hpp"

#include "../../Cropper/include/Cropper.hpp"

#include "../../CornerDetector/include/CornerDetector.hpp"

#include "../../Warper/include/Warper.hpp"

#include "../include/GroundTruthEvaluator.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
// #include <filesystem>
#include <string>
#include <chrono> 
#include <utility>

using namespace cv;
using namespace std;
// using path = std::filesystem::path;


int main(){

    Mat image1 = imread("../TestImages/testImage1.jpg", IMREAD_COLOR);
    Mat image2 = imread("../TestImages/testImage2.jpg", IMREAD_COLOR);
    Mat image3 = imread("../TestImages/testImage3.jpg", IMREAD_COLOR);
    Mat image4 = imread("../TestImages/testImage4.jpg", IMREAD_COLOR);
    Mat image5 = imread("../TestImages/testImage5.jpg", IMREAD_COLOR);
    if(!(image1.data && image2.data && image3.data && image4.data && image5.data))                              // Check for invalid input
    {
        cout <<  "Could not open or find one or more images" << std::endl ;
        return -1;
    }

    Mat images[5] = {image1, image2, image3, image4, image5};
    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    auto total = duration.count();


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
    blobDetector.setThresholdParams(200, 255, 10);
    blobDetector.setColorParams(true, 255);
    blobDetector.setAreaParams(false, 0, 2);
    blobDetector.setCircularityParams(false, 0, 1);
    blobDetector.setInertiaParams(true, 0.3, 1);
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
    for (int i = 0; i < 5; i++){
        groundTruthEvaluator.setInputImage(images[i]);
        groundTruthEvaluator.doTheJob();
        
        groundTruthEvaluator.saveInputImage("../Results/"+
        to_string(i+1)+"a_inputImage.jpg");
        
        groundTruthEvaluator.saveResultAsImage("../Results/"+
        to_string(i+1)+"b_groundTruthImage.jpg");
    }

    ///////////////////////////////////////////////////////////////////////////
    images[0].release();
    images[1].release();
    images[2].release();
    images[3].release();
    images[4].release();
    image1.release();
    image2.release();
    image3.release();
    image4.release();
    image5.release();


    ///////////////////////////////////////////////////////////////////////////
    Mat image;
    // image = imread("../100images/raspicam_cv_image_"+
    //     to_string(0)+".jpg", IMREAD_COLOR);
    
    // cout << "Read image 0" << endl;


    for (int i = 0; i < 5; i++){
        cout << "Reading image # " << i << endl;
        image = imread("../100images/raspicam_cv_image_"+
        to_string(i)+".jpg", IMREAD_COLOR);

        if(!(image.data)){
            cout <<  "Could not open or find one or more images" << endl ;
            return -1;
        }
        cout << "Finished reading" << endl;

        start = chrono::high_resolution_clock::now();

        groundTruthEvaluator.setInputImage(image);
        groundTruthEvaluator.doTheJob();

        stop = chrono::high_resolution_clock::now();
        cout << "Evaluated Ground Truth" << endl;
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        total += duration.count();
    }

    // cout << "Total time for 100 images: " << total << endl;
    cout << "Finished Test" << endl;
    cout << "Check Results directory for the image files" << endl;

}