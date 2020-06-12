#include "../include/Warper.hpp"

#include "../../Cropper/include/Cropper.hpp"

#include "../../CornerDetector/include/CornerDetector.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
// #include <filesystem>
#include <string>
#include <chrono> 

using namespace cv;
using namespace std;
// using path = std::filesystem::path;
vector<Point2f> orderFourPoints(const vector<Point2f> & inputVector){
    vector<Point2f> output(4,Point(0,0));

    pair<float, int> yCoord [4] = {{inputVector[0].y, 0},
                                   {inputVector[1].y, 1},
                                   {inputVector[2].y, 2},
                                   {inputVector[3].y, 3}
                                   };
    
    int yN = sizeof(yCoord)/sizeof(yCoord[0]); 
  
    sort(yCoord, yCoord+yN);

    if (inputVector[yCoord[0].second].x < inputVector[yCoord[1].second].x){
        output[0] = inputVector[yCoord[0].second];
        output[1] = inputVector[yCoord[1].second];
    }
    else{
        output[0] = inputVector[yCoord[1].second];
        output[1] = inputVector[yCoord[0].second];
    }

    if (inputVector[yCoord[2].second].x < inputVector[yCoord[3].second].x){
        output[3] = inputVector[yCoord[2].second];
        output[2] = inputVector[yCoord[3].second];
    }
    else{
        output[2] = inputVector[yCoord[2].second];
        output[3] = inputVector[yCoord[3].second];
    }
    return output;
}

Point2f getCorner(Cropper& region, CornerDetector& singleCorner){

    singleCorner.setInputImage(region.getResult());
    singleCorner.doTheJob();
    Point2f result = singleCorner.getResult()[0];
    Point2f origin;
    origin.x =  (float) (int) (region.getLeftRatio() * (region.getInputImage().cols));
    origin.y = (float) (int) (region.getTopRatio() * (region.getInputImage().rows));
    result += origin;

    
    return result;

}

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

    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    auto total = duration.count();


    ///////////////////////////////////////////////////////////////////////////
    Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
    Cropper topLeftRegion(0, 0.10, 0.0, 0.10);
    Cropper topRightRegion(0.05, 0.15, 0.85, 1);
    Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
    Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);

    ///////////////////////////////////////////////////////////////////////////
    CornerDetector singleCorner(1,0.1,100);
    // CornerDetector fourPlaneCorners(4, 0.1, 100);
    ///////////////////////////////////////////////////////////////////////////

    Warper warper;

    ///////////////////////////////////////////////////////////////////////////
    
    initialDiscard.setInputImage(image1);
    initialDiscard.doTheJob();

    
    // fourPlaneCorners.setInputImage(initialDiscard.getResult());
    // fourPlaneCorners.doTheJob();

    // warper.setAllSrcPoints(orderFourPoints(fourPlaneCorners.getResult()));
    // warper.setInputImage(initialDiscard.getResult());
    // warper.doTheJob();




    topLeftRegion.setInputImage(initialDiscard.getResult());
    topRightRegion.setInputImage(initialDiscard.getResult());
    bottomRightRegion.setInputImage(initialDiscard.getResult());
    bottomLeftRegion.setInputImage(initialDiscard.getResult());
    
    topLeftRegion.doTheJob();
    topRightRegion.doTheJob();
    bottomRightRegion.doTheJob();
    bottomLeftRegion.doTheJob();
    
    warper.setTopLeftSrcPoint(getCorner(topLeftRegion, singleCorner));
    warper.setTopRightSrcPoint(getCorner(topRightRegion, singleCorner));
    warper.setBottomRightSrcPoint(getCorner(bottomRightRegion, singleCorner));
    warper.setBottomLeftSrcPoint(getCorner(bottomLeftRegion, singleCorner));
    warper.setInputImage(initialDiscard.getResult());
    warper.doTheJob();



    initialDiscard.saveInputImage("../Results/1a_inputImage.jpg");
    initialDiscard.saveResultAsImage("../Results/1b_croppedImage.jpg");
    // fourPlaneCorners.saveResultAsImage("../Results/1c_cornerImage.jpg");

    warper.saveResultAsImage("../Results/1d_warpedImage.jpg");




    // fourPlaneCorners.saveResultAsImage("../Results/5b_cornerImage5b.jpg");

    
    // singleCorner.setInputImage(image1);

    // start = chrono::high_resolution_clock::now();
    
    // singleCorner.doTheJob();
    
    // stop = chrono::high_resolution_clock::now();
    // duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "Time taken to detect corner in testImage1.jpg: " << duration.count() << endl;
    // total += duration.count();

    // singleCorner.saveInputImage("../Results/1a_inputImage1.jpg");
    // singleCorner.saveResultAsImage("../Results/1b_cornerImage1.jpg");

    // ///////////////////////////////////////////////////////////////////////////

    // singleCorner.setInputImage(image2);
    
    // start = chrono::high_resolution_clock::now();
    
    // singleCorner.doTheJob();
    
    // stop = chrono::high_resolution_clock::now();
    // duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "Time taken to detect corner in testImage2.jpg: " << duration.count() << endl;
    // total += duration.count();

    // singleCorner.saveInputImage("../Results/2a_inputImage2.jpg");
    // singleCorner.saveResultAsImage("../Results/2b_cornerImage2.jpg");

    // ///////////////////////////////////////////////////////////////////////////

    // singleCorner.setInputImage(image3);
    
    // start = chrono::high_resolution_clock::now();
    
    // singleCorner.doTheJob();
    
    // stop = chrono::high_resolution_clock::now();
    // duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "Time taken to detect corner in testImage3.jpg: " << duration.count() << endl;
    // total += duration.count();

    // singleCorner.saveInputImage("../Results/3a_inputImage3.jpg");
    // singleCorner.saveResultAsImage("../Results/3b_cornerImage3.jpg");

    // ///////////////////////////////////////////////////////////////////////////

    // singleCorner.setInputImage(image4);
    
    // start = chrono::high_resolution_clock::now();
    
    // singleCorner.doTheJob();
    
    // stop = chrono::high_resolution_clock::now();
    // duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "Time taken to detect corner in testImage4.jpg: " << duration.count() << endl;
    // total += duration.count();
    // cout << endl;
    // cout << "Total time taken for the first 4 test images: " <<  total << endl;

    // singleCorner.saveInputImage("../Results/4a_inputImage4a.jpg");
    // singleCorner.saveResultAsImage("../Results/4b_cornerImage4b.jpg");

    // ///////////////////////////////////////////////////////////////////////////

    // fourPlaneCorners.setInputImage(image5);

    // start = chrono::high_resolution_clock::now();
    
    // fourPlaneCorners.doTheJob();
    
    // stop = chrono::high_resolution_clock::now();
    // duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "Time taken to detect corner in testImage5.jpg: " << duration.count() << endl;

    // fourPlaneCorners.saveInputImage("../Results/5a_inputImage5a.jpg");
    // fourPlaneCorners.saveResultAsImage("../Results/5b_cornerImage5b.jpg");

    ///////////////////////////////////////////////////////////////////////////
    cout << "Finished Test" << endl;
    cout << "Check Results directory for the image files" << endl;

}